#include "timer_imi.h"
#include "initcalls.h"
#include "elog.h"
#include "clock.h"
#include "vic_int.h"
#include "portmacro.h"
#include "FreeRTOSConfig.h"
#include "linux_compat.h"

/*
	timer0   for delay functiong
	timer1   for trigger
	timer2   reserved
	timer64_0   for systick
	timer64_1   for DSP
*/

#if ((defined TARGET_SRAM) || (defined TARGET_DDR))
#define TIMER_CLOCK             (24 * 1000 * 1000)
#define TICKS_PER_HZ            (TIMER_CLOCK / configTICK_RATE_HZ)
#define TICKS_TO_HZ(x)          ((x) / configTICK_RATE_HZ)

#define U0_TIMER32_BASE_ADDR            (TIMER0_BASE_ADDR)
#define U1_TIMER32_BASE_ADDR            (TIMER1_BASE_ADDR)
#define U0_TIMER64_BASE_ADDR            (TIMER64_0_BASE_ADDR)
#define U1_TIMER64_BASE_ADDR            (TIMER64_1_BASE_ADDR)

/* u0_timer32 */
#define TIMER32_INT_STAT_REG        (TIMER0_BASE_ADDR)
#define TIMER32_CTRL_REG(n)         (TIMER0_BASE_ADDR + 0x04 + n * (0x40))
#define TIMER32_LD_REG(n)           (TIMER0_BASE_ADDR + 0x08 + n * (0x40))
#define TIMER32_EN_REG(n)           (TIMER0_BASE_ADDR + 0x10 + n * (0x40))
#define TIMER32_RELD_REG(n)         (TIMER0_BASE_ADDR + 0x14 + n * (0x40))
#define TIMER32_VAL_REG(n)          (TIMER0_BASE_ADDR + 0x18 + n * (0x40))
#define TIMER32_INT_CLR_REG(n)      (TIMER0_BASE_ADDR + 0x20 + n * (0x40))
#define TIMER32_INT_MASK_REG(n)     (TIMER0_BASE_ADDR + 0x24 + n * (0x40))
#define U0_WDOG_LOCK_REG(n)         (TIMER0_BASE_ADDR + 0x13c)

#define TIMER64_CTR_REG(base)                   (base + 0x04)
#define TIMER64_LOAD_L_REG(base)                (base + 0x08)
#define TIMER64_LOAD_H_REG(base)                (base + 0x0c)
#define TIMER64_EN_REG(base)                    (base + 0x10)
#define TIMER64_RELOAD_COUNTER_REG(base)        (base + 0x14)
#define TIMER64_VALUE_L_REG(base)               (base + 0x18)
#define TIMER64_VALUE_H_REG(base)               (base + 0x1c)
#define TIMER64_INT_CLR_REG(base)               (base + 0x20)
#define TIMER64_INT_MASK_REG(base)              (base + 0x24)

void ConfigureTimeForRunTimeStats(void)
{
	//todo
}

static void timer_2280_timer64_enable(enum timer  tm, enum timer64_mode mode, enum timer_int_mode interrupt,
		u32 val_l, u32 val_h)
{
	unsigned int base;
	unsigned int mask;

	if (interrupt == TIMER_INT_EN)
		mask = 0;
	else
		mask = 1;

	if (tm == TIMER_64_BIT_0)
		base = TIMER64_0_BASE_ADDR;
	else
		base = TIMER64_1_BASE_ADDR;

	writel(0x80000001, 0x30100198);         /* timer_clock_24MHz */

	writel(0x0, TIMER64_EN_REG(base));          /* disable */
	writel(0x1, TIMER64_INT_CLR_REG(base)); /* clear */
	writel(mask, TIMER64_INT_MASK_REG(base));   /* unmask */
	writel(val_l, TIMER64_LOAD_L_REG(base));    /* load_L */
	writel(val_h, TIMER64_LOAD_H_REG(base));    /* load_H */
	writel(mode, TIMER64_CTR_REG(base));        /* timer mode */
	writel(1, TIMER64_EN_REG(base));            /* enable */
}

static void timer_2280_timer32_enable(enum timer num, enum timer_mode mode, enum timer_int_mode interrupt, u32 val)
{
	unsigned int mask;

	if (interrupt == TIMER_INT_EN)
		mask = 0;
	else
		mask = 1;

	writel(0x1, 0x30100014); /* clock 24MHz */

	writel(0x0, TIMER32_EN_REG(num));       /* disable */
	writel(0x1, TIMER32_INT_CLR_REG(num));  /* int clear */
	writel(mask, TIMER32_INT_MASK_REG(num));    /* unmask */
	writel(mode, TIMER32_CTRL_REG(num));        /* timer mode */
	writel(val, TIMER32_LD_REG(num));       /* load */
	writel(0x1, TIMER32_EN_REG(num));       /* enable */
}

