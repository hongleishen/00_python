#include <usb/linux/ch9.h>
#include <usb/linux/gadget.h>
#include <usb/linux/musb.h>
#include <usb/linux/phy.h>
#include <usb/linux/composite.h>

#include "linux_compat.h"

#include "musb_core.h"
#include "musbhsdma.h"

#define TA_WAIT_BCON(m) max_t(int, (m)->a_wait_bcon, OTG_TIME_A_WAIT_BCON)

#define MUSB_VERSION "6.0"

#define DRIVER_INFO DRIVER_DESC ", v" MUSB_VERSION

#define MUSB_DRIVER_NAME "musb-hdrc"
const char musb_driver_name[] = MUSB_DRIVER_NAME;

/*-------------------------------------------------------------------------*/
static void musb_indexed_ep_select(void __iomem *mbase, u8 epnum)
{
	musb_writeb(mbase, MUSB_INDEX, epnum);
}

static u32 musb_indexed_ep_offset(u8 epnum, u16 offset)
{
	return 0x10 + offset;
}

static u32 musb_default_fifo_offset(u8 epnum)
{
	return 0x20 + (epnum * 4);
}


#if !defined(CONFIG_USB_MUSB_TUSB6010)

/*
 * Load an endpoint's FIFO
 */
void musb_write_fifo(struct musb_hw_ep *hw_ep, u16 len, const u8 *src)
{
	struct musb *musb = hw_ep->musb;
	void __iomem *fifo = hw_ep->fifo;

	(void)musb;

	prefetch((u8 *)src);

	dev_dbg(musb->controller, "%cX ep%d fifo %p count %d buf %p\n",
			'T', hw_ep->epnum, fifo, len, src);

	/* we can't assume unaligned reads work */
	if (likely((0x01 & (unsigned long) src) == 0)) {
		u16 index = 0;

		/* best case is 32bit-aligned source address */
		if ((0x02 & (unsigned long) src) == 0) {
			if (len >= 4) {
				writesl(fifo, src + index, len >> 2);
				index += len & ~0x03;
			}
			if (len & 0x02) {
				musb_writew(fifo, 0, *(u16 *)&src[index]);
				index += 2;
			}
		} else {
			if (len >= 2) {
				writesw(fifo, src + index, len >> 1);
				index += len & ~0x01;
			}
		}
		if (len & 0x01)
			musb_writeb(fifo, 0, src[index]);
	} else  {
		/* byte aligned */
		writesb(fifo, src, len);
	}
}

#if !defined(CONFIG_USB_MUSB_AM35X) && !defined(CONFIG_USB_MUSB_PIC32)
/*
 * Unload an endpoint's FIFO
 */
void musb_read_fifo(struct musb_hw_ep *hw_ep, u16 len, u8 *dst)
{
	void __iomem *fifo = hw_ep->fifo;

	dev_dbg(NULL, "%cX ep%d fifo %p count %d buf %p\n",
			'R', hw_ep->epnum, fifo, len, dst);

	/* we can't assume unaligned writes work */
	if (likely((0x01 & (unsigned long) dst) == 0)) {
		u16 index = 0;

		/* best case is 32bit-aligned destination address */
		if ((0x02 & (unsigned long) dst) == 0) {
			if (len >= 4) {
				readsl(fifo, dst, len >> 2);
				index = len & ~0x03;
			}
			if (len & 0x02) {
				*(u16 *)&dst[index] = musb_readw(fifo, 0);
				index += 2;
			}
		} else {
			if (len >= 2) {
				readsw(fifo, dst, len >> 1);
				index = len & ~0x01;
			}
		}
		if (len & 0x01)
			dst[index] = musb_readb(fifo, 0);
	} else  {
		/* byte aligned */
		readsb(fifo, dst, len);
	}
}
#endif

#endif  /* normal PIO */


/*-------------------------------------------------------------------------*/

/* for high speed test mode; see USB 2.0 spec 7.1.20 */
static const u8 musb_test_packet[53] = {
	/* implicit SYNC then DATA0 to start */

	/* JKJKJKJK x9 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/* JJKKJJKK x8 */
	0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
	/* JJJJKKKK x8 */
	0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
	/* JJJJJJJKKKKKKK x8 */
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	/* JJJJJJJK x8 */
	0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd,
	/* JKKKKKKK x10, JK */
	0xfc, 0x7e, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0x7e

	/* implicit CRC16 then EOP to end */
};

