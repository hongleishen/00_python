//#include "imi-csi.h"
#if defined(CHIP_IMI_2280)
#include "linux_common.h"
#include "csi_rx.h"
#include "reset.h"
#include "delay.h"
#include "clock.h"

#define DEFAULT_IPI_HSA_TIME		0x1f
#define DEFAULT_IPI_HBP_TIME		0x1f
#define DEFAULT_IPI_HSD_TIME		0x0a

static int debug = 0;

static LIST_HEAD(csi_list);

struct porch_timming {
	unsigned int hsd_time;
	unsigned int hsa_time;
	unsigned int hbp_time;
};

static const struct {
	unsigned int max_mbps;
	unsigned int hsfreqrange;
	unsigned int osc_freq;
} hsfreq_map[] = {
	{80, 0x00, 438}, {90, 0x10, 438}, {100, 0x20, 438},
	{110, 0x30, 438}, {120, 0x01, 438}, {130, 0x11, 438},
	{140, 0x21, 438}, {150, 0x31, 438}, {160, 0x02, 438},
	{170, 0x12, 438}, {180, 0x22, 438}, {190, 0x32, 438},
	{205, 0x03, 438}, {220, 0x13, 438}, {235, 0x23, 438},
	{250, 0x33, 438}, {275, 0x04, 438}, {300, 0x14, 438},
	{325, 0x25, 438}, {350, 0x35, 438}, {400, 0x05, 438},
	{450, 0x16, 438}, {500, 0x26, 438}, {550, 0x37, 438},
	{600, 0x07, 438}, {650, 0x18, 438}, {700, 0x28, 438},
	{750, 0x39, 438}, {800, 0x09, 438}, {850, 0x19, 438},
	{900, 0x29, 438}, {950, 0x3a, 438}, {1000, 0x0a, 438},
	{1050, 0x1a, 438}, {1100, 0x2a, 438}, {1150, 0x3b, 438},
	{1200, 0x0b, 438}, {1250, 0x1b, 438}, {1300, 0x2b, 438},
	{1350, 0x3c, 438}, {1400, 0x0c, 438}, {1450, 0x1c, 438},
	{1500, 0x2c, 438}, {1550, 0x3d, 271}, {1600, 0x0d, 280},
	{1650, 0x1d, 289}, {1700, 0x2d, 298}, {1750, 0x3e, 306},
	{1800, 0x0e, 315}, {1850, 0x1e, 324}, {1900, 0x2e, 333},
	{1950, 0x3f, 341}, {2000, 0x0f, 350}, {2050, 0x40, 359},
	{2100, 0x41, 368}, {2150, 0x42, 376}, {2200, 0x43, 385},
	{2250, 0x44, 394}, {2300, 0x45, 403}, {2350, 0x46, 411},
	{2400, 0x47, 420}, {2450, 0x48, 428}, {2500, 0x49, 438},
};

static int mbps_to_index(unsigned int mbps)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(hsfreq_map); i++) {
		if (hsfreq_map[i].max_mbps >= mbps)
			return i;
	}

	if (i >= ARRAY_SIZE(hsfreq_map))
		i = ARRAY_SIZE(hsfreq_map) - 1;

	return i;
}

void imi_csi_calc_freq(unsigned int mbps,
		unsigned int *hsfreqrange, unsigned int *osc_freq)
{
	int index;

	index = mbps_to_index(mbps);
	*hsfreqrange = hsfreq_map[index].hsfreqrange;
	*osc_freq = hsfreq_map[index].osc_freq;
}

struct imi_csi *imi_get_csi(int id)
{
	struct imi_csi *icsi;

	/*search matched csi*/
	list_for_each_entry(icsi, &csi_list, list) {
		if (icsi->csi_id == id)
			return icsi;
	}

	return NULL;
}

static inline void csi2_write_reg(struct imi_csi *csi,
		unsigned int reg, unsigned int val)
{
	writel(val, csi->base + reg);
//	imi_printf("vidomain %x=%x\n", csi->base + reg, val);
}

