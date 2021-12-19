#include <soc/imi_video.h>
#include <usb/linux/gadget.h>
#include <errno.h>
#include <linux_compat.h>
#include <imi_dev.h>
#include <task.h>
#include "../include/vendor_cmd.h"
#include "../include/vendor_device.h"
#include "../include/vendor_misc.h"
#include "../include/vendor_sub_system.h"
#include "initcalls.h"

#define LOG_TAG "vendor_device"
#include "elog.h"

#define MAX_STREAM_CTL_ITEM 10
//static int factory_test_result;

struct stream_ctl_info {
	struct imi_stream_ctl_cmd cmd[MAX_STREAM_CTL_ITEM];
	int w_index;
	int r_index;
};

struct stream_ctl_tsk {
	int exit_tsk;
	TaskHandle_t cmd_thd;
	struct stream_ctl_info ctl;
};

struct vendor_device_tab {
	unsigned int param;
	unsigned int id_sensor;
};

static const struct vendor_device_tab vendor_device_map[] = {
	{VENDOR_DEVICE_PARAM_MAX,   VENDOR_STREAM_DEPTH       },
	{COLOR_IMG_PARAM_MAX,       VENDOR_STREAM_COLOR       },
	{DEPTH_IMG_PARAM_MAX,       VENDOR_STREAM_DEPTH       },
	{IR_IMG_PARAM_MAX,          VENDOR_STREAM_IR          },
	{USER_TRACK_PARAM_MAX,      VENDOR_STREAM_USER_TRACK  },
	{FACE_DETECT_PARAM_MAX,     VENDOR_STREAM_FACE_DETECT },
	{SLAM_IMU_PARAM_MAX,        VENDOR_STREAM_SLAM        },
	{IMI_PAY_PARAM_MAX,         VENDOR_STREAM_ENCRY       },
};

static struct imi_vendor_device_ver vendor_device_ver = {
	.fw_major = 0x01,
	.fw_minor = 0x00,
	.fw_ver = 0x01,

	.ap_major = 0x01,
	.ap_minor = 0x00,
	.ap_ver = 0x01,

	.hardware = 0x01,
	.chip = 0x01,
	.ap_hardware = 0x01,
};

static FRTOS_mutex mutex;
static struct uvc_request_data vendor_resp;
static struct stream_ctl_tsk imi_stream_tsk;
//static struct imi_sub_system_version sub_sys_ver;

static inline void vendor_device_fill_rsp(void *rsp_head, void *data, int data_len)
{
	memcpy((void *)vendor_resp.data, rsp_head, VENDOR_CMD_RSP_HEADER_LEN);

	if (data_len) {
		memcpy((void *)&vendor_resp.data[VENDOR_CMD_RSP_HEADER_LEN], data, data_len);
		vendor_resp.length = VENDOR_CMD_RSP_HEADER_LEN + data_len;
	} else {
		vendor_resp.length = VENDOR_CMD_RSP_HEADER_LEN;
	}
}

static inline void vendor_device_create_header(struct imi_vendor_cmd_rsp_header *rsp_head, struct imi_vendor_cmd_header *ptr,
	int data_len, enum imi_protocol_ret_code code)
{
	rsp_head->rsp.flag = DEVICE_TO_HOST;
	rsp_head->rsp.cmd = ptr->cmd;
	rsp_head->rsp.cmd_cnt = ptr->cmd_cnt + 1;
	rsp_head->rsp.data_len = data_len;
	rsp_head->ret = code;
}

static int vendor_device_get_ver(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	int ver_len = sizeof(struct imi_vendor_device_ver);

	if (req == NULL)
		return -EINVAL;

	vendor_device_create_header(&rsp_head, ptr, sizeof(struct imi_vendor_device_ver), IMI_PROTOCOL_OK);
	vendor_device_fill_rsp((void *)&rsp_head, (void *)&vendor_device_ver, ver_len);

	return 0;
}

