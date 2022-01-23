#if defined(CHIP_IMI_1680)

#if 0
struct list_head *imi_bm_handler = 0;
TaskHandle_t dputask_handle;

#if PARA_USING_FS_FILE
struct dpu_parameter_cfg dpc = {
	.path[DPU_PARA_TYPE_REF].name = "ref_img.txt",
	.path[DPU_PARA_TYPE_REF2].name = "ref_img2.txt",
	.path[DPU_PARA_TYPE_INIT].name = "init_seq.txt",
	.path[DPU_PARA_TYPE_SHIFT].name = "s2d.txt",
	.path[DPU_PARA_TYPE_DYRAM].name = "dyram.txt",
	.ini.name = "dpu_config.ini",
};
#else
struct dpu_parameter_cfg dpc;
#endif

static struct dpu_control *dbgdc = 0;
int depth_frame_dispatch(int target, void *buf, int buf_len)
{
#if x
	bm_buf_addusers(buffer->buf, IMI_BUFFER_USER_USB_IN);
	
	if (!imi_send_buffer(target, buffer->buf, sizeof(*buffer->buf), IMI_BUF_16BITPP)) {
		bm_buf_put(buffer->buf, IMI_BUFFER_USER_DEPTH);
	} else {
	}
#endif
	return 0;
}

int ir_frame_dispatch(void)
{
	return 0;
}

struct dpu_tick_item {
	int req;
	int rsp;
};

struct dpu_tick_indicator {
	int frm0;
	int frm1;
	int frm2;
};
int dpu_tick_generator(void)
{
	return 0;
}


#if x
int dpu_depth_open(struct dpu_handle *handle)
{
	dpu_power(handle, true);
	return 0;
}

int dpu_irrx_open(struct irrx_handle *handle)
{

}

int dpu_register_isr_event(void)
{
	return 0;
}
#endif


#if x
//dpu hal depth
int dpu_hal_depth_para(struct imi_dpu *idpu, struct imi_dpu_msg *msg)
{
	int ret = 0;
	return ret;
}

//dpu hal trigger
int dpu_hal_trigger_para(struct trigger_handle *ss, struct imi_dpu_msg *msg)
{
	int ret = 0;
	return ret;
}
#endif
#if x
int dpu_trigger_config(int mode, int a)
{
	//if mode == hw auto
	//if mode == hw sing
}
#endif
int dpu_depth_isr_cb(void * addr, int *sz, int idx, int ext)
{	
	LOGE("%s %d\n", __func__, __LINE__);
	imi_send_event_isr(IMI_IPC_MODULE_DPU, DPU_CMD_DEPTH_IRQ);
	return 0;
}

int dpu_ir_isr_cb(void * addr, int *sz, int idx, int ext)
{
	LOGE("%s %d\n", __func__, __LINE__);
	imi_send_event_isr(IMI_IPC_MODULE_DPU, DPU_CMD_IR_IRQ);
	return 0;
}

