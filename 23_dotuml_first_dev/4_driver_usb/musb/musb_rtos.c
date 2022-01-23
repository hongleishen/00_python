#include <errno.h>
#include <usb/linux/ch9.h>
#include <usb/linux/gadget.h>
#include "linux_compat.h"
#include "musb_core.h"
#include "clock.h"
#include "musb_gadget.h"
#include <irqs.h>

static struct musb *gadget;

int usb_gadget_handle_interrupts(int index)
{
	if (!gadget || !gadget->isr)
		return -EINVAL;

	return gadget->isr(0, gadget);
}
#if (defined TARGET_SRAM) || (defined TARGET_DDR)
static struct musb_fifo_cfg  imi2280_cfg[] = {
//ep1/ep2 max fifo 8192(4096*DOUBLE)
#if 0
{ .hw_ep_num = 1, .style = FIFO_RXTX,   .maxpacket = 4096, .mode = BUF_DOUBLE,},
{ .hw_ep_num = 2, .style = FIFO_RXTX,   .maxpacket = 4096, .mode = BUF_DOUBLE,},
#else
{ .hw_ep_num = 1, .style = FIFO_RX,   .maxpacket = 512, .mode = BUF_DOUBLE,},
{ .hw_ep_num = 2, .style = FIFO_TX,   .maxpacket = 512, .mode = BUF_DOUBLE,},
#endif

//ep3/ep4 max fifo 512(256*DOUBLE)
{ .hw_ep_num = 3, .style = FIFO_TX, .maxpacket = 256, .mode = BUF_DOUBLE,},
{ .hw_ep_num = 4, .style = FIFO_RX, .maxpacket = 256, .mode = BUF_DOUBLE,},

//how to config ep5(tx 512,rx 8)
{ .hw_ep_num = 5, .style = FIFO_TX, .maxpacket = 256, .mode = BUF_DOUBLE,},
{ .hw_ep_num = 5, .style = FIFO_RX, .maxpacket = 8, .mode = BUF_DOUBLE,},

};
#else
static struct musb_fifo_cfg  imi2280_cfg[] = {
{ .hw_ep_num = 1, .style = FIFO_RX,   .maxpacket = 512, .mode = BUF_DOUBLE,},
{ .hw_ep_num = 2, .style = FIFO_TX,   .maxpacket = 512, .mode = BUF_DOUBLE,},

//ep3/ep4 max fifo 512(256*DOUBLE)
{ .hw_ep_num = 3, .style = FIFO_TX, .maxpacket = 256, .mode = BUF_DOUBLE,},
{ .hw_ep_num = 4, .style = FIFO_RX, .maxpacket = 256, .mode = BUF_DOUBLE,},

//how to config ep5(tx 512,rx 8)
{ .hw_ep_num = 5, .style = FIFO_TX, .maxpacket = 256, .mode = BUF_DOUBLE,},
{ .hw_ep_num = 5, .style = FIFO_RX, .maxpacket = 8, .mode = BUF_DOUBLE,},

};
#endif

static struct musb_hdrc_config imi2280_musb_hdrc_config = {
	.fifo_cfg   = &imi2280_cfg[0],
	.fifo_cfg_size = sizeof(imi2280_cfg)/sizeof(imi2280_cfg[0]),/*ARRAY_SIZE(imi2280_cfg),*/
	.multipoint = true,/*false*/
	.dyn_fifo   = true,/*not read hw fifo cfg reg*/
	.num_eps    = 6,
	.ram_bits   = 13,/*fix me*/
};

static struct musb_hdrc_platform_data imi2280_musb_platform_data = {
	.mode = MUSB_PERIPHERAL,

	.config = &imi2280_musb_hdrc_config,
	.board_data = NULL,//&imi2280_musb_board_data,
	.platform_ops = NULL,
};

static struct device musb_device =
{
	.device_data = &imi2280_musb_platform_data,
};