static inline unsigned int csi2_read_reg(struct imi_csi *csi,
		unsigned int reg)
{
	int val;

	val = readl(csi->base + reg);
	//	imi_printf("%s reg %x val %x\n", __func__, csi->base + reg, val);
	return val;
}

static void csi2_dhpy_write(struct imi_csi *csi,
		unsigned int code, unsigned int data)
{
	u8 code_msb = (code >> 8) & 0xf;
	u8 code_lsb = code & 0xff;

	//csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, 0);
	//csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);

	/* write 4-bit MSB testcode */
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, DPHY_TESTEN);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, DPHY_TESTCLK);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, DPHY_TESTEN | 0x00);
	/* failing edge on testclk, testdin signal context is latched internally*/
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, code_msb);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, DPHY_TESTCLK);

	/* write 8-bit LSB testcode */
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, DPHY_TESTEN);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, DPHY_TESTCLK);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, DPHY_TESTEN | code_lsb);
	/* failing edge on testclk, testdin signal context is latched internally*/
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, 0);

	/* write 8-bit testdata */
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, data);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, DPHY_TESTCLK);
}

static int csi2_dhpy_read(struct imi_csi *csi,
		unsigned int code)
{
	int val;
	u8 code_msb = (code >> 8) & 0xf;
	u8 code_lsb = code & 0xff;

	//csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, 0);
	//csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);

	/* write 4-bit MSB testcode */
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, DPHY_TESTEN);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, DPHY_TESTCLK);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, DPHY_TESTEN | 0x00);
	/* failing edge on testclk, testdin signal context is latched internally*/
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, code_msb);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, DPHY_TESTCLK);

	/* write 8-bit LSB testcode */
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, DPHY_TESTEN);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, DPHY_TESTCLK);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, DPHY_TESTEN | code_lsb);
	/* failing edge on testclk, testdin signal context is latched internally*/
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL1, 0);

	/* read 8-bit testdata */
	val = csi2_read_reg(csi, MIPI_PHY_TEST_CTRL1);
	val = (val >> 8) & 0xff;

	return val;
}

static unsigned int calc_best_div_ipi(unsigned int src,
		unsigned int dst, unsigned int *diff)
{
	unsigned int div = 1;

	while (dst * div <= src) {
		div++;
	}

	div--;
	*diff = src - div * dst;

	csi2_dbg(1, debug, "div:%d, diff:%d\n", div, *diff);

	return div;
}

static unsigned int calc_ipi_pixel_clk(struct vi_param *vip,
		unsigned int *pll_index)
{
	unsigned int ipiclk;
	unsigned int pixmbps =
		vip->mipi_lanes * vip->mipi_mbps * 1000000;
	unsigned int div0 = 1;
	unsigned int div1 = 1;
	unsigned int diff0, diff1;

	if (vip->mipi_dt == MIPI_8BIT)
		ipiclk = pixmbps / 8;
	else if (vip->mipi_dt == MIPI_10BIT)
		ipiclk = pixmbps / 10;
	else if (vip->mipi_dt == MIPI_12BIT)
		ipiclk = pixmbps / 12;
	else
		ipiclk = pixmbps / 12;

	csi2_dbg(1, debug, "pixclk:%d\n", ipiclk);

	div0 = calc_best_div_ipi(CSI_RX_PLL_ALT0, ipiclk, &diff0);
	div1 = calc_best_div_ipi(CSI_RX_PLL_ALT1, ipiclk, &diff1);

	if (diff0 > diff1) {
		ipiclk = CSI_RX_PLL_ALT1 / div1;
		*pll_index = 1;
	} else {
		ipiclk = CSI_RX_PLL_ALT0 / div0;
		*pll_index = 0;
	}

	csi2_dbg(1, debug, "ipiclk:%d, src pll:%s\n", ipiclk,
			*pll_index ? "pll1" : "pll0");

	return ipiclk;
}

