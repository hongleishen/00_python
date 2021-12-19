#if defined(CHIP_IMI_2280)
#include "reset.h"
#include "linux_common.h"
#include "hellboy_map.h"
#include "initcalls.h"

struct imi2280_reset_data {
	struct reset_controller_dev rcdev;
	void __iomem *sbase;	//Set
	void __iomem *cbase;	//Clear
	void __iomem *tbase;	//sTatus
	spinlock_t lock;
};

#define to_imi2280_reset_data(_rst) \
	container_of(_rst, struct imi2280_reset_data, rcdev)

static int imi2280_reset_assert(struct reset_controller_dev *rcdev,
				   unsigned long idx)
{
	struct imi2280_reset_data *ird = to_imi2280_reset_data(rcdev);
	u32 bank = idx / BITS_PER_LONG;
	u32 offset = idx % BITS_PER_LONG;
	unsigned long flags;
	void __iomem *reg;
	u32 val;

	spin_lock_irqsave(&ird->lock, flags);

	reg = ird->sbase + bank * sizeof(u32);
	val = readl(reg);
	val |= BIT(offset);
	writel(val, reg);

	val &= ~BIT(offset);
	writel(val, reg);

	spin_unlock_irqrestore(&ird->lock, flags);

	imi_printf("%s idx %d, reg %x val %x\n", __func__, idx, reg, val);
	return 0;
}

static int imi2280_reset_deassert(struct reset_controller_dev *rcdev,
				 unsigned long idx)
{
	struct imi2280_reset_data *ird = to_imi2280_reset_data(rcdev);
	u32 bank = idx / BITS_PER_LONG;
	u32 offset = idx % BITS_PER_LONG;
	unsigned long flags;
	void __iomem *reg;
	u32 val;

	spin_lock_irqsave(&ird->lock, flags);

	reg = ird->cbase + bank * sizeof(u32);
	val = readl(reg);
	val |= BIT(offset);
	writel(val, reg);

	val &= ~BIT(offset);
	writel(val, reg);

	spin_unlock_irqrestore(&ird->lock, flags);

	imi_printf("%s idx %d, reg %x val %x\n", __func__, idx, reg, val);
	return 0;
}

static int imi2280_reset_dev(struct reset_controller_dev *rcdev,
				unsigned long idx)
{
	int ret;

	ret = imi2280_reset_assert(rcdev, idx);
	if (ret)
		return ret;

	return imi2280_reset_deassert(rcdev, idx);
}

static struct reset_control_ops imi2280_reset_ops = {
	.reset	  = imi2280_reset_dev,
	.rassert   = imi2280_reset_assert,
	.deassert = imi2280_reset_deassert,
};

void imi2280_reset_probe(void)
{
	struct imi2280_reset_data *ird;
	imi_printf("%s \n", __func__);

	ird = pvPortMalloc(sizeof(*ird));
	if (!ird) {
		log_e("%s malloc failed\n", __func__);
		return;
	}

	ird->sbase = (void __iomem *)(RST_GEN_BASE_ADDR);
	ird->cbase = (void __iomem *)(RST_GEN_BASE_ADDR + 0x100);
	ird->tbase = (void __iomem *)(RST_GEN_BASE_ADDR + 0x200);
	spin_lock_init(&ird->lock);

	ird->rcdev.ops = &imi2280_reset_ops;
	memcpy(ird->rcdev.rcname, reset_name, strlen(reset_name));
	reset_controller_register(&ird->rcdev);
	return;
}

CORE_INIT(imi2280_reset_probe);
#endif/*CHIP_IMI_2280*/
