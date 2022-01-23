#include <usb/linux/composite.h>
#include <usb/linux/gadget.h>
#include <soc/imi_video.h>
#include <errno.h>
#include <linux_compat.h>
#include "u_imi.h"

#define IMI_VIDEO_QUEUED_BUF 0
#define IMI_VIDEO_STARTED_BUF 1

unsigned long long g_frame_id[IMI_STREAM_MAX];
static unsigned int ep_max_payload;

struct imi_video_dev *g_imi_video;

static inline unsigned long long
imi_video_get_tm_stamp(struct imi_video_buffer *buf)
{
	unsigned char *val;

	if (!buf->mem)
		return 0;

	val = ((unsigned char *)buf->mem) + buf->frame_len;

	return *((unsigned long long *)val);
}

static int
imi_video_encode_header(struct imi_video_buffer *buf,
		unsigned char *data, int stream_type)
{
	struct imi_packet_header *header = (struct imi_packet_header *)data;
	int header_size = sizeof(struct imi_packet_header);

	unsigned long long tmp = imi_video_get_tm_stamp(buf);

	header->magic = IMI_PACKET_MAGIC;

	if (stream_type == IMI_STREAM_AUDIO_LEFT_USB)
		header->type = cpu_to_le16(0xD0);
	else if (stream_type == IMI_STREAM_AUDIO_RIGHT_USB)
		header->type = cpu_to_le16(0xD1);
	else
		header->type = cpu_to_le16(stream_type);

	header->packet_id = cpu_to_le64(g_frame_id[stream_type]);
	header->offset = cpu_to_le32(header_size);
	header->buf_size = cpu_to_le32(buf->frame_len);
	header->time_stamp = cpu_to_le64(tmp);

	g_frame_id[stream_type]++;

	return header_size;
}

static int
imi_video_encode_data(struct imi_video_buffer *buf,
		unsigned char *data, int len)
{
	int ret;

	ret = buf->frame_len - buf->transferred;
	ret = ret > len ? len : ret;

	memcpy(data, (buf->mem + buf->transferred), ret);
	buf->transferred += ret;

	return ret;
}

static struct imi_video_buffer *
imi_video_get_buf(struct imi_video_dev *video)
{
	struct imi_video_buffer *buf;
	struct list_head *head = &video->video_queue.queued_entry;

	if (!list_empty(head)) {
		buf = list_first_entry(head, struct imi_video_buffer, list);
		return buf;
	} else
		return NULL;
}

static void
imi_video_put_buf(struct imi_video_dev *video, struct imi_video_buffer *buf)
{
	struct list_head *head = &video->video_queue.done_entry;

	list_add_tail(&buf->list, head);
	imi_wakeup_all(&(video->wait), IMI_WAKEUP_FROM_ISR);
}

static void
imi_video_cancel_buf(struct imi_video_dev *video, unsigned char type)
{
	struct imi_video_buffer *buf;
	struct list_head *head;

	if (type == IMI_VIDEO_QUEUED_BUF)
		head = &video->video_queue.queued_entry;
	else
		head = &video->video_queue.started_entry;

	while (!list_empty(head)) {
		buf = list_first_entry(head, struct imi_video_buffer, list);
		list_del(&buf->list);
		buf->state = IMI_BUF_STATE_ERROR;
		imi_video_put_buf(video, buf);
	}
}

static void
imi_video_cancel(struct imi_video_dev *video, int disconnect)
{
	unsigned long flags;

	spin_lock_irqsave(&video->video_queue.queue_lock, flags);

	imi_video_cancel_buf(video, IMI_VIDEO_QUEUED_BUF);

	imi_video_cancel_buf(video, IMI_VIDEO_STARTED_BUF);

	spin_unlock_irqrestore(&video->video_queue.queue_lock, flags);
}

static int
imi_video_encode_bulk(struct usb_request *req, struct imi_video_sub_dev *dev,
		struct imi_video_buffer *buf)
{
	void *mem = req->buf;
	int len = ep_max_payload;
	int ret;

	if (buf->transferred == 0) {
		ret = imi_video_encode_header(buf, mem, dev->type);
		mem += ret;
		len -= ret;
	}

	ret = imi_video_encode_data(buf, mem, len);
	len -= ret;

	req->length = ep_max_payload - len;

	if (buf->transferred == buf->frame_len) {
		req->zero = 1;
		list_del(&buf->list);
		buf->state = IMI_BUF_STATE_DONE;
		imi_video_put_buf(g_imi_video, buf);
	} else
		req->zero = 0;

	return 0;
}

static int
imi_video_encode_dma(struct usb_request *req, struct imi_video_sub_dev *dev,
		struct imi_video_buffer *buf)
{
	void *mem = buf->mem;
	int len = ep_max_payload;
	int ret;