int dpu_hw_init(struct dpu_control *ctrl, struct dpu_buf_prop *dbp)
{
	int reg;
	int w;
	int h;
	int ret = 0;
	
	log_i("%s\n", __func__);
	ret |= dpu_parameter_parse(ctrl, &dpc);

	ret |= dpu_register_depth(&ctrl->dpu, &ctrl->dpu_para);
	ret |= dpu_register_irrx(&ctrl->irrx, &ctrl->snr[IRRX_SETTING_GRP_CACHED]);
	ret |= dpu_register_vcsel(&ctrl->dot, &ctrl->dotlight[VCSEL_SETTING_GRP_CACHED], VCSEL_TYPE_DOT);
//	ret |= dpu_register_vcsel(&ctrl->flood, &ctrl->floodlight[VCSEL_SETTING_GRP_CACHED], VCSEL_TYPE_FLOOD);
	ret |= dpu_register_trigger(&ctrl->trigger, &ctrl->tt);

/*irrx*/
	ret |= irrx_power(ctrl->irrx, true);
	ret |= irrx_get_id(ctrl->irrx, &reg);
	ret |= irrx_get_maxwin(ctrl->irrx, &w, &h);
	ret |= irrx_set_timing(ctrl->irrx, &ctrl->snr[IRRX_SETTING_GRP_CACHED].setting);
	log_i("ir sensor id %x , maxwin %dx%d\n", reg, w, h);
	ret |= irrx_init(ctrl->irrx, w, h);
	ret |= irrx_set_ag(ctrl->irrx, &ctrl->snr[IRRX_SETTING_GRP_CACHED].setting.analog_gain);
	ret |= irrx_set_dg(ctrl->irrx, &ctrl->snr[IRRX_SETTING_GRP_CACHED].setting.digital_gain);
	ret |= irrx_set_mirrorflip(ctrl->irrx, &ctrl->snr[IRRX_SETTING_GRP_CACHED].setting.mirror_flip);
	ret |= irrx_set_exp(ctrl->irrx, &ctrl->snr[IRRX_SETTING_GRP_CACHED].setting.exposure);
	ret |= irrx_set_hts(ctrl->irrx, &ctrl->snr[IRRX_SETTING_GRP_CACHED].setting.hts);
	ret |= irrx_set_vts(ctrl->irrx, &ctrl->snr[IRRX_SETTING_GRP_CACHED].setting.vts);
/*vcsel*/
	ret |= vcsel_power(ctrl->dot, true);
//	ret |= vcsel_power(ctrl->flood, true);
	ret |= vcsel_set_current(ctrl->dot, ctrl->dotlight[VCSEL_SETTING_GRP_CACHED].cur);
//	ret |= vcsel_set_current(ctrl->flood, ctrl->floodlight[VCSEL_SETTING_GRP_CACHED].cur);

/*trigger*/
	ret |= trigger_power(ctrl->trigger, true);
	ret |= trigger_init(ctrl->trigger, ctrl->trigger->timing);
	ret |= trigger_enable(ctrl->trigger, 1 << TRIG_CHAN_IR, true);
	ret |= trigger_enable(ctrl->trigger, 1 << TRIG_CHAN_FLOOD, true);
	ret |= trigger_enable(ctrl->trigger, 1 << TRIG_CHAN_DOT, true);
	
/*dpu*/
	ret |= dpu_power(ctrl->dpu, true);
	ret |= dpu_reset(ctrl->dpu);
	ret |= dpu_set_para(ctrl->dpu, DPU_PARA_CMD_LOAD_PARA, 0);
	ret |= dpu_register_isr_cb(ctrl->dpu, DPU_DMA_PATH_DEPTH, &dpu_depth_isr_cb);
	ret |= dpu_register_isr_cb(ctrl->dpu, DPU_DMA_PATH_IR, &dpu_ir_isr_cb);

	dbp->in_h = dbp->out_h = SYS_DEF_IR_HEIGHT;
	dbp->in_w = dbp->out_w = SYS_DEF_IR_WIDTH;
	dbp->rotate = SYS_DEF_BL_VERTICAL;
	ret |= dpu_set_para(ctrl->dpu, DPU_PARA_CMD_BUF_INIT, dbp);
	return ret;
}

#else
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


static struct dpu_control *dbgdc = 0;
struct list_head *imi_bm_handler = 0;

enum
{
	DPU_STAT_STOP = 0,
	DPU_STAT_IDLE,
	DPU_STAT_TRIG_B,
	DPU_STAT_TRIG_P,
	DPU_STAT_TRIG_R,
	DPU_STAT_TRIG_BP,
	DPU_STAT_B_DONE,
	DPU_STAT_P_DONE,
	DPU_STAT_BP_DONE,
	DPU_STAT_MAX
};

static int dpu_state = 0;
static const char dpu_str_stat[][8] = {
	"stop",
	"idle",
	"TrigB",
	"TrigP",
	"TrigR",
	"TrigBP",
	"BDone",
	"PDone",
	"BPDone",
	"INVAL"
};

