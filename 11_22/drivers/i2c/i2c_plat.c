/*******************************************************************************
  Copyright (c) 2020 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        hardware platform
  @creator      Qianyu Liu

  @History
      When          Who             What, where, why
      ----------    ------------    ----------------------------------------
      2020/10/10    Qianyu Liu      the initial version
*******************************************************************************/

#include "string.h"
#include "imi_io.h"
#include "i2c-designware-core.h"
#include "irqs.h"
#include "i2c_plat.h"
#include "gpio.h"
#include "clock.h"
#include "hellboy_map.h"

#if defined(CHIP_IMI_2280)
#elif defined(CHIP_IMI_1680)
void i2c_open(int i)
{
	clock_enable("gpio");
	clock_enable("i2c");

	gpad_request_one(GPIO_i2c0_0, 0);
	gpad_request_one(GPIO_i2c0_1, 0);
	gpad_request_one(GPIO_i2c1_0, 0);
	gpad_request_one(GPIO_i2c1_1, 0);
	gpad_request_one(GPIO_i2c2_0, 0);
	gpad_request_one(GPIO_i2c2_1, 0);
	gpad_request_one(GPIO_i2c3_0, 0);
	gpad_request_one(GPIO_i2c3_1, 0);
	//gpad_request_one(GPIO_i2c4_0, 0);
	//gpad_request_one(GPIO_i2c4_1, 0);
}

#endif


