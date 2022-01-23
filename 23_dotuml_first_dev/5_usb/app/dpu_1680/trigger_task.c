#if defined(CHIP_IMI_1680)
#include "FreeRTOS.h"
#include "uart.h"
#include "delay.h"
#include "task.h"
#include "queue.h"
#include "uart_task.h"
#include "string.h"
#include "cpu.h"
#include "clock.h"
#include "imi_ipc.h"
#include "imi_bm.h"
//#include "imi_sim.h"
#include "pinmux.h"
#include "vcsel.h"
#include "dpu.h"
#include "dpu_hal.h"
#include "trigger.h"
#include "msg_module.h"

#define EVT_CLEAR		(1)
#define WAITALL_EVT		(0)
#define MAX_TIMEOUT		(0xFFFF)

#define TRIG_CHAN_MAX		(4)
char trig_seq[TRIG_CHAN_MAX] = {0};

/*seq :
		B/P/R/NULL*/
int trig_chan_init(int seq, char *tab)
{
	int i;
	for (i = 0; i < TRIG_CHAN_MAX; i++) {
		switch (seq >> (i * 8) & 0xff)
		{
			case 'B':
				tab[i] = 'B';
			break;
			case 'P':
				tab[i] = 'P';
			break;
			case 'R':
				tab[i] = 'R';
			break;
			default:
				tab[i] = 0;
			break;
		}
	}
	return 0;
}

int trig_next(char *table, char *cur, char *next)
{
	int i;
	if (*cur == 0) {
		*next = *table;
		return 0;
	}
	for (i = 0; i <TRIG_CHAN_MAX; i++) {
		if (*cur == *(table + i)) {
			*next = *(table + i + 1);
			return 0;
		}
	}
	*next = *table;
	return 0;
}

/*
*	only following pattern supported:
*	R
*	P
*	B|P
*	P|R
*	B|P|R
*/
enum {
	SYNC_PATTERN_R,
	SYNC_PATTERN_P,
	SYNC_PATTERN_BP,
	SYNC_PATTERN_PR,
	SYNC_PATTERN_BPR,
	SYNC_PATTERN_MAX,
};

static int trigger_pattern = SYNC_PATTERN_MAX;
static void set_trigger_pattern(int pat)
{
	trigger_pattern = pat;
}

int get_trigger_pattern(void)
{
	return trigger_pattern;
}

int trigger_cmd_parse(struct imi_ipc_msg *msg, int *evt)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGD("%s: %d\n", (idm->opcmd & 0x80000000)?"rsp":"cmd",
			(idm->opcmd & 0x80000000)?(idm->opcmd & 0x7FFFFFFF):idm->opcmd);
	switch(idm->opcmd) {
		case SYNC_CMD_ONE_SHOT_B:
			LOGV("SYNC_CMD_ONE_SHOT_B\n");
			switch(get_trigger_pattern()) {
				case SYNC_PATTERN_BP:
				case SYNC_PATTERN_BPR:
					*evt = DPU_CMD_TRIGGER_B;
					break;
				default:
					ret = -EINVAL;
					*evt = DPU_CMD_INVALID;
					break;
			}
			break;
		case SYNC_CMD_ONE_SHOT_P:
			LOGV("SYNC_CMD_ONE_SHOT_P\n");
			switch(get_trigger_pattern()) {
				case SYNC_PATTERN_P:
				case SYNC_PATTERN_PR:
					*evt = DPU_CMD_TRIGGER_P;
					break;
				case SYNC_PATTERN_BP:
				case SYNC_PATTERN_BPR:
					*evt = DPU_CMD_TRIGGER_BP;
					break;
				default:
					ret = -EINVAL;
					*evt = DPU_CMD_INVALID;
					break;
			}
			break;
		case SYNC_CMD_ONE_SHOT_R:
			LOGV("SYNC_CMD_ONE_SHOT_R\n");
			switch(get_trigger_pattern()) {
				case SYNC_PATTERN_R:
				case SYNC_PATTERN_PR:
				case SYNC_PATTERN_BPR:
					*evt = DPU_CMD_TRIGGER_R;
					break;
				default:
					ret = -EINVAL;
					*evt = DPU_CMD_INVALID;
					break;
			}
			break;
		case SYNC_CMD_ONE_SHOT_C:
			LOGV("SYNC_CMD_ONE_SHOT_C\n");
			ret = 0;
//			*evt = SYNC_CMD_ONE_SHOT_C;
			break;
		case DPU_CMD_TRIGGER_B | MSG_ID_RSP_PREFIX:
			LOGV("rsp of SYNC_CMD_ONE_SHOT_B");
//			trig_next(&trig_seq, cur, next);
			break;
		case DPU_CMD_TRIGGER_P | MSG_ID_RSP_PREFIX:
			LOGV("rsp of DPU_CMD_TRIGGER_P");
//			trig_next(&trig_seq, cur, next);
			break;
		case DPU_CMD_TRIGGER_R | MSG_ID_RSP_PREFIX:
			LOGV("rsp of DPU_CMD_TRIGGER_R");
//			trig_next(&trig_seq, cur, next);
			break;
		case DPU_CMD_TRIGGER_BP | MSG_ID_RSP_PREFIX:
			LOGV("rsp of DPU_CMD_TRIGGER_BP");
//			trig_next(&trig_seq, cur, next);
			break;
		default:
			LOGE("%s cmd %d invalid\n", __func__, idm->opcmd);
			imi_ipc_dump_msg(msg);
			ret = -EINVAL;
			*evt = -DPU_CMD_INVALID;
			break;
	}
	if (ret) {
		LOGI("ret %x\n", ret);
	}
	return ret;
}

TaskHandle_t trigtask_handler;
void triggertask(void *para)
{
	struct imi_ipc_msg msg;
	int ret = 0;
	int evt = -1;
	EventBits_t bits;
	
	LOGI("enter");
	memset(&msg, 0, sizeof(msg));
	set_trigger_pattern(SYNC_PATTERN_BPR);
	while(1) {
		bits = imi_ipc_wait_event(1 << IMI_IPC_MODULE_SYNC,
							EVT_CLEAR, WAITALL_EVT, 0xFFFF);
		LOGD("waitevt evtbits %x", bits);
		while (!imi_ring_buf_read(IMI_IPC_MODULE_SYNC, &msg, 0)) {

			//msg parse
			switch(msg.id) {
				case IMI_MSG_ID_EVT:
					evt = DPU_CMD_INVALID;
					ret |= trigger_cmd_parse(&msg, &evt);
					if (ret) {
						LOGE("retval %d\n", ret);
					}
					break;
				default:
					LOGE("%s , msg id invalid %x\n", __func__, msg.id);
					break;
			}
		}
		if ((evt > DPU_CMD_INVALID) && (evt < DPU_CMD_MAX)) {
			LOGD("sending event %d to %s\n", evt, ipc_get_module_str(IMI_IPC_MODULE_DPU));
			imi_send_event(IMI_IPC_MODULE_DPU, evt, 0);
		}
	}
}
#endif/*CHIP_IMI_1680*/