GCC_CHEATING char* dpu_get_str_state(void)
{
	return (char*)&dpu_str_stat[dpu_state][0];
}

static char* get_dpu_cmd_str(int cmd)
{
	switch(cmd&0x7FFFFFFF) {
		case DPU_CMD_INVALID:
			return "inval";
		case DPU_CMD_B_IRQ:
			return "b_irq";
		case DPU_CMD_P_IRQ:
			return "p_irq";
		case DPU_CMD_R_IRQ:
			return "r_irq";
		case DPU_CMD_REF_DEPTH_IRQ:
			return "refdep";
		case DPU_CMD_REF_SHIFT_IRQ:
			return "refsft";
		case DPU_CMD_TRIGGER_B:
			return "trigb";
		case DPU_CMD_TRIGGER_P:
			return "trigp";
		case DPU_CMD_TRIGGER_R:
			return "trigr";
		case DPU_CMD_TRIGGER_BP:
			return "trigbp";
		case DPU_CMD_TIMEOUT:
			return "timout";
		default:
			return "";
	}
}

int dpu_hw_init(struct dpu_control *ctrl, struct dpu_buf_prop *dbp)
{
	int ret = 0;
	
	return ret;
}

/*ISR HANDLER*/
int dpu_depth_isr_cb(void * addr, int *sz, int idx, int ext)
{	
	LOGD("%s %d\n", __func__, __LINE__);
	imi_send_event_isr(IMI_IPC_MODULE_DPU, DPU_CMD_REF_DEPTH_IRQ, 0);
	return 0;
}

int dpu_ir_isr_cb(void * addr, int *sz, int idx, int ext)
{
	LOGD("%s %d\n", __func__, __LINE__);
	imi_send_event_isr(IMI_IPC_MODULE_DPU, DPU_CMD_R_IRQ, 0);
	return 0;
}

int dpu_bfrm_isr_cb(void * addr, int *sz, int idx, int ext)
{
	LOGD("%s %d\n", __func__, __LINE__);
	imi_send_event_isr(IMI_IPC_MODULE_DPU, DPU_CMD_B_IRQ, 0);
	return 0;
}

int dpu_pfrm_isr_cb(void * addr, int *sz, int idx, int ext)
{
	LOGD("%s %d\n", __func__, __LINE__);
	imi_send_event_isr(IMI_IPC_MODULE_DPU, DPU_CMD_P_IRQ, 0);
	return 0;
}

int dpu_register_isr(void)
{
	int ret = 0;
//	ret |= dpu_register_isr_cb(ctrl->dpu, DPU_DMA_PATH_DEPTH, &dpu_depth_isr_cb);
//	ret |= dpu_register_isr_cb(ctrl->dpu, DPU_DMA_PATH_IR, &dpu_ir_isr_cb);
	return ret;
}

int dpu_response_control(int evt_cmd, int *para)
{
	LOGD("enter");
	imi_send_event(IMI_IPC_MODULE_CONTROL, evt_cmd | MSG_ID_RSP_PREFIX, para);
	return 0;
}

int dpu_response_trig(int evt_cmd, int *para)
{
	LOGD("enter");
	imi_send_event(IMI_IPC_MODULE_SYNC, evt_cmd|MSG_ID_RSP_PREFIX, para);
	return 0;
}

int dpu_send_to_ipp(int type)
{
	LOGD("enter");
	imi_send_event(IMI_IPC_MODULE_IPP, type, 0);
	return 0;
}


TaskHandle_t dputask_handle;
GCC_CHEATING void dpu_set_state(int stat)
{
	dpu_state = stat;
	LOGV("new stat[%s]", dpu_get_str_state());
}

GCC_CHEATING int dpu_get_state(void)
{
	return dpu_state;
}

