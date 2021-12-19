#include <usb/linux/composite.h>
#include <usb/linux/gadget.h>
#include <usb/g_imi_usb.h>
#include <soc/imi_video.h>
#include <imi_dev.h>
#include <errno.h>
#include <linux_compat.h>
#include "u_imi.h"

#define IMI_MAX_VIDEO_NUM 2
#define IMI_BULK_BUFFER_SIZE	4096

static struct usb_interface_descriptor imi_interface_desc = {
	.bLength                = USB_DT_INTERFACE_SIZE,
	.bDescriptorType        = USB_DT_INTERFACE,
	.bInterfaceNumber       = 0,
	.bNumEndpoints          = IMI_MAX_VIDEO_NUM,
	.bInterfaceClass        = 0xFF,
	.bInterfaceSubClass     = 0xA6,
	.bInterfaceProtocol     = 0,
};

static struct usb_endpoint_descriptor color_ss_in_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_IN,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize         = cpu_to_le16(1024),
	.bInterval              = 0,
};

static struct usb_endpoint_descriptor upload_ss_out_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_OUT,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize         = cpu_to_le16(1024),
	.bInterval              = 0,
};

static struct usb_endpoint_descriptor color_hs_in_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_IN,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize         = cpu_to_le16(512),
	.bInterval              = 0,
};

static struct usb_endpoint_descriptor upload_hs_out_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_OUT,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize         = cpu_to_le16(512),
	.bInterval              = 0,
};

static struct usb_endpoint_descriptor color_fs_in_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_IN,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize         = cpu_to_le16(512),
	.bInterval              = 0,
};

static struct usb_endpoint_descriptor upload_fs_out_desc = {
	.bLength                = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType        = USB_DT_ENDPOINT,
	.bEndpointAddress       = USB_DIR_OUT,
	.bmAttributes           = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize         = cpu_to_le16(512),
	.bInterval              = 0,
};

static struct usb_ss_ep_comp_descriptor color_ss_comp_desc = {
	.bLength        = sizeof(color_ss_comp_desc),
	.bDescriptorType    = USB_DT_SS_ENDPOINT_COMP,
	.bMaxBurst          = 9, //value: 0 ~ 15
	.bmAttributes       = 0,
	.wBytesPerInterval  = 0,
};

static struct usb_descriptor_header *fs_imi_function[] = {
	(struct usb_descriptor_header *) &imi_interface_desc,
	(struct usb_descriptor_header *) &color_fs_in_desc,
	(struct usb_descriptor_header *) &upload_fs_out_desc,
	NULL,
};

static struct usb_descriptor_header *hs_imi_function[] = {
	(struct usb_descriptor_header *) &imi_interface_desc,
	(struct usb_descriptor_header *) &color_hs_in_desc,
	(struct usb_descriptor_header *) &upload_hs_out_desc,
	NULL,
};

static struct usb_descriptor_header *ss_imi_function[] = {
	(struct usb_descriptor_header *) &imi_interface_desc,
	(struct usb_descriptor_header *) &color_ss_in_desc,
	(struct usb_descriptor_header *) &color_ss_comp_desc,
	(struct usb_descriptor_header *) &upload_ss_out_desc,
	(struct usb_descriptor_header *) &color_ss_comp_desc,
	NULL,
};

/* string descriptors: */

static struct usb_string imi_string_defs[] = {
	[0].s = "imi device",
	{  } /* end of list */
};

static struct usb_gadget_strings imi_string_table = {
	.language =     0x0409, /* en-us */
	.strings =      imi_string_defs,
};

static struct usb_gadget_strings *imi_strings[] = {
	&imi_string_table,
	NULL,
};

/* temporary variable used between imi_open() and imi_gadget_bind() */
static struct imi_vendor_dev _imi_vendor_dev; //for drv

static struct imi_device hjimi_usb_dev; //for usr
static struct imi_device_handle hjimi_usb_handle = {
	.name = "imi_color",  //imi_open will use this name
	.dev = &hjimi_usb_dev,
};

