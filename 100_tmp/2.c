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


struct musb_fifo_cfg {
	u8          hw_ep_num;
	enum musb_fifo_style    style;
	enum musb_buf_mode  mode;
	u16         maxpacket;
};


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