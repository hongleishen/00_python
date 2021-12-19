#if defined(CHIP_IMI_2280)
#include "dpu_2280.h"
#include "dpu.h"
#include "dpu_dev.h"
#include "reset.h"
#include "debug.h"
#include "gpio.h"
#include "clock.h"

#define DPU_DEV_NAME	"dpu_2280"

static LIST_HEAD(dpu_buf_list);
int dbg_lvl = LOG_LVL_ERR| LOG_LVL_INFO;

//suntongce
struct dpu_dev_buf {
	struct list_head list;
	int buf_addr;
};
#define USING_UVC_PKT_SWITCH 1
#ifndef USING_UVC_PKT_SWITCH
#define UVC_PKT_DISABLE_WAIT	0x4000
#else
#define UVC_PKT_DISABLE_WAIT	0x100
#endif

int write32_log1(int addr, int val)
{
	DPU_LOGV("dpu write: {0x%x, 0x%x}\n", addr & 0x3FF, val);
	write32(addr, val);
	return 0;
}
int read32_log1(int addr)
{
	int val;
	val = read32(addr);
	//DPU_LOGV("dpu read %x = %x\n", addr, val);
	return val;
}

static int uvc_pkt_init(struct imi_dpu *idpu, int path,
		int w, int h, int rotate)
{
	int val;
	int base;
	//int status
	if (path == DPU_DMA_PATH_IR) {
		base = idpu->hw.dpu_rbase.uvc_ir;
	} else if (path == DPU_DMA_PATH_DEPTH) {
		base = idpu->hw.dpu_rbase.uvc_dep;
	} else {
		return -EINVAL;
	}

	write32(base + 0x8, 0x1FF);
	write32(base + 0xC, 0xFFFFFFFC);
	val = (w & 0x7FF) | ((h & 0x7FF) << 16);
	if (path == DPU_DMA_PATH_DEPTH) {
		//horizon
		if (rotate == 0) {
			val = ((w / 2) & 0x7FF) | (((h / 2) & 0x7FF) << 16);
		} else {
			//vertical
			val = ((h / 2) & 0x7FF) | (((w / 2) & 0x7FF) << 16);
		}
	}
	write32(base + 0x14, val);
	write32(base + 0x18, 0x400);
	write32(base + 0x18, 0x400);
	write32(base + 0x1C, 0x3F);
	val = w * h * 16 / 64;
	if (path == DPU_DMA_PATH_DEPTH) {
		val /= 4;
	}
	write32(base + 0x38, val);
	//disable uvc_pkt at first
	write32(base + 0x10, UVC_PKT_DISABLE_WAIT | 0x14);
	idpu->res.h = h;
	idpu->res.w = w;
	idpu->res.rotate = rotate;
	idpu->res.sz = w * h * 2;
	return 0;
}

static int uvc_pkt_get_dma_size(struct imi_dpu *idpu,
		int path, int *w, int *h)
{
	int base;
	int reg;
	if (path == DPU_DMA_PATH_IR) {
		base = idpu->hw.dpu_rbase.uvc_ir;
	} else if (path == DPU_DMA_PATH_DEPTH) {
		base = idpu->hw.dpu_rbase.uvc_dep;
	} else {
		return 0;
	}
	reg = read32(base + 0x14);
	*w = reg & 0x7FF;
	*h = (reg >> 16) & 0x7FF;
	return (*w) * (*h);
}

static int uvc_pkt_set_dma_addr(struct imi_dpu *idpu, int path, int idx,
				int addr)
{
	int base;
	if (idx > 2) {
		DPU_LOGE("uvcpkt set max 3 buffer, wrong %d\n", idx);
		return -EINVAL;
	}
	if (path == DPU_DMA_PATH_IR) {
		base = idpu->hw.dpu_rbase.uvc_ir;
	} else if (path == DPU_DMA_PATH_DEPTH) {
		base = idpu->hw.dpu_rbase.uvc_dep;
	} else {
		DPU_LOGE("uvcpkt set invalid type %d\n", path);
		return -EINVAL;
	}
	write32(base + 0x20 + idx * 4, addr);
	DPU_LOGV("%s t: %d, buf %d addr %x\n", __func__, path, idx, addr);
	return 0;
}