static int csi2_calc_porch(struct vi_param *vip,
		struct porch_timming *htime, unsigned int ipiclk)
{
	unsigned int pixkclk, porch_min, porch_tal;
	unsigned int pixkbps =
		vip->mipi_lanes * vip->mipi_mbps * 1000;

	if (vip->mipi_dt == MIPI_8BIT)
		pixkclk = pixkbps / 8;
	else if (vip->mipi_dt == MIPI_10BIT)
		pixkclk = pixkbps / 10;
	else if (vip->mipi_dt == MIPI_12BIT)
		pixkclk = pixkbps / 12;
	else
		pixkclk = pixkbps / 12;

	porch_min = (ipiclk/1000 - pixkclk) * vip->img_width / pixkclk;
	csi2_dbg(1, debug, "porch_min:%d\n", porch_min);

	htime->hbp_time = DEFAULT_IPI_HBP_TIME;
	htime->hsd_time = DEFAULT_IPI_HSD_TIME;
	htime->hsa_time = DEFAULT_IPI_HSA_TIME;
	porch_tal = htime->hbp_time + htime->hsd_time + htime->hsa_time;

	if (porch_min > porch_tal)
		/* 0x10 use for safe gap. */
		htime->hbp_time += porch_min - porch_tal + 0x10;

	csi2_dbg(1, debug, "hbp_time:%d\n", htime->hbp_time);
	csi2_dbg(1, debug, "hsd_time:%d\n", htime->hsd_time);
	csi2_dbg(1, debug, "hsa_time:%d\n", htime->hsa_time);

	return 0;
}

static unsigned int csi2_clk_init(struct imi_csi *csi, struct vi_param *vip)
{
	unsigned int ipiclk;
	unsigned int pll_index;
	const unsigned int esc_rate = 20000000;
	const unsigned int osc_rate = 24000000;

	/*ipi clk*/
	ipiclk = calc_ipi_pixel_clk(vip, &pll_index);
	if (pll_index == 0)
		clk_parent_select(csi->mout_ipi, "pll0_clk");
	else
		clk_parent_select(csi->mout_ipi, "pll1_clk");
	clk_set_rate(csi->ipi, ipiclk);
//	do {csi->ipi->clk_val = ipiclk;}while(0);
	clk_prepare_enable(csi->ipi);

	/*ref clk*/
	clk_parent_select(csi->mout_ref, "osc_24");
	clk_set_rate(csi->refclk, osc_rate);
	clk_prepare_enable(csi->refclk);

	/*cfg clk*/
	clk_parent_select(csi->mout_cfg, "osc_24");
	clk_set_rate(csi->cfgclk, osc_rate);
	clk_prepare_enable(csi->cfgclk);

	/*esc clk*/
	clk_parent_select(csi->mout_esc, "pll1_clk");
	clk_set_rate(csi->esc, esc_rate);
	clk_prepare_enable(csi->esc);

	return ipiclk;
}

static void csi2_host_reset(struct imi_csi *csi)
{
	reset_control_assert(csi->csi_rst);
	reset_control_assert(csi->ipi_rst);
	udelay(1);
	reset_control_deassert(csi->csi_rst);
	reset_control_deassert(csi->ipi_rst);
	udelay(1);
}

static void csi2_host_init(struct imi_csi *csi,
		struct vi_param *vip, unsigned int ipiclk)
{
	struct porch_timming htime;

	csi2_calc_porch(vip, &htime, ipiclk);

