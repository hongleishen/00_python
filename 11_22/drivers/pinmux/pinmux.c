#include "pinmux.h"
#include "linux_common.h"
#include "initcalls.h"
#if defined(CHIP_IMI_2280)

static const char pin_map[PIN_MAX][2] = {
	{0x34, 4},       //				GPIO0 = 0,
	{0x34, 8},       //	GPIO1,
	{0x34, 12},       //	GPIO2,
	{0x34, 16},       //	GPIO3,
	{0x34, 20},       //	GPIO4,
	{0x34, 24},       //	GPIO5,
	{0x34, 28},       //	GPIO6,
	{0x38, 0},       //	GPIO7,
	{0x28, 24},       //	GPIO8,
	{0x28, 28},       //	GPIO9,
	{0x2c, 0},       //	GPIO10,
	{0x2c, 4},       //	GPIO11,
	{0x00|U2, 0},       //	GPIO12,
	{0x00|U2, 4},       //	GPIO13,
	{0x00|U2, 8},       //	GPIO14,
	{0x00|U2, 12},       //	GPIO15,
	{0x00|U2, 16},       //	GPIO16,
	{0x00|U2, 20},       //	GPIO17,
	{0x00|U2, 24},       //	GPIO18,
	{0x00|U2, 28},       //	GPIO19,
	{0x04|U2, 0},       //	GPIO20,
	{0x04|U2, 4},       //	GPIO21,
	{0x04|U2, 8},       //	GPIO22,
	{0x04|U2, 12},       //	GPIO23,
	{0x04|U2, 16},       //	GPIO24,
	{0x04|U2, 20},       //	GPIO25,
	{0x04|U2, 24},       //	GPIO26,
	{0x04|U2, 28},       //	GPIO27,
	{0x08|U2, 0},       //	GPIO28,
	{0x08|U2, 4},       //	GPIO29,
	{0x08|U2, 8},       //	GPIO30,
	{0x08|U2, 12},       //	GPIO31,
	{0x08|U2, 16},       //	GPIO32,
	{0x08|U2, 20},       //	GPIO33,
	{0x08|U2, 24},       //	GPIO34,
	{0x08|U2, 28},       //	GPIO35,
	{0x0c|U2, 0},       //	GPIO36,
	{0x0c|U2, 4},       //	GPIO37,
	{0x0c|U2, 8},       //	GPIO38,
	{0x0c|U2, 12},       //	GPIO39,
	{0x0c|U2, 16},       //	GPIO40,
	{0x0c|U2, 20},       //	GPIO41,
	{0x0c|U2, 24},       //	GPIO42,
	{0x0c|U2, 28},       //	GPIO43,
	{0x14, 0},       //	GPIO44,
	{0x14, 4},       //	GPIO45,
	{0x14, 8},       //	GPIO46,
	{0x14, 12},       //	GPIO47,
	{0x14, 16},       //	GPIO48,
	{0x14, 20},       //	GPIO49,
	{0x14, 24},       //	GPIO50,
	{0x14, 28},       //	GPIO51,
	{0x18, 0},       //	GPIO52,
	{0x18, 4},       //	GPIO53,
	{0x18, 8},       //	GPIO54,
	{0x18, 12},       //	GPIO55,
	{0x18, 16},       //	GPIO56,
	{0x18, 20},       //	GPIO57,
	{0x18, 24},       //	GPIO58,
	{0x18, 28},       //	GPIO59,
	{0x04, 8},       //	CM0_VREF,
	{0x04, 12},       //	CM0_HREF,
	{0x04, 16},       //	CM0_DAT0,
	{0x04, 20},       //	CM0_DAT1,
	{0x04, 24},       //	CM0_DAT2,
	{0x04, 28},       //	CM0_DAT3,
	{0x08, 0},       //	CM0_DAT4,
	{0x08, 4},       //	CM0_DAT5,
	{0x08, 8},       //	CM0_DAT6,
	{0x08, 12},       //	CM0_DAT7,
	{0x08, 16},       //	CM0_DAT8,
	{0x08, 20},       //	CM0_DAT9,
	{0x08, 24},       //	CM0_DAT10,
	{0x08, 28},       //	CM0_DAT11,
	{0x04, 4},       //	CM0_PCLK,
	{0x04, 0},       //	CM0_CLK,
	{0x0c, 8},       //	CM1_VREF,
	{0x0c, 12},       //	CM1_HREF,
	{0x10, 28},       //	CM1_DAT0,
	{0x10, 24},       //	CM1_DAT1,
	{0x10, 20},       //	CM1_DAT2,
	{0x10, 16},       //	CM1_DAT3,
	{0x10, 12},       //	CM1_DAT4,
	{0x10, 8},       //	CM1_DAT5,
	{0x10, 4},       //	CM1_DAT6,
	{0x10, 0},       //	CM1_DAT7,
	{0x0c, 28},       //	CM1_DAT8,
	{0x0c, 24},       //	CM1_DAT9,
	{0x0c, 20},       //	CM1_DAT10,
	{0x0c, 16},       //	CM1_DAT11,
	{0x0c, 4},       //	CM1_PCLK,
	{0x0c, 0},       //	CM1_CLK,
	{0x28, 0},       //	TCK,
	{0x28, 4},       //	TMS,
	{0x28, 8},       //	TDI,
	{0x28, 12},       //	TDO,
	{0x28, 16},       //	RTCK,
	{0x28, 20},       //	TRST,
	{0x1c, 28},       //	UART0_TXD,
	{0x20, 0},       //	UART0_RXD,
	{0x20, 4},       //	SPI0_FRM,
	{0x20, 8},       //	SPI0_CLK,
	{0x20, 12},       //	SPI0_TXD,
	{0x20, 16},       //	SPI0_RXD,
	{0x20, 20},       //	I2C0_SCL,
	{0x20, 24},       //	I2C0_SDA,
	{0x20, 28},       //	I2C1_SCL,
	{0x24, 0},       //	I2C1_SDA,
	{0x1c, 12},       //	SPI1_FRM,
	{0x1c, 16},       //	SPI1_CLK,
	{0x1c, 20},       //	SPI1_TXD,
	{0x1c, 24},       //	SPI1_RXD,
	{0x2c, 16},       //	IR_PCLK,
	{0x2c, 8},       //	IR_LINEV,
	{0x2c, 12},       //	IR_FRMV,
	{0x2c, 20},       //	IR_DOUT0,
	{0x2c, 24},       //	IR_DOUT1,
	{0x2c, 28},       //	IR_DOUT2,
	{0x30, 0},       //	IR_DOUT3,
	{0x30, 4},       //	IR_DOUT4,
	{0x30, 8},       //	IR_DOUT5,
	{0x30, 12},       //	IR_DOUT6,
	{0x30, 16},       //	IR_DOUT7,
	{0x30, 20},       //	IR_DOUT8,
	{0x30, 24},       //	IR_DOUT9,
	{0x30, 28},       //	IR_DOUT10,
	{0x34, 0},       //	IR_DOUT11,
	{0x24, 4},       //	I2S_MCLK,
	{0x24, 8},       //	I2S0_BCLK,
	{0x24, 12},       //	I2S0_LRCK,
	{0x24, 16},       //	I2S0_SDI,
};
#define PINMUX_GET_REG(pin)		(((!!(pin_map[pin][0] & U2)) * PINMUX_GAP) + \
		(pin_map[pin][0] & 0x7F) + SYS_CON_BASE_ADDR)

