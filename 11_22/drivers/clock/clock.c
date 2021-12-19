/*******************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        clock driver

  @History
  When          Who             What, where, why
  ----------    ------------    ----------------------------------------

 *******************************************************************************/
 #if 0
#define LOG_TAG		"clock"
#include "clock.h"
#include "string.h"
#include "dev.h"
#include "elog_init.h"
#include "imi_type.h"
#include "initcalls.h"
#include "clock.h"

struct clk {
    struct clk_core *core;
    const char *dev_id;
    const char *con_id;
    unsigned long min_rate;
    unsigned long max_rate;
    struct hlist_node clks_node;
};

struct clk_core {
    const char      *name;
    const struct clk_ops    *ops;
    struct clk_hw       *hw;
//    struct module       *owner;
    struct clk_core     *parent;
    const char      **parent_names;
    struct clk_core     **parents;
    u8          num_parents;
    u8          new_parent_index;
    unsigned long       rate;
    unsigned long       req_rate;
    unsigned long       new_rate;
    struct clk_core     *new_parent;
    struct clk_core     *new_child;
    unsigned long       flags;
    bool            orphan;
    unsigned int        enable_count;
    unsigned int        prepare_count;
    unsigned long       min_rate;
    unsigned long       max_rate;
    unsigned long       accuracy;
    int         phase;
    struct hlist_head   children;
    struct hlist_node   child_node;
    struct hlist_head   clks;
};

struct clk_hw {
    struct clk_core *core;
    struct clk *clk;
//    const struct clk_init_data *init;
};

struct clk_ops {
	int 	(*prepare)(struct clk_hw *hw);
	void		(*unprepare)(struct clk_hw *hw);
	int 	(*is_prepared)(struct clk_hw *hw);
	void		(*unprepare_unused)(struct clk_hw *hw);
	int 	(*enable)(struct clk_hw *hw);
	void		(*disable)(struct clk_hw *hw);
	int 	(*is_enabled)(struct clk_hw *hw);
	void		(*disable_unused)(struct clk_hw *hw);
	unsigned long	(*recalc_rate)(struct clk_hw *hw,
					unsigned long parent_rate);
	long		(*round_rate)(struct clk_hw *hw, unsigned long rate,
					unsigned long *parent_rate);
	int 	(*determine_rate)(struct clk_hw *hw,
					  struct clk_rate_request *req);
	int 	(*set_parent)(struct clk_hw *hw, u8 index);
	u8		(*get_parent)(struct clk_hw *hw);
	int 	(*set_rate)(struct clk_hw *hw, unsigned long rate,
					unsigned long parent_rate);
	int 	(*set_rate_and_parent)(struct clk_hw *hw,
					unsigned long rate,
					unsigned long parent_rate, u8 index);
	unsigned long	(*recalc_accuracy)(struct clk_hw *hw,
					   unsigned long parent_accuracy);
	int 	(*get_phase)(struct clk_hw *hw);
	int 	(*set_phase)(struct clk_hw *hw, int degrees);
	void		(*init)(struct clk_hw *hw);
	int 	(*debug_init)(struct clk_hw *hw, struct dentry *dentry);
};

struct clk *clk_register(struct device *dev, struct clk_hw *hw)
{
    int i, ret;
    struct clk_core *core;

    core = kzalloc(sizeof(*core), GFP_KERNEL);
    if (!core) {
        ret = -ENOMEM;
        goto fail_out;
    }

    core->name = kstrdup_const(hw->init->name, GFP_KERNEL);
    if (!core->name) {
        ret = -ENOMEM;
        goto fail_name;
    }
    core->ops = hw->init->ops;
    if (dev && dev->driver)
        core->owner = dev->driver->owner;
    core->hw = hw;
    core->flags = hw->init->flags;
    core->num_parents = hw->init->num_parents;
    core->min_rate = 0;
    core->max_rate = ULONG_MAX;
    hw->core = core;

    /* allocate local copy in case parent_names is __initdata */
    core->parent_names = kcalloc(core->num_parents, sizeof(char *),
                    GFP_KERNEL);

    if (!core->parent_names) {
        ret = -ENOMEM;
        goto fail_parent_names;
    }


    /* copy each string name in case parent_names is __initdata */
    for (i = 0; i < core->num_parents; i++) {
        core->parent_names[i] = kstrdup_const(hw->init->parent_names[i],
                        GFP_KERNEL);
        if (!core->parent_names[i]) {
            ret = -ENOMEM;
            goto fail_parent_names_copy;
        }
    }

    /* avoid unnecessary string look-ups of clk_core's possible parents. */
    core->parents = kcalloc(core->num_parents, sizeof(*core->parents),
                GFP_KERNEL);
    if (!core->parents) {
        ret = -ENOMEM;
        goto fail_parents;
    };

    INIT_HLIST_HEAD(&core->clks);

    hw->clk = __clk_create_clk(hw, NULL, NULL);
    if (IS_ERR(hw->clk)) {
				ret = PTR_ERR(hw->clk);
				goto fail_parents;
			}
		
			ret = __clk_core_init(core);
			if (!ret)
				return hw->clk;
		
