#if defined(CHIP_IMI_2280)
#include "hellboy_map.h"
#include "imi_vidomain.h"
#include "csi_rx.h"
#include "linux_common.h"
#include "irrx.h"

#define DEVICE_NAME "vi_domain"

static struct vi_adapter *gvia;
static DEFINE_MUTEX(vi_mutex);

static inline void vi_write_reg(struct vi_adapter *vi,
	unsigned int reg, unsigned int val)
{
	writel(val, vi->base + reg);
}

static inline unsigned int vi_read_reg(struct vi_adapter *vi,
	unsigned int reg)
{
	int val;
	val = readl(vi->base + reg);
	return val;
}

static inline void vi_update_reg(struct vi_adapter *vi,
	unsigned int reg, unsigned int mask, unsigned int val)
{
	unsigned int reg_val;

	reg_val = vi_read_reg(vi, reg);
	reg_val = (reg_val & ~mask) | (val & mask);
	vi_write_reg(vi, reg, reg_val);
}

#if 0
static void vi_domain_isp_sclk_setup(struct vi_adapter *via,
	struct vi_param *vip)
{
	struct clk *mux_sclk = __clk_lookup("mout_u0_isp_sclk");

	if (IMI_ISP == vip->consumer) {
		if (SRC_SEL_MIPI0 == vip->src_sel)
			clk_parent_select(mux_sclk, "u0_csi_ipi_clk");
		else if (SRC_SEL_MIPI1 == vip->src_sel)
			clk_parent_select(mux_sclk, "u1_csi_ipi_clk");
		else if (SRC_SEL_MIPI2 == vip->src_sel)
			clk_parent_select(mux_sclk, "u2_csi_ipi_clk");

		clk_prepare_enable(via->isp_sclk);
	}
}
#endif

static void vi_domain_mux_ctrl(struct vi_adapter *via,
	struct vi_param *vip)
{
	switch (vip->consumer) {
	case IMI_DPU0:
		/*00:DVP0, 01:MIPI0, 02:MIPI1, 03:MIPI2*/
		if (SRC_SEL_DVP == vip->src_sel)
			vi_update_reg(via, VI_TOP_CTRL, 0x30, 0 << 4);
		else
			vi_update_reg(via, VI_TOP_CTRL, 0x30, (vip->src_sel + 1) << 4);
		break;
	case IMI_DPU1:
		vi_update_reg(via, VI_TOP_CTRL, 0xc0, (vip->src_sel) << 6);
		break;
	case IMI_DPU2:
		vi_update_reg(via, VI_TOP_CTRL, 0x300, (vip->src_sel) << 8);
		break;
	case IMI_ISP:
		vi_update_reg(via, VI_TOP_CTRL, 0xc00, (vip->src_sel) << 10);
		break;
	default:
		break;
	}
}

static void vi_domain_ipi2dvp(struct vi_adapter *via,
	struct vi_param *vip)
{
	unsigned int ctrl;

	if (MIPI_8BIT == vip->mipi_dt)
		ctrl = IPI_8BIT_DATA | IPI_HIGH_16BIT;
	else if (MIPI_10BIT == vip->mipi_dt)
		ctrl = IPI_10BIT_DATA | IPI_HIGH_16BIT;
	else if (MIPI_12BIT == vip->mipi_dt)
		ctrl = IPI_12BIT_DATA | IPI_HIGH_16BIT;

	switch (vip->src_sel) {
	case SRC_SEL_MIPI0:
		vi_update_reg(via, VI_MIPI_OUT_EN, 0x03, MIPI0_ENABLE);
		vi_write_reg(via, VI_CSI_0_IPI_CTRL, ctrl);
		vi_write_reg(via, VI_CSI_0_FRAME_SIZE,
				vip->img_height | (vip->img_width << 16));
		break;
	case SRC_SEL_MIPI1:
		vi_update_reg(via, VI_MIPI_OUT_EN, 0x0c, MIPI1_ENABLE);
		vi_write_reg(via, VI_CSI_1_IPI_CTRL, ctrl);
		vi_write_reg(via, VI_CSI_1_FRAME_SIZE,
				vip->img_height | (vip->img_width << 16));
		break;
	case SRC_SEL_MIPI2:
		vi_update_reg(via, VI_MIPI_OUT_EN, 0x30, MIPI2_ENABLE);
		vi_write_reg(via, VI_CSI_2_IPI_CTRL, ctrl);
		vi_write_reg(via, VI_CSI_2_FRAME_SIZE,
				vip->img_height | (vip->img_width << 16));
		break;
	default:
		break;
	}
}

