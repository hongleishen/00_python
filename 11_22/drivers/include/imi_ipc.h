#ifndef __IMI_IPC__
#define __IMI_IPC__
#include "linux_list.h"
#include "linux_compat.h"
#include <portable.h>
#include "event_groups.h"
#include "imi_config.h"

enum imi_ipc_module{
	IMI_IPC_MODULE_SENSOR = 0,
	IMI_IPC_MODULE_DPU,
	IMI_IPC_MODULE_CAMIF,
	IMI_IPC_MODULE_USB,
	IMI_IPC_MODULE_DSP,
	IMI_IPC_MODULE_MAX
};

enum imi_msg_id{
	IMI_MSG_ID_EVT = 0xFFFFFFF,
	IMI_MSG_ID_BUF = 0x8FFFFFFF,
};

struct imi_ipc_msg {
    unsigned int magic;
    unsigned int version;
    unsigned int seqnum;
    unsigned int id;

    unsigned int status;
    unsigned int timeL;
    unsigned int timeH;

    unsigned int buf0_addr;
    unsigned int buf0_size;
    unsigned int buf1_addr;
    unsigned int buf1_size;

    unsigned int param[5];
};

struct imi_rb_desc {
	short magic;
	short rd_idx;
	short wr_idx;
	short padding;
	int reserved[2];
};

struct imi_ring_buf {
	struct imi_rb_desc desc;
	struct imi_ipc_msg msg[IMI_IPC_MSG_NUM];
};

struct imi_ipc
{
	struct imi_ring_buf rb[IMI_IPC_MODULE_MAX];
};

extern EventGroupHandle_t imi_evt_grps;

/*
	evt = 1<<IMI_IPC_MODULE_ | 1 << IMI_IPC_MODULE_ |.....
*/
#define imi_ipc_send_event(evt) xEventGroupSetBits(imi_evt_grps, evt)
#define imi_ipc_send_event_irq(evt, w) xEventGroupSetBitsFromISR(imi_evt_grps, evt, w)
#define imi_ipc_wait_event(evt, clear, waitallevt, timeout) \
	xEventGroupWaitBits(imi_evt_grps, evt, clear, waitallevt, timeout)
#if 0
#define imi_ipc_wait_evt(evt, clear, waitallevt, timeout)	\
	xEventGroupWaitBits(imi_evt_grps, evt, clear, waitallevt, timeout)
#endif


int imi_ring_buf_init(struct imi_ring_buf *rb, short magic);
void imi_ring_buf_deinit(struct imi_ring_buf *rb);
void imi_ring_buf_reset(enum imi_ipc_module module);
int imi_ring_buf_count(enum imi_ipc_module module);
int imi_ring_buf_is_empty(enum imi_ipc_module module);
//overwrite mode do not need check buffer full
int imi_ring_buf_is_full(enum imi_ipc_module module);
int imi_ring_buf_write(enum imi_ipc_module module, struct imi_ipc_msg *data, int overwrite);
int imi_ring_buf_read(enum imi_ipc_module module, struct imi_ipc_msg *data, int overwrite);
void imi_ipc_init(void);
void imi_ipc_dump_msg(struct imi_ipc_msg *msg);
#endif
