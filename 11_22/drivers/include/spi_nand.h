/***************************************************************************
  Copyright (c) 2019 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    spi_nand interface
  @creator

  History:
	1.  @author
		@date
		@brief  the initial version
	2.  @author Qianyu Liu
		@date   2019-02-11
		@brief  optimize
**************************************************************************/

#ifndef _QSPI_NAND_CTRL_H__
#define _QSPI_NAND_CTRL_H__
// ----------------------------------------------------------------------------

#include "candence_qspi.h"
#include "imi_io.h"

#define QSPI_RET_SUCCESS        (0)
#define QSPI_RET_READ_ERR       (-1)
#define QSPI_RET_WRITE_ERR      (-2)
#define QSPI_RET_ERASE_ERR      (-3)

enum {
	ERROR_TYPE_WAIT_QSPI_NAND_OIP_TIMEOUT = 1,
	UPDATE_TABLE_FAIL,
	INIT_FAILED,
	ERROR_TYPE_READ_FAILED,
	ERROR_TYPE_WRITE_FAILED,
	ERROR_TYPE_ERASE_FAILED,
	ERROR_TYPE_ECC_ERROR,
	ERROR_TYPE_TIMEOUT,
	ERROR_TYPE_CLEAR_BBT_FAIL,

	ERROR_TYPE_FIND_FAIL,
	ERROR_PARAM,
	ERROR_BACKUP_FAIL,
	ERROR_DIED,
};

#define NAND_SLOT_SHIFT(nand)   ((nand)->block_shift - (nand)->page_shift)
#define NAND_SLOT_SIZE(nand)    (BIT(NAND_SLOT_SHIFT(nand)))
#define NAND_BLK_SIZE(nand) (BIT((nand)->block_shift))
#define NAND_PAGE_SIZE(nand)    (BIT((nand)->page_shift))

#define NAND_GET_PAGE(block, nand) ((block) << NAND_SLOT_SHIFT(nand))
#define NAND_GET_BLK(page, nand) ((page) >> NAND_SLOT_SHIFT(nand))

int spi_nand_read_process(U32 row, U32 column, U32 numbyte, unsigned char *buffer);

/*
 * addr logical address
 * length read from addr to addr + length
 * buf, get data from nand and write to buf.
 * success: return QSPI_RET_SUCCESS         (0)
 * read error: return QSPI_RET_READ_ERR     (-1)
 * erase error: return QSPI_RET_ERASE_ERR   (-3)
 */
int spi_nand_read_data(U32 addr, U32 length, char* buf);
/*
 * addr logical address
 * length write from addr to addr + length
 * buf, get data from buf and write to nand.
 * success: return QSPI_RET_SUCCESS         (0)
 * read error: return QSPI_RET_READ_ERR     (-1)
 * write error: return QSPI_RET_WRITE_ERR   (-2)
 * erase error: return QSPI_RET_ERASE_ERR   (-3)
 */
int spi_nand_write_data(U32 addr, U32 length, char * buf);

/*
 * addr: logical address: 10 block + 6 page + 11 page offset
 * length:
 * function will erase from the block of addr to the block of (addr + length)
 *
 * success: return QSPI_RET_SUCCESS         (0)
 * erase error: return QSPI_RET_ERASE_ERR   (-3)
 */

int spi_nand_erase_process(U32 row);

struct nand_flash_info {
	S8  *name;
	U32 id;
	U32 page_size;
};

const struct spi_imi_flash_info * res_get_nand_info(void);

int nand_flash_read(unsigned int addr, int size, char *buf);
int nand_flash_write(unsigned int addr, char *buf, int length);
int nand_flash_erase(unsigned int addr, int length);

int nand_flash_rawwrite(U32 row, U32 column, U32 numbyte, unsigned char *buffer);
int nand_flash_rawread(U32 row, U32 column, U32 numbyte, unsigned char *buffer);
int nand_flash_rawerase(U32 row);

void nand_init(void);

// ----------------------------------------------------------------------------
#endif // _QSPI_CTRL_H_INCLUDED_

