#include "rvl.h"
#include "elog.h"
#include "imi_io.h"
#include "dev.h"
#include "clock.h"
#include "initcalls.h"
#include "delay.h"
#include "irqs.h"

extern u32 ulPortInterruptNesting;
extern u32 ulPortYieldRequired;

#define setbit(x, y) x |= (1<<y)
#define clrbit(x, y) x &= ~(1<<y)

irqreturn_t rvl_handler(int irq, void *arg)
{
   	int val = 0xDEADDEAD;
    writel(0x0, RVL_BASE_ADDR + INTR_ENABLE);   //clear interrupt
    while(readl(RVL_BASE_ADDR + INTR_STAGE) != 1);
	writel(0x0, RVL_BASE_ADDR + INTR_STAGE);    //clear stage
   	writel(0xF, 0x25F0041C);   //reset open

    ulPortYieldRequired = 0;
	ulPortInterruptNesting = 1;
    *(int *)arg = val;
   	return IRQ_HANDLED;
}

int imi_rvl_out_data(u32 src, u32 out, u32 width, u32 height)
{
    writel(src, RVL_BASE_ADDR + I_ADDRESS_IN);
    writel(out, RVL_BASE_ADDR + I_ADDRESS_OUT);
    write16((volatile unsigned short *)(RVL_BASE_ADDR + I_WIDTH), width);
    write16((volatile unsigned short *)(RVL_BASE_ADDR + I_HEIGHT), height);
    writel((int)(width * height), RVL_BASE_ADDR + I_FRAME_PIX);
    writel(setbit(readl(RVL_BASE_ADDR + I_FRAME_PIX), DATA_BIT), RVL_BASE_ADDR + I_FRAME_PIX);//1:data from memory;0:from input data
    writel(setbit(readl(RVL_BASE_ADDR + I_FRAME_PIX), MODULE_EN_BIT), RVL_BASE_ADDR + I_FRAME_PIX);
    writel(0x1, RVL_BASE_ADDR + INTR_ENABLE);   //open interrupt

    return 0;
}

int imi_dpu_rvl_out_data(u32 out, u32 width, u32 height)
{
    writel(out, RVL_BASE_ADDR + I_ADDRESS_OUT);
    write16((volatile unsigned short *)(RVL_BASE_ADDR + I_WIDTH), width);
    write16((volatile unsigned short *)(RVL_BASE_ADDR + I_HEIGHT), height);
    writel((int)(width * height), RVL_BASE_ADDR + I_FRAME_PIX);
    writel(clrbit(readl(RVL_BASE_ADDR + I_FRAME_PIX), DATA_BIT), RVL_BASE_ADDR + I_FRAME_PIX);//1:data from memory;0:from input data
    writel(setbit(readl(RVL_BASE_ADDR + I_FRAME_PIX), MODULE_EN_BIT), RVL_BASE_ADDR + I_FRAME_PIX);
    writel(0x1, RVL_BASE_ADDR + INTR_ENABLE);   //open interrupt

    return 0;
}

int imi_rvl(u32 src, u32 out, u32 width, u32 height)
{
    struct rvl_driver *my_rvl_driver = (struct rvl_driver *)(dev_list[RVL].ops);
    if (!my_rvl_driver->rvl_out_data) {
		log_e("my_rvl_driver->rvl_out_data is NULL\n");
		return -1;
	}
    my_rvl_driver->rvl_out_data(src, out, width, height);
    return 0;
}

int imi_dpu_rvl(u32 out, u32 width, u32 height)
{
    struct rvl_driver *my_rvl_driver = (struct rvl_driver *)(dev_list[RVL].ops);
    if (!my_rvl_driver->dpu_rvl_out_data) {
		log_e("my_rvl_driver->dpu_rvl_out_data is NULL\n");
		return -1;
	}
    my_rvl_driver->dpu_rvl_out_data(out, width, height);
    return 0;
}

struct rvl_driver imi_rvl_driver = {
	.rvl_out_data = imi_rvl_out_data,
	.dpu_rvl_out_data = imi_dpu_rvl_out_data,
};

void rvl_dev_init(void)
{
	clock_enable("rvl");
}

DEV_INIT(rvl_dev_init);
