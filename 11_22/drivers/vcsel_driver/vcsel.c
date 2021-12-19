#include "vcsel.h"

static LIST_HEAD(vcsel_list);
static FRTOS_mutex vcsel_mutex;

struct vcsel_handle *vcsel_register(enum vcsel_type type,
		struct vcsel_ops *ops) {
	struct vcsel_handle *handle = 0;
	handle = pvPortMalloc(sizeof(*handle));
	memset(handle, 0, sizeof(*handle));
	handle->ops = ops;
	handle->type = type;
	FRTOS_mutex_lock(vcsel_mutex);
	list_add_tail(&handle->list, &vcsel_list);
	FRTOS_mutex_unlock(vcsel_mutex);
	return handle;
}
EXPORT_SYMBOL(vcsel_register);

struct vcsel_handle *vcsel_get_handle(enum vcsel_type type) {
	struct vcsel_handle *handle = 0;
	FRTOS_mutex_lock(vcsel_mutex);
	if (!list_empty(&vcsel_list)) {
		list_for_each_entry(handle, &vcsel_list, list) {
			if (handle->type == type) {
				FRTOS_mutex_unlock(vcsel_mutex);
				return handle;
			}
		}
	}
	log_e("%s type %d not registered\n", __func__, type);
	FRTOS_mutex_unlock(vcsel_mutex);
	return 0;
}
EXPORT_SYMBOL(vcsel_get_handle);

int vcsel_power(struct vcsel_handle *handle, int onoff)
{
	if (handle->ops->power) {
		return handle->ops->power(handle->priv, onoff);
	}
	return 0;
}
EXPORT_SYMBOL(vcsel_power);

int vcsel_reset(struct vcsel_handle *handle)
{
	if (handle->ops->reset) {
		handle->ops->reset(handle);
	}
	return 0;
}
EXPORT_SYMBOL(vcsel_reset);

int vcsel_set_reg(struct vcsel_handle *handle, int reg, int val)
{
	if (handle->ops->set_reg) {
		return handle->ops->set_reg(handle->priv, reg, val);
	}
	return 0;
}
EXPORT_SYMBOL(vcsel_set_reg);

int vcsel_get_reg(struct vcsel_handle *handle, int reg, int *val)
{
	if (handle->ops->get_reg) {
		return handle->ops->get_reg(handle->priv, reg, val);
	}
	return 0;
}
EXPORT_SYMBOL(vcsel_get_reg);

int vcsel_set_current(struct vcsel_handle *handle, int current)
{
	if (handle->ops->set_current) {
		return handle->ops->set_current(handle->priv, current);
	}
	return 0;
}
EXPORT_SYMBOL(vcsel_set_current);

int vcsel_set_channel(struct vcsel_handle *handle, int chan)
{
	if (handle->ops->set_channel) {
		return handle->ops->set_channel(handle->priv, chan);
	}
	return 0;
}
EXPORT_SYMBOL(vcsel_set_channel);

int  dpu_register_vcsel(struct vcsel_handle **vcsel,
			struct vcsel_setting *vs, enum vcsel_type type)
{
	*vcsel = vcsel_get_handle(type);
	(*vcsel)->setting.cur = vs->cur;
	return 0;
}
EXPORT_SYMBOL(dpu_register_vcsel);


void vcsel_sys_init(void)
{
	FRTOS_mutex_create(&vcsel_mutex);
	log_i("%s done\n", __func__);
}
EXPORT_SYMBOL(vcsel_sys_init);

SUBSYS_INIT(vcsel_sys_init);
