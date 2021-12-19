/*******************************************************************************
  Copyright (c) 2020 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        i2c slave driver interface .h
  @creator      Qianyu Liu

  @History
      When          Who             What, where, why
      ----------    ------------    ----------------------------------------
      2020/10/27    Qianyu Liu      the initial version
*******************************************************************************/

#ifndef __I2C_PLAT_SLAVE_H__
#define __I2C_PLAT_SLAVE_H__
#include "i2c.h"

#define REG_LEN I2C_SIZE_32BIT

struct eeprom_data {
	spinlock_t buffer_lock;
	//struct bin_attribute bin;
	bool first_write;
	u8 buffer_idx;
	u8 send_idx;
	//u8 reserve[2];
	u8 buffer[REG_LEN << 2];
	u32 val;
};

static int i2c_dw_reg_slave(struct i2c_client *slave);
static int i2c_dw_unreg_slave(struct i2c_client *slave);
int i2c_slave_register(struct i2c_client *client,
		i2c_slave_cb_t slave_cb);
int i2c_slave_unregister(struct i2c_client *client);
struct i2c_client *i2c_slave_enable(struct dw_i2c_dev *i2c_controller);
//struct i2c_client *i2c_slave_enable(void *i2c_controller);


#endif

