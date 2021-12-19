#include "dpu.h"
#include "elog.h"

#define DDR_PP_BASEADD (0xc1000000)

static inline void dpu_wr_reg(u32 val, u32 reg)
{
	writel(val, DPU_BASE_ADDR + reg);
}

static inline u32 dpu_rd_reg(u32 reg)
{
	return readl(DPU_BASE_ADDR + reg);
}

unsigned int dpu_dbg_cfg(enum debug_data_type type)
{
	if (type < 0 || type > 0xF)
		return (-1);

	dpu_wr_reg(type, DPU_DBG_0);

	return 0;
}

void dpu_input_enable(void)
{
	u32 temp1;

	temp1 = dpu_rd_reg(DPU_CTRL);
	temp1 |= (1 << 0);
	dpu_wr_reg(temp1, DPU_CTRL);
}

void dpu_ctrl_reg_cfg(void)
{
	u32 ret = 0x0;

	ret |= (1 << 4);//dpu read dyram_0
	ret |= (1 << 7);//dpu read s2d
	dpu_wr_reg(ret, DPU_CTRL);
}

void bin_init(void)
{
	write32(DPU_BASE_ADDR + BIN_0, 0xfffd);
	write32(DPU_BASE_ADDR + BIN_1, 0x14);
	write32(DPU_BASE_ADDR + BIN_2, 0x14);
	write32(DPU_BASE_ADDR + BIN_3, 0x3fc);
	write32(DPU_BASE_ADDR + BIN_4, 0x0);
}

void ce_init(bool AGG,bool MM)
{
	u32 reg;
	if(AGG==true){
		write32(DPU_BASE_ADDR + CE_0, (0x3 << 1) | (0x1)|(0x8b8b << 8));
		}
	else{
		write32(DPU_BASE_ADDR + CE_0,(0x3 << 1) |(0x8b8b << 8));
		}
	if(MM==true){
		reg=dpu_rd_reg(CE_0);
		reg= reg | 0x80;
		dpu_wr_reg(reg, CE_0);
		}
	write32(DPU_BASE_ADDR + CE_1,  0x16);
	write32(DPU_BASE_ADDR + CE_2, 0xa | (0xa << 16));
	write32(DPU_BASE_ADDR + CE_SHIFT_CALC_0, 0x1c3000a);
	write32(DPU_BASE_ADDR + CE_SHIFT_CALC_1, 0x5);
	//write32(DPU_BASE_ADDR + CE_OUT_CTRL_0, 0xa00|(1<<16)|(1<<19)|(1<<20));
	//write32(DPU_BASE_ADDR + CE_OUT_CTRL_0, 0x5dc|(1<<16)|(1<<19)|(1<<20));
	//write32(DPU_BASE_ADDR + CE_OUT_CTRL_0, 0x5dc|(1<<16)|(1<<19)|(1<<20));
	write32(DPU_BASE_ADDR + CE_OUT_CTRL_0, 0x30a00);
	write32(DPU_BASE_ADDR + CE_LE_0, 0x32 << 1);
}

void pp_init(void)
{
	write32(DPU_BASE_ADDR + DPU_PP, DDR_PP_BASEADD);
}

void qq_init(void)
{
	write32(DPU_BASE_ADDR + DPU_QQ, 0x8);
}

void s2d_init(void)
{
	write32(DPU_BASE_ADDR + DEPTH_FILTER_0, 0x280);
	write32(DPU_BASE_ADDR + DEPTH_FILTER_1, 0x6 | (0x14 << 6));
	write32(DPU_BASE_ADDR + DEPTH_FILTER_2, 0x6 | (0x14 << 6));
}

void blob_init(void)
{
	write32(DPU_BASE_ADDR + DPU_BLOB_0, 0xA00);
	write32(DPU_BASE_ADDR + DPU_BLOB_1, 0xc8 | (0x14 << 16));
	//write32(DPU_BASE_ADDR + DPU_BLOB_2, 0x1);
}

void holefill_init(void)
{
	write32(DPU_BASE_ADDR + DPU_HOLEFILL_0, 0xA00);
	write32(DPU_BASE_ADDR + DPU_HOLEFILL_1, 0x6 | (0x14 << 6));
	write32(DPU_BASE_ADDR + DPU_HOLEFILL_2, 0x6 | (0x14 << 6));
	write32(DPU_BASE_ADDR + DPU_HOLEFILL_3, 0x6 | (0x14 << 6));
}

void shftmat_init(void)
{
	write32(DPU_BASE_ADDR + SHIFT_MAT_0, 0x4 | (0x4 << 4) | (0x2 << 10));
}

void depthmat_init(u32 width,u32 height)
{
	write32(DPU_BASE_ADDR + DEPTH_MAT_0, 16383);
	write32(DPU_BASE_ADDR + DEPTH_MAT_1, -148);
	write32(DPU_BASE_ADDR + DEPTH_MAT_2, -63);
	write32(DPU_BASE_ADDR + DEPTH_MAT_3, 147);
	write32(DPU_BASE_ADDR + DEPTH_MAT_4, 16382);
	write32(DPU_BASE_ADDR + DEPTH_MAT_5, -104);
	write32(DPU_BASE_ADDR + DEPTH_MAT_6, 64);
	write32(DPU_BASE_ADDR + DEPTH_MAT_7, 104);
	write32(DPU_BASE_ADDR + DEPTH_MAT_8, 16383);
	write32(DPU_BASE_ADDR + DEPTH_MAT_9, -11264);
	write32(DPU_BASE_ADDR + DEPTH_MAT_10, 0);
	write32(DPU_BASE_ADDR + DEPTH_MAT_11, -1024);
	write32(DPU_BASE_ADDR + DEPTH_MAT_12, 0xfa0 | (0xfa0 << 16));
//	write32(DPU_BASE_ADDR + DEPTH_MAT_13, 0xa00 | (0x640 << 16));
	write32(DPU_BASE_ADDR + DEPTH_MAT_13, width | (height << 16));
	write32(DPU_BASE_ADDR + DEPTH_MAT_14, 0xa00 | (0x40 << 16) | (0x1 << 24));
}

