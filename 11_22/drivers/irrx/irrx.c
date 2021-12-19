#include "ov928x.h"
#include "i2c.h"
#include "linux_common.h"
#include "imi_config.h"
#include "gpio.h"
#include "irrx.h"

static FRTOS_mutex irrx_mutex;
static LIST_HEAD(irrx_list);

//suntongce warning
#if defined(CHIP_IMI_1680)
int init_mipi_itf_ir(void *data)
{
	return 0;
}
#endif

int irrx_power(struct irrx_handle *irrx, int onoff)
{
	int ret = 0;
	if (onoff) {
		ret |= init_mipi_itf_ir((void *)&irrx->cfg);
	}
	if (irrx->ops->power) {
		ret |= irrx->ops->power(irrx->priv, onoff);
	} else {
		return ret;
	}
	return ret;
}
EXPORT_SYMBOL(irrx_power);

int irrx_reset(struct irrx_handle *irrx)
{
	if (irrx->ops->reset) {
		return irrx->ops->reset(irrx->priv);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_reset);

int irrx_init(struct irrx_handle *irrx, int w, int h)
{
	if (irrx->ops->init) {
		return irrx->ops->init(irrx->priv, w, h);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_reset);

int irrx_set_reg(struct irrx_handle *irrx, int reg, int val)
{
	if (irrx->ops->set_reg) {
		return irrx->ops->set_reg(irrx->priv, reg, val);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_reg);

int irrx_get_reg(struct irrx_handle *irrx, int reg, int *val)
{
	if (irrx->ops->get_reg) {
		return irrx->ops->get_reg(irrx->priv, reg, val);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_get_reg);

int irrx_set_window(struct irrx_handle *irrx, int wh, int offset)
{
	struct irrx_window_info info;
	memset(&info, 0, sizeof(info));
	info.w = (wh & 0xFFFF0000) >> 16;
	info.h = (wh & 0xFFFF);
	info.off_x = (offset & 0xFFFF0000) >> 16;
	info.off_y = (offset & 0xFFFF);
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_WINDOW, &info);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_window);
int irrx_set_timing(struct irrx_handle *irrx, struct irrx_setting *setting)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_FILL_TIMING, setting);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_timing);

int irrx_set_mirrorflip(struct irrx_handle *irrx, void *mirror)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_MIRROR_FLIP, mirror);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_mirrorflip);

int irrx_set_ag(struct irrx_handle *irrx, void *gain)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_ANALOG_GAIN, gain);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_ag);

int irrx_set_dg(struct irrx_handle *irrx, void *dg)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_DIGITAL_GAIN, dg);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_dg);

int irrx_set_exp(struct irrx_handle *irrx, void *exp)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_EXPOSURE, exp);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_exp);

int irrx_set_hts(struct irrx_handle *irrx, void *hts)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_HTS, hts);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_hts);

int irrx_set_vts(struct irrx_handle *irrx, void *vts)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_VTS, vts);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_vts);

int irrx_get_timing(struct irrx_handle *irrx, void *timing)
{
	if (irrx->ops->get_para) {
		return irrx->ops->get_para(irrx->priv,
				IRRX_PARA_CMD_GET | IRRX_PARA_GET_TIMING, timing);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_timing);

int irrx_get_id(struct irrx_handle *irrx, int *id)
{
	if (irrx->ops->get_para) {
		return irrx->ops->get_para(irrx->priv,
				IRRX_PARA_CMD_GET | IRRX_PARA_GET_ID, id);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_get_id);

int irrx_get_maxwin(struct irrx_handle *irrx, int *w, int *h)
{
	struct irrx_hw_info info;
	int ret = 0;
	memset(&info, 0, sizeof(info));
	if (irrx->ops->get_para) {
		ret = irrx->ops->get_para(irrx->priv,
				IRRX_PARA_CMD_GET | IRRX_PARA_GET_MAXWINDOW, &info);
		if (ret) {
			log_e("%s %d failed\n", __func__, __LINE__);
			return ret;
		}
		*w = info.max_width;
		*h = info.max_height;
		return 0;
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_get_maxwin);

int irrx_set_stream(struct irrx_handle *irrx, int stream)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_WORKMODE, &stream);
	} else {
		return 0;
	}
}

EXPORT_SYMBOL(irrx_set_stream);

int irrx_set_dbg(struct irrx_handle *irrx, void *dbg)
{
	if (irrx->ops->set_para) {
		return irrx->ops->set_para(irrx->priv,
				IRRX_PARA_CMD_SET | IRRX_PARA_SET_DBG, dbg);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(irrx_set_dbg);

struct irrx_handle * irrx_register(struct irrx_dev_ops *ops)
{
	struct irrx_handle *handle = 0;
	handle = pvPortMalloc(sizeof(*handle));
	memset(handle, 0, sizeof(*handle));
	handle->ops = ops;
	FRTOS_mutex_lock(irrx_mutex);
	list_add_tail(&handle->list, &irrx_list);
	FRTOS_mutex_unlock(irrx_mutex);
	return handle;
}
EXPORT_SYMBOL(irrx_register);

struct irrx_handle *irrx_get_handle(char *name)
{
	struct irrx_handle *handle = 0;
	FRTOS_mutex_lock(irrx_mutex);
	if (!list_empty(&irrx_list)) {
		list_for_each_entry(handle, &irrx_list, list) {
			if (!strcmp(handle->name, name)) {
				FRTOS_mutex_unlock(irrx_mutex);
				return handle;
			}
		}
	}
	FRTOS_mutex_unlock(irrx_mutex);
	log_e("%s dev %s not registered\n", __func__, name);
	return 0;
}
EXPORT_SYMBOL(irrx_get_handle);

int dpu_register_irrx(struct irrx_handle **irrx, struct irrx_config *cfg)
{
	struct irrx_handle *handle;
	handle = irrx_get_handle("ov928x");
	memcpy(&handle->cfg, cfg , sizeof(struct irrx_config));
	*irrx = handle;
	return 0;
}
EXPORT_SYMBOL(dpu_register_irrx);

void irrx_sys_init(void)
{
	FRTOS_mutex_create(&irrx_mutex);
	log_i("%s done\n", __func__);
}
SUBSYS_INIT(irrx_sys_init);

