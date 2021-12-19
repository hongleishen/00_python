/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */
/* ------------------------------------------------------------------------- */
/*									     */
/* i2c.h - definitions for the i2c-bus interface			     */
/*									     */
/* ------------------------------------------------------------------------- */
/*   Copyright (C) 1995-2000 Simon G. Vogl

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.							     */
/* ------------------------------------------------------------------------- */

/* With some changes from Kyösti Mälkki <kmalkki@cc.hut.fi> and
   Frodo Looijaard <frodol@dds.nl> */

#ifndef __I2C_H__
#define __I2C_H__

#include "imi_type.h"
#include "linux_compat.h"
#include "reset.h"

#define I2C_NAME_SIZE   20
struct i2c_client;
enum I2C_SPD_E {
	I2C_SPD_100K,
	I2C_SPD_400K,
	I2C_SPD_1M,
	I2C_SPD_3P4M,
	I2C_SPD_BUTT
};

enum i2c_id_enum {
	I2C_0 = 0,
	I2C_1,
	I2C_2,
	I2C_3,
	I2C_4,
};
typedef enum i2c_id_enum I2C_ENUM;

enum I2C_ADDR_M_E {
	I2C_ADDR_M_7BIT,
	I2C_ADDR_M_10BIT,
	I2C_ADDR_M_BUTT,
};
typedef u8 i2c_addr_m_u8;

enum I2C_SIZE_E {
	I2C_SIZE_8BIT  = 1,
	I2C_SIZE_16BIT = 2,
	I2C_SIZE_32BIT = 4,
	I2C_SIZE_BUTT,
};
typedef u8 i2c_size_u8;

enum i2c_slave_event {
	I2C_SLAVE_READ_REQUESTED,
	I2C_SLAVE_WRITE_REQUESTED,
	I2C_SLAVE_READ_PROCESSED,
	I2C_SLAVE_WRITE_RECEIVED,
	I2C_SLAVE_STOP,
};

typedef int (*i2c_slave_cb_t)(struct i2c_client *, enum i2c_slave_event, u8 *);

struct i2c_client {
    unsigned short flags;       /* div., see below      */
    unsigned short addr;        /* chip address - NOTE: 7bit    */
                    /* addresses are stored in the  */
                    /* _LOWER_ 7 bits       */
    char name[I2C_NAME_SIZE];
    struct i2c_adapter *adapter;    /* the adapter we sit on    */
    struct device dev;      /* the device structure     */
    int irq;            /* irq issued by device     */
    i2c_slave_cb_t slave_cb;    /* callback for slave mode  */
	struct list_head to_adap;		//list to adapter list
};

extern int i2c_slave_register(struct i2c_client *client, i2c_slave_cb_t slave_cb);
extern int i2c_slave_unregister(struct i2c_client *client);

static inline int i2c_slave_event(struct i2c_client *client,
				  enum i2c_slave_event event, u8 *val)
{
	return client->slave_cb(client, event, val);
}


#define I2C_SMBUS_BLOCK_MAX 32  /* As specified in SMBus standard */