#if (defined TARGET_SRAM) || (defined TARGET_DDR)
#define INTNR_MC_NINT               IRQ_U0_MUSBHSFC_MC

#define USB_BASE_ADDR          0x20000000  // 0x20000000 ~ 0x2000FFFF (64KB)
static void pre_init_chip(void);
static void imi_musb_reset(void);
#else
#define INTNR_MC_NINT               IRQ_USB_DEV

#define USB_BASE_ADDR          0x22000000  // 0x20000000 ~ 0x2000FFFF (64KB)
#endif

int musb_init(void)
{
	int ret = 0;
	int irq = INTNR_MC_NINT;
#if (defined TARGET_SRAM) || (defined TARGET_DDR)
	imi_musb_reset();
	pre_init_chip();
#else
	clock_enable("usb");
#endif
	struct musb **musbp = &gadget;
	ret = musb_init_controller(&musb_device, irq, (void *)USB_BASE_ADDR, musbp) ;
	if (ret) {
		imi_printf("Failed to init the controller\n");
		gadget = NULL;
		return -EIO;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////
#if (defined TARGET_SRAM) || (defined TARGET_DDR)
#define IMI_GPIO0_BASE_ADDR     0x30600000
#define CLK_GEN_BASE_ADDR       0x30100000
#define SYS_CON3_BASE_ADDR      0x32000000
#define SYS_CON2_BASE_ADDR      0x32D00000
#define USB_PHY_CTL_BASE_ADDR   0x32200000

static void usb_set_reg_val(unsigned long reg, unsigned long val)
{
	volatile unsigned long tmp;

	tmp = __raw_readl(reg);
	tmp |= val;
	__raw_writel(tmp, reg);
}

static void pre_init_chip(void)
{
	unsigned int dly = 100;
	unsigned long val;

	val = __raw_readl(CLK_GEN_BASE_ADDR + 0x288);
	__raw_writel(val & (~(1 << 16)), CLK_GEN_BASE_ADDR + 0x288); //usb phy clk

	usb_set_reg_val(SYS_CON2_BASE_ADDR + 0x0094, 0x1); // USB3 PHY RESET asserted

	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01a8);//acaenb0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01ac);//adpchrg0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01b0);//adpdischrg0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01b4);//adpprbenb0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01b8);//atereset
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01bc);//bypassdmdata0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01c0);//bypassdmen0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01c4);//bypassdpdata0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01c8);//bypassdpen0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01cc);//bypasssel0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01d0);//chrgsel0
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x01d4);//commononn
	__raw_writel(0x00000004, USB_PHY_CTL_BASE_ADDR + 0x01d8);//compdistune0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0190);//dcdenb0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0194);//dmpulldown0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0198);//dppulldown0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x019c);//drvvbus0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x017c);//fsdataext0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0180);//fsse0ext0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0188);//idpullup0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x018c);//loopbackenb0
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x01a0);//otgdisable0
	__raw_writel(0x00000004, USB_PHY_CTL_BASE_ADDR + 0x01a4);//otgtune0
	__raw_writel(0x00000002, USB_PHY_CTL_BASE_ADDR + 0x001c);//refclksel:2'b10: HS share with SS clock source, 2'b11: HS use EXTREFCLK
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x0174);//retenablen
	__raw_writel(0x00000003, USB_PHY_CTL_BASE_ADDR + 0x0178);//sqrxtune0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01dc);//txbitstuffen0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01e0);//txbitstuffenh0
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x01e4);//txenablen0
	__raw_writel(0x00000003, USB_PHY_CTL_BASE_ADDR + 0x01e8);//txfslstune0
	__raw_writel(0x00000003, USB_PHY_CTL_BASE_ADDR + 0x01ec);//txhsxvtune0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01f0);//txpreempamptune0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x01f4);//txpreemppulsetune0
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x01f8);//txrestune0
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x01fc);//txrisetune0
	__raw_writel(0x00000003, USB_PHY_CTL_BASE_ADDR + 0x016c);//txverftune0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0170);//vatestenb
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x0008);//vbusvldext0
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x000c);//vbusvldextsel0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0164);//vdatdetenb0
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0168);//vdatsrcenb0
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x0160);//wordinterface0:16bit utmi +
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0154);//acjt_level
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0158);//alt_clk_en
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x015c);//alt_clk_sel
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x005c);//bs_capture_dr
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0060);//bs_clamp
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0064);//bs_clk
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0068);//bs_extest
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x006c);//bs_extest_ac
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0070);//bs_highz
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0074);//bs_in
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0078);//bs_init
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x007c);//bs_invert
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0080);//bs_preload
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0084);//bs_shift_dr
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0088);//bs_update_dr
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0090);//cr_cap_addr
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0094);//cr_cap_data
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0098);//cr_data_in
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x009c);//cr_read
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0100);//cr_write
	__raw_writel(0x0000002A, USB_PHY_CTL_BASE_ADDR + 0x0104);//fsel: SS:0x31; HS:0x5
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0134);//lane0_ext_pclk_req
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0138);//lane0_tx2rx_loopbk
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x013c);//lane0_tx_term_offset
	__raw_writel(0x00000005, USB_PHY_CTL_BASE_ADDR + 0x0140);//los_bias
	__raw_writel(0x00000009, USB_PHY_CTL_BASE_ADDR + 0x0144);//los_level
	__raw_writel(0x00000068, USB_PHY_CTL_BASE_ADDR + 0x0148);//mpll_multiplier
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x014c);//mpll_refssc_clk_en
	__raw_writel(0x000000c8, USB_PHY_CTL_BASE_ADDR + 0x0150);//pcs_rx_los_mask_val
	__raw_writel(0x00000015, USB_PHY_CTL_BASE_ADDR + 0x0128);//pcs_tx_deemph_3p5db
	__raw_writel(0x00000020, USB_PHY_CTL_BASE_ADDR + 0x012c);//pcs_tx_deemph_6db
	__raw_writel(0x0000005d, USB_PHY_CTL_BASE_ADDR + 0x0130);//pcs_tx_swing_full
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0124);//ref_clkdiv2
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x0120);//ref_ssp_en
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0118);//ref_use_pad
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x011c);//rtune_req
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x0010);//ssc_en
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0014);//ssc_range
	__raw_writel(0x00000108, USB_PHY_CTL_BASE_ADDR + 0x0018);//ssc_ref_clk_sel
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0108);//test_burnin
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x010c);//test_powerdown_hsp
	__raw_writel(0x00000000, USB_PHY_CTL_BASE_ADDR + 0x0110);//test_powerdown_ssp
	__raw_writel(0x00000004, USB_PHY_CTL_BASE_ADDR + 0x0114);//tx_vboost_lvl
	__raw_writel(0x00000001, USB_PHY_CTL_BASE_ADDR + 0x021c); //old usb2.0

	dly = 100;
	while (dly--)
		;

	__raw_writel(0x0, SYS_CON2_BASE_ADDR + 0x0094); // USB3 PHY RESET released
}

static void usb_set_reg(unsigned int addr, unsigned char bit)
{
	unsigned int val;
	volatile unsigned int *ptr = (volatile unsigned int *)addr;
	unsigned int j;

	val = *ptr;
	val &= ~(0x01 << bit);
	*ptr = val;
	for (j = 0; j < 0x1000; j++)
		;

	val |= 0x01 << bit;
	*ptr = val;
	for (j = 0; j < 0x1000; j++)
		;

	val &= ~(0x01 << bit);
	*ptr = val;
	for (j = 0; j < 0x1000; j++)
		;
}

static void imi_musb_reset(void)
{
	usb_set_reg(0x30200000, 16);
	usb_set_reg(0x30200100, 16);

	usb_set_reg(0x30200000, 17);
	usb_set_reg(0x30200100, 17);

	usb_set_reg(0x30200000, 18);
	usb_set_reg(0x30200100, 18);
}
#endif