static int vendor_device_keep_alive(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;

	if (req == NULL)
		return -EINVAL;

	vendor_device_create_header(&rsp_head, ptr, 2, IMI_PROTOCOL_OK);
	vendor_device_fill_rsp((void *)&rsp_head, NULL, 0);

	return 0;
}

static inline int vendor_device_dev_info_fix_up(unsigned int param, unsigned int *id_sensor)
{
	int ret = -EINVAL;

	switch (param) {
	case DPU_LIGHT_CONTROL:
		*id_sensor = VENDOR_STREAM_DEPTH;
		ret = 0;
		break;

	default:
		break;
	}

	return ret;
}

static int vendor_deivce_get_dev_info(unsigned int param, unsigned int *id_sensor)
{
	int index, len;

	len = ARRAY_SIZE(vendor_device_map);
	for (index = 0; index < len; index++) {
		if (param <= vendor_device_map[index].param)
			break;
	}

	if (index < len) {
		*id_sensor = vendor_device_map[index].id_sensor;
		return 0;
	}

	return vendor_device_dev_info_fix_up(param, id_sensor);
}

static int vendor_device_get_property(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct imi_sensor_param_info *info = (struct imi_sensor_param_info *)&req->data[VENDOR_CMD_HEADER_LEN];
	struct imi_vendor_sensor_cmd sensor_cmd;
	unsigned int id_sensor;
	int ret = 0;

	if (req == NULL)
		return -EINVAL;

	ret = vendor_deivce_get_dev_info(info->type, &id_sensor);
	if (ret < 0) {
		log_e("get dev info err\n");
		ret = IMI_PROTOCOL_PROPERTY_TYPE_ERR;
		goto end;
	}

	memset((void *)&sensor_cmd, 0x00, sizeof(struct imi_vendor_sensor_cmd));
	sensor_cmd.cmd_type = ptr->cmd;
	sensor_cmd.sensor_param.type = info->type;

	if (id_sensor != VENDOR_STREAM_DEVICE) {
		ret = sub_system_process_events(id_sensor, &sensor_cmd);
	} else {
		ret = vendor_misc_handle_cmd(ptr->cmd, &sensor_cmd);
	}

	if (ret < 0)
		ret = IMI_PROTOCOL_FAILURE;

end:
	vendor_device_create_header(&rsp_head, ptr, sensor_cmd.sensor_param.data_len + 2, ret);
	vendor_device_fill_rsp(&rsp_head, &sensor_cmd.sensor_param.data, sensor_cmd.sensor_param.data_len);

	return 0;
}

static void vendor_device_stream_ctl_internal(enum imi_stream_type type, int on, int mode)
{
	struct stream_ctl_tsk *p_tsk = &imi_stream_tsk;

	p_tsk->ctl.cmd[p_tsk->ctl.w_index].stream_type = type;
	p_tsk->ctl.cmd[p_tsk->ctl.w_index].onoff = on;
	p_tsk->ctl.cmd[p_tsk->ctl.w_index].mode = mode;
	p_tsk->ctl.w_index++;
	p_tsk->ctl.w_index = p_tsk->ctl.w_index % MAX_STREAM_CTL_ITEM;

	xTaskNotifyGive(p_tsk->cmd_thd);
}

