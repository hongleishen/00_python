#if defined(CHIP_IMI_1680)
#include "FreeRTOS.h"
//#include "uart.h"
#include "delay.h"
#include "task.h"
#include "queue.h"
//#include "uart_task.h"
//#include "string.h"
//#include "cpu.h"
//#include "clock.h"
#include "imi_ipc.h"
#include "imi_bm.h"
//#include "imi_sim.h"
#include "pinmux.h"
#include "vcsel.h"
#include "dpu.h"
#include "dpu_hal.h"
#include "trigger.h"
#include "dpu_table.h"
#include "msg_module.h"

#if PARA_USING_FS_FILE
int dpu_parameter_parse(struct dpu_control *ctrl, struct dpu_parameter_cfg *cfg)
{
	LOGV("start\n");
	ctrl->dpu_para.initseq.init_seq_context =;
	ctrl->dpu_para.initseq.init_seq_size =;

	ctrl->dpu_para.ref.ref_img = ;
	ctrl->dpu_para.ref.ref_img_size = ;

	ctrl->dpu_para.s2d.s2d = ;
	ctrl->dpu_para.s2d.s2d_size = ;

	ctrl->dpu_para.dy.dyram = ;
	ctrl->dpu_para.dy.dyram_sz = ;

	dpu_irrx_para_parse(&ctrl->snr[0], &cfg->ini);
	dpu_vcsel_para_parse(&ctrl->dotlight[0], &cfg->ini);
	dpu_vcsel_para_parse(&ctrl->floodlight[0], &cfg->ini);

	LOGV("end\n");
	return 0;
}
#elif PARA_USING_PRESET_TABLE
int dpu_parameter_parse(struct dpu_control *ctrl, struct dpu_parameter_cfg *cfg)
{
	LOGV("start\n");
#if 0	
	ctrl->dpu_para.initseq.init_seq_context =
		(struct dpu_depth_setting *)&dpu_init_seq[0];
	ctrl->dpu_para.initseq.init_seq_size =
		sizeof(dpu_init_seq) / sizeof(struct dpu_depth_setting);

	ctrl->dpu_para.ref[0].ref_img = (int *)&ref_img[0];
	ctrl->dpu_para.ref[0].ref_img_size = sizeof(ref_img);
	ctrl->dpu_para.ref[1].ref_img = (int *)&ref_img[0];
	ctrl->dpu_para.ref[1].ref_img_size = sizeof(ref_img);

	ctrl->dpu_para.s2d.s2d = (int *)&s2d[0];
	ctrl->dpu_para.s2d.s2d_size = sizeof(s2d)/sizeof(int);

	ctrl->dpu_para.dy.dyram = (int *)&dpu_ram_frame[0];
	ctrl->dpu_para.dy.dyram_sz = sizeof(dpu_ram_frame) / sizeof(int);

	//irrx para load
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].res.in_w = 1280;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].res.in_h = 960;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].res.out_w = 640;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].res.out_h = 480;

	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.analog_gain = 0x10;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.digital_gain = 0x400;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.exposure = 1500;
	//	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.fps = 30;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.rotate = SYS_DEF_BL_VERTICAL;
#if defined(PRJ_A300)
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.hts = 980;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.vts = 1000;
#elif defined(PRJ_S300)
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.hts = 728;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.vts = 984;
#endif
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.mipi_mbps = 800;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.mirror_flip = 1;
	ctrl->snr[IRRX_SETTING_GRP_DEPTH].setting.sysclk = 80;

	ctrl->snr[IRRX_SETTING_GRP_IR].res.in_w = 1280;
	ctrl->snr[IRRX_SETTING_GRP_IR].res.in_h = 960;
	ctrl->snr[IRRX_SETTING_GRP_IR].res.out_w = 1280;
	ctrl->snr[IRRX_SETTING_GRP_IR].res.out_h = 960;

	ctrl->snr[IRRX_SETTING_GRP_IR].setting.analog_gain = 0x30;
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.digital_gain = 0x400;
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.exposure = 4000;
	//	ctrl->snr[IRRX_SETTING_GRP_IR].setting.fps = 30;
