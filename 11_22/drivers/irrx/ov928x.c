#include "ov928x.h"
#include "i2c.h"
#include "linux_common.h"
#include "imi_config.h"
#include "gpio.h"
#include "irrx.h"

//suntongce warning
#define DPU_LOGP	imi_printf
#define DPU_LOGE	imi_printf

struct ov928x_dev_info hw_info = {
	.i2c_info.type = "ov928x",
	.i2c_info.addr = 0x60,
	.bus_num = IR_SNR_I2C_BUS,
	.mipi_num = IR_SNR_INTERFACE,
	.gpio_rst = IR_SNR_IO_RST,
	.gpio_avdd = IR_SNR_IO_AVDD,
	.gpio_dvdd = IR_SNR_IO_DVDD,
	.gpio_dovdd = IR_SNR_IO_DOVDD,
};

static int ov928x_read_reg(struct i2c_client *client, u16 reg, u8 *ret_val)
{
	int ret;
	unsigned short r = swab16(reg);

	ret = i2c_master_send(client, (char *)&r, 2);
	if (ret < 2) {
		dev_err(&client->dev, "%s: i2c send error, reg: %x, ret: %d\n",
				__func__, reg, ret);
		return ret < 0 ? ret : -EIO;
	}

	ret = i2c_master_recv(client, (char *)ret_val, 1);
	if (ret < 1) {
		dev_err(&client->dev, "%s: i2c recv error, reg: %x, ret: %d\n",
				__func__, reg, ret);
		return ret < 0 ? ret : -EIO;
	}

	log_i("%s : reg 0x%x val 0x%x\n", __func__, reg, *ret_val);
	return 0;
}

static int ov928x_write_reg(struct i2c_client *client, u16 reg, u8 val)
{
	char msg[3];
	int ret;
	// unsigned char get = 0;
	// signed char retry;

	log_i("%s : reg 0x%x val 0x%x\n", __func__, reg, val);
	msg[0] = (reg >> 8) & 0xFF;
	msg[1] = reg & 0xFF;
	msg[2] = val;

	ret = i2c_master_send(client, msg, 3);
	if (ret < 3) {
		dev_err(&client->dev, "%s: i2c write error, ret %d reg: %x\n",
				__func__, ret, reg);
		return ret < 0 ? ret : -EIO;
	}
#if 0
	if (unlikely(priv->dbg.i2c_check)) {
		retry = 5;
		while (--retry) {
			ret = ov9282_read_reg(client, reg, &get);
			if ((!ret) && (get == val)) {
				return 0;
			}
			ret = i2c_master_send(client, msg, 3);
			if (ret < 3) {
				dev_err(&client->dev, "%s: i2c write error, ret %d reg: %x\n",
						__func__, ret, reg);
				return ret < 0 ? ret : -EIO;
			}
		}
		DPU_LOGV(" i2c write 0x%x with 0x%x retry %d times\n", reg, val, 5-retry);
		if (!retry) {
			dev_err(&client->dev, "%s: i2c write check error, ret %d reg[0x%x]: writein 0x%x readback 0x%x\n",
					__func__, ret, reg, val, get);
			return -EFAULT;
		}
	}
#endif
	return 0;
}

static const struct regval_list ov928x_triggermode_regs[] = {
#if 1
	{ 0x3666, 0x10 },
	{ 0x4f00, 0x01 },
	{ 0x3030, 0x84 },
	{ 0x303f, 0x01 },
	{ 0x3023, 0x07 },
#else
	{ 0x3666, 0x0a },
	{ 0x4f00, 0x00 },
	{ 0x3030, 0x10 },
	{ 0x0100, 0x01 },
#endif
	ENDAMRK
};
static const struct regval_list ov928x_streammode_regs[] = {

	{ 0x3666, 0x0a },
	{ 0x4f00, 0x00 },
	{ 0x3030, 0x10 },
	{ 0x0100, 0x01 },
	ENDAMRK
};

