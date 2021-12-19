/***************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    RGB sensor OV02K10 init
  @author   Dejie Zhang

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------
  2021/01/15   Dejie Zhang   the initial version

***************************************************************************/
#if defined(CHIP_IMI_1680)
#include "sensor_def.h"
#include "imi_io.h"
#include "i2c.h"
#include "delay.h"
#include "elog.h"

struct i2c_client ov02k10_slave =
	{0x36, I2C_ADDR_M_7BIT, I2C_SIZE_16BIT, I2C_SIZE_8BIT, 0};

IMI_CAM_PARAMS *ov02k10_ircam = NULL;

static int ov02k10_write_reg(u32 reg, u32 val)
{
	return i2c_write(ov02k10_ircam->dev, ov02k10_ircam->slave, reg, val);
}

static int ov02k10_read_reg(u32 reg, u32 *ret_val)
{
	return i2c_read(ov02k10_ircam->dev, ov02k10_ircam->slave, reg, ret_val);
}

int ov02k10_get_sensor_id(void)
{
	u32 id = 0;

	ov02k10_read_reg(0x302a, &id);
	imi_printf("<<<<< OV02K10 ID 0x%x >>>>>\n", id);

	return (int)(id);
}

int ov02k10_sensor_open(IMI_CAM_PARAMS *cam)
{
	int ret = 0;

	imi_printf("Enter %s, cam = 0x%x\n", __func__, cam);
	if(NULL == ov02k10_ircam) {
		ov02k10_ircam = cam;
	}
	cam->slave = &ov02k10_slave;
	// check sensor chip_id
	ret = ov02k10_get_sensor_id();

	return ret;
}

