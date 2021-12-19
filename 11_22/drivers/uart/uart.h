#ifndef __UART_H__
#define __UART_H__
#include "hellboy_map.h"

#define BAUD_RATE					115200
#define USE_UART1					0

#ifdef TARGET_FPGA
//#define UART_BASE_ADDR				(0x20500000)
//#define UART1_BASE_ADDR				(0x20500000+0x1000)  //uart1 base addr 4kB offset
#define UART_CLK					20000000
#endif	/* TARGET_FPGA */

#ifdef TARGET_SRAM
#define	UART_BASE_ADDR				UART0_BASE_ADDR
#define UART_CLK					24000000
#endif	/* TARGET_SRAM */

#ifdef TARGET_DDR
#define	UART_BASE_ADDR				UART0_BASE_ADDR
#define UART_CLK					24000000
#endif	/* TARGET_DDR */

#ifdef TARGET_FPGA_SIMULATION
#define UART_BASE_ADDR				0x20500000
//#define UART1_BASE_ADDR				(0x20501000)
#define UART_CLK					20000000
#endif	/* TARGET_FPGA */

#define UART_PL011_REG_DR			(UART0_BASE_ADDR)
#define UART_PL011_REG_ECR			(UART0_BASE_ADDR + 0x4)
#define UART_PL011_REG_FR			(UART0_BASE_ADDR + 0x18)
#define UART_PL011_REG_ILPR			(UART0_BASE_ADDR + 0X20)
#define UART_PL011_REG_IBRD			(UART0_BASE_ADDR + 0x24)
#define UART_PL011_REG_FBRD			(UART0_BASE_ADDR + 0x28)
#define UART_PL011_REG_LCRH			(UART0_BASE_ADDR + 0x2C)
#define UART_PL011_REG_CR			(UART0_BASE_ADDR + 0x30)
#define UART_PL011_REG_IFLS			(UART0_BASE_ADDR + 0x34)
#define UART_PL011_REG_IMSC			(UART0_BASE_ADDR + 0x38)
#define UART_PL011_REG_RIS			(UART0_BASE_ADDR + 0x3C)
#define UART_PL011_REG_ICR			(UART0_BASE_ADDR + 0x44)
#define UART_PL011_REG_DMACR		(UART0_BASE_ADDR + 0x48)

#define UART1_PL011_REG_DR			(UART1_BASE_ADDR)
#define UART1_PL011_REG_ECR			(UART1_BASE_ADDR + 0x4)
#define UART1_PL011_REG_FR			(UART1_BASE_ADDR + 0x18)
#define UART1_PL011_REG_ILPR		(UART1_BASE_ADDR + 0X20)
#define UART1_PL011_REG_IBRD		(UART1_BASE_ADDR + 0x24)
#define UART1_PL011_REG_FBRD		(UART1_BASE_ADDR + 0x28)
#define UART1_PL011_REG_LCRH		(UART1_BASE_ADDR + 0x2C)
#define UART1_PL011_REG_CR			(UART1_BASE_ADDR + 0x30)
#define UART1_PL011_REG_IFLS		(UART1_BASE_ADDR + 0x34)
#define UART1_PL011_REG_IMSC		(UART1_BASE_ADDR + 0x38)
#define UART1_PL011_REG_RIS			(UART1_BASE_ADDR + 0x3C)
#define UART1_PL011_REG_ICR			(UART1_BASE_ADDR + 0x44)
#define UART1_PL011_REG_DMACR		(UART1_BASE_ADDR + 0x48)

