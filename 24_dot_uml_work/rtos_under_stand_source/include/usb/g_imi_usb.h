/* SPDX-License-Identifier: GPL-2.0+ */
/*
 *  Copyright (C) 2012 Samsung Electronics
 *  Lukasz Majewski <l.majewski@samsung.com>
 */

#ifndef __G_DOWNLOAD_H_
#define __G_DOWNLOAD_H_
#include <usb/linux/ch9.h>
#include <usb/linux/gadget.h>
#include <usb/linux/composite.h>

typedef int (*g_bind_callback_f)(struct usb_configuration *);

/* used in Gadget downloader callback linker list */
struct g_usb_bind_callback {
	const char *usb_function_name;
	g_bind_callback_f fptr;
};

#endif /* __G_DOWNLOAD_H_ */
