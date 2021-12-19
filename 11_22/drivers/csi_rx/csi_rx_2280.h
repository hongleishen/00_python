#ifndef __CSI_RX_2280_H__
#define __CSI_RX_2280_H__
#include "hellboy_map.h"
#define CSI_RX_PLL_ALT0					(600000000UL)
#define CSI_RX_PLL_ALT1					(800000000UL)

//================= MIPI_CSI CFG
#define MIPI_VERSION                  0x00000000
#define MIPI_N_LANES                  0x00000004
#define MIPI_CSI2_RESET               0x00000008
#define MIPI_INT_ST_MAIN              0x0000000C
#define MIPI_DATA_IDS_1               0x00000010
#define MIPI_DATA_IDS_2               0x00000014
#define MIPI_PHY_SHUTDOWNZ            0x00000040
#define MIPI_DPHY_RSTZ                0x00000044
#define MIPI_PHY_RX                   0x00000048
#define MIPI_PHY_STOPSTATE            0x0000004C
#define STOPSTATE_CLK		BIT(16)
#define STOPSTATE_D0		BIT(0)
#define STOPSTATE_D1		BIT(1)
#define STOPSTATE_D2		BIT(2)
#define STOPSTATE_D3		BIT(3)
#define MIPI_PHY_TEST_CTRL0           0x00000050
#define DPHY_TESTCLR		BIT(0)
#define DPHY_TESTCLK		BIT(1)
#define MIPI_PHY_TEST_CTRL1           0x00000054
#define DPHY_TESTEN			BIT(16)
#define MIPI_PHY2_TEST_CTRL0          0x00000058
#define MIPI_PHY2_TEST_CTRL1          0x0000005C
#define MIPI_IPI_MODE                 0x00000080
#define IPI_48BIT_MODE		(0 << 8)
#define IPI_16BIT_MODE		(1 << 8)
#define CAMERA_TIMING		(0 << 0)
#define CONTROLLER_TIMING	(1 << 0)

#define MIPI_IPI_VCID                 0x00000084
#define MIPI_IPI_DATA_TYPE            0x00000088
#define MIPI_IPI_MEM_FLUSH            0x0000008C
#define MIPI_IPI_HSA_TIME             0x00000090
#define MIPI_IPI_HBP_TIME             0x00000094
#define MIPI_IPI_HSD_TIME             0x00000098
#define MIPI_IPI_HLINE_TIME           0x0000009C
#define MIPI_IPI_VSA_LINES            0x000000B0
#define MIPI_IPI_VBP_LINES            0x000000B4
#define MIPI_IPI_VFP_LINES            0x000000B8
#define MIPI_IPI_VACTIVE_LINES        0x000000BC
#define MIPI_PHY_CAL                  0x000000CC
#define MIPI_INT_ST_PHY_FATAL         0x000000E0
#define MIPI_INT_MSK_PHY_FATAL        0x000000E4
#define MIPI_INT_FORCE_PHY_FATAL      0x000000E8
#define MIPI_INT_ST_PKT_FATAL         0x000000F0
#define MIPI_INT_MSK_PKT_FATAL        0x000000F4
#define MIPI_INT_FORCE_PKT_FATAL      0x000000F8
#define MIPI_INT_ST_FRAME_FATAL       0x00000100
#define MIPI_INT_MSK_FRAME_FATAL      0x00000104
#define MIPI_INT_FORCE_FRAME_FATAL    0x00000108
#define MIPI_INT_ST_PHY               0x00000110
#define MIPI_INT_MSK_PHY              0x00000114
#define MIPI_INT_FORCE_PHY            0x00000118
#define MIPI_INT_ST_PKT               0x00000120
#define MIPI_INT_MSK_PKT              0x00000124
#define MIPI_INT_FORCE_PKT            0x00000128
#define MIPI_INT_ST_LINE              0x00000130
#define MIPI_INT_MSK_LINE             0x00000134
#define MIPI_INT_FORCE_LINE           0x00000138
#define MIPI_INT_ST_IPI               0x00000140
#define MIPI_INT_MSK_IPI              0x00000144
#define MIPI_INT_FORCE_IPI            0x00000148
#endif/*__CSI_RX_2280_H__*/