void musb_load_testpacket(struct musb *musb)
{
	void __iomem    *regs = musb->endpoints[0].regs;

	musb_ep_select(musb->mregs, 0);
	musb_write_fifo(musb->control_ep,
			sizeof(musb_test_packet), musb_test_packet);
	musb_writew(regs, MUSB_CSR0, MUSB_CSR0_TXPKTRDY);
}

__attribute__((unused)) static char * otg_state_string(unsigned char state)
{
	static char *buf = NULL;
	switch(state)
	{
		case	OTG_STATE_UNDEFINED:
			buf = "undefined";
			break;

		case	OTG_STATE_B_IDLE:
			buf = "b_idle";
			break;

		case 	OTG_STATE_B_PERIPHERAL:
			buf = "b_peripheral";
			break;

		case 	OTG_STATE_B_WAIT_ACON:
			buf = "b_wait_acon";
			break;

		default:
			imi_printf("state: %d\n", state);
			buf = "null";
			break;
	}

	return buf;
}

/*
 * Interrupt Service Routine to record USB "global" interrupts.
 * Since these do not happen often and signify things of
 * paramount importance, it seems OK to check them individually;
 * the order of the tests is specified in the manual
 *
 * @param musb instance pointer
 * @param int_usb register contents
 * @param devctl
 * @param power
 */

static irqreturn_t musb_stage0_irq(struct musb *musb, u8 int_usb,
				u8 devctl, u8 power)
{
	irqreturn_t handled = IRQ_HANDLED;
#if 0
	dev_dbg(musb->controller, "<== Power=%02x, DevCtl=%02x, int_usb=0x%x\n", power, devctl,
		int_usb);
#endif
	/* in host mode, the peripheral may issue remote wakeup.
	 * in peripheral mode, the host may resume the link.
	 * spurious RESUME irqs happen too, paired with SUSPEND.
	 */
	if (int_usb & MUSB_INTR_RESUME) {
		handled = IRQ_HANDLED;
		dev_dbg(musb->controller, "RESUME (%s)\n", otg_state_string(musb->state));

		if (devctl & MUSB_DEVCTL_HM) {
			//host mode
		} else {
			switch (musb->state) {
			case OTG_STATE_B_PERIPHERAL:
				/* disconnect while suspended?  we may
				 * not get a disconnect irq...
				 */
				if ((devctl & MUSB_DEVCTL_VBUS)
						!= (3 << MUSB_DEVCTL_VBUS_SHIFT)
						) {
					musb->int_usb |= MUSB_INTR_DISCONNECT;
					musb->int_usb &= ~MUSB_INTR_SUSPEND;
					break;
				}
				break;
			case OTG_STATE_B_IDLE:
				musb->int_usb &= ~MUSB_INTR_SUSPEND;
				break;
			default:
				dev_warn(musb->controller,"bogus %s RESUME (%s)\n",
					"peripheral",
					otg_state_string(musb->state));
			}
		}
	}

	if (int_usb & MUSB_INTR_SUSPEND) {
		dev_dbg(musb->controller, "SUSPEND (%s) devctl %02x power %02x\n",
			otg_state_string(musb->state), devctl, power);
		handled = IRQ_HANDLED;

		switch (musb->state) {
		case OTG_STATE_B_IDLE:
			if (!musb->is_active)
				break;
		case OTG_STATE_B_PERIPHERAL:
			//musb_g_suspend(musb);
			musb->is_active = 0;
			break;

		default:
			/* "should not happen" */
			musb->is_active = 0;
			break;
		}
	}




	if ((int_usb & MUSB_INTR_DISCONNECT) && !musb->ignore_disconnect) {
		dev_dbg(musb->controller, "DISCONNECT (%s), devctl %02x\n",
				otg_state_string(musb->state),
				 devctl);
		handled = IRQ_HANDLED;

		switch (musb->state) {
		case OTG_STATE_B_PERIPHERAL:
		case OTG_STATE_B_IDLE:
			musb_g_disconnect(musb);
			break;
		default:
			dev_warn(musb->controller,"unhandled DISCONNECT transition (%s)\n",
				otg_state_string(musb->state));
			break;
		}
	}

	/* mentor saves a bit: bus reset and babble share the same irq.
	 * only host sees babble; only peripheral sees bus reset.
	 */
	if (int_usb & MUSB_INTR_RESET) {
		handled = IRQ_HANDLED;
		if (is_peripheral_capable()) {
			dev_dbg(musb->controller, "BUS RESET as %s\n",
				otg_state_string(musb->state));
			switch (musb->state) {
			case OTG_STATE_B_IDLE:
				musb->state = OTG_STATE_B_PERIPHERAL;
				/* FALLTHROUGH */
			case OTG_STATE_B_PERIPHERAL:
				musb_g_reset(musb);
				break;
			default:
				dev_dbg(musb->controller, "Unhandled BUS RESET as %s\n",
					otg_state_string(musb->state));
			}
		}
	}

	return handled;
}

