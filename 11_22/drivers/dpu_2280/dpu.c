#include "dpu.h"

static LIST_HEAD(dpu_list);
static FRTOS_mutex dpu_mutex;


/*later improvement :
	ops replaced with ioctl method
	dev get from device struct other than get() API
*/
struct dpu_handle * dpu_register(struct dpu_dev_ops *ops)
{
	struct dpu_handle *handle = 0;
	handle = pvPortMalloc(sizeof(*handle));
	memset(handle, 0, sizeof(*handle));
	handle->ops = ops;
	FRTOS_mutex_lock(dpu_mutex);
	list_add_tail(&handle->list, &dpu_list);
	FRTOS_mutex_unlock(dpu_mutex);
	return handle;
}
EXPORT_SYMBOL(dpu_register);

struct dpu_handle *dpu_get_handle(char *name) {
	struct dpu_handle *handle = 0;
	FRTOS_mutex_lock(dpu_mutex);
	if (!list_empty(&dpu_list)) {
		list_for_each_entry(handle, &dpu_list, list) {
			if (!strcmp(handle->name, name)) {
				FRTOS_mutex_unlock(dpu_mutex);
				return handle;
			}
		}
	}
	log_e("%s dev %s not registered\n", __func__, name);
	FRTOS_mutex_unlock(dpu_mutex);
	return 0;
}
EXPORT_SYMBOL(dpu_get_handle);

int dpu_power(struct dpu_handle *handle, int onoff)
{
	if (handle->ops->power) {
		return handle->ops->power(handle->priv, onoff);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_power);

int dpu_reset(struct dpu_handle *handle)
{
	if (handle->ops->reset) {
		return handle->ops->reset(handle->priv);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_reset);

int dpu_update_dma_buf(struct dpu_handle *handle, int addr, int len, int path)
{
	if (handle->ops->update_dma_buf) {
		return handle->ops->update_dma_buf(handle->priv, addr, len, path);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_update_dma_buf);

int dpu_update_dma_size(struct dpu_handle *handle, int w, int h, int path)
{
	if (handle->ops->update_dma_size) {
		return handle->ops->update_dma_size(handle->priv, w, h, path);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_update_dma_size);

//get last hw irqed dma buffer addr
int dpu_get_dma_buf(struct dpu_handle *handle, int path, int *addr, int *sz)
{
	if (handle->ops->get_dma_buf) {
		return handle->ops->get_dma_buf(handle->priv, path, addr, sz);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_get_dma_buf);

int dpu_dma_enable(struct dpu_handle *handle, int path, int en)
{
	if (handle->ops->dma_enable) {
		return handle->ops->dma_enable(handle->priv, path, en);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_dma_enable);

int dpu_register_isr_cb(struct dpu_handle *handle, int path, dpu_isr_cb cb)
{
	if (handle->ops->register_isr_cb) {
		return handle->ops->register_isr_cb(handle->priv, path, cb);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_register_isr_cb);

int dpu_set_reg(struct dpu_handle *handle, int reg, int val)
{
	if (handle->ops->set_reg) {
		return handle->ops->set_reg(handle->priv, reg, val);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_set_reg);

int dpu_get_reg(struct dpu_handle *handle, int reg, int *val)
{
	if (handle->ops->get_reg) {
		return handle->ops->get_reg(handle->priv, reg, val);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_get_reg);

int dpu_set_para(struct dpu_handle *handle, int para, void *data)
{
	if (handle->ops->set_para) {
		return handle->ops->set_para(handle->priv, para, data);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_set_para);

int dpu_get_para(struct dpu_handle *handle, int para, void *data)
{
	if (handle->ops->get_para) {
		return handle->ops->get_para(handle->priv, para, data);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(dpu_get_para);

int dpu_register_depth(struct dpu_handle **handle, struct dpu_parameter *para)
{
	struct dpu_handle *dh;
	int ret = 0;
	dh = dpu_get_handle("dpu_2280");
	if (!dh) {
		log_e("%s %d failed\n", __func__, __LINE__);
		return -ENODEV;
	}
	ret = dpu_set_para(dh, DPU_PARA_CMD_SET_INIT, (void*)para);
	if (ret) {
		log_e("%s %d failed return %d\n", __func__, __LINE__, ret);
		return ret;
	}
	*handle = dh;
	return ret;
}
EXPORT_SYMBOL(dpu_register_depth);

void dpu_sys_init(void)
{
	FRTOS_mutex_create(&dpu_mutex);
	log_i("%s done\n", __func__);
}
SUBSYS_INIT(dpu_sys_init);