static void imi_video_free_buffers(struct imi_video_dev *dev);

static inline struct imi_vendor_dev *func_to_imi(struct usb_function *f)
{
	return container_of(f, struct imi_vendor_dev, function);
}

static struct usb_request *imi_request_new(struct usb_ep *ep, int buffer_size)
{
	struct usb_request *req = usb_ep_alloc_request(ep, GFP_KERNEL);

	if (!req)
		return NULL;

	/* now allocate buffers for the requests */
	req->buf = pvPortMalloc(buffer_size);
	if (!req->buf) {
		usb_ep_free_request(ep, req);
		return NULL;
	}

	return req;
}

static void imi_request_free(struct usb_request *req, struct usb_ep *ep)
{
	if (req) {
		vPortFree(req->buf);
		usb_ep_free_request(ep, req);
	}
}

static void imi_complete_out(struct usb_ep *ep, struct usb_request *req)
{
	struct imi_vendor_dev *dev = &_imi_vendor_dev;

	dev->vdev.udev.rx_done = 1;
	if (req->status != 0 && req->status != -ECONNRESET)
		dev->vdev.udev.error = 1;

	imi_wakeup_one(&dev->vdev.udev.read_wq, IMI_WAKEUP_FROM_ISR);
}

static int imi_upload_read(struct imi_device_handle *handle, void *buf, unsigned int len)
{
	struct imi_video_dev *dev = handle->dev->priv;
	struct usb_request *req;
	int r = len, xfer;
	int ret;

	pr_debug("%s (%d)\n", __func__);

	if (len > IMI_BULK_BUFFER_SIZE)
		return -EINVAL;

	/* we will block until we're online */
	while (!(_imi_vendor_dev.online || dev->udev.error)) {
		pr_debug("%s: waiting for online state\n", __func__);
		ret = wait_event(dev->udev.read_wq,
				(_imi_vendor_dev.online || dev->udev.error));
		if (ret < 0) {
			return ret;
		}
	}
	if (dev->udev.error) {
		r = -EIO;
		goto done;
	}

requeue_req:
	/* queue a request */
	req = dev->udev.rx_req;

	/* buffer size is large enough to accommodate */
	if (len % dev->udev.bulk_out_maxpacket)
		len += (dev->udev.bulk_out_maxpacket -
			(len % dev->udev.bulk_out_maxpacket));
	req->length = len;
	dev->udev.rx_done = 0;
	ret = usb_ep_queue(dev->udev.ep_out, req, GFP_ATOMIC);
	if (ret < 0) {
		pr_debug("%s: failed req %p (%d)\n", __func__, req, ret);
		r = -EIO;
		dev->udev.error = 1;
		goto done;
	} else {
		pr_debug("rx %p queue\n", req);
	}

	/* wait for a request to complete */
	ret = wait_event(dev->udev.read_wq, dev->udev.rx_done);
	if (ret < 0) {
		if (ret != -EIO)
			dev->udev.error = 1;
		r = ret;
		usb_ep_dequeue(dev->udev.ep_out, req);
		goto done;
	}
	if (!dev->udev.error) {
		/* If we got a 0-len packet, throw it back and try again. */
		if (req->actual == 0)
			goto requeue_req;

		pr_debug("rx %p %d\n", req, req->actual);
		xfer = (req->actual < len) ? req->actual : len;
		memcpy(buf, req->buf, xfer);
	} else
		r = -EIO;

done:
	pr_debug("%s returning %d\n", __func__, r);
	return r < 0 ? r : xfer;
}

static void imi_complete_in(struct usb_ep *ep, struct usb_request *req)
{
	struct imi_vendor_dev *dev = &_imi_vendor_dev;

	FRTOS_complete(dev->vdev.udev.tx_complete);
}