	if (buf->transferred == 0) {
		req->dma = (dma_addr_t)((char *)mem - sizeof(struct imi_packet_header)); //adjust mem start address;
		ret = imi_video_encode_header(buf, (unsigned char *)req->dma, dev->type);
		len -= ret;
	} else {
		req->dma = (dma_addr_t)((char *)buf->mem + buf->transferred);
	}

	ret = buf->frame_len - buf->transferred;
	ret = ret > len ? len : ret;

	buf->transferred += ret;

	len -= ret;

	req->length = ep_max_payload - len;

	if (buf->transferred == buf->frame_len) {
		req->zero = 1;
		list_del(&buf->list);
		buf->state = IMI_BUF_STATE_DONE;
		imi_video_put_buf(g_imi_video, buf);
	} else
		req->zero = 0;

	return 0;
}

static void
imi_video_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct imi_video_dev *video = g_imi_video;
	struct imi_video_queue *queue = &video->video_queue;
	struct imi_video_buffer *buf = req->context;
	unsigned long flags;
	int ret;

	(void)queue;

	switch (req->status) {
	case 0:
		break;

	case -ESHUTDOWN:    /* disconnect from host. */
		pr_debug("video request cancelled.\n");
		imi_video_cancel(video, 1);
		goto requeue;

	default:
		pr_debug("video completed with %d.\n", req->status);
		imi_video_cancel(video, 0);
		goto requeue;
	}

	spin_lock_irqsave(&queue->queue_lock, flags);

	buf = imi_video_get_buf(video);
	if (buf == NULL) {
		spin_unlock_irqrestore(&queue->queue_lock, flags);
		goto requeue;
	}

	req->context = buf;
	ret = buf->dev->encode(req, buf->dev, buf);
	if (ret) {
		spin_unlock_irqrestore(&queue->queue_lock, flags);
		goto requeue;
	}

	ret = usb_ep_queue(ep, req, GFP_ATOMIC);
	if (ret < 0) {
		pr_debug("Failed to queue request (%d).\n", ret);
		usb_ep_set_halt(ep);
		spin_unlock_irqrestore(&queue->queue_lock, flags);
		imi_video_cancel(video, 0);
		goto requeue;
	}
	spin_unlock_irqrestore(&queue->queue_lock, flags);

	return;

requeue:
	spin_lock_irqsave(&video->video_req.req_lock, flags);
	list_move_tail(&req->list, &video->video_req.req_free);
	spin_unlock_irqrestore(&video->video_req.req_lock, flags);
}

static int
imi_video_free_requests(struct imi_video_dev *video)
{
	unsigned int i;

	for (i = 0; i < VIDEO_REQ_NUM; ++i) {
		if (video->video_req.req[i]) {
			usb_ep_free_request(video->ep, video->video_req.req[i]);
			video->video_req.req[i] = NULL;
		}
	}

	INIT_LIST_HEAD(&video->video_req.req_free);
	INIT_LIST_HEAD(&video->video_req.req_busy);

	return 0;
}

static int
imi_video_alloc_requests(struct imi_video_dev *video)
{
	unsigned int i;
	struct usb_request *req;
	int ret = -ENOMEM;

	for (i = 0; i < VIDEO_REQ_NUM; ++i) {
		req = usb_ep_alloc_request(video->ep, GFP_KERNEL);
		if (req == NULL)
			goto error;

		video->video_req.req[i] = req;
#ifndef CONFIG_USB_INVENTRA_DMA
		req->buf = video->video_req.req_buffer[i];
#endif
		req->length = 0;
		req->complete = imi_video_complete;
		req->context = video;

		list_add_tail(&req->list, &video->video_req.req_free);
	}

	ep_max_payload = IMI_BULK_PAYLOAD_LEN;

	return 0;

error:
	imi_video_free_requests(video);
	return ret;
}

int imi_video_pump(struct imi_video_dev *video)
{
	struct imi_video_queue *queue = &video->video_queue;
	struct usb_request *req;
	struct imi_video_buffer *buf;
	struct imi_video_req *video_req = &video->video_req;
	unsigned long flags;
	int ret;

	(void)queue;

	while (1) {
		spin_lock_irqsave(&video_req->req_lock, flags);
		if (list_empty(&video_req->req_free)) {
			spin_unlock_irqrestore(&video_req->req_lock, flags);
			return 0;
		}
		req = list_first_entry(&video_req->req_free, struct usb_request,
					list);
		list_move_tail(&req->list, &video->video_req.req_busy);
		spin_unlock_irqrestore(&video_req->req_lock, flags);

		spin_lock_irqsave(&queue->queue_lock, flags);
		buf = imi_video_get_buf(video);
		if (buf == NULL) {
			spin_unlock_irqrestore(&queue->queue_lock, flags);
			break;
		}

		req->context = buf;
		ret = buf->dev->encode(req, buf->dev, buf);
		if (ret) {
			spin_unlock_irqrestore(&queue->queue_lock, flags);
			break;
		}

		ret = usb_ep_queue(video->ep, req, GFP_ATOMIC);
		if (ret < 0) {
			pr_debug("Failed to queue request (%d)\n", ret);
			usb_ep_set_halt(video->ep);
			spin_unlock_irqrestore(&queue->queue_lock, flags);
			imi_video_cancel(video, 0);
			break;
		}
		spin_unlock_irqrestore(&queue->queue_lock, flags);
	}

	spin_lock_irqsave(&video_req->req_lock, flags);
	list_move_tail(&req->list, &video->video_req.req_free);
	spin_unlock_irqrestore(&video_req->req_lock, flags);
	return 0;
}

