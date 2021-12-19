#ifndef ___VCSEL_DRV_H___
#define ___VCSEL_DRV_H___
#include "linux_common.h"

enum vcsel_type {
	VCSEL_TYPE_FLOOD = 0,
	VCSEL_TYPE_DOT,
	VCSEL_TYPE_MAX,
};

enum vcsel_chan {
	VCSEL_CHAN_0 = 1,
	VCSEL_CHAN_1 = 2,
	VCSEL_CHAN_ALL = 3,
};


enum vcsel_setting_grp {
	VCSEL_SETTING_GRP_CACHED = 0,
	VCSEL_SETTING_GRP_SETVAL,
	VCSEL_SETTING_GRP_MAX,
};
	
enum vcsel_para_set {
	VCSEL_PARA_SET_POWER = 0,
	VCSEL_PARA_RESET,
	VCSEL_PARA_SET_REG,
	VCSEL_PARA_GET_REG,
	VCSEL_PARA_SET_CURRENT,
	VCSEL_PARA_SET_CHANNEL,
};

#if 0
struct vcsel_para {
	char reset_pin;
	char channel;
};
#endif

struct vcsel_setting {
	short cur;		//current mA
};

struct vcsel_ops {
    int (*power)(void *ctrl, int onoff);
    int (*reset)(void *ctrl);
    int (*set_current)(void *ctrl, int current);
    int (*set_channel)(void *ctrl, enum vcsel_chan chan);
    int (*set_reg)(void *ctrl, int reg, int val);
    int (*get_reg)(void *ctrl, int reg, int *val);
};
struct vcsel_handle {
	enum vcsel_type type;
	struct vcsel_ops *ops;
	struct list_head list;
	struct vcsel_setting setting;
	void *priv;
};

struct vcsel_handle *vcsel_register(enum vcsel_type type, struct vcsel_ops *ops);
struct vcsel_handle *vcsel_get_handle(enum vcsel_type type);
int vcsel_power(struct vcsel_handle *handle, int onoff);
int vcsel_reset(struct vcsel_handle *handle);
int vcsel_set_reg(struct vcsel_handle *handle, int reg, int val);
int vcsel_get_reg(struct vcsel_handle *handle, int reg, int *val);
int vcsel_set_current(struct vcsel_handle *handle, int current);
int vcsel_set_channel(struct vcsel_handle *handle, int chan);
void vcsel_sys_init(void);
int  dpu_register_vcsel(struct vcsel_handle **vcsel,
			struct vcsel_setting *vs, enum vcsel_type type);

#endif/*___VCSEL_DRV_H___*/