void imi_timer_en(enum timer tm, unsigned int  mode, enum timer_int_mode interrupt, u64 period)
{
	if (tm <= TIMER3)
		timer_2280_timer32_enable(tm, mode, interrupt, period);
	else
		timer_2280_timer64_enable(tm,  mode, interrupt, (u32)(period & 0xffffff), period >> 32);
}

static void timer_2280_timer32_int_clr(enum timer num)
{
	writel(0x1, TIMER32_INT_CLR_REG(num)); /* int clear */
}

static void timer_2280_timer64_int_clr(enum timer  tm)
{
	unsigned int base;

	if (tm == TIMER_64_BIT_0)
		base = TIMER64_0_BASE_ADDR;
	else
		base = TIMER64_1_BASE_ADDR;

	writel(0x1, TIMER64_INT_CLR_REG(base)); /* clear */
}

void imi_timer_int_clr(enum timer tm)
{
	if (tm <= TIMER3)
		timer_2280_timer32_int_clr(tm);
	else
		timer_2280_timer64_int_clr(tm);
}

static void timer_2280_timer64_disable(enum timer  tm)
{
	unsigned int base;

	if (tm == TIMER_64_BIT_0)
		base = TIMER64_0_BASE_ADDR;
	else
		base = TIMER64_1_BASE_ADDR;

	writel(0, TIMER64_EN_REG(base)); /* disable */
}

static void timer_2280_timer32_disable(enum timer num)
{
	writel(0x1, TIMER32_EN_REG(num)); /* disable */
}

void imi_timer_disable(enum timer tm)
{
	if (tm <= TIMER3)
		timer_2280_timer32_disable(tm);
	else
		timer_2280_timer64_disable(tm);
}

u32 timer_2280_get_timer32_val(enum timer tm)
{
	return readl(TIMER32_VAL_REG(tm));
}

u64 timer_2280_get_timer64_val(enum timer tm)
{
	u64 val_l, val_h;
	u64 val;
	unsigned long flags;
	unsigned int base;

	if (tm == TIMER_64_BIT_0)
		base = TIMER64_0_BASE_ADDR;
	else
		base = TIMER64_1_BASE_ADDR;

	flags = local_irq_save();
	val_l = readl(TIMER64_VALUE_L_REG(base));
	val_h = readl(TIMER64_VALUE_H_REG(base));
	local_irq_restore(flags);

	val = ((u64)(val_h) << 32) + ((u64)(val_l));

	return val;
}

u64 imi_timer_get_val(enum timer tm)
{
	if (tm <= TIMER3)
		return timer_2280_get_timer32_val(tm);
	else
		return timer_2280_get_timer64_val(tm);
}

static irqreturn_t TIM64_u0IRQHandler(int irq, void *arg)
{
	timer_2280_timer64_int_clr(TIMER_64_BIT_0);

	__asm volatile
	(
		"BL xTaskIncrementTick  \t\n" \
		"CMP r0, #0             \t\n" \
		"BEQ SkipContextSwitch  \t\n" \
		"BL vTaskSwitchContext  \t\n" \
		"SkipContextSwitch:         \t\n"
	);

	return IRQ_HANDLED;
}

int timer_init(void)
{
	volatile u64 SysTick;
	int mode = 0;
	SysTick = (0 != configTICK_RATE_HZ ?
				TICKS_PER_HZ : (u64) (-1));

	request_irq(IRQ_U0_TIMER64, TIM64_u0IRQHandler, "TIMER64", NULL);
	timer_2280_timer64_enable(TIMER_64_BIT_0, mode, TIMER_INT_EN,(u32)(SysTick & 0xffffffff), (u32)((SysTick) >> 32));

	return 0;
}

void ConfigSysTime(void)
{
	timer_2280_timer64_enable(TIMER_64_BIT_1, 0, 0, 0xffffffff, 0xffffffff);
}

void imi_get_cur_time(struct timeval_t *val)
{
	unsigned long long reg_val;
	reg_val = ~timer_2280_get_timer64_val(TIMER_64_BIT_1);

	val->tv_sec = reg_val / (200 * 1000 * 1000);
	val->tv_usec = (reg_val - val->tv_sec * TIMER_CLOCK) / 200;
}

POST_CORE_INIT(ConfigSysTime);
//EARLY_INIT(imi_time_init);
#endif
