/***************************************************************************
  Copyright (c) 2019 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    mipi private header
  @author   Dejie Zhang

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------
  2019/03/21   Dejie Zhang   the initial version

===========================================================================
***************************************************************************/
#include "hellboy_map.h"

/*================= MIPI CTL CFG =================*/
#define MIPI_VERSION					0x00000000
#define MIPI_N_LANES					0x00000004
#define MIPI_CSI2_RESET					0x00000008
#define MIPI_INT_ST_MAIN				0x0000000C
#define MIPI_DATA_IDS_1					0x00000010
#define MIPI_DATA_IDS_2					0x00000014
#define MIPI_PHY_SHUTDOWNZ				0x00000040
#define MIPI_DPHY_RSTZ					0x00000044
#define MIPI_PHY_RX						0x00000048
#define MIPI_PHY_STOPSTATE				0x0000004C
#define STOPSTATE_CLK					BIT(16)
#define STOPSTATE_D0					BIT(0)
#define STOPSTATE_D1					BIT(1)
#define STOPSTATE_D2					BIT(2)
#define STOPSTATE_D3					BIT(3)
#define MIPI_PHY_TEST_CTRL0				0x00000050
#define DPHY_TESTCLR					BIT(0)
#define DPHY_TESTCLK					BIT(1)
#define MIPI_PHY_TEST_CTRL1				0x00000054
#define DPHY_TESTEN						BIT(16)
#define MIPI_PHY2_TEST_CTRL0			0x00000058
#define MIPI_PHY2_TEST_CTRL1			0x0000005C
#define MIPI_IPI_MODE					0x00000080
#define IPI_48BIT_MODE					(0 << 8)
#define IPI_16BIT_MODE					(1 << 8)
#define CAMERA_TIMING					(0 << 0)
#define CONTROLLER_TIMING				(1 << 0)

#define MIPI_IPI0_VCID					0x00000084
#define MIPI_IPI_DATA_TYPE				0x00000088
#define MIPI_IPI_MEM_FLUSH				0x0000008C
#define MIPI_IPI_HSA_TIME				0x00000090
#define MIPI_IPI_HBP_TIME				0x00000094
#define MIPI_IPI_HSD_TIME				0x00000098
#define MIPI_IPI_HLINE_TIME				0x0000009C
#define MIPI_IPI_VSA_LINES				0x000000B0
#define MIPI_IPI_VBP_LINES				0x000000B4
#define MIPI_IPI_VFP_LINES				0x000000B8
#define MIPI_IPI_VACTIVE_LINES			0x000000BC
#define MIPI_PHY_CAL					0x000000CC
#define MIPI_INT_ST_PHY_FATAL			0x000000E0
#define MIPI_INT_MSK_PHY_FATAL			0x000000E4
#define MIPI_INT_FORCE_PHY_FATAL		0x000000E8
#define MIPI_INT_ST_PKT_FATAL			0x000000F0
#define MIPI_INT_MSK_PKT_FATAL			0x000000F4
#define MIPI_INT_FORCE_PKT_FATAL		0x000000F8
#define MIPI_INT_ST_FRAME_FATAL			0x00000100
#define MIPI_INT_MSK_FRAME_FATAL		0x00000104
#define MIPI_INT_FORCE_FRAME_FATAL		0x00000108
#define MIPI_INT_ST_PHY					0x00000110
#define MIPI_INT_MSK_PHY				0x00000114
#define MIPI_INT_FORCE_PHY				0x00000118
#define MIPI_INT_ST_PKT					0x00000120
#define MIPI_INT_MSK_PKT				0x00000124
#define MIPI_INT_FORCE_PKT				0x00000128
#define MIPI_INT_ST_LINE				0x00000130
#define MIPI_INT_MSK_LINE				0x00000134
#define MIPI_INT_FORCE_LINE				0x00000138
#define MIPI_INT_ST_IPI					0x00000140
#define MIPI_INT_MSK_IPI				0x00000144
#define MIPI_INT_FORCE_IPI				0x00000148
#define LANE1_RX						(1 << 17)

/*================ MIPI EXTEN CFG ================*/
#define MIPI_IPI1_MEM_FLUSH				0x00000070
#define MIPI_IPI2_MEM_FLUSH				0x00000074
#define MIPI_IPI1_VCID					0x00000078
#define MIPI_IPI2_VCID					0x0000007c

#define MIPI_IPI1_INT_ST				0x00000150
#define MIPI_IPI1_INT_MASK				0x00000154
#define MIPI_IPI1_INT_FORCE				0x00000158
#define MIPI_IPI2_INT_ST				0x00000160
#define MIPI_IPI2_INT_MASK				0x00000164
#define MIPI_IPI2_INT_FORCE				0x00000168

/*================= MIPI TOP CFG =================*/
#define TOP_DPHY_CTRL				0x00000000
#define PHY_CLK_ENABLE				(1 << 0)
#define PHY_DIR_TX					(0 << 1)
#define PHY_DIR_RX					(1 << 1)

