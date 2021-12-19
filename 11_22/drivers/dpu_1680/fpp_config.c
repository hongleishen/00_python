#include "fpp.h"
#include "gpio.h"
#include "elog.h"
#include "sensor_def.h"

extern uint32_t ulPortInterruptNesting;
extern uint32_t ulPortYieldRequired;

void fpp_write_reg(u32 val, u32 reg)
{
	writel(val, FPP_BASE_ADDR + reg);
}

static u32 fpp_read_reg(u32 reg)
{
	return readl(FPP_BASE_ADDR + reg);
}

void read_fpp_start_bit(void)
{
	unsigned int ret;

	ret = fpp_read_reg(FPP_CTRL);
	
	if (ret & 0x800) {
		ret &= (~FPP_RD_START);
	} else {
		ret |= FPP_RD_START;
	}
	
	fpp_write_reg(ret, FPP_CTRL);
	log_d(" fpp ctrl = 0x%x\n", fpp_read_reg(FPP_CTRL));
}

static void PP_filter_config(void)
{
	unsigned int i, val;
	unsigned int reg = FPP_PP_FILTER_COE0;
	short coe[11] = {1, -4, 9, -15, 18, 104, 25, -17, 9, -4, 1};

	for (i = 0; i < 11; i += 4, reg += 0x4) {
		if (i < 8) {
			val = (coe[i] & 0xff) | ((coe[i + 1] & 0xff) << 8)
					| ((coe[i + 2] & 0xff) << 16) | ((coe[i + 3] & 0xff) << 24);
			fpp_write_reg(val, reg);
		} else {
			val = (coe[i] & 0xff) | ((coe[i + 1] & 0xff) << 8)
					| ((coe[i + 2] & 0xff) << 16);
			fpp_write_reg(val, reg);
		}
	}
}

static void NN_filter_config(void)
{
	unsigned int i, val;
	unsigned int reg = FPP_FILTER_COE0;
	unsigned short coe[25] = {0, 4, 7, 4, 0,
							  4, 17, 24, 17, 4,
							  7, 24, 32, 24, 7,
							  4, 17, 24, 17, 4,
							  0, 4, 7, 4, 0};

	for (i = 0; i < 25; reg += 0x4, i += 4) {
		if (i < 24) {
			val = coe[i] | (coe[i+1] << 8) | (coe[i+2] << 16) | (coe[i+3] << 24);
			fpp_write_reg(val, reg);
		} else if ( i == 24) {
			fpp_write_reg(coe[i], reg);
		}
	}
}

static void und_config(void)
{
	u32 i, val;
	u32 reg = FPP_UND_PARA0;
	short dist[9] = {231, -1166, 1374, -2, 23,
					8000, 5120, 8000, 3840}; //k1, k2, k3, p1, p2
											//fx, cx, fy, cy

	for (i = 0; i < 9; i += 1, reg += 0x4) {
		if (i != 2) {
			fpp_write_reg((dist[i] & 0xFFFF) | (dist[i+1] & 0xFFFF) << 16, reg);
			i += 1;
			//log_d("i = %d, reg = 0x%x, val = 0x%x\n", i, reg, fpp_read_reg(reg));
		} else {
			val = fpp_read_reg(reg) & 0xFFFF0000;
			fpp_write_reg((dist[i] & 0xFFFF) | val, reg);
			//log_d("i = %d, reg1 = 0x%x, val1 = 0x%x\n", i, reg, fpp_read_reg(reg));
		}
	}
}

static void fpp_res_cfg(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 width_wr, height_wr;
	u32 width_rd, height_rd;
	u32 fpp_stride;

	width_wr = (stream_ctrl_cmd->img_width);
	height_wr = (stream_ctrl_cmd->img_height);
	fpp_write_reg((width_wr | (height_wr << 16)), FPP_WR_IMG_RES);//wr_img res

	if (stream_ctrl_cmd->img_input_vertical == TRUE) {
		width_rd = stream_ctrl_cmd->img_height;
		height_rd = stream_ctrl_cmd->img_width;
	} else {
		width_rd = stream_ctrl_cmd->img_width;
		height_rd = stream_ctrl_cmd->img_height;
	}
	fpp_write_reg((width_rd | (height_rd << 16)), FPP_RD_IMG_RES);//rd_img res

	if (! (width_rd % 6)) {
		fpp_stride = (width_rd / 6) * 8;
	} else {
		fpp_stride = ((width_rd / 6) + 1) * 8;
	}
	fpp_write_reg(fpp_stride, FPP_STRIDE_OFF);//stride offset
}

static void fpp_blank_cfg(void)
{
	fpp_write_reg(FPP_OUT_HBLANK_SET, FPP_OUT_HBLANK1);//out hblank1
	fpp_write_reg(FPP_INPUT_HBLANK, FPP_IN_HBLANK);//in hblank
	fpp_write_reg(FPP_INPUT_VBLANK, FPP_IN_VBLANK);//in vblank
	fpp_write_reg(FPP_OUTPUT_HBLANK, FPP_OUT_HBLANK);//out hblank
	fpp_write_reg(FPP_OUTPUT_VBLANK, FPP_OUT_VBLANK);//out vblank
}

static void fpp_rd_addr_init(void)
{
	fpp_write_reg(STA_ADDR0_RD_0, FPP_STA_ADDR0_RD);//start addr0 read
	fpp_write_reg(STA_ADDR0_RD_1, FPP_STA_ADDR1_RD);//start addr1 read
}

