#ifndef __IMI_CSI__
#define __IMI_CSI__
#include "linux_common.h"
#if defined(CHIP_IMI_2280)
#include "csi_rx_2280.h"
#elif defined(CHIP_IMI_1680)
#include "csi_rx_1680.h"
#endif
#include "imi_vidomain.h"


#if defined(CHIP_IMI_1680)
#include "vic_int.h"
/* mipi data type */
#define MIPI_RAW_8BIT		0x2a
#define MIPI_RAW_10BIT		0x2b
#define MIPI_RAW_12BIT		0x2c
#define VCH_MAX_NUM			3
typedef struct imi_mipi_s {
	unsigned short id;
	unsigned short mbps;
	unsigned short height;
	unsigned short width;
	unsigned short data_t;
	unsigned short lanes;
	unsigned short vcno;
	unsigned int   cfg_clk;
	unsigned int   mod_base;
	unsigned int   bus_base;
	unsigned int   flags;
	unsigned int   addr[VCH_MAX_NUM];
}IMI_MIPI_S, *IMI_MIPI_T;

int mipi_init(IMI_MIPI_T mipi_cfg);
irqreturn_t mipi_ctl_irq_handle(int irq, void* param);
irqreturn_t mipi_ipi_irq_handle(int irq, void* param);
void mipi_ctl_int_enable(void);
void ipi_int_enable(int ipi_int);
void ipi_int_disable(int ipi_int);

#elif defined(CHIP_IMI_2280)


struct imi_csi {
	struct device *dev;

	struct clk *ipi;
	struct clk *refclk;
	struct clk *cfgclk;
	struct clk *esc;
	struct clk *mout_ipi;
	struct clk *mout_ref;
	struct clk *mout_cfg;
	struct clk *mout_esc;

	struct reset_control *csi_rst;
	struct reset_control *ipi_rst;

	void __iomem *base;

	int csi_id;
	struct list_head list;
};

#define csi2_dbg(level, debug, fmt, arg...)			\
	do {								\
		if (debug >= (level))					\
			imi_printf("imi-csi2: " fmt, ## arg);	\
	} while (0)

#define csi2_debug(fmt, arg...)			\
		imi_printf("imi-csi2: " fmt, ## arg);


void imi_csi_calc_freq(unsigned int mbps,
	unsigned int *hsfreqrange, unsigned int *osc_freq);

struct imi_csi *imi_get_csi(int id);

void imi_csi2_start(struct imi_csi *csi, struct vi_param *vip);

void imi_csi2_stop(struct imi_csi *csi);

void imi_csi2_start_reset(struct imi_csi *csi);

int imi_csi2_dhpy_reset(struct imi_csi *csi, unsigned int ccf,
	unsigned int osc_freq, unsigned int lanes);

int imi_csi2_dhpy_calib_check(struct imi_csi *csi, struct vi_param *vip);

#endif
#endif/*__IMI_CSI__*/

