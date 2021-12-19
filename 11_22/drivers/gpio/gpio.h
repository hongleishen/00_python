#ifndef __GPIO_HANDLER__
#define __GPIO_HANDLER__
#if defined(CHIP_IMI_2280)
#include "gpio_hw_2280.h"
#elif defined(CHIP_IMI_1680)
#include "gpio_hw_1680.h"
#endif

#include "vic_int.h"

enum {
	IRQ_TYPE_HIGH_LEVEL = 0,
	IRQ_TYPE_LOW_LEVEL = 1,
	IRQ_TYPE_RISING_EDGE = 2,
	IRQ_TYPE_FALLING_EDGE = 3,
};

enum {
	OUTPUT_LOW_LEVEL = 0,
	OUTPUT_HIGH_LEVEL = 1,
};
/*
enum {
	GPAD_FUNC,
	GPAD_GPIO,
};
*/
void gpad_request_one(unsigned int gpio, int flag);
int gpio_request_one(unsigned gpio, unsigned long flags, const char *label);
void gpio_set_debounce (unsigned int gpio, unsigned int debounce);
void gpio_set_value(unsigned int gpio, int value);
int gpio_get_value(unsigned int gpio);
void gpio_direction_output(unsigned int gpio, int value);
void gpio_direction_input(unsigned int gpio);
int gpio_get_direction(unsigned int gpio);
int gpio_irq_set_type(unsigned int gpio, unsigned int type);
void gpio_irq_mask(unsigned int gpio);
void gpio_irq_unmask(unsigned int gpio);
int gpio_request_irq(unsigned int irq, irq_handler_t handler, const char *name, void *arg);
int gpio_is_valid(int gpio);
void gpio_print_irq_statistics(void);
#endif