//soc mipi cfg
static const struct regval_list ov9282_init_regs[] = {
	//soft reset
	{ 0x0103, 0x01 },
	//pll1
	{ 0x0300, 0x06 },
	//ov9282 cannot run at 800Mbps
	{ 0x0301, 0x01 },
	{ 0x0302, 0x90 },
	{ 0x0303, 0x00 },
	{ 0x0304, 0x03 },
	{ 0x0305, 0x02 },
	{ 0x0306, 0x01 },
	//pll2
	{ 0x030a, 0x01 },
	{ 0x030b, 0x04 },
	{ 0x030c, 0x00 },
	{ 0x030d, 0x50 },
	{ 0x030e, 0x02 },
	{ 0x030f, 0x03 },
	{ 0x0312, 0x07 },
	{ 0x0313, 0x01 },
	{ 0x0314, 0x00 },
	//
	{ 0x3001, 0x00 },

	{ 0x3006, 0x04 },
	{ 0x3011, 0x0a },
	{ 0x3013, 0x18 },
	//awb
	{ 0x3400, 0x08 },
	{ 0x3402, 0x08 },
	{ 0x3404, 0x08 },
	//aec
	{ 0x3500, 0x00 },
	{ 0x3501, 0x2a },
	{ 0x3502, 0x90 },
	{ 0x3503, 0x08 },
	{ 0x3505, 0x8c },
	{ 0x3507, 0x03 },
	{ 0x3508, 0x00 },
	{ 0x3509, 0x00 },
	//analog ctrl
	{ 0x3610, 0x80 },
	{ 0x3611, 0xa0 },
	{ 0x3620, 0x6f },
	{ 0x3632, 0x56 },
	{ 0x3633, 0x78 },
	{ 0x3662, 0x05 },
	{ 0x3666, 0x00 },
	{ 0x366f, 0x5a },
	{ 0x3680, 0x84 },
	//sensor ctrl
	{ 0x3712, 0x80 },
	{ 0x372d, 0x22 },
	{ 0x3731, 0x80 },
	{ 0x3732, 0x30 },
	{ 0x3778, 0x00 },
	{ 0x377d, 0x22 },
	{ 0x3788, 0x02 },
	{ 0x3789, 0xa4 },
	{ 0x378a, 0x00 },
	{ 0x378b, 0x4a },
	{ 0x3799, 0x20 },
	//timing ctrl
	{ 0x380c, 0x04 },
	{ 0x380d, 0x10 },
	{ 0x380e, 0x04 },
	{ 0x380f, 0x00 },
	//timing format
	{ 0x3820, 0x44 },
	{ 0x3821, 0x04 },

	{ 0x3881, 0x42 },
	{ 0x38b1, 0x00 },
	{ 0x3920, 0xff },
	//blc
	{ 0x4003, 0x40 },
	{ 0x4008, 0x04 },
	{ 0x4009, 0x0b },
	{ 0x4010, 0x40 },
	//read out ctrl
	{ 0x4501, 0x00 },
	{ 0x4507, 0x00 },
	{ 0x4509, 0x00 },
	{ 0x450a, 0x08 },
	//vfifo ctrl
	{ 0x4601, 0x04 },
	//dvp ctrl
	{ 0x470f, 0x00 },
	{ 0x4f07, 0x00 },
	//mipi ctrl
	{ 0x4800, 0x00 },
	//isp top
	{ 0x5000, 0x87 },
	{ 0x5001, 0x00 },

	{ 0x5d00, 0x07 },
	{ 0x5d01, 0x00 },
	//test pattern
#if 0
	{ 0x5e00, 0x80 },
	{ 0x4320, 0x80 },
	{ 0x4322, 0x03 },
	{ 0x4323, 0x9a },
	{ 0x4324, 0x03 },
	{ 0x4325, 0x9a },
	{ 0x4326, 0x03 },
	{ 0x4327, 0x9a },
	{ 0x4328, 0x03 },
	{ 0x4329, 0x9a },
#endif
	//trigger
	//stream on
	ENDAMRK
};

