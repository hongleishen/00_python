#include "reset.h"
#include "linux_common.h"

static DEFINE_MUTEX(reset_list_mutex);
static LIST_HEAD(reset_controller_list);


//reset controller device API
int reset_controller_register(struct reset_controller_dev *rcdev)
{
	INIT_LIST_HEAD(&rcdev->reset_control_head);

	mutex_lock(&reset_list_mutex);
	list_add(&rcdev->list, &reset_controller_list);
	mutex_unlock(&reset_list_mutex);
	return 0;
}
EXPORT_SYMBOL(reset_controller_register);

void reset_controller_unregister(struct reset_controller_dev *rcdev)
{
	mutex_lock(&reset_list_mutex);
	list_del(&rcdev->list);
	mutex_unlock(&reset_list_mutex);
}
EXPORT_SYMBOL(reset_controller_unregister);

int reset_control_reset(struct reset_control *rstc)
{
	int ret;

	if (!rstc){
		configASSERT(0);
		log_e("%s %d\n", __func__, __LINE__);
		return -EFAULT;
	}
#if 0
	if (WARN_ON(IS_ERR(rstc)))
		return -EINVAL;
#endif

	if (!rstc->rcdev->ops->reset)
		return -ENOTSUPP;

	ret = rstc->rcdev->ops->reset(rstc->rcdev, rstc->id);
	return ret;
}
EXPORT_SYMBOL(reset_control_reset);


//hw device reset ops API
int reset_control_assert(struct reset_control *rstc)
{
	if (!rstc){
		configASSERT(0);
		log_e("%s %d\n", __func__, __LINE__);
		return -EFAULT;
	}
#if 0
	if (WARN_ON(IS_ERR(rstc)))
		return -EINVAL;
#endif

	if (!rstc->rcdev->ops->rassert)
		return -ENOTSUPP;

	return rstc->rcdev->ops->rassert(rstc->rcdev, rstc->id);
}
EXPORT_SYMBOL(reset_control_assert);
int reset_control_deassert(struct reset_control *rstc)
{
	if (!rstc){
		//		configASSERT(0);
		log_i("%s %d\n", __func__, __LINE__);
		return -ENODEV;
	}
#if 0
	if (WARN_ON(IS_ERR(rstc)))
		return -EINVAL;
#endif
	if (!rstc->rcdev->ops->deassert)
		return 0;

	return rstc->rcdev->ops->deassert(rstc->rcdev, rstc->id);
}
EXPORT_SYMBOL(reset_control_deassert);

int reset_control_status(struct reset_control *rstc)
{
	if (!rstc){
		configASSERT(0);
		log_e("%s %d\n", __func__, __LINE__);
		return -EFAULT;
	}
#if 0
	if (WARN_ON(IS_ERR(rstc)) || reset_control_is_array(rstc))
		return -EINVAL;
#endif
	if (rstc->rcdev->ops->status)
		return rstc->rcdev->ops->status(rstc->rcdev, rstc->id);

	return -ENOTSUPP;
}
EXPORT_SYMBOL(reset_control_status);

#if 0
int device_reset(struct device *dev)
{
	struct reset_control *rstc;
	int ret;

	rstc = reset_control_get(dev, NULL);
	if (IS_ERR(rstc))
		return PTR_ERR(rstc);

	ret = reset_control_reset(rstc);

	reset_control_put(rstc);

	return ret;
}
EXPORT_SYMBOL_GPL(device_reset);
#endif

//hw reset control acquire
int reset_control_get(const char *rcd, int id,
		struct reset_control **rc)
{
	struct reset_control *rstc;
	struct reset_controller_dev *r, *rcdev;
	//	int rstc_id;
	if (id == 255) {
		return 0;
	}
	*rc = 0;
	mutex_lock(&reset_list_mutex);
	rcdev = NULL;
	list_for_each_entry(r, &reset_controller_list, list) {
		if (!strcmp(rcd, &r->rcname[0])) {
			rcdev = r;
			break;
		}
	}

	if (!rcdev) {
		mutex_unlock(&reset_list_mutex);
		return (-EPROBE_DEFER);
	}
#if 0
	rstc_id = rcdev->id_xlate(rcdev, id);
	if (rstc_id < 0) {
		mutex_unlock(&reset_list_mutex);
		return (rstc_id);
	}
#endif


	list_for_each_entry(rstc, &rcdev->reset_control_head, list) {
		if (rstc->id == id) {
			log_e("%s rc %d already in use\n", __func__, id);
			return (-EBUSY);
		}
	}

	rstc = pvPortMalloc(sizeof(*rstc));
	if (!rstc)
		return (-ENOMEM);

	rstc->rcdev = rcdev;
	list_add(&rstc->list, &rcdev->reset_control_head);
	rstc->id = id;

	mutex_unlock(&reset_list_mutex);
	*rc =rstc;
	log_i("%s\n", __func__);
	return 0;
}
EXPORT_SYMBOL_GPL(reset_control_get);

void reset_probe(void)
{
	mutex_init(&reset_list_mutex);
}
SUBSYS_INIT(reset_probe);