#if USE_UART1
#define UART_REG_DR					(UART1_BASE_ADDR)
#define UART_REG_ECR				(UART1_BASE_ADDR + 0x4)
#define UART_REG_FR					(UART1_BASE_ADDR + 0x18)
#define UART_REG_ILPR				(UART1_BASE_ADDR + 0X20)
#define UART_REG_IBRD				(UART1_BASE_ADDR + 0x24)
#define UART_REG_FBRD				(UART1_BASE_ADDR + 0x28)
#define UART_REG_LCRH				(UART1_BASE_ADDR + 0x2C)
#define UART_REG_CR					(UART1_BASE_ADDR + 0x30)
#define UART_REG_IFLS				(UART1_BASE_ADDR + 0x34)
#define UART_REG_IMSC				(UART1_BASE_ADDR + 0x38)
#define UART_REG_RIS				(UART1_BASE_ADDR + 0x3C)
#define UART_REG_ICR				(UART1_BASE_ADDR + 0x44)
#define UART_REG_DMACR				(UART1_BASE_ADDR + 0x48)
#else
#define UART_REG_DR					(UART0_BASE_ADDR)
#define UART_REG_ECR				(UART0_BASE_ADDR + 0x4)
#define UART_REG_FR					(UART0_BASE_ADDR + 0x18)
#define UART_REG_ILPR				(UART0_BASE_ADDR + 0X20)
#define UART_REG_IBRD				(UART0_BASE_ADDR + 0x24)
#define UART_REG_FBRD				(UART0_BASE_ADDR + 0x28)
#define UART_REG_LCRH				(UART0_BASE_ADDR + 0x2C)
#define UART_REG_CR					(UART0_BASE_ADDR + 0x30)
#define UART_REG_IFLS				(UART0_BASE_ADDR + 0x34)
#define UART_REG_IMSC				(UART0_BASE_ADDR + 0x38)
#define UART_REG_RIS				(UART0_BASE_ADDR + 0x3C)
#define UART_REG_ICR				(UART0_BASE_ADDR + 0x44)
#define UART_REG_DMACR				(UART0_BASE_ADDR + 0x48)
#endif
#define UART_PL01x_RSR_OE			0x08
#define UART_PL01x_RSR_BE			0x04
#define UART_PL01x_RSR_PE			0x02
#define UART_PL01x_RSR_FE			0x01

#define UART_PL01x_FR_TXFE			0x80
#define UART_PL01x_FR_RXFF			0x40
#define UART_PL01x_FR_TXFF			0x20
#define UART_PL01x_FR_RXFE			0x10
#define UART_PL01x_FR_BUSY			0x08
#define UART_PL01x_FR_TMSK			(UART_PL01x_FR_TXFF + UART_PL01x_FR_BUSY)

#define UART_BIT_LCRH_SPS			(1 << 7)
#define UART_BIT_LCRH_WLEN_8		(3 << 5)
#define UART_BIT_LCRH_WLEN_7		(2 << 5)
#define UART_BIT_LCRH_WLEN_6		(1 << 5)
#define UART_BIT_LCRH_WLEN_5		(0 << 5)
#define UART_BIT_LCRH_FEN			(1 << 4)
#define UART_BIT_LCRH_STP2			(1 << 3)
#define UART_BIT_LCRH_EPS			(1 << 2)
#define UART_BIT_LCRH_PEN			(1 << 1)
#define UART_BIT_LCRH_BRK			(1 << 0)

#define UART_BIT_CR_CTSEN			(1 << 15)
#define UART_BIT_CR_RTSEN			(1 << 14)
#define UART_BIT_CR_OUT2			(1 << 13)
#define UART_BIT_CR_OUT1			(1 << 12)
#define UART_BIT_CR_RTS				(1 << 11)
#define UART_BIT_CR_DTR				(1 << 10)
#define UART_BIT_CR_RXE				(1 << 9)
#define UART_BIT_CR_TXE				(1 << 8)
#define UART_BIT_CR_LPE				(1 << 7)
#define UART_BIT_CR_IIRLP			(1 << 2)
#define UART_BIT_CR_SIREN			(1 << 1)
#define UART_BIT_CR_UARTEN			(1 << 0)

#define UART_BIT_IMSC_OEIM			(1 << 10)
#define UART_BIT_IMSC_BEIM			(1 << 9)
#define UART_BIT_IMSC_PEIM			(1 << 8)
#define UART_BIT_IMSC_FEIM			(1 << 7)
#define UART_BIT_IMSC_RTIM			(1 << 6)
#define UART_BIT_IMSC_TXIM			(1 << 5)
#define UART_BIT_IMSC_RXIM			(1 << 4)
#define UART_BIT_IMSC_DSRMIM		(1 << 3)
#define UART_BIT_IMSC_DCDMIM		(1 << 2)
#define UART_BIT_IMSC_CTSMIM		(1 << 1)
#define UART_BIT_IMSC_RIMIM 		(1 << 0)

void uart_init(void);
void uart1_init(void);
void uart_stop(void);
void uart_putc(const char c);
char uart_getc(void);
void uart_puts(const char *s);
int uart_tstc (void);
void uart_delay( int tick);
void _sys_exit(int x);

#endif /* __UART_H__ */