/*-------------------------------------------------------------------------*/

/*
* Program the HDRC to start (enable interrupts, dma, etc.).
*/

int musb_start(struct musb *musb)
{
	void __iomem    *regs = musb->mregs;
	u8      devctl = musb_readb(regs, MUSB_DEVCTL);
	int ret;

	dev_dbg(musb->controller, "<== devctl %02x\n", devctl);

	/*  Set INT enable registers, enable interrupts */
	musb_writew(regs, MUSB_INTRTXE, musb->epmask);
	musb_writew(regs, MUSB_INTRRXE, musb->epmask & 0xfffe);
	musb_writeb(regs, MUSB_INTRUSBE, 0xf7);

	musb_writeb(regs, MUSB_TESTMODE, 0);

	/* put into basic highspeed mode and start session */
	musb_writeb(regs, MUSB_POWER, MUSB_POWER_ISOUPDATE
						| MUSB_POWER_HSENAB
						/* ENSUSPEND wedges tusb */
						/* | MUSB_POWER_ENSUSPEND */
						);

	musb->is_active = 0;
	devctl = musb_readb(regs, MUSB_DEVCTL);
	devctl &= ~MUSB_DEVCTL_SESSION;

	if (is_otg_enabled(musb)) {
	//otg mode
	} else if (is_host_enabled(musb)) {
		/* assume ID pin is hard-wired to ground */
		devctl |= MUSB_DEVCTL_SESSION;
	} else /* peripheral is enabled */ {
		if ((devctl & MUSB_DEVCTL_VBUS) == MUSB_DEVCTL_VBUS)
			musb->is_active = 1;
	}

	ret = musb_platform_enable(musb);
	if (ret) {
		musb->is_active = 0;
		return ret;
	}

	musb_writeb(regs, MUSB_DEVCTL, devctl);

	return 0;
}


static void musb_generic_disable(struct musb *musb)
{
	void __iomem    *mbase = musb->mregs;

	/* disable interrupts */
	musb_writeb(mbase, MUSB_INTRUSBE, 0);
	musb_writew(mbase, MUSB_INTRTXE, 0);
	musb_writew(mbase, MUSB_INTRRXE, 0);

	/* off */
	musb_writeb(mbase, MUSB_DEVCTL, 0);

	/*  flush pending interrupts */
	musb_readb(mbase, MUSB_INTRUSB);
	musb_readw(mbase, MUSB_INTRTX);
	musb_readw(mbase, MUSB_INTRRX);
}

/*
 * Make the HDRC stop (disable interrupts, etc.);
 * reversible by musb_start
 * called on gadget driver unregister
 * with controller locked, irqs blocked
 * acts as a NOP unless some role activated the hardware
 */
void musb_stop(struct musb *musb)
{
	/* stop IRQs, timers, ... */
	musb_platform_disable(musb);
	musb_generic_disable(musb);
	dev_dbg(musb->controller, "HDRC disabled\n");
}

/*-------------------------------------------------------------------------*/

/*
 * The silicon either has hard-wired endpoint configurations, or else
 * "dynamic fifo" sizing.  The driver has support for both, though at this
 * writing only the dynamic sizing is very well tested.   Since we switched
 * away from compile-time hardware parameters, we can no longer rely on
 * dead code elimination to leave only the relevant one in the object file.
 *
 * We don't currently use dynamic fifo setup capability to do anything
 * more than selecting one of a bunch of predefined configurations.
 */

static unsigned short fifo_mode = 2;

/*
 * configure a fifo; for non-shared endpoints, this may be called
 * once for a tx fifo and once for an rx fifo.
 *
 * returns negative errno or offset for next fifo.
 */
