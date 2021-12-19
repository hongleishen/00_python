#ifndef __MSG_MODULE_H__
#define __MSG_MODULE_H__
#include "linux_common.h"
#include "task.h"
#include "queue.h"
//#include "common.h"
//#include "string.h"
//#include "cpu.h"
//#include "clock.h"
#include "imi_ipc.h"
#include "imi_bm.h"

struct module_buffer
{
	struct list_head list;
	struct imi_buf_node *buf;
	//FRTOS_MUTEX;
};

int imi_send_event(enum imi_ipc_module target, int evt_id);
int imi_send_event_isr(enum imi_ipc_module target, int evt_id);
int imi_send_buffer(enum imi_ipc_module target, void* buf, int len, int type);
int module_buffer_init(struct list_head *free, struct list_head *busy, int cnt);
int module_push_buffer(struct list_head *free, struct list_head *busy,
		struct imi_buf_node* node);
int module_pop_buffer(struct list_head *free, struct list_head *busy,
		enum imi_bm_user user);

#endif/*__MSG_MODULE_H__*/
