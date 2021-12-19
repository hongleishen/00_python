/*******************************************************************************
  Copyright (c) 2020 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        i2c private .h
  @creator      Qianyu Liu

  @History
      When          Who             What, where, why
      ----------    ------------    ----------------------------------------
      2020/10/09    Qianyu Liu      the initial version
*******************************************************************************/

#ifndef __I2C_PRIV_H__
#define __I2C_PRIV_H__
#include "delay.h"

/*flags for the client struct: */
#define I2C_CLIENT_PEC			0x04	/* Use Packet Error Checking */
#define I2C_CLIENT_TEN			0x10	/* we have a ten bit chip address */
/* Must equal I2C_M_TEN below */
#define I2C_CLIENT_SLAVE		0x20	/* we are the slave */
#define I2C_CLIENT_HOST_NOTIFY	0x40	/* We want to use I2C host notify */
#define I2C_CLIENT_WAKE			0x80	/* for board_info; true iff can wake */
#define I2C_CLIENT_SCCB			0x9000	/* Use Omnivision SCCB protocol */

/* i2c adapter classes (bitmask) */
#define I2C_CLASS_HWMON			(1<<0)	/* lm_sensors, ... */
#define I2C_CLASS_DDC			(1<<3)	/* DDC bus on graphics adapters */
#define I2C_CLASS_SPD			(1<<7)	/* Memory modules */
/* Warn users that the adapter doesn't support classes anymore */
#define I2C_CLASS_DEPRECATED	(1<<8)

/* Internal numbers to terminate lists */
#define I2C_CLIENT_END			(0xfffeU)


#define __iomem //__attribute__((noderef, address_space(2)))
#define ___constant_swab32(x) ((__u32)(                         \
		(((__u32)(x) & (__u32)0x000000ffUL) << 24) |            \
		(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |            \
		(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |            \
		(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#define BITS_PER_LONG 32

/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_ULL(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#define GENMASK(h, l) \
		(((~0UL) - (1UL << (l)) + 1) & (~0UL >> (BITS_PER_LONG - 1 - (h))))


#define readw_relaxed read16
#define readl_relaxed readl
#define writew_relaxed write16
#define writel_relaxed writel

#define usleep udelay
#define usleep_range(a, b) udelay(b)

static inline unsigned long find_next_bit(const unsigned long *p, int size,
		int offset)
{
	for (; offset < size; offset++)
		if (*p & (1 << offset))
			break;
	return offset;
}

#define find_first_bit(addr, size) find_next_bit(addr, size, 0)

#define for_each_set_bit(bit, addr, size) \
	for ((bit) = find_first_bit((addr), (size));		\
	     (bit) < (size);					\
	     (bit) = find_next_bit((addr), (size), (bit) + 1))

#endif

