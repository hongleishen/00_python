#include "vic_int.h"
#include "initcalls.h"
#include "elog.h"
#include "gpio.h"
#include "linux_compat.h"
#include "errno.h"

unsigned int cpu_flag = 0;

struct irq_desc irq_desc[NR_VIC_IRQS];

unsigned long vic_addr[VIC_NR] = {VIC0_BASE_ADDR, VIC1_BASE_ADDR,
								  VIC2_BASE_ADDR, VIC3_BASE_ADDR};

struct irq_desc *irq_to_desc(unsigned int irq)
{
	return (irq < NR_VIC_IRQS) ? irq_desc + irq : NULL;
}

void vic_unmask_irq(unsigned int base, unsigned int irq)
{
	writel(1 << irq, base + VIC_INT_ENABLE);
}

void vic_mask_irq(unsigned int base, unsigned int irq)
{
	writel(1 << irq, base + VIC_INT_ENABLE_CLEAR);
}

void vic_ack_irq(unsigned int base, unsigned int irq)
{
	writel(1 << irq, base + VIC_INT_ENABLE_CLEAR);
	writel(1 << irq, base + VIC_INT_SOFT_CLEAR);
}

void vic_disable(unsigned int base)
{
	writel(0x0, base + VIC_INT_SELECT); /* select irq, not fiq */
	writel(0x0, base + VIC_INT_ENABLE); /* mask all irq */
	writel(~0x0, base + VIC_INT_ENABLE_CLEAR); /* clear all interrupts */
	writel(0x0, base + VIC_ITCR); /* vector address register */
	writel(~0x0, base + VIC_INT_SOFT_CLEAR); /* clear all software interrupts */
}

void irq_enable(struct irq_desc *desc)
{
	vic_unmask_irq(desc->irq_data.base, desc->irq_data.hwirq);
}

int setup_irq(unsigned int irq, struct irq_desc *desc)
{
	int vic_num, irq_num;

	if (!desc)
		return (-1);

	desc->irq_data.irq = desc->irq;
	if(irq != desc->irq_data.irq)
		return (-1);

	vic_num = irq / VIC_BITS;
	irq_num = irq % VIC_BITS;
	if (vic_num < 0 || vic_num > VIC_NR)
		return (-EINVAL);
	if (irq_num < 0 || irq_num > VIC_BITS)
		return (-EINVAL);

	desc->irq_data.base = (unsigned int)vic_addr[vic_num];
	desc->irq_data.hwirq = irq_num;
	irq_enable(desc);

	return 0;
}

int request_irq(unsigned int irq, irq_handler_t handler, const char *name, void *arg)
{
	struct irq_desc *desc;
	int retval;

	if (irq >= NR_VIC_IRQS)
		return gpio_request_irq(irq, handler, name, arg);

	desc = irq_to_desc(irq);
	if (!desc)
		return -EINVAL;

	if (!handler) {
		log_e("your irq handler func is NULL\n");
		return -EINVAL;
	}

	if (desc->handler != NULL) {
		imi_printf("irq :%d is busy\n", irq);
		return -EINVAL;
	}

	desc->irq = irq;
	desc->handler = handler;
	desc->name = name;
	desc->arg = arg;

	retval = setup_irq(irq, desc);
	if (retval < 0) {
		log_e("request_irq fail\n");
		return (-EINVAL);
	}

	return retval;
}

int handle_one_vic(unsigned int base, int i)
{
	unsigned int stat, irq, hwirq;
	int handled = 0;

	while ((stat = readl(base))) {
		hwirq = ffs(stat) -1;
		irq = hwirq + i * 32;
		vic_mask_irq(base, hwirq);
		if (irq_desc[irq].handler) {
			irq_desc[irq].handler(irq_desc[irq].irq, irq_desc[irq].arg);
			irq_desc[irq].irq_cnt++;
		} else {
			imi_printf("unknown irq : %d\n", irq);
		}
		vic_unmask_irq(base, hwirq);
		handled = 1;
	}

	return handled;
}

void vic_print_irq_statistics(void)
{
	int index = 0;
	struct irq_desc *desc;

	imi_printf("irq    name        irq_cnt\r\n");
	for (index = 0; index < NR_VIC_IRQS; index++) {
		desc = &irq_desc[index];
		if (desc->handler != NULL) {
			imi_printf("%d,    %s,        %d\r\n", desc->irq, desc->name ?  desc->name : "NULL", desc->irq_cnt);
		}
	}

	gpio_print_irq_statistics();
}

void vic_handle_irq(void)
{
	int i, handled;

	do {
		for (i = 0, handled = 0; i < VIC_NR; i++) {
			handled |= handle_one_vic(vic_addr[i], i);
		}
	} while (handled);
}

void vic_init(void)
{
	 int i;
#if  defined(CHIP_IMI_1680)
	 if (cpu_flag == 1) {
		vic_addr[0] = VIC0_BASE_ADDR+0x400000;
		vic_addr[1] = VIC1_BASE_ADDR+0x400000;
		vic_addr[2] = VIC2_BASE_ADDR+0x400000;
		vic_addr[3] = VIC3_BASE_ADDR+0x400000;

	 }
#endif
	 memset(&irq_desc, 0x00, sizeof(irq_desc));

	 for (i = 0; i < VIC_NR; i++) {
		vic_disable((unsigned int)vic_addr[i]);
	 }
}

CORE_INIT(vic_init);
