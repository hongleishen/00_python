#include <soc/imi_video.h>
#include <usb/linux/gadget.h>
#include <errno.h>
#include <linux_compat.h>
#include <imi_dev.h>
#include <task.h>
#include "../../include/vendor_cmd.h"
#include "../../include/vendor_sub_system.h"
#include "../../include/vendor_device.h"

#define LOG_TAG "vendor_usb"
#include "elog.h"

#define VENDOR_USB_STREAM_NUM           3
#define VENDOR_USB_MAX_BUF_NUM          6
#define  USB_DEBUG  					1
static const char *imi_usb_path = "imi_color";

struct usb_stream_info {
	int stream_id;
	//max buf num for usb transfer
	int max_buf;
	enum sensor_stream_type buf_id;
	//dma or memcpy
	enum imi_buf_type buf_type;
};

struct imi_usb_buf {
	struct list_head list;
	struct imi_video_buf_info info;
};

struct imi_usb_tsk {
	TaskHandle_t  tsk_handle;
	int exit;
};

struct imi_usb_stream {
	struct usb_stream_info *info;
	struct imi_usb_buf buf[VENDOR_USB_MAX_BUF_NUM];
	int imi_open_cnt;
	int used_buf;
	FRTOS_mutex mutex;
	struct list_head idle;
};

struct vendor_usb {
	//for buf transfer
	struct imi_device_handle  *usb_handle;
	//imi_opened stream
	int imi_open_cnt;
	FRTOS_mutex mutex;
	struct list_head buf;
	//process usb ep in cmd
	struct imi_usb_tsk dequeue_thread;
	struct imi_usb_tsk get_buf_thread;
	struct imi_usb_tsk queue_thread;
	//for buf
#define MAX_VIDEO_BUF 10
	QueueHandle_t buf_queue;
	//total stream for hjimi
	struct imi_usb_stream usb_stream[VENDOR_USB_STREAM_NUM];
};

#define VENDOR_USB_GET()    &imi_vendor_usb
static struct vendor_usb imi_vendor_usb;
//all kinds of usb ep
static struct usb_stream_info stream_info[VENDOR_USB_STREAM_NUM] = {
	{VENDOR_STREAM_COLOR_USB,       3,  ISP_SENSOR_STREAM_YUV,     IMI_BUF_TYPE_RAW},
	{VENDOR_STREAM_DEPTH_USB,       3,  DEPTH_SENSOR_STREAM,       IMI_BUF_TYPE_RAW},
	{VENDOR_STREAM_IR_USB,          3,  IR_SENSOR_STREAM,          IMI_BUF_TYPE_RAW},
};

static int vendor_usb_queue_buf(struct imi_usb_buf *buf)
{
	int ret;
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	struct imi_usb_stream *stream = &imi_usb->usb_stream[buf->info.dev_index];

	if (buf == NULL)
		return -EINVAL;

	ret = imi_ioctl(imi_usb->usb_handle, VIDIOC_IMI_QBUF, &buf->info);
	if (ret < 0) {
		log_e("stream %d queue buf fail\n", stream->info->stream_id);
		return ret;
	}

	return 0;
}

static int vendor_usb_dequeue_buf(struct imi_usb_buf **buf)
{
	int ret;
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	struct imi_video_buf_info info;
	struct imi_usb_stream *stream;

	ret = imi_ioctl(imi_usb->usb_handle, VIDIOC_IMI_DQBUF, &info);
	if (ret < 0) {
		log_e("dequeue buf fail\n");
		return ret;
	}

	if (info.dev_index >= VENDOR_USB_STREAM_NUM) {
		log_e("dev index fail %d\n", info.dev_index);
		return -EINVAL;
	}

	stream = &imi_usb->usb_stream[info.dev_index];

	*buf = &stream->buf[info.index];
	 log_e(" buf:%p, 0x%x \r\n", &(*buf)->info,  (*buf)->info.phy_addr);
	 return 0;
}

