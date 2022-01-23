#include <soc/imi_video.h>
#include <usb/linux/gadget.h>
#include <errno.h>
#include <linux_compat.h>
#include <imi_dev.h>
#include "../include/vendor_device.h"
#include "../include/vendor_cmd.h"

#define LOG_TAG "cmd_hal"
#include "elog.h"

#define IMI_CMD_DEV "imi_usb_cmd"
#define EP0_MAX_PACKET  60

struct cmd_hal {
	struct imi_device_handle *cmd_handle;

	TaskHandle_t cmd_thd;
	int exit_read;

	struct uvc_request_data buf;
	struct usb_ctrlrequest ctrl;
	enum vendor_cmd_stage  stage;

	struct uvc_request_data *cmd_ret;
	int (*data_handle)(struct uvc_request_data *resp);
};

struct cmd_hal imi_cmd_hal;

static int cmd_hal_wait_data(struct cmd_hal *p_cmd_hal)
{
	int ret;

	if (p_cmd_hal->cmd_handle == NULL)
		return -1;

	ret = imi_ioctl(p_cmd_hal->cmd_handle, VIDIOC_GET_DATA_STAGE, (void *)(&p_cmd_hal->stage));
	if (ret < 0)
		log_e("fail to get cmd");

	return ret;
}

static int cmd_hal_write_data(struct cmd_hal *p_cmd_hal, struct uvc_request_data *data)
{
	int ret, len;

	if (p_cmd_hal->cmd_handle == NULL)
		return -1;

	len = p_cmd_hal->ctrl.wLength < data->length ? p_cmd_hal->ctrl.wLength : data->length;
	data->length = len;

	ret = imi_write(p_cmd_hal->cmd_handle, data, sizeof(*data));
	if (ret != sizeof(*data))
		log_e("write fail\n");

	return ret;
}

static int cmd_hal_read_data(struct cmd_hal *p_cmd_hal)
{
	int ret;
	struct usb_ctrlrequest *ctrl = &p_cmd_hal->ctrl;
	(void)ctrl;

	if (p_cmd_hal->cmd_handle == NULL)
		return -1;
	
	if (p_cmd_hal->stage == VENDOR_CMD_STAGE_DATA) {
		ret = imi_read(p_cmd_hal->cmd_handle, &p_cmd_hal->buf.data, sizeof(p_cmd_hal->buf.data));
		if (ret > 0)
			p_cmd_hal->buf.length = ret;

		//log_e("rd: %x %x %x %x\n", p_cmd_hal->buf.data[0],p_cmd_hal->buf.data[1],p_cmd_hal->buf.data[2],p_cmd_hal->buf.data[3]);
	} else if (p_cmd_hal->stage == VENDOR_CMD_STAGE_SETUP) {
		ret = imi_read(p_cmd_hal->cmd_handle, &p_cmd_hal->ctrl, sizeof(p_cmd_hal->ctrl));
	} else {
		//err
		log_e("unknow stage\n");
	}

	return ret;
}

static void cmd_hal_handle_cmd(struct cmd_hal *p_cmd_hal)
{
	switch (p_cmd_hal->stage) {
	case VENDOR_CMD_STAGE_SETUP:
		if (p_cmd_hal->ctrl.bRequestType & 0x80) { //get cmd ret phase
			//log_e("in len:%d\n",p_cmd_hal->cmd_ret->length);
			cmd_hal_write_data(p_cmd_hal, p_cmd_hal->cmd_ret);
			p_cmd_hal->cmd_ret->length = -EL2HLT;
		} else { //out vendor cmd phase
			//log_e("out len:%d\n",p_cmd_hal->ctrl.wLength);
			p_cmd_hal->buf.length = p_cmd_hal->ctrl.wLength;
			cmd_hal_write_data(p_cmd_hal, &p_cmd_hal->buf);
		}
		break;

	case VENDOR_CMD_STAGE_DATA:
		p_cmd_hal->data_handle(&p_cmd_hal->buf);
		break;

	default:
		log_e("unkonwn stage cmd\n");
		break;
	}
}

static void cmd_hal_read_thread(void *priv)
{
	struct cmd_hal *p_cmd_hal = (struct cmd_hal *)priv;
	int ret;

	while (!(p_cmd_hal->exit_read)) {
		ret = cmd_hal_wait_data(p_cmd_hal);
		if (ret < 0)
			continue;

		ret = cmd_hal_read_data(p_cmd_hal);
		if (ret > 0)
			cmd_hal_handle_cmd(p_cmd_hal);
	}
}

static void cmd_hal_exit(struct cmd_hal *p_cmd_hal)
{
	int ret;

	if (p_cmd_hal->cmd_handle == NULL)
		return;

	p_cmd_hal->exit_read = 1;

	ret = imi_ioctl(p_cmd_hal->cmd_handle, VIDIOC_READ_CMD_EXIT, (void *)(&p_cmd_hal->exit_read));
	if (ret < 0)
		log_e("fail to exit");

	return;

}

static int cmd_hal_init(struct cmd_hal *p_cmd_hal)
{
	int ret, init = 1;

	if (p_cmd_hal->cmd_handle == NULL)
		return -1;

	ret = imi_ioctl(p_cmd_hal->cmd_handle, VIDIOC_READ_CMD_INIT, (void *)(&init));
	if (ret < 0)
		log_e("fail to init");

	return ret;
}

int imi_cmd_hal_init(struct uvc_request_data *cmd_ret, int (*data_handle)(struct uvc_request_data *))
{
	struct cmd_hal *p_cmd_hal = &imi_cmd_hal;
	int ret;

	memset((void *)p_cmd_hal, 0x00, sizeof(struct cmd_hal));

	p_cmd_hal->cmd_handle = imi_open(IMI_CMD_DEV);
	if (p_cmd_hal->cmd_handle == NULL) {
		log_e("can not imi_open %s\n", IMI_CMD_DEV);
		return -1;
	}

	p_cmd_hal->data_handle = data_handle;
	p_cmd_hal->cmd_ret = cmd_ret;

	ret = cmd_hal_init(p_cmd_hal);
	if (ret)
		return ret;

	xTaskCreate(cmd_hal_read_thread, "cmd_hal", IMI_DEV_STK_SIZE, p_cmd_hal, TSK_PRIO_INT_BOTTOM_HALF, &p_cmd_hal->cmd_thd);

	return 0;
}

void imi_cmd_hal_deinit(void)
{
	struct cmd_hal *p_cmd_hal = &imi_cmd_hal;

	cmd_hal_exit(p_cmd_hal);

	vTaskDelete(p_cmd_hal->cmd_thd);
}