static int uvc_pkt_get_dma_addr(struct imi_dpu *idpu, int path, int idx,
				int *addr)
{
	int base;
	if (idx > 2) {
		DPU_LOGE("uvcpkt get max 3 buffer, wrong %d\n", idx);
		return -EINVAL;
	}
	if (path == DPU_DMA_PATH_IR) {
		base = idpu->hw.dpu_rbase.uvc_ir;
	} else if (path == DPU_DMA_PATH_DEPTH) {
		base = idpu->hw.dpu_rbase.uvc_dep;
	} else {
		DPU_LOGE("uvcpkt get invalid type %d\n", path);
		return -EINVAL;
	}
	*addr = read32(base + 0x20 + idx * 4);
	DPU_LOGV("%s t: %d, idx %d addr %x\n", __func__, path, idx, *addr);
	return 0;
}

static int uvc_pkt_path_enable(void *ctrl, int path, int en)
{
	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	int base;
	int reg;
	if (path == DPU_DMA_PATH_IR) {
		base = idpu->hw.dpu_rbase.uvc_ir;
	} else if (path == DPU_DMA_PATH_DEPTH) {
		base = idpu->hw.dpu_rbase.uvc_dep;
	} else {
		DPU_LOGE("uvcpkt get invalid type %d\n", path);
		return -EINVAL;
	}
	reg = read32(idpu->hw.dpu_rbase.fpp + 0x10);
	if (en) {
		write32(base + 0x10, UVC_PKT_DISABLE_WAIT | 0x15);
	} else {
		write32(base + 0x10, UVC_PKT_DISABLE_WAIT | 0x14);
		udelay(10);
		write32(base + 0x10, UVC_PKT_DISABLE_WAIT | 0x14);
		udelay(10);
		write32(base + 0x10, UVC_PKT_DISABLE_WAIT | 0x14);
		udelay(10);
		write32(base + 0x10, UVC_PKT_DISABLE_WAIT | 0x14);
		udelay(10);
		write32(idpu->hw.dpu_rbase.fpp + 0x10, (reg & 0xFFFFFFFE) | 0x8);
	}
	return 0;
}

int fpp0_onoff(struct imi_dpu *idpu, int onoff)
{
	int base = idpu->hw.dpu_rbase.fpp;
	if (onoff) {
		//suntongce  warning dbg interface
		//		if (!gs->dbg_info.sensor_only) {
		write32(base + 0x10, 0x15);
		//		}
	} else {
		write32(base + 0x10, 0x8);
	}
	return 0;
}

int fpp0_config(struct imi_dpu *idpu, int w, int h)
{
	int base = idpu->hw.dpu_rbase.fpp;
	write32(base + 0x90, (w & 0x7FF) | ((h & 0x7FF) << 16));
	write32(base + 0x94, 0x04000100);
	write32(base + 0x98, 0x10);
	return 0;
}

void depth_irq_process(struct imi_dpu *idpu)
{
	int idx = 0;
//	int nxt = 0;
	int status = idpu->depth_irq_status;
	int buf_mask;
	int ret = -EINVAL;
	int phys_addr = 0;
	int sz, w, h;
	//	ktime_t kt;

	buf_mask = 0x3;

	if ((status & buf_mask) == 0) {
		dump_dpu_setting(idpu);
		DPU_LOGE("%s invalid buf msk %x\n", __func__, status);
		return;
	}
	/*
	   kt = ktime_sub(ktime_get(), gs->kt_start);
	   if (ktime_to_ms(kt)	< USER_DEFINE_SENSOR_FRAME_LEN_MS) {
	   DPU_LOGI("-D, req:%d, delta time %lld\n", gs->online.req.type, ktime_to_ms(kt));
	   uvc_pkt_select(gs, SENSOR_DEPTH, 0);
	   return;
	   }
	   */
	DPU_LOGT("[D%xD]", status);

#ifdef USING_UVC_PKT_SWITCH
	if ((status & buf_mask) == 0x1) {
		idx = 0;
//		nxt = 1;
	} else if ((status & buf_mask) == 0x2) {
		idx = 1;
//		nxt = 0;
	} else {
		DPU_LOGE("********ERROR status %x********\r\n", status);
		idx = 0;
//		nxt = 0;	//sunotngce warning, uvc_pkt switch need finish
		dump_dpu_setting(idpu);
	}
#else
	if ((status & buf_mask) == 0x1) {
		idx = 0;
//		nxt = 0;
	} else if ((status & buf_mask) == 0x2) {
		idx = 1;
//		nxt = 1;
	} else {
		DPU_LOGE("********ERROR status %x********\r\n", status);
		idx = 0;
//		nxt = 0;
		dump_dpu_setting(idpu);
	}
#endif

	ret = uvc_pkt_get_dma_addr(idpu, DPU_DMA_PATH_DEPTH, idx, &phys_addr);
	if (ret) {
		return;
	}
	sz = uvc_pkt_get_dma_size(idpu, DPU_DMA_PATH_DEPTH, &w, &h);
	//suntongce warning
	//	spin_lock(&idpu->lock);
	idpu->hw.depth_idx = idx;
	//	spin_unlock(&idpu->lock);

	if (idpu->isr_cb_depth) {
		ret = idpu->isr_cb_depth((void*)phys_addr, &sz, idx, 0);
	}
	return;
}

