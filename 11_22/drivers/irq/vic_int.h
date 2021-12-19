#ifndef __VIC_INT_H__
#define __VIC_INT_H__

#include "imi_type.h"
#include "imi_io.h"

#if defined(CHIP_IMI_2280)
#include "hellboy_map.h"
#elif defined(CHIP_IMI_1680)
#include "hellboy_map.h"
#endif
#include "irqs.h"

#if defined(CHIP_IMI_2280)
#define VIC_NR                      0x4
#elif defined(CHIP_IMI_1680)
#define VIC_NR                      0x4
#endif

#define VIC_IRQ_STATUS              0x00
#define VIC_FIQ_STATUS              0x04
#define VIC_RAW_STATUS              0x08
#define VIC_INT_SELECT              0x0c /* 1 = FIQ, 0 = IRQ */
#define VIC_INT_ENABLE              0x10 /* 1 = enable, 0 = disable */
#define VIC_INT_ENABLE_CLEAR        0x14
#define VIC_INT_SOFT                0x18
#define VIC_INT_SOFT_CLEAR          0x1c
#define VIC_PROTECT                 0x20
#define VIC_PL190_VECT_ADDR         0x30 /* PL190 only */
#define VIC_PL190_DEF_VECT_ADDR     0x34 /* PL190 only */
#define VIC_VECT_ADDR0              0x100 /* 0 to 15 (0..31 PL192) */
#define VIC_VECT_CNTL0              0x200 /* 0 to 15 (0..31 PL192) */
#define VIC_ITCR                    0x300 /* VIC test control register */
#define VIC_VECT_CNTL_ENABLE        (1 << 5)

enum irqreturn {
	IRQ_NONE = 0,
	IRQ_HANDLED = 1,
	IRQ_WAKE_TASK = 2,
};

typedef enum irqreturn irqreturn_t;
typedef irqreturn_t (*irq_handler_t)(int, void *);
#define IRQ_RETVAL(x)   ((x) ? IRQ_HANDLED : IRQ_NONE)

struct irq_data {
	unsigned int irq;
	unsigned int hwirq;
	unsigned int base;
};

struct irq_desc {
	unsigned int irq;
	irq_handler_t handler;
	const char *name;
	void *arg;
	struct irq_data irq_data;
	unsigned int irq_cnt;
};

int request_irq(unsigned int irq, irq_handler_t handler, const char *name, void *arg);
void vic_mask_irq(unsigned int base, unsigned int irq);
void vic_handle_irq(void);
void vic_init(void);
void vic_print_irq_statistics(void);

#endif//__VIC_INT_H__
