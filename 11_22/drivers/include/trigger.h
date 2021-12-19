#ifndef __TRIGGER_H__
#define __TRIGGER_H__
#include "linux_common.h"

enum trigger_channel {
	TRIG_CHAN_IR = 0,
	TRIG_CHAN_FLOOD,
	TRIG_CHAN_DOT,
	TRIG_CHAN_COLOR,
	TRIG_CHAN_MAX,
};

enum trigger_mode {
//hardware auto set all path trigger signal	
	TRIG_MODE_AUTO = 0,
//user set a ext trigger signal, all path trigger generate once
	TRIG_MODE_SINGLE,
//user/timer set seperate path trigger once
	TRIG_MODE_SW_SEPERATE,
	TRIG_MODE_MAX
};

//suntongce
struct trigger_path{
	char freq;				//max 255Hz is enough
	int gpio_out;			//gpio pin out when software mode
	int offset;				//us from trig_en to pulse become active
	int pulse_w;			//us when pulse active
};

struct trigger_pd {
	signed int delay;
	signed int pulse;
};
struct trigger_period {
//	int mode;
//	signed int freq_hz;	
	struct trigger_pd T0;
	struct trigger_pd T1;
	struct trigger_pd T2;
	struct trigger_pd T3;
};

struct trigger_timing {
	int freq;		
	int trig_mode;			//supported mode, hw_auto/hw_single/sw_single
	struct trigger_period path[TRIG_CHAN_MAX];
};
	
struct trigger_dev_ops {
	int (*power)(void *ctrl, int onff);
	int (*reset)(void *ctrl);
	int (*init)(void *ctrl, void *data);
    int (*enable)(void *ctrl, int chan, int onoff);
    int (*config)(void *ctrl, int chan, struct trigger_period *pd);
    int (*update)(void *ctrl);
	int (*valid_sync)(void *ctrl, int chan, int highlow);
	int (*chan_mode)(void *ctrl, int chan, int mode);
	int (*timer_sync)(void *ctrl, int mode, int stop);
};

struct trigger_handle {
	struct device dev;
	struct trigger_dev_ops *ops;
	struct trigger_timing *timing;
	FRTOS_mutex lock;
	struct list_head list;
	char *name;
	void *priv;
};

#if 0
#if defined(CHIP_IMI_2280)
#include "sensor_sync_2280.h"
#elif defined(CHIP_IMI_1680)
#include "sensor_sync_1680.h"
#endif
#endif
int  dpu_register_trigger(struct trigger_handle **trigger,
		struct trigger_timing *tt);
struct trigger_handle * trigger_register(struct trigger_dev_ops *ops);
struct trigger_handle *trigger_get_handle(char *name);
int trigger_power(struct trigger_handle *trig, int onoff);
int trigger_reset(struct trigger_handle *trig);
int trigger_enable(struct trigger_handle *trig, int channel, int onoff);
int trigger_init(struct trigger_handle *trig, struct trigger_timing *tm);
int trigger_config(struct trigger_handle *trig, int channel, void* cfg);
int trigger_update(struct trigger_handle *trig, int channel);
int trigger_valid_sync(struct trigger_handle *trig, int chan, int highlow);
int trigger_mode_set(struct trigger_handle *trig, int channel, int mode);
int trigger_timer_sync(struct trigger_handle *trig, int mode, int stop);
#endif/*__TRIGGER_H__*/
