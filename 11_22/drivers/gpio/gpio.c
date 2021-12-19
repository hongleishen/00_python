#include "gpio.h"
#include "clock.h"
#include "dev.h"
#include "initcalls.h"
#include "elog.h"
#include "vic_int.h"
#include "linux_list.h"
#include "errno.h"
#include "linux_compat.h"

#if defined(CHIP_IMI_2280)

static const char gpio_remap[NGPIO_NUM_MAX] = {
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
	10, 11, 64, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
	82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
	92, 93, 94, 95, 50, 51, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61, 62, 36, 37, 38
};

#define GPIO_GET_BANK_ADDR(pin)         (GPIO_BASE_ADDR + ((pin) / 32) * GPIO_BANK_GAP)
#define GPIO_GET_CFG(pin)           GPIO_CFG_REG(((pin) % 32))
#define GPIO_GET_STAT(pin)          GPIO_STS_REG(((pin) % 32))

irqreturn_t gpio_handler1 (int irq, void *arg)
{
	return IRQ_HANDLED;
}

void gpio_module_init(void)
{
}

void gpad_request_one(unsigned int gpio, int flag)
{
}

inline int gpio_request_one(unsigned gpio, unsigned long flags, const char *label)
{
	return 0;
}

void gpio_set_debounce (unsigned int gpio, unsigned int debounce)
{
}

void gpio_set_value(unsigned int gpio, int value)
{
	unsigned int val, reg;
	int pin;

	if (gpio > GPIO_NUM_MAX) {
		pin = gpio & 0xFF;
	} else if (gpio < NGPIO_NUM_MAX) {
		pin = gpio_remap[gpio];
	} else {
		configASSERT(0);
		imi_printf("%s err gpio %x\n", __func__, gpio);
		return;
	}

	reg = GPIO_GET_BANK_ADDR(pin) + GPIO_GET_CFG(pin);
	val = readl(reg) & 0xfc;
	val |= (1 << CFG_BIT_DIR);
	if (value)
		val |= (1 << CFG_BIT_OUT);
	writel(val, reg);
}

int gpio_get_value(unsigned int gpio)
{
	unsigned int val, reg;
	int pin;

	if (gpio > GPIO_NUM_MAX) {
		pin = gpio & 0xFF;
	} else if (gpio < NGPIO_NUM_MAX) {
		pin = gpio_remap[gpio];
	} else {
		configASSERT(0);
		imi_printf("%s err gpio %x\n", __func__, gpio);
		return -EINVAL;
	}

	reg = GPIO_GET_BANK_ADDR(pin) + GPIO_GET_STAT(pin);
	val = readl(reg) & (1 << STS_BIT_IN);
	return !!val;
}

void gpio_direction_output(unsigned int gpio, int value)
{
	unsigned int val, reg;
	int pin;

	if (gpio > GPIO_NUM_MAX) {
		pin = gpio & 0xFF;
	} else if (gpio < NGPIO_NUM_MAX) {
		pin = gpio_remap[gpio];
	} else {
		configASSERT(0);
		imi_printf("%s err gpio %x\n", __func__, gpio);
		return;
	}

	reg = GPIO_GET_BANK_ADDR(pin) + GPIO_GET_CFG(pin);
	val = readl(reg) & 0xF8;
	val |= (1 << CFG_BIT_DIR);
	if (value)
		val |= (1 << CFG_BIT_OUT);
	writel(val, reg);
}

void gpio_direction_input(unsigned int gpio)
{
	unsigned int val, reg;
	int pin;

	if (gpio > GPIO_NUM_MAX) {
		pin = gpio & 0xFF;
	} else if (gpio < NGPIO_NUM_MAX) {
		pin = gpio_remap[gpio];
	} else {
		configASSERT(0);
		imi_printf("%s err gpio %x\n", __func__, gpio);
		return;
	}

	reg = GPIO_GET_BANK_ADDR(pin) + GPIO_GET_CFG(pin);
	val = readl(reg) & 0xF8;
	val |= (1 << CFG_BIT_IEN);
	writel(val, reg);
}

int gpio_get_direction(unsigned int gpio)
{
	unsigned int val, reg;
	int pin;

	if (gpio > GPIO_NUM_MAX) {
		pin = gpio & 0xFF;
	} else if (gpio < NGPIO_NUM_MAX) {
		pin = gpio_remap[gpio];
	} else {
		configASSERT(0);
		imi_printf("%s err gpio %x\n", __func__, gpio);
		return -EINVAL;
	}

	reg = GPIO_GET_BANK_ADDR(pin) + GPIO_GET_CFG(pin);
	val = readl(reg) & (1 << CFG_BIT_DIR);
	return !!val;
}

