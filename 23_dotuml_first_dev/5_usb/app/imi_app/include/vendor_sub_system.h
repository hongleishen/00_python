#ifndef __VENDOR_SUB_SYSTEM_H__
#define __VENDOR_SUB_SYSTEM_H__

#include <errno.h>
#include <linux_compat.h>
#include <imi_dev.h>

enum imi_sub_system_type {
	VENDOR_SUB_SYSTEM_ISP = 0,
	VENDOR_SUB_SYSTEM_DPU,
	VENDOR_SUB_SYSTEM_USB,
	VENDOR_SUB_SYSTEM_BULK_OUT,
	VENDOR_SUB_SYSTEM_MAX,
};

struct sub_sys_param {
	struct imi_device_handle *binder_handle;
};

struct sub_sys_callback {
	int (*send_response)(char *buf, int len);
};

struct sub_system_device {
	char *name;
	FRTOS_mutex mutex;
	int (*init)(struct sub_sys_param *param);
	void (*deinit)(void);
	int (*stream_ctl)(struct imi_stream_ctl_cmd *cmd);
	int (*process_events)(int stream_id, struct imi_vendor_sensor_cmd *cmd);
	int (*handshake)(void *pdata);
	int (*parameter_init)(int id, void *pdata);
	int (*send_response)(char *buf, int len);
	int (*handle_payment)(char *packet, int packet_len);
};

void sub_system_register_callback(enum imi_sub_system_type sub, struct sub_sys_callback *callback);
int sub_system_stream_ctl(struct imi_stream_ctl_cmd *cmd);
int sub_system_process_events(int stream_id, struct imi_vendor_sensor_cmd *cmd);
int sub_system_handshake(void *);
int sub_system_handle_payment(enum imi_sub_system_type sub, char *packet, int packet_len);

int sub_system_init(void *dev_param);
void sub_system_deinit(void);

#endif