static int vendor_usb_data_ctl(struct vendor_usb *usb, int onoff)
{
	int ret;

	if (onoff) {
		ret = imi_ioctl(usb->usb_handle, VIDIOC_IMI_STREAMON, &onoff);
		if (ret < 0) {
			log_e("stream on fail\n");
			return ret;
		}
	} else {
		ret = imi_ioctl(usb->usb_handle, VIDIOC_IMI_STREAMOFF, &onoff);
		if (ret < 0) {
			log_e("stream off fail\n");
			return ret;
		}
	}

	return 0;
}

static void vendor_usb_give_back_buf(struct imi_usb_buf *buf)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	struct imi_usb_stream *stream = NULL;

//to do give back buf

	stream = &imi_usb->usb_stream[buf->info.dev_index];

	if (stream->imi_open_cnt > 0) {
		FRTOS_mutex_lock(stream->mutex);
		list_add(&buf->list, &stream->idle);
		stream->used_buf--;
		FRTOS_mutex_unlock(stream->mutex);
	}
}
int vendor_usb_send_buf(struct imi_video_buf_info *buf)
{
	

	if (buf == NULL)
		return -EINVAL;
#ifdef USB_DEBUG
	buf->tm_stamp = 0x123456;
	buf->phy_addr = 0x89000000;
	buf->frame_len = 614400;
	buf->dev_index = 2;
	vTaskDelay(5);
	return 0;
#else
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	return xQueueSendToBack(imi_usb->buf_queue, buf, pdMS_TO_TICKS(30));
#endif
}

static void vendor_usb_get_buf_thread(void *param)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	struct imi_usb_stream *stream = NULL;
	struct imi_usb_buf *buf = NULL;
	struct imi_video_buf_info info;

	while (!imi_usb->get_buf_thread.exit) {
		if (!imi_usb->imi_open_cnt) {
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			continue;
		}
#ifdef USB_DEBUG
		vendor_usb_send_buf(&info);
#else
		xQueueReceive(imi_usb->buf_queue, &info, portMAX_DELAY);
#endif
		stream = &imi_usb->usb_stream[info.dev_index];

		if (stream->imi_open_cnt == 0 || stream->used_buf >= stream->info->max_buf - 1) {
			//give back buf
			continue;
		}

		buf = NULL;

		FRTOS_mutex_lock(stream->mutex);
		if (!list_empty(&stream->idle)) {
			buf = list_first_entry(&stream->idle,
					struct imi_usb_buf, list);
		}
		FRTOS_mutex_unlock(stream->mutex);

		if (!buf) {
			//give back buf
			continue;
		}

		buf->info.frame_len = info.frame_len;
		buf->info.phy_addr = info.phy_addr;
		buf->info.tm_stamp = info.tm_stamp;
		log_e("%p 0x%x \r\n", &buf->info, buf->info.phy_addr);
		FRTOS_mutex_lock(stream->mutex);
		stream->used_buf++;
		list_del(&buf->list);
		FRTOS_mutex_unlock(stream->mutex);

		FRTOS_mutex_lock(imi_usb->mutex);
		list_add(&buf->list, &imi_usb->buf);
		FRTOS_mutex_unlock(imi_usb->mutex);

		xTaskNotifyGive(imi_usb->queue_thread.tsk_handle);
	}

	vTaskDelete(NULL);
}

static int vendor_usb_stream_init(struct imi_usb_stream *stream)
{
	struct imi_video_buf_info *buf;
	int index;

	FRTOS_mutex_lock(stream->mutex);
	INIT_LIST_HEAD(&stream->idle);

	stream->used_buf = 0;
	for (index = 0; index < stream->info->max_buf; ++index) {
		buf = &stream->buf[index].info;
		buf->index = index;
		buf->dev_index = stream->info->stream_id - VENDOR_STREAM_COLOR_USB;
		list_add(&stream->buf[index].list, &stream->idle);
		log_e("%p 0x%x \r\n", buf, buf->phy_addr);
	}
	FRTOS_mutex_unlock(stream->mutex);

	return 0;
}

