#include "timer_imi.h"
#include "imi_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "dev.h"
#include "initcalls.h"
#include "clock.h"
#include "elog.h"
#include "linux_compat.h"
#include "hellboy_map.h"

/*
	timer0 is for delay functiong
	timer1 is for trigger
	timer2 is reserved
	timer3 is for systick
	timer64 is for wall_time
*/

#if !((defined TARGET_SRAM) || (defined TARGET_DDR))
#define TIMER_OFF               (0x20)
#define TIMER_NUM               4

#define TIMER_BASE_ADDR(i)      (COMMON_TIMER_BASE_ADDR + 0x20 * i)
#define PERIOD_REG              (0x0)
#define COUNT_REG               (0x4)   /* read only */
#define CTRL_REG                (0x8)
#define INTCLR_REG              (0xC)   /* write only */
#define RAWINT_REG              (0x10)  /* read only */
#define INT_REG                 (0x14)  /* read only */
#define COUNT64_EN              (0x80)
#define COUNT64_VAL_L           (0x84)
#define COUNT64_VAL_H           (0x88)

#define CTRL_BIT_ONE_SHOT       (0)
#define CTRL_BIT_SIZE           (1)
#define CTRL_BIT_SCALE          (2)
#define CTRL_BIT_INT_EN         (5)
#define CTRL_BIT_MODE           (6)
#define CTRL_BIT_EN             (7)

#define WDOG_LOAD               (0x0)
#define WDOG_VAL                (0x4)
#define WDOG_CTRL               (0x8)
#define WDOG_INT_CLR            (0xC)
#define WDOG_RAWINT             (0x10) /* read only */
#define WDOG_MASKINT            (0x14) /* read only */
#define WDOG_TEST               (0x418)
#define WDOG_LOCK               (0xC00) /* unlock:0, lock:1*/
#define WDOG_PERI_4             (0xFD0)
#define WDOG_PERI_5             (0xFD4)
#define WDOG_PERI_6             (0xFD8)
#define WDOG_PERI_7             (0xFDC)
#define WDOG_PERI_0             (0xFE0)
#define WDOG_PERI_1             (0xFE4)
#define WDOG_PERI_2             (0xFE8)
#define WDOG_PERI_3             (0xFEC)
#define WDOG_PCELL_0            (0xFF0)
#define WDOG_PCELL_1            (0xFF4)
#define WDOG_PCELL_2            (0xFF8)
#define WDOG_PCELL_3            (0xFFC)

#define WDOG_CTRL_BIT_INTEN     (0)
#define WDOG_CTRL_BIT_RESEN     (1)

#define WTDOG_UNLOCK_VAL        0x1ACCE551
#define WTDOG_LOCK_VAL          0x1

#define TIMER_CLOCK             (24 * 1000 * 1000)
#define TICKS_PER_HZ            (TIMER_CLOCK / configTICK_RATE_HZ)
#define TICKS_TO_HZ(x)          ((x) / configTICK_RATE_HZ)

enum {
	SIZE_16_BIT = 0,
	SIZE_32_BIT = 1,
};

enum {
	WDOG_RES_DIS,
	WDOG_RES_EN,
};

static void write_timer_reg(u32 val, u32 reg, u32 num)
{
	writel(val, reg + num * TIMER_OFF + COMMON_TIMER_BASE_ADDR);
}

static u32 read_timer_reg(u32 reg, u32 num)
{
	return readl(reg + num * TIMER_OFF + COMMON_TIMER_BASE_ADDR);
}

static void timer_1680_timer32_en(u32 timer, u32 mode, u32 interrupt, u32 period)
{
	unsigned int val;

	if (mode == MODE_RELOAD)
			mode = 6;

	if (timer >= 0 && timer < 4) {
		if (period)
			write_timer_reg(period, PERIOD_REG, timer);

		val = read_timer_reg(CTRL_REG, timer) & 0x0;
		val |= (1 << mode); /* bit[0]:one shot; bit[6]:reload mode */
		val |= (1 << CTRL_BIT_SIZE); /* 32 bit */
		if (interrupt)
			val |= (1 << CTRL_BIT_INT_EN); /* interrupt en/dis */
		val |= (1 << CTRL_BIT_EN); /* timer en */
		write_timer_reg(val, CTRL_REG, timer);
	}
}

static void timer_1680_timer32_disable(u32 timer)
{
	unsigned int val;

	val = read_timer_reg(CTRL_REG, timer);
	val &= ~(1 << CTRL_BIT_EN);
	write_timer_reg(val, CTRL_REG, timer);
}

static void timer_1680_timer64_disable(u32 timer)
{
	write_timer_reg(0x0, COUNT64_EN, 0);
}

void imi_timer_disable(enum timer tm)
{
	if (tm <= TIMER3)
		timer_1680_timer32_disable(tm);
	else
		timer_1680_timer64_disable(tm);
}

