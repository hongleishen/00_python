#if defined(CHIP_IMI_1680)
#include "reset.h"
#include "linux_common.h"

struct imi1680_reset_data {
	struct reset_controller_dev rcdev;
	void __iomem *sbase;	//Set
	void __iomem *cbase;	//Clear
	void __iomem *tbase;	//sTatus
	spinlock_t lock;
};

#define to_imi1680_reset_data(_rst) \
	container_of(_rst, struct imi1680_reset_data, rcdev)

static int imi1680_reset_assert(struct reset_controller_dev *rcdev,
				   unsigned long idx)
{
	return 0;
}


static int imi1680_reset_deassert(struct reset_controller_dev *rcdev,
				 unsigned long idx)
{
   return 0;
}


static int imi1680_reset_dev(struct reset_controller_dev *rcdev,
				unsigned long idx)
{
	int ret;

	ret = imi1680_reset_assert(rcdev, idx);
	if (ret)
		return ret;

	return imi1680_reset_deassert(rcdev, idx);
}

static struct reset_control_ops imi1680_reset_ops = {
	.reset	  = imi1680_reset_dev,
	.rassert   = imi1680_reset_assert,
	.deassert = imi1680_reset_deassert,
};

void imi1680_reset_probe(void)
{
	struct imi1680_reset_data *ird;
	log_i("start\n");

	ird = pvPortMalloc(sizeof(*ird));
	if (!ird) {
		log_e("%s malloc failed\n", __func__);
		return;
	}

//	ird->sbase = (void __iomem *)(RST_GEN_BASE_ADDR);
//	ird->cbase = (void __iomem *)(RST_GEN_BASE_ADDR + 0x100);
//	ird->tbase = (void __iomem *)(RST_GEN_BASE_ADDR + 0x200);
//	spin_lock_init(&ird->lock);

	ird->rcdev.ops = &imi1680_reset_ops;
	//suntongce warning  using fake to make gcc happy
//	memcpy(ird->rcdev.rcname, reset_name, strlen(reset_name));
	memcpy(ird->rcdev.rcname, "fake", strlen("fake"));
	reset_controller_register(&ird->rcdev);
	log_i("done\n");
	return;
}


CORE_INIT(imi1680_reset_probe);
#endif/*CHIP_IMI_1680*/