#if defined(PRJ_A300)
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.hts = 980;
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.vts = 1000;
#elif defined(PRJ_S300)
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.hts = 728;
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.vts = 984;
#endif
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.mipi_mbps = 800;
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.mirror_flip = 1;
	ctrl->snr[IRRX_SETTING_GRP_IR].setting.sysclk = 80;

	memcpy(&ctrl->snr[IRRX_SETTING_GRP_CACHED],
			&ctrl->snr[IRRX_SETTING_GRP_DEPTH],
			sizeof(struct irrx_config));

	ctrl->dotlight[VCSEL_SETTING_GRP_SETVAL].cur =
		ctrl->dotlight[VCSEL_SETTING_GRP_CACHED].cur = 800;

	ctrl->floodlight[VCSEL_SETTING_GRP_SETVAL].cur =
		ctrl->floodlight[VCSEL_SETTING_GRP_CACHED].cur = 800;

	memset(&ctrl->dbg_info, 0, sizeof(struct dpu_dbg));

	ctrl->tt.path[TRIG_CHAN_DOT].T0.delay = 825;
	ctrl->tt.path[TRIG_CHAN_DOT].T0.pulse = 1500;
	ctrl->tt.path[TRIG_CHAN_FLOOD].T0.delay = 825;
	ctrl->tt.path[TRIG_CHAN_FLOOD].T0.pulse = 4000;
	ctrl->tt.path[TRIG_CHAN_IR].T0.delay = 0;
	ctrl->tt.path[TRIG_CHAN_IR].T0.pulse = 2000;
	ctrl->tt.path[TRIG_CHAN_IR].T1.delay = 0;
	ctrl->tt.path[TRIG_CHAN_IR].T1.pulse = 0;
	ctrl->tt.path[TRIG_CHAN_COLOR].T0.delay = 0;
	ctrl->tt.path[TRIG_CHAN_COLOR].T0.pulse = 2000;
	ctrl->tt.path[TRIG_CHAN_COLOR].T1.delay = 0;
	ctrl->tt.path[TRIG_CHAN_COLOR].T1.pulse = 0;
	ctrl->tt.trig_mode = TRIG_MODE_SINGLE;
	ctrl->tt.freq = 30;
#endif
	LOGV("end\n");
	return 0;
}
#endif

int dpu_hal_vcsel_para(struct vcsel_handle *handle, struct imi_dpu_msg *msg)
{
	int ret = 0;
	LOGV("cmd %x\n", msg->oppara0);
#if 0	
	switch(msg->oppara0) {
		case VCSEL_PARA_SET_POWER:
			ret = vcsel_power(handle, msg->oppara1);
			break;
		case VCSEL_PARA_RESET:
			ret = vcsel_reset(handle);
			break;
		case VCSEL_PARA_SET_REG:
			ret = vcsel_set_reg(handle, msg->oppara1, msg->oppara2);
			break;
		case VCSEL_PARA_GET_REG:
			ret = vcsel_get_reg(handle, msg->oppara1, (int *)&msg->oppara2);
			break;
		case VCSEL_PARA_SET_CURRENT:
			ret = vcsel_set_current(handle, msg->oppara1);
			break;
		case VCSEL_PARA_SET_CHANNEL:
			ret = vcsel_set_channel(handle, msg->oppara1);
			break;
		default:
			ret = -EINVAL;
	}
#endif
	LOGV("ret %x\n", ret);
	return ret;
}

