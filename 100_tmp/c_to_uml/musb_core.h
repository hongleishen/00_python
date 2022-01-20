/* SPDX-License-Identifier: GPL-2.0 */
/*
 * MUSB OTG driver defines
 *
 * Copyright 2005 Mentor Graphics Corporation
 * Copyright (C) 2005-2006 by Texas Instruments
 * Copyright (C) 2006-2007 Nokia Corporation
 */

#ifndef __MUSB_CORE_H__
#define __MUSB_CORE_H__
#include <errno.h>
#include <usb/linux/ch9.h>
#include <usb/linux/gadget.h>
#include <usb/linux/musb.h>
#include "imi_type.h"
#include "imi_io.h"
#include "vic_int.h"
struct musb;
struct musb_hw_ep;
struct musb_ep;

/* Helper defines for struct musb->hwvers */
#define MUSB_HWVERS_MAJOR(x)    ((x >> 10) & 0x1f)
#define MUSB_HWVERS_MINOR(x)    (x & 0x3ff)
#define MUSB_HWVERS_RC      0x8000
#define MUSB_HWVERS_1300    0x52C
#define MUSB_HWVERS_1400    0x590
#define MUSB_HWVERS_1800    0x720
#define MUSB_HWVERS_1900    0x784
#define MUSB_HWVERS_2000    0x800

#include "musb_dma.h"
#include "musb_io.h"
#include "musb_regs.h"
#include "musb_gadget.h"

#define is_peripheral_enabled(musb) (1)
#define is_host_enabled(musb)       (0)
#define is_otg_enabled(musb)        (0)

/* NOTE:  otg and peripheral-only state machines start at B_IDLE.
 * OTG or host-only go to A_IDLE when ID is sensed.
 */
#define is_peripheral_active(m)     (1)
#define is_host_active(m)       (0)


/****************************** PERIPHERAL ROLE *****************************/

#define is_peripheral_capable() (1)

extern irqreturn_t musb_g_ep0_irq(struct musb *);
extern void musb_g_tx(struct musb *, u8);
extern void musb_g_rx(struct musb *, u8);
extern void musb_g_reset(struct musb *);
extern void musb_g_suspend(struct musb *);
extern void musb_g_disconnect(struct musb *);

/****************************** CONSTANTS ********************************/

#ifndef MUSB_C_NUM_EPS
#define MUSB_C_NUM_EPS ((u8)16)
#endif

#ifndef MUSB_MAX_END0_PACKET
#define MUSB_MAX_END0_PACKET ((u16)MUSB_EP0_FIFOSIZE)
#endif

/* host side ep0 states */
enum musb_h_ep0_state {
	MUSB_EP0_IDLE,
	MUSB_EP0_START,         /* expect ack of setup */
	MUSB_EP0_IN,            /* expect IN DATA */
	MUSB_EP0_OUT,           /* expect ack of OUT DATA */
	MUSB_EP0_STATUS,        /* expect ack of STATUS */
} __attribute__ ((packed));

/* peripheral side ep0 states */
enum musb_g_ep0_state {
	MUSB_EP0_STAGE_IDLE,        /* idle, waiting for SETUP */
	MUSB_EP0_STAGE_SETUP,       /* received SETUP */
	MUSB_EP0_STAGE_TX,      /* IN data */
	MUSB_EP0_STAGE_RX,      /* OUT data */
	MUSB_EP0_STAGE_STATUSIN,    /* (after OUT data) */
	MUSB_EP0_STAGE_STATUSOUT,   /* (after IN data) */
	MUSB_EP0_STAGE_ACKWAIT,     /* after zlp, before statusin */
} __attribute__ ((packed));

/*************************** REGISTER ACCESS ********************************/

/* Endpoint registers (other than dynfifo setup) can be accessed either
 * directly with the "flat" model, or after setting up an index register.
 */

#define musb_ep_select(_mbase, _epnum) \
	musb_writeb((_mbase), MUSB_INDEX, (_epnum))
#define MUSB_EP_OFFSET          MUSB_INDEXED_OFFSET

/******************************** TYPES *************************************/

/**
 * struct musb_platform_ops - Operations passed to musb_core by HW glue layer
 * @init:   turns on clocks, sets up platform-specific registers, etc
 * @exit:   undoes @init
 * @set_mode:   forcefully changes operating mode
 * @try_ilde:   tries to idle the IP
 * @vbus_status: returns vbus status if possible
 * @set_vbus:   forces vbus status
 * @adjust_channel_params: pre check for standard dma channel_program func
 * @pre_root_reset_end: called before the root usb port reset flag gets cleared
 * @post_root_reset_end: called after the root usb port reset flag gets cleared
 */