#define TOP_PHY_LAN0ESC_CTRL		0x00000004
#define TOP_PLL_CTRL0				0x00000008
#define TOP_PLL_CTRL1				0x0000000c
#define TOP_PLL_CTRL2				0x00000010

#define TOP_IPI0_CTRL				0x00000040
#define IPI_ENABLE					(1 << 0)
#define IPI_8BIT_DATA				(0 << 1)
#define IPI_10BIT_DATA				(1 << 1)
#define IPI_12BIT_DATA				(2 << 1)
#define IPI_LOW_16BIT				(0 << 3)
#define IPI_MID_16BIT				(1 << 3)
#define IPI_HIGH_16BIT				(2 << 3)

#define TOP_IPI0_FRAMESIZE			0x00000044
#define TOP_IPI0_VSYNCDELAY			0x00000048
#define TOP_IPI0_INT_ENABLE			0x0000004c
#define TOP_IPI0_INT_CLEAR			0x00000050
#define TOP_IPI0_INT_STATUS			0x00000054
#define TOP_IPI0_INT_PENDDING		0x00000058
#define TOP_IPI0_FRAME_COUNT_IN		0x0000005c
#define TOP_IPI0_PIXEL_COUNT_IN		0x00000060
#define TOP_IPI0_FRAME_COUNT_OUT	0x00000064
#define TOP_IPI0_PIXEL_COUNT_OUT	0x00000068

#define TOP_IPI1_CTRL				0x00000080
#define TOP_IPI1_FRAMESIZE			0x00000084
#define TOP_IPI1_VSYNCDELAY			0x00000088
#define TOP_IPI1_INT_ENABLE			0x0000008c
#define TOP_IPI1_INT_CLEAR			0x00000090
#define TOP_IPI1_INT_STATUS			0x00000094
#define TOP_IPI1_INT_PENDDING		0x00000098
#define TOP_IPI1_FRAME_COUNT_IN		0x0000009c
#define TOP_IPI1_PIXEL_COUNT_IN		0x000000a0
#define TOP_IPI1_FRAME_COUNT_OUT	0x000000a4
#define TOP_IPI1_PIXEL_COUNT_OUT	0x000000a8

#define TOP_IPI2_CTRL				0x000000c0
#define TOP_IPI2_FRAMESIZE			0x000000c4
#define TOP_IPI2_VSYNCDELAY			0x000000c8
#define TOP_IPI2_INT_ENABLE			0x000000cc
#define TOP_IPI2_INT_CLEAR			0x000000d0
#define TOP_IPI2_INT_STATUS			0x000000d4
#define TOP_IPI2_INT_PENDDING		0x000000d8
#define TOP_IPI2_FRAME_COUNT_IN		0x000000dc
#define TOP_IPI2_PIXEL_COUNT_IN		0x000000e0
#define TOP_IPI2_FRAME_COUNT_OUT	0x000000e4
#define TOP_IPI2_PIXEL_COUNT_OUT	0x000000e8


/*================ MIPI CAMIF CFG ================*/
#define CMIF_CHN0_SEN_IF_0			0x00000000
#define CMIF_CHN0_SEN_IF_1			0x00000004
#define SEN_IF_EN_HCLK				(1 << 26)
#define RAW_ENABLE					(1 << 28)
#define RAW_8BIT					(0 << 29)
#define RAW_10BIT					(1 << 29)
#define RAW_12BIT					(2 << 29)
#define CMIF_CHN0_WIN_LEFT_UP		0x00000010
#define CMIF_CHN0_WIN_RIGHT_DOWN	0x00000014
#define CMIF_CHN0_ADR_00			0x00000030
#define CMIF_CHN1_SEN_IF_0			0x00000100
#define CMIF_CHN1_SEN_IF_1			0x00000104
#define CMIF_CHN1_WIN_LEFT_UP		0x00000110
#define CMIF_CHN1_WIN_RIGHT_DOWN	0x00000114
#define CMIF_CHN1_ADR_00			0x00000130
#define CMIF_CHN2_SEN_IF_0			0x00000200
#define CMIF_CHN2_SEN_IF_1			0x00000204
#define CMIF_CHN2_WIN_LEFT_UP		0x00000210
#define CMIF_CHN2_WIN_RIGHT_DOWN	0x00000214
#define CMIF_CHN2_ADR_00			0x00000230
#define CMIF_CHN3_SEN_IF_0			0x00000300
#define CMIF_CHN3_SEN_IF_1			0x00000304
#define CMIF_CHN3_WIN_LEFT_UP		0x00000310
#define CMIF_CHN3_WIN_RIGHT_DOWN	0x00000314
#define CMIF_CHN3_ADR_00			0x00000330
#define CMIF_INT_STATUS				0x000000F0

