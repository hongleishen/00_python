#ifndef __RESET_H__
#define __RESET_H__
#include "linux_common.h"
#if defined(CHIP_IMI_2280)
#include "reset_imi2280.h"
#elif defined(CHIP_IMI_1680)
#include "reset_imi1680.h"
#endif

struct reset_controller_dev {
    const struct reset_control_ops *ops;
	struct list_head list;
    struct list_head reset_control_head;
    unsigned int nr_resets;
	char rcname[8];
};

struct reset_control_ops {
	int (*reset)(struct reset_controller_dev *rcdev, unsigned long id);
	int (*rassert)(struct reset_controller_dev *rcdev, unsigned long id);
	int (*deassert)(struct reset_controller_dev *rcdev, unsigned long id);
	int (*status)(struct reset_controller_dev *rcdev, unsigned long id);
};

struct reset_control {
    struct reset_controller_dev *rcdev;
    struct list_head list;
    unsigned int id;
//    bool shared;		//not support now
//    bool array;		//not support now
//    atomic_t deassert_count;		//not support now
//    atomic_t triggered_count;		//not support now
};

int reset_control_reset(struct reset_control *rstc);
int reset_control_assert(struct reset_control *rstc);
int reset_control_deassert(struct reset_control *rstc);
int reset_control_status(struct reset_control *rstc);

//reset controller device API
int reset_controller_register(struct reset_controller_dev *rcdev);
void reset_controller_unregister(struct reset_controller_dev *rcdev);
int reset_control_get(const char *rcd, int id, struct reset_control **rc);


//suntongce warning , linux-compat
inline struct reset_control *devm_reset_control_get(
                struct device *dev, const char *id)
{
    return 0;
}

#endif/*__RESET_H__*/
