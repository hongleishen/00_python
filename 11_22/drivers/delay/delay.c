/***************************************************************************
  Copyright (c) 2019 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    delay_us and delay_ms by timer32
  @author   RongFeng shao

  @History
  When         Who             What, where, why
  ----------   -------------   ----------------------------------------------
  2019/06/30   RongFeng shao   the initial version

===========================================================================
***************************************************************************/
#include "timer_imi.h"
#include "delay.h"
#include "imi_type.h"
#include "initcalls.h"

#define TIMER32_LOAD_VAL		(0xFFFFFFFFL)
#define READ_TIMER32()	        (TIMER32_LOAD_VAL - imi_timer_get_val(TIMER0))
#define USEC_TO_COUNT(x)		(x * 24)

static void timer32_init(void)
{
	imi_timer_en(TIMER0, MODE_RELOAD, TIMER_INT_DIS,TIMER32_LOAD_VAL);
}

/**
 * delayus() - delay usec useconds
 *
 * @usec: delay seconds
 */
void delayus(unsigned long usec)
{
	long tmo = USEC_TO_COUNT(usec);
	unsigned long  now, last = READ_TIMER32();

	while (tmo > 0) {
		now = READ_TIMER32();
		if (now >= last) /* normal (non rollover) */
			tmo -= now - last;
		else		/* rollover */
			tmo -= TIMER32_LOAD_VAL - last + now;
		last = now;
	}
}

void delayms(unsigned int msdelay)
{
	int i = 0;
	for (i = 0; i < msdelay; i++) {
		delayus(1000);
	}
}

void imi_delay(unsigned long cycles)
{
	u32 i;
	for(i = 0; i < cycles; i++);
}

void udelay(unsigned long cycles)
{
	while(cycles --)
		;
}
void mdelay(unsigned long cycles)
{
	int i = 0;
	for(i=0; i<cycles; i++)
	{
		udelay(1000);
	}
}

CORE_INIT(timer32_init);
