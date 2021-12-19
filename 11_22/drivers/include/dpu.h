#ifndef __DPU_H__
#define __DPU_H__
#include "linux_common.h"
#if defined(CHIP_IMI_2280)
#elif defined(CHIP_IMI_1680)
#endif
#define USER_DEFINE_SENSOR_FRAME_LEN_MS	(10)  //warning,  this value should be passed from dpu_cfg.ini

typedef int (*dpu_isr_cb)(void *, int *, int, int);


#define DPU_PARA_CMD_SET		(0x0)
#define DPU_PARA_CMD_GET		(0x80000000)

enum dpu_para_cmd {
	DPU_PARA_CMD_SET_INIT = 0,
	DPU_PARA_CMD_GET_INIT,
	DPU_PARA_CMD_LOAD_PARA = 10,
	DPU_PARA_CMD_DEV_INIT = 20,
	DPU_PARA_CMD_BUF_INIT = 30,
	DPU_PARA_CMD_MAX
};

enum dpu_para_type {
	DPU_PARA_TYPE_REF = 0,
	DPU_PARA_TYPE_REF2,
	DPU_PARA_TYPE_INIT,
	DPU_PARA_TYPE_SHIFT,
	DPU_PARA_TYPE_DYRAM,
	DPU_PARA_TYPE_MAX,
};

enum dpu_dma_path {
	DPU_DMA_PATH_IR = 0,
	DPU_DMA_PATH_DEPTH,
	DPU_DMA_PATH_MAX,
};

struct dpu_depth_setting {
    int op;
    int reg;
    int val;
    int delay;
};

struct dpu_para_init_seq {
	struct dpu_depth_setting *init_seq_context;
	int init_seq_size;
};

enum dpu_ref_img_grp {
	DPU_REF_IMG_GRP_0 = 0,
	DPU_REF_IMG_GRP_1,
	DPU_REF_IMG_GRP_MAX,
};

struct dpu_para_ref_img {
	int *ref_img;
	int ref_img_size;
};

struct dpu_para_shift2depth {
	int *s2d;
	int s2d_size;
};

struct dpu_para_dyram {
	int *dyram;
	int dyram_sz;
};

struct dpu_para_geo {
	int le_calc_step_x;
	int le_calc_step_y;
	int l1_para_line;
	int sadw;
};

#define MAX_DMA_BUFFER		(3)
struct dpu_buf_prop {
	char buf_num;
	char rotate;
	char bits_per_pixel;
	short in_w;
	short in_h;
	short out_w;
	short out_h;
	int buf_addr[DPU_DMA_PATH_MAX][MAX_DMA_BUFFER];		
};

struct dpu_parameter {
	struct dpu_para_init_seq initseq;
	struct dpu_para_ref_img ref[DPU_REF_IMG_GRP_MAX];
	struct dpu_para_shift2depth s2d;
	struct dpu_para_dyram dy;
	struct dpu_para_geo geo;
};

struct dpu_para_file {
	char name[32];
};

struct dpu_parameter_cfg {
	struct dpu_para_file path[DPU_PARA_TYPE_MAX];
	struct dpu_para_file ini;
};

struct dpu_dev_ops {
	int (*power)(void *ctrl, int onoff);
	int (*reset)(void *ctrl);
	int (*update_dma_buf)(void *ctrl, int addr, int len, int path);
	int (*update_dma_size)(void *ctrl, int w, int h, int path);
	int (*get_dma_buf)(void *ctrl, int path, int *addr, int *sz);
	int (*dma_enable)(void *ctrl, int path, int en);
	int (*register_isr_cb)(void *ctrl, int path, dpu_isr_cb cb);
	int (*set_reg)(void *ctrl, int reg, int val);
	int (*get_reg)(void *ctrl, int reg, int *val);
	int (*set_para)(void *ctrl, int para, void *data);
	int (*get_para)(void *ctrl, int para, void *data);
};


struct irae_val{
    int irae_cur;
    int irae_exp;
    int irae_ag;
    int irae_dg;
};

struct dpu_dbg {
    int light_switch;
    int trig_stop;
    int ext_trig_ctrl;
    int le_enable;
    int le_cnt;

    spinlock_t trig_lock;
    int dpu_only;
    int sensor_only;
    int usb_sim;
//    struct light_only light;
    struct irae_val irae_val;
};

struct dpu_handle {
	struct dpu_dev_ops *ops;
	struct list_head list;
	struct dpu_parameter *para;
	void *priv;
	char *name;
};

struct dpu_handle * dpu_register(struct dpu_dev_ops *ops);
struct dpu_handle *dpu_get_handle(char *name);
int dpu_power(struct dpu_handle *handle, int onoff);
int dpu_reset(struct dpu_handle *handle);
int dpu_update_dma_buf(struct dpu_handle *handle, int addr, int len, int path);
int dpu_update_dma_size(struct dpu_handle *handle, int w, int h, int path);
int dpu_get_dma_buf(struct dpu_handle *handle, int path, int *addr, int *sz);
int dpu_dma_enable(struct dpu_handle *handle, int path, int en);
int dpu_register_isr_cb(struct dpu_handle *handle, int path, dpu_isr_cb cb);
int dpu_set_reg(struct dpu_handle *handle, int reg, int val);
int dpu_get_reg(struct dpu_handle *handle, int reg, int *val);
int dpu_set_para(struct dpu_handle *handle, int para, void *data);
int dpu_get_para(struct dpu_handle *handle, int para, void *data);
int dpu_register_depth(struct dpu_handle **handle, struct dpu_parameter *para);
#endif /*__DPU_H__*/
