#ifndef __IMI_VIDOMAIN__
#define __IMI_VIDOMAIN__

#include "linux_compat.h"

//================= VI_CFG
#define VI_TOP_CTRL                   0x00000000
#define VI_CSI_0_IPI_CTRL             0x00000004
#define VI_CSI_1_IPI_CTRL             0x00000008
#define VI_CSI_2_IPI_CTRL             0x0000000C
#define IPI_8BIT_DATA		(0 << 4)
#define IPI_10BIT_DATA		(1 << 4)
#define IPI_12BIT_DATA		(2 << 4)
#define IPI_LOW_16BIT		(0 << 6)
#define IPI_MID_16BIT		(1 << 6)
#define IPI_HIGH_16BIT		(2 << 6)

#define VI_DMA_IRQ_EN                 0x00000010
#define VI_MAIN_DMA_Y_CNT             0x00000014
#define VI_MAIN_DMA_U_CNT             0x00000018
#define VI_MAIN_DMA_V_CNT             0x0000001C
#define VI_SELF_DMA_Y_CNT             0x00000020
#define VI_SELF_DMA_U_CNT             0x00000024
#define VI_SELF_DMA_V_CNT             0x00000028
#define VI_MIPI_OUT_EN                0x0000002C
#define MIPI0_ENABLE		(3 << 0)
#define MIPI1_ENABLE		(3 << 2)
#define MIPI2_ENABLE		(3 << 4)

#define VI_SKIP_FRAME                 0x00000030
#define VI_MAIN_PIC_OUT_SIZE          0x00000034
#define VI_SELF_PIC_OUT_SIZE          0x00000038

#define VI_CSI_0_DPHY_CTRL            0x00000040
#define PHY_CLK_ENABLE		BIT(0)
#define PHY_DIR_TX			(0 << 1)
#define PHY_DIR_RX			(1 << 1)

#define VI_CSI_0_DPHY_LANE0           0x00000044
#define VI_CSI_0_DPHY_PLL_REG0        0x00000048
#define VI_CSI_0_DPHY_PLL_REG1        0x0000004C
#define VI_CSI_0_DPHY_PLL_REG2        0x00000050

#define VI_CSI_1_DPHY_CTRL            0x00000054
#define VI_CSI_1_DPHY_LANE0           0x00000058
#define VI_CSI_1_DPHY_PLL_REG0        0x0000005C
#define VI_CSI_1_DPHY_PLL_REG1        0x00000060
#define VI_CSI_1_DPHY_PLL_REG2        0x00000064

#define VI_CSI_2_DPHY_CTRL            0x00000068
#define VI_CSI_2_DPHY_LANE0           0x0000006C
#define VI_CSI_2_DPHY_PLL_REG0        0x00000070
#define VI_CSI_2_DPHY_PLL_REG1        0x00000074
#define VI_CSI_2_DPHY_PLL_REG2        0x00000078

#define VI_CSI_3_DPHY_CTRL            0x0000007C
#define VI_CSI_3_DPHY_LANE0           0x00000080
#define VI_CSI_3_DPHY_LANE1           0x00000084
#define VI_CSI_3_DPHY_LANE2           0x00000088
#define VI_CSI_3_DPHY_LANE3           0x0000008C

#define VI_CSI_0_DMA_Y_BASEADDR       0x000000CC
#define VI_CSI_0_DMA_C_BASEADDR       0x000000D0
#define VI_CSI_0_DMA_V_SIZE           0x000000D4
#define VI_CSI_0_DMA_H_SIZE           0x000000D8

#define VI_CSI_1_DMA_Y_BASEADDR       0x000000DC
#define VI_CSI_1_DMA_C_BASEADDR       0x000000E0
#define VI_CSI_1_DMA_V_SIZE           0x000000E4
#define VI_CSI_1_DMA_H_SIZE           0x000000E8

#define VI_CSI_2_DMA_Y_BASEADDR       0x000000EC
#define VI_CSI_2_DMA_C_BASEADDR       0x000000F0
#define VI_CSI_2_DMA_V_SIZE           0x000000F4
#define VI_CSI_2_DMA_H_SIZE           0x000000F8

#define VI_CSI_3_DMA_Y_BASEADDR       0x000000FC
#define VI_CSI_3_DMA_C_BASEADDR       0x00000100
#define VI_CSI_3_DMA_V_SIZE           0x00000104
#define VI_CSI_3_DMA_H_SIZE           0x00000108

#define VI_CSI_4_DMA_Y_BASEADDR       0x0000010C
#define VI_CSI_4_DMA_C_BASEADDR       0x00000110
#define VI_CSI_4_DMA_V_SIZE           0x00000114
#define VI_CSI_4_DMA_H_SIZE           0x00000118

#define VI_CSI_5_DMA_Y_BASEADDR       0x0000011C
#define VI_CSI_5_DMA_C_BASEADDR       0x00000120
#define VI_CSI_5_DMA_V_SIZE           0x00000124
#define VI_CSI_5_DMA_H_SIZE           0x00000128

#define VI_CSI_6_DMA_Y_BASEADDR       0x0000012C
#define VI_CSI_6_DMA_C_BASEADDR       0x00000130
#define VI_CSI_6_DMA_V_SIZE           0x00000134
#define VI_CSI_6_DMA_H_SIZE           0x00000138

#define VI_CSI_0_FRAME_SIZE           0x00000140
#define VI_CSI_1_FRAME_SIZE           0x00000144
#define VI_CSI_2_FRAME_SIZE           0x00000148
//==== END

#define CSI_NAME_SIZE	20

struct vi_adapter {
	struct device *dev;

//	struct clk *pclk;

	void __iomem *base;
};

typedef enum interface_type {
	INTF_DVP = 0,
	INTF_MIPI0,
	INTF_MIPI1,
	INTF_MIPI2,
	INTF_MAX,
} intf_type_t;

/* consumer */
#define IMI_DPU0			0x0
#define IMI_DPU1			0x1
#define IMI_DPU2			0x2
#define IMI_ISP				0x3

/* src_sel */
#define SRC_SEL_MIPI0		0
#define SRC_SEL_MIPI1		1
#define SRC_SEL_MIPI2		2
#define SRC_SEL_DVP			3

/* mipi_dt */
#define MIPI_8BIT		0x2a
#define MIPI_10BIT		0x2b
#define MIPI_12BIT		0x2c

struct vi_param {
	unsigned int consumer;
	unsigned int src_sel;
	unsigned int mipi_dt;
	unsigned int mipi_lanes;
	unsigned int mipi_mbps;
	unsigned int img_width;
	unsigned int img_height;
};

int imi_request_vi_domain(struct vi_param *vip);
int imi_release_vi_domain(struct vi_param *vip);
int init_mipi_itf_ir(void      *data);

#if 0//suntongce
static inline int clk_parent_select(struct clk *clk, const char *name)
{
	struct clk *parent;

	parent = __clk_lookup(name);
	if (!parent) {
		pr_err("%s: could not find clock %s\n", __func__, name);
		return -ENODATA;
	}

	if (!clk_has_parent(clk, parent)) {
		pr_err("%s: can not find parent %s\n", __func__, name);
		return -ENODATA;
	}

	clk_set_parent(clk, parent);

	return 0;
}
#endif

#endif

