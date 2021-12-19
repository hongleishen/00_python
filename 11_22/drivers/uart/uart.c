#include "uart.h"
#include "imi_io.h"
#include "clock.h"
#include "initcalls.h"

#define WATCHDOG_RESET()    do{}while(0)
#define UART_CLEAR_BIT(addr,n)   ((~(1<<(n)))&read32(addr))

void _sys_exit(int x)
{
	x=x;
}

void uart_init(void)
{
	unsigned int temp;
	unsigned int divider;
	unsigned int remainder;
	unsigned int fraction;
	unsigned int lcr;

#if defined(TARGET_FPGA)|| defined(TARGET_FPGA_SIMULATION)
	clock_enable("uart");
#endif

	write32(UART_PL011_REG_CR, 0);
	write32(UART_PL011_REG_ICR, 0x7FF);

	temp = 16 * BAUD_RATE;
	divider = UART_CLK / temp;
	remainder = UART_CLK % temp;
	temp = (8 * remainder) / BAUD_RATE;
	fraction = (temp >> 1) + (temp & 1);

	write32(UART_PL011_REG_IBRD, divider);
	write32(UART_PL011_REG_FBRD, fraction);

	/* Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled */
	lcr = UART_BIT_LCRH_WLEN_8 | UART_BIT_LCRH_FEN;
	write32(UART_PL011_REG_LCRH, lcr);

	write32(UART_PL011_REG_CR, UART_BIT_CR_UARTEN | UART_BIT_CR_TXE | UART_BIT_CR_RXE | UART_BIT_CR_RTS);

	write32(UART_PL011_REG_IFLS, 0); // fifo levle  1/8
	write32(UART_PL011_REG_IMSC, UART_BIT_IMSC_RXIM|UART_BIT_IMSC_OEIM);
}

void uart1_init(void)
{
	unsigned int temp;
	unsigned int divider;
	unsigned int remainder;
	unsigned int fraction;
	unsigned int lcr;

	write32(UART1_PL011_REG_CR, 0);
	write32(UART1_PL011_REG_ICR, 0x7FF);

	temp = 16 * BAUD_RATE;
	divider = UART_CLK / temp;
	remainder = UART_CLK % temp;
	temp = (8 * remainder) / BAUD_RATE;
	fraction = (temp >> 1) + (temp & 1);

	write32(UART1_PL011_REG_IBRD, divider);
	write32(UART1_PL011_REG_FBRD, fraction);

	/* Set the UART to be 8 bits, 1 stop bit, no parity, fifo enabled */
	lcr = UART_BIT_LCRH_WLEN_8 | UART_BIT_LCRH_FEN;
	write32(UART1_PL011_REG_LCRH, lcr);

	write32(UART1_PL011_REG_CR, UART_BIT_CR_UARTEN | UART_BIT_CR_TXE | UART_BIT_CR_RXE | UART_BIT_CR_RTS);

	//write32(UART1_PL011_REG_IFLS, (1<<3)|0x1); // fifo levle  1/4

	write32(UART1_PL011_REG_IFLS, 0);
	write32(UART1_PL011_REG_IMSC, UART_BIT_IMSC_RXIM|UART_BIT_IMSC_OEIM);
}
static void out (char c)
{

	/* Wait until there is space in the FIFO */
	while (read32(UART_REG_FR) & UART_PL01x_FR_TXFF)
		WATCHDOG_RESET();

	write32(UART_REG_DR, c);
}

void uart_putc (char c)
{
	if (c == '\n')
		out('\r');
	out(c);
}

char uart_getc (void)
{
	unsigned int data;

	while (read32(UART_REG_FR) & UART_PL01x_FR_RXFE)
		WATCHDOG_RESET();

	data = read32(UART_REG_DR);

	if (data & 0xF00) {
		write32(UART_REG_ECR, 0x0F);
		return (char)-1;
	}

	return (char) data;
}

int uart_tstc (void)
{
	WATCHDOG_RESET();
	return !(read32(UART_REG_FR) & UART_PL01x_FR_RXFE);
}

void uart_puts(const char *s)
{
	char *p = (char *)s;
	while (*p)
		uart_putc(*p++);
}

void uart_delay( int tick)
{
	while(tick)
		tick--;
}

#ifndef TARGET_FPGA_SIMULATION
EARLY_INIT(uart_init);
EARLY_INIT(uart1_init);
#endif