static int imi_video_write(struct imi_device_handle *handle, void *buf, unsigned int count)
{
	struct imi_video_dev *dev = handle->dev->priv;
	struct usb_request *req = dev->udev.tx_req;
	int ret, len;

	len = count > IMI_BULK_BUFFER_SIZE ? IMI_BULK_BUFFER_SIZE : count;
	memcpy(req->buf, buf, len);
	req->length = len;

	ret = usb_ep_queue(dev->ep, req, GFP_ATOMIC);
	if (unlikely(ret < 0))
		return ret;

	ret = FRTOS_completion_wait(dev->udev.tx_complete, 0);
	if (unlikely(ret)) {
		usb_ep_dequeue(dev->ep, req);
		return -EINTR;
	}

	return req->status ? req->status : req->actual;
}

static int imi_video_open(struct imi_device_handle *handle)
{

	handle->dev->priv = &_imi_vendor_dev.vdev;

	/* clear the error latch */
	_imi_vendor_dev.vdev.udev.error = 0;

	pr_debug("%s end\n", __func__);

	return 0;
}

static int imi_video_close(struct imi_device_handle *handle)
{
	struct imi_video_dev *dev = handle->dev->priv;
	int ret = 0;

	ret = imi_video_enable(dev, 0);

	imi_video_free_buffers(dev);

	return ret;
}

static unsigned int imi_video_poll(struct imi_device_handle *handle, struct imi_wait_node *node)
{
	int ret, mask = 0;
	unsigned long flags;
	struct imi_video_dev *dev = handle->dev->priv;
	struct list_head *head = &dev->video_queue.done_entry;

	imi_add_to_wait(&dev->wait, node);
	spin_lock_irqsave(&dev->video_queue.queue_lock, flags);
	ret = list_empty(head);
	spin_unlock_irqrestore(&dev->video_queue.queue_lock, flags);
	if (!ret)
		mask |= POLLOUT | POLLWRNORM;

	return mask;
}

static int imi_video_alloc_buffers(struct imi_video_dev *dev,
		struct imi_video_req_buf *req)
{
	struct imi_video_buffer *buf;
	struct imi_video_sub_dev *sub_dev = &dev->sub_dev[req->dev_index];
	int index;

	if (req->dev_index >= IMI_STREAM_MAX) {
		pr_debug("%s, dev_index param err\n", __func__);
		return -EINVAL;
	}

	if (sub_dev->vbuf == NULL) {
		sub_dev->vbuf = pvPortMalloc(req->num * sizeof(*buf));
		if (!sub_dev->vbuf) {
			pr_debug("alloc mem fail\n");
			return -ENOMEM;
		}
		sub_dev->vbuf_num = req->num;
	}

	sub_dev->buf_type = req->type;

	for (index = 0; index < sub_dev->vbuf_num; index++) {
		buf = &sub_dev->vbuf[index];
		memset((void *)buf, 0x00, sizeof(*buf));
		buf->index = index;
		buf->state = IMI_BUF_STATE_IDLE;
		buf->dev = sub_dev;
	}

	return 0;
}

static void imi_video_free_buffers(struct imi_video_dev *dev)
{
	struct imi_video_sub_dev *subdev;
	int i;

	for (i = 0; i < IMI_STREAM_MAX; i++) {
		subdev = &dev->sub_dev[i];
		if (subdev->vbuf) {
			vPortFree(subdev->vbuf);
			subdev->vbuf = 0;
		}
	}
	memset((void *)(&dev->sub_dev), 0x00,
			IMI_STREAM_MAX * sizeof(struct imi_video_sub_dev));
}

static inline void imi_video_return_buf(struct imi_video_dev *dev,
		struct imi_video_buf_info *info)
{
	unsigned long flags;
	struct imi_video_sub_dev *sub_dev = &dev->sub_dev[info->dev_index];
	struct imi_video_buffer *buf = &sub_dev->vbuf[info->index];

	spin_lock_irqsave(&dev->video_queue.queue_lock, flags);
	list_add_tail(&buf->list, &dev->video_queue.done_entry);
	spin_unlock_irqrestore(&dev->video_queue.queue_lock, flags);
}

static int imi_video_queue_buffer(struct imi_video_dev *dev,
		struct imi_video_buf_info *info)
{
	unsigned long flags;
	struct imi_video_sub_dev *sub_dev;
	struct imi_video_buffer *buf;

