#ifndef __IMI2280_DPU_DEV_H__
#define __IMI2280_DPU_DEV_H__
#include "vic_int.h"
#include "reset.h"
#include "linux_common.h"
#include "dpu.h"

//#define USER_DEFINE_SENSOR_FRAME_LEN_MS	(10)  //warning,  this value should be passed from dpu_cfg.ini

//typedef int (*dpu_isr_cb)(void *, int *, int, int);


struct dpu_le
{
	unsigned int le_calc_step_x; //64
	unsigned int le_calc_step_y; //64
	unsigned int sadw; //17
	unsigned int l1_para_line; //8
};
struct corr_cand
{
	unsigned char sad;  // ȡֵ??Χ: [0, 17*17], ????:δ????
	unsigned char sfx;  // ȡֵ??Χ: [0, SFT_RANGE_X), 511:δ??ֵ, ????: δ????
	unsigned char sfy;  // ȡֵ??Χ: [0, SFT_RANGE_Y), 255:δ??ֵ, ????: δ????
	unsigned char nod;  // ??Чֵ
};

struct geometrical_data
{
	unsigned int total_size;
	struct corr_cand **test_cand;
	char *test_cand_offset;
	unsigned int test_cand_size;
	unsigned int le_height;
	unsigned int le_width;
	unsigned short **pdy;
	char *pdy_offset;
	unsigned int pdy_size;
	unsigned int dy_ram_height;
	unsigned int dy_ram_width;
	unsigned int image_width;
	unsigned int image_height;
	float deform_param[6];
	float dist_param[6];
	short calc_thresh;
	int gd_is_inited;
	int reserved[5];
};


struct dpu_reg_base {
	int dpu;
	//    int global_trigger;
	//    int sensor_sync[5];
	int depth;
	int uvc_ir;
	int uvc_dep;
	int fpp;
	//    int timer;
	int dy_ram;
	int s2d_ram;
	int ref_ram;
	//    int dpu_clk;
};


struct dpu_hw {
	char depth_idx;
	char ir_idx;

	int busclk;
	struct clk *clk_dpu;
	struct clk *clk_fast_bus_s;
	struct clk *clk_fast_bus_m;
	struct clk *clk_slv;
	struct clk *clk_apb;
	struct clk *clk_pix0;

	struct reset_control *slv_rst;
	struct reset_control *refsram_rst;

	void __iomem *refsram;
	void __iomem *mbox_mem;

	int irq_uvc_dep;
	int irq_uvc_ir;
	int irq_uvc_ss0;

	struct dpu_reg_base dpu_rbase;
	//	phys_addr_t mbox_phys;
	//	phys_addr_t rb_phys;
	//	int rb_len;
};

struct dpu_res {

	int irq_uvc_dep;
	int irq_uvc_ir;
	int irq_uvc_ss0;
};

struct depth_setting
{
	/*
	   unsigned char binary;
	   unsigned char bypassblob;
	   unsigned char bypassholefilling;
	   unsigned char mirror;
	   unsigned char bypassregistration;
	   unsigned char bypassshiftprocess;
	   unsigned char shift;
	   unsigned short shift_maxval;        //suntongce
	   unsigned char regis_maxi_subup_cfg;
	   unsigned int regis_offx;
	   unsigned int regis_offy;
	   struct dpu_hblank   hblank;
	   */
	//struct dpu_ref ref_cfg;
	signed int registration_tab[13];
	struct dpu_le le_cfg;
	struct geometrical_data gd;
};

struct dpu_dma_res {
	short w;
	short h;
	char rotate;
	int sz;
};
struct imi_dpu {
	struct device *dev;
	struct dpu_hw hw;

	dpu_isr_cb isr_cb_depth;
	dpu_isr_cb isr_cb_ir;

	char name[20];
	int power;
	struct dpu_res iopin;
	int depth_irq_status;
	int ir_irq_status;
	struct dpu_parameter param;
	struct dpu_dma_res	res;
	FRTOS_mutex mutex;
	spinlock_t lock;
};

enum {
	I2C_BUS_SNR = 0,
	I2C_BUS_DOT,
	I2C_BUS_FLOOD,
	I2C_DEV_MAX,
};

irqreturn_t UVC_DEP_IRQHandler(int irq, void *devid);
irqreturn_t UVC_IR_IRQHandler(int irq, void *devid);
irqreturn_t TRIGGER_0_IRQHandler(int irq, void *devid);
/*
   extern enum hrtimer_restart fps_callback(struct hrtimer *timer);
   extern enum hrtimer_restart close_light(struct hrtimer *timer);
   */
void dpu2280_probe(void);
#endif /*__IMI2280_DPU_DEV_H__*/
