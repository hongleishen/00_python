#include <usb/linux/composite.h>
#include <usb/linux/gadget.h>
#include <imi_dev.h>
#include <errno.h>
#include <linux_compat.h>
#include <soc/imi_video.h>

struct imi_cmd_dev {
	struct imi_wait_head read_wq;
	int read_done;

	int event_setup_out;
	struct usb_request *control_req;
	void *control_buf;
	int cmd_len;
	struct usb_ctrlrequest ctrl;

	enum vendor_cmd_stage  stage;

	struct usb_composite_dev *cdev;
	int exit;
};

#define EP0_MAX_PACKET_SIZE 60

static struct imi_cmd_dev cmd_dev; //for driv

static struct imi_device hjimi_usb_cmd_dev; //for usr
static struct imi_device_handle hjimi_usb_cmd_handle = {
	.name = "imi_usb_cmd",  //imi_open will use this name
	.dev = &hjimi_usb_cmd_dev,
};

static int cmd_dev_open(struct imi_device_handle *handle)
{
	handle->dev->priv = &cmd_dev;

	return 0;
}

static int cmd_dev_close(struct imi_device_handle *handle)
{
	//struct imi_cmd_dev *dev = handle->dev->priv;

	return 0;
}

static int cmd_dev_read(struct imi_device_handle *handle, void *buf, unsigned int count)
{
	struct imi_cmd_dev *dev = handle->dev->priv;
	void *ptr = NULL;
	int xfer;

	if (dev->exit)
		return -EFAULT;

	xfer = (dev->cmd_len < count) ? dev->cmd_len : count;
	if (dev->stage == VENDOR_CMD_STAGE_DATA)
		ptr = dev->control_buf;
	else
		ptr = &dev->ctrl;

	memcpy(buf, ptr, xfer);

	//imi_printf("stage: %d %s: len %d\n", dev->stage, __func__, xfer);

	return xfer;
}

static void cmd_dev_get_vendor_cmd_cxt(struct imi_cmd_dev *dev)
{
	struct usb_request *req = dev->control_req;
	int ret;

	req->length = le16_to_cpu(dev->ctrl.wLength);
	req->zero = 0;

	ret = usb_ep_queue(dev->cdev->gadget->ep0, req, GFP_ATOMIC);
	if (ret < 0) {
		imi_printf("%s: xfer error %d\n", __func__, ret);
	}
}

static int cmd_dev_write(struct imi_device_handle *handle, void *buf, unsigned int count)
{
	struct imi_cmd_dev *dev = handle->dev->priv;
	struct usb_request *req = dev->control_req;
	struct uvc_request_data data;
	int ret;

	if (dev->exit)
		return -EFAULT;

	memcpy(&data, buf, count);

	//imi_printf("%s: len %d\n", __func__, data.length);

	if (data.length < 0) {
		usb_ep_set_halt(dev->cdev->gadget->ep0);
		return count;
	}

	req->length = min_t(unsigned int, le16_to_cpu(dev->ctrl.wLength), data.length);
	req->zero = 0;

	memcpy(req->buf, data.data, req->length);
	ret = usb_ep_queue(dev->cdev->gadget->ep0, req, GFP_ATOMIC);
	if (ret < 0) {
		imi_printf("%s: xfer error %d\n", __func__, ret);
		ret = -EIO;
		return ret;
	}

	return count;
}

static int cmd_dev_ioctl(struct imi_device_handle *handle, unsigned int cmd, void *cmd_buf)
{
	struct imi_cmd_dev *dev = handle->dev->priv;
	int ret;

	switch (cmd) {
	case VIDIOC_GET_DATA_STAGE:
		ret = wait_event(dev->read_wq, (dev->read_done || dev->exit));
		if (ret < 0) {
			imi_printf("get data  ret < 0\n");
			return -EFAULT;
		}

		if (dev->exit)
			return 0;

		dev->read_done = 0;

		memcpy(cmd_buf, &dev->stage, sizeof(enum vendor_cmd_stage));

		break;

	case VIDIOC_READ_CMD_EXIT:
		dev->exit = 1;
		imi_wakeup_one(&(dev->read_wq), IMI_WAKEUP_NORMAL);
		break;

	case VIDIOC_READ_CMD_INIT:
		dev->exit = 0;
		break;

	default:
		break;
	}

	return 0;
}

