#ifndef __VENDOR_DEVICE_H__
#define __VENDOR_DEVICE_H__

#include <usb/linux/gadget.h>
#include "projdefs.h"

enum usb_cmd_stage {
	USB_CMD_SETUP = 0,
	USB_CMD_DATA,
};

#define VENDOR_CMD_RSP_TOTAL_LEN sizeof(struct uvc_request_data)

struct imi_vendor_cmd {
	enum usb_cmd_stage cmd_stage;
	struct usb_ctrlrequest *usbReq;
};

struct imi_vendor_cmd_handle {
	int cmd_type;
	int (*callback)(struct uvc_request_data *req);
};

struct imi_vendor_param {
	int isp_fmt_to_dsp;
};

int imi_vendor_device_init(void);
void imi_vendor_device_deinit(void);

int imi_cmd_hal_init(struct uvc_request_data *cmd_ret, int (*data_handle)(struct uvc_request_data *));
void imi_cmd_hal_deinit(void);

#endif
