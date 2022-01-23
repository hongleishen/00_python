#include <usb/g_imi_usb.h>
#include <usb/linux/musb.h>
#include <errno.h>
#include <linux_compat.h>
#include "initcalls.h"

#define IMI_DEV_VERSION                 0x0001
#define CONFIG_USB_GADGET_MANUFACTURER  "hjimi"
#define CONFIG_USB_GADGET_VENDOR_NUM    0x2DBB
#define CONFIG_USB_GADGET_PRODUCT_NUM   0x0503
#define CONFIG_IMI_SERIALNO             "0001"
#define CONFIG_IMI_DEV_CLASS                0
#define CONFIG_IMI_DEV_SUBCLASS             0
#define STRING_CFG 2
#define STRING_SERIAL  3
#define MAX_STRING_SERIAL   20
#define CONFIGURATION_NUMBER 1

#define DRIVER_VERSION      "hjimi 1.0"

extern struct g_usb_bind_callback imi_1680_usb;

struct g_usb_bind_callback *usb_g_fun[] = {
	&imi_1680_usb,
};

static const char product[] = "imi_1680";
static char g_imi_usb_serial[MAX_STRING_SERIAL];
static const char manufacturer[] = CONFIG_USB_GADGET_MANUFACTURER;

void g_imi_usb_set_serialnumber(char *s)
{
	memset((u8 *)g_imi_usb_serial, 0, MAX_STRING_SERIAL);
	strncpy(g_imi_usb_serial, s, MAX_STRING_SERIAL - 1);
}

static struct usb_device_descriptor device_desc = {
	.bLength = sizeof(device_desc),
	.bDescriptorType = USB_DT_DEVICE,

	.bcdUSB = __constant_cpu_to_le16(0x0200),
	.bDeviceClass = CONFIG_IMI_DEV_CLASS,
	.bDeviceSubClass = CONFIG_IMI_DEV_SUBCLASS,

	.idVendor = __constant_cpu_to_le16(CONFIG_USB_GADGET_VENDOR_NUM),
	.idProduct = __constant_cpu_to_le16(CONFIG_USB_GADGET_PRODUCT_NUM),
	/* .iProduct = DYNAMIC */
	/* .iSerialNumber = DYNAMIC */
	.bNumConfigurations = 1,
};

/*
 * static strings, in UTF-8
 * IDs for those strings are assigned dynamically at g_imi_usb_bind()
 */
static struct usb_string g_imi_usb_string_defs[] = {
	{.s = manufacturer},
	{.s = product},
	{.s = g_imi_usb_serial},
	{ }     /* end of list */
};

static struct usb_gadget_strings g_imi_usb_string_tab = {
	.language = 0x0409, /* en-us */
	.strings = g_imi_usb_string_defs,
};

static struct usb_gadget_strings *g_imi_usb_composite_strings[] = {
	&g_imi_usb_string_tab,
	NULL,
};

static int g_imi_usb_unbind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;

	debug("%s: calling usb_gadget_disconnect for "
			"controller '%s'\n", __func__, gadget->name);
	usb_gadget_disconnect(gadget);

	return 0;
}

static int g_imi_usb_do_config(struct usb_configuration *c)
{
	int n, ret;

	debug("%s: configuration: 0x%p composite dev: 0x%p\n",
		  __func__, c, c->cdev);

	for (n = 0; n < sizeof(usb_g_fun) / sizeof(usb_g_fun[0]); n++) {
		if (usb_g_fun[n] && usb_g_fun[n]->fptr) {
			ret = usb_g_fun[n]->fptr(c);
			if (ret < 0) {
				imi_printf("%s fail, ret = %d\n", __func__, ret);
				return ret;
			}
		}
	}
	return 0;
}


static struct usb_configuration _imi_config;

static int g_imi_usb_config_register(struct usb_composite_dev *cdev)
{
	struct usb_configuration *config = &_imi_config;
	const char *name = "hjimi";

	memset((u8 *)config, 0, sizeof(*config));

	config->label = name;
	config->bmAttributes = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER;
	config->bConfigurationValue = CONFIGURATION_NUMBER;
	config->iConfiguration = STRING_CFG;

	return usb_add_config(cdev, config, g_imi_usb_do_config);
}

static int g_imi_usb_bind(struct usb_composite_dev *cdev)
{
	struct usb_gadget *gadget = cdev->gadget;
	int id, ret;

	debug("%s: gadget: 0x%p cdev: 0x%p\n", __func__, gadget, cdev);

	id = usb_string_id(cdev);

	if (id < 0)
		return id;

	g_imi_usb_string_defs[0].id = id;
	device_desc.iManufacturer = id;

	id = usb_string_id(cdev);
	if (id < 0)
		return id;

	g_imi_usb_string_defs[1].id = id;
	device_desc.iProduct = id;

	if (strlen(g_imi_usb_serial)) {
		id = usb_string_id(cdev);
		if (id < 0)
			return id;

		g_imi_usb_string_defs[2].id = id;
		device_desc.iSerialNumber = id;
	}

	ret = g_imi_usb_config_register(cdev);
	if (ret)
		goto error;

	device_desc.bcdDevice = __constant_cpu_to_le16(IMI_DEV_VERSION);


	debug("%s: calling usb_gadget_connect for "
			"controller '%s'\n", __func__, gadget->name);
	usb_gadget_connect(gadget);

	return 0;

 error:
	g_imi_usb_unbind(cdev);
	return -ENOMEM;
}

static struct usb_composite_driver g_imi_usb_driver = {
	.name = NULL,
	.dev = &device_desc,
	.strings = g_imi_usb_composite_strings,
	.max_speed = USB_SPEED_HIGH,

	.bind = g_imi_usb_bind,
	.unbind = g_imi_usb_unbind,
};

/*
 * NOTICE:
 * Registering via USB function name won't be necessary after rewriting
 * g_imi_usb to support multiple USB functions.
 */
int g_imi_usb_register(const char *name)
{
	int ret;

	debug("%s: g_imi_usb_driver.name = %s\n", __func__, name);
	g_imi_usb_driver.name = name;

	ret = usb_composite_register(&g_imi_usb_driver);
	if (ret) {
		imi_printf("%s: failed!, error: %d\n", __func__, ret);
		return ret;
	}
	return 0;
}

void USB_init(void)
{
	musb_init();
	g_imi_usb_register("usb");
}

#ifndef TARGET_FPGA_SIMULATION
DEV_INIT(USB_init);
#endif