void mirror_init(void)
{

}

void dpu_global_init(u32 width, u32 height)
{
	u32 gwidth;
	if(width < 484)
		gwidth=480;
	else if(width < 960)
		gwidth=960;
	else
		gwidth=width;

	write32(DPU_BASE_ADDR + DPU_RESOL_BIN_CE_0, width | (height << 16));//BIN+CE
	write32(DPU_BASE_ADDR + DPU_RESOL_DNSCA_1, width / 2 | ((height/2) << 16));//DNSCAL
	write32(DPU_BASE_ADDR + DPU_RESOL_FILTE_MAT_MIRR_2,
			gwidth / 2 | ((height / 2) << 16));//MED_FILTER;SHIFT2DEPTH;REGDEPTH_MAT;DEPTH_FILTER;MIRROR;
	write32(DPU_BASE_ADDR + DPU_RESOL_BLOB_HOLEFILL_3,
			width / 2 | ((height / 2) << 16));//BLOB;HOLFILLING
}

void set_dpu_func_default(void)
{
	dpu_wr_reg(0xFFF, DPU_BYPASS);
}

void L3_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x01));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void L4_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x02));
	dpu_wr_reg(reg, DPU_BYPASS);
}
/*
void downscal_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x04));
	dpu_wr_reg(reg, DPU_BYPASS);
}
*/
void pp_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x04));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void qq_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x8));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void region_grow_enable(void)
{
	u32 reg;
	log_d("region_grow_enable\n");
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x10));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void blob_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x20));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void line_col_holefilling_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x40));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void blobholefilling_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x80));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void shiftmat_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x100));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void regdepthmat_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x200));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void depthfilterimat_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x400));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void mirror_enable(void)
{
	u32 reg;
	reg=dpu_rd_reg(DPU_BYPASS);
	reg= reg & (~(0x800));
	dpu_wr_reg(reg, DPU_BYPASS);
}

void set_dpu_fucn_bypass(struct dpu_bypass_ctrl *dpu_bypass_cfg)
{
	set_dpu_func_default();
	if((dpu_bypass_cfg->ce->L3) == true)
		L3_enable();
	if(dpu_bypass_cfg->ce->L4 == true)
		L4_enable();
/*
	if(dpu_bypass_cfg->downscal == true)
		downscal_enable();
*/
	if(dpu_bypass_cfg->pp == true)
		pp_enable();
	if(dpu_bypass_cfg->qq == true)
		qq_enable();
	if(dpu_bypass_cfg->region_grow == true)
		region_grow_enable();
	if(dpu_bypass_cfg->blob == true)
		blob_enable();
	if(dpu_bypass_cfg->line_col_holefilling == true)
		line_col_holefilling_enable();
	if(dpu_bypass_cfg->blob_holefilling == true)
		blobholefilling_enable();
	if(dpu_bypass_cfg->shiftmat==true)
		shiftmat_enable();
	if(dpu_bypass_cfg->regdepthmat==true)
		regdepthmat_enable();
	if(dpu_bypass_cfg->depthmat == true)
		depthfilterimat_enable();
	if(dpu_bypass_cfg->mirror == true)
		mirror_enable();
}

void dpu_init(struct dpu_manager_cfg *manager)
{
	u32 width = manager->img_width;
	u32 height = manager->img_height;

	dpu_wr_reg(0xF, DPU_INT_CLR);
	dpu_wr_reg(0xF, DPU_INT_EN);
	dpu_global_init(width, height);
	bin_init();

	if(manager->HAL_AGG == true){
		if(manager->HAL_MM)
			ce_init(true,true);
		else
			ce_init(true,false);
	}else{
		if(manager->HAL_MM)
			ce_init(false,true);
		else
			ce_init(false,false);
			}
	pp_init();
	qq_init();
	s2d_init();
	blob_init();
	holefill_init();
	shftmat_init();
	depthmat_init(width,height);
	mirror_init();
	dpu_ctrl_reg_cfg();
	dpu_wr_reg(0x1, REF_0);
}

void dpu_config_init(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	struct dpu_bypass_ctrl dpu_bypass_ctrl;
	struct ce_cfg ce_cfg;
	struct dpu_manager_cfg manager;

	dpu_bypass_ctrl.ce = &ce_cfg;
	dpu_bypass_config(&dpu_bypass_ctrl);
	set_dpu_fucn_bypass(&dpu_bypass_ctrl);

	if (stream_ctrl_cmd->img_input_vertical == TRUE) {
		manager.img_width = stream_ctrl_cmd->img_height;
		manager.img_height = stream_ctrl_cmd->img_width;
	} else {
		manager.img_width = stream_ctrl_cmd->img_width;
		manager.img_height = stream_ctrl_cmd->img_height;
	}

	if (dpu_bypass_ctrl.ce->AGG == true)
		manager.HAL_AGG=true;
	else
		manager.HAL_AGG=false;

	dpu_init(&manager);
}