int dpu_hal_irrx_para(struct irrx_handle *irrx, struct imi_dpu_msg *msg)
{
	int ret = 0;
	LOGV("cmd %x\n", msg->oppara0);
#if 0	
	switch(msg->oppara0) {
		case IRRX_PARA_SET_POWER:
			ret = irrx_power(irrx, (int)msg->oppara1);
			break;
		case IRRX_PARA_RESET:
			ret = irrx_reset(irrx);
			break;
		case IRRX_PARA_SET_REG:
			ret = irrx_set_reg(irrx, msg->oppara1, msg->oppara2);
			break;
		case IRRX_PARA_GET_REG:
			ret = irrx_get_reg(irrx, msg->oppara1, (int *)&msg->oppara2);
			break;
		case IRRX_PARA_SET_WINDOW:
			ret = irrx_set_window(irrx, msg->oppara1, msg->oppara2);
			break;
		case IRRX_PARA_SET_MIRROR_FLIP:
			ret = irrx_set_mirrorflip(irrx, &msg->oppara1);
			break;
		case IRRX_PARA_SET_ANALOG_GAIN:
			ret = irrx_set_ag(irrx, &msg->oppara1);
			break;
		case IRRX_PARA_SET_DIGITAL_GAIN:
			ret = irrx_set_dg(irrx, &msg->oppara1);
			break;
		case IRRX_PARA_SET_EXPOSURE:
			ret = irrx_set_exp(irrx, &msg->oppara1);
			break;
		case IRRX_PARA_SET_HTS:
			ret = irrx_set_hts(irrx, &msg->oppara1);
			break;
		case IRRX_PARA_SET_VTS:
			ret = irrx_set_vts(irrx, &msg->oppara1);
			break;
		case IRRX_PARA_GET_ID:
			ret = irrx_get_id(irrx, (int *)&msg->oppara1);
			break;
		case IRRX_PARA_GET_MAXWINDOW:
			ret = irrx_get_maxwin(irrx, (int *)&msg->oppara1,
					(int *)&msg->oppara2);
			break;
		case IRRX_PARA_SET_WORKMODE:
			ret = irrx_set_stream(irrx, msg->oppara1);
			break;
		case IRRX_PARA_SET_DBG:
			ret = irrx_set_dbg(irrx, msg);
			break;
		default:
			ret = -EINVAL;
	}
#endif
	LOGV("ret %x\n", ret);
	return ret;
}

int dpu_hal_buffer_dequeue(int path, void *priv, int this_usr,
		struct imi_buf_node ** buf)
{
	int ret = 0;
#if 0	
	int addr;
	int sz;
	struct imi_buf_node *bn = 0;
	struct dpu_control *ctrl = (struct dpu_control *)priv;
	int found = 0;
	struct list_head *head;
	if (path == DPU_DMA_PATH_DEPTH) {
		head = &ctrl->dep_buf_list;
	} else if (path == DPU_DMA_PATH_IR) {
		head = &ctrl->ir_buf_list;
	} else {
		LOGE("path %d invalid\n", path);
		return -EINVAL;
	}

	ret |= dpu_get_dma_buf(ctrl->dpu, path, &addr, &sz);
	if (!list_empty(head)) {
		list_for_each_entry(bn, head, usrlist) {
			if (bn->buf_addr == addr) {
				found = 1;
				break;
			}
		}
		if (!found) {
			LOGE("%x irq, dma addr %x not on list\n", path, addr);
			return -EFAULT;
		}
	}
	LOGV("path %d node %p phys %x %x\n", path, bn, bn->buf_addr, sz);

	FRTOS_mutex_lock(ctrl->dpu_mutex);
	list_del(&bn->usrlist);
	FRTOS_mutex_unlock(ctrl->dpu_mutex);
	if (path == DPU_DMA_PATH_DEPTH) {
		imi_bm_set_buf_type(bn, IMI_BM_BUF_TYPE_DEPTH);
	} else if (path == DPU_DMA_PATH_IR){
		imi_bm_set_buf_type(bn, IMI_BM_BUF_TYPE_IR);
	}
	*buf = bn;

