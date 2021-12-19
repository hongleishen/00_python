/***************************************************************************
  Copyright (c) 2019 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    ddr support
  @author   Dejie Zhang

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------
  2019/03/21   Dejie Zhang   the initial version

===========================================================================
***************************************************************************/
#ifndef __DDR_H__
#define __DDR_H__

int DDR3_init(unsigned int value, void *key);
void init_chip_ddr(unsigned int ddr_clk_freq);

#endif /* __DDR_H__ */