	if (info->dev_index >= IMI_STREAM_MAX) {
		pr_debug("%s, sub_dev param err\n", __func__);
		return -EINVAL;
	}

	sub_dev = &dev->sub_dev[info->dev_index];
	if (!sub_dev->stream_on) {
		pr_debug("stream off\n");
		return -EPERM;
	}

	if (info->index >= sub_dev->vbuf_num) {
		pr_debug("buf index = %x err\n", info->index);
		return -EINVAL;
	}

	buf = &sub_dev->vbuf[info->index];
	buf->dev = sub_dev;

	buf->frame_len = info->frame_len;
	buf->transferred = 0;
	buf->mem = (void *)(info->phy_addr);
	if (!buf->mem) {
		pr_debug("imi_video_get_kern_addr fail\n");
		return -EFAULT;
	}

	spin_lock_irqsave(&dev->video_queue.queue_lock, flags);
	list_add_tail(&buf->list, &dev->video_queue.queued_entry);
	spin_unlock_irqrestore(&dev->video_queue.queue_lock, flags);

	return 0;
}

static int imi_video_dequeue_buffer(struct imi_video_dev *dev,
		struct imi_video_buf_info *info)
{
	unsigned long flags;
	struct imi_video_buffer *buf;
	struct list_head *head = &dev->video_queue.done_entry;

	spin_lock_irqsave(&dev->video_queue.queue_lock, flags);
	if (list_empty(head)) {
		spin_unlock_irqrestore(&dev->video_queue.queue_lock, flags);
		return -ENOBUFS;
	}

	buf = list_first_entry(head, struct imi_video_buffer, list);
	list_del(&buf->list);
	spin_unlock_irqrestore(&dev->video_queue.queue_lock, flags);

	info->index = buf->index;
	info->dev_index = buf->dev->type;

	return 0;
}

static void
imi_video_sub_dev_stream_off(struct imi_video_dev *video, unsigned char index)
{
	struct imi_video_buffer *buf;
	struct imi_video_buffer *tmp;
	struct list_head *head;
	unsigned long flags;

	video->sub_dev[index].stream_on = 0;

	spin_lock_irqsave(&video->video_queue.queue_lock, flags);

	head = &video->video_queue.queued_entry;
	list_for_each_entry_safe(buf, tmp, head, list) {
		if (buf->dev->type == index)
			list_del(&buf->list);
	}

	head = &video->video_queue.started_entry;
	list_for_each_entry_safe(buf, tmp, head, list) {
		if (buf->dev->type == index)
			list_del(&buf->list);
	}

	head = &video->video_queue.done_entry;
	list_for_each_entry_safe(buf, tmp, head, list) {
		if (buf->dev->type == index)
			list_del(&buf->list);
	}

	spin_unlock_irqrestore(&video->video_queue.queue_lock, flags);
}

static int imi_video_ioctl(struct imi_device_handle *handle, unsigned int cmd, void *cmd_buf)
{
	struct imi_video_dev *dev = handle->dev->priv;
	struct imi_video_sub_dev_ctl *ctl = cmd_buf;
	int ret = 0;

	switch (cmd) {
	case VIDIOC_IMI_REQ_BUF:
		ret = imi_video_alloc_buffers(dev, cmd_buf);
		break;

	case VIDIOC_IMI_QBUF:
		ret = imi_video_queue_buffer(dev, cmd_buf);
		if (ret)
			return ret;

		ret = imi_video_pump(dev);
		break;

	case VIDIOC_IMI_DQBUF:
		ret = imi_video_dequeue_buffer(dev, cmd_buf);
		break;

	case VIDIOC_IMI_STREAMON:
		ret = imi_video_enable(dev, 1);
		break;

	case VIDIOC_IMI_STREAMOFF:
		ret = imi_video_enable(dev, 0);
		break;

	case VIDIOC_IMI_SUB_DEV_CTL:
		if (!ctl->cmd) {
			imi_video_sub_dev_stream_off(dev, ctl->dev_index);
		} else {
			ret = imi_video_sub_dev_stream_on(ctl->dev_index);
		}
		break;

	default:
		break;
	}

	return ret;
}

