#ifndef __FPP_H__
#define __FPP_H__

#include "imi_io.h"
#include "dpu_manage.h"
#include "vic_int.h"

#define FPGA_DPU_BASE_ADDR			0x25000000
#define FPP_BASE_ADDR				(FPGA_DPU_BASE_ADDR + 0x40000)

#define FPP_INT_STS					0x0
#define FPP_INT_MASK				0x4
#define FPP_CTRL					0x8
#define FPP_MONI_RES				0xc /* read only */
#define FPP_MONI_HBLANK				0x10 /* read only */
#define FPP_MONI_VBLANK				0x14 /* read only */
#define FPP_WR_IMG_RES				0x18
#define FPP_RD_IMG_RES				0x1c
#define FPP_AHEAD_LINE				0x20
#define FPP_OUT_HBLANK1				0x24
#define FPP_IN_HBLANK				0x28
#define FPP_IN_VBLANK				0x2c
#define FPP_OUT_HBLANK				0x34
#define FPP_OUT_VBLANK				0x38
#define FPP_STA_ADDR0_RD			0x40
#define FPP_STA_ADDR1_RD			0x44
#define FPP_FILTER_COE0				0x48
#define FPP_FILTER_COE1				0x4c
#define FPP_FILTER_COE2				0x50
#define FPP_FILTER_COE3				0x54
#define FPP_FILTER_COE4				0x58
#define FPP_FILTER_COE5				0x5c
#define FPP_FILTER_COE6				0x60
#define FPP_UND_PARA0				0x64
#define FPP_UND_PARA1				0x68
#define FPP_UND_PARA2				0x6c
#define FPP_UND_PARA3				0x70
#define FPP_UND_PARA4				0x74
#define FPP_UND_LINE_ID				0x78 /* read only */
#define FPP_STRIDE_OFF				0x7c
#define FPP_MONI_IN_FRAME_CNT		0x80 /* read only */
#define FPP_MONI_OUT_FRAME_CNT		0x84 /* read only */
#define FPP_PP_FILTER_COE0 			0x88
#define FPP_PP_FILTER_COE1			0x8c
#define FPP_PP_FILTER_COE2 			0x90

#define STA_ADDR0_RD_0				0x85000000
#define STA_ADDR1_RD_0				0x86000000
#define STA_ADDR0_RD_1				0x87000000
#define STA_ADDR1_RD_1				0x88000000

#define FPP_OUT_HBLANK_SET			0x1400
#define FPP_INPUT_HBLANK			0xFFFF0000
#define FPP_INPUT_VBLANK			0xFFFF
#define FPP_OUTPUT_HBLANK			0xFFFF0000
#define FPP_OUTPUT_VBLANK			0xF00

#define FPP_STREAM_ON				(0x1 << 0)
#define FPP_FRM_CHECK				(0x1 << 2)
#define FPP_DPU_DONE_MASK			(0x1 << 3)
#define FPP_FRM_TWO					(0x1 << 5)
#define FPP_PP_EN					(0x1 << 6)
#define FPP_UNREAL_ON				(0x1 << 7)
#define FPP_REAL_ON					(0x1 << 8)
#define FPP_NN_EN					(0x1 << 9)//BPR
#define FPP_RD_START				(0x1 << 11)
#define FPP_FRM_DIFF_ON				(0x1 << 13)
#define FPP_CTRL_TIR_MODE			(0x1 << 24)
#define FPP_UNDIST_EN				(0x1 << 25)//BP

#define FPP_CTRL_P_FRM				(0x1 << 14)
#define FPP_CTRL_R_FRM				(0x2 << 14)
#define FPP_CTRL_BP_FRM				(0x104 << 14)
#define FPP_CTRL_PR_FRM				(0x109 << 14)
#define FPP_CTRL_BPP_FRM			(0x224 << 14)

#define INTR_RD_LFIFO_OVERFLOW 		(0x8)
#define INTR_RD_AFIFO_OVERFLOW 		(0x10)
#define INTR_OUT_LFIFO_OVERFLOW		(0x20)
#define INTR_FLT_LRAM_OVERFLOW 		(0x40)
#define INTR_UND_LBUF_OVERFLOW 		(0x80)
#define INTR_OUT_VBLANK_MISMATCH 	(0x100)
#define INTR_IN_VBLANK_MISMATCH 	(0x200)
#define INTR_IN_HBLANK_MISMATCH 	(0x400)
#define INTR_IN_FRM_MISMATCH 		(0x800)
#define INTR_IN_ROW_MISMATCH 		(0x1000)
#define INTR_RD_MEM_DONE 			(0x2000)
#define INTR_OUT_HBLANK_MISMATCH 	(0x8000)
#define INTR_RECV_FRM_END_IN 		(0x10000)

#define FPP_INTR_DEFAULT_MASK_VAL	(INTR_RD_MEM_DONE | \
									INTR_RECV_FRM_END_IN)

