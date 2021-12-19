#ifndef _IMI_SPI_H
#define _IMI_SPI_H
#include "imi_type.h"

#define bit8_test 1

#define MAX_GPIO_PER_BANK			(32)
#define REG_CTRL_OFFSET(n)		(0x8 + n * 8)
#define REG_STATUS_OFFSET(n)		(0xC + n * 8)

#define BASE_GPIO_CTRL(x, y)	\
	(REG_GPIO0_BASE + (x) * 0x100000 +	\
	 REG_CTRL_OFFSET(y))

#define BASE_GPIO_STATUS(x, y)	\
	(REG_GPIO0_BASE + (x) * 0x100000 +	\
	 REG_STATUS_OFFSET(y))

#define BITS_PER_WORD		8
#define SPI_FIFO_SIZE		16

#define min(a, b) (((a) < (b)) ? (a) : (b))

#if ((defined TARGET_SRAM) || (defined TARGET_DDR))
#define SPI0_REG_BASE		0x31300000
#define SPI1_REG_BASE		0x31400000
#define CS_REG_BASE			0x30000020
#define REG_GPIO0_BASE 		0x30600000
#define REG_GPIO1_BASE 		0x30700000
#define REG_GPIO2_BASE 		0x30800000
#define SPI0_REG_RST		0x30200008
#define SPI0_REG_CLR		0x30200108


#else
#define  SPI0_REG_BASE           0x20300000
#define  SPI1_REG_BASE           0x20301000
#define  REG_GPIO0_BASE 		 0x20100000

#endif

#define SSPRST			BIT(8)

#define SSPCR0			0x0000
#define SSPCR0_FRF_SHIFT	4
#define SSPCR0_MODE_SHIFT	6
#define SSPCR0_SCR_SHIFT	8

#define SSPIIR			0x0014
#define SSPIIR_RIS		BIT(0)
#define SSPIIR_TIS		BIT(1)
#define SSPIIR_RORIS		BIT(2)
#define SSPICR			SSPIIR

#define SPI_CLK_INPUT		60000000

#define SSPCR1			0x0004
#define SSPCR1_RIE		BIT(0)
#define SSPCR1_TIE		BIT(1)
#define SSPCR1_RORIE		BIT(2)
#define SSPCR1_LBM		BIT(3)
#define SSPCR1_SSE		BIT(4)
#define SSPCR1_MS		BIT(5)
#define SSPCR1_SOD		BIT(6)

#define SSPSR			0x000c
#define SSPSR_TFE		BIT(0)
#define SSPSR_TNF		BIT(1)
#define SSPSR_RNE		BIT(2)
#define SSPSR_RFF		BIT(3)
#define SSPSR_BSY		BIT(4)
#define SSPCPSR			0x0010

#define SSPDR			0x0008
#define SPI_TIMEOUT_MS		1

enum {
	SPI_ID0,
	SPI_ID1
};

enum {
	SSI_MOTO_SPI = 0,
	SSI_TI_SSP,
	SSI_NS_MICROWIRE
};

enum {
	SPI_CPHA0CPOL0,
	SPI_CPHA0CPOL1,
	SPI_CPHA1CPOL0,
	SPI_CPHA1CPOL1,
	SPI_TIMODE,
};

struct spi_info {
	unsigned int rate;
	unsigned int id;
	unsigned int mode;
};
//void imi2280_spi_dma_rx_test(U32 bytelen,  void *dout, void *din,struct test_cfg config);
//void imi2280_spi_dma_tx_test(U32 bytelen,  void *dout, void *din,struct test_cfg config);
extern int dma_init(u32 addr);
extern void imi_udelay(U32 cycles);
extern void imi_mdelay(U32 cycles);
extern int imi2280_spi_master_init(struct spi_info *info);
extern int imi2280_spi_slave_init(struct spi_info *info);
extern void imi2280_spi_master_xfer(U32 bytelen,  void *dout, void *din);
extern void imi2280_spi_loop_test(U32 bytelen,  void *dout, void *din);
extern void spi_clear_irq(void);

#endif
