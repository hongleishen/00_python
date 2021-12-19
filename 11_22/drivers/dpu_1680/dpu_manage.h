#ifndef __DPU_TEST_H__
#define __DPU_TEST_H__

#include "imi_io.h"

enum debug_data_type {
	DEBUG_IR_IN = 0,
	DEBUG_BIN_OUT = 1,
	DEBUG_COR_OUT = 2,
	DEBUG_PP_OUT = 3,
	DEBUG_QQ_OUT = 4,
	DEBUG_DIS2DEPTH_OUT = 5,
	DEBUG_BLOB_OUT = 6,
	DEBUG_HOLEFILLING_OUT = 7,
	DEBUG_SHIFTMAT_OUT = 8,
	DEBUG_REGDEPTHMAT_OUT = 9,
	DEBUG_DEPTHFILTER_OUT = 10,
	DEBUG_MIRROR_OUT = 11,
	DEBUG_SET_CORRES_L3 = 12,
	DEBUG_SET_CORRES_L4 = 13,
	DEBUG_SET_CORRES_L5 = 14,
};

enum debug_frm_mode {
	DBG_SINGLE = 0x0,
	DBG_CONTINUS = 0x1,
};

enum req_frm_type {
	P_FRM = 0x0,
	R_FRM = 0x1,
	BP_FRM = 0x2,
	PR_FRM = 0x3,
	BPR_FRM = 0x4,
};

enum data_path {
	FPP_TO_DPU_SINGLE = 0x0,
	FPP_TO_DPU_DIFF = 0x1,
	FPP_TO_G2D = 0x2,
	DDR_TO_DPU_SINGLE = 0x3,
	DDR_TO_DPU_DIFF = 0x4,
};

enum g2d_frm_mode {
	G2D_CONTINUS = 0x0,
	G2D_SINGLE = 0x1,
};

/*
enum g2d_rotate_mode {
	NO_ROTATE = 0x0,
	ROTATE_90 = 0x1,
	ROTATE_180 = 0x2,
	ROTATE_270 = 0x3,
};
*/

enum in_format {
	RAW10 = 0x0,
	RAW12 = 0x1,
};

enum out_format {
	RAW_IN = 0x0,
	RAW8 = 0x1,
};

enum rotate_type {
	NO_ROTATE = 0x0,
	ROTATE90 = 0x1,
	ROTATE180 = 0x2,
	ROTATE270 = 0x3,
};

enum flip_type {
	NO_FLIP = 0x0,
	FLIP_HORIZON = 0x1,
	FLIP_VERTICAL = 0x2,
};

enum scale_factor {
	NO_SCALE = 0x0,
	SCALE_FOURTH = 0x1,
};

enum scale_mode {
	POINT = 0x0,
	AVER_VAL = 0x1,
};

struct g2d_frm_type {
	enum in_format in_format;
	enum out_format out_format;
	enum rotate_type rotate;
	enum flip_type flip;
	enum scale_factor scale_factor;
	enum scale_mode scale;
};

struct g2d_config {
	enum g2d_frm_mode g2d_frm_mode;
	//enum g2d_rotate_mode g2d_rotate_mode;
	struct g2d_frm_type bp_frm;
	struct g2d_frm_type r_frm;
};

struct debug_config {
	enum debug_data_type dbg_data_type;
	enum debug_frm_mode dbg_frm_mode;
};

struct fpp_config {
	u32 ctrl_cfg;
	u32 fpp_prefilter;
	u32 fpp_pp_filter;
	u32 fpp_undist;
	u32 fpp_frm_diff;
};

struct stream_ctrl_cmd {
	u32 img_width;
	u32 img_height;
	u32 img_size;
	u32 pixel_bits;
	u32 img_input_vertical;
	enum req_frm_type req_frm_type;
	enum data_path data_path;
	struct g2d_config g2d_config;
	struct debug_config dbg_config;
	struct fpp_config fpp_config;
};

u32 dpu_module_init(void);
void enable_fpp_and_g2d(void);

#endif//__DPU_TEST_H__