static void fpp_cfg(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	fpp_write_reg(0x0, FPP_CTRL);//disable
	fpp_write_reg(0x1FFFF, FPP_INT_STS);//clear int
	fpp_write_reg(~FPP_INTR_DEFAULT_MASK_VAL, FPP_INT_MASK);//int unmask

	if (stream_ctrl_cmd->fpp_config.fpp_undist)
		und_config();//set unditort para
	if (stream_ctrl_cmd->fpp_config.fpp_prefilter)
		NN_filter_config();//set NN-filter
	if (stream_ctrl_cmd->fpp_config.fpp_pp_filter)
		PP_filter_config();//set PP-filter

	fpp_res_cfg(stream_ctrl_cmd);
	fpp_blank_cfg();
	fpp_rd_addr_init();
}

u32 fpp_frm_set(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 frm_type;

	if (stream_ctrl_cmd->req_frm_type == P_FRM) {
		frm_type = FPP_CTRL_P_FRM;
	} else if (stream_ctrl_cmd->req_frm_type == R_FRM) {
		frm_type = FPP_CTRL_R_FRM;
	} else if (stream_ctrl_cmd->req_frm_type == BP_FRM) {
		frm_type = FPP_CTRL_BP_FRM;
	} else if (stream_ctrl_cmd->req_frm_type == PR_FRM) {
		frm_type = FPP_CTRL_PR_FRM;
	} else if (stream_ctrl_cmd->req_frm_type == BPR_FRM) {
		frm_type = FPP_CTRL_BPP_FRM;
	}
	frm_type |= FPP_CTRL_TIR_MODE;
	
	return frm_type;
}

u32 fpp_data_path(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 data_path;

	switch (stream_ctrl_cmd->data_path) {
		case FPP_TO_DPU_SINGLE:
			data_path = FPP_STREAM_ON | FPP_REAL_ON;
			break;
		case FPP_TO_DPU_DIFF:
			data_path = FPP_STREAM_ON | FPP_FRM_TWO | FPP_UNREAL_ON | FPP_FRM_DIFF_ON;
			break;
		case FPP_TO_G2D:
			data_path = FPP_STREAM_ON | FPP_UNREAL_ON | FPP_RD_START;
			break;
		case DDR_TO_DPU_SINGLE:
			data_path = FPP_UNREAL_ON | FPP_RD_START;
			break;
		case DDR_TO_DPU_DIFF:
			data_path = FPP_FRM_TWO | FPP_UNREAL_ON | FPP_RD_START | FPP_FRM_DIFF_ON;
			break;
		default:
			break;
	}

	return data_path;
}

void fpp_enable(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 ret = 0x0;
	ret = FPP_FRM_CHECK | FPP_DPU_DONE_MASK;

	if (stream_ctrl_cmd->fpp_config.fpp_prefilter)
		ret |= FPP_NN_EN;
	if (stream_ctrl_cmd->fpp_config.fpp_pp_filter)
		ret |= FPP_PP_EN;
	if (stream_ctrl_cmd->fpp_config.fpp_undist)
		ret |= FPP_UNDIST_EN;
	
	ret |= fpp_frm_set(stream_ctrl_cmd);
	ret |= fpp_data_path(stream_ctrl_cmd);
	
	fpp_write_reg(ret, FPP_CTRL);
	
	stream_ctrl_cmd->fpp_config.ctrl_cfg = ret;
}

u32 FPP_out_cnt = 0x0;
irqreturn_t fpp_irq_handler(int irq, void *arg)
{
	u32 stat;
	struct stream_ctrl_cmd *stream_ctrl_cmd = (struct stream_ctrl_cmd *) arg;

	stat = fpp_read_reg(FPP_INT_STS);
	//imi_printf("fpp sts = 0x%x\n", stat);

	FPP_out_cnt = fpp_read_reg(FPP_MONI_OUT_FRAME_CNT);

	if (stat & INTR_RECV_FRM_END_IN)
		fpp_write_reg(INTR_RECV_FRM_END_IN, FPP_INT_STS);
	
	if (stat & INTR_RD_MEM_DONE) {
		fpp_write_reg(INTR_RD_MEM_DONE, FPP_INT_STS);
		if (stream_ctrl_cmd->data_path == DDR_TO_DPU_SINGLE || 
				stream_ctrl_cmd->data_path == DDR_TO_DPU_DIFF) {
			if ((fpp_read_reg(FPP_CTRL) >> 7) & 0x1) {
				if ( FPP_out_cnt % 2) {
					fpp_write_reg(STA_ADDR1_RD_0, FPP_STA_ADDR0_RD);
					fpp_write_reg(STA_ADDR1_RD_1, FPP_STA_ADDR1_RD);
				} else {
					fpp_write_reg(STA_ADDR0_RD_0, FPP_STA_ADDR0_RD);
					fpp_write_reg(STA_ADDR0_RD_1, FPP_STA_ADDR1_RD);
				}
				read_fpp_start_bit();
			}
		}
	}
 
	ulPortYieldRequired = 0UL;
	ulPortInterruptNesting = 1UL;
	
	return IRQ_HANDLED;
}

 u32 fpp_init(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 ret;

	fpp_cfg(stream_ctrl_cmd);
	
	ret = request_irq(IRQ_DPU_FPP, fpp_irq_handler, NULL, stream_ctrl_cmd);
	if (ret) {
		log_e("failure request irq %d : %d\n", IRQ_DPU_FPP, ret);
		return ret;
	}
	return 0;
}
