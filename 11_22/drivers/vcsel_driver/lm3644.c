#include "lm3644.h"
#include "gpio.h"

#define LM3644_NAME	"lm3644"

#if (PRJ_A300)
//suntongce warning , maybe this should mv to imi_config.h
struct lm3644_dev_info flood_dev = {
	.i2c_bus = 0,
	.channel = VCSEL_CHAN_0,
	.type = VCSEL_TYPE_FLOOD,
	.gpio_rst = (256 + 70),
	.i2c_info.type = "lm3644",
	.i2c_info.addr = 0x63,
};
#endif

struct lm3644_dev_info dot_dev = {
	.i2c_bus = 1,
	.channel = VCSEL_CHAN_0,
	.type = VCSEL_TYPE_DOT,
	.gpio_rst = 17,
	.i2c_info.type = "lm3644",
	.i2c_info.addr = 0x63,
};

static int lm3644_read(struct i2c_client *client, u8 reg, u8 *rt_value)
{
	int ret;
	u8 read_cmd[3] = {0};
	int cmd_len = 0;

	read_cmd[0] = reg;
	cmd_len = 1;

	if (client->adapter == NULL)
		pr_err("lm3644_read client->adapter==NULL\n");

	ret = i2c_master_send(client, (char*)read_cmd, cmd_len);
	if (ret != cmd_len) {
		dev_err(&client->dev, "%s: i2c read error, reg: %x\n",
				__func__, reg);
		return ret < 0 ? ret : -EIO;
	}

	ret = i2c_master_recv(client, (char*)rt_value, 1);
	if (ret != 1) {
		dev_err(&client->dev, "%s: i2c read error, reg: %x\n",
				__func__, reg);
		return ret < 0 ? ret : -EIO;
	}

	return 0;
}

static int lm3644_write(struct i2c_client *client,
		u8 reg, u8 value)
{
	int ret = 0;
	u8 write_cmd[2] = {0};

	imi_printf("%s : reg 0x%x val 0x%x\n", __func__, reg, value);
	write_cmd[0] = reg;
	write_cmd[1] = value;

	ret = i2c_master_send(client, (char*)write_cmd, 2);
	if (ret != 2) {
		pr_err("%s, error,REG-0x%02x,val-0x%02x\n",
				__func__, reg, value);
		return -1;
	}

	return 0;
}
#if 0
static int lm3644_update_bits(struct i2c_client *client,
		u8 reg, u8 mask, u8 value)
{
	u8 val_old, val_new;
	int ret;

	ret = lm3644_read(client, reg, &val_old);

	if (ret == 0) {
		val_new = (val_old & ~mask) | (value & mask);
		if (val_new != val_old) {
			ret = lm3644_write(client, reg, val_new);
		}
	}

	return ret;
}
#endif

static int lm3644_chip_init(struct lm3644_flash *flash)
{
	struct i2c_client *client  = flash->client;
	char enable = 0xA4;

	gpio_set_value(flash->reset_pin, 0);
	mdelay(1);
	gpio_set_value(flash->reset_pin, 1);

	lm3644_write(client, REG_TIME_CR, 0x0);
	lm3644_write(client, REG_LED1_FLASH_BR, 0);
	lm3644_write(client, REG_LED2_FLASH_BR, 0);
	lm3644_write(client, REG_ENABLE, enable | (flash->channel & 0x3));

	return 0;
}

#if 0
static ssize_t lm3644_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	int val, ret;
	u8 reg, reg_val;

	val = simple_strtol(buf, NULL, 16);

	reg = (val >> 8) & 0xff;
	reg_val = val & 0xff;

	ret = lm3644_write(client, reg, reg_val);
	if (ret)
		pr_err("lm3644 write reg:0x%02x, value:0x%02x error\n", reg, reg_val);
	else
		pr_info("lm3644 write reg:0x%02x, value:0x%02x ok\n", reg, reg_val);

	return count;
}

static ssize_t lm3644_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 reg, reg_val;

	for (reg = 0x01; reg < 0x0E; reg++) {
		lm3644_read(client, reg, &reg_val);
		pr_info("REG[0x%02x]: 0x%02x\n", reg, reg_val);
	}

	return 0;
}

static ssize_t reset_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	int val;
	struct imi_dpu_subdev *subdev = (struct imi_dpu_subdev *) dev_get_drvdata(dev);
	struct lm3644_flash *flash = (struct lm3644_flash *)subdev->priv;

	val = simple_strtol(buf, NULL, 10);

	if (val) {
		gpio_set_value(flash->reset_pin, 0);
		mdelay(1);
		gpio_set_value(flash->reset_pin, 1);
	}

	return count;
}

static ssize_t reset_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return 0;
}
#endif

static int lm3644_power(void *ctrl, int onoff)
{
	struct lm3644_flash *flash = (struct lm3644_flash *)ctrl;
	struct i2c_client *client = flash->client;
	char enable = 0xA4;

	if (onoff) {
		lm3644_chip_init(flash);

		//lm3644_write(client, REG_TIME_CR, 0x0);
		if (flash->channel & 0x1) {	//LEDA1
			lm3644_write(client, REG_LED1_FLASH_BR, CUR_TO_REG(300));
		}
		if (flash->channel & 0x2) {	//LEDA1
			lm3644_write(client, REG_LED2_FLASH_BR, CUR_TO_REG(300));
		}

		lm3644_write(client, REG_ENABLE, enable | (flash->channel & 0x3));
	} else {
		lm3644_write(client, REG_ENABLE, 0x00);
	}

	return 0;
}