int pinmux_set_mode(int pin, int mode)
{
	int addr;
	int reg;
	if (pin >= PIN_MAX) {
		return -EINVAL;
	}
	addr = PINMUX_GET_REG(pin);
	reg = readl(addr);
	reg &= ~(0x7 << pin_map[pin][1]);
	reg |= (mode << pin_map[pin][1]);
	writel(reg, addr);
	imi_printf("%s pin %d, reg %x val %x\n", __func__, pin, addr, reg);
	return 0;
}

int pinmux_get_mode(int pin)
{
	int addr = PINMUX_GET_REG(pin);
	int reg;
	reg = readl(addr);
	return (reg >> pin_map[pin][1]) & 0x7;
}

void pinmux_default(void)
{
	//uart1
	pinmux_set_mode(GPIO30, 1);
	pinmux_set_mode(GPIO31, 1);
	pinmux_set_mode(SPI0_FRM, 1);
	pinmux_set_mode(GPIO7, 3);
//	pinmux_set_mode(GPIO1, 7);
	pinmux_set_mode(GPIO3, 1);
	pinmux_set_mode(GPIO4, 1);
	pinmux_set_mode(GPIO5, 1);
	pinmux_set_mode(GPIO6, 1);
	pinmux_set_mode(GPIO20, 0);
}
POST_CORE_INIT(pinmux_default);

#elif defined(CHIP_IMI_1680)
#define IMI_1680_PINMUX_BASE 0x20F08400
#define IMI_1680_PINS_PER_BANK 32

int pinmux_set_mode(int pin, int mode)
{
	int index = pin / IMI_1680_PINS_PER_BANK;
	unsigned int reg = IMI_1680_PINMUX_BASE + index * 4;
	volatile unsigned int reg_val = readl(reg);
	int pin_index = pin % IMI_1680_PINS_PER_BANK;

	if (pin >= GPIOMAX)
		return -EINVAL;

	if (mode == GPIO_TO_FUNC)
		reg_val = reg_val & (~(0x01 << pin_index));
	else
		reg_val = reg_val | (0x01 << pin_index);

	writel(reg_val, reg);

	return 0;
}

int pinmux_get_mode(int pin)
{
	int index = pin / IMI_1680_PINS_PER_BANK;
	unsigned int reg = IMI_1680_PINMUX_BASE + index * 4;
	volatile unsigned int reg_val = readl(reg);
	int pin_index = pin % IMI_1680_PINS_PER_BANK;

	if (reg_val & (0x01 << pin_index))
		return GPIO_TO_GPIO;
	else
		return GPIO_TO_FUNC;
}

void pinmux_default(void)
{
	pinmux_set_mode(GPIO0, GPIO_TO_FUNC);//SCLK
	pinmux_set_mode(GPIO1, GPIO_TO_FUNC);//CSN
	pinmux_set_mode(GPIO2, GPIO_TO_FUNC);//DAT0
	pinmux_set_mode(GPIO3, GPIO_TO_FUNC);//DAT1
	pinmux_set_mode(GPIO4, GPIO_TO_FUNC);//DAT2
	pinmux_set_mode(GPIO5, GPIO_TO_FUNC);//DAT3
}

POST_CORE_INIT(pinmux_default);
#endif