static void timer_1680_timer64_en(u32 val_l, u32 val_h)
{
	write_timer_reg(val_l, COUNT64_VAL_L, 0);
	write_timer_reg(val_h, COUNT64_VAL_H, 0);
	write_timer_reg(0x1, COUNT64_EN, 0);
}

void imi_timer_en(enum timer tm, unsigned int  mode, enum timer_int_mode interrupt, u64 period)
{
	if (tm <= TIMER3)
		timer_1680_timer32_en(tm, mode, interrupt, (u32)period);
	else
		timer_1680_timer64_en((u32)(period & 0xffffffff), (u32)(period >> 32 ));
}

static void timer_1680_int_clr(enum timer tm)
{
	write_timer_reg(0x1, INTCLR_REG, tm);
}

void imi_timer_int_clr(enum timer tm)
{
	if (tm <= TIMER3)
		timer_1680_int_clr(tm);
	else
		imi_printf("timer num err\n");
}

static u32 timer_1680_get_timer32_val(u32 timer)
{
	return read_timer_reg(COUNT_REG, timer);
}

static u64 timer_1680_get_timer64_val(u32 timer)
{
	u32 val_l, val_h;
	u64 val;
	unsigned long flags;

	flags = local_irq_save();
	val_l = read_timer_reg(COUNT64_VAL_L, 0);
	val_h = read_timer_reg(COUNT64_VAL_H, 0);
	local_irq_restore(flags);

	val = ((u64)(val_h) << 32) + ((u64)(val_l));

	return val;
}

u64 imi_timer_get_val(enum timer tm)
{
	if (tm <= TIMER3)
		return timer_1680_get_timer32_val(tm);
	else
		return timer_1680_get_timer64_val(tm);
}

irqreturn_t common_timer0_handler(int irq, void *arg)
{
	timer_1680_int_clr(TIMER0);
	return IRQ_HANDLED;
}

static irqreturn_t common_timer3_handler(int irq, void *arg)
{
	int timer = TIMER3;

	timer_1680_int_clr(timer);

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

void ConfigSysTime(void)
{
	imi_timer_en(TIMER_64_BIT_0, 0, 0, 0);
}

void imi_get_cur_time(struct timeval_t *val)
{
	unsigned long long reg_val;
	reg_val = timer_1680_get_timer64_val(TIMER_64_BIT_0);
	val->tv_sec = reg_val / TIMER_CLOCK;
	val->tv_usec = (reg_val - val->tv_sec * TIMER_CLOCK) / 24;
}

int timer_init(void)
{
	int timer = TIMER3;
	unsigned int period = 0x1312D00;
	int intr = TIMER_INT_EN;
	int mode = MODE_RELOAD;
	clock_enable("ctimer");
	period = (0 != configTICK_RATE_HZ ?
				TICKS_PER_HZ : (u32) (-1));

	request_irq(IRQ_TIMER_3, common_timer3_handler, "COMMON TIMER3", NULL);
	timer_1680_timer32_en(timer, mode, intr, period);

	return 0;
}

/*
	watchdog
*/
static void write_wdog_reg(u32 val, u32 reg)
{
	writel(val, reg + WATCHDOG_BASE_ADDR);
}

static u32 read_wdog_reg(u32 reg)
{
	return readl(reg + WATCHDOG_BASE_ADDR);
}

void imi_watchdog_cfg(int reset, int intr, u32 period)
{
	unsigned int ret;

	write_wdog_reg(WTDOG_UNLOCK_VAL, WDOG_LOCK);
	ret = read_wdog_reg(WDOG_CTRL);

	if (reset)
		ret |= (1 << WDOG_CTRL_BIT_RESEN);
	if (intr)
		ret |= (1 << WDOG_CTRL_BIT_INTEN);
	write_wdog_reg(ret, WDOG_CTRL);
	write_wdog_reg(period, WDOG_LOAD);
	write_wdog_reg(WTDOG_LOCK_VAL, WDOG_LOCK);
}

irqreturn_t watchdog_handler(int irq, void *arg)
{
	write_wdog_reg(0x1, WDOG_INT_CLR);
	return IRQ_HANDLED;
}

static void wdog_module_init(void)
{
	write_wdog_reg(WTDOG_UNLOCK_VAL, WDOG_LOCK);
	write_wdog_reg(0x1, WDOG_INT_CLR);
	write_wdog_reg(0x0, WDOG_CTRL);
}

void wdog_device_init(void)
{
	clock_enable("wtimer");
	wdog_module_init();
}

static void timer_module_init(void)
{
	u32 num;

	for (num = 0; num <= TIMER3; num++) {
		write_timer_reg(0x0, CTRL_REG, num);//timer32_dis
		write_timer_reg(0x1, INTCLR_REG, num);
	}
	write_timer_reg(0x0, COUNT64_EN, 0);//timer64_dis
}

void timer_device_init(void)
{
	clock_enable("ctimer");
	timer_module_init();
}

CORE_INIT(timer_device_init);
POST_CORE_INIT(ConfigSysTime);
DEV_INIT(wdog_device_init);
#endif