int dpu_idle_event_process(struct imi_ipc_msg *msg, struct dpu_control *ctrl)
{
	int ret = 0;
	int ack = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGV("%s: %s[%d]\n", (idm->opcmd & 0x80000000)?"rsp":"cmd",get_dpu_cmd_str(idm->opcmd),
			(idm->opcmd & 0x80000000)?(idm->opcmd & 0x7FFFFFFF):idm->opcmd);
	switch(idm->opcmd) {
/*process control msg*/		
		case DPU_CMD_OPEN_STREAM:
			LOGI("DPU_CMD_OPEN_STREAM\n");
			ret = 0;
			ack = idm->opcmd | MSG_ID_RSP_PREFIX;
			break;
		case DPU_CMD_CLOSE_STREAM:
			ret = 0;
			LOGI("DPU_CMD_CLOSE_STREAM\n");
			ack = idm->opcmd | MSG_ID_RSP_PREFIX;
			break;
		case DPU_CMD_UPDATE_DPU_PARA:
			//			ret = dpu_hal_depth_para(ctrl->dpu, idm);
			LOGI("DPU_CMD_UPDATE_DPU_PARA\n");
			ack = idm->opcmd | MSG_ID_RSP_PREFIX;
			break;
		case DPU_CMD_UPDATE_IRRX_PARA:
			LOGI("DPU_CMD_UPDATE_IRRX_PARA\n");
//			ret = dpu_hal_irrx_para(ctrl->irrx, idm);
			ack = idm->opcmd | MSG_ID_RSP_PREFIX;
			break;
		case DPU_CMD_UPDATE_VCSEL_FLOOD_PARA:
			LOGI("DPU_CMD_UPDATE_VCSEL_FLOOD_PARA\n");
//			ret = dpu_hal_vcsel_para(ctrl->flood, idm);
			ack = idm->opcmd | MSG_ID_RSP_PREFIX;
			break;
		case DPU_CMD_UPDATE_VCSEL_DOT_PARA:
			LOGI("DPU_CMD_UPDATE_VCSEL_DOT_PARA\n");
//			ret = dpu_hal_vcsel_para(ctrl->dot, idm);
			ack = idm->opcmd | MSG_ID_RSP_PREFIX;
			break;
		case DPU_CMD_UPDATE_TRIGGER_PARA:
			//suntongce
			//		ret = dpu_trigger_config(ctrl->ss, idm);
			LOGI("DPU_CMD_UPDATE_TRIGGER_PARA\n");
			ack = idm->opcmd | MSG_ID_RSP_PREFIX;
			break;
/*process trigger start*/		
		case DPU_CMD_TRIGGER_R:
			LOGI("DPU_CMD_TRIGGER_R\n");
			//1 todo
			dpu_set_state(DPU_STAT_TRIG_R);
			break;
		case DPU_CMD_TRIGGER_B:
			LOGI("DPU_CMD_TRIGGER_B\n");
			//1 todo
			dpu_set_state(DPU_STAT_TRIG_B);
			break;
		case DPU_CMD_TRIGGER_P:
			LOGI("DPU_CMD_TRIGGER_P\n");
			//1 todo
			dpu_set_state(DPU_STAT_TRIG_P);
			break;
		case DPU_CMD_SYNC_IRQ:
			LOGI("DPU_CMD_SYNC_IRQ\n");
			//???
			break;
/*process frame done event*/		
		case DPU_CMD_B_IRQ:
		case DPU_CMD_P_IRQ:
		case DPU_CMD_R_IRQ:
		case DPU_CMD_REF_DEPTH_IRQ:
		case DPU_CMD_REF_SHIFT_IRQ:
		case DPU_CMD_TIMEOUT:
			LOGE("cmd %d in stat %s invalid\n", idm->opcmd,
					dpu_get_str_state());
			//invalid command
			ret = -EINVAL;
			break;
		default:
			LOGE("%s cmd %d invalid\n", __func__, idm->opcmd);
			imi_ipc_dump_msg(msg);
			ret = -EINVAL;
			break;
	}
	if (ret) {
		LOGV("ret %x\n", ret);
	}
	if (ack) {
		dpu_response_trig(idm->opcmd, 0);
	}
	return ret;
}