struct musb_platform_ops {
	int (*init)(struct musb *musb);
	int (*exit)(struct musb *musb);
	int (*enable)(struct musb *musb);
	void    (*disable)(struct musb *musb);
	int (*set_mode)(struct musb *musb, u8 mode);

	int (*vbus_status)(struct musb *musb);
	void    (*set_vbus)(struct musb *musb, int on);

	int (*adjust_channel_params)(int a);
	void    (*pre_root_reset_end)(struct musb *musb);
	void    (*post_root_reset_end)(struct musb *musb);
};

/*
 * struct musb_hw_ep - endpoint hardware (bidirectional)
 *
 * Ordered slightly for better cacheline locality.
 */
struct musb_hw_ep {
	struct musb     *musb;
	void __iomem        *fifo;
	void __iomem        *regs;

	/* index in musb->endpoints[]  */
	u8          epnum;

	/* hardware configuration, possibly dynamic */
	bool            is_shared_fifo;
	bool            tx_double_buffered;
	bool            rx_double_buffered;
	u16         max_packet_sz_tx;
	u16         max_packet_sz_rx;

	struct dma_channel  *tx_channel;
	struct dma_channel  *rx_channel;
	void __iomem        *target_regs;

	u8          rx_reinit;
	u8          tx_reinit;

	/* peripheral side */
	struct musb_ep      ep_in;          /* TX */
	struct musb_ep      ep_out;         /* RX */
};

static inline struct musb_request *next_in_request(struct musb_hw_ep *hw_ep)
{
	return next_request(&hw_ep->ep_in);
}

static inline struct musb_request *next_out_request(struct musb_hw_ep *hw_ep)
{
	return next_request(&hw_ep->ep_out);
}

struct musb_csr_regs {
	/* FIFO registers */
	u16 txmaxp, txcsr, rxmaxp, rxcsr;
	u16 rxfifoadd, txfifoadd;
	u8 txtype, txinterval, rxtype, rxinterval;
	u8 rxfifosz, txfifosz;
	u8 txfunaddr, txhubaddr, txhubport;
	u8 rxfunaddr, rxhubaddr, rxhubport;
};

struct musb_context_registers {

	u8 power;
	u16 intrtxe, intrrxe;
	u8 intrusbe;
	u16 frame;
	u8 index, testmode;

	u8 devctl, busctl, misc;
	u32 otg_interfsel;

	struct musb_csr_regs index_regs[MUSB_C_NUM_EPS];
};

/*
 * struct musb - Driver instance data.
 */
struct musb {
	/* device lock */
	spinlock_t      lock;
	struct musb_io io;

	const struct musb_platform_ops *ops;
	struct musb_context_registers context;

	irqreturn_t     (*isr)(int, void *);
	u16         hwvers;

/* this hub status bit is reserved by USB 2.0 and not seen by usbcore */
#define MUSB_PORT_STAT_RESUME   (1 << 31)

	u32         port1_status;

	unsigned long       rh_timer;

	enum musb_h_ep0_state   ep0_stage;

	/* bulk traffic normally dedicates endpoint hardware, and each
	 * direction has its own ring of host side endpoints.
	 * we try to progress the transfer at the head of each endpoint's
	 * queue until it completes or NAKs too much; then we try the next
	 * endpoint.
	 */
	struct musb_hw_ep   *bulk_ep;

	struct list_head    control;    /* of musb_qh */
	struct list_head    in_bulk;    /* of musb_qh */
	struct list_head    out_bulk;   /* of musb_qh */

	struct dma_controller   *dma_controller;

	struct device       *controller;
	void __iomem        *ctrl_base;
	void __iomem        *mregs;

	/* passed down from chip/board specific irq handlers */
	u8          int_usb;
	u16         int_rx;
	u16         int_tx;

	unsigned char state;

	int nIrq;
	unsigned        irq_wake:1;

	struct musb_hw_ep    endpoints[MUSB_C_NUM_EPS];
#define control_ep      endpoints

#define VBUSERR_RETRY_COUNT 3
	u16         vbuserr_retry;
	u16 epmask;
	u8 nr_endpoints;

	u8 board_mode;      /* enum musb_mode */
	int         (*board_set_power)(int state);

	u8          min_power;  /* vbus for periph, in mA/2 */

	bool            is_host;

	int         a_wait_bcon;    /* VBUS timeout in msecs */
	unsigned long       idle_timeout;   /* Next timeout in jiffies */

	/* active means connected and not suspended */
	unsigned        is_active:1;

	unsigned is_multipoint:1;
	unsigned ignore_disconnect:1;   /* during bus resets */

	unsigned        hb_iso_rx:1;    /* high bandwidth iso rx? */
	unsigned        hb_iso_tx:1;    /* high bandwidth iso tx? */
	unsigned        dyn_fifo:1; /* dynamic FIFO supported? */

