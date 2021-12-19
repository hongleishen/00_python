/***************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    clock driver
  @author

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------

  ===========================================================================
 ***************************************************************************/
#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "imi_type.h"
#include "imi_io.h"

#if defined(CHIP_IMI_1680)
#include "clock_hw_1680.h"
#endif /*CHIP_IMI_1680*/

#if defined(CHIP_IMI_2280)
#include "clock_hw_2280.h"
#endif /*CHIP_IMI_2280*/

int clock_enable(char* name);
int clock_set_freq(char* name, unsigned int freq);

struct clk {
        char* devname;
        unsigned int address;
		unsigned int clk_val;
};

void clock_dev_init(void);


#endif /* __CLOCK_H__ */
