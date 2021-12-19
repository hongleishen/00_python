#if defined(CHIP_IMI_2280)
#include "FreeRTOS.h"
#include "uart.h"
#include "delay.h"
#include "task.h"
#include "queue.h"
//#include "string.h"
//#include "cpu.h"
//#include "clock.h"
#include "imi_ipc.h"
#include "imi_bm.h"
#include "msg_module.h"
#include "linux_common.h"
static DEFINE_MUTEX(msg_module_lock);

int imi_send_event(enum imi_ipc_module target, int evt_id)
{
	struct imi_ipc_msg msg;
	static int seq = 0;
	int ret = 0;
	memset(&msg, 0, sizeof(msg));

	msg.id = IMI_MSG_ID_EVT;
	msg.magic = 0x4321;
	msg.seqnum = seq++;
	msg.buf0_addr = evt_id;

	spin_lock(lock);
	ret = imi_ring_buf_write(target, &msg, 0);
	if (ret) {
		configASSERT(0);
		log_e("ringbuffer write %d failed %x\n", evt_id, ret);
		spin_unlock(lock);
		return ret;
	}
	//sending out event;
	imi_ipc_send_event(1 << target);
	spin_unlock(lock);
	return ret;
}

int imi_send_event_isr(enum imi_ipc_module target, int evt_id)
{
	struct imi_ipc_msg msg;
	static int seq = 0;
	int ret = 0;
	BaseType_t need_resch = pdFALSE;
	BaseType_t sent;
	memset(&msg, 0, sizeof(msg));

	msg.id = IMI_MSG_ID_EVT;
	msg.magic = 0x4321;
	msg.seqnum = seq++;
	msg.buf0_addr = evt_id;

	//	spin_lock(lock);
	ret = imi_ring_buf_write(target, &msg, 0);
	if (ret) {
		configASSERT(0);
		log_e("ringbuffer write %d failed %x\n", evt_id, ret);
		spin_unlock(lock);
		return ret;
	}
#if 1
	//sending out event;
	//suntongce should be isr routine
	sent = imi_ipc_send_event_irq(1 << target, &need_resch);
	log_v("send %d, need %d, evt %d\n", sent, need_resch, evt_id);
	if (sent == pdPASS) {
		if (need_resch == pdTRUE) {
			portEND_SWITCHING_ISR(need_resch);
		}
	}
#else
#endif
	//	spin_unlock(lock);
	return ret;
}

//int imi_send_buffer(enum imi_ipc_module target, struct imi_ipc_msg *msg)
int imi_send_buffer(enum imi_ipc_module target, void* buf, int len, int type)
{
	struct imi_ipc_msg msg;
	static int seq = 0;
	int ret = 0;
	memset(&msg, 0, sizeof(msg));

	msg.id = IMI_MSG_ID_BUF;
	msg.magic = 0x1234;
	msg.seqnum = seq++;
	msg.buf0_addr = (int)buf;
	msg.buf0_size = len;
	msg.buf1_addr = type;		//notify buffer type , is an IR frame type

	spin_lock(lock);
	ret = imi_ring_buf_write(target, &msg, 0);
	if (ret) {
		configASSERT(0);
		log_e("ringbuffer write %d failed %x\n", IMI_IPC_MODULE_DPU, ret);
		spin_unlock(lock);
		return ret;
	}
	//sending out event;
	imi_ipc_send_event(1 << target);
	spin_unlock(lock);
	return ret;
}

int module_buffer_init(struct list_head *free, struct list_head *busy, int cnt)
{
	struct module_buffer *buffer;
	int i;
	INIT_LIST_HEAD(free);
	INIT_LIST_HEAD(busy);
	for (i = 0; i < cnt; i++){
		buffer = pvPortMalloc(sizeof(*buffer));
		memset(buffer, 0, sizeof(*buffer));
		INIT_LIST_HEAD(&buffer->list);
		list_add_tail(&buffer->list, free);
	}
	return 0;
}
int module_push_buffer(struct list_head *free, struct list_head *busy,
			struct imi_buf_node* node)
{
	struct module_buffer *mbuffer;
	struct module_buffer *t;
	int already_in = 0;
	if (list_empty(free)) {
		configASSERT(0);
		log_e("usb_free_list empty\n");
		return -ENOBUFS;
	}
	mbuffer = list_first_entry(free, struct module_buffer, list);
	mbuffer->buf = node;
	// incoming buffer enqueue
	mutex_lock(&msg_module_lock);
	if (!list_empty(busy)) {
		list_for_each_entry(t, busy, list) {
			if (t->buf->buf_addr == mbuffer->buf->buf_addr) {
				already_in = 1;
				return 0;
			}
		}
	}
	if (!already_in) {
		log_v("incoming new buffer node %p addr:%x\n", mbuffer->buf,
				mbuffer->buf->buf_addr);
		list_move_tail(&mbuffer->list, busy);
	}
	already_in = 0;
	mutex_unlock(&msg_module_lock);
	return 0;
}

int module_pop_buffer(struct list_head *free, struct list_head *busy,
		enum imi_bm_user user)
{
	struct module_buffer *buffer;
	struct module_buffer *t;
	mutex_lock(&msg_module_lock);
	if (!list_empty(busy)) {
		list_for_each_entry_safe(buffer, t, busy, list) {
			log_v("node %p\n", buffer->buf);
			if (!bm_buf_put(buffer->buf, user)) {
				log_v("buffer %x eliminated\n", buffer->buf);
			}
			list_move_tail(&buffer->list, free);
		}
	}
	mutex_unlock(&msg_module_lock);
	return 0;
}

void msg_module_init(void)
{
	mutex_init(&msg_module_lock);
}
SUBSYS_INIT(msg_module_init);
#endif/*CHIP_IMI_2280*/