int dpu_trig_b_event_process(struct imi_ipc_msg *msg,
		struct dpu_control *ctrl)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGV("cmd %s[%d]\n", get_dpu_cmd_str(idm->opcmd), idm->opcmd);
	switch(idm->opcmd) {
		case DPU_CMD_B_IRQ:
			ret = 0;
			dpu_set_state(DPU_STAT_B_DONE);
		break;
		case DPU_CMD_TIMEOUT:
			dpu_set_state(DPU_STAT_IDLE);
			ret = -ETIMEDOUT;
			break;
		default:
			LOGE("cmd %d in stat %s invalid\n", idm->opcmd,
					dpu_get_str_state());
			ret = -EINVAL;
			break;
	}
	if (ret) {
		LOGE("ret %x\n", ret);
	}
	return ret;
}

int dpu_trig_p_event_process(struct imi_ipc_msg *msg,
		struct dpu_control *ctrl)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGD("cmd %s[%d]\n", get_dpu_cmd_str(idm->opcmd), idm->opcmd);
	switch(idm->opcmd) {
		case DPU_CMD_REF_DEPTH_IRQ:
		case DPU_CMD_REF_SHIFT_IRQ:
			ret = 0;
			dpu_set_state(DPU_STAT_IDLE);
			break;
		case DPU_CMD_P_IRQ:
			ret = 0;
			dpu_set_state(DPU_STAT_P_DONE);
			break;
		case DPU_CMD_TIMEOUT:
			dpu_set_state(DPU_STAT_IDLE);
			ret = -ETIMEDOUT;
			break;
		default:
			LOGE("cmd %d in stat %s invalid\n", idm->opcmd,
					dpu_get_str_state());
			ret = -EINVAL;
			break;
	}
	if (ret) {
		LOGE("ret %x\n", ret);
	}
	return ret;
}

int dpu_trig_r_event_process(struct imi_ipc_msg *msg,
		struct dpu_control *ctrl)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGD("cmd %s[%d]\n", get_dpu_cmd_str(idm->opcmd), idm->opcmd);
	switch(idm->opcmd) {
		case DPU_CMD_R_IRQ:
			LOGV("DPU_CMD_R_IRQ\n");
			ret = 0;
			dpu_set_state(DPU_STAT_IDLE);
			dpu_response_trig(DPU_CMD_TRIGGER_R | MSG_ID_RSP_PREFIX, 0);
			break;
		case DPU_CMD_TIMEOUT:
			dpu_set_state(DPU_STAT_IDLE);
			ret = -ETIMEDOUT;
			break;
		default:
			LOGE("cmd %d in stat %s invalid\n", idm->opcmd,
					dpu_get_str_state());
			ret = -EINVAL;
			break;
	}
	if (ret) {
		LOGE("ret %x\n", ret);
	}
	return ret;
}

int dpu_trig_bp_event_process(struct imi_ipc_msg *msg,
		struct dpu_control *ctrl)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGD("cmd %s[%d]\n", get_dpu_cmd_str(idm->opcmd), idm->opcmd);
	switch(idm->opcmd) {
		case DPU_CMD_P_IRQ:
			LOGV("DPU_CMD_P_IRQ\n");
			ret = 0;
			dpu_set_state(DPU_STAT_BP_DONE);
			break;
		case DPU_CMD_REF_DEPTH_IRQ:
		case DPU_CMD_REF_SHIFT_IRQ:
			ret = 0;
			dpu_set_state(DPU_STAT_IDLE);
			break;
		case DPU_CMD_TIMEOUT:
			dpu_set_state(DPU_STAT_IDLE);
			ret = -ETIMEDOUT;
			break;
		default:
			LOGE("cmd %d in stat %s invalid\n", idm->opcmd,
					dpu_get_str_state());
			ret = -EINVAL;
			break;
	}
	if (ret) {
		LOGE("ret %x\n", ret);
	}
	return ret;
}

