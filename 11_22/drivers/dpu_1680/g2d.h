#ifndef __G2D_H__
#define __G2D_H__

#include "fpp.h"
#include "vic_int.h"

#define G2D_BASE_ADDR			0x24100000
#define G2D_CTRL_REG			0x00
#define G2D_INT_STAT			0x04
#define G2D_INT_MASK			0x08
#define G2D_FRM_CNT				0x0C
#define G2D_OUT_BBUF			0x10
#define G2D_IN_BSIZE			0x14
#define G2D_IN_BCONFIG			0x18
#define G2D_OUT_PBUF			0x20
#define G2D_IN_PSIZE			0x24
#define G2D_IN_PCONFIG			0x28
#define G2D_OUT_RBUF			0x30
#define G2D_IN_RSIZE			0x34
#define G2D_IN_RCONFIG			0x38
#define G2D_ROT_BLANK			0x40

#define BBUF_OUT_ADDR_0			0x85000000
#define BBUF_OUT_ADDR_1			0x86000000
#define PBUT_OUT_ADDR_0			0x85000000
#define PBUT_OUT_ADDR_1			0x86000000
#define RBUF_OUT_ADDR_0			0x89000000
#define RBUF_OUT_ADDR_1			0x8a000000

#define G2D_INTR_BFRM 			(0x1)
#define G2D_INTR_PFRM 			(0x2)
#define G2D_INTR_RFRM 			(0x4)
#define G2D_INTR_AXI_ERR 		(0x8)
#define G2D_INTR_FIFO_OVERFLOW 	(0x10)

#define G2D_INTR_DEFAULT_MASK_VAL 	(G2D_INTR_BFRM | \
									G2D_INTR_PFRM | \
									G2D_INTR_RFRM)
typedef union G2D_CTRL_U
{
	unsigned long v;
	struct G2D_CTRL_T {
		unsigned int start			:1;
		unsigned int mode			:1;
		unsigned int axi_len		:1;
		unsigned int rsvd			:29;
	}bit_info;
}G2D_CTRL_P;

typedef union INT_STAT_U
{
	unsigned long v;
	struct INT_STAT_T {
		unsigned int int_bstat		:1;
		unsigned int int_pstat		:1;
		unsigned int int_rstat		:1;
		unsigned int axi_erro		:1;
		unsigned int fifo_overf		:1;
		unsigned int rsvd			:27;
	}bit_info;
}INT_STAT_P;

typedef union INT_MASK_U
{
	unsigned long v;
	struct INT_MASK_T {
		unsigned int int_bmask		:1;
		unsigned int int_pmask		:1;
		unsigned int int_rmask		:1;
		unsigned int axi_erro		:1;
		unsigned int fifo_overf		:1;
		unsigned int rsvd			:27;
	}bit_info;
}INT_MASK_P;

typedef union OUT_BBUF_U
{
	unsigned long v;
	struct OUT_BBUF_T {
		unsigned int out_bbuf		:32;
	}bit_info;
}OUT_BBUF_P;

typedef union IN_BSIZE_U
{
	unsigned long v;
	struct IN_BSIZE_T {
		unsigned int bsize_height	:13;
		unsigned int rsvd0			:3;
		unsigned int bsize_width	:13;
		unsigned int rsvd1			:3;
	}bit_info;
}IN_BSIZE_P;

typedef union IN_BCONFIG_U
{
	unsigned long v;
	struct IN_BCONFIG_T {
		unsigned int in_bformat		:1;
		unsigned int rsvd0			:3;
		unsigned int out_bformat	:1;
		unsigned int rsvd1			:3;
		unsigned int brotate		:2;
		unsigned int rsvd2			:6;
		unsigned int b_flip			:2;
		unsigned int rsvd3			:6;
		unsigned int bscale_mode	:1;
		unsigned int bscale_factor	:1;
		unsigned int rsvd4			:6;
	}bit_info;
}IN_BCONFIG_P;