static const struct regval_list ov9286_init_regs[] = {
	{0x0103, 0x01},

	{0x0300, 0x06},
	//ov9286 need run @1200Mbps to minimize readout time
	{0x0301, 0x02},
	{0x0302, 0x58},
	{0x0303, 0x00},
	{0x0304, 0x03},
	{0x0305, 0x02},
	{0x0306, 0x01},
	{0x030a, 0x01},
	{0x030b, 0x04},
	{0x030c, 0x00},
	{0x030d, 0x50},
	{0x030e, 0x02},
	{0x030f, 0x03},
	{0x0312, 0x07},
	{0x0313, 0x01},
	{0x0314, 0x00},

	{0x302c, 0x00},
	{0x302f, 0x05},
	{0x3210, 0x04},
	{0x3503, 0x08},
	{0x3505, 0x8c},
	{0x3507, 0x03},

	{0x3400, 0x08},
	{0x3402, 0x08},
	{0x3404, 0x08},

	//analog ctrl
	{0x3610, 0x80},
	{0x3611, 0xb8},
	{0x3612, 0x02},
	{0x3614, 0x80},
	{0x3620, 0x6e},
	{0x3632, 0x56},
	{0x3633, 0x78},
	{0x3662, 0x15},	 // MIPI lane & RAW select
	{0x3666, 0x70},	 // FSIN/VSYNC
	{0x3670, 0x68},
	{0x3680, 0x84},

	//sensor ctrl
	{0x3707, 0x5c},
	{0x3712, 0x00},
	{0x372d, 0x22},
	{0x3731, 0x90},
	{0x3732, 0x30},
	{0x3778, 0x00},
	{0x377d, 0x22},
	{0x3788, 0x02},
	{0x3789, 0xa4},
	{0x378a, 0x00},
	{0x378b, 0x44},
	{0x3799, 0x20},
	{0x379b, 0x01},
	{0x379c, 0x10},
	{0x37a8, 0x42},
	{0x37aa, 0x52},
	{0x37ab, 0x3c},

	//timing ctrl
	// 1280*960
	{0x3800, 0x00}, //timing_x_addr_start
	{0x3801, 0x00}, //timing_x_addr_start
	{0x3802, 0x00}, //timing_y_addr_start
	{0x3803, 0x50}, //timing_y_addr_start
	{0x3804, 0x05}, //timing_x_addr_end
	{0x3805, 0x1f}, //timing_x_addr_end
	{0x3806, 0x04}, //timing_y_addr_end
	{0x3807, 0x6f}, //timing_y_addr_end

	{0x3810, 0x00}, //ISP H window offset
	{0x3811, 0x18},
	{0x3812, 0x00}, //ISP V window offset
	{0x3813, 0x00},

	{0x3814, 0x11},
	{0x3815, 0x11},
	{0x3816, 0x00},
	{0x3817, 0x01},
	{0x3818, 0x00},
	{0x3819, 0x05},
	{0x3820, 0x40},
	{0x3821, 0x00},
	{0x382b, 0x3a},
	{0x382c, 0x09},
	{0x382d, 0x9a},

	{0x3880, 0x00},
	{0x3881, 0x44},
	{0x3882, 0x01},
	{0x3883, 0xcc},
	{0x3885, 0x07},
	{0x389d, 0x03},
	{0x38a6, 0x00},
	{0x38a7, 0x01},
	{0x38a8, 0x00},
	{0x38a9, 0xf0},
	{0x38b1, 0x02},
	{0x38b3, 0x07},
	{0x38c4, 0x01},
	{0x38c5, 0x18},
	{0x38c6, 0x02},
	{0x38c7, 0xa8},
	{0x38e5, 0x02},
	{0x38e7, 0x00},
	{0x38e8, 0x00},
	{0x38ed, 0x00},
	{0x38ee, 0x00},
	{0x38ef, 0x00},
	{0x393e, 0x00},
	{0x393f, 0x00},



	{0x4001, 0x00},
	{0x4003, 0x40},
	{0x4008, 0x02},
	{0x4009, 0x09},
	{0x400a, 0x01},
	{0x400b, 0x70},
	{0x400c, 0x02},
	{0x400d, 0x09},
	{0x4010, 0xf0},
	{0x4016, 0x00},
	{0x4012, 0x08},
	{0x4017, 0x10},
	{0x4042, 0xd1},
	{0x4043, 0x60},
	{0x4045, 0x20},
	{0x404b, 0x20},
	{0x4507, 0x40},
	{0x4509, 0x00},
	{0x450b, 0x80},
	{0x450f, 0x00},
	{0x4800, 0x60},
	{0x4837, 0x14},
	{0x4f00, 0x00},
	{0x4f07, 0x00},
	{0x4f08, 0x03},
	{0x4f09, 0x08},
	{0x4f0c, 0x04},
	{0x4f0d, 0xe4},
	{0x4f10, 0x00},
	{0x4f11, 0x00},
	{0x4f12, 0x0f},
	{0x4f13, 0xc4},
	{0x4f07, 0x00},
	{0x5000, 0x87},
	{0x5e00, 0x00},
	ENDAMRK
};