static void cmd_dev_ep0_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct imi_cmd_dev *dev = req->context;

	if (dev->event_setup_out) {
		dev->event_setup_out = 0;
		dev->read_done = 1;
		dev->cmd_len = req->actual;
		dev->stage = VENDOR_CMD_STAGE_DATA;
		imi_wakeup_all(&(dev->read_wq), IMI_WAKEUP_FROM_ISR);
	}
//	imi_printf("%s out:%d, act %d\n", __func__,dev->event_setup_out, req->actual);
}


/* file operations for IMI device /dev/imi_* */
static struct imi_device_ops cmd_dev_fops = {
	.read = cmd_dev_read,
	.write = cmd_dev_write,
	.open = cmd_dev_open,
	.close = cmd_dev_close,
	.ioctl = cmd_dev_ioctl,
};

int imi_cmd_dev_setup(struct usb_function *f, const struct usb_ctrlrequest *ctrl)
{
	struct imi_cmd_dev *p_cmd_dev = &cmd_dev;
#if 0
	imi_printf("cmd setup request %02x %02x value %04x index %04x %04x\n",
	   ctrl->bRequestType, ctrl->bRequest, le16_to_cpu(ctrl->wValue),
	   le16_to_cpu(ctrl->wIndex), le16_to_cpu(ctrl->wLength));
#endif
	if ((ctrl->bRequestType & USB_TYPE_MASK) != USB_TYPE_VENDOR) {
		INFO(f->config->cdev, "invalid request type -----\n");
		return -EINVAL;
	}

	if (le16_to_cpu(ctrl->wLength) > EP0_MAX_PACKET_SIZE) {
		return -EINVAL;
	}

	p_cmd_dev->event_setup_out = !(ctrl->bRequestType & USB_DIR_IN);
	p_cmd_dev->cmd_len = sizeof(*ctrl);

	memcpy(&p_cmd_dev->ctrl, ctrl, sizeof(*ctrl));

	if (!p_cmd_dev->event_setup_out) { //in, get cmd ret from another tsk.
		p_cmd_dev->read_done = 1;
		p_cmd_dev->stage = VENDOR_CMD_STAGE_SETUP;
		imi_wakeup_all(&(p_cmd_dev->read_wq), IMI_WAKEUP_FROM_ISR);
	} else { //out
		cmd_dev_get_vendor_cmd_cxt(p_cmd_dev);
	}

	return 0;
}

bool imi_cmd_dev_req_match(struct usb_function *f, const struct usb_ctrlrequest *ctrl, bool cfg0)
{
	if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_VENDOR)
		return true;
	else
		return false;
}

int imi_cmd_dev_init(struct usb_composite_dev *cdev)
{
	struct imi_cmd_dev *p_cmd_dev = &cmd_dev;

	memset((void*)p_cmd_dev, 0x00, sizeof(struct imi_cmd_dev));

	p_cmd_dev->cdev = cdev;
	p_cmd_dev->control_req = usb_ep_alloc_request(cdev->gadget->ep0, GFP_KERNEL);
	p_cmd_dev->control_buf = pvPortMalloc(EP0_MAX_PACKET_SIZE);
	if (p_cmd_dev->control_req == NULL || p_cmd_dev->control_buf == NULL) {
		return -ENOMEM;
	}

	p_cmd_dev->control_req->buf = p_cmd_dev->control_buf;
	p_cmd_dev->control_req->complete = cmd_dev_ep0_complete;
	p_cmd_dev->control_req->context = p_cmd_dev;

	INIT_LIST_HEAD(&(p_cmd_dev->read_wq.wait_head));

	hjimi_usb_cmd_handle.ops = &cmd_dev_fops;

	imi_register_dev(&hjimi_usb_cmd_handle);

	return 0;
}

void imi_cmd_dev_deinit(void)
{
	struct imi_cmd_dev *dev = &cmd_dev;

	if (dev->control_req)
		usb_ep_free_request(dev->cdev->gadget->ep0, dev->control_req);

	vPortFree(dev->control_buf);

	imi_deregister_dev(&hjimi_usb_cmd_handle);
}