typedef union OUT_PBUF_U
{
	unsigned long v;
	struct OUT_PBUF_T {
		unsigned int out_bbuf		:32;
	}bit_info;
}OUT_PBUF_P;

typedef union IN_PSIZE_U
{
	unsigned long v;
	struct IN_PSIZE_T {
		unsigned int psize_height	:13;
		unsigned int rsvd0			:3;
		unsigned int psize_width	:13;
		unsigned int rsvd1			:3;
	}bit_info;
}IN_PSIZE_P;

typedef union IN_PCONFIG_U
{
	unsigned long v;
	struct IN_PCONFIG_T {
		unsigned int in_pformat		:1;
		unsigned int rsvd0			:3;
		unsigned int out_pformat	:1;
		unsigned int rsvd1			:3;
		unsigned int protate		:2;
		unsigned int rsvd2			:6;
		unsigned int pflip			:2;
		unsigned int rsvd3			:6;
		unsigned int pscale_mode	:1;
		unsigned int pscale_factor	:1;
		unsigned int rsvd4			:6;
	}bit_info;
}IN_PCONFIG_P;

typedef union OUT_RBUF_U
{
	unsigned long v;
	struct OUT_RBUF_T {
		unsigned int out_rbuf		:32;
	}bit_info;
}OUT_RBUF_P;

typedef union IN_RSIZE_U
{
	unsigned long v;
	struct IN_RSIZE_T {
		unsigned int rbuf_height	:13;
		unsigned int rsvd0			:3;
		unsigned int rbuf_width		:13;
		unsigned int rsvd1			:3;
	}bit_info;
}IN_RSIZE_P;

typedef union IN_RCONFIG_U
{
	unsigned long v;
	struct IN_RCONFIG_T {
		unsigned int in_rformat		:1;
		unsigned int rsvd0			:3;
		unsigned int out_rformat	:1;
		unsigned int rsvd1			:3;
		unsigned int rrotate		:2;
		unsigned int rsvd2			:6;
		unsigned int r_flip			:2;
		unsigned int rsvd3			:6;
		unsigned int rscale_mode	:1;
		unsigned int rscale_factor	:1;
		unsigned int rsvd4			:6;
	}bit_info;
}IN_RCONFIG_P;

typedef union ROTATE_HBLANK_U
{
	unsigned long v;
	struct ROTATE_HBLANK_T {
		unsigned int rot_hblank		:3;
		unsigned int rsvd			:29;
	}bit_info;
}ROTATE_HBLANK_P;

typedef struct G2D_CONFIG_T
{
	G2D_CTRL_P			g2d_ctrl;	//0x0
	INT_STAT_P			int_stat;	//0x4
	INT_MASK_P			int_mask;	//0x8
	int					rsvd0;
	OUT_BBUF_P			out_bbuf;	//0x10
	IN_BSIZE_P			in_bsize;	//0x14
	IN_BCONFIG_P		in_bconfig;	//0x18
	int					rsvd1;
	OUT_PBUF_P			out_pbuf;	//0x20
	IN_PSIZE_P			in_psize;	//0x24
	IN_PCONFIG_P		in_pconfig;	//0x28
	int					rsvd2;
	OUT_RBUF_P			out_rbuf;	//0x30
	IN_RSIZE_P			in_rsize;	//0x34
	IN_RCONFIG_P		in_rconfig;	//0x38
	int					rsvd3;
	ROTATE_HBLANK_P		rotate_hblank;//0x40
}G2D_CONFIG_P;

irqreturn_t g2d_irq_handler_0(int irq, void *arg);
irqreturn_t g2d_irq_handler_1(int irq, void *arg);
irqreturn_t g2d_irq_handler_2(int irq, void *arg);
unsigned int g2d_init(struct stream_ctrl_cmd *stream_ctrl_cmd);
void g2d_enable(void);
#endif//__G2D_H__