/* file operations for IMI device /dev/imi_* */
static struct imi_device_ops imi_video_fops = {
	.read = imi_upload_read,
	.write = imi_video_write,
	.open = imi_video_open,
	.close = imi_video_close,
	.poll = imi_video_poll,
	.ioctl = imi_video_ioctl,
};

static int imi_create_video_endpoints(struct imi_vendor_dev *dev)
{
	struct usb_composite_dev *cdev = dev->cdev;
	struct usb_ep *ep;
	struct usb_request *req;

	if (gadget_is_superspeed(cdev->gadget))
		ep = usb_ep_autoconfig_ss(cdev->gadget, &color_ss_in_desc,
				&color_ss_comp_desc);
	else if (gadget_is_dualspeed(cdev->gadget))
		ep = usb_ep_autoconfig(cdev->gadget, &color_hs_in_desc);
	else
		ep = usb_ep_autoconfig(cdev->gadget, &color_fs_in_desc);

	if (!ep) {
		DBG(cdev, "usb_ep_autoconfig for color in failed\n");
		return -ENODEV;
	}
	DBG(cdev, "usb_ep_autoconfig for ep got %s\n", ep->name);
	ep->driver_data = &dev->vdev;
	dev->vdev.ep = ep;

	color_ss_in_desc.bEndpointAddress = ep->address;
	color_hs_in_desc.bEndpointAddress = ep->address;
	color_fs_in_desc.bEndpointAddress = ep->address;

	/* for out endpoint */
	if (gadget_is_superspeed(cdev->gadget))
		ep = usb_ep_autoconfig_ss(cdev->gadget, &upload_ss_out_desc,
				&color_ss_comp_desc);
	else if (gadget_is_dualspeed(cdev->gadget))
		ep = usb_ep_autoconfig(cdev->gadget, &upload_hs_out_desc);
	else
		ep = usb_ep_autoconfig(cdev->gadget, &upload_fs_out_desc);

	if (!ep) {
		DBG(cdev, "usb_ep_autoconfig for upload bulkout infailed\n");
		return -ENODEV;
	}
	DBG(cdev, "usb_ep_autoconfig for ep_out got %s\n", ep->name);
	ep->driver_data = &dev->vdev;
	dev->vdev.udev.ep_out = ep;

	upload_ss_out_desc.bEndpointAddress = ep->address;
	upload_hs_out_desc.bEndpointAddress = ep->address;
	upload_fs_out_desc.bEndpointAddress = ep->address;

	/* now allocate requests for out endpoint */
	req = imi_request_new(dev->vdev.udev.ep_out, IMI_BULK_BUFFER_SIZE);
	if (!req)
		goto fail;
	req->complete = imi_complete_out;
	dev->vdev.udev.rx_req = req;

	/* now allocate requests for in endpoint */
	req = imi_request_new(dev->vdev.ep, IMI_BULK_BUFFER_SIZE);
	if (!req)
		goto fail1;
	req->complete = imi_complete_in;
	dev->vdev.udev.tx_req = req;

	return 0;

fail1:
	imi_request_free(dev->vdev.udev.rx_req, dev->vdev.udev.ep_out);
	dev->vdev.udev.rx_req = NULL;
fail:
	pr_debug("imi_bind() could not allocate requests\n");
	return -1;
}

static int imi_function_bind(struct usb_configuration *c,
		struct usb_function *f)
{
	struct usb_composite_dev *cdev = c->cdev;
	struct imi_vendor_dev      *imi = func_to_imi(f);
	int         status;

	imi->cdev = cdev;
	status = -ENODEV;

	if (imi_string_defs[0].id == 0) {
		status = usb_string_id(c->cdev);
		if (status < 0)
			return status;
		imi_string_defs[0].id = status;
	}

	status = usb_interface_id(c, f);
	if (status < 0)
		return status;
	imi_interface_desc.bInterfaceNumber = status;