static int imi_queue_enable(struct imi_video_queue *queue, int enable)
{
	unsigned long flags;

	if (!enable) {
		spin_lock_irqsave(&queue->queue_lock, flags);
		INIT_LIST_HEAD(&queue->queued_entry);
		INIT_LIST_HEAD(&queue->started_entry);
		INIT_LIST_HEAD(&queue->done_entry);
		spin_unlock_irqrestore(&queue->queue_lock, flags);
	}

	return 0;
}

int imi_video_sub_dev_stream_on(enum imi_stream_type type)
{
	struct imi_video_sub_dev *dev;

	if (type < IMI_STREAM_COLOR_USB || type >= IMI_STREAM_MAX)
		return -EINVAL;

	dev = &g_imi_video->sub_dev[type];

	dev->type = type;

	g_frame_id[type] = 0;

	if (dev->buf_type == IMI_BUF_TYPE_DMA_SINGLE)
		dev->encode = imi_video_encode_dma;
	else
		dev->encode = imi_video_encode_bulk;

	dev->stream_on = 1;

	return 0;
}

/*
 * Enable or disable the video stream.
 */
int imi_video_enable(struct imi_video_dev *video, int enable)
{
	struct imi_video_queue *queue = &video->video_queue;
	struct usb_request *req;
	struct usb_request *tmp;
	int ret;
	unsigned long flags;
	if (video->ep == NULL) {
		pr_debug("%s failed\n", __func__);
		return -ENODEV;
	}

	if (!enable && video->connect) {
		spin_lock_irqsave(&video->video_req.req_lock, flags);
		if (!list_empty(&video->video_req.req_busy)) {
			list_for_each_entry_safe(req, tmp, &video->video_req.req_busy,
				list) {
					usb_ep_dequeue(video->ep, req);
					list_move_tail(&req->list, &video->video_req.req_free);
			}
		}
		spin_unlock_irqrestore(&video->video_req.req_lock, flags);

		imi_video_free_requests(video);
		imi_queue_enable(queue, 0);
		return 0;
	}

	ret = imi_queue_enable(queue, 1);
	if (ret)
		return ret;

	ret = imi_video_alloc_requests(video);
	if (ret)
		return ret;

	video->connect = 1;

	return imi_video_pump(video);
}

int imi_alloc_transfer_buf(struct imi_video_dev *video)
{
	int i;
	void *ptr = NULL;

	for (i = 0; i < VIDEO_REQ_NUM; ++i) {
		ptr = pvPortMalloc(IMI_BULK_PAYLOAD_LEN);
		if (ptr == NULL) {
			imi_printf("can not malloc trans buf\n");
			return -ENOMEM;
		}
		video->video_req.req_buffer[i] = ptr;
	}

	return 0;
}

/*
 * Initialize the imi video stream.
 */
int imi_video_init(struct imi_video_dev *video)
{
	int ret;

	INIT_LIST_HEAD(&video->video_req.req_free);
	INIT_LIST_HEAD(&video->video_req.req_busy);
	spin_lock_init(&video->video_req.req_lock);

	INIT_LIST_HEAD(&video->video_queue.queued_entry);
	INIT_LIST_HEAD(&video->video_queue.started_entry);
	INIT_LIST_HEAD(&video->video_queue.done_entry);
	spin_lock_init(&video->video_queue.queue_lock);

	INIT_LIST_HEAD(&(video->wait.wait_head));

	memset((void *)(&video->sub_dev), 0x00, IMI_STREAM_MAX * sizeof(struct imi_video_sub_dev));

#ifndef CONFIG_USB_INVENTRA_DMA
	ret = imi_alloc_transfer_buf(video);
	if (ret)
		return ret;
#endif
	g_imi_video = video;

	return 0;
}

/*
 * Initialize the imi bulkout upload
 */
int imi_upload_init(struct imi_video_dev *dev)
{
	spin_lock_init(&dev->udev.lock);

	INIT_LIST_HEAD(&dev->udev.read_wq.wait_head);
	dev->udev.tx_complete = FRTOS_completion_init();

	return 0;
}