static int ov928x_set_resolution(struct i2c_client *client, int w, int h)
{
	struct ov928x_priv *priv = priv;
	u8 reg = 0;
	if ((w > OV9286_DEFAULT_WIDTH) || (h > OV9286_DEFAULT_HEIGHT)) {
		pr_err("%s window invalid: %d * %d\n", __func__, w, h);
		return -EINVAL;
	}
	if ((priv->info.chip_id == OV9282CHIP_ID) && (h > 800)) {
		ov928x_write_reg(client, 0x3807, 0xc7);	//add dummy line
	}
	ov928x_write_reg(client, 0x3808, (char)((w>> 8) & 0x7));
	ov928x_write_reg(client, 0x3809, (char)(w& 0xFF));
	ov928x_write_reg(client, 0x380a, (char)((h>> 8) & 0x7));
	ov928x_write_reg(client, 0x380b, (char)(h& 0xFF));

	if (priv->info.chip_id == OV9286CHIP_ID) {
		if ((w == 640) || (h == 480)) {
			printk("set ir resolution to 640*480\n");
			ov928x_write_reg(client, 0x3814, 0x22);
			ov928x_write_reg(client, 0x3815, 0x22);
			//BLC
			//ov928x_write_reg(client, 0x4008, 0x02);
			//ov928x_write_reg(client, 0x4009, 0x05);
			//ov928x_write_reg(client, 0x400d, 0x03);

			ov928x_write_reg(client, 0x4507, 0xd0);
			ov928x_write_reg(client, 0x4509, 0x80);
			ov928x_write_reg(client, 0x450b, 0xb0);

			ov928x_write_reg(client, 0x3810, 0x00); //ISP H window offset
			ov928x_write_reg(client, 0x3811, 0x00);
			ov928x_write_reg(client, 0x3812, 0x00); //ISP V window offset
			ov928x_write_reg(client, 0x3813, 0x00);
			//binning
			ov928x_write_reg(client, 0x3778, 0x10);
			ov928x_read_reg(client, 0x3820, &reg);
			ov928x_write_reg(client, 0x3820, reg | 0x1);
			ov928x_read_reg(client, 0x3821, &reg);
			ov928x_write_reg(client, 0x3820, reg | 0x1);
			ov928x_read_reg(client, 0x3837, &reg);
			ov928x_write_reg(client, 0x3837, reg | 0xd);
		}
		/*
		   else if (w == 1280) {
		   printk("set ir resolution to 1280*960\n");
		   ov928x_write_reg(client, 0x3814, 0x11);
		   ov928x_write_reg(client, 0x3815, 0x11);

		   ov928x_write_reg(client, 0x4008, 0x02);
		   ov928x_write_reg(client, 0x4009, 0x09);
		   ov928x_write_reg(client, 0x400d, 0x09);

		   ov928x_write_reg(client, 0x4507, 0x40);
		   ov928x_write_reg(client, 0x4509, 0x00);
		   ov928x_write_reg(client, 0x450b, 0x80);

		   ov928x_write_reg(client, 0x3810, 0x00); //ISP H window offset
		   ov928x_write_reg(client, 0x3811, 0x18);
		   ov928x_write_reg(client, 0x3812, 0x00); //ISP V window offset
		   ov928x_write_reg(client, 0x3813, 0x00);
		   }
		   */
	}
	return 0;
}