static int dp_err = 0;

irqreturn_t UVC_DEP_IRQHandler(int irq, void *devid)
{
	struct imi_dpu *idpu = (struct imi_dpu *)devid;
	unsigned int reg;
	int buf_mask;
	//clear irq
	//DPU_LOGT("%s\n", __func__);
	reg = read32(idpu->hw.dpu_rbase.uvc_dep + 0x8);
	write32(idpu->hw.dpu_rbase.uvc_dep + 0x8, reg);

	if (reg & 0x40) {
		dp_err++;
		printk("## dp err = %d\r\n", dp_err);
	}

	buf_mask = 0x3;
	if ((reg & buf_mask) == 0) {
		return IRQ_HANDLED;
	}

	idpu->depth_irq_status = reg;
	depth_irq_process(idpu);
	return IRQ_HANDLED;
}
EXPORT_SYMBOL(UVC_DEP_IRQHandler);

void ir_irq_process(struct imi_dpu *idpu)
{
	int idx = 0;
//	int nxt = 0;
	int status = idpu->ir_irq_status;
	int buf_mask;
	int ret;
	int phys_addr;
	int sz, w, h;
	//	ktime_t kt;

	buf_mask = 0x3;

	if ((status & buf_mask) == 0) {
		DPU_LOGE("invalid buf msk %x\n", status);
		dump_dpu_setting(idpu);
		return;
	}
	/*
	   kt = ktime_sub(ktime_get(), gs->kt_start);
	   if (ktime_to_ms(kt)	< USER_DEFINE_SENSOR_FRAME_LEN_MS) {
	   DPU_LOGI("-I, req:%d, delta time %lld\n", gs->online.req.type, ktime_to_ms(kt));
	   uvc_pkt_select(gs, SENSOR_IR, 0);
	   return;
	   }
	   */

	DPU_LOGT("[I%xI]", status);

#ifdef USING_UVC_PKT_SWITCH
	//	ret = bm_get_next(gs, SENSOR_IR);
	if ((status & buf_mask) == 0x1) {
		idx = 0;
//		nxt = 1;
	} else if ((status & buf_mask) == 0x2) {
		idx = 1;
//		nxt = 0;
	} else {
		DPU_LOGE("********ERROR status %x********\r\n", status);
		idx = 0;
//		nxt = 0;
		dump_dpu_setting(idpu);
	}
#else
	if ((status & buf_mask) == 0x1) {
		idx = 0;
//		nxt = 0;
	} else if ((status & buf_mask) == 0x2) {
		idx = 1;
//		nxt = 1;
	} else {
		DPU_LOGE("********ERROR status %x********\r\n", status);
		idx = 0;
//		nxt = 0;
		dump_dpu_setting(idpu);
	}
#endif

	ret = uvc_pkt_get_dma_addr(idpu, DPU_DMA_PATH_IR, idx, &phys_addr);
	if (ret) {
		return;
	}
	spin_lock(&idpu->lock);
	idpu->hw.ir_idx = idx;
	spin_unlock(&idpu->lock);

	sz = uvc_pkt_get_dma_size(idpu, DPU_DMA_PATH_IR, &w, &h);
	if (idpu->isr_cb_ir) {
		ret = idpu->isr_cb_ir((void*)phys_addr, &sz, idx, 0);
	}
	return;
}


static int ir_err = 0;
static unsigned int cnt_ss = 0;
static unsigned int cnt_min = 0;