static int _vendor_device_set_property(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct imi_sensor_param_info *info = (struct imi_sensor_param_info *)&req->data[VENDOR_CMD_HEADER_LEN];
	struct imi_vendor_sensor_cmd sensor_cmd;
	unsigned int id_sensor;
	int ret = IMI_PROTOCOL_OK;

	if (req == NULL)
		return -EINVAL;

	ret = vendor_deivce_get_dev_info(info->type, &id_sensor);
	if (ret < 0) {
		log_e("get dev info err\n");
		ret = IMI_PROTOCOL_PROPERTY_TYPE_ERR;
		goto end;
	}

	memset((void *)&sensor_cmd, 0x00, sizeof(struct imi_vendor_sensor_cmd));
	sensor_cmd.cmd_type = ptr->cmd;
	sensor_cmd.sensor_param.type = info->type;
	sensor_cmd.sensor_param.data_len = ptr->data_len - 2;
	memcpy(sensor_cmd.sensor_param.data, info->data, sensor_cmd.sensor_param.data_len);

	if (id_sensor != VENDOR_STREAM_DEVICE) {
		ret = sub_system_process_events(id_sensor, &sensor_cmd);
	} else {
		ret = vendor_misc_handle_cmd(ptr->cmd, &sensor_cmd);
	}

	if (ret < 0)
		ret = IMI_PROTOCOL_FAILURE;

end:
	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return ret;
}

static int vendor_device_echo_stream_ctl(struct uvc_request_data *req, int on, int mode)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	int ret = 0;

	vendor_device_stream_ctl_internal(VENDOR_STREAM_BULK_OUT, on, mode);

	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_set_property(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct imi_sensor_param_info *info = (struct imi_sensor_param_info *)&req->data[VENDOR_CMD_HEADER_LEN];
	int ret = 0;

	switch (info->type) {
	case IMI_PAY_PARAM_FRAME_OPEN:
		vendor_device_stream_ctl_internal(VENDOR_STREAM_ENCRY, 1, 0);
		ret = _vendor_device_set_property(req);
		if (ret != IMI_PROTOCOL_OK) {
			vendor_device_stream_ctl_internal(VENDOR_STREAM_ENCRY, 0, 0);
		}
		break;

	case IMI_PAY_PARAM_FRAME_CLOSE:
		_vendor_device_set_property(req);
		vendor_device_stream_ctl_internal(VENDOR_STREAM_ENCRY, 0, 0);
		break;

	case IMI_ECHO_DEBUG_SENSOR_OPEN:
		vendor_device_echo_stream_ctl(req, 1, info->type);
		break;

	case IMI_ECHO_DEBUG_SENSOR_CLOSE:
		vendor_device_echo_stream_ctl(req, 0, info->type);
		break;

	case REF_IMG_SEND_START:
		vendor_device_echo_stream_ctl(req, 1, info->type);
		break;

	case REF_IMG_SEND_END:
		vendor_device_echo_stream_ctl(req, 0, info->type);
		break;

	case IMI_RECOVERY_UPDATE_START:
		vendor_device_create_header(&rsp_head, ptr, 2, ret);
		vendor_device_fill_rsp(&rsp_head, NULL, 0);
		log_e("===== recovery update system start =====\n");
		break;

	default:
		ret = _vendor_device_set_property(req);
		break;
	}

	return ret;
}

