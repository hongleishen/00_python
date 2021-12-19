/***************************************************************************
  Copyright (c) 2019 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    nand bad block manage
  @creator  Qianyu Liu

  History:
    1.  @author Qianyu Liu
        @date   2019-02-11
        @brief  the initial version
**************************************************************************/

#ifdef SPI_FLASH_BBM

#include "utils.h"
#include "spi_nand.h"
#include "spi_bbm.h"
#include "my_printf.h"
#include "candence_qspi.h"

#define IMI_SPI_NAND_DEV_SIZE       (0x10000000)
#define NAND_RELOC_HEADER           (0x524e)

#define IMI_SPI_NAND_SLOT_OFFSET    (2)

#define NAND_MAX_BBT_SLOTS          (40)//(32)

#define NAND_MAX_RELOC_ENTRY        (127)
#define NAND_RELOC_ENTRY(nand)      \
		MIN(((nand)->block_num * 2) / 100, NAND_MAX_RELOC_ENTRY)

#define NAND_BBT_BLOCK				3
#define NAND_FACTORY_DATA_BLOCK		5
#define NAND_BLK_END(nand)    ((nand)->block_num - NAND_BBT_BLOCK - NAND_FACTORY_DATA_BLOCK)
#define NAND_BLK_ENTRY(nand)    (NAND_BLK_END(nand) - g_bbm.max_reloc_entry)

struct bbm g_bbm;


/**
 * nand_is_bad() - Is bad block
 *
 * @block:  block id
 */
static bool nand_is_bad(U32 block)
{
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	unsigned char buf[4];
	U8 page = 0;

	for (; page < 2; page++) {
		if (spi_nand_read_process(NAND_GET_PAGE(block, nand_info) +
				page, NAND_PAGE_SIZE(nand_info), 4, buf) || buf[0] != 0xff)
			return TRUE;
	}

	return FALSE;
}


/**
 * find_a_good_block() - Look for a good block from @reloc_block to front
 *
 * @reloc_block:    start with this block
 */
static U16 find_a_good_block(U16 reloc_block)
{
    const struct spi_imi_flash_info * nand_info = res_get_nand_info();
    struct bbm * bbm = &g_bbm;;
    struct reloc_table *table = &bbm->bbt.table;
    struct reloc_item *item = bbm->bbt.item;
    U16 reloc_num = table->total;

    while (1) {
        /* if reloc_block is invalid block, find another reloc_block
            in bad block table */
        if (reloc_block > nand_info->block_num) {
            if (reloc_num)
                reloc_block = item[--reloc_num].to;
            else
                reloc_block = NAND_BLK_END(nand_info);
            continue;
        }

        if (reloc_block < NAND_BLK_ENTRY(nand_info)) {
            printf("cannot found good block from block area,"
                "reloc_block=%d\r\n", reloc_block);
            return NAND_INVALID_BLK;
        }

        if (nand_is_bad(--reloc_block)) {
            printf("%s: %d is bad block\r\n", __func__, reloc_block);
            continue;
        }

        if (!spi_nand_erase_process(reloc_block << 6))
            break;
	}

	return reloc_block;
}

/**
 * update_reloc_tb() - Update reloc table
 *
 * @buf:    table
 * @block:  block id
 */
S32 update_reloc_tb(U16 block, U16 reloc_block)
{
	struct bbt *bbt = &g_bbm.bbt;
	struct reloc_table *table = (struct reloc_table *)bbt;
	struct reloc_item *item = (struct reloc_item *)bbt->item;

	if (NAND_INVALID_BLK == reloc_block) {
		reloc_block = find_a_good_block(reloc_block);
		if (NAND_INVALID_BLK == reloc_block)
			return UPDATE_TABLE_FAIL;
	}

	/* Create the relocated block information in the table */
	item[table->total].from = block;
	item[table->total].to = reloc_block;

	printf("block: %d is relocated to block: %d\r\n",
			item[table->total].from, item[table->total].to);
	table->total++;

	return 0;
}

/**
 * sync_reloc_tb() - Write table
 *
 * @buf:    table
 * @idx:    page id
 */
S32 sync_reloc_tb(void)
{
	int start_page;
	struct bbm *bbm = &g_bbm;
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	U32 block_bbt_f = nand_info->block_num;
	u32 block_bbt_t = nand_info->block_num - NAND_BBT_BLOCK;
	int ret = 0;

	if (bbm->cur_slot >= NAND_MAX_BBT_SLOTS) {
		INFO("Can't write relocat table to dev any more\n");
		return FAIL;
	}

	bbm->cur_slot++;
	printf("write the bbt to device, bbm->cur_slot=%d\r\n" ,bbm->cur_slot);

	for (; block_bbt_f > block_bbt_t; block_bbt_f--) {
		if (nand_is_bad(block_bbt_f)) {
			printf("block_bbt_f %d is bad\r\n", block_bbt_f);
			continue;
		}
		/* write to device */
		start_page = NAND_GET_PAGE(block_bbt_f, nand_info);
		start_page = start_page - IMI_SPI_NAND_SLOT_OFFSET - bbm->cur_slot;
		printf("start_page is %d\r\n", start_page);
		ret = spi_nand_write_data(start_page << nand_info->page_shift,
			NAND_PAGE_SIZE(nand_info), (char*)&bbm->bbt);
	}
	return ret;
}