	status = imi_create_video_endpoints(imi);
	if (status)
		return status;

	status = usb_assign_descriptors(f, fs_imi_function, hs_imi_function,
			ss_imi_function, NULL);
	if (status)
		goto fail;

	status = imi_cmd_dev_init(cdev);
	if (status)
		goto fail;

	hjimi_usb_handle.ops = &imi_video_fops;
	imi_register_dev(&hjimi_usb_handle);

	return 0;

fail:
	ERROR(cdev, "%s: can't bind, err %d\n", f->name, status);
	return status;
}

static void imi_function_unbind(struct usb_configuration *c,
		struct usb_function *f)
{
	struct imi_vendor_dev  *dev = func_to_imi(f);

	dev->online = 0;
	dev->vdev.udev.error = 1;
	imi_wakeup_all(&dev->vdev.udev.read_wq, IMI_WAKEUP_NORMAL);

	imi_deregister_dev(&hjimi_usb_handle);

	imi_request_free(dev->vdev.udev.rx_req, dev->vdev.udev.ep_out);
	imi_request_free(dev->vdev.udev.tx_req, dev->vdev.ep);

	imi_cmd_dev_deinit();
}

static int imi_function_set_alt(struct usb_function *f,
		unsigned int intf, unsigned int alt)
{
	struct imi_vendor_dev  *dev = func_to_imi(f);
	struct usb_composite_dev *cdev = f->config->cdev;
	struct usb_ep *ep;
	int ret;

	DBG(cdev, "%s intf: %d alt: %d\n", __func__, intf, alt);
	ep = dev->vdev.ep;
	usb_ep_disable(ep);

	ret = config_ep_by_speed(cdev->gadget, f, ep);
	if (ret)
		return ret;

	ret = usb_ep_enable(ep);
	if (ret)
		return ret;

	usb_ep_disable(dev->vdev.udev.ep_out);
	ret = config_ep_by_speed(cdev->gadget, f, dev->vdev.udev.ep_out);
	if (ret)
		return ret;

	ret = usb_ep_enable(dev->vdev.udev.ep_out);
	if (ret) {
		usb_ep_disable(dev->vdev.ep);
		return ret;
	}

	dev->vdev.udev.bulk_out_maxpacket =
		usb_endpoint_maxp(dev->vdev.udev.ep_out->desc);

	dev->online = 1;
	/* readers may be blocked waiting for us to go online */
	imi_wakeup_all(&dev->vdev.udev.read_wq, IMI_WAKEUP_NORMAL);

	return 0;
}

static void imi_function_disable(struct usb_function *f)
{
	struct imi_vendor_dev  *dev = func_to_imi(f);

	dev->online = 0;
	dev->vdev.udev.error = 1;
	usb_ep_disable(dev->vdev.ep);
	usb_ep_disable(dev->vdev.udev.ep_out);
}

static int imi_init(struct imi_vendor_dev *dev)
{
	struct imi_video_dev *video;
	int ret;

	video = &dev->vdev;
	ret = imi_video_init(video);
	if (ret)
		return ret;

	imi_upload_init(video);

	return 0;
}

static int imi_add(struct usb_configuration *c)
{
	struct imi_vendor_dev *p_imi = &_imi_vendor_dev;
	int status;

	memset((void *)p_imi, 0, sizeof(*p_imi));

	if (imi_init(p_imi)) {
		return -ENODEV;
	}

	p_imi->function.name = "f_imi";
	p_imi->function.bind = imi_function_bind;
	p_imi->function.unbind = imi_function_unbind;
	p_imi->function.set_alt = imi_function_set_alt;
	p_imi->function.disable = imi_function_disable;
	p_imi->function.setup =  imi_cmd_dev_setup;
	p_imi->function.req_match = imi_cmd_dev_req_match;
	p_imi->function.strings = imi_strings;

	status = usb_add_function(c, &p_imi->function);

	return status;
}

struct g_usb_bind_callback imi_1680_usb = {
	.usb_function_name = "imi_usb",
	.fptr = imi_add,
};