irqreturn_t UVC_IR_IRQHandler(int irq, void *devid)
{
	struct imi_dpu *idpu = (struct imi_dpu *)devid;
	unsigned int reg;
	int buf_mask;

	//DPU_LOGT("%s\n", __func__);
	//clear irq
	reg = read32(idpu->hw.dpu_rbase.uvc_ir + 0x8);
	write32(idpu->hw.dpu_rbase.uvc_ir + 0x8, reg);

	if (reg & 0x40) {
		ir_err++;
		printk("## ir err = %d\r\n", ir_err);
	}

	buf_mask = 0x3;

	if ((reg & buf_mask) == 0) {
		return IRQ_HANDLED;
	}

	idpu->ir_irq_status = reg;
	ir_irq_process(idpu);

	if (cnt_ss > 30 * 60 * 10) {
		cnt_ss = 0;
		cnt_min++;
		DPU_LOGV("## %d0min: ir: %d, dp: %d\n", cnt_min, ir_err, dp_err);
	} else {
		cnt_ss++;
	}
	return IRQ_HANDLED;
}
EXPORT_SYMBOL(UVC_IR_IRQHandler);


irqreturn_t TRIGGER_0_IRQHandler(int irq, void *devid)
{
#if 0
	struct imi_dpu *idpu = (struct imi_dpu *)devid;
	int reg;
	DPU_LOGT("%s\n", __func__);
	reg = read32(idpu->hw.dpu_rbase.sensor_sync[0] + 0x8);
	if (reg != 3)
		DPU_LOGE("ss0 %x\n", reg);
	schedule_work(&idpu->work);
#endif
	return IRQ_HANDLED;
}
EXPORT_SYMBOL(TRIGGER_0_IRQHandler);

static int dpu_init_code(struct imi_dpu *idpu)
{
	struct dpu_depth_setting *dep;
	struct dpu_parameter *p = &idpu->param;
	unsigned int *sft;
	int sz;
	int i, j;

	if (!p->ref[0].ref_img|| (!p->initseq.init_seq_context)
			|| (!p->dy.dyram) || (!p->s2d.s2d)) {
		DPU_LOGE("%s err, %d, %p %p %p %p\r\n", __func__, __LINE__,
				p->ref[0].ref_img, p->initseq.init_seq_context,
				p->dy.dyram, p->s2d.s2d);
		return -EINVAL;
	}
	memcpy(idpu->hw.refsram, p->ref[0].ref_img, p->ref[0].ref_img_size);

	dep = p->initseq.init_seq_context;
	sz = p->initseq.init_seq_size;
	for (i = 0; i < sz; i++) {
		DPU_LOGV("%x %x %x %x\n", (dep + i)->op, (dep + i)->reg,
				(dep + i)->val, (dep + i)->delay);
		switch((dep + i)->op) {
			case 0:
				for (j = 0; j < (dep + i)->delay; j++) {
					if ((dep + i)->val == read32_log1(idpu->hw.dpu_rbase.dpu +
							(dep + i)->reg)) {
						break;
					}
					udelay(100);
				}
				if (j == (dep + i)->delay) {
					DPU_LOGE("dpu read %x timeout\n", (dep + i)->reg);
				}
				break;
			case 1:	//read times
				write32(idpu->hw.dpu_rbase.dpu + (dep + i)->reg ,
						(dep + i)->val);
				if ((dep + i)->delay) {
					udelay((dep + i)->delay);
				}
				break;
			case 0xFFFE:	//shift2depth
				sft = (unsigned int*)p->s2d.s2d;
				for (j = 0; j < p->s2d.s2d_size; j++) {
					write32(idpu->hw.dpu_rbase.s2d_ram + j * 4,
							*(sft + j));
				}
				break;
			case 0xFFFD:	//dyram
				sft = (unsigned int*)p->dy.dyram;
				for (j = 0; j < p->dy.dyram_sz; j++) {
					write32(idpu->hw.dpu_rbase.dy_ram + j * 4,
							*(sft + j));
				}
				break;
		}
	}
	return 0;
}

static void dpu_2280_clk_enable(struct imi_dpu *idpu)
{
	struct dpu_hw *dhw = &idpu->hw;
	clk_prepare_enable(dhw->clk_dpu);
	clk_set_rate(dhw->clk_dpu, idpu->hw.busclk);
	clk_prepare_enable(dhw->clk_slv);
	clk_set_rate(dhw->clk_slv, idpu->hw.busclk);
}