static int ov928x_write_array(struct i2c_client *client,
				const struct regval_list *vals)
{
	int ret;
	struct ov928x_priv *priv = priv;
	int i2c_chk;

	i2c_chk = priv->dbg.i2c_check;
	priv->dbg.i2c_check = 0;
	while ((vals->reg_num != 0xffff) && (vals->value != 0xffff)) {
		ret = ov928x_write_reg(client, vals->reg_num, vals->value);
		if (ret < 0)
			return ret;

		vals++;
	}
	priv->dbg.i2c_check = i2c_chk;

	return 0;
}

int get_chip_id(struct i2c_client *client)
{
	u8 id_h, id_l;
	int chipid;

	ov928x_read_reg(client, REG_CHIP_ID_HIGH, &id_h);
	ov928x_read_reg(client, REG_CHIP_ID_LOW, &id_l);
	chipid = ((int)id_h << 8) | (int)id_l;
	imi_printf("%s, id %x\n", __func__, chipid);
	return chipid;
}

//extend to get_sensor_capacity later  //by suntongce
// extends : mipi lane num , mipi bps , data type and so on
static int get_sensor_max_window(int chip_id, int *width, int *height)
{

	if (chip_id == OV9282CHIP_ID) {
		*width = OV9282_DEFAULT_WIDTH;
		*height = OV9282_DEFAULT_HEIGHT;
		return 0;
	} else if (chip_id == OV9286CHIP_ID) {
		*width = OV9286_DEFAULT_WIDTH;
		*height = OV9286_DEFAULT_HEIGHT;
		return 0;
	} else {
		return -EINVAL;
	}
	return 0;
}
int ov928x_get_sensor_info(struct ov928x_priv *priv)
{
	int ret = 0;
	priv->info.chip_id = get_chip_id(priv->client);
	ret =  get_sensor_max_window(priv->info.chip_id, &(priv->info.max_width),
			&(priv->info.max_height));
	if (ret) {
		pr_err("Get info failed(err=%d)\n", ret);
		return ret;
	}
	DPU_LOGP("sensor info: id=0x%x, w=%d, h=%d\n", priv->info.chip_id,
			priv->info.max_width, priv->info.max_height);
	return ret;
}
static int ov928x_power_on(struct ov928x_priv *priv)
{
#if 0
	void __iomem *vxclk_base, *mclk_base;
	u32 regval;
	vxclk_base = devm_ioremap(&client->dev, 0x30000038, 0x4);
	if (IS_ERR_OR_NULL(vxclk_base)) {
		pr_err("Cannot get vxclk pinmux base address\n");
		return PTR_ERR(vxclk_base);
	}

	mclk_base = devm_ioremap(&client->dev, 0x30100150, 0x4);
	if (IS_ERR_OR_NULL(mclk_base)) {
		pr_err("Cannot get mclk base address\n");
		return PTR_ERR(mclk_base);
	}
	pr_info("%s, vxclk_base: %p, mclk_base: %p\n",
			__func__, vxclk_base, mclk_base);
	regval = readl_relaxed(vxclk_base);
	regval |= 0x3;
	writel_relaxed(regval, vxclk_base);

	regval = 0x80000019;
	writel_relaxed(regval, mclk_base);

	devm_iounmap(&client->dev, vxclk_base);
	devm_iounmap(&client->dev, mclk_base);
#endif
	imi_printf("%s %d\n", __func__, __LINE__);

	gpio_set_value(priv->dovdd_pin, 1);
	mdelay(10);
	gpio_set_value(priv->avdd_pin, 1);
	mdelay(10);
	gpio_set_value(priv->dvdd_pin, 1);
	mdelay(10);
	gpio_set_value(priv->reset_pin, 0);
	mdelay(50);
	gpio_set_value(priv->reset_pin, 1);
	mdelay(10);
	return 0;
}

