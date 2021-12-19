#ifndef __LM3644_H__
#define __LM3644_H__
#include "vcsel.h"
#include "i2c.h"



/* registers definitions */
#define REG_ENABLE			0x01
#define MASK_ENABLE			GENMASK(1, 0)
#define MASK_MODE			GENMASK(3, 2)
#define MASK_TX				BIT(7)
#define MASK_STROBE			BIT(5)

#define REG_IVFM			0x02
#define REG_LED1_FLASH_BR	0x03
#define REG_LED2_FLASH_BR	0x04

#define REG_LED1_TORCH_BR	0x05
#define REG_LED2_TORCH_BR	0x06

#define REG_BOOST_CR		0x07
#define MASK_SOFT_RESET		BIT(7)

#define REG_TIME_CR			0x08
#define REG_TEMP			0x09
#define REG_FLAG1			0x0A
#define REG_FLAG2			0x0B
#define REG_DEVID			0x0C
#define REG_LAST_FR			0x0D

#define LM3644_CURRENT_1100MA	(0x5D)
#define CUR_TO_REG(cur)			((((cur) * 1000 - 10900) / 11725) & 0x7F)
enum led_mode {
	MODE_IDLE = 0x0,
	MODE_IR = 0x1,
	MODE_TORCH = 0x2,
	MODE_FLASH = 0x3,
};

enum led_enable {
	ENABLE_NONE = 0x0,
	ENABLE_LED1 = 0x1,
	ENABLE_LED2 = 0x2,
	ENABLE_ALL = 0x3,
};

struct lm3644_flash {
	u8 channel;
	short reset_pin;
	short current_mA;
	struct i2c_client *client;
	struct device dev;
};


struct lm3644_dev_info
{
	char i2c_bus;
	char channel;
	enum vcsel_type type;
	int gpio_rst;
	struct i2c_board_info i2c_info;
};
#endif