static void dpu_clk_disable(struct imi_dpu *idpu)
{
//	struct dpu_hw *dhw = &idpu->hw;

	//	printk("%s clk_dpu name: %s, parent name: %s, rate: %lu\n",
	//			__func__,
	//			dhw->clk_dpu->core->name,
	//			clk_get_parent(dhw->clk_dpu)->core->name,
	//			clk_get_rate(dhw->clk_dpu));
	clk_disable_unprepare(dhw->clk_dpu);
	clk_disable_unprepare(dhw->clk_slv);
}

static int dpu_power_off(struct imi_dpu *idpu)
{
	struct dpu_hw *dhw = &idpu->hw;
	if (!idpu->power)
		return 0;
	DPU_LOGI("%s\n", __func__);

	reset_control_assert(dhw->slv_rst);
	udelay(10);
	reset_control_assert(dhw->refsram_rst);
	udelay(10);

	dpu_clk_disable(idpu);

	idpu->power = 0;

	return 0;
}

static void dpu_2280_cfgfile_reset(struct imi_dpu *idpu)
{
	DPU_LOGI("%s\n", __func__);
	memset(&idpu->param, 0, sizeof(struct dpu_parameter));
}

static int dpu_power_on(struct imi_dpu *idpu)
{
	struct dpu_hw *dhw = &idpu->hw;
	int ret = 0;
	if (idpu->power)
		return 0;

	DPU_LOGI("%s\n", __func__);

	//dpu_2280_clk_enable(idpu);
	reset_control_deassert(dhw->slv_rst);
	udelay(10);
	reset_control_deassert(dhw->refsram_rst);
	udelay(10);

	idpu->power = 1;

	//	if (ret)
	//		dpu_power_off(idpu);

	return ret;
}

/*
   int dpu_auto_close(struct imi_dpu *idpu)
   {
   int ret;
   ret = dpu_power_off(idpu);
   dpu_2280_cfgfile_reset(idpu);
   return ret;
   }
   */

static int dpu_2280_buf_init(struct imi_dpu *idpu,
		struct dpu_buf_prop *prop)
{
	int ret = 0;
	ret = uvc_pkt_init(idpu, DPU_DMA_PATH_IR, prop->out_w,
			prop->out_h, prop->rotate);
	if (ret) {
		DPU_LOGV("IR buf init failed %x\n", ret);
		return ret;
	}

	ret = uvc_pkt_init(idpu, DPU_DMA_PATH_DEPTH, prop->out_w,
			prop->out_h, prop->rotate);
	if (ret) {
		DPU_LOGV("DEPTH buf init failed %x\n", ret);
		return ret;
	}

	uvc_pkt_set_dma_addr(idpu, DPU_DMA_PATH_IR, 0,
			prop->buf_addr[DPU_DMA_PATH_IR][0]);
	uvc_pkt_set_dma_addr(idpu, DPU_DMA_PATH_IR, 1,
			prop->buf_addr[DPU_DMA_PATH_IR][1]);
	uvc_pkt_set_dma_addr(idpu, DPU_DMA_PATH_DEPTH, 0,
			prop->buf_addr[DPU_DMA_PATH_DEPTH][0]);
	uvc_pkt_set_dma_addr(idpu, DPU_DMA_PATH_DEPTH, 1,
			prop->buf_addr[DPU_DMA_PATH_DEPTH][1]);

	fpp0_onoff(idpu, true);
	fpp0_config(idpu, prop->in_w, prop->in_h);

	return ret;
}

#if x
static int dpu_geom_data(struct imi_dpu *idpu, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int ret = 0;

	ret = copy_to_user(argp, &idpu->gs.dep.gd,
			sizeof(struct geometrical_data));
	if (ret)
		return ret;

	return 0;
}

static int dpu_gd_ready(struct imi_dpu *idpu, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int ret = 0;

	DPU_LOGG("pdy_ready: %d, test_cond_ready: %d\n",
			idpu->gs.geo.gdr.pdy_ready, idpu->gs.geo.gdr.test_cond_ready);
	ret = copy_from_user(&idpu->gs.geo.gdr, argp,
			sizeof(struct geometrical_data_ready));
	if (ret)
		return ret;

	DPU_LOGG("pdy_ready: %d, test_cond_ready: %d\n",
			idpu->gs.geo.gdr.pdy_ready, idpu->gs.geo.gdr.test_cond_ready);

	return 0;
}
#endif