void ov928x_power_off(struct ov928x_priv *priv)
{
	gpio_set_value(priv->reset_pin, 0);
	mdelay(5);
	gpio_set_value(priv->dvdd_pin, 0);
	mdelay(1);
	//	gpio_set_value(priv->dovdd_pin, 0);
	//	mdelay(1);
	gpio_set_value(priv->avdd_pin, 0);
	memset(&priv->info, 0, sizeof(struct ov928x_hw_info));
}

static int ov928x_soft_reset(struct ov928x_priv *priv)
{
	int ret = 0;
	int i2c_chk;

	i2c_chk = priv->dbg.i2c_check;
	priv->dbg.i2c_check = 0;

	ret = ov928x_write_reg(priv->client, 0x0103, 0x01);

	priv->dbg.i2c_check = i2c_chk;
	return ret;
}

static int ov9282_chip_init(struct i2c_client *client, int w, int h)
{
	int ret = 0;
	struct ov928x_priv *priv = priv;

	ret = ov928x_write_array(client, ov9282_init_regs);
	if (ret) {
		return ret;
	}
	ret = ov928x_set_resolution(client, w, h);
	if (ret) {
		return ret;
	}
	ret = ov928x_write_array(client, ov928x_triggermode_regs);
	if (ret) {
		return ret;
	}
	return ret;
}

static int ov9286_chip_init(struct i2c_client *client, int w, int h)
{
	int ret = 0;
	struct ov928x_priv *priv = priv;

	ret = ov928x_write_array(client, ov9286_init_regs);
	if (ret) {
		return ret;
	}
	ret = ov928x_set_resolution(client, w, h);
	if (ret) {
		return ret;
	}
	ret = ov928x_write_array(client, ov928x_triggermode_regs);
	if (ret) {
		return ret;
	}

	return ret;
}

static int ov928x_power(void *ctrl, int onoff)
{
	struct ov928x_priv *priv = (struct ov928x_priv *)ctrl;
	int ret = 0;
	if (onoff) {
		ret = ov928x_power_on(priv);
	} else {
		ov928x_power_off(priv);
	}
	return ret;
}

static int ov928x_reset(void *ctrl)
{
	struct ov928x_priv *priv = (struct ov928x_priv *)ctrl;
	int ret = 0;
	//	ov928x_power_off(priv);
	//	ret |= ov928x_power_on(priv);
	ret = ov928x_soft_reset(priv);
	return ret;

}

static int ov928x_init(void *ctrl, int w, int h)
{
	struct ov928x_priv *priv = (struct ov928x_priv *)ctrl;
	int ret = -EFAULT;
	if (priv->info.chip_id == OV9282CHIP_ID) {
		ret = ov9282_chip_init(priv->client, w, h);
	} else if (priv->info.chip_id == OV9286CHIP_ID) {
		ret = ov9286_chip_init(priv->client, w, h);
	}
	return ret;
}

static int ov928x_set_reg(void *ctrl, int reg, int val)
{
	struct ov928x_priv *priv = (struct ov928x_priv *)ctrl;
	int ret = 0;
	ov928x_write_reg(priv->client, reg & 0xFFFF, val & 0xFF);
	return ret;
}