struct i2c_msg {
    u16 addr; /* slave address            */
    u16 flags;
#define I2C_M_RD        0x0001  /* read data, from slave to master */
                    /* I2C_M_RD is guaranteed to be 0x0001! */
#define I2C_M_TEN       0x0010  /* this is a ten bit chip address */
#define I2C_M_RECV_LEN      0x0400  /* length will be first received byte */
#define I2C_M_NO_RD_ACK     0x0800  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_IGNORE_NAK    0x1000  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_REV_DIR_ADDR  0x2000  /* if I2C_FUNC_PROTOCOL_MANGLING */
#define I2C_M_NOSTART       0x4000  /* if I2C_FUNC_NOSTART */
#define I2C_M_STOP      0x8000  /* if I2C_FUNC_PROTOCOL_MANGLING */
    u16 len;      /* msg length               */
    u8 *buf;      /* pointer to msg data          */
};
#define I2C_FUNC_I2C            0x00000001
#define I2C_FUNC_10BIT_ADDR     0x00000002
#define I2C_FUNC_PROTOCOL_MANGLING  0x00000004 /* I2C_M_IGNORE_NAK etc. */
#define I2C_FUNC_SMBUS_PEC      0x00000008
#define I2C_FUNC_NOSTART        0x00000010 /* I2C_M_NOSTART */
#define I2C_FUNC_SLAVE          0x00000020
#define I2C_FUNC_SMBUS_BLOCK_PROC_CALL  0x00008000 /* SMBus 2.0 */
#define I2C_FUNC_SMBUS_QUICK        0x00010000
#define I2C_FUNC_SMBUS_READ_BYTE    0x00020000
#define I2C_FUNC_SMBUS_WRITE_BYTE   0x00040000
#define I2C_FUNC_SMBUS_READ_BYTE_DATA   0x00080000
#define I2C_FUNC_SMBUS_WRITE_BYTE_DATA  0x00100000
#define I2C_FUNC_SMBUS_READ_WORD_DATA   0x00200000
#define I2C_FUNC_SMBUS_WRITE_WORD_DATA  0x00400000
#define I2C_FUNC_SMBUS_PROC_CALL    0x00800000
#define I2C_FUNC_SMBUS_READ_BLOCK_DATA  0x01000000
#define I2C_FUNC_SMBUS_WRITE_BLOCK_DATA 0x02000000
#define I2C_FUNC_SMBUS_READ_I2C_BLOCK   0x04000000 /* I2C-like block xfer  */
#define I2C_FUNC_SMBUS_WRITE_I2C_BLOCK  0x08000000 /* w/ 1-byte reg. addr. */
#define I2C_FUNC_SMBUS_HOST_NOTIFY  0x10000000

#define I2C_FUNC_SMBUS_BYTE     (I2C_FUNC_SMBUS_READ_BYTE | \
                     I2C_FUNC_SMBUS_WRITE_BYTE)
#define I2C_FUNC_SMBUS_BYTE_DATA    (I2C_FUNC_SMBUS_READ_BYTE_DATA | \
                     I2C_FUNC_SMBUS_WRITE_BYTE_DATA)
#define I2C_FUNC_SMBUS_WORD_DATA    (I2C_FUNC_SMBUS_READ_WORD_DATA | \
                     I2C_FUNC_SMBUS_WRITE_WORD_DATA)
#define I2C_FUNC_SMBUS_BLOCK_DATA   (I2C_FUNC_SMBUS_READ_BLOCK_DATA | \
                     I2C_FUNC_SMBUS_WRITE_BLOCK_DATA)
#define I2C_FUNC_SMBUS_I2C_BLOCK    (I2C_FUNC_SMBUS_READ_I2C_BLOCK | \
                     I2C_FUNC_SMBUS_WRITE_I2C_BLOCK)
#define I2C_FUNC_SMBUS_EMUL     (I2C_FUNC_SMBUS_QUICK | \
						 I2C_FUNC_SMBUS_BYTE | \
						 I2C_FUNC_SMBUS_BYTE_DATA | \
						 I2C_FUNC_SMBUS_WORD_DATA | \
						 I2C_FUNC_SMBUS_PROC_CALL | \
						 I2C_FUNC_SMBUS_WRITE_BLOCK_DATA | \
						 I2C_FUNC_SMBUS_I2C_BLOCK | \
						 I2C_FUNC_SMBUS_PEC)
						 

/* operation modes */
enum DW_IC_MODE_E {
	DW_IC_MASTER,
	DW_IC_SLAVE,
	DW_IC_DMA_SHIFT,
	DW_IC_DMA_R = 4,
	DW_IC_DMA_W = 8,
	DW_IC_DMA = DW_IC_DMA_R | DW_IC_DMA_W,
	DW_IC_BUTT
};

typedef u8 i2c_spd_u8;
typedef u8 i2c_opt_mode_u8;

#define I2C_INITIALIZED    1
#define I2C_UNINITIALIZED  0


struct i2c_algorithm {
    int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs,
               int num);
//    int (*smbus_xfer) (struct i2c_adapter *adap, u16 addr,
//               unsigned short flags, char read_write,
//               u8 command, int size, union i2c_smbus_data *data);

    /* To determine what the adapter supports */
    u32 (*functionality) (struct i2c_adapter *);

#if 1//defined(CONFIG_I2C_SLAVE)
    int (*reg_slave)(struct i2c_client *client);
    int (*unreg_slave)(struct i2c_client *client);
#endif
};

struct i2c_lock_operations {
    void (*lock_bus)(struct i2c_adapter *, unsigned int flags);
    int (*trylock_bus)(struct i2c_adapter *, unsigned int flags);
    void (*unlock_bus)(struct i2c_adapter *, unsigned int flags);
};