/**
 * create_reloc_tb() - Create table
 *
 * @void
 */
static S32 create_reloc_tb(void)
{
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	struct bbm *bbm = &g_bbm;
	struct reloc_table *table = &bbm->bbt.table;
	int block;
	int ret = FAIL;

	table->header = NAND_RELOC_HEADER;
	table->total = 0;

	for (block = 1; block < NAND_BLK_ENTRY(nand_info); block++) {
		if (nand_is_bad(block)) {
			printf("block %d is bad\r\n", block);
			ret = update_reloc_tb(block, NAND_INVALID_BLK);
			if (ret) {
				INFO("Failed to update relocation table\n");
				goto EXIT;
			}
		}
	}

	ret = sync_reloc_tb();
	INFO("Create relocation table end\n");

EXIT:
	return ret;
}

/**
 * dump_reloc_table() - print the reloc table info
 *
 * @void
 */
static void dump_reloc_table(struct bbm *bbm, U32 page)
{
	//const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	int i;

	printf("relocation table at page: %d, length: %d\r\n",
			page, bbm->bbt.table.total);
	for (i = 0; i < bbm->bbt.table.total; i++) {
		printf("block: %d is relocated to block: %d\r\n",
			bbm->bbt.item[i].from, bbm->bbt.item[i].to);
	}
}


/**
 * scan_reloc_tb() - Scan table
 *
 * @void
 */

S32 scan_reloc_tb(void)
{
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	struct bbm *bbm = &g_bbm;
	U32 block_bbt_f = nand_info->block_num;
	U32 block_bbt_t = nand_info->block_num - NAND_BBT_BLOCK;
	U32 maxslot = 0;
	U32 page = 0;
	int ret = FAIL;

	BUILD_BUG_ON(NAND_RELOC_ENTRY(nand_info) > NAND_MAX_RELOC_ENTRY);
	BUILD_BUG_ON(sizeof(struct reloc_table) != sizeof(struct reloc_item));
	BUILD_BUG_ON(NAND_MAX_BBT_SLOTS >= (1 << 6));

	bbm->cur_slot = 0;
	bbm->max_reloc_entry = NAND_RELOC_ENTRY(nand_info);

	for (; block_bbt_f > block_bbt_t; block_bbt_f--) {
		maxslot = NAND_GET_PAGE(block_bbt_f, nand_info);
		page = maxslot - NAND_MAX_BBT_SLOTS - IMI_SPI_NAND_SLOT_OFFSET;
		printf("scan reloc tb, from page %d to maxslot %d\r\n", page, maxslot);
		for (; page < maxslot; page++) {
			memset((U8*)&bbm->bbt, 0, sizeof(bbm->bbt));
			ret = spi_nand_read_process(page, 0, NAND_PAGE_SIZE(nand_info),
					(unsigned char*)&bbm->bbt);
			if (!ret && bbm->bbt.table.header == NAND_RELOC_HEADER) {
				bbm->cur_slot = maxslot - page - 1;
				printf("bbm->cur_slot: %d\r\n", bbm->cur_slot);
				dump_reloc_table(bbm, page);
				goto EXIT;
			}
		}
	}

	ret = create_reloc_tb();

EXIT:
	return ret;
}

/**
 * search_reloc_tb() - Search reloc item in table
 *
 * @block:  block id
 */
U16 search_reloc_tb(U16 block)
{
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	struct bbm *bbm = &g_bbm;
	struct reloc_table *table = (struct reloc_table *)&bbm->bbt;
	struct reloc_item *item = bbm->bbt.item;
	int i, reloc_block;

	if (block <= 0 || block >= NAND_BLK_ENTRY(nand_info) ||
			bbm->cur_slot == 0 || table->total == 0)
		return block;

	/* If can't find reloc tb entry for block, return block */
	reloc_block = block;
	for (i = 0; i < table->total; i++) {
		if (item[i].from == reloc_block) {
			reloc_block = item[i].to;
			/* Start over. */
			i = 0;
			continue;
		}
	}

	return reloc_block;
}

/**
 * bbm_search() - Search reloc item in table
 *
 * @addrup: up addr
 */
U32 bbm_search(U32 addrup)
{
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	U32 addrup_switch = addrup;
	U32 block_switch;
	U32 block;

	block = addrup >> NAND_SLOT_SHIFT(nand_info);

	block_switch = search_reloc_tb(block);
	if (block_switch != block)
		addrup_switch = (block_switch << NAND_SLOT_SHIFT(nand_info))
			| (addrup & (NAND_SLOT_SIZE(nand_info) - 1));

	return addrup_switch;
}

#endif