static int ov928x_get_reg(void *ctrl, int reg, int *val)
{
	struct ov928x_priv *priv = (struct ov928x_priv *)ctrl;
	int ret = 0;
	u8 get;
	ov928x_read_reg(priv->client, reg & 0xFFFF, &get);
	*val = get;
	return ret;

}

static int ov928x_set_para(void *ctrl, int para, void *data)
{
	struct ov928x_priv *priv = (struct ov928x_priv*)ctrl;
	struct irrx_setting *t = &priv->timing;
	int set;
	int ret = 0;
	if (!data) {
		DPU_LOGE("invalid para\n");
		ret = -EINVAL;
		return ret;;
	}
	set = *(int*)data;

	switch(para) {
		case IRRX_PARA_SET_WINDOW:
			{
				struct irrx_window_info *window = (struct irrx_window_info *)data;
				ret = ov928x_set_resolution(priv->client, window->w, window->h);
				break;
			}
		case IRRX_PARA_FILL_TIMING:
			{
				struct irrx_setting *timing = (struct irrx_setting *)data;
				memcpy(t, timing, sizeof(struct irrx_setting));
				break;
			}
		case IRRX_PARA_SET_MIRROR_FLIP:
			{
				u8 reg;
				if ((set & 0x2) != (t->mirror_flip & 0x2)) {
					ov928x_read_reg(priv->client, 0x3821, &reg);
					reg &= ~(1<<2);
					if (set & 0x2) {
						reg |= (1 << 2);
					}
					ov928x_write_reg(priv->client, 0x3821, reg);
				}
				if ((set & 0x1) != (t->mirror_flip & 0x1)) {
					ov928x_read_reg(priv->client, 0x3820, &reg);
					reg &= ~(1<<2);
					if (set & 0x1) {
						reg |= (1 << 2);
					}
					ov928x_write_reg(priv->client, 0x3820, reg);
				}
				t->mirror_flip = set;
			}
			break;
		case IRRX_PARA_SET_ANALOG_GAIN:
			if (set != t->analog_gain) {
				ret = ov928x_write_reg(priv->client, 0x3509, set);
				t->exposure = set;
				break;
			}
		case IRRX_PARA_SET_DIGITAL_GAIN:
			if (set != t->digital_gain) {
				ov928x_write_reg(priv->client, 0x3400, (set >> 8) & 0xF);
				ov928x_write_reg(priv->client, 0x3401, set & 0xFF);
				ov928x_write_reg(priv->client, 0x3402, (set >> 8) & 0xF);
				ov928x_write_reg(priv->client, 0x3403, set & 0xFF);
				ov928x_write_reg(priv->client, 0x3404, (set >> 8) & 0xF);
				ov928x_write_reg(priv->client, 0x3405, set & 0xFF);
				t->digital_gain = set;
			}
			break;
		case IRRX_PARA_SET_EXPOSURE:
			{
				int exp_num;
				exp_num = set * t->sysclk / t->hts;
				if (exp_num != t->exposure) {
					ret = ov928x_write_reg(priv->client, 0x3501, (exp_num >> 4));
					ret |= ov928x_write_reg(priv->client, 0x3502,
								(exp_num << 4) & 0xFF);
					t->exposure = exp_num;
				}
				break;
			}
		case IRRX_PARA_SET_HTS:
			if (set != t->hts) {
				ov928x_write_reg(priv->client, 0x380c, (set >> 8) & 0xFF);
				ov928x_write_reg(priv->client, 0x380d, set & 0xFF);
				t->hts = set;
			}
			break;
		case IRRX_PARA_SET_VTS:
			if (set != t->vts) {
				ov928x_write_reg(priv->client, 0x380e, (set >> 8) & 0xFF);
				ov928x_write_reg(priv->client, 0x380f, set & 0xFF);
				t->vts = set;
			}
			break;
		case IRRX_PARA_SET_WORKMODE:
			if (set) {
				ov928x_write_array(priv->client, ov928x_triggermode_regs);
			} else {
				ov928x_write_array(priv->client, ov928x_streammode_regs);
			}
			break;
		case IRRX_PARA_SET_DBG:
			priv->dbg.i2c_check = set;
			break;
		default:
			DPU_LOGE("unsupported cmd %x", para);
			break;
	}
	return 0;
}