int vendor_device_handle_stream_ctl(const struct imi_stream_ctl_cmd *p_cmd)
{
	int ret, stream_type, stream_id[5];
	int index, len, j, cnt = 0;
	struct imi_stream_ctl_cmd cmd = *p_cmd;

	stream_type = cmd.stream_type;

	switch (stream_type) {
	case VENDOR_STREAM_COLOR:
		if (cmd.fmt.pixel == IMI_PIXEL_COLOR_MJPEG
			|| cmd.fmt.pixel == IMI_PIXEL_COLOR_H264) {
			stream_id[cnt++] = VENDOR_STREAM_DEPTH; //color is triggered by dpu
			stream_id[cnt++] = stream_type;
			stream_id[cnt++] = VENDOR_STREAM_VPU;
			stream_id[cnt++] = stream_type + VENDOR_STREAM_COLOR_USB;
			len = cnt;
		} else {
			stream_id[cnt++] = VENDOR_STREAM_DEPTH;
			stream_id[cnt++] = stream_type;
			stream_id[cnt++] = stream_type + VENDOR_STREAM_COLOR_USB;
			len = cnt;
		}
		break;

	case VENDOR_STREAM_DEPTH:
	case VENDOR_STREAM_IR:
	//	stream_id[cnt++] = stream_type;
		stream_id[cnt++] = stream_type + VENDOR_STREAM_COLOR_USB;
		len = cnt;
		break;

	case VENDOR_STREAM_FACE_DETECT:
		stream_id[cnt++] = VENDOR_STREAM_DEPTH;

		//IMI_FACE_ECHO_FRAME_MODE: host will send RGB to dsp
		if (cmd.face_detect.alg_type != IMI_FACE_ECHO_FRAME_MODE)
			stream_id[cnt++] = VENDOR_STREAM_COLOR;

		stream_id[cnt++] = stream_type;
		stream_id[cnt++] = stream_type + VENDOR_STREAM_COLOR_USB;
		len = cnt;
		break;

	case VENDOR_STREAM_USER_TRACK:
		if (cmd.user_track.alg_type != IMI_HAND_TRACK_MODE)
			stream_id[cnt++] = VENDOR_STREAM_COLOR;

		stream_id[cnt++] = VENDOR_STREAM_DEPTH;
		stream_id[cnt++] = stream_type;
		stream_id[cnt++] = stream_type + VENDOR_STREAM_COLOR_USB;
		len = cnt;
		break;

	case VENDOR_STREAM_ENCRY:
		stream_id[cnt++] = VENDOR_STREAM_ENCRY;
		stream_id[cnt++] = VENDOR_STREAM_ENCRY_USB;
		len = cnt;
		break;

	case VENDOR_STREAM_BULK_OUT:
		stream_id[cnt++] = VENDOR_STREAM_BULK_OUT;
		len = cnt;
		break;

	case 0xD0:
		stream_id[cnt++] = VENDOR_STREAM_AUDIO_LEFT_USB;
	//	stream_id[cnt++] = VENDOR_STREAM_AUDIO_RIGHT_USB;
		len = cnt;
		break;

	default:
		len = 0;
		break;
	}

	cmd.stream_2_usb = stream_type;

	for (index = 0; index < len; index++) {
		cmd.stream_type = stream_id[index];
		ret = sub_system_stream_ctl(&cmd);
		if (ret) {
			log_e("stream %d %s fail\n", cmd.stream_type, cmd.onoff ? "on" : "off");
			goto fail;
		}
	}

	return 0;

fail:
	for (j = 0; j < index; j++) {
		cmd.stream_type = stream_id[j];
		cmd.onoff = 0;
		sub_system_stream_ctl(&cmd);
	}

	return -1;
}

void vendor_device_stream_tsk(void *param)
{
	struct stream_ctl_tsk *p_tsk = (struct stream_ctl_tsk *)param;
	int ret;

	while (!p_tsk->exit_tsk) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (p_tsk->exit_tsk)
			return;

		FRTOS_mutex_lock(mutex);

		while (p_tsk->ctl.r_index != p_tsk->ctl.w_index) {
			ret = vendor_device_handle_stream_ctl(&p_tsk->ctl.cmd[p_tsk->ctl.r_index]);
			if (ret) {
				p_tsk->ctl.r_index = p_tsk->ctl.w_index;
				break;
			} else {
				p_tsk->ctl.r_index++;
				p_tsk->ctl.r_index = p_tsk->ctl.r_index % MAX_STREAM_CTL_ITEM;
			}
		}

		FRTOS_mutex_unlock(mutex);
	}
	vTaskDelete(NULL);
}

static int vendor_device_stream_ctl(struct uvc_request_data *req, unsigned char on)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct stream_ctl_tsk *p_tsk = &imi_stream_tsk;
	int ret = IMI_PROTOCOL_OK;

	if (req == NULL)
		return -EINVAL;

	memcpy(&p_tsk->ctl.cmd[p_tsk->ctl.w_index], &req->data[VENDOR_CMD_HEADER_LEN], sizeof(struct imi_stream_ctl_cmd));
	p_tsk->ctl.cmd[p_tsk->ctl.w_index].onoff = on;
	p_tsk->ctl.w_index++;
	p_tsk->ctl.w_index = p_tsk->ctl.w_index % MAX_STREAM_CTL_ITEM;

	xTaskNotifyGive(p_tsk->cmd_thd);

	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_open_stream(struct uvc_request_data *req)
{
	return vendor_device_stream_ctl(req, 1);
}