static int
fifo_setup(struct musb *musb, struct musb_hw_ep  *hw_ep,
		const struct musb_fifo_cfg *cfg, u16 offset)
{
	void __iomem    *mbase = musb->mregs;
	int size = 0;
	u16 maxpacket = cfg->maxpacket;
	u16 c_off = offset >> 3;
	u8  c_size;

	/* expect hw_ep has already been zero-initialized */
extern  int ffs(int x);

	size = ffs(max(maxpacket, (u16) 8)) - 1;
	maxpacket = 1 << size;

	c_size = size - 3;
	if (cfg->mode == BUF_DOUBLE) {
		if ((offset + (maxpacket << 1)) >
				(1 << (musb->config->ram_bits + 2)))
			return -EMSGSIZE;
		c_size |= MUSB_FIFOSZ_DPB;
	} else {
		if ((offset + maxpacket) > (1 << (musb->config->ram_bits + 2)))
			return -EMSGSIZE;
	}

	/* configure the FIFO */
	musb_writeb(mbase, MUSB_INDEX, hw_ep->epnum);

	/* EP0 reserved endpoint for control, bidirectional;
	 * EP1 reserved for bulk, two unidirection halves.
	 */
	if (hw_ep->epnum == 1)
		musb->bulk_ep = hw_ep;
	/* REVISIT error check:  be sure ep0 can both rx and tx ... */
	switch (cfg->style) {
	case FIFO_TX:
		musb_write_txfifosz(mbase, c_size);
		musb_write_txfifoadd(mbase, c_off);
		hw_ep->tx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_tx = maxpacket;
		break;
	case FIFO_RX:
		musb_write_rxfifosz(mbase, c_size);
		musb_write_rxfifoadd(mbase, c_off);
		hw_ep->rx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_rx = maxpacket;
		break;
	case FIFO_RXTX:
		musb_write_txfifosz(mbase, c_size);
		musb_write_txfifoadd(mbase, c_off);
		hw_ep->rx_double_buffered = !!(c_size & MUSB_FIFOSZ_DPB);
		hw_ep->max_packet_sz_rx = maxpacket;

		musb_write_rxfifosz(mbase, c_size);
		musb_write_rxfifoadd(mbase, c_off);
		hw_ep->tx_double_buffered = hw_ep->rx_double_buffered;
		hw_ep->max_packet_sz_tx = maxpacket;

		hw_ep->is_shared_fifo = true;
		break;
	}

	/* NOTE rx and tx endpoint irqs aren't managed separately,
	 * which happens to be ok
	 */
	musb->epmask |= (1 << hw_ep->epnum);

	return offset + (maxpacket << ((c_size & MUSB_FIFOSZ_DPB) ? 1 : 0));
}

static struct musb_fifo_cfg  ep0_cfg = {
	.style = FIFO_RXTX, .maxpacket = 64,
};

static int  ep_config_from_table(struct musb *musb)
{
	const struct musb_fifo_cfg  *cfg;
	unsigned        i, n;
	int         offset;
	struct musb_hw_ep   *hw_ep = musb->endpoints;

	if (musb->config->fifo_cfg) {
		cfg = musb->config->fifo_cfg;
		n = musb->config->fifo_cfg_size;
		goto done;
	}

	switch (fifo_mode) {
	default:
		fifo_mode = 0;
		/* FALLTHROUGH */
	}

	pr_debug("%s: setup fifo_mode %d\n", musb_driver_name, fifo_mode);

done:
	offset = fifo_setup(musb, hw_ep, &ep0_cfg, 0);
	/* assert(offset > 0) */

	/* NOTE:  for RTL versions >= 1.400 EPINFO and RAMINFO would
	 * be better than static musb->config->num_eps and DYN_FIFO_SIZE...
	 */

	for (i = 0; i < n; i++) {
		u8  epn = cfg->hw_ep_num;

		if (epn >= musb->config->num_eps) {
			pr_debug("%s: invalid ep %d\n",
					musb_driver_name, epn);
			return -EINVAL;
		}
		offset = fifo_setup(musb, hw_ep + epn, cfg++, offset);
		if (offset < 0) {
			pr_debug("%s: mem overrun, ep %d\n",
					musb_driver_name, epn);
			return -EINVAL;
		}
		epn++;
		musb->nr_endpoints = max(epn, musb->nr_endpoints);
	}

	pr_debug("%s: %d/%d max ep, %d/%d memory\n", musb_driver_name, n + 1,
		 musb->config->num_eps * 2 - 1, offset,
		 (1 << (musb->config->ram_bits + 2)));

	if (!musb->bulk_ep) {
		pr_debug("%s: missing bulk\n", musb_driver_name);
		return -EINVAL;
	}

	return 0;
}


