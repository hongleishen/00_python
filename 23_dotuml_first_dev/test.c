/*
 * This is used to for host and peripheral modes of the driver for
 * Inventra (Multidrop) Highspeed Dual-Role Controllers:  (M)HDRC.
 *
 * Board initialization should put one of these into dev->platform_data,
 * probably on some platform_device named "musb-hdrc".  It encapsulates
 * key configuration differences between boards.
 */

#ifndef __LINUX_USB_MUSB_H
#define __LINUX_USB_MUSB_H

/* The USB role is defined by the connector used on the board, so long as
 * standards are being followed.  (Developer boards sometimes won't.)
 */
enum musb_mode {
	MUSB_UNDEFINED = 0,
	MUSB_HOST,      /* A or Mini-A connector */
	MUSB_PERIPHERAL,    /* B or Mini-B connector */
	MUSB_OTG        /* Mini-AB connector */
};

enum musb_fifo_style {
	FIFO_RXTX,
	FIFO_TX,
	FIFO_RX
} __attribute__ ((packed));

enum musb_buf_mode {
	BUF_SINGLE,
	BUF_DOUBLE
} __attribute__ ((packed));

struct musb_fifo_cfg {
	u8          hw_ep_num;
	enum musb_fifo_style    style;
	enum musb_buf_mode  mode;
	u16         maxpacket;
};

#define MUSB_EP_FIFO(ep, st, m, pkt)        \
{                       \
	.hw_ep_num  = ep,           \
	.style      = st,           \
	.mode       = m,            \
	.maxpacket  = pkt,          \
}

#define MUSB_EP_FIFO_SINGLE(ep, st, pkt)    \
	MUSB_EP_FIFO(ep, st, BUF_SINGLE, pkt)

#define MUSB_EP_FIFO_DOUBLE(ep, st, pkt)    \
	MUSB_EP_FIFO(ep, st, BUF_DOUBLE, pkt)

struct musb_hdrc_eps_bits {
	const char  name[16];
	u8      bits;
};

struct musb_hdrc_config {
	struct musb_fifo_cfg    *fifo_cfg;  /* board fifo configuration */
	unsigned        fifo_cfg_size;  /* size of the fifo configuration */

	/* MUSB configuration-specific details */
	unsigned    multipoint:1;   /* multipoint device */
	unsigned    dyn_fifo:1; /* supports dynamic fifo sizing */
	unsigned    soft_con:1; /* soft connect required */
	unsigned    utm_16:1; /* utm data witdh is 16 bits */
	unsigned    big_endian:1;   /* true if CPU uses big-endian */
	unsigned    mult_bulk_tx:1; /* Tx ep required for multbulk pkts */
	unsigned    mult_bulk_rx:1; /* Rx ep required for multbulk pkts */
	unsigned    high_iso_tx:1;  /* Tx ep required for HB iso */
	unsigned    high_iso_rx:1;  /* Rx ep required for HD iso */
	unsigned    dma:1; /* supports DMA */
	unsigned    vendor_req:1; /* vendor registers required */

	u8      num_eps;    /* number of endpoints _with_ ep0 */
	u8      dma_channels; /* number of dma channels */
	u8      dyn_fifo_size;  /* dynamic size in bytes */
	u8      vendor_ctrl; /* vendor control reg width */
	u8      vendor_stat; /* vendor status reg witdh */
	u8      dma_req_chan; /* bitmask for required dma channels */
	u8      ram_bits;   /* ram address size */

	struct musb_hdrc_eps_bits *eps_bits;
};

struct musb_hdrc_platform_data {
	/* MUSB_HOST, MUSB_PERIPHERAL, or MUSB_OTG */
	u8      mode;

	/* for clk_get() */
	const char  *clock;

	/* (HOST or OTG) switch VBUS on/off */
	int     (*set_vbus)(struct device *dev, int is_on);

	/* (PERIPHERAL) mA/2 max power consumed (default = 100mA) */
	u8      min_power;

	/* Power the device on or off */
	int     (*set_power)(int state);

	/* MUSB configuration-specific details */
	struct musb_hdrc_config *config;

	/* Architecture specific board data */
	void        *board_data;

	/* Platform specific struct musb_ops pointer */
	const void  *platform_ops;
};

int musb_init(void);

#endif /* __LINUX_USB_MUSB_H */




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
#define can_bulk_split(musb,type) \
	(((type) == USB_ENDPOINT_XFER_BULK) && (musb)->bulk_split)

	unsigned        bulk_combine:1;
#define can_bulk_combine(musb,type) \
	(((type) == USB_ENDPOINT_XFER_BULK) && (musb)->bulk_combine)

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



struct usb_gadget_driver {
	char            *function;
	enum usb_device_speed   max_speed;
	int         (*bind)(struct usb_gadget *gadget,
					struct usb_gadget_driver *driver);
	void            (*unbind)(struct usb_gadget *);
	int         (*setup)(struct usb_gadget *,
					const struct usb_ctrlrequest *);
	void            (*disconnect)(struct usb_gadget *);
	void            (*suspend)(struct usb_gadget *);
	void            (*resume)(struct usb_gadget *);
	void            (*reset)(struct usb_gadget *);
	char            *udc_name;
	struct list_head    pending;
	unsigned                match_existing_only:1;
};


struct musb_fifo_cfg {
	u8          hw_ep_num;
	enum musb_fifo_style    style;
	enum musb_buf_mode  mode;
	u16         maxpacket;
};