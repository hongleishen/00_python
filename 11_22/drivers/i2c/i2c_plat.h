/*******************************************************************************
  Copyright (c) 2020 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        i2c private .h
  @creator      Qianyu Liu

  @History
      When          Who             What, where, why
      ----------    ------------    ----------------------------------------
      2020/10/09    Qianyu Liu      the initial version
*******************************************************************************/

#ifndef __I2C_PLAT_H__
#define __I2C_PLAT_H__

#define GPIO_i2c0_0				12
#define GPIO_i2c0_1				13
#define GPIO_i2c1_0				14
#define GPIO_i2c1_1				15
#define GPIO_i2c2_0				29
#define GPIO_i2c2_1				30
#define GPIO_i2c3_0				31
#define GPIO_i2c3_1				32
#define GPIO_i2c4_0				X
#define GPIO_i2c4_1				X

void i2c_open(int i);


#endif