static int dpu_2280_clk_init(struct imi_dpu *idpu)
{
#if 0
	struct dpu_hw *dhw = &idpu->hw;
	struct clk *ipi_clk = NULL;

	dhw->clk_dpu = devm_clk_get(idpu->dev, "clk_dpu");
	if (IS_ERR_OR_NULL(dhw->clk_dpu)) {
		pr_err("Failed to get clk_dpu\n");
		return PTR_ERR(dhw->clk_dpu);
	}

	dhw->clk_fast_bus_s = devm_clk_get(idpu->dev, "clk_dpu_fast_bus_s");
	if (IS_ERR_OR_NULL(dhw->clk_fast_bus_s)) {
		pr_err("Failed to get clk_fast_bus_s\n");
		return PTR_ERR(dhw->clk_fast_bus_s);
	}

	dhw->clk_fast_bus_m = devm_clk_get(idpu->dev, "clk_dpu_fast_bus_m");
	if (IS_ERR_OR_NULL(dhw->clk_fast_bus_m)) {
		pr_err("Failed to get clk_fast_bus_m\n");
		return PTR_ERR(dhw->clk_fast_bus_m);
	}

	dhw->clk_slv = devm_clk_get(idpu->dev, "clk_dpu_slv");
	if (IS_ERR_OR_NULL(dhw->clk_slv)) {
		pr_err("Failed to get clk_slv\n");
		return PTR_ERR(dhw->clk_slv);
	}

	dhw->clk_apb = devm_clk_get(idpu->dev, "clk_dpu_apb");
	if (IS_ERR_OR_NULL(dhw->clk_apb)) {
		pr_err("Failed to get clk_apb\n");
		return PTR_ERR(dhw->clk_apb);
	}

	ipi_clk = __clk_lookup("u2_csi_ipi_clk");
	dhw->clk_pix0 = devm_clk_get(idpu->dev, "clk_dpu_pix0");
	if (IS_ERR_OR_NULL(dhw->clk_pix0)) {
		pr_err("Failed to get clk_dpu_pix0\n");
		return PTR_ERR(dhw->clk_pix0);
	}
	clk_set_parent(dhw->clk_pix0, ipi_clk);

#endif
	return 0;
}
static int dpu_2280_reset(void *ctrl)
{
	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	struct dpu_hw *dhw = &idpu->hw;
	int ret = 0;

	//	dpu_power_off(idpu);
	//	dpu_power_on(idpu);
	ret |= reset_control_reset(dhw->slv_rst);
	ret |= reset_control_reset(dhw->refsram_rst);
	return ret;
}

//static int dpu_2280_resource_init(struct imi_dpu *idpu, struct platform_device *pdev)
static int dpu_2280_resource_init(struct imi_dpu *idpu)
{
	//struct resource *res;
	int dpu_base;
	int ret = 0;

	idpu->hw.refsram = (void __iomem *)DPU_REF_BASE_ADDR;
	dpu_base = DPU_BASE_ADDR;
	idpu->hw.dpu_rbase.dpu = (int)dpu_base;
	idpu->hw.dpu_rbase.ref_ram = (int)dpu_base + 0x8000;
	idpu->hw.dpu_rbase.depth = (int)dpu_base + 0x9000;
	idpu->hw.dpu_rbase.fpp = (int)dpu_base + 0xa000;
	idpu->hw.dpu_rbase.uvc_dep = (int)dpu_base + 0xb000;
	idpu->hw.dpu_rbase.uvc_ir = (int)dpu_base + 0xd000;
	idpu->hw.dpu_rbase.dy_ram = DPU_DY_RAM0;
	idpu->hw.dpu_rbase.s2d_ram = DPU_DY_RAM1;

	idpu->iopin.irq_uvc_dep = IMI_2280_DEP_IRQ_NO;	//119
	idpu->iopin.irq_uvc_ir = IMI_2280_IR_IRQ_NO;	//121
	idpu->iopin.irq_uvc_ss0 = IMI_2280_SS0_IRQ_NO;	//122

	ret = reset_control_get("reset", DPU_SLV_RST, &idpu->hw.slv_rst);
	ret |= reset_control_get("reset", DPU_REFSRAM_RST, &idpu->hw.refsram_rst);

	return ret;
}

