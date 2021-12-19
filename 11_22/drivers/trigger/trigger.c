#if defined(CHIP_IMI_2280)
#include "trigger.h"
#include "elog.h"

static LIST_HEAD(trigger_list);
static FRTOS_mutex trigger_mutex;

struct trigger_handle * trigger_register(struct trigger_dev_ops *ops)
{
	struct trigger_handle *handle = 0;
	handle = pvPortMalloc(sizeof(*handle));
	memset(handle, 0, sizeof(*handle));
	handle->ops = ops;
	FRTOS_mutex_lock(trigger_mutex);
	list_add_tail(&handle->list, &trigger_list);
	FRTOS_mutex_unlock(trigger_mutex);
	return handle;
}
EXPORT_SYMBOL(trigger_register);

struct trigger_handle *trigger_get_handle(char *name) {
	struct trigger_handle *handle = 0;
	FRTOS_mutex_lock(trigger_mutex);
	if (!list_empty(&trigger_list)) {
		list_for_each_entry(handle, &trigger_list, list) {
			if (!strcmp(handle->name, name)) {
				FRTOS_mutex_unlock(trigger_mutex);
				return handle;
			}
		}
	}
	FRTOS_mutex_unlock(trigger_mutex);
	log_e("%s dev %s not registered\n", __func__, name);
	return 0;
}
EXPORT_SYMBOL(trigger_get_handle);

int trigger_power(struct trigger_handle *trig, int onoff)
{
	if (trig->ops->power) {
		return trig->ops->power(trig->priv, onoff);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(trigger_power);

int trigger_reset(struct trigger_handle *trig)
{
	if (trig->ops->reset) {
		return trig->ops->reset(trig->priv);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(trigger_reset);

int trigger_enable(struct trigger_handle *trig, int channel,
		int onoff)
{
	if (trig->ops->enable) {
		return trig->ops->enable(trig->priv, channel, onoff);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(trigger_enable);

int trigger_init(struct trigger_handle *trig, struct trigger_timing *tm)
{
	if (trig->ops->init) {
		return trig->ops->init(trig->priv, (void*)tm);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(trigger_init);

int trigger_config(struct trigger_handle *trig, int channel, void* cfg)
{
	struct trigger_period *pd = (struct trigger_period *)cfg;
	if (trig->ops->config) {
		return trig->ops->config(trig->priv, channel, pd);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(trigger_config);

int trigger_valid_sync(struct trigger_handle *trig, int chan, int highlow)
{
	if (trig->ops->valid_sync) {
		return trig->ops->valid_sync(trig->priv, chan, highlow);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(trigger_valid_sync);

int trigger_mode_set(struct trigger_handle *trig, int channel, int mode)
{
	if (trig->ops->chan_mode) {
		return trig->ops->chan_mode(trig->priv, channel, mode);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(trigger_mode_set);

int trigger_timer_sync(struct trigger_handle *trig, int mode, int stop)
{
	if (trig->ops->timer_sync) {
		return trig->ops->timer_sync(trig->priv, mode, stop);
	} else {
		return 0;
	}
}
EXPORT_SYMBOL(trigger_timer_sync);

int  dpu_register_trigger(struct trigger_handle **trigger,
		struct trigger_timing *tt)
{
	struct trigger_handle *handle = 0;
	handle = trigger_get_handle("ss_2280");
	if (handle) {
		*trigger = handle;
		handle->timing = tt;
		trigger_init(handle, tt);
		log_i("%s done\n", __func__);
		return 0;
	} else {
		log_i("%s failed\n", __func__);
		return -EBADF;
	}
}
EXPORT_SYMBOL(dpu_register_trigger);

void trigger_sys_init(void)
{
	FRTOS_mutex_create(&trigger_mutex);
	log_i("%s done\n", __func__);
}
SUBSYS_INIT(trigger_sys_init);
#endif/*CHIP_IMI_2280*/
