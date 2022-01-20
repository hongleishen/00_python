struct  test_struct {
	/* data */
	int a;
	int b;
};


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


struct musb_ep {
	/* stuff towards the head is basically write-once. */
	struct usb_ep           end_point;
	char                name[12];
	struct musb_hw_ep       *hw_ep;
	struct musb         *musb;
	u8              current_epnum;

	/* ... when enabled/disabled ... */
	u8              type;
	u8              is_in;
	u16             packet_sz;
	const struct usb_endpoint_descriptor    *desc;
	struct dma_channel      *dma;

	/* later things are modified based on usage */
	struct list_head        req_list;

	u8              wedged;

	/* true if lock must be dropped but req_list may not be advanced */
	u8              busy;

	u8              hb_mult;
};

struct dma_channel {
	void            *private_data;
	/* FIXME not void* private_data, but a dma_controller * */
	size_t          max_len;
	size_t          actual_len;
	enum dma_channel_status status;
	bool            desired_mode;
};