static int lm3644_reset(void *ctrl)
{
	struct lm3644_flash *flash = (struct lm3644_flash *)ctrl;
	int ret;
	//	ret = lm3644_update_bits(client, REG_BOOST_CR, MASK_SOFT_RESET, 0x80);
	ret = lm3644_chip_init(flash);
	return ret;
}

int lm3644_set_current(void *ctrl, int current)
{
	struct lm3644_flash *flash = (struct lm3644_flash *)ctrl;
	int ret = 0;
	if (flash->current_mA != current) {
		switch (flash->channel) {
			case VCSEL_CHAN_0:			//LEDA1
				ret |= lm3644_write(flash->client, REG_LED1_FLASH_BR,
						CUR_TO_REG(current));
				break;
			case VCSEL_CHAN_1:			//LEDA2
				ret |= lm3644_write(flash->client, REG_LED2_FLASH_BR,
						CUR_TO_REG(current));
				break;
			case VCSEL_CHAN_ALL:
				ret |= lm3644_write(flash->client, REG_LED1_FLASH_BR,
						CUR_TO_REG(current / 2));
				ret |= lm3644_write(flash->client, REG_LED2_FLASH_BR,
						CUR_TO_REG(current / 2));
				break;
			default:
				break;
		}
		if (!ret) {
			flash->current_mA = current;
		}
		log_i("%s %s :%d-0x%x, current: %d\n", __func__,
				flash->client->name, flash->client->adapter->nr,
				flash->client->addr, current);
	}
	return 0;
}

static int lm3644_set_channel(void *ctrl, enum vcsel_chan chan)
{
	struct lm3644_flash *flash = (struct lm3644_flash *)ctrl;
	u8 en;
	int ret;

	ret = lm3644_read(flash->client, REG_ENABLE, &en);
	en &= 0xFC;
	lm3644_write(flash->client, REG_ENABLE, en | (flash->channel & 0x3));
	flash->channel = chan;
	return ret;
}

static int lm3644_set_reg(void *ctrl, int reg, int val)
{
	struct lm3644_flash *flash = (struct lm3644_flash *)ctrl;
	int ret;
	ret = lm3644_write(flash->client, (u8)reg, (u8)val);
	return ret;
}

static int lm3644_get_reg(void *ctrl, int reg , int *val)
{
	struct lm3644_flash *flash = (struct lm3644_flash *)ctrl;
	int ret;
	u8 out;
	ret = lm3644_read(flash->client, (u8)reg, &out);
	*val = out;
	return ret;
}


struct vcsel_ops lm3644_vcsel_ops = {
	.power = lm3644_power,
	.reset = lm3644_reset,
	.set_current = lm3644_set_current,
	.set_channel = lm3644_set_channel,
	.set_reg = lm3644_set_reg,
	.get_reg = lm3644_get_reg,
};


static int lm3644_probe(struct lm3644_dev_info *dev_info)
{
	struct lm3644_flash *flash = 0;
	struct i2c_adapter *adap;
	struct i2c_client *client;
	struct vcsel_handle *handle = 0;
	struct device *dev = &flash->dev;
	int ret;

	adap = i2c_get_adapter(dev_info->i2c_bus);
	client = i2c_new_device(adap, &dev_info->i2c_info);
	client->adapter = adap;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(dev, "i2c functionality check fail.\n");
		return -EOPNOTSUPP;
	}

	flash = pvPortMalloc(sizeof(*flash));
	if (flash == NULL)
		return -ENOMEM;
	memset(flash, 0, sizeof(*flash));

	flash->reset_pin = dev_info->gpio_rst;
	flash->channel = dev_info->channel;
	//suntongce warning gpio ops
	if (gpio_is_valid(flash->reset_pin)) {
		ret = gpio_request_one(flash->reset_pin, 0, "vcsel reset");
		if (ret < 0) {
			dev_err(&client->dev, "request reset pin error\n");
			return ret;
		}
	}

	flash->client = client;
#if 0
	ret = lm3644_chip_init(flash);
	if (ret < 0) {
		dev_err(dev, "lm3644 chip init error\n");
		return ret;
	}
#endif
	handle = vcsel_register(dev_info->type, &lm3644_vcsel_ops);
	handle->priv = flash;
	return 0;
}

#if (PRJ_A300)
void flood_driver_register(void)
{
	lm3644_probe(&flood_dev);
}
#endif
void dot_driver_register(void)
{
	lm3644_probe(&dot_dev);
}

#if 0
//no need imple 3644 suspend/resume
static u8 reg_store[15] = {0};

static int lm3644_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);

	return lm3644_write(client, REG_ENABLE, 0x00);
}

static int lm3644_prepare(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 reg;

	for (reg = 0x01; reg < 0x0E; reg++) {
		lm3644_read(client, reg, &reg_store[reg]);
	}
	return 0;
}

static int lm3644_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 reg;

	for (reg = 0x0D; reg > 0x00; reg--) {
		lm3644_write(client, reg, reg_store[reg]);
	}

	return 0;
}
#endif
#if (PRJ_A300)
//DEV_INIT(flood_driver_register);
#endif
//DEV_INIT(dot_driver_register);
