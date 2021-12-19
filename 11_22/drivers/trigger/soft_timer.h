#if 0//ndef __SENSOR_SYNC_H__
#define __SENSOR_SYNC_H__
#include "linux_common.h"
#include "trigger.h"
#include "reset.h"


#define SS_PCLK_DIV		(100 - 1)

#define TRG_ATTR					(0)
//trigger unit 1us
#define TU(bus_clk)			(bus_clk / (SS_PCLK_DIV + 1) / 1000)
#define GLOBAL_TRIG_CLK(bus_clk)		(bus_clk)

#define ss_2280_global						(0)
#define ss_2280_hw_snr						(1)
#define ss_2280_hw_flood					(2)
#define ss_2280_hw_dot						(3)


#define SS_2280_OFFSET(n)				((n) * 0x1000)
#define SS_2280_GLOBAL_EN(n)			((n-1) * 4)
struct snr_sync_hw {
	void __iomem *base;
	int ss0_irq;
	int ss1_irq;
	int ss2_irq;
	
	int busclk;
	struct clk *clk_ss;
	struct clk *clk_gt;

	struct reset_control *snr_sync;
	struct reset_control *global_trig;

	int ss0_gpio;
	int ss1_gpio;
	int ss2_gpio;
	int ext_gpio;

	int pin_mux_mode;

	int channel_bit;		//supported channel
	int trig_mode;			//supported mode, hw_auto/hw_single/sw_single
	struct trigger_timing tm;
};


static int ss_2280_power(void *ctrl, int onoff);
//static int ss_2280_reset(void *ctrl);
static int ss_2280_enable(void *ctrl, int chan, int onoff);
static int ss_2280_config(void *ctrl, int chan,
		struct trigger_period *pd);
static int ss_2280_valid_sync(void *ctrl, int chan);
//static int ss_2280_update(void *ctrl);
static int ss_2280_chan_mode(void *ctrl, int chan, int mode);
void ss_2280_probe(void);


#endif
