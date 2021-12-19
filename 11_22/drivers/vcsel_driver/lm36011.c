#include "lm36011.h"

#define LM36011_NAME	"lm36011"

#if (PRJ_S300)
struct lm36011_dev_info flood_dev = {
	.i2c_bus = 2,
	.channel = VCSEL_CHAN_0,
	.type = VCSEL_TYPE_FLOOD,
	.gpio_rst = 0xFFF,
	.i2c_info.type = "lm36011",
	.i2c_info.addr = 0x64,
};
#endif
static int lm36011_read(struct i2c_client *client,
	u8 reg, u8 *rt_value)
{
	int ret;
	u8 read_cmd[3] = {0};
	int cmd_len = 0;

	read_cmd[0] = reg;
	cmd_len = 1;

	if (client->adapter == NULL)
		pr_err("lm36011_read client->adapter==NULL\n");

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

static int lm36011_write(struct i2c_client *client,
	u8 reg, unsigned char value)
{
	int ret = 0;
	u8 write_cmd[2] = {0};

	log_v(": reg 0x%x val 0x%x\n", reg, value);
	write_cmd[0] = reg;
	write_cmd[1] = value;

	ret = i2c_master_send(client, (char*)write_cmd, 2);
	if (ret != 2) {
		pr_err("%s, error, REG-0x%02x,val-0x%02x\n",
			__func__, reg, value);
		return -1;
	}

	return 0;
}
	
static int lm36011_update_bits(struct i2c_client *client,
	u8 reg, u8 mask, u8 value)
{
	u8 val_old, val_new;
	int ret;

	ret = lm36011_read(client, reg, &val_old);

	if (ret == 0) {
		val_new = (val_old & ~mask) | (value & mask);
		if (val_new != val_old) {
			ret = lm36011_write(client, reg, val_new);
		}
	}

	return ret;
}
	
static inline int lm36011_soft_reset(struct lm36011_flash *flash)
{
	struct i2c_client *client = flash->client;
	int ret;

	ret = lm36011_update_bits(client, REG_DEVID, MASK_SOFT_RESET, 0x80);

	return ret;
}

#if 0
static ssize_t lm36011_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct i2c_client *client = to_i2c_client(dev);
	int val, ret;
	u8 reg, reg_val;

	val = simple_strtol(buf, NULL, 16);

	reg = (val >> 8) & 0xff;
	reg_val = val & 0xff;

	ret = lm36011_write(client, reg, reg_val);
	if (ret)
		pr_err("lm36011 write reg:0x%02x, value:0x%02x error\n", reg, reg_val);
	else
		pr_info("lm36011 write reg:0x%02x, value:0x%02x ok\n", reg, reg_val);

	return count;
}

static ssize_t lm36011_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 reg, reg_val;

	for (reg = 0x01; reg < 0x07; reg++) {
		lm36011_read(client, reg, &reg_val);
		pr_info("REG[0x%02x]: 0x%02x\n", reg, reg_val);
	}

	return 0;
}

static ssize_t reset_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	int val;
	struct imi_dpu_subdev *subdev = (struct imi_dpu_subdev *)dev_get_drvdata(dev);
	struct lm36011_flash *flash = (struct lm36011_flash *)subdev->priv;

	val = simple_strtol(buf, NULL, 10);

	if (val)
		lm36011_soft_reset(flash);

	return count;
}

static ssize_t reset_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return 0;
}

static DEVICE_ATTR(lm36011, 0644, lm36011_show, lm36011_store);
static DEVICE_ATTR(reset, 0644, reset_show, reset_store);

static struct attribute *lm36011_irlight_attrs[] = {
	&dev_attr_lm36011.attr,
	&dev_attr_reset.attr,
	NULL,
};

static struct attribute_group lm36011_debug_attr_group = {
	.name = "lm36011_irlight",
	.attrs = lm36011_irlight_attrs,
};
#endif

