#ifndef __OV928x_H__
#define __OV928x_H__
#include "linux_common.h"
#include "irrx.h"
#include "i2c.h"

#define I2C_SLAVE_OV928x        0xc0

#define REG_CHIP_ID_HIGH		0x300a
#define REG_CHIP_ID_LOW			0x300b

#define OV9282CHIP_ID				0x9281
#define OV9286CHIP_ID				0x9286

#define  OV9286_DEFAULT_WIDTH    1280
#define  OV9286_DEFAULT_HEIGHT   960

#define  OV9282_DEFAULT_WIDTH    1280
#define  OV9282_DEFAULT_HEIGHT   800

#define ENDAMRK		{0xffff, 0xffff}

struct regval_list {
	u16 reg_num;
	u16 value;
};

struct ov928x_dev_info {
	struct i2c_board_info i2c_info;
	char bus_num;
	char mipi_num;
	int gpio_rst;
	int gpio_avdd;
	int gpio_dvdd;
	int gpio_dovdd;
};

struct ov928x_dbginfo
{
	char i2c_check;
};

struct ov928x_hw_info {
	int chip_id;
	int max_width;
	int max_height;
};

struct ov928x_priv {
	struct device *dev;

	u32 reset_pin;
	u32 avdd_pin;
	u32 dovdd_pin;
	u32 dvdd_pin;

	struct ov928x_hw_info info;
	struct ov928x_dbginfo dbg;
	struct irrx_setting timing;

	struct i2c_client *client;
};

int ov928x_get_sensor_info(struct ov928x_priv *priv);
int get_chip_id(struct i2c_client *client);
void ov928x_power_off(struct ov928x_priv *priv);
int ov928x_power_up(struct ov928x_priv *priv);
void ov928x_probe(void);

#endif /*__OV928x_H__*/
