#ifndef __DPU_H__
#define __DPU_H__

#include "imi_type.h"
#include "imi_io.h"
#include "dpu_manage.h"

/************************DPU REG************************************/
#define DPU_BASE_ADDR 				(0x25000000+0x30000)
#define DPU_PP_BASE_ADDR			(0x0)

#define BIN_0						(0x00)
#define BIN_1						(0x04)
#define BIN_2						(0x08)
#define BIN_3						(0x0C)
#define BIN_4						(0x10)
#define BIN_5						(0x14)
#define CE_0						(0x20)
#define CE_1						(0x24)
#define CE_2						(0x28)
#define CE_3						(0x2C)
#define CE_SHIFT_CALC_0				(0x30)
#define CE_SHIFT_CALC_1				(0x34)
#define CE_OUT_CTRL_0				(0x38)
#define CE_LE_0						(0x3C)
#define DPU_BLOB_0					(0x40)
#define DPU_BLOB_1					(0x44)
#define DPU_BLOB_2					(0x48)
#define DPU_HOLEFILL_0				(0x4c)
#define DPU_HOLEFILL_1				(0x50)
#define DPU_HOLEFILL_2				(0x54)
#define DPU_HOLEFILL_3				(0x58)
#define DPU_HOLEFILL_4				(0x5c)
#define DEPTH_MAT_0					(0x60)
#define DEPTH_MAT_1					(0x64)
#define DEPTH_MAT_2					(0x68)
#define DEPTH_MAT_3					(0x6C)
#define DEPTH_MAT_4					(0x70)
#define DEPTH_MAT_5					(0x74)
#define DEPTH_MAT_6					(0x78)
#define DEPTH_MAT_7					(0x7C)
#define DEPTH_MAT_8					(0x80)
#define DEPTH_MAT_9					(0x84)
#define DEPTH_MAT_10				(0x88)
#define DEPTH_MAT_11				(0x8C)
#define DEPTH_MAT_12				(0x90)
#define DEPTH_MAT_13				(0x94)
#define DEPTH_MAT_14				(0x98)
#define DEPTH_MAT_15				(0x9C)
#define DEPTH_FILTER_0				(0xa0)
#define DEPTH_FILTER_1				(0xa4)
#define DEPTH_FILTER_2				(0xa8)
#define SHIFT_MAT_0					(0xAC)
#define DPU_DBG_0					(0xB0)
#define DPU_PP						(0xB4)
#define DPU_QQ						(0xB8)
#define DPU_RESOL_BIN_CE_0			(0xC0)
#define DPU_RESOL_DNSCA_1			(0xC4)
#define DPU_RESOL_FILTE_MAT_MIRR_2	(0xC8)
#define DPU_RESOL_BLOB_HOLEFILL_3	(0xCC)
#define DPU_BYPASS					(0xD0)
#define DPU_CTRL					(0xD4)
#define REF_0				 		(0xEC)
#define DPU_INT_STATUS				(0x100)
#define DPU_INT_EN					(0x104)
#define DPU_INT_CLR					(0x108)
#define DPU_CE_L1_DBG				(0x110)
#define DPU_CE_L2_DBG				(0x114)
/******************************************************************/
struct ce_cfg
{
	bool AGG;
	bool L3;
	bool L4;
	bool MM;
};

struct dpu_bypass_ctrl
{
	struct	ce_cfg	*ce;
//	bool	downscal;
	bool	pp;
	bool	qq;
	bool	region_grow;
	bool	blob;
	bool	line_col_holefilling;
	bool	blob_holefilling;
	bool	shiftmat;
	bool	regdepthmat;
	bool	depthmat;
	bool	mirror;
};

typedef struct dpu_manager_cfg
{
	bool HAL_AGG;
	bool HAL_MM;
	u32 img_width;
	u32 img_height;
}dpu_manager_cfg;

enum bypass_data_type {
	BYPASS_ALL = 0,
	NOT_BYPASS_QQ = 1,
};

unsigned int dpu_dbg_cfg(enum debug_data_type type);
void dpu_config_init(struct stream_ctrl_cmd *stream_ctrl_cmd);
void dpu_input_enable(void);
void dpu_bypass_config(struct dpu_bypass_ctrl *dpu_bypass_ctrl);

#endif
