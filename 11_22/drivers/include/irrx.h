#ifndef __IRRX_H__
#define __IRRX_H__
#include "linux_common.h"
#define IRRX_PARA_CMD_SET		(0x0)
#define IRRX_PARA_CMD_GET		(0x80000000)

enum irrx_setting_grp {
	IRRX_SETTING_GRP_CACHED = 0,
	IRRX_SETTING_GRP_IR,
	IRRX_SETTING_GRP_DEPTH,
	IRRX_SETTING_GRP_MAX,
};

enum irrx_para_set {
	IRRX_PARA_SET_POWER = 0,
	IRRX_PARA_RESET,
	IRRX_PARA_SET_REG = 10,
	IRRX_PARA_GET_REG,
	IRRX_PARA_GET_ID = 20,
	IRRX_PARA_GET_MAXWINDOW,
	IRRX_PARA_SET_WINDOW = 30,
	IRRX_PARA_FILL_TIMING,
	//exposure/dgain/again/hts/vts/mipimbps/sysclk...
	IRRX_PARA_SET_MIRROR_FLIP,
	IRRX_PARA_SET_ANALOG_GAIN,
	IRRX_PARA_SET_DIGITAL_GAIN,
	IRRX_PARA_SET_EXPOSURE,
	IRRX_PARA_SET_HTS,
	IRRX_PARA_SET_VTS,
	IRRX_PARA_GET_TIMING,
	IRRX_PARA_SET_WORKMODE,
	IRRX_PARA_SET_DBG = 40,
	IRRX_PARA_CMD_MAX,
};

struct frm_res {
	short in_w;
	short in_h;
	short out_w;
	short out_h;
};

enum irrx_frm_res {
	IRRX_FRM_GRP_IR = 0,
	IRRX_FRM_GRP_DEPTH = 0,
	IRRX_FRM_GRP_MAX = 0,
};

struct irrx_setting {
//	char fps;
	int mirror_flip;
	int analog_gain;
	int rotate;
	int digital_gain;
	int width;
	int height;
	int mipi_mbps;
	int exposure;
	int sysclk;
	int hts;
	int vts;
};

struct irrx_hw_info {
	int chip_id;
	int max_width;
	int max_height;
};

struct irrx_window_info {
	short w;
	short h;
	short off_x;
	short off_y;
};

struct irrx_dev_ops {
	int (*power)(void *ctrl, int onoff);
	int (*reset)(void *ctrl);
	int (*init)(void *ctrl, int w, int h);
	int (*set_reg)(void *ctrl, int reg, int val);
	int (*get_reg)(void *ctrl, int reg, int *val);
	int (*set_para)(void *ctrl, int para, void *data);
	int (*get_para)(void *ctrl, int para, void *data);
};


struct irrx_config {
	struct frm_res res;
	struct irrx_setting setting;
};


struct irrx_handle{
	struct device *dev;

	struct irrx_hw_info info;
	struct irrx_hw_info setval;
	//	struct vi_domain *handle;
	void *priv;
	struct irrx_config cfg;
	struct irrx_dev_ops *ops;
	struct list_head list;
	char *name;
};

int irrx_power(struct irrx_handle *irrx, int onoff);
int irrx_reset(struct irrx_handle *irrx);
int irrx_init(struct irrx_handle *irrx, int w, int h);
int irrx_set_reg(struct irrx_handle *irrx, int reg, int val);
int irrx_get_reg(struct irrx_handle *irrx, int reg, int *val);
int irrx_set_window(struct irrx_handle *irrx, int wh, int offset);
int irrx_set_timing(struct irrx_handle *irrx, struct irrx_setting *setting);
int irrx_set_mirrorflip(struct irrx_handle *irrx, void *mirror);
int irrx_set_ag(struct irrx_handle *irrx, void *gain);
int irrx_set_dg(struct irrx_handle *irrx, void *dg);
int irrx_set_exp(struct irrx_handle *irrx, void *exp);
int irrx_set_hts(struct irrx_handle *irrx, void *hts);
int irrx_set_vts(struct irrx_handle *irrx, void *vts);
int irrx_get_timing(struct irrx_handle *irrx, void *timing);
int irrx_get_id(struct irrx_handle *irrx, int *id);
int irrx_get_maxwin(struct irrx_handle *irrx, int *w, int *h);
int irrx_set_stream(struct irrx_handle *irrx, int stream);
int irrx_set_dbg(struct irrx_handle *irrx, void *dbg);
struct irrx_handle * irrx_register(struct irrx_dev_ops *ops);
struct irrx_handle *irrx_get_handle(char *name);
int dpu_register_irrx(struct irrx_handle **irrx, struct irrx_config *cfg);
#endif /*__IRRX_H__*/
