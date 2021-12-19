/*******************************************************************************
  Copyright (c) 2020 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        dw_i2c dma driver
  @creator      Qianyu Liu

  @History
      When          Who             What, where, why
      ----------    ------------    ----------------------------------------
      2020/10/21    Qianyu Liu      the initial version
*******************************************************************************/

#ifdef __cpluscplus
extern "C" {
#endif

#include "imi_io.h"
#include "i2c-designware-core.h"
#include "elog.h"

/*
 * i2c_dw_dma_en
 *
 * @dma
 * @mode
 *
 * @return: i2c fifo register, 0 if failed
*/
int i2c_dw_dma_en(void *dma, i2c_opt_mode_u8 mode)
{
	struct dw_i2c_dev *dev = dma;
	int en = (mode >> DW_IC_DMA_SHIFT) & 0x3;

	if (!REG_READ_MASK(i2c_dw_read_comp_param(dev), IC_COMP_PARAM_1_HAS_DMA)) {
		log_e("not support dma\n");
		return 0;//EOPNOTSUPP
	}

	if (DW_IC_SLAVE == dev->mode) {
		log_e("slave not support dma %d\n", mode);
		return 0;//EOPNOTSUPP
	}

	dw_writel(dev, en, DW_IC_DMA_CR);
	dw_writel(dev, 1, DW_IC_DMA_TDLR);
	dw_writel(dev, 0, DW_IC_DMA_RDLR);

	if (mode | DW_IC_DMA)
		dev->mode = mode;
	else
		dev->mode = DW_IC_MASTER;

//	log_d("i2c%d dma en=%d\n", dev->i2c_id, en);

	return (int)(dev->base + DW_IC_DATA_CMD);
}

#ifdef __cpluscplus
}
#endif