void ov02k10_regs_init_hdr_disable(IMI_CAM_PARAMS *cam)
{
	log_d("Enter %s\n", __func__);
	ov02k10_write_reg(0x0103, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x0109, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x0104, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x0102, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x0305, 0x2c);//pll1_divp                                         //LINEAR
	ov02k10_write_reg(0x0306, 0x00);//pll1_divmipi                                      //LINEAR
	ov02k10_write_reg(0x0307, 0x01);//pll1_divm                                         //LINEAR
	ov02k10_write_reg(0x0317, 0x0a);                                                    //LINEAR
	ov02k10_write_reg(0x0323, 0x07);//pll2_prediv                                       //LINEAR
	ov02k10_write_reg(0x0324, 0x01);//pll2_divp                                         //LINEAR
	ov02k10_write_reg(0x0325, 0xb0);                                                    //LINEAR
	ov02k10_write_reg(0x0327, 0x07);//pll2_divsram                                      //LINEAR
	ov02k10_write_reg(0x300f, 0x11);                                                    //LINEAR
	ov02k10_write_reg(0x3012, 0x21);                                                    //LINEAR
	ov02k10_write_reg(0x3026, 0x10);                                                    //LINEAR
	ov02k10_write_reg(0x3027, 0x08);                                                    //LINEAR
	ov02k10_write_reg(0x302d, 0x24);                                                    //LINEAR
	ov02k10_write_reg(0x3106, 0x10);//SCLK                                              //LINEAR
	ov02k10_write_reg(0x3400, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3406, 0x08);                                                    //LINEAR
	ov02k10_write_reg(0x3408, 0x05);                                                    //LINEAR
	ov02k10_write_reg(0x340c, 0x05);                                                    //LINEAR
	ov02k10_write_reg(0x3425, 0x51);                                                    //LINEAR
	ov02k10_write_reg(0x3426, 0x10);                                                    //LINEAR
	ov02k10_write_reg(0x3427, 0x14);                                                    //LINEAR
	ov02k10_write_reg(0x3428, 0x50);                                                    //LINEAR
	ov02k10_write_reg(0x3429, 0x10);                                                    //LINEAR
	ov02k10_write_reg(0x342a, 0x10);                                                    //LINEAR
	ov02k10_write_reg(0x342b, 0x04);                                                    //LINEAR
	ov02k10_write_reg(0x3504, 0x08);                                                    //LINEAR
	ov02k10_write_reg(0x3508, 0x04);//first channel real gain                           //LINEAR
	ov02k10_write_reg(0x3509, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3544, 0x08);                                                    //LINEAR
	ov02k10_write_reg(0x3548, 0x01);//second channel real gain                          //LINEAR
	ov02k10_write_reg(0x3549, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3584, 0x08);                                                    //LINEAR
	ov02k10_write_reg(0x3588, 0x01);//third channel real gain                           //LINEAR
	ov02k10_write_reg(0x3589, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3601, 0x70);                                                    //LINEAR
	ov02k10_write_reg(0x3604, 0xe3);                                                    //LINEAR
	ov02k10_write_reg(0x3605, 0xff);                                                    //LINEAR
	ov02k10_write_reg(0x3606, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x3608, 0xa8);                                                    //LINEAR
	ov02k10_write_reg(0x360a, 0xd0);                                                    //LINEAR
	ov02k10_write_reg(0x360b, 0x08);                                                    //LINEAR
	ov02k10_write_reg(0x360e, 0xc8);                                                    //LINEAR
	ov02k10_write_reg(0x360f, 0x66);                                                    //LINEAR
	ov02k10_write_reg(0x3610, 0x81);                                                    //LINEAR
	ov02k10_write_reg(0x3611, 0x89);                                                    //LINEAR
	ov02k10_write_reg(0x3612, 0x4e);                                                    //LINEAR
	ov02k10_write_reg(0x3613, 0xbd);                                                    //LINEAR
	ov02k10_write_reg(0x362a, 0x0e);                                                    //LINEAR
	ov02k10_write_reg(0x362b, 0x0e);                                                    //LINEAR
	ov02k10_write_reg(0x362c, 0x0e);                                                    //LINEAR
	ov02k10_write_reg(0x362d, 0x0e);                                                    //LINEAR
	ov02k10_write_reg(0x362e, 0x0c);                                                    //LINEAR
	ov02k10_write_reg(0x362f, 0x1a);                                                    //LINEAR
	ov02k10_write_reg(0x3630, 0x32);                                                    //LINEAR
	ov02k10_write_reg(0x3631, 0x64);                                                    //LINEAR
	ov02k10_write_reg(0x3638, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3643, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3644, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3645, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3646, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3647, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3648, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3649, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x364a, 0x04);                                                    //LINEAR
	ov02k10_write_reg(0x364c, 0x0e);                                                    //LINEAR
	ov02k10_write_reg(0x364d, 0x0e);                                                    //LINEAR
	ov02k10_write_reg(0x364e, 0x0e);                                                    //LINEAR
	ov02k10_write_reg(0x364f, 0x0e);                                                    //LINEAR
	ov02k10_write_reg(0x3650, 0xff);                                                    //LINEAR
	ov02k10_write_reg(0x3651, 0xff);                                                    //LINEAR
	ov02k10_write_reg(0x3661, 0x07);                                                    //LINEAR
	ov02k10_write_reg(0x3662, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3663, 0x20);                                                    //LINEAR
	ov02k10_write_reg(0x3665, 0x12);//embedded data rows                                //LINEAR
	ov02k10_write_reg(0x3667, 0xd4);// 2 lane                                           //LINEAR
	ov02k10_write_reg(0x3668, 0x80);                                                    //LINEAR
	ov02k10_write_reg(0x366f, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3671, 0x08);                                                    //LINEAR
	ov02k10_write_reg(0x3673, 0x2a);                                                    //LINEAR
	ov02k10_write_reg(0x3681, 0x80);                                                    //LINEAR
	ov02k10_write_reg(0x3700, 0x26);                                                    //LINEAR
	ov02k10_write_reg(0x3701, 0x1e);                                                    //LINEAR
	ov02k10_write_reg(0x3702, 0x25);                                                    //LINEAR
	ov02k10_write_reg(0x3703, 0x28);                                                    //LINEAR
	ov02k10_write_reg(0x3706, 0xb1);                                                    //LINEAR
	ov02k10_write_reg(0x3707, 0x0a);                                                    //LINEAR
	ov02k10_write_reg(0x3708, 0x34);                                                    //LINEAR
	ov02k10_write_reg(0x3709, 0x50);                                                    //LINEAR
	ov02k10_write_reg(0x370a, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x370b, 0x21);                                                    //LINEAR
	ov02k10_write_reg(0x3714, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x371b, 0x13);                                                    //LINEAR
	ov02k10_write_reg(0x371c, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x371d, 0x08);                                                    //LINEAR
	ov02k10_write_reg(0x3756, 0xe7);                                                    //LINEAR
	ov02k10_write_reg(0x3757, 0xe7);                                                    //LINEAR
	ov02k10_write_reg(0x3762, 0x1d);                                                    //LINEAR
	ov02k10_write_reg(0x376c, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3776, 0x03);                                                    //LINEAR
	ov02k10_write_reg(0x3777, 0x22);                                                    //LINEAR
	ov02k10_write_reg(0x3779, 0x60);                                                    //LINEAR
	ov02k10_write_reg(0x377c, 0x48);                                                    //LINEAR
	ov02k10_write_reg(0x379c, 0x4d);                                                    //LINEAR
	ov02k10_write_reg(0x3784, 0x06);                                                    //LINEAR
	ov02k10_write_reg(0x3785, 0x0a);                                                    //LINEAR
	ov02k10_write_reg(0x37d1, 0xb1);                                                    //LINEAR
	ov02k10_write_reg(0x37d2, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x37d3, 0x21);                                                    //LINEAR
	ov02k10_write_reg(0x37d5, 0xb1);                                                    //LINEAR
	ov02k10_write_reg(0x37d6, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x37d7, 0x21);                                                    //LINEAR
	ov02k10_write_reg(0x37d8, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x37dc, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3790, 0x10);                                                    //LINEAR
	ov02k10_write_reg(0x3793, 0x04);                                                    //LINEAR
	ov02k10_write_reg(0x3794, 0x07);                                                    //LINEAR
	ov02k10_write_reg(0x3796, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3797, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x37a1, 0x80);                                                    //LINEAR
	ov02k10_write_reg(0x37bb, 0x88);                                                    //LINEAR
	ov02k10_write_reg(0x37be, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x37bf, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x37c0, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x37c7, 0x56);                                                    //LINEAR
	ov02k10_write_reg(0x37ca, 0x21);                                                    //LINEAR
	ov02k10_write_reg(0x37cd, 0x90);                                                    //LINEAR
	ov02k10_write_reg(0x37cf, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x3800, 0x00);//horizontal start                                  //LINEAR
	ov02k10_write_reg(0x3801, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3802, 0x00);//vertical start                                    //LINEAR
	ov02k10_write_reg(0x3803, 0x04);                                                    //LINEAR
	ov02k10_write_reg(0x3804, 0x07);//horizontal end                                    //LINEAR
	ov02k10_write_reg(0x3805, 0x8f);                                                    //LINEAR
	ov02k10_write_reg(0x3806, 0x04);//vertical end                                      //LINEAR
	ov02k10_write_reg(0x3807, 0x43);                                                    //LINEAR
	ov02k10_write_reg(0x3808, 0x07);//res width 1920                                    //LINEAR
	ov02k10_write_reg(0x3809, 0x80);                                                    //LINEAR
	ov02k10_write_reg(0x380a, 0x04);//res height 1080                                   //LINEAR
	ov02k10_write_reg(0x380b, 0x38);                                                    //LINEAR
	ov02k10_write_reg(0x380c, 0x3c);//HTS for first channel                             //LINEAR
	ov02k10_write_reg(0x380d, 0x80);                                                    //LINEAR
	ov02k10_write_reg(0x380e, 0x0a);//VTS                                               //LINEAR
	ov02k10_write_reg(0x380f, 0xb4);                                                    //LINEAR
	ov02k10_write_reg(0x3811, 0x08);//window horizontal truncation                      //LINEAR
	ov02k10_write_reg(0x3813, 0x04);//window vertical truncation                        //LINEAR
	ov02k10_write_reg(0x3814, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x3815, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x3816, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x3817, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x381c, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3820, 0x02);//mirror                                            //LINEAR
	ov02k10_write_reg(0x3821, 0x00);//flip                                              //LINEAR
	ov02k10_write_reg(0x3822, 0x14);                                                    //LINEAR
	ov02k10_write_reg(0x384c, 0x30);//0x30);//HTS for second and third channel          //LINEAR
	ov02k10_write_reg(0x384d, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3858, 0x0d);                                                    //LINEAR
	ov02k10_write_reg(0x3865, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3866, 0xc0);                                                    //LINEAR
	ov02k10_write_reg(0x3867, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3868, 0xc0);                                                    //LINEAR
	ov02k10_write_reg(0x3900, 0x13);                                                    //LINEAR
	ov02k10_write_reg(0x3940, 0x13);                                                    //LINEAR
	ov02k10_write_reg(0x3980, 0x13);                                                    //LINEAR
	ov02k10_write_reg(0x3c01, 0x11);                                                    //LINEAR
	ov02k10_write_reg(0x3c05, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3c0f, 0x1c);                                                    //LINEAR
	ov02k10_write_reg(0x3c12, 0x0d);                                                    //LINEAR
	ov02k10_write_reg(0x3c19, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x3c21, 0x40);                                                    //LINEAR
	ov02k10_write_reg(0x3c3b, 0x18);                                                    //LINEAR
	ov02k10_write_reg(0x3c3d, 0xc9);                                                    //LINEAR
	ov02k10_write_reg(0x3c55, 0xcb);                                                    //LINEAR
	ov02k10_write_reg(0x3c5d, 0xec);                                                    //LINEAR
	ov02k10_write_reg(0x3c5e, 0xec);                                                    //LINEAR
	ov02k10_write_reg(0x3ce0, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3ce1, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3ce2, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3ce3, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x3d8c, 0x70);                                                    //LINEAR
	ov02k10_write_reg(0x3d8d, 0x10);                                                    //LINEAR
	ov02k10_write_reg(0x4001, 0x2f);//BLC ctrl                                          //LINEAR
	ov02k10_write_reg(0x4033, 0x80);                                                    //LINEAR
	ov02k10_write_reg(0x4008, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x4009, 0x11);                                                    //LINEAR
	ov02k10_write_reg(0x4004, 0x00);//blk_lvl_target_L                                  //LINEAR
	ov02k10_write_reg(0x4005, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x400a, 0x03);                                                    //LINEAR
	ov02k10_write_reg(0x400b, 0x40);                                                    //LINEAR
	ov02k10_write_reg(0x410f, 0x01);                                                    //LINEAR
	ov02k10_write_reg(0x402e, 0x01);//blk_lvl_target_S                                  //LINEAR
	ov02k10_write_reg(0x402f, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x4030, 0x01);//blk_lvl_target_VS                                 //LINEAR
	ov02k10_write_reg(0x4031, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x4032, 0x9f);                                                    //LINEAR
	ov02k10_write_reg(0x4050, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x4051, 0x07);                                                    //LINEAR
	ov02k10_write_reg(0x4288, 0xcf);                                                    //LINEAR
	ov02k10_write_reg(0x4289, 0x03);                                                    //LINEAR
	ov02k10_write_reg(0x428a, 0x46);                                                    //LINEAR
	ov02k10_write_reg(0x430b, 0xff);                                                    //LINEAR
	ov02k10_write_reg(0x430c, 0xff);                                                    //LINEAR
	ov02k10_write_reg(0x430d, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x430e, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x4500, 0x18);                                                    //LINEAR
	ov02k10_write_reg(0x4501, 0x18);                                                    //LINEAR
	ov02k10_write_reg(0x4504, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x4507, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x4603, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x4640, 0x62);                                                    //LINEAR
	ov02k10_write_reg(0x4646, 0xaa);                                                    //LINEAR
	ov02k10_write_reg(0x4647, 0x55);                                                    //LINEAR
	ov02k10_write_reg(0x4648, 0x99);                                                    //LINEAR
	ov02k10_write_reg(0x4649, 0x66);                                                    //LINEAR
	ov02k10_write_reg(0x464d, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x4654, 0x11);                                                    //LINEAR
	ov02k10_write_reg(0x4655, 0x22);                                                    //LINEAR
	ov02k10_write_reg(0x4800, 0x04);                                                    //LINEAR
	ov02k10_write_reg(0x4810, 0xff);                                                    //LINEAR
	ov02k10_write_reg(0x4811, 0xff);                                                    //LINEAR
	ov02k10_write_reg(0x480e, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x4813, 0x04);                                                    //LINEAR
	ov02k10_write_reg(0x4837, 0x0c);                                                    //LINEAR
	ov02k10_write_reg(0x484b, 0x27);                                                    //LINEAR
	ov02k10_write_reg(0x4d00, 0x4e);                                                    //LINEAR
	ov02k10_write_reg(0x4d01, 0x0c);                                                    //LINEAR
	ov02k10_write_reg(0x4d09, 0x4f);                                                    //LINEAR
	ov02k10_write_reg(0x5000, 0x1f);//ISP_TOP_0                                         //LINEAR
	ov02k10_write_reg(0x5080, 0x00);//PRE_ISP_0_L                                       //LINEAR
	ov02k10_write_reg(0x50c0, 0x00);//PRE_ISP_0_S                                       //LINEAR
	ov02k10_write_reg(0x5100, 0x00);//PRE_ISP_0_VS                                      //LINEAR
	ov02k10_write_reg(0x5200, 0x00);                                                    //LINEAR
	ov02k10_write_reg(0x5201, 0x70);                                                    //LINEAR
	ov02k10_write_reg(0x5202, 0x03);                                                    //LINEAR
	ov02k10_write_reg(0x5203, 0x7f);                                                    //LINEAR
	//ov02k10_write_reg(0x3501, 0x02);                                                    //LINEAR
	ov02k10_write_reg(0x380e, 0x04);                                                    //LINEAR
	ov02k10_write_reg(0x380f, 0x65);                                                    //LINEAR
	ov02k10_write_reg(0x3501, 0x00);//first exposure in unit of rows                    //LINEAR
	ov02k10_write_reg(0x3502, 0x74);                                                    //LINEAR
	ov02k10_write_reg(0x3541, 0x00);//second exposure in unit of rows                   //LINEAR
	ov02k10_write_reg(0x3542, 0x20);                                                    //LINEAR
	ov02k10_write_reg(0x0000 ,0x00);                                                    //LINEAR
	log_d("Exit %s\n", __func__);
}

