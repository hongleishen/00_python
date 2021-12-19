/***************************************************************************
  Copyright (c) 2019 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    spi nand bad block manage for bootldr
  @creator  Qianyu Liu

  History:
    1.  @author Qianyu Liu
        @date   2019-02-22
        @brief  the initial version
**************************************************************************/

#ifndef _SPI_BBM_H__
#define _SPI_BBM_H__

#ifdef SPI_FLASH_BBM

#define NAND_INVALID_BLK            0xffff
#define ITEM_MAX_SIZE               (511)

struct reloc_item {
	unsigned short from;
	unsigned short to;
};

struct reloc_table {
	unsigned short header;
	unsigned short total;
};

struct bbt {
	struct reloc_table table;
	struct reloc_item item[ITEM_MAX_SIZE];
};

struct bbm {
	unsigned short max_reloc_entry;
	unsigned char cur_slot;
	unsigned char need_sync;
	struct bbt bbt;
};

extern S32  scan_reloc_tb(void);
extern U32  bbm_search(U32 addrup);
extern U16  search_reloc_tb(U16 block);
S32 sync_reloc_tb(void);
S32 update_reloc_tb(U16 block, U16 reloc);


#else

#define scan_reloc_tb()         SUCCESS
#define sync_reloc_tb()         FAIL
#define update_reloc_tb(block, NAND_INVALID_BLK) FAIL
#define search_reloc_tb(block)  block
#define bbm_search(addrup)      addrup

#endif

#endif /*_SPI_BBM_H__*/