	unsigned        bulk_split:1;


	/* is_suspended means USB B_PERIPHERAL suspend */
	unsigned        is_suspended:1;

	/* may_wakeup means remote wakeup is enabled */
	unsigned        may_wakeup:1;

	/* is_self_powered is reported in device status and the
	 * config descriptor.  is_bus_powered means B_PERIPHERAL
	 * draws some VBUS current; both can be true.
	 */
	unsigned        is_self_powered:1;
	unsigned        is_bus_powered:1;

	unsigned        set_address:1;
	unsigned        test_mode:1;
	unsigned        softconnect:1;

	u8          address;
	u8          test_mode_nr;
	u16         ackpend;        /* ep0 */
	enum musb_g_ep0_state   ep0_state;
	struct usb_gadget   g;          /* the gadget */
	struct usb_gadget_driver *gadget_driver;    /* its driver */

	/*
	 * FIXME: Remove this flag.
	 *
	 * This is only added to allow Blackfin to work
	 * with current driver. For some unknown reason
	 * Blackfin doesn't work with double buffering
	 * and that's enabled by default.
	 *
	 * We added this flag to forcefully disable double
	 * buffering until we get it working.
	 */
	unsigned                double_buffer_not_ok:1;

	struct musb_hdrc_config *config;
};

static inline struct musb *gadget_to_musb(struct usb_gadget *g)
{
	return container_of(g, struct musb, g);
}

static inline int musb_read_fifosize(struct musb *musb,
		struct musb_hw_ep *hw_ep, u8 epnum)
{
	void *mbase = musb->mregs;
	u8 reg = 0;

	/* read from core using indexed model */
	reg = musb_readb(mbase, MUSB_EP_OFFSET(epnum, MUSB_FIFOSIZE));
	/* 0's returned when no more endpoints */
	if (!reg)
		return -ENODEV;

	musb->nr_endpoints++;
	musb->epmask |= (1 << epnum);

	hw_ep->max_packet_sz_tx = 1 << (reg & 0x0f);

	/* shared TX/RX FIFO? */
	if ((reg & 0xf0) == 0xf0) {
		hw_ep->max_packet_sz_rx = hw_ep->max_packet_sz_tx;
		hw_ep->is_shared_fifo = true;
		return 0;
	} else {
		hw_ep->max_packet_sz_rx = 1 << ((reg & 0xf0) >> 4);
		hw_ep->is_shared_fifo = false;
	}

	return 0;
}

static inline void musb_configure_ep0(struct musb *musb)
{
	musb->endpoints[0].max_packet_sz_tx = MUSB_EP0_FIFOSIZE;
	musb->endpoints[0].max_packet_sz_rx = MUSB_EP0_FIFOSIZE;
	musb->endpoints[0].is_shared_fifo = true;
}

/***************************** Glue it together *****************************/

extern const char musb_driver_name[];
extern int musb_start(struct musb *musb);
extern void musb_stop(struct musb *musb);

extern void musb_write_fifo(struct musb_hw_ep *ep, u16 len, const u8 *src);
extern void musb_read_fifo(struct musb_hw_ep *ep, u16 len, u8 *dst);

extern void musb_load_testpacket(struct musb *);

extern irqreturn_t musb_interrupt(struct musb *);

static inline void musb_platform_set_vbus(struct musb *musb, int is_on)
{
	if (!musb->ops || !musb->ops->set_vbus)
		return;

	return  musb->ops->set_vbus(musb, is_on);
}

static inline int musb_platform_enable(struct musb *musb)
{
	if (!musb->ops || !musb->ops->enable)
		return 0;

	return musb->ops->enable(musb);
}

static inline void musb_platform_disable(struct musb *musb)
{
	if (!musb->ops || !musb->ops->disable)
		return;

	return  musb->ops->disable(musb);
}

static inline int musb_platform_set_mode(struct musb *musb, u8 mode)
{
	if (!musb->ops || !musb->ops->set_mode)
		return 0;

	return musb->ops->set_mode(musb, mode);
}

static inline int musb_platform_get_vbus_status(struct musb *musb)
{
	if (!musb->ops || !musb->ops->vbus_status)
		return 0;

	return musb->ops->vbus_status(musb);
}

static inline int musb_platform_init(struct musb *musb)
{
	if (!musb->ops || !musb->ops->init)
		return 0;

	return musb->ops->init(musb);
}

static inline int musb_platform_exit(struct musb *musb)
{
	if (!musb->ops || !musb->ops->exit)
		return 0;

	return musb->ops->exit(musb);
}

int musb_init_controller(struct device *dev, int nIrq, void __iomem *ctrl, struct musb **musbp);

#endif  /* __MUSB_CORE_H__ */