void ov02k10_regs_init_hdr_2frm(IMI_CAM_PARAMS *cam)
{
	log_d("Enter %s\n", __func__);
	ov02k10_write_reg(0x0103, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x0109, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x0104, 0x02);									//HDR_2FRAME
	ov02k10_write_reg(0x0102, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x0305, 0x30);									//HDR_2FRAME
	ov02k10_write_reg(0x0306, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x0307, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x0317, 0x0a);									//HDR_2FRAME
	ov02k10_write_reg(0x0323, 0x07);									//HDR_2FRAME
	ov02k10_write_reg(0x0324, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x0325, 0xb0);									//HDR_2FRAME
	ov02k10_write_reg(0x0327, 0x07);									//HDR_2FRAME
	ov02k10_write_reg(0x032c, 0x02);									//HDR_2FRAME
	ov02k10_write_reg(0x032d, 0x02);									//HDR_2FRAME
	ov02k10_write_reg(0x032e, 0x05);									//HDR_2FRAME
	ov02k10_write_reg(0x300f, 0x11);									//HDR_2FRAME
	ov02k10_write_reg(0x3012, 0x21);									//HDR_2FRAME
	ov02k10_write_reg(0x3026, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3027, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x302d, 0x24);									//HDR_2FRAME
	ov02k10_write_reg(0x3103, 0x29);									//HDR_2FRAME
	ov02k10_write_reg(0x3106, 0x11);									//HDR_2FRAME
	ov02k10_write_reg(0x3400, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3406, 0x08);									//HDR_2FRAME
	ov02k10_write_reg(0x3408, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x340c, 0x10);									//HDR_2FRAME
	ov02k10_write_reg(0x3425, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3426, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3427, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3428, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3429, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x342a, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x342b, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3504, 0x08);									//HDR_2FRAME
	ov02k10_write_reg(0x3508, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3509, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3544, 0x08);									//HDR_2FRAME
	ov02k10_write_reg(0x3548, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3549, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3584, 0x08);									//HDR_2FRAME
	ov02k10_write_reg(0x3588, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3589, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3601, 0x70);									//HDR_2FRAME
	ov02k10_write_reg(0x3604, 0xe3);									//HDR_2FRAME
	ov02k10_write_reg(0x3605, 0x7f);									//HDR_2FRAME
	ov02k10_write_reg(0x3606, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3608, 0xa8);									//HDR_2FRAME
	ov02k10_write_reg(0x360a, 0xd0);									//HDR_2FRAME
	ov02k10_write_reg(0x360b, 0x08);									//HDR_2FRAME
	ov02k10_write_reg(0x360e, 0xc8);									//HDR_2FRAME
	ov02k10_write_reg(0x360f, 0x66);									//HDR_2FRAME
	ov02k10_write_reg(0x3610, 0x81);									//HDR_2FRAME
	ov02k10_write_reg(0x3611, 0x89);									//HDR_2FRAME
	ov02k10_write_reg(0x3612, 0x4e);									//HDR_2FRAME
	ov02k10_write_reg(0x3613, 0xbd);									//HDR_2FRAME
	ov02k10_write_reg(0x362a, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x362b, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x362c, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x362d, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x362e, 0x0c);									//HDR_2FRAME
	ov02k10_write_reg(0x362f, 0x1a);									//HDR_2FRAME
	ov02k10_write_reg(0x3630, 0x32);									//HDR_2FRAME
	ov02k10_write_reg(0x3631, 0x64);									//HDR_2FRAME
	ov02k10_write_reg(0x3638, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3643, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3644, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3645, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3646, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3647, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3648, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3649, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x364a, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x364c, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x364d, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x364e, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x364f, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x3650, 0xff);									//HDR_2FRAME
	ov02k10_write_reg(0x3651, 0xff);									//HDR_2FRAME
	ov02k10_write_reg(0x3661, 0x07);									//HDR_2FRAME
	ov02k10_write_reg(0x3662, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3663, 0x20);									//HDR_2FRAME
	ov02k10_write_reg(0x3665, 0x12);									//HDR_2FRAME
	ov02k10_write_reg(0x3667, 0xd4);									//HDR_2FRAME
	ov02k10_write_reg(0x3668, 0x80);									//HDR_2FRAME
	ov02k10_write_reg(0x366f, 0xc4);									//HDR_2FRAME
	ov02k10_write_reg(0x3670, 0xc7);									//HDR_2FRAME
	ov02k10_write_reg(0x3671, 0x09);									//HDR_2FRAME
	ov02k10_write_reg(0x3673, 0x6a);									//HDR_2FRAME
	ov02k10_write_reg(0x3681, 0x80);									//HDR_2FRAME
	ov02k10_write_reg(0x3700, 0x26);									//HDR_2FRAME
	ov02k10_write_reg(0x3701, 0x1e);									//HDR_2FRAME
	ov02k10_write_reg(0x3702, 0x25);									//HDR_2FRAME
	ov02k10_write_reg(0x3703, 0x28);									//HDR_2FRAME
	ov02k10_write_reg(0x3706, 0x3e);									//HDR_2FRAME
	ov02k10_write_reg(0x3707, 0x0a);									//HDR_2FRAME
	ov02k10_write_reg(0x3708, 0x36);									//HDR_2FRAME
	ov02k10_write_reg(0x3709, 0x55);									//HDR_2FRAME
	ov02k10_write_reg(0x370a, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x370b, 0xa3);									//HDR_2FRAME
	ov02k10_write_reg(0x3714, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x371b, 0x16);									//HDR_2FRAME
	ov02k10_write_reg(0x371c, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x371d, 0x08);									//HDR_2FRAME
	ov02k10_write_reg(0x3756, 0x9b);									//HDR_2FRAME
	ov02k10_write_reg(0x3757, 0x9b);									//HDR_2FRAME
	ov02k10_write_reg(0x3762, 0x1d);									//HDR_2FRAME
	ov02k10_write_reg(0x376c, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3776, 0x05);									//HDR_2FRAME
	ov02k10_write_reg(0x3777, 0x22);									//HDR_2FRAME
	ov02k10_write_reg(0x3779, 0x60);									//HDR_2FRAME
	ov02k10_write_reg(0x377c, 0x48);									//HDR_2FRAME
	ov02k10_write_reg(0x3783, 0x02);									//HDR_2FRAME
	ov02k10_write_reg(0x3784, 0x06);									//HDR_2FRAME
	ov02k10_write_reg(0x3785, 0x0a);									//HDR_2FRAME
	ov02k10_write_reg(0x3790, 0x10);									//HDR_2FRAME
	ov02k10_write_reg(0x3793, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x3794, 0x07);									//HDR_2FRAME
	ov02k10_write_reg(0x3796, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3797, 0x02);									//HDR_2FRAME
	ov02k10_write_reg(0x379c, 0x4d);									//HDR_2FRAME
	ov02k10_write_reg(0x37a1, 0x80);									//HDR_2FRAME
	ov02k10_write_reg(0x37bb, 0x88);									//HDR_2FRAME
	ov02k10_write_reg(0x37be, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x37bf, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x37c0, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x37c7, 0x56);									//HDR_2FRAME
	ov02k10_write_reg(0x37ca, 0x21);									//HDR_2FRAME
	ov02k10_write_reg(0x37cc, 0x13);									//HDR_2FRAME
	ov02k10_write_reg(0x37cd, 0x90);									//HDR_2FRAME
	ov02k10_write_reg(0x37cf, 0x02);									//HDR_2FRAME
	ov02k10_write_reg(0x37d1, 0x3e);									//HDR_2FRAME
	ov02k10_write_reg(0x37d2, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x37d3, 0xa3);									//HDR_2FRAME
	ov02k10_write_reg(0x37d5, 0x3e);									//HDR_2FRAME
	ov02k10_write_reg(0x37d6, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x37d7, 0xa3);									//HDR_2FRAME
	ov02k10_write_reg(0x37d8, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x37da, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x37db, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x37dc, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x37dd, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3800, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3801, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3802, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3803, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x3804, 0x07);									//HDR_2FRAME
	ov02k10_write_reg(0x3805, 0x8f);									//HDR_2FRAME
	ov02k10_write_reg(0x3806, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x3807, 0x43);									//HDR_2FRAME
	ov02k10_write_reg(0x3808, 0x07);									//HDR_2FRAME
	ov02k10_write_reg(0x3809, 0x80);									//HDR_2FRAME
	ov02k10_write_reg(0x380a, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x380b, 0x38);									//HDR_2FRAME
	ov02k10_write_reg(0x380c, 0x3c);									//HDR_2FRAME
	ov02k10_write_reg(0x380d, 0x80);									//HDR_2FRAME
	ov02k10_write_reg(0x380e, 0x10);									//HDR_2FRAME
	ov02k10_write_reg(0x380f, 0x40);									//HDR_2FRAME
	ov02k10_write_reg(0x3502, 0x40);									//HDR_2FRAME
	ov02k10_write_reg(0x3811, 0x08);									//HDR_2FRAME
	ov02k10_write_reg(0x3813, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x3814, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3815, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3816, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3817, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x381c, 0x08);									//HDR_2FRAME
	ov02k10_write_reg(0x3820, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3821, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3822, 0x14);									//HDR_2FRAME
	ov02k10_write_reg(0x384c, 0x30);									//HDR_2FRAME
	ov02k10_write_reg(0x384d, 0xa0);									//HDR_2FRAME
	ov02k10_write_reg(0x3858, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3865, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3866, 0xc0);									//HDR_2FRAME
	ov02k10_write_reg(0x3867, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3868, 0xc0);									//HDR_2FRAME
	ov02k10_write_reg(0x3900, 0x13);									//HDR_2FRAME
	ov02k10_write_reg(0x3940, 0x13);									//HDR_2FRAME
	ov02k10_write_reg(0x3980, 0x13);									//HDR_2FRAME
	ov02k10_write_reg(0x3c01, 0x11);									//HDR_2FRAME
	ov02k10_write_reg(0x3c05, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3c0f, 0x1c);									//HDR_2FRAME
	ov02k10_write_reg(0x3c12, 0x0d);									//HDR_2FRAME
	ov02k10_write_reg(0x3c14, 0x21);									//HDR_2FRAME
	ov02k10_write_reg(0x3c19, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x3c21, 0x40);									//HDR_2FRAME
	ov02k10_write_reg(0x3c3b, 0x18);									//HDR_2FRAME
	ov02k10_write_reg(0x3c3d, 0xc9);									//HDR_2FRAME
	ov02k10_write_reg(0x3c55, 0xcb);									//HDR_2FRAME
	ov02k10_write_reg(0x3c5d, 0xcf);									//HDR_2FRAME
	ov02k10_write_reg(0x3c5e, 0xcf);									//HDR_2FRAME
	ov02k10_write_reg(0x3ce0, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3ce1, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3ce2, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3ce3, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x3d8c, 0x70);									//HDR_2FRAME
	ov02k10_write_reg(0x3d8d, 0x10);									//HDR_2FRAME
	ov02k10_write_reg(0x4001, 0xef);									//HDR_2FRAME
	ov02k10_write_reg(0x4033, 0x80);									//HDR_2FRAME
	ov02k10_write_reg(0x4008, 0x02);									//HDR_2FRAME
	ov02k10_write_reg(0x4009, 0x11);									//HDR_2FRAME
	ov02k10_write_reg(0x4004, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x4005, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x400a, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x400b, 0xf0);									//HDR_2FRAME
	ov02k10_write_reg(0x400e, 0x40);									//HDR_2FRAME
	ov02k10_write_reg(0x4011, 0xbb);									//HDR_2FRAME
	ov02k10_write_reg(0x410f, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x402e, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x402f, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x4030, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x4031, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x4032, 0x9f);									//HDR_2FRAME
	ov02k10_write_reg(0x4050, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x4051, 0x07);									//HDR_2FRAME
	ov02k10_write_reg(0x4288, 0xce);									//HDR_2FRAME
	ov02k10_write_reg(0x4289, 0x03);									//HDR_2FRAME
	ov02k10_write_reg(0x428a, 0x46);									//HDR_2FRAME
	ov02k10_write_reg(0x430b, 0xff);									//HDR_2FRAME
	ov02k10_write_reg(0x430c, 0xff);									//HDR_2FRAME
	ov02k10_write_reg(0x430d, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x430e, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x4314, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x4500, 0x18);									//HDR_2FRAME
	ov02k10_write_reg(0x4501, 0x18);									//HDR_2FRAME
	ov02k10_write_reg(0x4504, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x4507, 0x03);									//HDR_2FRAME
	ov02k10_write_reg(0x4508, 0x1a);									//HDR_2FRAME
	ov02k10_write_reg(0x4603, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x4640, 0x62);									//HDR_2FRAME
	ov02k10_write_reg(0x4646, 0xaa);									//HDR_2FRAME
	ov02k10_write_reg(0x4647, 0x55);									//HDR_2FRAME
	ov02k10_write_reg(0x4648, 0x99);									//HDR_2FRAME
	ov02k10_write_reg(0x4649, 0x66);									//HDR_2FRAME
	ov02k10_write_reg(0x464d, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x4654, 0x11);									//HDR_2FRAME
	ov02k10_write_reg(0x4655, 0x22);									//HDR_2FRAME
	ov02k10_write_reg(0x4800, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x4810, 0xff);									//HDR_2FRAME
	ov02k10_write_reg(0x4811, 0xff);									//HDR_2FRAME
	ov02k10_write_reg(0x480e, 0x05);									//HDR_2FRAME
	ov02k10_write_reg(0x4813, 0x84);									//HDR_2FRAME
	ov02k10_write_reg(0x4837, 0x0c);									//HDR_2FRAME
	ov02k10_write_reg(0x484b, 0x67);									//HDR_2FRAME
	ov02k10_write_reg(0x4d00, 0x4e);									//HDR_2FRAME
	ov02k10_write_reg(0x4d01, 0x0c);									//HDR_2FRAME
	ov02k10_write_reg(0x4d09, 0x4f);									//HDR_2FRAME
	ov02k10_write_reg(0x5000, 0x1f);									//HDR_2FRAME
	ov02k10_write_reg(0x5080, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x50c0, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x5100, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x5200, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x5201, 0x70);									//HDR_2FRAME
	ov02k10_write_reg(0x5202, 0x03);									//HDR_2FRAME
	ov02k10_write_reg(0x5203, 0x7f);									//HDR_2FRAME
	ov02k10_write_reg(0x5780, 0x53);									//HDR_2FRAME
	ov02k10_write_reg(0x5786, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x4837, 0x0e);									//HDR_2FRAME
	ov02k10_write_reg(0x376c, 0x20);									//HDR_2FRAME
	ov02k10_write_reg(0x032f, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x032e, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x032d, 0x05);									//HDR_2FRAME
	ov02k10_write_reg(0x3106, 0x92);									//HDR_2FRAME
	ov02k10_write_reg(0x3833, 0x41);									//HDR_2FRAME
	ov02k10_write_reg(0x3104, 0x03);									//HDR_2FRAME
	ov02k10_write_reg(0x0308, 0x00);									//HDR_2FRAME
	ov02k10_write_reg(0x030e, 0x01);									//HDR_2FRAME
	ov02k10_write_reg(0x0309, 0x04);									//HDR_2FRAME
	ov02k10_write_reg(0x3106, 0x92);									//HDR_2FRAME
	log_d("Exit %s\n", __func__);
}