struct i2c_adapter {
//    unsigned int class;       /* classes to allow probing for */
    const struct i2c_algorithm *algo; /* the algorithm to access the bus */
//    void *algo_data;

    /* data fields that are valid for all devices   */
    const struct i2c_lock_operations *lock_ops;
//    struct rt_mutex bus_lock;
//    struct rt_mutex mux_lock;
	FRTOS_mutex bus_lock;

    TickType_t timeout;            /* in jiffies */
    int retries;
    struct device dev;      /* the adapter device */

    int nr;
    char name[48];
    FRTOS_completion_t dev_released;

//    FRTOS_mutex userspace_clients_lock;
//    struct list_head userspace_clients;

//    struct i2c_bus_recovery_info *bus_recovery_info;
//    const struct i2c_adapter_quirks *quirks;
//    struct irq_domain *host_notify_domain;
	struct list_head client_list;
	struct list_head adap_list;
};

struct i2c_board_info {
    char        type[I2C_NAME_SIZE];
    unsigned short  flags;
    unsigned short  addr;
    int     irq;
};

#define  i2c_adapter_id(adap)		(adap->nr)


/* Adapter locking functions, exported for shared pin cases */
#define I2C_LOCK_ROOT_ADAPTER BIT(0)
#define I2C_LOCK_SEGMENT      BIT(1)

/**
 * i2c_lock_bus - Get exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER locks the root i2c adapter, I2C_LOCK_SEGMENT
 *  locks only this branch in the adapter tree
 */
static inline void
i2c_lock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
    adapter->lock_ops->lock_bus(adapter, flags);
}

/**
 * i2c_trylock_bus - Try to get exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER tries to locks the root i2c adapter,
 *  I2C_LOCK_SEGMENT tries to lock only this branch in the adapter tree
 *
 * Return: true if the I2C bus segment is locked, false otherwise
 */
static inline int
i2c_trylock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
    return adapter->lock_ops->trylock_bus(adapter, flags);
}

/**
 * i2c_unlock_bus - Release exclusive access to an I2C bus segment
 * @adapter: Target I2C bus segment
 * @flags: I2C_LOCK_ROOT_ADAPTER unlocks the root i2c adapter, I2C_LOCK_SEGMENT
 *  unlocks only this branch in the adapter tree
 */
static inline void
i2c_unlock_bus(struct i2c_adapter *adapter, unsigned int flags)
{
    adapter->lock_ops->unlock_bus(adapter, flags);
}

static inline void
i2c_lock_adapter(struct i2c_adapter *adapter)
{
    i2c_lock_bus(adapter, I2C_LOCK_ROOT_ADAPTER);
}

static inline void
i2c_unlock_adapter(struct i2c_adapter *adapter)
{
    i2c_unlock_bus(adapter, I2C_LOCK_ROOT_ADAPTER);
}

//struct i2c_client *i2c_slave_enable(struct dw_i2c_dev *i2c_controller);

int i2c_dw_dma_en(void *dev, i2c_opt_mode_u8 mode);
//suntongce: fake api
int i2c_read(void *dev, struct i2c_client *client, u32 reg,
		u32 *buf);
int i2c_write(void *dev, struct i2c_client *client, u32 reg,
		u32 val);
//suntongce: fake api


int i2c_add_numbered_adapter(struct i2c_adapter *adap);
struct i2c_adapter *i2c_get_adapter(int nr);
int i2c_master_recv(const struct i2c_client *client, char *buf, int count);
int i2c_master_send(const struct i2c_client *client, const char *buf, int count);
int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num);
void i2c_del_adapter(struct i2c_adapter *adap);
int i2c_add_numbered_adapter(struct i2c_adapter *adap);
int i2c_add_adapter(struct i2c_adapter *adapter, int id);
void i2c_unregister_device(struct i2c_client *client);
struct i2c_client * i2c_new_device(struct i2c_adapter *adap, struct i2c_board_info const *info);

/* Return the functionality mask */
static inline u32 i2c_get_functionality(struct i2c_adapter *adap)
{
    return adap->algo->functionality(adap);
}

/* Return 1 if adapter supports everything we need, 0 if not. */
static inline int i2c_check_functionality(struct i2c_adapter *adap, u32 func)
{
    return (func & i2c_get_functionality(adap)) == func;
}


#endif