int dpu_bdone_event_process(struct imi_ipc_msg *msg,
		struct dpu_control *ctrl)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGD("cmd %s[%d]\n", get_dpu_cmd_str(idm->opcmd), idm->opcmd);
	switch(idm->opcmd) {
		case DPU_CMD_TRIGGER_BP:
			LOGV("DPU_CMD_TRIGGER_BP\n");
			ret = 0;
			dpu_set_state(DPU_STAT_TRIG_BP);
			dpu_response_trig(DPU_CMD_TRIGGER_B | MSG_ID_RSP_PREFIX, 0);
			break;
		case DPU_CMD_TIMEOUT:
			dpu_set_state(DPU_STAT_IDLE);
			ret = -ETIMEDOUT;
			break;
		default:
			LOGE("cmd %d in stat %s invalid\n", idm->opcmd,
					dpu_get_str_state());
			ret = -EINVAL;
			break;
	}
	if (ret) {
		LOGE("ret %x\n", ret);
	}
	return ret;
}


int dpu_pdone_event_process(struct imi_ipc_msg *msg,
		struct dpu_control *ctrl)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGD("cmd %s[%d]\n", get_dpu_cmd_str(idm->opcmd), idm->opcmd);
	switch(idm->opcmd) {
		case DPU_CMD_REF_DEPTH_IRQ:
		case DPU_CMD_REF_SHIFT_IRQ:
			LOGV("DPU_CMD_REF_?_IRQ\n");
			ret = 0;
			//start feed fpp
			dpu_set_state(DPU_STAT_IDLE);
			dpu_response_trig(DPU_CMD_TRIGGER_P | MSG_ID_RSP_PREFIX, 0);
			break;
		case DPU_CMD_TIMEOUT:
			dpu_set_state(DPU_STAT_IDLE);
			ret = -ETIMEDOUT;
			break;
		default:
			LOGE("cmd %d in stat %s invalid\n", idm->opcmd,
					dpu_get_str_state());
			ret = -EINVAL;
			break;
	}
	if (ret) {
		LOGE("ret %x\n", ret);
	}
	return ret;
}


int dpu_bpdone_event_process(struct imi_ipc_msg *msg,
		struct dpu_control *ctrl)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGD("cmd %s[%d]\n", get_dpu_cmd_str(idm->opcmd), idm->opcmd);
	switch(idm->opcmd) {
		case DPU_CMD_REF_DEPTH_IRQ:
		case DPU_CMD_REF_SHIFT_IRQ:
			LOGV("DPU_CMD_REF_?_IRQ\n");
			ret = 0;
			dpu_set_state(DPU_STAT_IDLE);
			dpu_response_trig(DPU_CMD_TRIGGER_BP | MSG_ID_RSP_PREFIX, 0);
			break;
		case DPU_CMD_TIMEOUT:
			dpu_set_state(DPU_STAT_IDLE);
			ret = -ETIMEDOUT;
			break;
		default:
			LOGE("cmd %d in stat %s invalid\n", idm->opcmd,
					dpu_get_str_state());
			ret = -EINVAL;
			break;
	}
	if (ret) {
		LOGE("ret %x\n", ret);
	}
	return ret;
}


