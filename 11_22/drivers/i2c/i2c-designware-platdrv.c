#include "i2c_plat.h"
#include "i2c-designware-core.h"
#include "elog.h"
#include "irqs.h"


struct dw_i2c_ctrl_para {
	char id;
	char irq;
	char slavemode;
	char reset_id;
	void __iomem * base;
	int i2c_scl_freq;
	int sda_hold_time;		//ns
	int sda_falling_time;	//ns
	int scl_falling_time;	//ns
	u16 hcnt;
	u16 lcnt;
};

static u32 i2c_dw_get_clk_rate_khz(struct dw_i2c_dev *dev)
{
	return dev->clk_freq;
}
static void i2c_dw_configure_master(struct dw_i2c_dev *dev)
{
	dev->functionality = I2C_FUNC_10BIT_ADDR | DW_IC_DEFAULT_FUNCTIONALITY;

	dev->master_cfg = DW_IC_CON_MASTER | DW_IC_CON_SLAVE_DISABLE |
			  DW_IC_CON_RESTART_EN;

	dev->mode = DW_IC_MASTER;

	switch (dev->clk_freq) {
	case 100000:
		dev->master_cfg |= DW_IC_CON_SPEED_STD;
		break;
	case 3400000:
		dev->master_cfg |= DW_IC_CON_SPEED_HIGH;
		break;
	default:
		dev->master_cfg |= DW_IC_CON_SPEED_FAST;
	}
}

static void i2c_dw_configure_slave(struct dw_i2c_dev *dev)
{
	dev->functionality = I2C_FUNC_SLAVE | DW_IC_DEFAULT_FUNCTIONALITY;

	dev->slave_cfg = DW_IC_CON_RX_FIFO_FULL_HLD_CTRL |
			 DW_IC_CON_RESTART_EN | DW_IC_CON_STOP_DET_IFADDRESSED;

	dev->mode = DW_IC_SLAVE;

	switch (dev->clk_freq) {
	case 100000:
		dev->slave_cfg |= DW_IC_CON_SPEED_STD;
		break;
	case 3400000:
		dev->slave_cfg |= DW_IC_CON_SPEED_HIGH;
		break;
	default:
		dev->slave_cfg |= DW_IC_CON_SPEED_FAST;
	}
}

#if x
static int i2c_dw_plat_prepare_clk(struct dw_i2c_dev *i_dev, bool prepare)
{
	if (IS_ERR(i_dev->clk))
		return PTR_ERR(i_dev->clk);

	if (prepare)
		return clk_prepare_enable(i_dev->clk);

	clk_disable_unprepare(i_dev->clk);
	return 0;
}
#endif	

static void dw_i2c_set_fifo_size(struct dw_i2c_dev *dev, int id)
{
	u32 param, tx_fifo_depth, rx_fifo_depth;

	/*
	 * Try to detect the FIFO depth if not set by interface driver,
	 * the depth could be from 2 to 256 from HW spec.
	 */
	param = i2c_dw_read_comp_param(dev);
	tx_fifo_depth = ((param >> 16) & 0xff) + 1;
	rx_fifo_depth = ((param >> 8)  & 0xff) + 1;
	if (!dev->tx_fifo_depth) {
		dev->tx_fifo_depth = tx_fifo_depth;
		dev->rx_fifo_depth = rx_fifo_depth;
		dev->adapter.nr = id;
	} else if (tx_fifo_depth >= 2) {
		dev->tx_fifo_depth = min_t(u32, dev->tx_fifo_depth,
				tx_fifo_depth);
		dev->rx_fifo_depth = min_t(u32, dev->rx_fifo_depth,
				rx_fifo_depth);
	}
}