/***************************************************************************/
typedef union FPP_INT_MASK_U
{
	unsigned long v;
	struct FPP_INT_MASK_T {
		unsigned int rsvd0				:1;
		unsigned int rsvd1				:1;
		unsigned int rsvd2				:1;
		unsigned int rd_fifo_ovflow		:1;
		unsigned int rd_afifo_ovflow	:1;
		unsigned int out_fifo_ovflow	:1;
		unsigned int flt_ovflow			:1;
		unsigned int und_ovflow			:1;
		unsigned int out_hblank_mis		:1;
		unsigned int in_vblank_mis		:1;
		unsigned int in_hblank_mis		:1;
		unsigned int in_fr_mis			:1;
		unsigned int in_row_mis			:1;
		unsigned int rd_mem_done		:1;
		unsigned int rsvd3				:1;
		unsigned int out_hblank			:1;
		unsigned int recv_fr_end		:1;
		unsigned int rsvd4				:15;
	} bit_info;
} FPP_INT_MASK_P;

typedef union FPP_CTRL_U
{
	unsigned long v;
	struct FPP_CTRL_T {
		unsigned int ir_in_en			:1;
		unsigned int ir_vsync_polar		:1;
		unsigned int ir_fr_check_en		:1;
		unsigned int dpu_init_done		:1;
		unsigned int pack_align_mode	:1;
		unsigned int rd_mem_mode		:1;
		unsigned int rsvd0				:1;
		unsigned int rd_mem_en			:1;
		unsigned int real_out_en		:1;
		unsigned int pre_filter_en		:1;
		unsigned int r_undis_en			:1;
		unsigned int rd_start			:1;
		unsigned int rsvd1				:1;
		unsigned int fr_diff_en			:1;
		unsigned int fr_type			:11;
		unsigned int bp_undis_en		:1;
		unsigned int rsvd2				:6;
	}bit_info;
} FPP_CTRL_P;

typedef union WR_IMG_RES_U
{
	unsigned long v;
	struct WR_IMG_RES_T {
		unsigned int img_width			:16;
		unsigned int img_height			:16;
	} bit_info;
} WR_IMG_RES_P;

typedef union RD_IMG_RES_U
{
	unsigned long v;
	struct RD_IMG_RES_T {
		unsigned int img_width			:16;
		unsigned int img_height			:16;
	} bit_info;
} RD_IMG_RES_P;

typedef union OUT_HBLANK1_U
{
	unsigned long v;
	struct OUT_HBLANK1_T {
		unsigned int hblank1_min		:16;
		unsigned int rsvd				:16;
	} bit_info;
} OUT_HBLANK1_P;

typedef union IN_HBLANK_U
{
	unsigned long v;
	struct IN_HBLANK_T {
		unsigned int hblank_min			:16;
		unsigned int hblank_max			:16;
	} bit_info;
} IN_HBLANK_P;

typedef union IN_VBLANK_U
{
	unsigned long v;
	struct IN_VBLANK_T {
		unsigned int vblank_min			:32;
	} bit_info;
} IN_VBLANK_P;

typedef union OUT_HBLANK_U
{
	unsigned long v;
	struct OUT_HBLANK_T {
		unsigned int hblank_min			:16;
		unsigned int hblank_max			:16;
	} bit_info;
} OUT_HBLANK_P;

typedef union OUT_VBLANK_U
{
	unsigned long v;
	struct OUT_VBLANK_T {
		unsigned int vblank_min			:32;
	} bit_info;
} OUT_VBLANK_P;

typedef union STA_ADDR0_RD_U
{
	unsigned long v;
	struct STA_ADDR0_RD_T {
		unsigned int addr0_rd			:32;
	} bit_info;
} STA_ADDR0_RD_P;

typedef union STA_ADDR1_RD_U
{
	unsigned long v;
	struct STA_ADDR1_RD_T {
		unsigned int addr1_rd			:32;
	} bit_info;
} STA_ADDR1_RD_P;

typedef union STR_OFFSET_U
{
	unsigned long v;
	struct STR_OFFSET_T {
		unsigned int str_offset			:32;
	} bit_info;
} STR_OFFSET_P;

typedef struct FPP_CONFIG_T
{
	int					rsvd0;
	FPP_INT_MASK_P		int_mask;	//0x4
	FPP_CTRL_P			fpp_ctrl;	//0x8
	int					rsvd2[3];
	WR_IMG_RES_P		wr_img;		//0x18
	RD_IMG_RES_P		rd_img;		//0x1c
	int					rsvd3;
	OUT_HBLANK1_P		out_hblank1;//0x24
	IN_HBLANK_P			in_hblank;	//0x28
	IN_VBLANK_P			in_vblank;	//0x2c
	int					rsvd4;
	OUT_HBLANK_P		out_hblank;	//0x34
	OUT_VBLANK_P		out_vblank;	//0x38
	int					rsvd5;
	STA_ADDR0_RD_P		addr0_rd;	//0x40
	STA_ADDR1_RD_P		addr1_rd;	//0x44
	int					rsvd6[13];
	STR_OFFSET_P		str_offset;	//0x7c
} FPP_CONFIG_P;

irqreturn_t fpp_irq_handler(int irq, void *arg);
void read_fpp_start_bit(void);
u32 fpp_init(struct stream_ctrl_cmd *stream_ctrl_cmd);
void fpp_enable(struct stream_ctrl_cmd *stream_ctrl_cmd);

#endif//__FPP_H__
