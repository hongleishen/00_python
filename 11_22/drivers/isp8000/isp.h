
/*******************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        isp head
  @creator      rain zhou

  @History
  When          Who             What, where, why
  ----------    ------------    ----------------------------------------
  2021/02/07    rain zhou       the initial version
 *******************************************************************************/

#ifndef __ISP_H__
#define __ISP_H__

#include "hellboy_map.h"

/****************************************************
IMSC set Mask register
RIS  raw interrupt
MIS  mask interrupt status
ICR	 interrupt clear
ISR	 interrupt set
****************************************************/

#define	MI_IMSC			(ISP_BASE_ADDR + 0x16c0)
#define	MI_IMSC1		(ISP_BASE_ADDR + 0x16c4)
#define	MI_ISR			(ISP_BASE_ADDR + 0x16c8)
#define	MI_ISR1			(ISP_BASE_ADDR + 0x16cc)
#define	MI_MIS			(ISP_BASE_ADDR + 0x16d0)
#define	MI_MIS1			(ISP_BASE_ADDR + 0x16d4)
#define	MI_ICR			(ISP_BASE_ADDR + 0x16d8)
#define	MI_ICR1			(ISP_BASE_ADDR + 0x16dc)
#define	MI_RIS			(ISP_BASE_ADDR + 0x16e0)
#define	MI_RIS1			(ISP_BASE_ADDR + 0x16e4)
#define	MI_IMSC2		(ISP_BASE_ADDR + 0x16e8)
#define	MI_ISR2			(ISP_BASE_ADDR + 0x16ec)
#define	MI_MIS2			(ISP_BASE_ADDR + 0x16f0)
#define	MI_ICR2			(ISP_BASE_ADDR + 0x16f4)
#define	MI_RIS2			(ISP_BASE_ADDR + 0x16f8)













#endif