static int vendor_device_close_stream(struct uvc_request_data *req)
{
	return vendor_device_stream_ctl(req, 0);
}

static int vendor_device_reset(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;

	if (req == NULL)
		return -EINVAL;

	vendor_device_create_header(&rsp_head, ptr, 2, IMI_PROTOCOL_OK);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_usb_disconnect(struct uvc_request_data *req)
{
	struct imi_vendor_sensor_cmd sensor_cmd;
	int stream_id;

	if (req == NULL)
		return -EINVAL;

	sensor_cmd.cmd_type = VENDOR_CMD_USB_DISCONNECT;

	FRTOS_mutex_lock(mutex);

	for (stream_id = VENDOR_STREAM_COLOR; stream_id < VENDOR_STREAM_DEVICE; stream_id++)
		sub_system_process_events(stream_id, &sensor_cmd);

	for (stream_id = VENDOR_STREAM_COLOR_USB; stream_id < VENDOR_STREAM_MAX; stream_id++)
		sub_system_process_events(stream_id, &sensor_cmd);

	FRTOS_mutex_unlock(mutex);

	return 0;
}

static int vendor_device_get_log(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct vendor_file_info file_data;
	int ret = IMI_PROTOCOL_OK;

#define LOG_IN_TRANSFER 0x08
#define LOG_TRANSFER_OVER 0x00

	if (req == NULL)
		return -EINVAL;

	memset((void *)&file_data, 0x00, sizeof(struct vendor_file_info));
	file_data.type =  *((unsigned char *)&req->data[VENDOR_CMD_HEADER_LEN]);

	ret = vendor_misc_handle_cmd(ptr->cmd, &file_data); //return the rest data size
	if (ret > 0) {
		if (file_data.offset + ret >= file_data.len) {
			vendor_device_create_header(&rsp_head, ptr, ret + 2, LOG_TRANSFER_OVER);
			vendor_device_fill_rsp(&rsp_head, file_data.data + file_data.offset, ret);
			vPortFree(file_data.data); //vPortFree the data we pvPortMalloced in vendor_misc.c
		} else {
			vendor_device_create_header(&rsp_head, ptr, ret + 2, LOG_IN_TRANSFER);
			vendor_device_fill_rsp(&rsp_head, file_data.data + file_data.offset, ret);
		}

		return 0;
	} else {
		if (file_data.data) {
			vPortFree(file_data.data);
		}
		ret = IMI_PROTOCOL_FAILURE;
	}

	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_get_cmos_reg(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct imi_cmos_reg_info *info = (struct imi_cmos_reg_info *)&req->data[VENDOR_CMD_HEADER_LEN];
	struct imi_cmos_reg_info rsp_info;
	struct imi_vendor_sensor_cmd sensor_cmd;
	unsigned int id_sensor;
	int ret;

	if (req == NULL)
		return -EINVAL;

	if (info->type == IMI_CMOS_TYPE_IMAGE) {
		id_sensor = VENDOR_STREAM_COLOR;
	} else if (info->type == IMI_CMOS_TYPE_DEPTH) {
		id_sensor = VENDOR_STREAM_DEPTH;
	} else {
		log_e("sensor type err\n");
		ret = IMI_PROTOCOL_REG_TYPE_ERR;
		goto end;
	}

	memset((void *)&sensor_cmd, 0x00, sizeof(struct imi_vendor_sensor_cmd));
	sensor_cmd.cmd_type = ptr->cmd;
	sensor_cmd.sensor_reg.addr = info->reg;

	ret = sub_system_process_events(id_sensor, &sensor_cmd);
	if (ret < 0)
		ret = IMI_PROTOCOL_FAILURE;

	rsp_info.type = info->type;
	rsp_info.reg = info->reg;
	rsp_info.val = sensor_cmd.sensor_reg.value;

end:
	vendor_device_create_header(&rsp_head, ptr, sizeof(rsp_info.val) + 2, ret);
	vendor_device_fill_rsp(&rsp_head, &rsp_info.val, sizeof(rsp_info.val));

	return 0;
}

static int vendor_device_set_cmos_reg(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct imi_cmos_reg_info *info = (struct imi_cmos_reg_info *)&req->data[VENDOR_CMD_HEADER_LEN];
	struct imi_vendor_sensor_cmd sensor_cmd;
	unsigned int id_sensor;
	int ret;

	if (req == NULL)
		return -EINVAL;

	if (info->type == IMI_CMOS_TYPE_IMAGE) {
		id_sensor = VENDOR_STREAM_COLOR;
	} else if (info->type == IMI_CMOS_TYPE_DEPTH) {
		id_sensor = VENDOR_STREAM_DEPTH;
	} else {
		log_e("sensor type err\n");
		ret = IMI_PROTOCOL_REG_TYPE_ERR;
		goto end;
	}

	memset((void *)&sensor_cmd, 0x00, sizeof(struct imi_vendor_sensor_cmd));
	sensor_cmd.cmd_type = ptr->cmd;
	sensor_cmd.sensor_reg.addr = info->reg;
	sensor_cmd.sensor_reg.value = info->val;

	ret = sub_system_process_events(id_sensor, &sensor_cmd);
	if (ret < 0)
		ret = IMI_PROTOCOL_FAILURE;

end:
	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_init_file_upload(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct vendor_file_info *info = (struct vendor_file_info *)&req->data[VENDOR_CMD_HEADER_LEN];
	int ret = IMI_PROTOCOL_OK;

	if (req == NULL)
		return -EINVAL;

	ret = vendor_misc_handle_cmd(ptr->cmd, info);
	if (ret < 0)
		ret = IMI_PROTOCOL_FAILURE;

	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_write_file_upload(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	unsigned char *buf = (unsigned char *)&req->data[VENDOR_CMD_HEADER_LEN];
	struct vendor_file_info file_data;
	int ret = IMI_PROTOCOL_OK;

	if (req == NULL)
		return -EINVAL;

//cmd(8)  buf(res)
	memset((void *)&file_data, 0x00, sizeof(struct vendor_file_info));
	file_data.len = ptr->data_len;
	file_data.data = buf;

	ret = vendor_misc_handle_cmd(ptr->cmd, &file_data);
	if (ret < 0)
		ret = IMI_PROTOCOL_FAILURE;

	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_finish_file_upload(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct vendor_file_info file_data;
	int ret = IMI_PROTOCOL_OK;

	if (req == NULL)
		return -EINVAL;

	memset((void *)&file_data, 0x00, sizeof(struct vendor_file_info));
	file_data.type =  *((unsigned short *)&req->data[VENDOR_CMD_HEADER_LEN]);
	file_data.crc =  *((unsigned short *)&req->data[VENDOR_CMD_HEADER_LEN + 2]);

	ret = vendor_misc_handle_cmd(ptr->cmd, &file_data);
	if (ret < 0)
		ret = IMI_PROTOCOL_FAILURE;

	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_download_file(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	struct vendor_file_info file_data;
	int ret = IMI_PROTOCOL_OK;

#define IN_TRANSFER 0x00
#define TRANSFER_OVER 0x00

	if (req == NULL)
		return -EINVAL;

	memset((void *)&file_data, 0x00, sizeof(struct vendor_file_info));
	memcpy(&file_data.type, &req->data[VENDOR_CMD_HEADER_LEN], sizeof(file_data.type));
	memcpy(&file_data.offset, &req->data[VENDOR_CMD_HEADER_LEN + 2], sizeof(file_data.offset));
	
	ret = vendor_misc_handle_cmd(ptr->cmd, &file_data); //return the rest data size
	if (ret > 0) {
		if (file_data.offset + ret >= file_data.len) {
			vendor_device_create_header(&rsp_head, ptr, ret + 2, TRANSFER_OVER);
			vendor_device_fill_rsp(&rsp_head, file_data.data + file_data.offset, ret);
			vPortFree(file_data.data); //vPortFree the data we pvPortMalloced in vendor_misc.c
		} else {
			vendor_device_create_header(&rsp_head, ptr, ret + 2, IN_TRANSFER);
			vendor_device_fill_rsp(&rsp_head, file_data.data + file_data.offset, ret);
		}
		return 0;
	} else {
		if (file_data.data) {
			vPortFree(file_data.data);
		}
		ret = IMI_PROTOCOL_FAILURE;
	}

	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_delete_file(struct uvc_request_data *req)
{
	struct imi_vendor_cmd_rsp_header rsp_head;
	struct imi_vendor_cmd_header *ptr = (struct imi_vendor_cmd_header *)req->data;
	unsigned short file_id =  *((unsigned short *)&req->data[VENDOR_CMD_HEADER_LEN]);
	int ret = 0;

	if (req == NULL)
		return -EINVAL;

	ret = vendor_misc_handle_cmd(ptr->cmd, &file_id);
	if (ret < 0)
		ret = IMI_PROTOCOL_FAILURE;

	vendor_device_create_header(&rsp_head, ptr, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return 0;
}

static int vendor_device_switch_source(struct uvc_request_data *req)
{
	if (req == NULL)
		return -EINVAL;

	log_e("%s\n", __FUNCTION__);

	return 0;
}

static int vendor_device_set_serial_num(struct uvc_request_data *req)
{
	if (req == NULL)
		return -EINVAL;

	log_e("%s\n", __FUNCTION__);

	return 0;
}

static int vendor_device_get_serial_num(struct uvc_request_data *req)
{
	if (req == NULL)
		return -EINVAL;

	log_e("%s\n", __FUNCTION__);

	return 0;
}

static int vendor_device_set_registration_param(struct uvc_request_data *req)
{
	if (req == NULL)
		return -EINVAL;

	log_e("%s\n", __FUNCTION__);

	return 0;
}

static int vendor_device_get_registration_param(struct uvc_request_data *req)
{
	if (req == NULL)
		return -EINVAL;

	log_e("%s\n", __FUNCTION__);

	return 0;
}

static const struct imi_vendor_cmd_handle imi_cmd_map[] = {
	{VENDOR_CMD_GET_VER,                vendor_device_get_ver               },
	{VENDOR_CMD_RESET,                  vendor_device_reset                 },
	{VENDOR_CMD_KEEP_ALIVE,             vendor_device_keep_alive            },
	{VENDOR_CMD_GET_PROPERTY,           vendor_device_get_property          },
	{VENDOR_CMD_SET_PROPERTY,           vendor_device_set_property          },
	{VENDOR_CMD_OPEN_STREAM,            vendor_device_open_stream           },
	{VENDOR_CMD_CLOSE_STREAM,           vendor_device_close_stream          },
	{VENDOR_CMD_SWTICH_SOURCE,          vendor_device_switch_source         },
	{VENDOR_CMD_GET_LOG,                vendor_device_get_log               },
	{VENDOR_CMD_GET_CMOS_REG,           vendor_device_get_cmos_reg          },
	{VENDOR_CMD_SET_CMOS_REG,           vendor_device_set_cmos_reg          },
	{VENDOR_CMD_INIT_FILE_UPLOAD,       vendor_device_init_file_upload      },
	{VENDOR_CMD_WRITE_FILE_UPLOAD,      vendor_device_write_file_upload     },
	{VENDOR_CMD_FINISH_FILE_UPLOAD,     vendor_device_finish_file_upload    },
	{VENDOR_CMD_DOWNLOAD_FILE,          vendor_device_download_file         },
	{VENDOR_CMD_DELETE_FILE,            vendor_device_delete_file           },
	{VENDOR_CMD_SET_SERIAL_NUM,         vendor_device_set_serial_num        },
	{VENDOR_CMD_GET_SERIAL_NUM,         vendor_device_get_serial_num        },
	{VENDOR_CMD_SET_REGISTRATION_PARAM, vendor_device_set_registration_param},
	{VENDOR_CMD_GET_REGISTRATION_PARAM, vendor_device_get_registration_param},
	{VENDOR_CMD_USB_DISCONNECT,         vendor_device_usb_disconnect        },
};

static int vendor_device_handle_data(struct uvc_request_data *resp)
{
	struct imi_vendor_cmd_header *head;
	struct imi_vendor_cmd_rsp_header rsp_head;
	int len, index, ret;
	unsigned short crc_code;

	if (resp == NULL)
		return -EINVAL;

	head = (struct imi_vendor_cmd_header *)(resp->data);

	//log_e("data_len = %x, cmd = %x\n", head->data_len, head->cmd);

	if (head->flag != HOST_TO_DEVICE) {
		log_e("check head fail flg:%x, cmd:%x\n", head->flag, head->cmd);
		ret = IMI_PROTOCOL_PARSE_CMD_ERR;
		goto end;
	}

	if (head->data_len != resp->length - VENDOR_CMD_HEADER_LEN) {
		log_e("check data len fail\n");
		ret = IMI_PROTOCOL_CMD_LENGH_ERR;
		goto end;
	}

	memcpy(&crc_code, &resp->data[resp->length - 2], 2);
	if (crc_code != vendor_misc_calculate_crc16(resp->data, resp->length - 2)) {
		//return -1;
	}

	len = sizeof(imi_cmd_map) / sizeof(imi_cmd_map[0]);

	for(index = 0; index < len; index++) {
		if (imi_cmd_map[index].cmd_type == head->cmd) {
			return imi_cmd_map[index].callback(resp);
		}
	}

	ret = IMI_PROTOCOL_CMD_NOT_SUPPORT;

end:
	vendor_device_create_header(&rsp_head, head, 2, ret);
	vendor_device_fill_rsp(&rsp_head, NULL, 0);

	return -1;
}

static void vendor_device_tsk_init(void)
{
	struct stream_ctl_tsk *p_tsk = &imi_stream_tsk;

	memset((void *)p_tsk, 0x00, sizeof(struct stream_ctl_tsk));

	xTaskCreate(&vendor_device_stream_tsk, "stream_ctl", IMI_DEV_STK_SIZE, (void *)p_tsk, TSK_PRIO_APP, &p_tsk->cmd_thd);
}

int imi_vendor_device_init(void)
{
	struct imi_vendor_param param = {0};
	int ret;

	FRTOS_mutex_create(&mutex);

	ret = vendor_misc_init();
	if (ret) {
		log_e("vendor_misc_init fail\n");
		return -EFAULT;
	}
	
	ret = sub_system_init(&param);
	if (ret) {
		log_e("sub_system_init init fail\n");
		goto fail1;
	}

	vendor_device_tsk_init();

	ret = imi_cmd_hal_init(&vendor_resp, vendor_device_handle_data);
	if (ret) {
		log_e("imi_cmd_hal_init fail\n");
		//goto fail2;
	}

	return 0;

//fail2:
//	sub_system_deinit();
fail1:
	vendor_misc_deinit();

	return -EFAULT;
}

void imi_vendor_device_deinit(void)
{
	struct stream_ctl_tsk *p_tsk = &imi_stream_tsk;

	imi_cmd_hal_deinit();

	p_tsk->exit_tsk = 1;

	sub_system_deinit();

	vendor_misc_deinit();

	FRTOS_mutex_free(&mutex);
}