void dputask(void *para)
{
	struct imi_ipc_msg msg;
	struct dpu_control *ctrl;
	struct imi_buf_node *bn = 0;
	struct dpu_buf_prop dbp;
	int ret;
	int i;
	EventBits_t bits;

	ctrl = pvPortMalloc(sizeof(*ctrl));
	memset(ctrl, 0, sizeof(*ctrl));
	INIT_LIST_HEAD(&ctrl->ir_buf_list);
	INIT_LIST_HEAD(&ctrl->dep_buf_list);
	FRTOS_mutex_create(&ctrl->dpu_mutex);

//	module_buffer_init(&ctrl->free_list, &ctrl->busy_list, DPU_BUFFER_MAX_NUM);
	memset(&msg, 0, sizeof(msg));

/* alloc buffer */
	memset(&dbp, 0, sizeof(struct dpu_buf_prop));
	for (i = 0; i < 2; i++) {
		ret = bm_buf_get(SYS_DEF_IR_WIDTH * SYS_DEF_IR_HEIGHT * 2, &bn);
		if (ret) {
			LOGE("buf dep get failed %d\n", ret);
			return;
		}
		list_add(&bn->usrlist, &ctrl->ir_buf_list);
		bm_buf_addusers(bn, IMI_BUFFER_USER_IR);
		dbp.buf_addr[DPU_DMA_PATH_IR][i] = bn->buf_addr;
	}
	for (i = 0; i < 2; i++) {
		ret = bm_buf_get(SYS_DEF_IR_WIDTH * SYS_DEF_IR_HEIGHT / 2, &bn);
		if (ret) {
			LOGE("buf ir get failed %d\n", ret);
			return;
		}
		list_add(&bn->usrlist, &ctrl->dep_buf_list);
		bm_buf_addusers(bn, IMI_BUFFER_USER_DEPTH);
		dbp.buf_addr[DPU_DMA_PATH_DEPTH][i] = bn->buf_addr;
	}
	dbgdc = ctrl;
	dpu_hw_init(ctrl, &dbp);

	dpu_set_state(DPU_STAT_IDLE);
	while (1) {
		bits = imi_ipc_wait_event(1 << IMI_IPC_MODULE_DPU, EVT_CLEAR, WAITALL_EVT, 0xFFFF);
		LOGD("%s %d evtbits %x, stat[%s]\n", __func__, __LINE__, bits, dpu_get_str_state());
		while (!imi_ring_buf_read(IMI_IPC_MODULE_DPU, &msg, 0)) {
			switch (dpu_get_state()) {
				case DPU_STAT_IDLE:
					LOGV("stat DPU_STAT_IDLE\n");
					switch(msg.id) {
						case IMI_MSG_ID_EVT:
							ret = dpu_idle_event_process(&msg, ctrl);
							break;
						default:
							LOGE("%s , msg id invalid %x\n", __func__, msg.id);
							break;
					}
					break;
				case DPU_STAT_TRIG_B:
					LOGV("stat DPU_STAT_TRIG_B\n");
					switch(msg.id) {
						case IMI_MSG_ID_EVT:
							ret = dpu_trig_b_event_process(&msg, ctrl);
							break;
						default:
							LOGE("%s , msg id invalid %x\n", __func__, msg.id);
							break;
					}
					break;
				case DPU_STAT_TRIG_P:
					LOGV("stat DPU_STAT_TRIG_B\n");
					switch(msg.id) {
						case IMI_MSG_ID_EVT:
							ret = dpu_trig_p_event_process(&msg, ctrl);
							break;
						default:
							LOGE("%s , msg id invalid %x\n", __func__, msg.id);
							break;
					}
					break;
				case DPU_STAT_TRIG_R:
					LOGV("stat DPU_STAT_TRIG_B\n");
					switch(msg.id) {
						case IMI_MSG_ID_EVT:
							ret = dpu_trig_r_event_process(&msg, ctrl);
							break;
						default:
							LOGE("%s , msg id invalid %x\n", __func__, msg.id);
							break;
					}
					break;
				case DPU_STAT_TRIG_BP:
					LOGV("stat DPU_STAT_TRIG_BP\n");
					switch(msg.id) {
						case IMI_MSG_ID_EVT:
							ret = dpu_trig_bp_event_process(&msg, ctrl);
							break;
						default:
							LOGE("%s , msg id invalid %x\n", __func__, msg.id);
							break;
					}
					break;
				case DPU_STAT_B_DONE:
					LOGV("stat DPU_STAT_B_DONE\n");
					switch(msg.id) {
						case IMI_MSG_ID_EVT:
							ret = dpu_bdone_event_process(&msg, ctrl);
							break;
						default:
							LOGE("%s , msg id invalid %x\n", __func__, msg.id);
							break;
					}
					break;
				case DPU_STAT_P_DONE:
					LOGV("stat DPU_STAT_P_DONE\n");
					switch(msg.id) {
						case IMI_MSG_ID_EVT:
							ret = dpu_pdone_event_process(&msg, ctrl);
							break;
						default:
							LOGE("%s , msg id invalid %x\n", __func__, msg.id);
							break;
					}
					break;
				case DPU_STAT_BP_DONE:
					LOGV("stat DPU_STAT_BP_DONE\n");
					switch(msg.id) {
						case IMI_MSG_ID_EVT:
							ret = dpu_bpdone_event_process(&msg, ctrl);
							break;
						default:
							LOGE("%s , msg id invalid %x\n", __func__, msg.id);
							break;
					}
					break;
				default:
					LOGE("stat %d ERROR\n", dpu_get_state());

			}
		}
	}
}