	csi2_write_reg(csi, MIPI_CSI2_RESET, 0x1);
	csi2_write_reg(csi, MIPI_IPI_MODE, IPI_16BIT_MODE | CAMERA_TIMING);
	csi2_write_reg(csi, MIPI_IPI_VCID, 0x0);
	csi2_write_reg(csi, MIPI_IPI_DATA_TYPE, vip->mipi_dt);
	csi2_write_reg(csi, MIPI_IPI_HSA_TIME, htime.hsa_time);
	csi2_write_reg(csi, MIPI_IPI_HBP_TIME, htime.hbp_time);
	csi2_write_reg(csi, MIPI_IPI_HSD_TIME, htime.hsd_time);
	csi2_write_reg(csi, MIPI_IPI_HLINE_TIME, 0x650);
	csi2_write_reg(csi, MIPI_IPI_VSA_LINES, 0x02);
	csi2_write_reg(csi, MIPI_IPI_VBP_LINES, 0x02);
	csi2_write_reg(csi, MIPI_IPI_VFP_LINES, 0x0f);
	csi2_write_reg(csi, MIPI_IPI_VACTIVE_LINES, 0x78);
	csi2_write_reg(csi, MIPI_N_LANES, vip->mipi_lanes - 1);
}

void imi_csi2_start(struct imi_csi *csi, struct vi_param *vip)
{
	unsigned int ipiclk;

	ipiclk = csi2_clk_init(csi, vip);
	csi2_host_reset(csi);
	csi2_host_init(csi, vip, ipiclk);
}
EXPORT_SYMBOL(imi_csi2_start);

void imi_csi2_stop(struct imi_csi *csi)
{
	csi2_write_reg(csi, MIPI_CSI2_RESET, 0);
	udelay(1);

	reset_control_assert(csi->csi_rst);
	reset_control_assert(csi->ipi_rst);
	udelay(1);

	clk_disable_unprepare(csi->ipi);
	clk_disable_unprepare(csi->refclk);
	clk_disable_unprepare(csi->cfgclk);
	clk_disable_unprepare(csi->esc);
}
EXPORT_SYMBOL(imi_csi2_stop);

void imi_csi2_start_reset(struct imi_csi *csi)
{
	csi2_write_reg(csi, MIPI_PHY_SHUTDOWNZ, 0);
	csi2_write_reg(csi, MIPI_DPHY_RSTZ, 0);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, DPHY_TESTCLR);
	udelay(1);
	csi2_write_reg(csi, MIPI_PHY_TEST_CTRL0, 0);
}
EXPORT_SYMBOL(imi_csi2_start_reset);

int imi_csi2_dhpy_reset(struct imi_csi *csi, unsigned int ccf,
		unsigned int osc_freq, unsigned int lanes)
{
//	int counter;
	unsigned int mask = STOPSTATE_CLK | ((1 << lanes) - 1);
	int count = 0;

#if 0
	/*cfgclkfreqrange[5:0]*/
	ccf &= 0x3f;

	if (ccf < 13)
		counter = 0;
	else if (ccf < 39)
		counter = 1;
	else if (ccf < 66)
		counter = 2;
	else if (ccf < 93)
		counter = 3;
	else if (ccf < 119)
		counter = 4;
	else if (ccf < 141)
		counter = 5;
#endif
	//csi2_dhpy_write(csi, 0xe5, 0x1);
	/*write osc_freq_target*/
	csi2_dhpy_write(csi, 0xe2, osc_freq & 0xff);
	csi2_dhpy_write(csi, 0xe3, (osc_freq >> 8) & 0xf);
	csi2_dhpy_write(csi, 0xe4, 0x1);
	//csi2_dhpy_write(csi, 0xe4, (counter << 4) | 0x1);

	csi2_dhpy_write(csi, 0x08, 0x38);

	csi2_write_reg(csi, MIPI_PHY_SHUTDOWNZ, 1);
	udelay(1);
	csi2_write_reg(csi, MIPI_DPHY_RSTZ, 1);
	udelay(1);

	/*wait LP11 mode*/
#if 0
	ret = readl_poll_timeout(csi->base + MIPI_PHY_STOPSTATE, reg,
			(reg & mask) == mask, 0, 100000);
	if (ret) {
		csi2_dbg(0, debug, "ULP timeout, phy_state = 0x%08x\n", reg);
		return ret;
	}
#else
	while (1) {
		unsigned rd_dat = readl(csi->base + MIPI_PHY_STOPSTATE);

		if ((rd_dat & mask) == mask) {
			break;
		}
		else{
			//delay50us
			udelay(50);
		}
		if (++count>5000) {
			csi2_dbg(0, debug, "MIPI PHY INIT TIMEOUT\n");
			break;
		}

	}
#endif

	return 0;
}
EXPORT_SYMBOL(imi_csi2_dhpy_reset);

