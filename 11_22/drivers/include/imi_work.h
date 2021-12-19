#ifndef __IMI_WORK__
#define __IMI_WORK__
#include "imi_config.h"
#include "linux_compat.h"
#include "errno.h"
#include "linux_list.h"

struct work_struct;
typedef void (*work_func_t)(struct work_struct *work);

struct work_struct {
	unsigned long data;
	struct list_head entry;
	work_func_t func;
	int state;
};

int schedule_work(struct work_struct *work);
void init_work(struct work_struct *work, void *func, void *param);

#endif /* __IMI_WORK__*/
