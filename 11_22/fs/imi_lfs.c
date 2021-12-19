#include <board.h>
#include <elog.h>
#include <candence_qspi.h>
#include <spi_nand.h>
#include <spi_nor.h>

#include "littlefs/lfs.h"
#include "littlefs/lfs_util.h"

#define LFS_FS_OFFSET 0U
#define LFS_FS_SIZE 128U

struct lfs_flash_info {
    /* @block_offset: where littlefs start from, default block 0*/
    lfs_block_t block_offset;
    /* @erase_unit: minmum erase unit in bytes */
    uint32_t erase_unit;
    /* @prog_unit: minmum programe unit in bytes */
    uint32_t prog_unit;

    struct {
        /* @off2pa: convert offset to page address */
        uint32_t off2pa;
        /* @blk2pa: convert block count to page address */
        uint32_t blk2pa;
    } unit_conv;
};

static struct lfs_flash_info flash_info;

static int lfs_read(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, void *buffer, lfs_size_t size)
{
    uint32_t pa, ca;
    int ret;

    block += flash_info.block_offset;
    pa = (block * flash_info.unit_conv.blk2pa) + (off / flash_info.unit_conv.off2pa);
    ca = off % flash_info.unit_conv.off2pa;

    log_d("read block %u, off: %u\n", block, off);

    ret = flash_rawread(pa, ca, size, (unsigned char *)buffer);

    log_d("read block end,ret=%d\n", ret);

    return ret == 0 ? LFS_ERR_OK : LFS_ERR_CORRUPT;

}

static int lfs_prog(const struct lfs_config *c, lfs_block_t block,
        lfs_off_t off, const void *buffer, lfs_size_t size)
{
    uint32_t pa, ca;
    int ret;

    block += flash_info.block_offset;
    pa = (block * flash_info.unit_conv.blk2pa) + (off / flash_info.unit_conv.off2pa);
    ca = off % flash_info.unit_conv.off2pa;

    log_d("read block %u, off: %u\n", block, off);

    ret = flash_rawwrite(pa, ca, size, (unsigned char *)buffer);

    log_d("read block end,ret=%d\n", ret);

    return ret == 0 ? LFS_ERR_OK : LFS_ERR_CORRUPT;
}

static int lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    int ret;

    block += flash_info.block_offset;

    log_d("erase block %u\n", block);
    ret = flash_rawerase(block * flash_info.unit_conv.blk2pa);
    log_d("erase block end,ret=%d\n", ret);

    return ret == 0 ? LFS_ERR_OK : LFS_ERR_CORRUPT;
}

static int lfs_sync(const struct lfs_config *c)
{
    return LFS_ERR_OK;
}

lfs_t g_system_lfs;

static struct lfs_config lfs_config = {
    .read  = lfs_read,
    .prog  = lfs_prog,
    .erase = lfs_erase,
    .sync  = lfs_sync,
};


int imi_lfs_init(void)
{
    int boot_mode = get_bootmode();

    if (boot_mode == NOR_BOOT || boot_mode == NOR_BOOT_DISABLE_LOOPBACK) {
        const struct spi_imi_flash_info *nor_info = res_get_nor_info();

        flash_info.block_offset = LFS_FS_OFFSET;
        flash_info.erase_unit = 1U << nor_info->sector_shift;
        flash_info.prog_unit = 1U << nor_info->page_shift;
        flash_info.unit_conv.off2pa = flash_info.prog_unit;
        flash_info.unit_conv.blk2pa = 1;

        lfs_config.read_size      = 1;
        lfs_config.prog_size      = 1;
        lfs_config.block_size     = flash_info.erase_unit;
        lfs_config.cache_size     = flash_info.prog_unit;
        lfs_config.lookahead_size = 512;
        lfs_config.block_cycles   = 500;
        lfs_config.block_count    = LFS_FS_SIZE;
    } else {
        const struct spi_imi_flash_info *nand_info = res_get_nand_info();

        flash_info.block_offset = LFS_FS_OFFSET;
        flash_info.erase_unit = 1U << nand_info->block_shift;
        flash_info.prog_unit = 1U << nand_info->page_shift;
        flash_info.unit_conv.off2pa = flash_info.prog_unit;
        flash_info.unit_conv.blk2pa = 1U << (nand_info->block_shift - nand_info->page_shift);

        lfs_config.read_size      = 16;
        lfs_config.prog_size      = 16;
        lfs_config.block_size     = flash_info.erase_unit;
        lfs_config.cache_size     = flash_info.prog_unit;
        lfs_config.lookahead_size = 512;
        lfs_config.block_cycles   = 500;
        lfs_config.block_count    = LFS_FS_SIZE;
    }

    int ret = lfs_mount(&g_system_lfs, &lfs_config);

    if (ret < 0) {
        log_e("lfs mount failed: can't find lfs\n");
#ifndef LFS_READONLY
        log_e("trying format flash ...\n");
        ret = lfs_format(&g_system_lfs, &lfs_config);
        log_e("format lfs %s\n", ret < 0 ? "fail" : "success");
        if (ret < 0)
            goto exit;

        ret = lfs_mount(&g_system_lfs, &lfs_config);
        if (ret < 0) {
            log_e("lfs mount failed again\n");
            goto exit;
        }

#endif
    }

    log_e("lfs mount  %s\n", ret < 0 ? "fail" : "success");
exit:
    return ret;
}

void imi_lfs_deinit(void)
{
    lfs_unmount(&g_system_lfs);
}