/*
 * ep_config_from_hw - when MUSB_C_DYNFIFO_DEF is false
 * @param musb the controller
 */
static int  ep_config_from_hw(struct musb *musb)
{
	u8 epnum = 0;
	struct musb_hw_ep *hw_ep;
	void *mbase = musb->mregs;
	int ret = 0;

	dev_dbg(musb->controller, "<== static silicon ep config\n");

	/* FIXME pick up ep0 maxpacket size */

	for (epnum = 1; epnum < musb->config->num_eps; epnum++) {
		musb_ep_select(mbase, epnum);
		hw_ep = musb->endpoints + epnum;

		ret = musb_read_fifosize(musb, hw_ep, epnum);
		if (ret < 0)
			break;

		/* FIXME set up hw_ep->{rx,tx}_double_buffered */

		/* pick an RX/TX endpoint for bulk */
		if (hw_ep->max_packet_sz_tx < 512
				|| hw_ep->max_packet_sz_rx < 512)
			continue;

		/* REVISIT:  this algorithm is lazy, we should at least
		 * try to pick a double buffered endpoint.
		 */
		if (musb->bulk_ep)
			continue;
		musb->bulk_ep = hw_ep;
	}

	if (!musb->bulk_ep) {
		pr_debug("%s: missing bulk\n", musb_driver_name);
		return -EINVAL;
	}

	return 0;
}

enum { MUSB_CONTROLLER_MHDRC, MUSB_CONTROLLER_HDRC, };

/* Initialize MUSB (M)HDRC part of the USB hardware subsystem;
 * configure endpoints, or take their config from silicon
 */
static int  musb_core_init(u16 musb_type, struct musb *musb)
{
	u8 reg;
	void __iomem    *mbase = musb->mregs;
	int     status = 0;
	int     i;

	/* log core options (read using indexed model) */
	reg = musb_read_configdata(mbase);

	if (reg & MUSB_CONFIGDATA_DYNFIFO) {
		musb->dyn_fifo = true;
	}
	if (reg & MUSB_CONFIGDATA_MPRXE) {
		musb->bulk_combine = true;
	}
	if (reg & MUSB_CONFIGDATA_MPTXE) {
		musb->bulk_split = true;
	}
	if (reg & MUSB_CONFIGDATA_HBRXE) {
		musb->hb_iso_rx = true;
	}
	if (reg & MUSB_CONFIGDATA_HBTXE) {
		musb->hb_iso_tx = true;
	}
	if (reg & MUSB_CONFIGDATA_SOFTCONE)

	pr_debug("%s:ConfigData=0x%02x \n", musb_driver_name, reg);


	if (MUSB_CONTROLLER_MHDRC == musb_type) {
		musb->is_multipoint = 1;
	} else {
		musb->is_multipoint = 0;
	}

	/* log release info */
	musb->hwvers = musb_read_hwvers(mbase);

	/* configure ep0 */
	musb_configure_ep0(musb);

	/* discover endpoint configuration */
	musb->nr_endpoints = 1;
	musb->epmask = 1;

	musb->dyn_fifo = true;

	if (musb->dyn_fifo)
		status = ep_config_from_table(musb);
	else
		status = ep_config_from_hw(musb);

	if (status < 0)
		return status;

	/* finish init, and print endpoint config */
	for (i = 0; i < musb->nr_endpoints; i++) {
		struct musb_hw_ep   *hw_ep = musb->endpoints + i;

		hw_ep->fifo = musb_default_fifo_offset(i) + mbase;

		hw_ep->regs = MUSB_EP_OFFSET(i, 0) + mbase;
		hw_ep->target_regs = musb_read_target_reg_base(i, mbase);
		hw_ep->rx_reinit = 1;
		hw_ep->tx_reinit = 1;

		if (hw_ep->max_packet_sz_tx) {
			dev_dbg(musb->controller,
				"%s: hw_ep %d%s, %smax %d\n",
				musb_driver_name, i,
				hw_ep->is_shared_fifo ? "shared" : "tx",
				hw_ep->tx_double_buffered
					? "doublebuffer, " : "",
				hw_ep->max_packet_sz_tx);
		}
		if (hw_ep->max_packet_sz_rx && !hw_ep->is_shared_fifo) {
			dev_dbg(musb->controller,
				"%s: hw_ep %d%s, %smax %d\n",
				musb_driver_name, i,
				"rx",
				hw_ep->rx_double_buffered
					? "doublebuffer, " : "",
				hw_ep->max_packet_sz_rx);
		}
		if (!(hw_ep->max_packet_sz_tx || hw_ep->max_packet_sz_rx))
			dev_dbg(musb->controller, "hw_ep %d not configured\n", i);
	}

	return 0;
}