int gpio_irq_set_type(unsigned int gpio, unsigned int type)
{
	return 0;
}

void gpio_irq_mask(unsigned int gpio)
{
}

void gpio_irq_unmask(unsigned int gpio)
{
}

int gpio_request_irq(unsigned int irq, irq_handler_t handler, const char *name, void *arg)
{
	return -EINVAL;
}

void gpio_print_irq_statistics(void)
{
}

void gpio_device_init(void)
{
	clock_enable("gpio");

	gpio_module_init();
}

int gpio_is_valid(int gpio)
{
	return 1;
}
ARCH_INIT(gpio_device_init);


#elif defined(CHIP_IMI_1680)
static LIST_HEAD(gpio_irq_list);
struct gpio_irq_desc {
	struct list_head node;
	struct irq_desc  desc;
};

static struct irq_desc *gpio_find_irq_desc(int hw_irq)
{
	struct gpio_irq_desc *tmp = NULL;
	unsigned long flags;

	flags = local_irq_save();
	list_for_each_entry(tmp, &gpio_irq_list, node) {
		if (tmp->desc.irq_data.hwirq == hw_irq) {
			local_irq_restore(flags);
			return &tmp->desc;
		}
	}
	local_irq_restore(flags);

	return NULL;
}

void gpio_print_irq_statistics(void)
{
	struct gpio_irq_desc *tmp = NULL;
	unsigned long flags;

	flags = local_irq_save();
	list_for_each_entry(tmp, &gpio_irq_list, node) {
		imi_printf("%d,    %s,        %d\r\n", tmp->desc.irq, tmp->desc.name ?  tmp->desc.name : "NULL", tmp->desc.irq_cnt);
	}
	local_irq_restore(flags);
}

static inline unsigned int gpio_get_int_status_reg_base(int irq)
{
	switch (irq) {
	case IRQ_GPIO_BANK_0:
		return GPIO_MINT0_REG;

	case IRQ_GPIO_BANK_1:
		return GPIO_MINT1_REG;

	case IRQ_GPIO_BANK_2:
		return GPIO_MINT2_REG;

	case IRQ_GPIO_BANK_3:
		return GPIO_MINT3_REG;

	default:
		pr_err("irq invalid\n");
		break;
	}
	return 0;
}

void gpio_module_init(void)
{
	unsigned int i, val;

	/* clear and mask all interrupts*/
	for (i = 0; i < NGPIO_NUM; i++) {
		writel(1 << STS_BIT_INTR, GPIO_STS_REG(i));

		val = readl(GPIO_CFG_REG(i));
		val |= (1 << CFG_BIT_INTR_MASK);
		val &= ~(1 << CFG_BIT_INTR_EN);
		writel(val, GPIO_CFG_REG(i));
	}
}

void gpad_request_one(unsigned int gpio, int flag)
{
	unsigned int val, reg, div, rem;

	if (gpio < 0 && gpio >= NGPIO_NUM) {
		log_e("gpio %d is not exit\n", gpio);
		return ;
	}

	div = gpio / 32;
	rem = gpio % 32;

	switch (div) {
	case 0 :
			reg = GIOM_CFG0;
			break;
	case 1 :
			reg = GIOM_CFG1;
			break;
	case 2 :
			reg = GIOM_CFG2;
			break;
	case 3 :
			reg = GIOM_CFG3;
			break;
	default:
			return ;
	}

	val = readl(reg);
	if(flag)
		val |= (1 << rem);
	else
		val &= ~(1 << rem);
	writel(val, reg);
}

void gpio_set_debounce (unsigned int gpio, unsigned int debounce)
{
	unsigned int val, reg;

	reg = GPIO_DB_PERIOD_REG;
	writel(debounce, reg);

	reg = GPIO_CFG_REG(gpio);
	val = readl(reg);
	if (debounce)
		val |= (1 << CFG_BIT_DB_EN);
	else
		val &= ~(1 << CFG_BIT_DB_EN);
	writel(val, reg);
}

void gpio_set_value(unsigned int gpio, int value)
{
	unsigned int val, reg;

	reg = GPIO_CFG_REG(gpio);
	val = readl(reg) & 0xfc;
	val |= (1 << CFG_BIT_DIR);
	if (value)
		val |= (1 << CFG_BIT_OUT);
	writel(val, reg);
}

