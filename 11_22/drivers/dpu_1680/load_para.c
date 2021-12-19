#include "dpu.h"
#include "load_para.h"

static inline void dbg_cfg_shift_wr_reg(u32 val, u32 reg)
{
	writel(val, REF_BASE_ADDR_SHIFT + reg);
}

static inline void dbg_cfg_depth_wr_reg(u32 val, u32 reg)
{
	writel(val, REF_BASE_ADDR_DEPTH + reg);
}

void debug_cfg_shitf(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 width, height, size;
	u32 dbg_frm_mode;

	if ( stream_ctrl_cmd->img_input_vertical == TRUE) {
		width = stream_ctrl_cmd->img_height;
		height = stream_ctrl_cmd->img_width;
	} else {
		width = stream_ctrl_cmd->img_width;
		height = stream_ctrl_cmd->img_height;		
	}	
	
	if (stream_ctrl_cmd->dbg_config.dbg_data_type > 2) {
		width = width / 2;
		height = height / 2;
	} 
	size = width * height * 2;
	dbg_frm_mode = stream_ctrl_cmd->dbg_config.dbg_frm_mode;
	
	dbg_cfg_shift_wr_reg(DEBUG_SRC_ADDR0, DEBUG_WR_CFG0); //addr
	dbg_cfg_shift_wr_reg(width * 2, DEBUG_WR_CFG2); //line stride
	dbg_cfg_shift_wr_reg(0x10, DEBUG_WR_CFG3);// max burst length, 16
	dbg_cfg_shift_wr_reg(dbg_frm_mode, DEBUG_WR_CFG4);//mode
	dbg_cfg_shift_wr_reg(width | (height << 16), DEBUG_IMG_CFG0);
	dbg_cfg_shift_wr_reg(size, DEBUG_IMG_CFG1);
	dbg_cfg_shift_wr_reg(0x1, DEBUG_WR_CFG5);
}

void ref_cfg_shitf(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 width, height, size;

	if ( stream_ctrl_cmd->img_input_vertical == TRUE) {
		width = stream_ctrl_cmd->img_height;
		height = stream_ctrl_cmd->img_width;
	} else {
		width = stream_ctrl_cmd->img_width;
		height = stream_ctrl_cmd->img_height;		
	}

	size = (width + 256) * (height + 32) * 2 / 8;

	dbg_cfg_shift_wr_reg(REF_SRC_ADDR0, REF_RD_CFG0); //addr
	dbg_cfg_shift_wr_reg(size, REF_RD_CFG2); //img size
	dbg_cfg_shift_wr_reg(0x10, REF_RD_CFG3); // max burst length, 16
	dbg_cfg_shift_wr_reg(0x0, REF_RD_CFG4); //software triggle
	dbg_cfg_shift_wr_reg(0x1, REF_INT_ENA); //int en
	dbg_cfg_shift_wr_reg(0x1, REF_RD_CFG5);
}

void debug_cfg_depth(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 width, height, size;
	u32 dbg_frm_mode;

	if ( stream_ctrl_cmd->img_input_vertical == TRUE) {
		width = stream_ctrl_cmd->img_height;
		height = stream_ctrl_cmd->img_width;
	} else {
		width = stream_ctrl_cmd->img_width;
		height = stream_ctrl_cmd->img_height;
	}

	if (stream_ctrl_cmd->dbg_config.dbg_data_type > 2) {
		width = width / 2;
		height = height / 2;
	}
	size = width * height * 2;
	dbg_frm_mode = stream_ctrl_cmd->dbg_config.dbg_frm_mode;

	dbg_cfg_depth_wr_reg(DEBUG_SRC_ADDR1, DEBUG_WR_CFG0); //addr
	dbg_cfg_depth_wr_reg(width * 2, DEBUG_WR_CFG2); //line stride
	dbg_cfg_depth_wr_reg(0x10, DEBUG_WR_CFG3);// max burst length, 16
	dbg_cfg_depth_wr_reg(dbg_frm_mode, DEBUG_WR_CFG4);//mode
	dbg_cfg_depth_wr_reg(width | (height << 16), DEBUG_IMG_CFG0);
	dbg_cfg_depth_wr_reg(size, DEBUG_IMG_CFG1);
	dbg_cfg_depth_wr_reg(0x1, DEBUG_WR_CFG5);
}

void ref_cfg_depth(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 width, height, size;

	if ( stream_ctrl_cmd->img_input_vertical == TRUE) {
		width = stream_ctrl_cmd->img_height;
		height = stream_ctrl_cmd->img_width;
	} else {
		width = stream_ctrl_cmd->img_width;
		height = stream_ctrl_cmd->img_height;
	}
	size = (width + 256) * (height + 32) * 2 / 8;

	dbg_cfg_depth_wr_reg(REF_SRC_ADDR1, REF_RD_CFG0); //addr
	dbg_cfg_depth_wr_reg(size, REF_RD_CFG2); //img size
	dbg_cfg_depth_wr_reg(0x10, REF_RD_CFG3); // max burst length, 16
	dbg_cfg_depth_wr_reg(0x0, REF_RD_CFG4); //software triggle
	dbg_cfg_depth_wr_reg(0x1, REF_INT_ENA); //int en
	dbg_cfg_depth_wr_reg(0x1, REF_RD_CFG5);
}

void dpu_dump_data(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	dpu_dbg_cfg(stream_ctrl_cmd->dbg_config.dbg_data_type);
	debug_cfg_shitf(stream_ctrl_cmd);
	debug_cfg_depth(stream_ctrl_cmd);
}

