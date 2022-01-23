/* SPDX-License-Identifier: GPL-2.0 */
/*
 * USB PHY defines
 *
 * These APIs may be used between USB controllers.  USB device drivers
 * (for either host or peripheral roles) don't use these calls; they
 * continue to use just usb_device and usb_gadget.
 */

#ifndef __LINUX_USB_PHY_H
#define __LINUX_USB_PHY_H
#include <linux_list.h>

enum usb_phy_interface {
	USBPHY_INTERFACE_MODE_UNKNOWN,
	USBPHY_INTERFACE_MODE_UTMI,
	USBPHY_INTERFACE_MODE_UTMIW,
	USBPHY_INTERFACE_MODE_ULPI,
	USBPHY_INTERFACE_MODE_SERIAL,
	USBPHY_INTERFACE_MODE_HSIC,
};

/* associate a type with PHY */
enum usb_phy_type {
	USB_PHY_TYPE_UNDEFINED,
	USB_PHY_TYPE_USB2,
	USB_PHY_TYPE_USB3,
};

/* OTG defines lots of enumeration states before device reset */
enum usb_otg_state {
	OTG_STATE_UNDEFINED = 0,
	/* single-role peripheral, and dual-role default-b */
	OTG_STATE_B_IDLE,
	OTG_STATE_B_PERIPHERAL,
	/* extra dual-role default-b states */
	OTG_STATE_B_WAIT_ACON,
	/* dual-role default-a */
};

struct usb_phy;

/* for phys connected thru an ULPI interface, the user must
 * provide access ops
 */
struct usb_phy_io_ops {
	int (*read)(struct usb_phy *x, u32 reg);
	int (*write)(struct usb_phy *x, u32 val, u32 reg);
};

struct usb_phy {
	const char      *label;
	unsigned int         flags;
	enum usb_otg_state  state;

	enum usb_phy_type   type;

	struct usb_phy_io_ops   *io_ops;
	void __iomem        *io_priv;

	/* to support controllers that have multiple phys */
	struct list_head    head;

	/* initialize/shutdown the phy */
	int (*init)(struct usb_phy *x);
	void    (*shutdown)(struct usb_phy *x);
};

static inline const char *usb_phy_type_string(enum usb_phy_type type)
{
	switch (type) {
	case USB_PHY_TYPE_USB2:
		return "USB2 PHY";
	case USB_PHY_TYPE_USB3:
		return "USB3 PHY";
	default:
		return "UNKNOWN PHY TYPE";
	}
}

#endif /* __LINUX_USB_PHY_H */
