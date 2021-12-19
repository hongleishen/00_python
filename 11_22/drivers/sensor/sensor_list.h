/***************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    all sensor lists
  @author   Dejie Zhang

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------
  2021/01/15   Dejie Zhang   the initial version

***************************************************************************/
#if defined(CHIP_IMI_1680)
#ifndef __SENSOR_HEAD__
#define __SENSOR_HEAD__

/**************************** Sensor ID ****************************/
#ifndef OV9286_SENSOR_ID
#define OV9286_SENSOR_ID	0x9286
#endif
#ifndef OV9282_SENSOR_ID
#define OV9282_SENSOR_ID	0x9281
#endif
#ifndef OV02K10_SENSOR_ID
#define OV02K10_SENSOR_ID	0xB0
#endif


/*************************** Sensor Name ***************************/
#ifndef OV9286_DRV_NAME
#define OV9286_DRV_NAME		"OV9286"
#endif
#ifndef OV9282_DRV_NAME
#define OV9282_DRV_NAME		"OV9282"
#endif
#ifndef OV02K10_DRV_NAME
#define OV02K10_DRV_NAME	"OV02K10"
#endif


/**************************** INIT Func ****************************/
//int ov9286_sensor_init(PIMI_CAM_FUNCS *funcs);
//int ov9282_sensor_init(PIMI_CAM_FUNCS *funcs);
int ov02k10_sensor_init(PIMI_CAM_FUNCS *funcs);


/*******************************************************************
 *  IR Sensor Lists
 *******************************************************************/
IMI_SENSOR_LIST ir_sensor_list[] = {
//	{OV9286_DRV_NAME, OV9286_SENSOR_ID, ov9286_sensor_init},
//	{OV9282_DRV_NAME, OV9282_SENSOR_ID, ov9282_sensor_init},
	{{0}},
};


/*******************************************************************
 *  RGB Sensor Lists
 *******************************************************************/
IMI_SENSOR_LIST rgb_sensor_list[] = {
	{OV02K10_DRV_NAME, OV02K10_SENSOR_ID, ov02k10_sensor_init},
	{{0}},
};

#endif
#endif/*CHIP_IMI_1680*/
