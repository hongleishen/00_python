/***************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    clock driver

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------

  ===========================================================================
 ***************************************************************************/
#if defined(CHIP_IMI_1680)

#define	PERI_CLOCK_BASE					0x20F00000

#define	WTimer_C					(PERI_CLOCK_BASE + 0x0000)
#define	GPIO_C						(PERI_CLOCK_BASE + 0x0400)
#define	CTimer_C					(PERI_CLOCK_BASE + 0x0800)
#define	SPI_C						(PERI_CLOCK_BASE + 0x0C00)
#define	I2C_C						(PERI_CLOCK_BASE + 0x1000)
#define	UART_C						(PERI_CLOCK_BASE + 0x1400)
#define	MAILBOX_C					(PERI_CLOCK_BASE + 0x1800)
#define	EFUSE_C					    (PERI_CLOCK_BASE + 0x1C00)
#define	ADC_C					    (PERI_CLOCK_BASE + 0x2000)
#define	CPWM_C					    (PERI_CLOCK_BASE + 0x2400)

#define	USB2_C						0x22F00000
#define CRYPTO_C					0x22F00400
#define IPP_C						0x22F00800
#define	DMAQ_C						0x22F01400
#define	MIPITX0_C					0x22F01800
#define	MIPITX1_C					0x22F01C00

#define	ISP_C						0x23F00000

#define	VPU_C						0x24F00000
#define	DD_C						0x24F00400
#define	MIPIRX0_C					0x24F00800
#define	MIPIRX1_C					0x24F00C00

#define	DPU_C						0x25F00000
#define	RVL_C						0x25F00400
#define	RG_C						0x25F00800

#define PLL_BASE					0x20F08800
#define APLL						(PLL_BASE + 0x0000)
#define DPLL						(PLL_BASE + 0x0030)
#define EPLL						(PLL_BASE + 0x0060)
#define VPLL						(PLL_BASE + 0x0090)

#define MCLK_BASE					0x20F088C0
#define CPU_CLK						(MCLK_BASE + 0x02c0)
#define PAPB_CLK					(MCLK_BASE + 0x0010)
#define BUS1_CLK					(MCLK_BASE + 0x0020)
#define BUS2_CLK					(MCLK_BASE + 0x0030)
#define BUS3_CLK					(MCLK_BASE + 0x0040)
#define BUS4_CLK					(MCLK_BASE + 0x0050)
#define MAPB_CLK					(MCLK_BASE + 0x0060)
#define MPU_CLK						(MCLK_BASE + 0x0070)
#define WDT_CLK						(MCLK_BASE + 0x0080)
#define TCLK0						(MCLK_BASE + 0x0090)
#define TCLK1						(MCLK_BASE + 0x00A0)
#define SSP_ECLK					(MCLK_BASE + 0x00B0)
#define I2C_ECLK					(MCLK_BASE + 0x00C0)
#define UART_ECLK					(MCLK_BASE + 0x00D0)
#define USB_REFCLK					(MCLK_BASE + 0x00E0)
#define QSPI_REFCLK					(MCLK_BASE + 0x00F0)
#define MIPI_CCLK					(MCLK_BASE + 0x0100)
#define MIPI_ECLK					(MCLK_BASE + 0x0110)
#define MIPI_RCLK					(MCLK_BASE + 0x0120)
#define MIPI_T0CLK					(MCLK_BASE + 0x0130)
#define MIPI_T1CLK					(MCLK_BASE + 0x0140)
#define RX_IPI0CLK					(MCLK_BASE + 0x0150)
#define RX_IPI1CLK					(MCLK_BASE + 0x0160)
#define VPU_CLK						(MCLK_BASE + 0x0170)
#define VPU_RCLK					(MCLK_BASE + 0x0180)
#define DPU_RCLK					(MCLK_BASE + 0x0190)
#define RG_RCLK						(MCLK_BASE + 0x01A0)
#define PNG_RCLK					(MCLK_BASE + 0x01B0)
#define ADC_RCLK					(MCLK_BASE + 0x01C0)

enum {
	SOURCE_APLL = 0x0,
	SOURCE_DPLL,
	SOURCE_EPLL,
	SOURCE_VPLL,
	SOURCE_OSC,
	SOURCE_EXT,
};

#endif /*CHIP_IMI_1680*/