#if 0
static int dpu_remove(struct platform_device *pdev)
{
	struct imi_dpu *idpu = platform_get_drvdata(pdev);

	kthread_stop(idpu->mbox_thread);
	idpu->mbox_thread = NULL;
	mbox_free_channel(idpu->mbox_chan);
	misc_deregister(&idpu->miscdev);
	dpu_send_deinit(idpu);
	dpu_recv_deinit(idpu);
	dpu_i2c_subdev_destroy(idpu);
	return 0;
}
#endif

#if 0
static int dpu_suspend(struct device *dev)
{
	struct imi_dpu *idpu = dev_get_drvdata(dev);
	struct dpu_hw *dhw = &idpu->hw;

	reset_control_assert(dhw->slv_rst);
	udelay(10);
	reset_control_assert(dhw->refsram_rst);
	udelay(10);
	dpu_clk_disable(idpu);

	return 0;
}

static int dpu_resume(struct device *dev)
{
	struct imi_dpu *idpu = dev_get_drvdata(dev);
	//	struct dpu_hw *dhw = &idpu->hw;

	dpu_2280_clk_enable(idpu);
	reset_control_deassert(dhw->slv_rst);
	udelay(10);
	reset_control_deassert(dhw->refsram_rst);
	udelay(10);

	return 0;
}
#endif

static int dump_dpu_2280_para(struct imi_dpu* idpu,
		struct dpu_parameter *para)
{
	return 0;
}

static int dpu_2280_power(void *ctrl, int onoff)
{
	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	int ret;
	DPU_LOGV("%s enter\n", __func__);
	if (onoff) {
		ret = dpu_power_on(idpu);
	} else {
		//dpu_auto_close(idpu);
		//	gs->dep.gd.gd_is_inited = 0;
		ret = dpu_power_off(idpu);
	}
	DPU_LOGV("%s exit\n", __func__);
	return ret;
}

static int dpu_2280_isr_cb(void *ctrl, int path, dpu_isr_cb cb)
{
	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	if (path == DPU_DMA_PATH_IR) {
		idpu->isr_cb_ir = cb;
	} else if (path == DPU_DMA_PATH_DEPTH) {
		idpu->isr_cb_depth = cb;
	}
	return 0;
}

//suntongtce
static int get_dma_addr(void *ctrl, int path, int *addr, int *sz)
{
	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	int ret = 0;
	int idx;
	spin_lock(&idpu->lock);
	if (path == DPU_DMA_PATH_DEPTH) {
		idx = idpu->hw.depth_idx;
		*sz = idpu->res.sz / 4;
	} else if (path == DPU_DMA_PATH_IR) {
		idx = idpu->hw.ir_idx;
		*sz = idpu->res.sz;
	} else {
		DPU_LOGE("path %d invalid\n", path);
		ret = -EINVAL;
	}
	spin_unlock(&idpu->lock);

	if (ret) {
		return ret;
	}

	ret = uvc_pkt_get_dma_addr(idpu, path, idx, addr);

	return ret;
}

static int update_dma_addr(void *ctrl, int addr, int len, int path)
{
	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	int ret = 0;
	int idx;
	spin_lock(&idpu->lock);
	if (path == DPU_DMA_PATH_DEPTH) {
		idx = idpu->hw.depth_idx;
	} else if (path == DPU_DMA_PATH_IR) {
		idx = idpu->hw.ir_idx;
	} else {
		DPU_LOGE("path %d invalid\n", path);
		ret = -EINVAL;
	}
	spin_unlock(&idpu->lock);

	if (ret) {
		return ret;
	}

	ret = uvc_pkt_set_dma_addr(idpu, path, idx, addr);

	return ret;
}

static int update_uvcpkt_size(void *ctrl, int w, int h, int path)
{
	//	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	//suntongce warning, need implement ??
	return 0;
}

static int dpu_2280_set_reg(void *ctrl, int reg, int val)
{
	write32(reg, val);
	return 0;
}

static int dpu_2280_get_reg(void *ctrl, int reg, int *val)
{
	*val = read32_log1(reg);
	return 0;
}

