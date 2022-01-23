#include "../include/vendor_device.h"
#include "../include/vendor_cmd.h"
#include "../include/vendor_sub_system.h"

#define LOG_TAG "vendor_sub_system"
#include "elog.h"

struct sub_system {
	struct sub_system_device *dev[VENDOR_SUB_SYSTEM_MAX];
	struct imi_device_handle *dev_handle;
};

extern struct sub_system_device imi_sub_system_usb;

static struct sub_system_device *imi_sub_system_devices[VENDOR_SUB_SYSTEM_MAX] = {
	NULL,
	NULL,
	&imi_sub_system_usb,
	NULL,
};

static struct sub_system imi_sub_system;
#define SUB_SYSTEM()	&imi_sub_system

static inline struct sub_system_device* sub_system_device_get(int stream_id)
{
	struct sub_system *sub = SUB_SYSTEM();
	struct sub_system_device *ptr = NULL;

	switch (stream_id) {
		case VENDOR_STREAM_COLOR:
			ptr = sub->dev[VENDOR_SUB_SYSTEM_ISP];
			break;

		case VENDOR_STREAM_DEPTH:
		case VENDOR_STREAM_IR:
			ptr = sub->dev[VENDOR_SUB_SYSTEM_DPU];
			break;

		case VENDOR_STREAM_BULK_OUT:
			ptr = sub->dev[VENDOR_SUB_SYSTEM_BULK_OUT];
			break;

		default:
			ptr = sub->dev[VENDOR_SUB_SYSTEM_USB];
			break;
	}

	return ptr;
}

int sub_system_stream_ctl(struct imi_stream_ctl_cmd *cmd)
{
	struct sub_system_device *sub_dev = sub_system_device_get(cmd->stream_type);
	int ret = 0;

	if (sub_dev && sub_dev->stream_ctl) {
		FRTOS_mutex_lock(sub_dev->mutex);
		ret = sub_dev->stream_ctl(cmd);
		FRTOS_mutex_unlock(sub_dev->mutex);
	}

	return ret;
}

int sub_system_process_events(int stream_id, struct imi_vendor_sensor_cmd *cmd)
{
	struct sub_system_device *sub_dev = sub_system_device_get(stream_id);
	int ret = -1;

	if (!sub_dev || !cmd)
		return -1;

	if (sub_dev && sub_dev->process_events) {
		FRTOS_mutex_lock(sub_dev->mutex);
		ret = sub_dev->process_events(stream_id, cmd);
		FRTOS_mutex_unlock(sub_dev->mutex);
	}

	return ret;
}

void sub_system_deinit(void)
{
	struct sub_system *sub = SUB_SYSTEM();
	int i;

	for (i = 0; i < VENDOR_SUB_SYSTEM_MAX; i++) {
		if (sub->dev[i] && sub->dev[i]->deinit) {
			FRTOS_mutex_free(&sub->dev[i]->mutex);
			sub->dev[i]->deinit();
			sub->dev[i] = NULL;
		}
	}

	sub->dev_handle = NULL;
}

int sub_system_handshake(void *pdata)
{
	struct sub_system *sub = SUB_SYSTEM();
	int i, ret;

	for (i = 0; i < VENDOR_SUB_SYSTEM_MAX; i++) {
		if (sub->dev[i] && sub->dev[i]->handshake) {
			FRTOS_mutex_lock(sub->dev[i]->mutex);
			ret = sub->dev[i]->handshake(pdata);
			FRTOS_mutex_unlock(sub->dev[i]->mutex);
		}

		if (ret) {
			log_e("sub %d handshake fail\n", i);
			return ret;
		}
	}

	return ret;
}

void sub_system_register_callback(enum imi_sub_system_type sub,
		struct sub_sys_callback *callback)
{
	struct sub_system *subsys = SUB_SYSTEM();

	if (sub >= VENDOR_SUB_SYSTEM_MAX) {
		log_e("sub index err\n");
		return;
	}

	if (subsys->dev[sub] && callback->send_response) {
		FRTOS_mutex_lock(subsys->dev[sub]->mutex);
		subsys->dev[sub]->send_response = callback->send_response;
		FRTOS_mutex_unlock(subsys->dev[sub]->mutex);
	}
}

int sub_system_handle_payment(enum imi_sub_system_type sub,
		char *packet, int packet_len)
{
	struct sub_system *subsys = SUB_SYSTEM();
	int ret = 0;

	if (sub >= VENDOR_SUB_SYSTEM_MAX) {
		log_e("sub index err\n");
		return -EINVAL;
	}

	if (subsys->dev[sub] && subsys->dev[sub]->handle_payment) {
		FRTOS_mutex_lock(subsys->dev[sub]->mutex);
		ret = subsys->dev[sub]->handle_payment(packet, packet_len);
		FRTOS_mutex_unlock(subsys->dev[sub]->mutex);
	}

	return ret;
}

int sub_system_init(void *dev_param)
{
	struct sub_system *sub = SUB_SYSTEM();
	struct sub_sys_param param;
//	struct imi_vendor_param *p_vendor_param = (struct imi_vendor_param *)dev_param;
	int i, ret;

	for (i = 0; i < VENDOR_SUB_SYSTEM_MAX; i++) {
		sub->dev[i] = imi_sub_system_devices[i];
		if (sub->dev[i] && sub->dev[i]->init) {
			FRTOS_mutex_create(&sub->dev[i]->mutex);
			ret = sub->dev[i]->init(&param);
			if (ret) {
				log_e("subsys %s init fail\n", sub->dev[i]->name);
				//goto err;
			}
		}
	}

	return 0;

#if 0
err:
	for (j = 0; j < i; j++) {
		if (sub->dev[j] && sub->dev[j]->deinit) {
			FRTOS_mutex_free(&sub->dev[i]->mutex);
			sub->dev[j]->deinit();
			sub->dev[j] = NULL;
		}
	}

	return -1;
#endif
}