void ov02k10_regs_init_hdr_3frm(IMI_CAM_PARAMS *cam)
{
	log_d("Enter %s\n", __func__);
	log_d("Exit %s\n", __func__);
}

int ov02k10_regs_init(IMI_CAM_PARAMS *cam)
{
	log_d("Enter %s\n", __func__);
	switch (cam->hdren) {
	case SNR_HDR_DISABLE:
		log_d("HDR Disabled, Linear Mode\n");
		ov02k10_regs_init_hdr_disable(cam);
		break;
	case SNR_HDR_2_FRAME:
		log_d("HDR 2 Frame Mode\n");
		ov02k10_regs_init_hdr_2frm(cam);
		break;
	case SNR_HDR_3_FRAME:
		log_d("HDR 3 Frame Mode\n");
		ov02k10_regs_init_hdr_3frm(cam);
		break;
	}
	log_d("Exit %s\n", __func__);
	return 0;
}


int ov02k10_set_feature(IMI_SENSOR_FEATURE feat, char *pparam)
{
	return 0;
}

int ov02k10_get_info(IMI_SENSOR_FEATURE feat, char *pparam)
{
	return 0;
}

int ov02k10_set_resolution(int w, int h)
{
	u32 val = 0;
#if 0
	if ((w > 1280) || (h > 960)) {
		imi_printf("%s window invalid: %d * %d\n", __func__, w, h);
		return -1;
	}
#endif
	//ov02k10_read_reg(0x0103, &val);
	//imi_printf("0x0103 is 0x%02x\n", val);
	//ov02k10_read_reg(0x0109, &val);
	//imi_printf("0x0109 is 0x%02x\n", val);
	ov02k10_read_reg(0x3808, &val);
	imi_printf("0x3808 is 0x%02x\n", val);
	ov02k10_read_reg(0x3809, &val);
	imi_printf("0x3809 is 0x%02x\n", val);
	ov02k10_read_reg(0x380a, &val);
	imi_printf("0x380a is 0x%02x\n", val);
	ov02k10_read_reg(0x380b, &val);
	imi_printf("0x380b is 0x%02x\n", val);

	ov02k10_read_reg(0x380c, &val);
	imi_printf("0x380c is 0x%02x\n", val);
	ov02k10_read_reg(0x380d, &val);
	imi_printf("0x380d is 0x%02x\n", val);
	ov02k10_read_reg(0x380e, &val);
	imi_printf("0x380e is 0x%02x\n", val);
	ov02k10_read_reg(0x380f, &val);
	imi_printf("0x380f is 0x%02x\n", val);

	//ov02k10_read_reg(0x3666, &val);
	//imi_printf("0x3666 is 0x%02x\n", val);
	//ov02k10_read_reg(0x3030, &val);
	//imi_printf("0x3030 is 0x%02x\n", val);
	return 0;
}

