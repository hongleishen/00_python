#ifndef __LM36011_H__
#define __LM36011_H__
#include "vcsel.h"
#include "i2c.h"

/* registers definitions */
#define REG_ENABLE			0x01
#define MASK_MODE		GENMASK(1,0)
#define MASK_STROBE		BIT(2)

#define REG_CR				0x02
#define REG_LED_FLASH_BR	0x03
#define REG_LED_TORCH_BR	0x04
#define REG_FLAG			0x05

#define REG_DEVID			0x06
#define	MASK_SOFT_RESET		BIT(7)

#define LM36011_CURRENT_750MA  (0x3F)
#define CUR_TO_REG(cur)			(((cur) * 1000 - 10900) / 11725)

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


struct lm36011_flash {
	u8 channel;
	short reset_pin;
	short current_mA;
	struct i2c_client *client;
	struct device *dev;
};
struct lm36011_dev_info
{
	char i2c_bus;
	char channel;
	enum vcsel_type type;
	int gpio_rst;
	struct i2c_board_info i2c_info;
};
#endif
