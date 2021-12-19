#include "dpu.h"
#include "fpp.h"
#include "g2d.h"
#include "load_para.h"
#include "gpio.h"
#include "elog.h"
#include "dev.h"
#include "initcalls.h"
#include "clock.h"

void dpu_bypass_config(struct dpu_bypass_ctrl *dpu_bypass_ctrl)
{
	dpu_bypass_ctrl->ce->AGG = 0;
	dpu_bypass_ctrl->ce->L3 = 0;
	dpu_bypass_ctrl->ce->L4 = 0;
	dpu_bypass_ctrl->ce->MM = 0;
//	dpu_bypass_ctrl->downscal = 0;
	dpu_bypass_ctrl->pp = 0;
	dpu_bypass_ctrl->qq = 0;
	dpu_bypass_ctrl->region_grow = 0;
	dpu_bypass_ctrl->blob = 0;
	dpu_bypass_ctrl->line_col_holefilling = 0;
	dpu_bypass_ctrl->blob_holefilling = 0;
	dpu_bypass_ctrl->shiftmat = 0;
	dpu_bypass_ctrl->regdepthmat = 0;
	dpu_bypass_ctrl->depthmat = 0;
	dpu_bypass_ctrl->mirror = 0;
}

void get_stream_ctrl_cmd(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	stream_ctrl_cmd->img_width = 1280;
#if (SYNPSYS_DPHY_TESTCHIP == 1)
	stream_ctrl_cmd->img_height = 960;
#else
	stream_ctrl_cmd->img_height = 800;
#endif
	//stream_ctrl_cmd->img_size = (1280 * 960 * 2);
	stream_ctrl_cmd->img_input_vertical = FALSE;
	stream_ctrl_cmd->req_frm_type = P_FRM;
	stream_ctrl_cmd->data_path = FPP_TO_DPU_SINGLE;

	stream_ctrl_cmd->dbg_config.dbg_data_type = DEBUG_IR_IN;
	stream_ctrl_cmd->dbg_config.dbg_frm_mode = DBG_CONTINUS;

	stream_ctrl_cmd->g2d_config.g2d_frm_mode = G2D_CONTINUS;
	stream_ctrl_cmd->g2d_config.bp_frm.in_format = RAW10;
	stream_ctrl_cmd->g2d_config.bp_frm.out_format = RAW_IN;
	stream_ctrl_cmd->g2d_config.bp_frm.rotate = NO_ROTATE;
	stream_ctrl_cmd->g2d_config.bp_frm.flip = NO_FLIP;
	stream_ctrl_cmd->g2d_config.bp_frm.scale = AVER_VAL;
	stream_ctrl_cmd->g2d_config.bp_frm.scale_factor = NO_SCALE;

	stream_ctrl_cmd->g2d_config.r_frm.in_format = RAW10;
	stream_ctrl_cmd->g2d_config.r_frm.out_format = RAW_IN;
	stream_ctrl_cmd->g2d_config.r_frm.rotate = NO_ROTATE;
	stream_ctrl_cmd->g2d_config.r_frm.flip = NO_FLIP;
	stream_ctrl_cmd->g2d_config.r_frm.scale = AVER_VAL;
	stream_ctrl_cmd->g2d_config.r_frm.scale_factor = NO_SCALE;

	stream_ctrl_cmd->fpp_config.fpp_prefilter = FALSE;
	stream_ctrl_cmd->fpp_config.fpp_pp_filter = FALSE;
	stream_ctrl_cmd->fpp_config.fpp_undist = FALSE;

	if (stream_ctrl_cmd->data_path == DDR_TO_DPU_DIFF ||
		stream_ctrl_cmd->data_path == DDR_TO_DPU_SINGLE) {
		stream_ctrl_cmd->img_height = 960;
	}
}

u32 dpu_module_init(void)
{
	static struct stream_ctrl_cmd stream_ctrl_cmd;

	get_stream_ctrl_cmd(&stream_ctrl_cmd);

	/* 1. dpu dump set */
	dpu_dump_data(&stream_ctrl_cmd);

	/* 2. dpu init */
	dpu_config_init(&stream_ctrl_cmd);

	/* 3. ref set */
	ref_cfg_depth(&stream_ctrl_cmd);
	ref_cfg_shitf(&stream_ctrl_cmd);

	/* 4. dpu enable */
	dpu_input_enable();
	while (!(readl(DPU_BASE_ADDR + DPU_INT_STATUS) & 0x4)) {
		log_d("dpu sts = 0x%x\n", readl(DPU_BASE_ADDR + DPU_INT_STATUS));
	};

	if ((stream_ctrl_cmd.data_path == FPP_TO_G2D)
		|| (stream_ctrl_cmd.data_path == FPP_TO_DPU_DIFF)) {
		g2d_init(&stream_ctrl_cmd);
		fpp_init(&stream_ctrl_cmd);
	} else {
		//g2d_init(&stream_ctrl_cmd);
		fpp_init(&stream_ctrl_cmd);
	}
	return 0;
}

void enable_fpp_and_g2d(void)
{
	struct stream_ctrl_cmd stream_ctrl_cmd;
	
	log_d("fpp and g2d enable\n");
	get_stream_ctrl_cmd(&stream_ctrl_cmd);

	if ((stream_ctrl_cmd.data_path == FPP_TO_G2D)
		|| (stream_ctrl_cmd.data_path == FPP_TO_DPU_DIFF)) {
		g2d_enable();
		fpp_enable(&stream_ctrl_cmd);
	} else {
		fpp_enable(&stream_ctrl_cmd);
	}
}

#if 0
extern void trig_sensor_stream(void);
void dpu_done(void)
{
	unsigned int i = 0;

	while (1) {
		if ((readl(FPP_BASE_ADDR + FPP_INT_STS) & 0x12000)) {
			imi_printf("fpp int sts = 0x%x\n", readl(FPP_BASE_ADDR + FPP_INT_STS));
			gpio_direction_output(27, 0);
			writel(0x12000, FPP_BASE_ADDR + FPP_INT_STS);
			for (int j = 0; j < 0x20000; j++);
			trig_sensor_stream();
			//read_fpp_start_bit();

			imi_printf("count = %d\n", i++);
		}
	}
	imi_printf("dpu test done\n");
}

void g2d_done(void)
{
	unsigned int i = 0;

	while (1) {
		if ((readl(G2D_BASE_ADDR + G2D_INT_STAT) & 0xF)) {
			log_d("g2d int sts = 0x%x\n", readl(G2D_BASE_ADDR + G2D_INT_STAT));
			gpio_direction_output(27, 0);
			writel(0x1F, G2D_BASE_ADDR + G2D_INT_STAT);
			read_fpp_start_bit();
			trig_sensor_stream();
			log_d("count = %d\n", i++);
		}
	}

	log_d("g2d test done\n");
}
#endif
void dpu_device_init(void)
{
	clock_enable("dpu");
    clock_enable("dd");

	dpu_module_init();
}

#if defined(TARGET_FPGA)
DEV_INIT(dpu_device_init);
#endif