int dw_i2c_plat_probe(struct dw_i2c_ctrl_para *para)
{
	struct i2c_adapter *adap;
	struct dw_i2c_dev *dev;
	int ret;

	dev = pvPortMalloc(sizeof(struct dw_i2c_dev));
	if (!dev) {
		log_e("%s malloc fail ,para %p\n", __func__, para);
		return -ENOMEM;
	}
	memset(dev, 0, sizeof(*dev));

	dev->base = para->base;
	if (IS_ERR(dev->base))
		return PTR_ERR(dev->base);

	dev->irq = para->irq;
	dev->mode = para->slavemode;

	ret = reset_control_get("reset", para->reset_id, &dev->rst);
	if (ret) {
		ret = -EPROBE_DEFER;
		goto probe_defer;
	} else {
		reset_control_deassert(dev->rst);
	}

	dev->clk_freq = para->i2c_scl_freq;
	dev->sda_hold_time = para->sda_hold_time;
	dev->sda_falling_time = para->sda_falling_time;
	dev->scl_falling_time = para->scl_falling_time;
	if (para->i2c_scl_freq <= 100000) {
		//standard speed
		dev->ss_hcnt = para->hcnt;
		dev->ss_lcnt = para->lcnt;
	} else if(para->i2c_scl_freq <= 400000) {
		//fast speed
		dev->fs_hcnt = para->hcnt;
		dev->fs_lcnt = para->lcnt;
	} else if(para->i2c_scl_freq <= 1000000) {
		//fast mode plus speed
		dev->fp_hcnt = para->hcnt;
		dev->fp_lcnt = para->lcnt;
	} else {
		//high speed  3.4M
		dev->hs_hcnt = para->hcnt;
		dev->hs_lcnt = para->lcnt;
	}

	if (!dev->clk_freq)
		dev->clk_freq = 400000;

	/*
	 * Only standard mode at 100kHz, fast mode at 400kHz,
	 * fast mode plus at 1MHz and high speed mode at 3.4MHz are supported.
	 */
	if (dev->clk_freq != 100000 && dev->clk_freq != 400000
	    && dev->clk_freq != 1000000 && dev->clk_freq != 3400000) {
		dev_err(&dev->dev,
			"%d Hz is unsupported, only 100kHz, 400kHz, 1MHz and 3.4MHz are supported\n",
			dev->clk_freq);
		ret = -EINVAL;
		goto exit_reset;
	}

	if (dev->mode) {
		i2c_dw_configure_slave(dev);
	} else {
		i2c_dw_configure_master(dev);
	}
	dev->get_clk_rate_khz = i2c_dw_get_clk_rate_khz;

	dw_i2c_set_fifo_size(dev, para->id);

	adap = &dev->adapter;
	adap->nr = para->id;

	if (dev->mode == DW_IC_SLAVE)
		ret = i2c_dw_probe_slave(dev);
	else
		ret = i2c_dw_probe(dev);

	if (ret)
		goto exit_probe;

	return ret;


exit_probe:
exit_reset:
	if (dev->rst)
		reset_control_assert(dev->rst);
	probe_defer:
	vPortFree(dev);
	dev = 0;
	return ret;
}
EXPORT_SYMBOL(dw_i2c_plat_probe);

int dw_i2c_plat_remove(struct dw_i2c_dev *dev)
{
	i2c_del_adapter(&dev->adapter);

	dev->disable(dev);

	if (dev->rst > 0) {
		reset_control_assert(dev->rst);
	}
	return 0;
}
EXPORT_SYMBOL(dw_i2c_plat_remove);
#if defined(CHIP_IMI_2280)
#include "hellboy_map.h"
void imi_2280_i2c_controller_init(void)
{
	struct dw_i2c_ctrl_para para;
	memset(&para, 0, sizeof(para));
	para.id = 0;
	para.base = (void __iomem*)I2C0_BASE_ADDR;
	para.irq = IRQ_U0_I2C;
	para.slavemode = DW_IC_MASTER;
	para.i2c_scl_freq = 400000;
	para.reset_id = 0xFF;
//	para.hcnt = ;
//	para.lcnt = ;
	dw_i2c_plat_probe(&para);

	memset(&para, 0, sizeof(para));
	para.id = 1;
	para.base = (void __iomem*)I2C1_BASE_ADDR;
	para.irq = IRQ_U1_I2C;
	para.slavemode = DW_IC_MASTER;
	para.i2c_scl_freq = 400000;
	para.reset_id = 0xFF;
	//	para.hcnt = ;
	//	para.lcnt = ;
	dw_i2c_plat_probe(&para);

	memset(&para, 0, sizeof(para));
	para.id = 2;
	para.base = (void __iomem*)I2C2_BASE_ADDR;
	para.irq = IRQ_U2_I2C;
	para.slavemode = DW_IC_MASTER;
	para.i2c_scl_freq = 400000;
	para.reset_id = 0xFF;
	//	para.hcnt = ;
	//	para.lcnt = ;
	dw_i2c_plat_probe(&para);
}
ARCH_INIT(imi_2280_i2c_controller_init);
#endif/*CHIP_IMI_2280*/