#if 0
static void vi_domain_dvp_init(struct vi_adapter *via,
	struct vi_param *vip)
{
	if (IMI_ISP == vip->consumer) {
		clk_parent_select(via->mout_mclk, "mout_main_root");
		clk_set_rate(via->cmif_mclk, 24000000);
		clk_prepare_enable(via->cmif_mclk);

		clk_parent_select(via->mout_sclk, "pad_pixclk2");

		clk_parent_select(via->mout_isp, "pll1_clk");
		clk_set_rate(via->isp_clk, 200000000);
		clk_prepare_enable(via->isp_clk);

		clk_parent_select(via->mout_jpeg, "pll1_clk");
		clk_set_rate(via->isp_jpeg, 400000000);
		clk_prepare_enable(via->isp_jpeg);
	}
}
#endif

static int vi_domain_dhpy_cfg_rst(struct vi_adapter *via,
	struct imi_csi *icsi, struct vi_param *vip)
{
	unsigned int hsfreqrange, osc_freq;
#if x //suntongce warning
	unsigned int cfg_rate =
		clk_get_rate(icsi->cfgclk) / 1000000;
#else
	unsigned int cfg_rate = 24;
#endif
	unsigned int cfgclkfreqrange = (cfg_rate - 17) * 4;
	int ret = 0;

	imi_csi_calc_freq(vip->mipi_mbps, &hsfreqrange, &osc_freq);

	if (SRC_SEL_MIPI0 == vip->src_sel) {
		vi_write_reg(via, VI_CSI_0_DPHY_CTRL,
			PHY_CLK_ENABLE | PHY_DIR_RX |
			(cfgclkfreqrange << 2) | (hsfreqrange << 10));
		vi_write_reg(via, VI_CSI_0_DPHY_LANE0, 0);
	} else if (SRC_SEL_MIPI1 == vip->src_sel) {
		vi_write_reg(via, VI_CSI_1_DPHY_CTRL,
			PHY_CLK_ENABLE | PHY_DIR_RX |
			(cfgclkfreqrange << 2) | (hsfreqrange << 10));
		vi_write_reg(via, VI_CSI_1_DPHY_LANE0, 0);
	} else if (SRC_SEL_MIPI2 == vip->src_sel) {
		vi_write_reg(via, VI_CSI_2_DPHY_CTRL,
			PHY_CLK_ENABLE | PHY_DIR_RX |
			(cfgclkfreqrange << 2) | (hsfreqrange << 10));
		vi_write_reg(via, VI_CSI_2_DPHY_LANE0, 0);
	}

	ret = imi_csi2_dhpy_reset(icsi, cfgclkfreqrange, osc_freq, vip->mipi_lanes);
	if (ret) {
		csi2_debug("imi_csi2_dhpy_reset fail, ret=%d\n", ret);
		return ret;
	}

	return 0;
}

static int vi_domain_csi_init(struct vi_adapter *via,
	struct imi_csi *icsi, struct vi_param *vip)
{
	int ret;

	imi_csi2_start(icsi, vip);
	vi_domain_ipi2dvp(via, vip);

	imi_csi2_start_reset(icsi);
	ret = vi_domain_dhpy_cfg_rst(via, icsi, vip);
	ret = imi_csi2_dhpy_calib_check(icsi, vip);

	return ret;
}

