#include "g2d.h"
#include "fpp.h"
#include "elog.h"
#include "gpio.h"
#include "sensor_def.h"

extern uint32_t ulPortInterruptNesting;
extern uint32_t ulPortYieldRequired;
extern void fpp_write_reg(u32 val, u32 reg);

static void g2d_write_reg(u32 val, u32 reg)
{
	writel(val, reg + G2D_BASE_ADDR);
}

static unsigned int g2d_read_reg(u32 reg)
{
	return readl(reg + G2D_BASE_ADDR);
}

irqreturn_t g2d_irq_handler_0(int irq, void *arg)
{
	//imi_printf("g2d irq (stat: reg 0x4)= 0x%x\n", g2d_read_reg(G2D_INT_STAT));
	if (g2d_read_reg(G2D_INT_STAT) & G2D_INTR_BFRM) {
		g2d_write_reg(G2D_INTR_BFRM, G2D_INT_STAT); //int clr
		//read_fpp_start_bit();
	}
	ulPortYieldRequired = 0UL;
	ulPortInterruptNesting = 1UL;
	return IRQ_HANDLED;
}

irqreturn_t g2d_irq_handler_1(int irq, void *arg)
{
	u32 g2d_count = g2d_read_reg(G2D_FRM_CNT);

	if (g2d_read_reg(G2D_INT_STAT) & G2D_INTR_PFRM) {
		g2d_write_reg(G2D_INTR_PFRM, G2D_INT_STAT); //int clr
		if (g2d_count % 2) {
			g2d_write_reg(PBUT_OUT_ADDR_1, G2D_OUT_PBUF);	
			fpp_write_reg(PBUT_OUT_ADDR_1, FPP_STA_ADDR0_RD);
			//imi_printf("g2d_count = %d\n", g2d_count);
		} else {
			g2d_write_reg(PBUT_OUT_ADDR_0, G2D_OUT_PBUF);	
			fpp_write_reg(PBUT_OUT_ADDR_0, FPP_STA_ADDR0_RD);	
			//imi_printf("g2d_count = %d\n", g2d_count);
		}
		read_fpp_start_bit();
	}
	ulPortYieldRequired = 0UL;
	ulPortInterruptNesting = 1UL;
	return IRQ_HANDLED;
}

irqreturn_t g2d_irq_handler_2(int irq, void *arg)
{
	//imi_printf("g2d irq (stat: reg 0x4)= 0x%x\n", g2d_read_reg(G2D_INT_STAT));
	if (g2d_read_reg(G2D_INT_STAT) & G2D_INTR_RFRM) {
		g2d_write_reg(G2D_INTR_RFRM, G2D_INT_STAT); //int clr
	}
	ulPortYieldRequired = 0UL;
	ulPortInterruptNesting = 1UL;
	return IRQ_HANDLED;
}

static void g2d_bpfrm_config(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 height, width;
	u32 rotate, informat, outformat, flip;
	u32 scale_fac, scale_mode;
	u32 ctrl = 0x0;

	width = stream_ctrl_cmd->img_width;
	height = stream_ctrl_cmd->img_height;
	rotate = stream_ctrl_cmd->g2d_config.bp_frm.rotate;
	informat = stream_ctrl_cmd->g2d_config.bp_frm.in_format;
	outformat = stream_ctrl_cmd->g2d_config.bp_frm.out_format;
	flip = stream_ctrl_cmd->g2d_config.bp_frm.flip;
	scale_fac = stream_ctrl_cmd->g2d_config.bp_frm.scale_factor;
	scale_mode = stream_ctrl_cmd->g2d_config.bp_frm.scale;

	ctrl = (scale_fac << 25) | (scale_mode << 24) | (flip << 16) |
			(rotate << 8) | (outformat << 4) | (informat << 0);

	g2d_write_reg(BBUF_OUT_ADDR_0, G2D_OUT_BBUF);
	g2d_write_reg((height | (width << 16)), G2D_IN_BSIZE);
	g2d_write_reg(ctrl, G2D_IN_BCONFIG);

	g2d_write_reg(PBUT_OUT_ADDR_0, G2D_OUT_PBUF);
	g2d_write_reg((height | (width << 16)), G2D_IN_PSIZE);
	g2d_write_reg(ctrl, G2D_IN_PCONFIG);
}

static void g2d_rfrm_config(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 height, width;
	u32 rotate, informat, outformat, flip;
	u32 scale_fac, scale_mode;
	u32 ctrl = 0x0;

	width = stream_ctrl_cmd->img_width;
	height = stream_ctrl_cmd->img_height;
	rotate = stream_ctrl_cmd->g2d_config.r_frm.rotate;
	informat = stream_ctrl_cmd->g2d_config.r_frm.in_format;
	outformat = stream_ctrl_cmd->g2d_config.r_frm.out_format;
	flip = stream_ctrl_cmd->g2d_config.r_frm.flip;
	scale_fac = stream_ctrl_cmd->g2d_config.r_frm.scale_factor;
	scale_mode = stream_ctrl_cmd->g2d_config.r_frm.scale;

	ctrl = (scale_fac << 25) | (scale_mode << 24) | (flip << 16) |
			(rotate << 8) | (outformat << 4) | (informat << 0);

	g2d_write_reg(RBUF_OUT_ADDR_0, G2D_OUT_RBUF);
	g2d_write_reg((height | (width << 16)), G2D_IN_RSIZE);
	g2d_write_reg(ctrl, G2D_IN_RCONFIG);
}

static void g2d_config(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 mode;

	mode = stream_ctrl_cmd->g2d_config.g2d_frm_mode;

	g2d_write_reg(0x0, G2D_CTRL_REG);//disable
	g2d_write_reg(0x1F, G2D_INT_STAT);//int clr
	g2d_write_reg(G2D_INTR_DEFAULT_MASK_VAL, G2D_INT_MASK);//int mask

	g2d_bpfrm_config(stream_ctrl_cmd);
	g2d_rfrm_config(stream_ctrl_cmd);

	g2d_write_reg((mode << 1), G2D_CTRL_REG);
}

void g2d_enable(void)
{
	u32 ret;

	ret = g2d_read_reg(G2D_CTRL_REG) | (1 << 0);
	g2d_write_reg(ret, G2D_CTRL_REG);
}

unsigned int g2d_init(struct stream_ctrl_cmd *stream_ctrl_cmd)
{
	u32 ret;

	g2d_config(stream_ctrl_cmd);
	
	ret = request_irq(IRQ_G2D_INT0, g2d_irq_handler_0, NULL, NULL);
	if (ret) {
		log_e("failure request irq %d : %d\n", IRQ_G2D_INT0, ret);
	}

	ret = request_irq(IRQ_G2D_INT1, g2d_irq_handler_1, NULL, NULL);
	if (ret) {
		log_e("failure request irq %d : %d\n", IRQ_G2D_INT1, ret);
	}
	
	ret = request_irq(IRQ_G2D_INT2, g2d_irq_handler_2, NULL, NULL);
	if (ret) {
		log_e("failure request irq %d : %d\n", IRQ_G2D_INT2, ret);
	}
	return 0;
}
