#ifndef __TIMER_FPGA_H__
#define __TIMER_FPGA_H__

#include "imi_io.h"
#include "vic_int.h"

enum timer {
	TIMER0 = 0,
	TIMER1,
	TIMER2,
	TIMER3,
	TIMER_64_BIT_0,
	TIMER_64_BIT_1,
};

enum timer_mode {
	MODE_ONE_SHOT = 0,
	MODE_RELOAD,
};

enum timer_int_mode {
	TIMER_INT_DIS = 0,
	TIMER_INT_EN = 1,
};

enum {
	WDOG_INT_DIS,
	WDOG_INT_EN,
};
//for imi2280 64bits timer
enum timer64_mode {
	auto_reload_decrease = 0,
	single_run_decrease = 1,
	auto_reload_increase = 2,
	single_run_increase = 3,
};

struct timeval_t {
	unsigned long	tv_sec;		/* seconds */
	unsigned long	tv_usec;	/* microseconds */
};

void imi_timer_en(enum timer tm, unsigned int  mode, enum timer_int_mode interrupt, u64 period);
void imi_timer_int_clr(enum timer tm);
u64 imi_timer_get_val(enum timer tm);
void imi_timer_disable(enum timer tm);

void imi_watchdog_cfg(int reset, int intr, u32 period);
void imi_get_cur_time(struct timeval_t *val);

#endif//__TIMER_FPGA_H__