int gpio_get_value(unsigned int gpio)
{
	unsigned int val, reg;

	reg = GPIO_STS_REG(gpio);
	val = readl(reg) & (1 << STS_BIT_IN);
	return !!val;
}

void gpio_direction_output(unsigned int gpio, int value)
{
	unsigned int val, reg;

	reg = GPIO_CFG_REG(gpio);
	val = readl(reg) & 0xf8;
	val |= (1 << CFG_BIT_DIR);
	if (value)
		val |= (1 << CFG_BIT_OUT);
	writel(val, reg);
}

void gpio_direction_input(unsigned int gpio)
{
	unsigned int val, reg;

	reg = GPIO_CFG_REG(gpio);
	val = readl(reg) & 0xf8;
	val |= (1 << CFG_BIT_IEN);
	writel(val, reg);
}

int gpio_get_direction(unsigned int gpio)
{
	unsigned int val, reg;

	reg = GPIO_CFG_REG(gpio);
	val = readl(reg) & (1 << CFG_BIT_DIR);
	return !!val;
}

int gpio_irq_set_type(unsigned int gpio, unsigned int type)
{
	unsigned int val, reg;

	reg = GPIO_CFG_REG(gpio);
	if ((type >= 0) && (type < 4)) {
		val = readl(reg) & 0x9F;
		val |= (type << CFG_BIT_INTR_TYPE);
		writel(val, reg);
	}
	return 0;
}

void gpio_irq_mask(unsigned int gpio)
{
	unsigned int val, reg;

	reg = GPIO_CFG_REG(gpio);
	val = readl(reg);
	val |= (1 << CFG_BIT_INTR_MASK);
	val &= ~(1 << CFG_BIT_INTR_EN);
	writel(val, reg);
}

void gpio_irq_unmask(unsigned int gpio)
{
	unsigned int val, reg;

	reg = GPIO_CFG_REG(gpio);
	val = readl(reg);
	val &= ~(1 << CFG_BIT_INTR_MASK);
	val |= (1 << CFG_BIT_INTR_EN);
	writel(val, reg);
}

irqreturn_t gpio_bank_irq_handler(int irq, void *arg)
{
	unsigned int val, gpio, index;
	struct irq_desc *desc = NULL;
	unsigned int base = gpio_get_int_status_reg_base(irq);

	val = readl(base);
	for (index = 0; index < 32; index++) {
		if (val & (1 << index)) {
			gpio = index + (irq - 1) * 32;
			writel(1 << STS_BIT_INTR, GPIO_STS_REG(gpio));

			gpio_irq_mask(gpio);

			desc = gpio_find_irq_desc(gpio);
			if (desc) {
				desc->handler(desc->irq, desc->arg);
				desc->irq_cnt++;
			} else
				pr_err("unknown irq, gpio is: %d\n", gpio);

			gpio_irq_unmask(gpio);
		}
	}

	return IRQ_HANDLED;
}
inline int gpio_request_one(unsigned gpio, unsigned long flags, const char *label)
{
	return 0;
}
int gpio_is_valid(int gpio)
{
	return 1;
}

int gpio_request_irq(unsigned int irq, irq_handler_t handler, const char *name, void *arg)
{
	struct gpio_irq_desc *desc;
	unsigned long flags;

	if (irq < NR_VIC_IRQS) {
		pr_err("irq: %d num err\n", irq);
		return -EINVAL;
	}

	if (gpio_find_irq_desc(irq - NR_VIC_IRQS)) {
		pr_err("irq : %d has been requested\n", irq);
		return -EINVAL;
	}

	desc = pvPortMalloc(sizeof(struct gpio_irq_desc));
	if (desc == NULL) {
		pr_err("alloc memory fail\n");
		return -ENOMEM;
	}

	desc->desc.irq = irq;
	desc->desc.handler = handler;
	desc->desc.name = name;
	desc->desc.arg = arg;
	desc->desc.irq_data.hwirq = irq - NR_VIC_IRQS;

	flags = local_irq_save();
	list_add_tail(&desc->node, &gpio_irq_list);
	local_irq_restore(flags);

	gpio_irq_unmask(irq - NR_VIC_IRQS);

	return 0;
}

static void gpio_irq_init(void)
{
	unsigned int index;

	for (index = IRQ_GPIO_BANK_0; index < IRQ_GPIO_BANK_4; index++)
		request_irq(index, gpio_bank_irq_handler, "gpio", NULL);
}

void gpio_device_init(void)
{
	clock_enable("gpio");

	gpio_module_init();

	gpio_irq_init();
}

DEV_INIT(gpio_device_init);

#endif