/*simulation*/
TaskHandle_t idle_handle;
#if IMI_1680_DPU_SIMS		
static int pusher = 0;
#endif		
void idletask(void *para)
{
	while(1) {
		vTaskDelay(1000);
#if IMI_1680_DPU_SIMS		
		switch (pusher) {
			case 0:
				imi_send_event(IMI_IPC_MODULE_SYNC, SYNC_CMD_ONE_SHOT_B, 0);
				break;
			case 1:
				LOGV("send DPU_CMD_B_IRQ to DPU");
				imi_send_event(IMI_IPC_MODULE_DPU, DPU_CMD_B_IRQ, 0);
				break;
			
			case 2:
				LOGV("send SYNC_CMD_ONE_SHOT_P to SYNC");
				imi_send_event(IMI_IPC_MODULE_SYNC, SYNC_CMD_ONE_SHOT_P, 0);
				break;
			case 3:
				LOGV("send DPU_CMD_P_IRQ to DPU");
				imi_send_event(IMI_IPC_MODULE_DPU, DPU_CMD_P_IRQ, 0);
				break;
			case 4:
				LOGV("send DPU_CMD_REF_DEPTH_IRQ to DPU");
				imi_send_event(IMI_IPC_MODULE_DPU, DPU_CMD_REF_DEPTH_IRQ, 0);
				break;			
			case 5:
				LOGV("send SYNC_CMD_ONE_SHOT_R to SYNC");
				imi_send_event(IMI_IPC_MODULE_SYNC, SYNC_CMD_ONE_SHOT_R, 0);
				break;
			case 6:
				LOGV("send DPU_CMD_R_IRQ to DPU");
				imi_send_event(IMI_IPC_MODULE_DPU, DPU_CMD_R_IRQ, 0);
				break;
			default:
				break;				
		}
		LOGI("%s %d pusher %d\n", __func__, __LINE__, pusher);
		pusher++;
		pusher %=7;
#endif		
	}
}

extern TaskHandle_t trigtask_handler;
extern void triggertask(void *para);
struct task_array task_init_table[] = {
   	{dputask, 	  	"dpu",	  	4096, 0,		4, &dputask_handle},
  	{triggertask,	"trig",	 	4096, 0,		2,	&trigtask_handler},
	{idletask,		"idle", 	4096, 0,		0, &idle_handle},
};

void imi_task_init(void)
{
	int i;
	struct task_array *ptsk;

	imi_ipc_init();
	imi_bm_handler = bm_bufman_init();

	//tsk init
	ptsk = &task_init_table[0];
	for (i = 0; i < sizeof(task_init_table)/sizeof(struct task_array); i++, ptsk++) {
		xTaskCreate(ptsk->tsk, ptsk->name, ptsk->stk_sz, ptsk->para,
				ptsk->prio, ptsk->handle);
	}
}
#endif
#endif/*CHIP_IMI_1680*/
