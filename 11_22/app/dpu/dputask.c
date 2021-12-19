#if defined(CHIP_IMI_2280)
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
	imi_printf("%s %d\n", __func__, __LINE__);
	imi_send_event_isr(IMI_IPC_MODULE_DPU, DPU_CMD_DEPTH_IRQ);
	return 0;
}

int dpu_ir_isr_cb(void * addr, int *sz, int idx, int ext)
{
	imi_printf("%s %d\n", __func__, __LINE__);
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

int dpu_start_sw_trigger(void)
{
	return 0;
}

void dputask(void *para)
{
	struct imi_ipc_msg msg;
	struct dpu_control *ctrl;
	struct imi_buf_node *bn = 0;
	struct dpu_buf_prop dbp;
	int ret;
	int i;

	ctrl = pvPortMalloc(sizeof(*ctrl));
	memset(ctrl, 0, sizeof(*ctrl));
	INIT_LIST_HEAD(&ctrl->ir_buf_list);
	INIT_LIST_HEAD(&ctrl->dep_buf_list);
	FRTOS_mutex_create(&ctrl->dpu_mutex);

//	module_buffer_init(&ctrl->free_list, &ctrl->busy_list, DPU_BUFFER_MAX_NUM);
	memset(&msg, 0, sizeof(msg));

	memset(&dbp, 0, sizeof(struct dpu_buf_prop));
	for (i = 0; i < 2; i++) {
		ret = bm_buf_get(SYS_DEF_IR_WIDTH * SYS_DEF_IR_HEIGHT * 2, &bn);
		if (ret) {
			log_e("buf dep get failed %d\n", ret);
			return;
		}
		list_add(&bn->usrlist, &ctrl->ir_buf_list);
		bm_buf_addusers(bn, IMI_BUFFER_USER_IR);
		dbp.buf_addr[DPU_DMA_PATH_IR][i] = bn->buf_addr;
	}
	for (i = 0; i < 2; i++) {
		ret = bm_buf_get(SYS_DEF_IR_WIDTH * SYS_DEF_IR_HEIGHT / 2, &bn);
		if (ret) {
			log_e("buf ir get failed %d\n", ret);
			return;
		}
		list_add(&bn->usrlist, &ctrl->dep_buf_list);
		bm_buf_addusers(bn, IMI_BUFFER_USER_DEPTH);
		dbp.buf_addr[DPU_DMA_PATH_DEPTH][i] = bn->buf_addr;
	}
	dbgdc = ctrl;
	dpu_hw_init(ctrl, &dbp);

	dpu_start_sw_trigger();
	while (1) {
		imi_ipc_wait_event(1 << IMI_IPC_MODULE_DPU, 1, 0, 0xFFFF);
		while (!imi_ring_buf_read(IMI_IPC_MODULE_DPU, &msg, 0)) {

			//msg parse
			switch(msg.id) {
				case IMI_MSG_ID_EVT:
					ret = dpu_op_cmd_parse(&msg, ctrl);
					break;
				case IMI_MSG_ID_BUF:
//					module_push_buffer(&ctrl->free_list, &ctrl->busy_list,
//							(struct imi_buf_node *)msg.buf0_addr);
					break;
				default:
					imi_printf("%s , msg id invalid %x\n", __func__, msg.id);
					break;
			}
		}
		imi_printf("%s %d\n", __func__, __LINE__);
	}
}


TaskHandle_t usb_handle;
struct list_head usb_free_list;
struct list_head usb_busy_list;
#define USB_BUFFER_MAX_NUM		(6)
void usbtask(void *para)
{
//	struct module_buffer *buffer;
//	struct module_buffer *t;
	struct imi_ipc_msg msg;

	module_buffer_init(&usb_free_list, &usb_busy_list, USB_BUFFER_MAX_NUM);
	memset(&msg, 0, sizeof(msg));

	while (1) {
		imi_ipc_wait_event(1 << IMI_IPC_MODULE_USB, 1, 0, 0xFFFF);
		while (!imi_ring_buf_read(IMI_IPC_MODULE_USB, &msg, 0)) {

			//msg parse
			switch(msg.id) {
				case IMI_MSG_ID_EVT:
					switch(msg.buf0_addr) {
						case 1:
						default:
					#if 0	
						mutex_lock(lock);
						//hw irq incoming , send hw processed buffer to next stage & local list dequeue
						if (!list_empty(&usb_busy_list)) {
							list_for_each_entry_safe(buffer, t, &usb_busy_list, list){
								buffer->buf->buf_len = 0x1000;
								bm_buf_addusers(buffer->buf, IMI_BUFFER_USER_USB_IN);

								if (!imi_send_buffer(IMI_IPC_MODULE_USB, buffer->buf, sizeof(*buffer->buf), IMI_BUF_16BITPP)) {
									bm_buf_put(buffer->buf, IMI_BUFFER_USER_VPU);
								} else {
									bm_buf_put(buffer->buf, IMI_BUFFER_USER_USB_IN);
								}
								list_move_tail(&buffer->list, &usb_free_list);
							}
						}
						mutex_unlock(lock);
					#endif	
					imi_printf("%s %d\n", __func__, __LINE__);
						break;
					}
					break;
				case IMI_MSG_ID_BUF:
//					module_push_buffer(&usb_free_list, &usb_busy_list, (struct imi_buf_node *)msg.buf0_addr);
					imi_printf("%s %d\n", __func__, __LINE__);
					bm_buf_put((struct imi_buf_node *)msg.buf0_addr, IMI_BUFFER_USER_USB_IN);
					break;
				default:
					imi_printf("%s , msg id invalid %x\n", __func__, msg.id);
					break;
			}
		}
	}
}


#if 0
TaskHandle_t camif_handle;
struct list_head camif_free_list;
struct list_head camif_busy_list;
#define CAMIF_BUFFER_MAX_NUM		(6)
void camiftask(void *para)
{
	struct module_buffer *buffer;
	struct module_buffer *t;
	struct imi_ipc_msg msg;
	int ret;

	module_buffer_init(&camif_free_list, &camif_busy_list, CAMIF_BUFFER_MAX_NUM);
	memset(&msg, 0, sizeof(msg));

	while (1) {
		imi_ipc_wait_event(1 << IMI_IPC_MODULE_CAMIF, 1, 0, 0xFFFF);
		while (!imi_ring_buf_read(IMI_IPC_MODULE_CAMIF, &msg, 0)) {

			//msg parse
			switch(msg.id) {
				case IMI_MSG_ID_EVT:
					switch(msg.buf0_addr) {
						case 1:
						default:
						mutex_lock(lock);
						//hw irq incoming , send hw processed buffer to next stage & local list dequeue
						if (!list_empty(&camif_busy_list)) {
							list_for_each_entry_safe(buffer, t, &camif_busy_list, list){
								buffer->buf->buf_len = 0x1000;
								bm_buf_addusers(buffer->buf, IMI_BUFFER_USER_USB_IN);

								if (!imi_send_buffer(IMI_IPC_MODULE_USB, buffer->buf, sizeof(*buffer->buf), IMI_BUF_16BITPP)) {
									bm_buf_put(buffer->buf, IMI_BUFFER_USER_VPU);
								} else {
									bm_buf_put(buffer->buf, IMI_BUFFER_USER_USB_IN);
								}
								list_move_tail(&buffer->list, &camif_free_list);
							}
						}
						mutex_unlock(lock);
						break;
					}
					break;
				case IMI_MSG_ID_BUF:
					module_push_buffer(&camif_free_list, &camif_busy_list, (struct imi_buf_node *)msg.buf0_addr);
					break;
				default:
					imi_printf("%s , msg id invalid %x\n", __func__, msg.id);
					break;
			}
		}
		imi_printf("%s %d\n", __func__, __LINE__);
	}
}

#endif
TaskHandle_t idle_handle;
void idletask(void *para)
{
	static int once=0;
	while(1) {
		vTaskDelay(1000);
		//suntongce 
		if (!once) {
		dpu_dma_enable(dbgdc->dpu, DPU_DMA_PATH_IR, 1);
		dpu_dma_enable(dbgdc->dpu, DPU_DMA_PATH_DEPTH, 1);
		once = 1;
		}
		trigger_valid_sync(dbgdc->trigger, 0, 1);
		imi_printf("%s %d mipi %x\n", __func__, __LINE__, read32(0x32a00178));
	//	imi_send_buffer(1 << IMI_IPC_MODULE_USB, 0, 0, 0);
	}
}

struct task_array task_init_table[] = {
	{idletask,		 "idle",	  4096, 0,		  0, &idle_handle},
   	{dputask, 	  	"dpu",	  4096, 0,		  4, &dputask_handle},
  	{usbtask,		"usb",	4096, 0,		2,	&usb_handle},
//	{camiftask,		"camif",	4096, 0,		2,	&camif_handle},
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
	imi_printf("%s %d\n", __func__, __LINE__);
}
#endif/*CHIP_IMI_2280*/
