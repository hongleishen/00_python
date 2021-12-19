#include "imi_io.h"
#include "dev.h"
#include "clock.h"
#include "initcalls.h"
#include "delay.h"
#include "mailbox.h"
#include "hellboy_map.h"
#include "linux_compat.h"
#include "imi_mailbox.h"

extern u32 ulPortInterruptNesting;
extern u32 ulPortYieldRequired;

//a2b function
static void imi_a2b_enable(struct mail_desc *desc)
{
	writel(0xf, desc->base_addr + REG_A2B_INTEN);
}

static void imi_a2b_clear(struct mail_desc *desc)
{
	writel(0xf, desc->base_addr + REG_A2B_STATUS);
}

static u32 imi_a2b_status(struct mail_desc *desc)
{
	return (0xf & readl(desc->base_addr + REG_A2B_STATUS));
}

static void imi_a2b_trigger(struct mail_desc *desc, u32 channel)
{
	writel(0x0, (desc->cross_addr + REG_A2B_TRIGGER0 + channel * 8));
}

//b2a function
static void imi_b2a_enable(struct mail_desc *desc)
{
	writel(0xf, desc->cross_addr + REG_B2A_INTEN);
}

static void imi_b2a_clear(struct mail_desc *desc)
{
	writel(0xf, desc->cross_addr + REG_B2A_STATUS);
}

static u32 imi_b2a_status(struct mail_desc *desc)
{
	return (0xf & readl(desc->cross_addr + REG_B2A_STATUS));
}

static void imi_b2a_trigger(struct mail_desc *desc, u32 channel)
{
	writel(0x0, (desc->base_addr + REG_B2A_TRIGGER0 + channel * 8));
}

struct mailbox_driver imi_mailbox_driver = {
	.a2b_enable = imi_a2b_enable,
   	.a2b_clear = imi_a2b_clear,
	.a2b_status = imi_a2b_status,
	.a2b_trigger = imi_a2b_trigger,
    .b2a_enable = imi_b2a_enable,
   	.b2a_clear = imi_b2a_clear,
	.b2a_status = imi_b2a_status,
	.b2a_trigger = imi_b2a_trigger,
};

irqreturn_t mailbox_a2b00_handler(int irq, void *arg)
{
    imi1680_a2b_clear(&imi_mail_desc);
    //imi_printf("mailbox a2b\n");
   	int val = 0xDEADDEAD;

    ulPortYieldRequired = 0;
	ulPortInterruptNesting = 1;
    *(int *)arg = val;
   	return IRQ_HANDLED;
}

irqreturn_t mailbox_b2a00_handler(int irq, void *arg)
{
    imi1680_b2a_clear(&imi_mail_desc);
    //imi_printf("mailbox b2a\n");
   	int val = 0xDEADDEAD;

    ulPortYieldRequired = 0;
	ulPortInterruptNesting = 1;
    *(int *)arg = val;
   	return IRQ_HANDLED;
}

void mailbox_init(void)
{
    imi_mail_desc.base_addr = MBOX0_BASE_ADDR;
    imi_mail_desc.cross_addr = MBOX0_BASE_ADDR;
    imi_mail_desc.mbox_id = 00;
}


void mailbox_dev_init(void)
{

	clock_enable("mailbox");

    mailbox_init();
}

ARCH_INIT(mailbox_dev_init);