static void vendor_usb_stream_deinit(struct imi_usb_stream *stream)
{
#if 0
	int index;
	struct imi_usb_buf *buf;

	for (index = 0; index < stream->info->max_buf; ++index) {
		buf = &stream->buf[index];
		//give back buf
	}
#endif
}

static int vendor_usb_sub_dev_ctl(struct imi_usb_stream *stream, char onoff)
{
	struct imi_video_sub_dev_ctl ctl;
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	int ret;

	ctl.cmd = onoff;
	ctl.dev_index = stream->info->stream_id - VENDOR_STREAM_COLOR_USB;

	ret = imi_ioctl(imi_usb->usb_handle, VIDIOC_IMI_SUB_DEV_CTL, &ctl);
	if (ret < 0) {
		log_e("sub_dev_ctl fail\n");
		return ret;
	}

	return 0;
}

static int vendor_usb_sub_stream_ctl(struct imi_usb_stream *stream, char onoff)
{
	int ret;

	if (onoff) {
		vendor_usb_stream_init(stream);
		ret = vendor_usb_sub_dev_ctl(stream, 1);
		if (ret)
			return ret;
	} else {
		vendor_usb_sub_dev_ctl(stream, 0);
		vendor_usb_stream_deinit(stream);
	}

	return 0;
}

static void vendor_usb_queue_thread(void *priv)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	struct imi_usb_buf *buf = NULL;
	int ret;

	while (!imi_usb->dequeue_thread.exit) {
		buf = NULL;

		FRTOS_mutex_lock(imi_usb->mutex);
		if (!list_empty(&imi_usb->buf)) {
			buf = list_first_entry(&imi_usb->buf,
					struct imi_usb_buf, list);

			list_del(&buf->list);
		}
		FRTOS_mutex_unlock(imi_usb->mutex);

		if (!buf) {
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			continue;
		}
    	log_e("%p 0x%x \r\n", &buf->info, buf->info.phy_addr);
		ret = vendor_usb_queue_buf(buf);
		if (ret < 0) {
			vendor_usb_give_back_buf(buf);

			log_e("queue buf fail\n");
			continue;
		}
	}

	vTaskDelete(NULL);

}

int vendor_usb_process_ep_in_cmd(void)
{
	struct imi_usb_buf *buf;
	int ret;

	ret = vendor_usb_dequeue_buf(&buf);
	if (ret < 0)
		return -EAGAIN;

	vendor_usb_give_back_buf(buf);

	return 0;
}

static void vendor_usb_dequeue_thread(void *info)
{
	//struct vendor_task_info *tsk = (struct vendor_task_info *)info;
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	TickType_t timeout;
	int ret;
	int wfds = 0;

	while(!imi_usb->dequeue_thread.exit) {
		if (imi_usb->imi_open_cnt == 0) {
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			continue;
		}

		wfds = 0;

		timeout = msecs_to_jiffies(1000);

		ret = imi_poll(imi_usb->usb_handle, NULL, &wfds, NULL, timeout);
		if (ret < 0) {
			log_e("error: select failed with %d\n", ret);
			continue;
		}

		if (imi_usb->imi_open_cnt == 0)
			continue;

		if (wfds) {
			vendor_usb_process_ep_in_cmd();
		}
	}

	vTaskDelete(NULL);
}

static void vendor_usb_param_init(void)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();

	INIT_LIST_HEAD(&imi_usb->buf);
}

static int vendor_usb_start_video(void)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();

	xTaskCreate(&vendor_usb_queue_thread, "usb_queue", IMI_DEV_STK_SIZE, (void *)imi_usb, TSK_PRIO_APP, &imi_usb->queue_thread.tsk_handle);

	xTaskCreate(&vendor_usb_dequeue_thread, "usb_dequeue", IMI_DEV_STK_SIZE, (void *)imi_usb, TSK_PRIO_APP, &imi_usb->dequeue_thread.tsk_handle);

	xTaskCreate(&vendor_usb_get_buf_thread, "usb_get_buf", IMI_DEV_STK_SIZE, (void *)imi_usb, TSK_PRIO_APP, &imi_usb->get_buf_thread.tsk_handle);

	return 0;
}