/*-------------------------------------------------------------------------*/

static irqreturn_t generic_interrupt(int irq, void *__hci)
{
	unsigned long   flags;
	irqreturn_t retval = IRQ_NONE;
	struct musb *musb = __hci;

	spin_lock_irqsave(&musb->lock, flags);

	musb->int_usb = musb_readb(musb->mregs, MUSB_INTRUSB);
	musb->int_tx = musb_readw(musb->mregs, MUSB_INTRTX);
	musb->int_rx = musb_readw(musb->mregs, MUSB_INTRRX);

	if (musb->int_usb || musb->int_tx || musb->int_rx)
		retval = musb_interrupt(musb);

	spin_unlock_irqrestore(&musb->lock, flags);

	return retval;
}


/*
 * handle all the irqs defined by the HDRC core. for now we expect:  other
 * irq sources (phy, dma, etc) will be handled first, musb->int_* values
 * will be assigned, and the irq will already have been acked.
 *
 * called in irq context with spinlock held, irqs blocked
 */
irqreturn_t musb_interrupt(struct musb *musb)
{
	irqreturn_t retval = IRQ_NONE;
	u8      devctl, power;
	int     ep_num;
	u32     reg;

	devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
	power = musb_readb(musb->mregs, MUSB_POWER);
#if 0
	dev_dbg(musb->controller, "** IRQ %s usb%04x tx%04x rx%04x\n",
		(devctl & MUSB_DEVCTL_HM) ? "host" : "peripheral",
		musb->int_usb, musb->int_tx, musb->int_rx);
#endif
	/* the core can interrupt us for multiple reasons; docs have
	 * a generic interrupt flowchart to follow
	 */
	if (musb->int_usb)
		retval |= musb_stage0_irq(musb, musb->int_usb,
				devctl, power);

	/* "stage 1" is handling endpoint irqs */

	/* handle endpoint 0 first */
	if (musb->int_tx & 1) {

		retval |= musb_g_ep0_irq(musb);
		musb->int_tx &= ~BIT(0);
	}

	/* RX on endpoints 1-15 */
	reg = musb->int_rx >> 1;
	ep_num = 1;
	while (reg) {
		if (reg & 1) {
			/* musb_ep_select(musb->mregs, ep_num); */
			/* REVISIT just retval = ep->rx_irq(...) */
			retval = IRQ_HANDLED;
			if (devctl & MUSB_DEVCTL_HM) {
			//host
			} else {
				if (is_peripheral_capable())
					musb_g_rx(musb, ep_num);
			}
		}

		reg >>= 1;
		ep_num++;
	}

	/* TX on endpoints 1-15 */
	reg = musb->int_tx >> 1;
	ep_num = 1;
	while (reg) {
		if (reg & 1) {
			/* musb_ep_select(musb->mregs, ep_num); */
			/* REVISIT just retval |= ep->tx_irq(...) */
			retval = IRQ_HANDLED;
			if (devctl & MUSB_DEVCTL_HM) {
			//host
			} else {
				if (is_peripheral_capable())
					musb_g_tx(musb, ep_num);
			}
		}
		reg >>= 1;
		ep_num++;
	}

	return retval;
}

#ifdef CONFIG_USB_INVENTRA_DMA
static bool  use_dma = 1;

void musb_dma_completion(struct musb *musb, u8 epnum, u8 transmit)
{

	/* called with controller lock already held */

	if (!epnum) {

		if (!is_cppi_enabled()) {
			/* endpoint 0 */

			musb_g_ep0_irq(musb);
		}

	} else {
		/* endpoints 1..15 */
		if (transmit) {

			musb_g_tx(musb, epnum);

		} else {

			musb_g_rx(musb, epnum);

		}
	}
}

#else
#define use_dma         0
void musb_dma_completion(struct musb *musb, u8 epnum, u8 transmit)
{
}
#endif
/* --------------------------------------------------------------------------
 * Init support
 */

