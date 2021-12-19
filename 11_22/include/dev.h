/*******************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        DEV  head
  @creator      rain zhou

  @History
  When          Who             What, where, why
  ----------    ------------    ----------------------------------------
  2021/01/27    rain zhou       the initial version
 *******************************************************************************/

#ifndef __DEV__H
#define	__DEV__H

#include "imi_type.h"

#define	dev_max     20

#define	UART        0
#define	QSPI        1
#define	DMA         2
#define	SPI         3
#define CLOCK       4
#define ISP         5
#define DPU         6
#define I2C         7
#define MIPIT1      8
#define MIPIT2      9
#define MIPIR1      10
#define MIPIR2      11
#define TIMER		12
#define VIC			13
#define WDOG		14
#define GPIO		15
#define USB2		16
#define RVL			17
#define MAILBOX		18



DEV	dev_list[dev_max];


#endif
