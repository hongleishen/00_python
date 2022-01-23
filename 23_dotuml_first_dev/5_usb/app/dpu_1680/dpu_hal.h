#if defined(CHIP_IMI_1680)
#ifndef __DPU_HAL_H__
#define __DPU_HAL_H__
#include "FreeRTOS.h"
#include "uart.h"
#include "delay.h"
#include "task.h"
#include "queue.h"
#include "string.h"
#include "cpu.h"
#include "clock.h"
#include "imi_ipc.h"
#include "imi_bm.h"
#include "pinmux.h"
#include "vcsel.h"
#include "irrx.h"
#include "trigger.h"


struct task_array{
	TaskFunction_t tsk;
	char name[16];
	configSTACK_DEPTH_TYPE stk_sz;
	void *para;
	UBaseType_t prio;
	TaskHandle_t *handle;
};

#define DPU_BUFFER_MAX_NUM		(6)

enum dpu_op_cmd {
//control task command event
	DPU_CMD_INVALID = -1,
	DPU_CMD_OPEN_STREAM = 0,
	DPU_CMD_CLOSE_STREAM,
	DPU_CMD_UPDATE_DPU_PARA = 50,
	DPU_CMD_UPDATE_IRRX_PARA,
	DPU_CMD_UPDATE_VCSEL_FLOOD_PARA,
	DPU_CMD_UPDATE_VCSEL_DOT_PARA,
	DPU_CMD_UPDATE_TRIGGER_PARA,
//	irq event
	DPU_CMD_B_IRQ = 100,
	DPU_CMD_P_IRQ,
	DPU_CMD_R_IRQ,
	DPU_CMD_REF_DEPTH_IRQ,
	DPU_CMD_REF_SHIFT_IRQ,
	DPU_CMD_SYNC_IRQ,
//	DPU_CMD_HW_TIMER_IRQ,
// trigger event
	DPU_CMD_TRIGGER_B = 150,
	DPU_CMD_TRIGGER_P,
	DPU_CMD_TRIGGER_R,
	DPU_CMD_TRIGGER_BP,
	//color
	DPU_CMD_TRIGGER_C,
	DPU_CMD_TIMEOUT,
	DPU_CMD_MAX
};

enum sync_op_cmd {
	SYNC_CMD_ONE_SHOT_B = 0,
	SYNC_CMD_ONE_SHOT_P,
	SYNC_CMD_ONE_SHOT_R,
	SYNC_CMD_ONE_SHOT_C,
	SYNC_CMD_MAX
};

struct imi_dpu_msg {
    unsigned int magic;
    unsigned int version;
    unsigned int seqnum;
    unsigned int id;

    unsigned int status;
    unsigned int timeL;
    unsigned int timeH;

    unsigned int opcmd;
    unsigned int oppara0;
    unsigned int oppara1;
    unsigned int oppara2;
    unsigned int oppara3;
    unsigned int oppara4;
    unsigned int oppara5;
    unsigned int oppara6;
    unsigned int oppara7;
};


struct frm_out_path {
	int outpath_usb;
	int outpath_mipi;
	int outpath_dsp;
};

struct dpu_scenery_info {
	struct frm_out_path out;
	int frm_request;
	int frm_response;
};

struct dpu_control {
	struct list_head ir_buf_list;
	struct list_head dep_buf_list;
	FRTOS_mutex	dpu_mutex;
	
	/*para*/
	struct dpu_parameter dpu_para;
	struct irrx_config snr[IRRX_SETTING_GRP_MAX];	
	struct vcsel_setting dotlight[VCSEL_SETTING_GRP_MAX];
	struct vcsel_setting floodlight[VCSEL_SETTING_GRP_MAX];
	struct trigger_timing tt;
	/*handle*/	
	struct vcsel_handle *flood;
	struct vcsel_handle *dot;
	struct irrx_handle *irrx;
	struct trigger_handle *trigger; 
	struct dpu_handle *dpu;
	void *priv;
	
	struct dpu_dbg dbg_info;
	//	struct geometrical geo;
};

int dpu_parameter_parse(struct dpu_control *ctrl, struct dpu_parameter_cfg *cfg);
int dpu_hal_vcsel_para(struct vcsel_handle *handle, struct imi_dpu_msg *msg);
int dpu_hal_irrx_para(struct irrx_handle *irrx, struct imi_dpu_msg *msg);
int dpu_hal_buffer_dequeue(int path, void *priv, int this_usr, struct imi_buf_node ** buf);
int dpu_hal_buffer_dispatch(struct imi_buf_node *bn, int next_user, int next_module);
int dpu_op_cmd_parse(struct imi_ipc_msg *msg, struct dpu_control *ctrl);

//int dpu_depth_isr_cb(void * addr, int *sz, int idx, int ext);
//int dpu_ir_isr_cb(void * addr, int *sz, int idx, int ext);
#endif/*__DPU_HAL_H__*/
#endif/*CHIP_IMI_2280*/
