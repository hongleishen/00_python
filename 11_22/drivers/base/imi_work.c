#include <stdlib.h>
#include <linux_compat.h>
#include <linux_list.h>
#include <errno.h>
#include <imi_dev.h>
#include "FreeRTOS.h"
#include "initcalls.h"
#include "imi_work.h"
static struct list_head irq_work_head;
static TaskHandle_t irq_work_handle;
static FRTOS_mutex  irq_mutex;
static spinlock_t irq_lock;

int schedule_work(struct work_struct *work)
{
	BaseType_t high_work;
	unsigned long flags;

	spin_lock_irqsave(&irq_lock, flags);
	++work->state;
	spin_unlock_irqrestore(&irq_lock, flags);

	vTaskNotifyGiveFromISR(irq_work_handle, &high_work);
	return 0;
}

void init_work(struct work_struct *work, void *func, void *param)
{
	work->state = 0;
	work->data = (unsigned long)param;
	work->func = (work_func_t)func;
	INIT_LIST_HEAD(&work->entry);

	FRTOS_mutex_lock(irq_mutex);
	list_add_tail(&work->entry, &irq_work_head);
	FRTOS_mutex_unlock(irq_mutex);
}

static void irq_bootom_half_work(void *param)
{
	struct work_struct *cur;
	unsigned long flags;

	while (1) {
		ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

		FRTOS_mutex_lock(irq_mutex);
		list_for_each_entry(cur, &irq_work_head, entry) {
			if (cur->state > 0) {
					cur->func((struct work_struct *)(cur->data));
					spin_lock_irqsave(&irq_lock, flags);
					--cur->state;
					spin_unlock_irqrestore(&irq_lock, flags);
			}
		}
		FRTOS_mutex_unlock(irq_mutex);
	}		
}

static void irq_bootom_half_init(void)
{
	INIT_LIST_HEAD(&irq_work_head);

	spin_lock_init(&irq_lock);
	FRTOS_mutex_create(&irq_mutex);
	xTaskCreate(&irq_bootom_half_work, "irq_work", IMI_DEV_STK_SIZE, (void *)&irq_work_head, TSK_PRIO_INT_BOTTOM_HALF, &irq_work_handle);
}

CORE_INIT(irq_bootom_half_init);