int imi_request_vi_domain(struct vi_param *vip)
{
	int ret;
	struct imi_csi *icsi;

	if (!gvia) {
		pr_err("can not get vidomain dev\n");
		return -ENODEV;
	}

	mutex_lock(&vi_mutex);
	vi_domain_mux_ctrl(gvia, vip);

	if (SRC_SEL_DVP == vip->src_sel) {
		mutex_unlock(&vi_mutex);
		return 0;
	}

	icsi = imi_get_csi(vip->src_sel);
	if (!icsi) {
		pr_err("can not get csi dev\n");
		mutex_unlock(&vi_mutex);
		return -ENODEV;
	}

//	csi2_debug("%s, index:%d\n", icsi->dev->kobj.name, icsi->csi_id);

	ret = vi_domain_csi_init(gvia, icsi, vip);
	mutex_unlock(&vi_mutex);

	return ret;
}
EXPORT_SYMBOL(imi_request_vi_domain);


int imi_release_vi_domain(struct vi_param *vip)
{
	struct imi_csi *icsi;

	mutex_lock(&vi_mutex);

	if (SRC_SEL_DVP == vip->src_sel) {
		mutex_unlock(&vi_mutex);
		return 0;
	}

	icsi = imi_get_csi(vip->src_sel);
	if (!icsi) {
		pr_err("can not get csi dev\n");
		return -ENODEV;
	}

	imi_csi2_stop(icsi);
	mutex_unlock(&vi_mutex);

	csi2_debug("%s ok, csi index:%d\n", __func__, icsi->csi_id);

	return 0;
}
#if 0
static ssize_t request_vi_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int val, ret;
	struct vi_param vip;

	vip.consumer = IMI_ISP;
	vip.src_sel = SRC_SEL_MIPI0;
	vip.mipi_dt = MIPI_10BIT;
	vip.mipi_lanes = 2;
	vip.mipi_mbps = 800;
	vip.img_height = 960;
	vip.img_width = 1280;

	ret = kstrtouint(buf, 10, &val);
	if (ret)
		return ret;

	if (val)
		imi_request_vi_domain(&vip);
	else
		imi_release_vi_domain(&vip);

	return count;
}


static DEVICE_ATTR(request_vi, 0644, NULL, request_vi_store);

static struct attribute *vi_domain_debug_attrs[] = {
	&dev_attr_request_vi.attr,
	NULL,
};

static struct attribute_group vi_domain_attr_group = {
	.name = "vi_domain",
	.attrs = vi_domain_debug_attrs,
};
#endif
void vi_domain_probe(void)
{
	//vi_domain_get_clk
	//vi_domain_get_base_addr
	mutex_init(&vi_mutex);
	imi_printf("%s\r\n", __func__);
	gvia = pvPortMalloc(sizeof(struct vi_adapter));
	memset(gvia, 0, sizeof(*gvia));
	gvia->base = (void __iomem *)VI_BASE_ADDR;
	return;
}
SUBSYS_INIT(vi_domain_probe);

int vi_domain_set_resolution(struct irrx_config *cfg)
{
	return 0;
}
EXPORT_SYMBOL(vi_domain_set_resolution);

int init_mipi_itf_ir(void *data)
{
	struct irrx_config *cfg = (struct irrx_config *)data;
	int ret;
	struct vi_param ir_vi_param;
	memset(&ir_vi_param, 0, sizeof(ir_vi_param));
	ir_vi_param.consumer = IMI_DPU0;
	ir_vi_param.src_sel = IR_SNR_INTERFACE;
	ir_vi_param.mipi_dt = MIPI_10BIT;
	ir_vi_param.mipi_lanes = IR_SNR_MIPI_LANE;

	ir_vi_param.mipi_mbps = cfg->setting.mipi_mbps;
	ir_vi_param.img_width = cfg->res.in_w;
	ir_vi_param.img_height = cfg->res.in_h;
	ret = imi_request_vi_domain(&ir_vi_param);

	return ret;
}
EXPORT_SYMBOL(init_mipi_itf_ir);


#endif/*CHIP_IMI_2280*/