	bn = 0;
	ret |= bm_buf_get(sz, &bn);
	FRTOS_mutex_lock(ctrl->dpu_mutex);
	list_add_tail(&bn->usrlist, head);
	FRTOS_mutex_unlock(ctrl->dpu_mutex);
	ret |= bm_buf_addusers(bn, this_usr);
	ret |= dpu_update_dma_buf(ctrl->dpu, bn->buf_addr, sz, path);
#endif

	return ret;
}

int dpu_hal_buffer_dispatch(struct imi_buf_node *bn, int next_user,
		int next_module)
{
	int ret = 0;
#if 0
	ret |= bm_buf_addusers(bn, next_user);
	if (ret) {
		LOGE("add usr %x failed\n", 1 << next_user);
		return -EFAULT;
	}
	ret |= imi_send_buffer(next_module, (void*)bn, sizeof(*bn), IMI_BUF_16BITPP);
	if (ret) {
		LOGE("send buffer %p to module %x failed\n", bn, next_module);
		return -ENOTSYNC;
	}
#endif
	return ret;
}

#if 0
int dpu_hal_buffer_ops(int path, void *priv, int this_user, int next_user, int next_module)
{
	int ret = 0;
	int addr;
	int sz;
	struct imi_buf_node *bn;
	struct dpu_control *ctrl = (struct dpu_handle *)priv;
	int found = 0;
	struct list_head *head;
	if (path == DPU_DMA_PATH_DEPTH) {
		head = &ctrl->dep_buf_list;
	} else if (path == DPU_DMA_PATH_IR) {
		head = &ctrl->ir_buf_list;
	} else {
		return -EINVAL;
	}
	//	imi_printf("%s %d path %d t_usr %d, n_usr %d, n_mod %d\n", __func__, __LINE__, path, this_user,
	//			next_user, next_module);
	ret |= dpu_get_dma_buf(ctrl->dpu, path, &addr, &sz);
	if (!list_empty(head)) {
		list_for_each_entry(bn, head, usrlist) {
			if (bn->buf_addr == addr) {
				found = 1;
				break;
			}
		}
		if (!found) {
			LOGE("%x irq, dma addr %x not on list\n", path, addr);
			return -EFAULT;
		}
		ret |= bm_buf_addusers(bn, next_user);
		imi_printf("path %d phys %x %x\n", path, bn->buf_addr, sz);
		if (!imi_send_buffer(next_module, (void*)bn,
					sizeof(*bn), IMI_BUF_16BITPP)) {
			bm_buf_put(bn, this_user);
		} else {
			LOGE("send buf %p to target %x failed\n", bn, next_user);
		}
		FRTOS_mutex_lock(ctrl->dpu_mutex);
		list_del(&bn->usrlist);
		bn = 0;
		FRTOS_mutex_unlock(ctrl->dpu_mutex);
	} else {
		LOGE("%x irq ,can not find buffer on list\n", path);
		return -EFAULT;
	}
	ret |= bm_buf_get(sz, &bn);
	FRTOS_mutex_lock(ctrl->dpu_mutex);
	list_add_tail(&bn->usrlist, head);
	FRTOS_mutex_unlock(ctrl->dpu_mutex);
	ret |= bm_buf_addusers(bn, this_user);
	ret |= dpu_update_dma_buf(ctrl->dpu, bn->buf_addr, sz, path);
	return ret;
}
#endif
#if 0
int dpu_op_cmd_parse(struct imi_ipc_msg *msg, struct dpu_control *ctrl)
{
	int ret = 0;
	struct imi_dpu_msg *idm = (struct imi_dpu_msg *)msg;
	LOGV("cmd %x\n", idm->opcmd);
	switch(idm->opcmd) {
		case DPU_CMD_OPEN_STREAM:
			LOGI("DPU_CMD_OPEN_STREAM\n");
			ret = 0;
			break;
		case DPU_CMD_CLOSE_STREAM:
			ret = 0;
			LOGI("DPU_CMD_CLOSE_STREAM\n");
			break;
		case DPU_CMD_UPDATE_DPU_PARA:
			//			ret = dpu_hal_depth_para(ctrl->dpu, idm);
			LOGI("DPU_CMD_UPDATE_DPU_PARA\n");
			break;
		case DPU_CMD_UPDATE_IRRX_PARA:
			LOGI("DPU_CMD_UPDATE_IRRX_PARA\n");
			ret = dpu_hal_irrx_para(ctrl->irrx, idm);
			break;
		case DPU_CMD_UPDATE_VCSEL_FLOOD_PARA:
			LOGI("DPU_CMD_UPDATE_VCSEL_FLOOD_PARA\n");
			ret = dpu_hal_vcsel_para(ctrl->flood, idm);
			break;
		case DPU_CMD_UPDATE_VCSEL_DOT_PARA:
			LOGI("DPU_CMD_UPDATE_VCSEL_DOT_PARA\n");
			ret = dpu_hal_vcsel_para(ctrl->dot, idm);
			break;
		case DPU_CMD_UPDATE_TRIGGER_PARA:
			//suntongce
			//		ret = dpu_trigger_config(ctrl->ss, idm);
			LOGE("DPU_CMD_UPDATE_TRIGGER_PARA\n");
			break;
		case DPU_CMD_B_IRQ:
			LOGE("DPU_CMD_B_IRQ\n");
			break;
		case DPU_CMD_P_IRQ:
			LOGE("DPU_CMD_P_IRQ\n");
			break;
		case DPU_CMD_R_IRQ:
			LOGE("DPU_CMD_R_IRQ\n");
			break;
		case DPU_CMD_REF_DEPTH_IRQ:
			LOGE("DPU_CMD_REF_DEPTH_IRQ\n");
			break;
		case DPU_CMD_REF_SHIFT_IRQ:
			LOGE("DPU_CMD_REF_SHIFT_IRQ\n");
			break;
		case DPU_CMD_SYNC_IRQ:
			LOGE("DPU_CMD_SYNC_IRQ\n");
			break;
		case DPU_CMD_TRIGGER_R:
			LOGE("DPU_CMD_TRIGGER_R\n");
			break;
		case DPU_CMD_TRIGGER_B:
			LOGE("DPU_CMD_TRIGGER_B\n");
			break;
		case DPU_CMD_TRIGGER_P:
			LOGE("DPU_CMD_TRIGGER_P\n");
			break;
		
		
#if 0		
		case DPU_CMD_DEPTH_IRQ:
			ret = dpu_hal_buffer_dequeue(DPU_DMA_PATH_DEPTH, (void*)ctrl,
						IMI_BUFFER_USER_DEPTH, &bn);
			if (!ret) {
				bm_buf_put(bn, IMI_BUFFER_USER_DEPTH);
			} else {
				break;
			}
			ret |= dpu_hal_buffer_dispatch(bn, IMI_BUFFER_USER_USB_IN,
						IMI_IPC_MODULE_USB);
			break;
		case DPU_CMD_IR_IRQ:
			ret = dpu_hal_buffer_dequeue(DPU_DMA_PATH_IR, (void*)ctrl,
						IMI_BUFFER_USER_IR, &bn);
			if (!ret) {
				bm_buf_put(bn, IMI_BUFFER_USER_IR);
			} else {
				break;
			}
			ret |= dpu_hal_buffer_dispatch(bn, IMI_BUFFER_USER_USB_IN,
						IMI_IPC_MODULE_USB);
			break;
		case DPU_CMD_GEO_DONE:
			ret = 0;
			LOGE("DPU_CMD_GEO_DONE\n");
			break;
		case DPU_CMD_BUFFER_IN:
			ret = 0;
			LOGE("DPU_CMD_BUFFER_IN\n");
			break;
#else
#endif
		default:
			LOGE("%s cmd %d invalid\n", __func__, idm->opcmd);
			imi_ipc_dump_msg(msg);
			ret = -EINVAL;
			break;
	}
	LOGV("ret %x\n", ret);
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
#endif/*CHIP_IMI_2280*/