int ov02k10_start_stream(IMI_CAM_PARAMS *ircam)
{
	u32 val = 0;
	imi_printf("Enter %s \n", __func__);	
	ov02k10_set_resolution(1920, 1080);
	if(ircam->trigen == 1) {
		//trigger mode
		ov02k10_write_reg(0x3666, 0x10); // trigger mode:0x10  normal mode:0x0a
		ov02k10_write_reg(0x4f00, 0x01); // trigger mode:0x01  normal mode:0x00
		ov02k10_write_reg(0x3030, 0x84); // trigger mode:0x84  normal mode:0x10

		ov02k10_write_reg(0x303f, 0x01); // frame number by one trigger action
		ov02k10_write_reg(0x3023, 0x07);
		imi_printf("Image Output in Trigger Mode\n");
	} else {
		//ov02k10_write_reg(0x3666, 0x0a);
		//ov02k10_write_reg(0x4f00, 0x00);
		//ov02k10_write_reg(0x3030, 0x10);	// r_aslp_repeat

		ov02k10_write_reg(0x0100, 0x01); // stream on
		ov02k10_read_reg(0x0100, &val);
		imi_printf("0x0100 is 0x%02x\n", val);
		imi_printf("Image Output in non-Trigger Mode\n");
	}

	return 0;
}

int ov02k10_sensor_close(IMI_CAM_PARAMS *ircam)
{
	// TODO: close all voltage and clock

	// TODO: release all mems
	return 0;
}

static IMI_CAM_FUNCS ir_ov02k10_funcs = {
	.sensor_open        	= ov02k10_sensor_open,
	.sensor_regs_init   	= ov02k10_regs_init,
	.sensor_get_info    	= ov02k10_get_info,
	.sensor_set_feature 	= ov02k10_set_feature,
	.sensor_start_stream	= ov02k10_start_stream,
	.sensor_close       	= ov02k10_sensor_close,
};

int ov02k10_sensor_init(PIMI_CAM_FUNCS *funcs)
{
	*funcs = &ir_ov02k10_funcs;
	imi_printf("ov02k10_sensor_init\n");
	return 0;
}
#endif/*CHIP_IMI_1680*/