static struct musb *
allocate_instance(struct device *dev,
		struct musb_hdrc_config *config, void __iomem *mbase)
{
	struct musb     *musb;
	struct musb_hw_ep   *ep;
	int         epnum;

	musb = pvPortMalloc(sizeof(*musb));
	if (!musb)
		return NULL;

	INIT_LIST_HEAD(&musb->control);
	INIT_LIST_HEAD(&musb->in_bulk);
	INIT_LIST_HEAD(&musb->out_bulk);

	musb->vbuserr_retry = VBUSERR_RETRY_COUNT;

	musb->mregs = mbase;
	musb->ctrl_base = mbase;
	musb->nIrq = -ENODEV;
	musb->config = config;
	BUG_ON(musb->config->num_eps > MUSB_C_NUM_EPS);
	for (epnum = 0, ep = musb->endpoints;
			epnum < musb->config->num_eps;
			epnum++, ep++) {
		ep->musb = musb;
		ep->epnum = epnum;
	}

	musb->controller = dev;

	return musb;
}

/*
 * Perform generic per-controller initialization.
 *
 * @pDevice: the controller (already clocked, etc)
 * @nIrq: irq
 * @mregs: virtual address of controller registers,
 *  not yet corrected for platform-specific offsets
 */

int  musb_init_controller(struct device *dev, int nIrq, void __iomem *ctrl, struct musb **musbp)
{
	int         status;
	struct musb     *musb;

	struct musb_hdrc_platform_data *plat = dev->device_data;

	/* The driver might handle more features than the board; OK.
	 * Fail when the board needs a feature that's not enabled.
	 */
	if (!plat) {
		dev_dbg(dev, "no platform_data?\n");
		status = -ENODEV;
		goto fail0;
	}

	/* allocate */
	musb = allocate_instance(dev, plat->config, ctrl);
	if (!musb) {
		status = -ENOMEM;
		goto fail0;
	}

	*musbp = musb;

	spin_lock_init(&musb->lock);
	musb->board_mode = plat->mode;
	musb->board_set_power = plat->set_power;
	musb->min_power = plat->min_power;
	musb->ops = plat->platform_ops;

	/* The musb_platform_init() call:
	 *   - adjusts musb->mregs and musb->isr if needed,
	 *   - may initialize an integrated tranceiver
	 *   - initializes musb->xceiv, usually by otg_get_phy()
	 *   - stops powering VBUS
	 *
	 * There are various transceiver configurations.  Blackfin,
	 * DaVinci, TUSB60x0, and others integrate them.  OMAP3 uses
	 * external/discrete ones in various flavors (twl4030 family,
	 * isp1504, non-OTG, etc) mostly hooking up through ULPI.
	 */
	musb->isr = generic_interrupt;
	status = musb_platform_init(musb);
	if (status < 0)
		goto fail1;

	if (!musb->isr) {
		status = -ENODEV;
		goto fail2;
	}

	musb->io.ep_offset = musb_indexed_ep_offset;
	musb->io.ep_select = musb_indexed_ep_select;

#ifdef CONFIG_USB_INVENTRA_DMA
	if (use_dma) {
		struct dma_controller   *c;

		c = musbhs_dma_controller_create(musb, musb->mregs);
		musb->dma_controller = c;
		if (c && c->start)
			(void) c->start(c);
	}
#endif

	/* be sure interrupts are disabled before connecting ISR */
	musb_platform_disable(musb);
	musb_generic_disable(musb);

	/* setup musb parts of the core (especially endpoints) */
	status = musb_core_init(plat->config->multipoint
			? MUSB_CONTROLLER_MHDRC
			: MUSB_CONTROLLER_HDRC, musb);
	if (status < 0)
		goto fail3;

	status = musb_gadget_setup(musb);
	if (status < 0)
		goto fail3;

	musb->state = OTG_STATE_B_IDLE;

	/* attach to the IRQ */
	if (request_irq(nIrq, musb->isr,   "musb", musb)) {
		dev_err(dev, "request_irq %d failed!\n", nIrq);
		status = -ENODEV;
		goto fail3;
	}
	musb->nIrq = nIrq;

	return 0;

fail3:

fail2:
	musb_platform_exit(musb);
fail1:
	dev_err(musb->controller,
		"musb_init_controller failed with status %d\n", status);
fail0:
	return status;
}