static int vendor_usb_stop_video(void)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();

	imi_usb->queue_thread.exit = 1;
	imi_usb->dequeue_thread.exit = 1;
	imi_usb->get_buf_thread.exit = 1;
	xTaskNotifyGive(imi_usb->queue_thread.tsk_handle);
	xTaskNotifyGive(imi_usb->dequeue_thread.tsk_handle);
	xTaskNotifyGive(imi_usb->get_buf_thread.tsk_handle);
	return 0;
}

void vendor_usb_reset_streams(void)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	struct imi_usb_stream *stream = NULL;
	int index;

	for (index = 0; index < VENDOR_USB_STREAM_NUM; index++) {
		stream = &imi_usb->usb_stream[index];
		if (stream->imi_open_cnt > 0) {
			vendor_usb_sub_stream_ctl(stream, 0);
			stream->imi_open_cnt = 0;
		}
	}
}

static int vendor_usb_process_events(int stream_id,
		struct imi_vendor_sensor_cmd *cmd)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();

	if (stream_id < VENDOR_STREAM_COLOR_USB || stream_id > VENDOR_STREAM_MAX)
		return IMI_PROTOCOL_STREAM_TYPE_ERR;

	if (cmd == NULL)
		return -EINVAL;

	switch (cmd->cmd_type) {
	case VENDOR_CMD_USB_DISCONNECT:
		if (imi_usb->imi_open_cnt > 0) {
			imi_usb->imi_open_cnt = 0;
			vendor_usb_reset_streams();
			vendor_usb_data_ctl(imi_usb, IMI_STREAM_OFF);
		}
		break;

	default:
		break;
	}

	return 0;
}

static int vendor_usb_stream_ctl(struct imi_stream_ctl_cmd *cmd)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	struct imi_usb_stream *stream = &imi_usb->usb_stream[cmd->stream_type - VENDOR_STREAM_COLOR_USB];
	int ret = 0;

	if (cmd->stream_type < VENDOR_STREAM_COLOR_USB || cmd->stream_type >= VENDOR_STREAM_MAX)
		return IMI_PROTOCOL_STREAM_TYPE_ERR;

	if (cmd->stream_type == VENDOR_STREAM_COLOR_USB) {
		switch(cmd->fmt.pixel) {
		case IMI_PIXEL_COLOR_MJPEG:
			stream->info->buf_id = VPU_SENSOR_STREAM_JPG;
			break;

		default:
			break;
		}
	}

	switch (cmd->onoff) {
		case IMI_STREAM_ON:
			if (stream->imi_open_cnt == 0) {
				ret = vendor_usb_sub_stream_ctl(stream, IMI_STREAM_ON);
				if (ret) {
					log_e("vendor_usb_sub_stream_ctl fail\n");
					return ret;
				}
			}
			stream->imi_open_cnt++;

			if (imi_usb->imi_open_cnt == 0) {
				vendor_usb_param_init();
				ret = vendor_usb_data_ctl(imi_usb, IMI_STREAM_ON);
				if (ret) {
					log_e("vendor_usb_data_ctl fail\n");
					vendor_usb_sub_stream_ctl(stream, IMI_STREAM_OFF);
					return ret;
				}
			}
			imi_usb->imi_open_cnt++;

			xTaskNotifyGive(imi_usb->get_buf_thread.tsk_handle);
			xTaskNotifyGive(imi_usb->dequeue_thread.tsk_handle);
			break;

		case IMI_STREAM_OFF:
			if (stream->imi_open_cnt > 0) {
				stream->imi_open_cnt--;
				if (stream->imi_open_cnt == 0)
					vendor_usb_sub_stream_ctl(stream, IMI_STREAM_OFF);
			}

			if (imi_usb->imi_open_cnt > 0) {
				imi_usb->imi_open_cnt--;
				if (imi_usb->imi_open_cnt == 0)
					vendor_usb_data_ctl(imi_usb, IMI_STREAM_OFF);
			}
			break;

		case IMI_STREAM_SUSPEND:
			break;

		case IMI_STREAM_RESUME:
			break;

		default:
			break;
	}

	return 0;
}