static int lm36011_power(void *ctrl, int onoff)
{
	struct lm36011_flash *flash = (struct lm36011_flash *)ctrl;
	struct i2c_client *client = flash->client;

	if (onoff) {
		lm36011_soft_reset(flash);
		lm36011_write(client, REG_ENABLE, 0x25);
		lm36011_write(client, REG_LED_FLASH_BR, LM36011_CURRENT_750MA);
	} else {
		lm36011_write(client, REG_ENABLE, 0x00);
	}

	return 0;
}

static int lm36011_reset(void *ctrl)
{
	return lm36011_soft_reset((struct lm36011_flash *)ctrl);
}

int lm36011_set_current(void *ctrl, int current)
{
	struct lm36011_flash *flash = (struct lm36011_flash *)ctrl;
	int ret = 0;
	if (flash->current_mA != current) {
				ret |= lm36011_write(flash->client, REG_LED_FLASH_BR,
						CUR_TO_REG(current));
		if (!ret) {
			flash->current_mA = current;
		}
		log_i("%s %s :%d-0x%x, current: %d\n", __func__,
				flash->client->name, flash->client->adapter->nr,
				flash->client->addr, current);
	}
	return 0;
}

#if 0
static int lm36011_set_channel(void *ctrl, enum vcsel_chan chan)
{
	return 0;
}
#endif

static int lm36011_set_reg(void *ctrl, int reg, int val)
{
	struct lm36011_flash *flash = (struct lm36011_flash *)ctrl;
	int ret;
	ret = lm36011_write(flash->client, (u8)reg, (u8)val);
	return ret;
}

static int lm36011_get_reg(void *ctrl, int reg , int *val)
{
	struct lm36011_flash *flash = (struct lm36011_flash *)ctrl;
	int ret;
	u8 out;
	ret = lm36011_read(flash->client, (u8)reg, &out);
	*val = out;
	return ret;
}

struct vcsel_ops lm36011_vcsel_ops = {
	.power = lm36011_power,
	.reset = lm36011_reset,
	.set_current = lm36011_set_current,
//	.set_channel = lm36011_set_channel,
	.set_reg = lm36011_set_reg,
	.get_reg = lm36011_get_reg,
};

static void lm36011_probe(struct lm36011_dev_info *dev_info)
{
	struct lm36011_flash *flash = 0;
	struct i2c_adapter *adap;
	struct i2c_client *client;
	struct vcsel_handle *handle = 0;
	struct device *dev = flash->dev;

	adap = i2c_get_adapter(dev_info->i2c_bus);
	client = i2c_new_device(adap, &dev_info->i2c_info);
	client->adapter = adap;

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(dev, "i2c functionality check fail.\n");
		return;
	}

	flash = pvPortMalloc(sizeof(*flash));
	if (flash == NULL) {
		log_e("malloc fail\n");
		return;
	}
	memset(flash, 0, sizeof(*flash));

	if (dev_info->gpio_rst < 0x1FF) {
		flash->reset_pin = dev_info->gpio_rst;
	}
	flash->channel = dev_info->channel;
//	if (gpio_is_valid(flash->reset_pin)) {
//		ret = gpio_request_one(flash->reset_pin, 0, "vcsel reset");
//		if (ret < 0) {
//			dev_err(&client->dev, "request reset pin error\n");
//			return ret;
//		}
//	}

	flash->client = client;
	handle = vcsel_register(dev_info->type, &lm36011_vcsel_ops);
	handle->priv = flash;
	return;
}

#if 0
static u8 reg_store[8] = {0};

static int lm36011_suspend(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);

	return lm36011_write(client, REG_ENABLE, 0x00);
}

static int lm36011_prepare(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 reg;

	for (reg = 0x01; reg < 0x07; reg++) {
		lm36011_read(client, reg, &reg_store[reg]);
	}

	return 0;
}

static int lm36011_resume(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	u8 reg;

	for (reg = 0x06; reg > 0x00; reg--) {
		lm36011_write(client, reg, reg_store[reg]);
	}

	return 0;
}
#endif

#if (PRJ_S300)
void flood_driver_register(void)
{
	lm36011_probe(&flood_dev);
}
//DEV_INIT(flood_driver_register);
#endif