			__clk_free_clk(hw->clk);
			hw->clk = NULL;
		
		fail_parents:
			kfree(core->parents);
		fail_parent_names_copy:
			while (--i >= 0)
				kfree_const(core->parent_names[i]);
			kfree(core->parent_names);
		fail_parent_names:
			kfree_const(core->name);
		fail_name:
			kfree(core);
		fail_out:
			return ERR_PTR(ret);
		}
		EXPORT_SYMBOL(clk_register);


		bool clk_is_match(const struct clk *p, const struct clk *q)
		{
			/* trivial case: identical struct clk's or both NULL */
			if (p == q)
				return true;
		
			/* true if clk->core pointers match. Avoid dereferencing garbage */
			if (!IS_ERR_OR_NULL(p) && !IS_ERR_OR_NULL(q))
				if (p->core == q->core)
					return true;
		
			return false;
		}
		EXPORT_SYMBOL_GPL(clk_is_match);
int clk_set_phase(struct clk *clk, int degrees)
{
	return 0;
}
EXPORT_SYMBOL_GPL(clk_set_phase);		

int clk_set_parent(struct clk *clk, struct clk *parent)
{
    if (!clk)
        return 0;

    return clk_core_set_parent(clk->core, parent ? parent->core : NULL);
}
EXPORT_SYMBOL_GPL(clk_set_parent);
bool clk_has_parent(struct clk *clk, struct clk *parent)
{
    struct clk_core *core, *parent_core;
    unsigned int i;

    /* NULL clocks should be nops, so return success if either is NULL. */
    if (!clk || !parent)
        return true;

    core = clk->core;
    parent_core = parent->core;

    /* Optimize for the case where the parent is already the parent. */
    if (core->parent == parent_core)
        return true;

    for (i = 0; i < core->num_parents; i++)
        if (strcmp(core->parent_names[i], parent_core->name) == 0)
            return true;

    return false;
}
EXPORT_SYMBOL_GPL(clk_has_parent);
struct clk *clk_get_parent(struct clk *clk)
{
    struct clk *parent;

    if (!clk)
        return NULL;

    clk_prepare_lock();
    /* TODO: Create a per-user clk and change callers to call clk_put */
    parent = !clk->core->parent ? NULL : clk->core->parent->hw->clk;
    clk_prepare_unlock();

    return parent;
}
EXPORT_SYMBOL_GPL(clk_get_parent);
int clk_set_rate_range(struct clk *clk, unsigned long min, unsigned long max)
{
    int ret = 0;

    if (!clk)
        return 0;

    if (min > max) {
        pr_err("%s: clk %s dev %s con %s: invalid range [%lu, %lu]\n",
               __func__, clk->core->name, clk->dev_id, clk->con_id,
               min, max);
        return -EINVAL;
    }

    clk_prepare_lock();

    if (min != clk->min_rate || max != clk->max_rate) {
        clk->min_rate = min;
        clk->max_rate = max;
        ret = clk_core_set_rate_nolock(clk->core, clk->core->req_rate);
    }

    clk_prepare_unlock();

    return ret;
}
EXPORT_SYMBOL_GPL(clk_set_rate_range);

int clk_set_rate(struct clk *clk, unsigned long rate)
{
    int ret;

    if (!clk)
        return 0;

    /* prevent racing with updates to the clock topology */
    clk_prepare_lock();

    ret = clk_core_set_rate_nolock(clk->core, rate);

    clk_prepare_unlock();

    return ret;
}
EXPORT_SYMBOL_GPL(clk_set_rate);
unsigned long clk_get_rate(struct clk *clk)
{
    if (!clk)
        return 0;

    return clk_core_get_rate(clk->core);
}
EXPORT_SYMBOL_GPL(clk_get_rate);
#if 0
long clk_round_rate(struct clk *clk, unsigned long rate)
{

}
EXPORT_SYMBOL_GPL(clk_round_rate);
#endif

int clk_enable(struct clk *clk)
{
	if (!clk)
		return 0;

	return clk_core_enable_lock(clk->core);
}
EXPORT_SYMBOL_GPL(clk_enable);

void clk_disable(struct clk *clk)
{
    if (IS_ERR_OR_NULL(clk))
        return;

    clk_core_disable_lock(clk->core);
}
EXPORT_SYMBOL_GPL(clk_disable);

int clk_prepare(struct clk *clk)
{
    if (!clk)
        return 0;

    return clk_core_prepare_lock(clk->core);
}
EXPORT_SYMBOL_GPL(clk_prepare);

void clk_unprepare(struct clk *clk)
{
    if (IS_ERR_OR_NULL(clk))
        return;

    clk_core_unprepare_lock(clk->core);
}
EXPORT_SYMBOL_GPL(clk_unprepare);


void clock_set(u32 adress)
{
}

unsigned int clock_enable(char* name)
{
	return 0;
}

void clock_config( CLKCFG clkcfg )
{
}

unsigned int clock_module_set(unsigned int addr, unsigned int value, unsigned int source)
{
	return 0;
}


void clock_dev_init(void)
{

}

CORE_INIT(clock_dev_init);
#endif