static int vendor_usb_buf_request(struct imi_usb_stream *stream)
{
	struct imi_video_req_buf req_buf;
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	int ret;

	if (stream == NULL)
		return -EINVAL;

	req_buf.num = stream->info->max_buf;
	req_buf.type = stream->info->buf_type;
	req_buf.dev_index = stream->info->stream_id - VENDOR_STREAM_COLOR_USB;

	ret = imi_ioctl(imi_usb->usb_handle, VIDIOC_IMI_REQ_BUF, &req_buf);
	if (ret < 0) {
		log_e("stream %d req buf fail\n", stream->info->stream_id);
		return ret;
	}

	return 0;
}

static void vendor_usb_close_sub_streams(void)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	struct imi_usb_stream *stream;
	int index;

	for (index = 0; index < VENDOR_USB_STREAM_NUM; index++) {
		stream = &imi_usb->usb_stream[index];
		FRTOS_mutex_free(&stream->mutex);
	}
}

static int vendor_usb_handshake(void *pdata)
{
	return 0; //todo, add handshake code here
}

static int vendor_usb_para_init(int id, void *pdata)
{
	return 0;
}


static int vendor_usb_init(struct sub_sys_param *param)
{
	struct imi_usb_stream *stream;
	struct vendor_usb *imi_usb = VENDOR_USB_GET();
	int index, ret = -EFAULT;

	imi_usb->imi_open_cnt = 0;

	FRTOS_mutex_create(&imi_usb->mutex);

	imi_usb->usb_handle = imi_open(imi_usb_path);
	if (imi_usb->usb_handle == NULL) {
		log_e("open %s fail\n", imi_usb_path);
		//ret = -ENOENT;
		//goto fail2;
		return 0;
	}

	imi_usb->buf_queue = xQueueCreate(MAX_VIDEO_BUF, sizeof(struct imi_video_buf_info));
	if (imi_usb->buf_queue == NULL) {
		log_e("Create buf queue fail\r\n");
		imi_close(imi_usb->usb_handle);
		return 0;
	}

	for (index = 0; index < VENDOR_USB_STREAM_NUM; index++) {
		stream = &imi_usb->usb_stream[index];
		stream->info = &stream_info[index];
		stream->imi_open_cnt = 0;
		FRTOS_mutex_create(&stream->mutex);
		memset((void *)&stream->buf, 0x00, sizeof(struct imi_video_buf_info) * VENDOR_USB_MAX_BUF_NUM);
		ret = vendor_usb_buf_request(stream);
		if (ret) {
			log_e("vendor_usb_buf_request fail\n");
			goto fail3;
		}
	}

	vendor_usb_param_init();

	ret = vendor_usb_start_video();
	if (ret != 0) {
		log_e("vendor_usb_start_video fail\n");
		goto fail4;
	}

	return 0;

fail4:
	vendor_usb_close_sub_streams();
fail3:
	vQueueDelete(imi_usb->buf_queue);
	imi_close(imi_usb->usb_handle);
//fail2:
	imi_usb->usb_handle = NULL;
	FRTOS_mutex_free(&imi_usb->mutex);
	return ret;
}

static void vendor_usb_deinit(void)
{
	struct vendor_usb *imi_usb = VENDOR_USB_GET();

	vendor_usb_close_sub_streams();
	vendor_usb_stop_video();

	if (imi_usb->usb_handle != NULL) {
		imi_close(imi_usb->usb_handle);
		imi_usb->usb_handle = NULL;
	}

	FRTOS_mutex_free(&imi_usb->mutex);
}

struct sub_system_device imi_sub_system_usb = {
	.name = "usb",
	.init = vendor_usb_init,
	.deinit = vendor_usb_deinit,
	.stream_ctl = vendor_usb_stream_ctl,
	.process_events = vendor_usb_process_events,
	.handshake = vendor_usb_handshake,
	.parameter_init = vendor_usb_para_init,
};