static int dpu_2280_set_para(void *ctrl, int para, void *data)
{
	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	int ret = 0;
	switch(para) {
		case DPU_PARA_CMD_SET_INIT:
			{
				struct dpu_parameter *para = (struct dpu_parameter *)data;
				if (!data) {
					DPU_LOGE("invalid para\n");
					ret = -EINVAL;
					break;
				}
				memcpy(&idpu->param, para, sizeof(struct dpu_parameter));
				break;
			}
		case DPU_PARA_CMD_LOAD_PARA:
			ret = dpu_init_code(idpu);
			if (ret) {
				DPU_LOGE("failed\n");
			}
			break;
			/*
			   case DPU_PARA_CMD_DEV_INIT:
			   ret = dpu_init_code(idpu);
			   if (ret) {
			   DPU_LOGE("failed\n");
			   }
			   break;
			   */
		case DPU_PARA_CMD_BUF_INIT:
			{
				struct dpu_buf_prop *p = (struct dpu_buf_prop *)data;
				ret = dpu_2280_buf_init(idpu, p);
				if (ret) {
					DPU_LOGE("failed\n");
				}
				break;
			}
		default:
			DPU_LOGE("unsupported cmd %x", para);
			break;
	}
	return 0;
}

static int dpu_2280_get_para(void *ctrl, int para, void *data)
{
	struct imi_dpu *idpu = (struct imi_dpu*)ctrl;
	switch(para) {
		case DPU_PARA_CMD_GET | DPU_PARA_CMD_GET_INIT:
			{
				struct dpu_parameter *para = (struct dpu_parameter *)data;
				dump_dpu_2280_para(idpu, para);
				memcpy(para, &idpu->param, sizeof(struct dpu_parameter));
				break;
			}
		default:
			DPU_LOGE("unsupported cmd %x", para);
			break;
	}
	return 0;
}

struct dpu_dev_ops dpu_2280_ops = {
	.power = dpu_2280_power,
	.reset = dpu_2280_reset,
	.update_dma_buf = update_dma_addr,
	.update_dma_size = update_uvcpkt_size,
	.get_dma_buf = get_dma_addr,
	.dma_enable = uvc_pkt_path_enable,
	.register_isr_cb = dpu_2280_isr_cb,
	.set_reg = dpu_2280_set_reg,
	.get_reg = dpu_2280_get_reg,
	.set_para = dpu_2280_set_para,
	.get_para = dpu_2280_get_para,
};

void dpu_2280_probe(void)
{
	struct imi_dpu *idpu = 0;
	struct dpu_handle *handle;
	int ret = 0;

	idpu = devm_kzalloc(idpu->dev, sizeof(*idpu), GFP_KERNEL);
	if (!idpu) {
		log_e("%s malloc fail\n", __func__);
		ret =  -ENOMEM;
		return;
	}

	//	idpu->dev = dev;

	ret = dpu_2280_resource_init(idpu);
	if (ret) {
		ret = -EINVAL;
		goto err;
	}

	ret = dpu_2280_reset(idpu);
	if (ret) {
		ret = -EINVAL;
		goto err;
	}

	ret = dpu_2280_clk_init(idpu);
	if (ret) {
		ret = -EINVAL;
		goto err;
	}

	idpu->hw.busclk = DPU_BUS_FREQ;


	//	init_waitqueue_head(&idpu->gs.geo.gd_wait);
	ret = request_irq(idpu->iopin.irq_uvc_dep, &UVC_DEP_IRQHandler,
			"uvc_dep_irq", idpu);
	ret = request_irq(idpu->iopin.irq_uvc_ir, &UVC_IR_IRQHandler,
			"uvc_ir_irq", idpu);
	//    ret = request_irq(idpu->iopin.irq_uvc_ss0, &TRIGGER_0_IRQHandler,
	//				"trigger_0_irq", idpu);
	//	disable_irq(idpu->iopin.irq_uvc_ss0);

	if (ret) {
		ret = -EINVAL;
		goto err;
	}

	dpu_2280_cfgfile_reset(idpu);

	dpu_2280_clk_enable(idpu);

	handle = dpu_register(&dpu_2280_ops);
	handle->priv = (void*)idpu;
	handle->name = DPU_DEV_NAME;
	return;
err:
	vPortFree(idpu);
	return;
}
DEV_INIT(dpu_2280_probe);
#endif/*CHIP_IMI_2280*/