static int ov928x_get_para(void *ctrl, int para, void *data)
{
	struct ov928x_priv *priv = (struct ov928x_priv*)ctrl;
	struct irrx_setting *t = &priv->timing;
	switch(para) {
		case IRRX_PARA_CMD_GET | IRRX_PARA_GET_REG:
			{
				u8 reg;
				int *p = (int*)data;
				ov928x_read_reg(priv->client, *p, &reg);
				reg &= 0xFF;
				*(p+1) = reg;
				break;
			}
		case IRRX_PARA_CMD_GET | IRRX_PARA_GET_ID:
			priv->info.chip_id = get_chip_id(priv->client);
			*(int*)data = priv->info.chip_id;
			break;
		case IRRX_PARA_CMD_GET | IRRX_PARA_GET_MAXWINDOW:
			{
				struct irrx_hw_info *info = (struct irrx_hw_info*)data;
				ov928x_get_sensor_info(priv);
				info->max_width = priv->info.max_width;
				info->max_height = priv->info.max_height;
				info->chip_id = priv->info.chip_id;
				break;
			}
		case IRRX_PARA_CMD_GET | IRRX_PARA_GET_TIMING:
			memcpy(data, (void*)t, sizeof(struct irrx_setting));
			break;
		default:
			DPU_LOGE("unsupported cmd %x", para);
			break;
	}
	return 0;
}


struct irrx_dev_ops ov928x_dev_ops = {
	.power = ov928x_power,
	.reset = ov928x_reset,
	.init = ov928x_init,
	.set_reg = ov928x_set_reg,
	.get_reg = ov928x_get_reg,
	.set_para = ov928x_set_para,
	.get_para = ov928x_get_para,
};

void ov928x_probe(void)
{
	struct i2c_adapter *adap;
	struct i2c_client *client;
	struct ov928x_priv *priv = 0;
	struct irrx_handle *handle = 0;

	adap = i2c_get_adapter(hw_info.bus_num);
	client = i2c_new_device(adap, &hw_info.i2c_info);
	client->adapter = adap;



	priv = pvPortMalloc(sizeof(*priv));
	memset(priv, 0, sizeof(*priv));
	priv->client = client;
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(client->dev, "i2c functionality check fail.\n");
		return;
	}

#if 0
	priv = devm_kzalloc(&client->dev,
			sizeof(struct ov928x_priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	subdev =  devm_kzalloc(&client->dev,
			sizeof(struct imi_dpu_subdev), GFP_KERNEL);
	if (!subdev)
		return -ENOMEM;
#endif

	//	snr = client->dev.platform_data;
	//	priv->idpu = container_of(snr, struct imi_dpu, snr_data);
	priv->reset_pin = hw_info.gpio_rst;
	priv->avdd_pin = hw_info.gpio_avdd;
	priv->dovdd_pin = hw_info.gpio_dovdd;
	priv->dvdd_pin = hw_info.gpio_dvdd;

	//	priv->dbg_mirror = 0;
	//	priv->dbg_flip = 1;

	//	subdev->sub_op.subdev_para_update = snr_para_update;
	priv->dbg.i2c_check = 1;
	handle = irrx_register(&ov928x_dev_ops);
	handle->name = hw_info.i2c_info.type;
	handle->priv = priv;
	log_e("%s done\n", __func__);
	return;
}
//DEV_INIT(ov928x_probe);