int imi_csi2_dhpy_calib_check(struct imi_csi *csi, struct vi_param *vip)
{
	unsigned int val;
	unsigned int done_flag, err_flag;

	/* Resistor calibration */
	val = csi2_dhpy_read(csi, 0x222);
	val = csi2_dhpy_read(csi, 0x221);
	if ((val & 0x80) == 0x0) {
		dev_err(csi->dev, "Resistor calibration err\n");
		pr_info("val:%08x\n", val);
		return val;
	}
	//pr_info("Resistor calibration OK\n");

	/* Offset calibration */
	/*clklane*/
	val = csi2_dhpy_read(csi, 0x39d);
	done_flag = val & 0x1;
	err_flag = (val & 0x1E) != 0x10;
	if (!done_flag || !err_flag) {
		dev_err(csi->dev, "clklane calibration err\n");
		pr_info("val:%08x\n", val);
		return val;
	}

	switch (vip->mipi_lanes) {
		case 4: /*datalane3*/
			/* Offset calibration */
			val = csi2_dhpy_read(csi, 0xb9f);
			done_flag = val & 0x02;
			err_flag = val & 0x04;
			if (!done_flag || err_flag) {
				dev_err(csi->dev, "datalane3 calibration err\n");
				pr_info("val:%08x\n", val);
				return val;
			}

			/* DDL calibration */
			val = csi2_dhpy_read(csi, 0xbe0);
			done_flag = val & 0x04;
			if (!done_flag) {
				dev_err(csi->dev, "datalane3 DDL err\n");
				pr_info("val:%08x\n", val);
				return val;
			}

		case 3: /*datalane2*/
			/* Offset calibration */
			val = csi2_dhpy_read(csi, 0x99f);
			done_flag = val & 0x02;
			err_flag = val & 0x04;
			if (!done_flag || err_flag) {
				dev_err(csi->dev, "datalane2 calibration err\n");
				pr_info("val:%08x\n", val);
				return val;
			}

			/* DDL calibration */
			val = csi2_dhpy_read(csi, 0x9e0);
			done_flag = val & 0x04;
			if (!done_flag) {
				dev_err(csi->dev, "datalane2 DDL err\n");
				pr_info("val:%08x\n", val);
				return val;
			}
		case 2: /*datalane1*/
			/* Offset calibration */
			val = csi2_dhpy_read(csi, 0x79f);
			done_flag = val & 0x02;
			err_flag = val & 0x04;
			if (!done_flag || err_flag) {
				dev_err(csi->dev, "datalane1 calibration err\n");
				pr_info("val:%08x\n", val);
				return val;
			}

			/* DDL calibration */
			val = csi2_dhpy_read(csi, 0x7e0);
			done_flag = val & 0x04;
			if (!done_flag) {
				dev_err(csi->dev, "datalane1 DDL err\n");
				pr_info("val:%08x\n", val);
				return val;
			}
		case 1: /*datalane0*/
			/* Offset calibration */
			val = csi2_dhpy_read(csi, 0x59f);
			done_flag = val & 0x02;
			err_flag = val & 0x04;
			if (!done_flag || err_flag) {
				dev_err(csi->dev, "datalane0 calibration err\n");
				pr_info("val:%08x\n", val);
				return val;
			}

			/* DDL calibration */
			val = csi2_dhpy_read(csi, 0x5e0);
			done_flag = val & 0x04;
			if (!done_flag) {
				dev_err(csi->dev, "datalane0 DDL err\n");
				pr_info("val:%08x\n", val);
				return val;
			}
		default:
			break;
	}
	//pr_info("offset & DDL calibration OK\n");

	/* Skew calibration */
	if (vip->mipi_mbps < 1500) {
		switch (vip->mipi_lanes) {
			case 4:
				/*datalane3*/
				val = csi2_dhpy_read(csi, 0xb2f);
				done_flag = val & 0x08;
				if (!done_flag) {
					dev_err(csi->dev, "datalane3 Skew err\n");
					pr_info("val:%08x\n", val);
					return val;
				}
			case 3:
				/*datalane2*/
				val = csi2_dhpy_read(csi, 0x92f);
				done_flag = val & 0x08;
				if (!done_flag) {
					dev_err(csi->dev, "datalane2 Skew err\n");
					pr_info("val:%08x\n", val);
					return val;
				}
			case 2:
				/*datalane1*/
				val = csi2_dhpy_read(csi, 0x72f);
				done_flag = val & 0x08;
				if (!done_flag) {
					dev_err(csi->dev, "datalane1 Skew err\n");
					pr_info("val:%08x\n", val);
					return val;
				}
			case 1:
				/*datalane0*/
				val = csi2_dhpy_read(csi, 0x52f);
				done_flag = val & 0x08;
				if (!done_flag) {
					dev_err(csi->dev, "datalane0 Skew err\n");
					pr_info("val:%08x\n", val);
					return val;
				}
			default:
				break;
		}
	}
	//pr_info("Skew calibration OK\n");

	csi2_dbg(1, debug, "MIPI INIT DONE\n");

	return 0;
}
EXPORT_SYMBOL(imi_csi2_dhpy_calib_check);


