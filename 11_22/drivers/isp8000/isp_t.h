/*******************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        isp test driver
  @creator      rain zhou

  @History
  When          Who             What, where, why
  ----------    ------------    ----------------------------------------
  2021/01/26    rain zhou       the initial version
 *******************************************************************************/

#ifndef __ISP_T_H__
#define __ISP_T_H__

#include "initcalls.h"
#include "vic_int.h"

#define MCM1		0
#define HDR0		1
#define LI			2
#define Bypass		3
#define IR			4
#define	TPG			5
#define IRQ0		6
#define	IRQ1		7
#define	IRQ2		8
#define	THDN		9
#define	DSensor		10
#define	Sensor1		11
#define	RET			12
#define	HDR1		13
#define	HDR2		14
#define	HDR3		15
#define	HDR4		16
#define	HDR5		17
#define	HDR6		18











#define ISP_CLK		200
int isp_t_init(int gid);

struct ISP_ops {
	void (*IOCTL)(unsigned int flag);
} isp_ops;

typedef struct case_type{
	char*	name;
	int		num;
} c_type;

typedef struct isp_conf{
	int		irq;
	int		sensor;
	int		tpg;
	int		mipi;
	int		hdr;
	int		ret;
} c_isp_conf;

void isp_init(void);
void isp_test_main(void *select ,int n);
irqreturn_t ISP_IRQHandler(int irq, void *arg);





#endif