void csi_dev_init(void)
{
	struct imi_csi *csi;
	csi = pvPortMalloc(sizeof(struct imi_csi));
	memset(csi, 0, sizeof(struct imi_csi));
	csi->base = (void __iomem*)MIPI_DPHY0_BASE_ADDR;
	csi->csi_id = 0;
	reset_control_get(
"reset", CSI0_TOP_RST, &csi->csi_rst);
	reset_control_get(
"reset", CSI0_IPI_RST, &csi->ipi_rst);
	INIT_LIST_HEAD(&csi->list);
	list_add_tail(&csi->list, &csi_list);

	csi = pvPortMalloc(sizeof(struct imi_csi));
	memset(csi, 0, sizeof(struct imi_csi));
	csi->base = (void __iomem*)MIPI_DPHY1_BASE_ADDR;
	csi->csi_id = 1;
	reset_control_get(
"reset", CSI1_TOP_RST, &csi->csi_rst);
	reset_control_get(
"reset", CSI1_IPI_RST, &csi->ipi_rst);
	INIT_LIST_HEAD(&csi->list);
	list_add_tail(&csi->list, &csi_list);

	csi = pvPortMalloc(sizeof(struct imi_csi));
	memset(csi, 0, sizeof(struct imi_csi));
	csi->base = (void __iomem*)MIPI_DPHY2_BASE_ADDR;
	csi->csi_id = 2;
	reset_control_get(
"reset", CSI2_TOP_RST, &csi->csi_rst);
	reset_control_get(
"reset", CSI2_IPI_RST, &csi->ipi_rst);
	INIT_LIST_HEAD(&csi->list);
	list_add_tail(&csi->list, &csi_list);
	log_i("%s done\n", __func__);

}
EXPORT_SYMBOL(csi_dev_init);

int csi2_remove(struct imi_csi *csi)
{
	reset_control_assert(csi->csi_rst);
	reset_control_assert(csi->ipi_rst);

	clk_disable_unprepare(csi->ipi);
	clk_disable_unprepare(csi->refclk);
	clk_disable_unprepare(csi->cfgclk);
	clk_disable_unprepare(csi->esc);

	return 0;
}

SUBSYS_INIT(csi_dev_init);
#endif /*CHIP_IMI_2280*/
