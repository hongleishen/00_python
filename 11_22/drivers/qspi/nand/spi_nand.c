/***************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    spi_nand support
  @creator

**************************************************************************/

#include "imi_type.h"
#include "elog.h"
#include "hellboy_map.h"
#include "candence_qspi.h"
#include "initcalls.h"
#include "delay.h"
#include "errno.h"
#include "spi_nand.h"
#include "linux_compat.h"
#include "clock.h"


#define NAND_ECC                (0)
#define NAND_ERASE              (1)
#define NAND_PROGRAM            (2)
#define NAND_OIP                (3)

static int nand_flash_id = 0;

enum spi_nand_flags {
	SPI_NAND_FLAGS_NONE = 0x0,
	SPI_NAND_USE_UNLOCK = 0x01,
	SPI_NAND_SET_BUF = 0x02,
};

static FRTOS_mutex nand_mutex;

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define IMI_NAND_INFO(_jedec_id, _page_shift, _block_shift, _block_num, _spare_size, _flag)\
	.id = {                         \
		((_jedec_id) >> 16) & 0xff,          \
		((_jedec_id) >> 8) & 0xff,          \
		(_jedec_id) & 0xff,             \
		},                      \
	.id_len = 3,\
	.page_shift = (_page_shift),                \
	.block_shift = (_block_shift),              \
	.block_num = (_block_num),              \
	.spare_size = (_spare_size),			\
	.flags = (_flag),

static const struct spi_imi_flash_info spi_nand_flash_ids[] = {
	{"GD5F1GQ5RE",	IMI_NAND_INFO(0xC84100, 11, 17, 1024, 64, SPI_NAND_USE_UNLOCK)},
	{"GD5F2GQ5RF",	IMI_NAND_INFO(0xC84200, 11, 17, 2048, 64, SPI_NAND_USE_UNLOCK)},
	{"GD5F2GQ4R",	IMI_NAND_INFO(0xC8C200, 11, 17, 2048, 64, SPI_NAND_USE_UNLOCK)},
	{"w25n01",      IMI_NAND_INFO(0xEFBA21, 11, 17, 1024, 64, SPI_NAND_USE_UNLOCK | SPI_NAND_SET_BUF)},
	{"w25n02",      IMI_NAND_INFO(0xEFBF22, 11, 17, 2048, 64, SPI_NAND_USE_UNLOCK | SPI_NAND_SET_BUF)},
	{"w25n512",     IMI_NAND_INFO(0xEFBA20, 11, 17, 512, 64, SPI_NAND_USE_UNLOCK | SPI_NAND_SET_BUF)},
	{"MX35UF1G",	IMI_NAND_INFO(0xC2A000, 11, 17, 1024, 64, SPI_NAND_USE_UNLOCK)},
	{"MX35UF2G",	IMI_NAND_INFO(0xC29000, 11, 17, 2048, 64, SPI_NAND_USE_UNLOCK)},
	{"TC58CYG0",	IMI_NAND_INFO(0x98D240, 11, 17, 1024, 64, SPI_NAND_USE_UNLOCK)},
	{"TC58CYG1",	IMI_NAND_INFO(0x98DB40, 11, 17, 2048, 64, SPI_NAND_USE_UNLOCK)},
	{"DS35M2GA",	IMI_NAND_INFO(0xE52200, 11, 17, 2048, 64, SPI_NAND_USE_UNLOCK)},
};

// ----------------------------------------------------------------------------
// ------------------------- function -----------------------------------------
// ----------------------------------------------------------------------------
const struct spi_imi_flash_info * res_get_nand_info(void)
{
	return &spi_nand_flash_ids[nand_flash_id];
}

static unsigned int spi_nand_get_page_size(void)
{
	const struct spi_imi_flash_info *flash = res_get_nand_info();

	return (0x01 << flash->page_shift);
}

static inline const struct spi_imi_flash_info * spi_nand_probe(U8 * id)
{
	const struct spi_imi_flash_info * nand_info = spi_nand_flash_ids;
	U8 idx = 0;

	for (; idx < ARRAY_SIZE(spi_nand_flash_ids); idx++, nand_info++) {
		if (!memcmp(nand_info->id, id, nand_info->id_len)) {
			nand_flash_id = idx;
			return nand_info;
		}
	}

	return NULL;
}

static unsigned char spi_nand_get_feature(unsigned char addr)
{
	unsigned char tmp[3];
	unsigned char feature = 0;

	tmp[0] = 0x0f; //get feature cmd
	tmp[1] = addr; // addr

	cadence_qspi_command_read(2, tmp, 1, &feature);

	return feature;
}

unsigned int spi_nand_set_feature(unsigned char addr, unsigned char val)
{
	unsigned char tmp[3];

	tmp[0] = 0x1f; //set feature cmd
	tmp[1] = addr; // addr

	return cadence_qspi_command_write(2, tmp, 1, &val);
}

static unsigned int spi_nand_write_enable(void)
{
	unsigned char tmp[3];

	tmp[0] = 0x06; //write enable cmd

	return cadence_qspi_command_read(1, tmp, 0, NULL);
}

static unsigned int spi_nand_load_page_to_cache(unsigned int row_addr)
{
	unsigned char tmp[4];

	tmp[0] = 0x13; //load page to cache cmd
	tmp[1] = (row_addr >> 16) & 0xff;
	tmp[2] = (row_addr >> 8) & 0xff;
	tmp[3] = row_addr & 0xff;

	return cadence_qspi_command_read(4, tmp, 0, NULL);
}

static unsigned int spi_nand_program_execute(unsigned int row_addr)
{
	unsigned char tmp[4];

	tmp[0] = 0x10; //program execute cmd
	tmp[1] = (row_addr >> 16) & 0xff;
	tmp[2] = (row_addr >> 8) & 0xff;
	tmp[3] = row_addr & 0xff;

	return cadence_qspi_command_read(4, tmp, 0, NULL);
}

static unsigned int spi_nand_block_erase(unsigned int row_addr)
{
	unsigned char tmp[4];

	tmp[0] = 0xd8; //block erase cmd
	tmp[1] = (row_addr >> 16) & 0xff;
	tmp[2] = (row_addr >> 8) & 0xff;
	tmp[3] = row_addr & 0xff;

	return cadence_qspi_command_read(4, tmp, 0, NULL);
}

int spi_nand_reset(void)
{
	unsigned char tmp[4];

	tmp[0] = 0xff; //reset cmd

	return cadence_qspi_command_read(1, tmp, 0, NULL);
}

static int spi_nand_status(U32 flag)
{
	const struct spi_imi_flash_info *flash = res_get_nand_info();
	U32  ecc, ecc_cmp_flg = 0;

	if (!memcmp((const U8*)flash->name, (const U8*)"GD5F", 4))
		ecc_cmp_flg = 1;

	ecc = spi_nand_get_feature(0xc0);
	switch (flag) {
		case NAND_ECC:
			if (ecc_cmp_flg) {
				if ((ecc & 0x70) == 0x70)
					return ERROR_TYPE_ECC_ERROR;
			} else {
				if ((ecc & 0x30) == 0x20)
					return ERROR_TYPE_ECC_ERROR;
			}
			break;

		case NAND_ERASE:
			if ((ecc & 0x04) != 0)
			{
				return ERROR_TYPE_ECC_ERROR;
			}
			break;

		case NAND_PROGRAM:
			if ((ecc & 0x08) != 0)
			{
				return ERROR_TYPE_ECC_ERROR;
			}
			break;

		case NAND_OIP:
			if ((ecc & 0x01) != 0)
			{
				return ERROR_TYPE_WAIT_QSPI_NAND_OIP_TIMEOUT;
			}
			break;

		default:
			return -1;
	}

	return 0;
}

static int spi_nand_wait_flash_idle(void)
{
	volatile unsigned int val;
	unsigned int time_out = 0xfffff;

	do {
		val = spi_nand_status(NAND_OIP);
		time_out--;
	} while(val && time_out);

	if (!time_out) {
		log_e("wait flash timeout\n");
		return -EIO;
	}

	return 0;
}

int spi_nand_erase_process(U32 row)
{
	U32  ret = 0;
	int retry;

	for (retry = 0; retry < (8); retry++)
	{
		ret = spi_nand_write_enable();
		if (ret) {
			log_e("wr enable fail\n");
			continue;
		}

		ret = spi_nand_block_erase(row);
		if (ret) {
			log_e("block_erase fail\n");
			continue;
		}

		ret = spi_nand_wait_flash_idle();
		if (ret)
			return ret;

		ret = spi_nand_status(NAND_ERASE);
		if (ret == 0)
			return QSPI_RET_SUCCESS;
	}

	return ret;
}

static int spi_nand_read_from_cache(unsigned int column, unsigned char *buf, unsigned int len)
{
	unsigned char cmd[4] = {0};
	unsigned char opcode = 0x03; //read from cache
	int ret;

#ifdef  QSPI_INST_TYPE_QUAD_MODE
	opcode = 0x6B; //read from cache x 4
#endif

	cmd[0] = opcode;
	cmd[1] = (column >> 8) & 0xff;
	cmd[2] = column & 0xff;
	//cmd[0] CMD;   cmd[1] addr hight;  cmd[2] addr low
	ret = cadence_qspi_indirect_read_setup(cmd, 3);   //cmd +  2 addr  + 1 dummy byte
	if (ret) {
		log_e("indirect_read_setup fail\n");
		return ret;
	}

	ret = cadence_qspi_indirect_read_execute(len, buf);
	if (ret) {
		log_e("indirect_read_setup fail\n");
		return ret;
	}

	return 0;
}

int spi_nand_read_process(U32 row, U32 column, U32 numbyte, unsigned char* buffer)
{
	int ret = 0;

#ifdef SPI_FLASH_BBM
	row = bbm_search(row);
#endif

	ret = spi_nand_load_page_to_cache(row); //row is page addr
	if (ret)
		return ret;

	ret = spi_nand_wait_flash_idle();
	if (ret)
		return ret;

#ifdef  QSPI_INST_TYPE_QUAD_MODE
		/* open quad mode */
		spi_nand_set_feature(0xb0, 0x11); //ECC default is enable, BIT4 is ecc_en
#endif
		ret = spi_nand_read_from_cache(column, buffer, numbyte); //column is relative addr in one page
		if (ret)
			goto EXIT;

		ret = spi_nand_status(NAND_ECC);
		if (ret) {
			log_e("read ecc log_e\r\n");
		}


EXIT:
#ifdef  QSPI_INST_TYPE_QUAD_MODE
		/* close quad mode */
		spi_nand_set_feature(0xb0, 0x10);//ECC default is enable
#endif

	return ret;
}

int spi_nand_write_to_cache(unsigned int column, unsigned char *buf, unsigned int len)
{
	unsigned char cmd[4];
	unsigned char opcode = 0x02; //write to cache
	unsigned int page_size = spi_nand_get_page_size();
	int ret;

#ifdef  QSPI_INST_TYPE_QUAD_MODE
	opcode = 0x32; //write to cache x 4
#endif

	//cadence_qspi_controller_init(256, 65536);

	cmd[0] = opcode;
	cmd[1] = (column >> 8) & 0xff;
	cmd[2] = column & 0xff;

	ret = cadence_qspi_indirect_write_setup(cmd, 3); //cmd
	if (ret) {
		log_e("indirect_read_write fail\n");
		return ret;
	}

	ret = cadence_qspi_indirect_write_execute(buf, len, page_size);
	if (ret) {
		log_e("indirect_write_setup fail\n");
		return ret;
	}

	return 0;
}

int spi_nand_write_process(U32 row, U32 column, U32 numbyte, unsigned char* buffer)
{
	int ret = 0;

#ifdef SPI_FLASH_BBM
	row = bbm_search(row);
#endif

	ret = spi_nand_write_enable();
	if (ret) {
		log_e("wr enable fail\n");
		return ret;
	}

#ifdef  QSPI_INST_TYPE_QUAD_MODE
	spi_nand_set_feature(0xb0, 0x11);
#endif

	ret = spi_nand_write_to_cache(column,  buffer, numbyte);
	if (ret) {
		log_e("wr to cache fail\n");
		return ret;
	}

#ifdef  QSPI_INST_TYPE_QUAD_MODE
	spi_nand_set_feature(0xb0, 0x10);
#endif

	ret = spi_nand_program_execute(row);
	if (ret) {
		log_e("program_execute fail\n");
		return ret;
	}

	ret = spi_nand_wait_flash_idle();
	if (ret) {
		log_e("wait_flash_idle fail\n");
		return ret;
	}

	ret = spi_nand_status(NAND_PROGRAM);

	return ret;
}

int spi_nand_read_data(U32 addr, U32 length, char* buf)
{
	const struct spi_imi_flash_info *flash_info = res_get_nand_info();
	U32 index, len = length;
	U32 row_start_addr, column_start_addr;
	U32 read_bytes, readed_bytes;
	U32 page_shift, page_size;
	int ret = 0;

	page_shift = flash_info->page_shift;
	page_size = 0x01 << page_shift;

	if ((length == 0) || (buf == NULL))
	{
		return -EINVAL;
	}

	column_start_addr = addr & (page_size - 1); //column  start addr
	row_start_addr = addr >> page_shift; //row start addr

	readed_bytes = 0;
	read_bytes = MIN((page_size - column_start_addr), len);

	for (index = 0; readed_bytes < length; index++)
	{
		ret = spi_nand_read_process(row_start_addr + index, column_start_addr,
				read_bytes, (unsigned char *)(buf + readed_bytes));
		if(ret)
			goto log_eor;

		readed_bytes += read_bytes;
		column_start_addr = 0;
		len -= read_bytes;

		read_bytes = MIN(len, page_size);
	}

	return 0;
log_eor:
	return ret;
}

int spi_nand_write_data(U32 addr, U32 length, char * buf)
{
	const struct spi_imi_flash_info *flash_info = res_get_nand_info();
	U32 index, len = length;
	U32 row_start_addr, column_start_addr;
	U32 write_bytes, written_bytes;
	U32 page_size, block_size;
	int ret = 0;

	page_size = 0x01 << flash_info->page_shift;
	block_size = 0x01 << flash_info->block_shift;

	if ((length == 0) || (buf == NULL))
	{
		return -EINVAL;
	}

	column_start_addr = addr & (page_size - 1); //column  start addr
	row_start_addr = addr >> flash_info->page_shift; //row start addr

	written_bytes = 0;
	write_bytes = MIN((page_size - column_start_addr), len);

#if 0 //we suppose block has been erased if start addr is not aligned to block size
	if ((addr % block_size) != 0) { //not align to block
		spi_nand_erase_process(addr >> flash_info->page_shift);
	}
#endif
	for (index = 0; written_bytes < length; index++)
	{
		if (((addr + written_bytes) % block_size) == 0) //span a block, erase new block
			spi_nand_erase_process((addr + written_bytes) >> flash_info->page_shift);

		ret = spi_nand_write_process(row_start_addr + index, column_start_addr,
				write_bytes, (unsigned char *)(buf + written_bytes));
		if (ret) {
			log_e("spi_nand_write_process fail\n");
			break;
		}

		written_bytes += write_bytes;

		column_start_addr = 0;
		len -= write_bytes;

		write_bytes = MIN(page_size, len);
	}

	return ret;
}

#ifdef SPI_FLASH_BBM
static inline int spi_nand_erase(U32 addr, U32 len)
{
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	U16 block = addr >> nand_info->block_shift;
	U32 block_r = addr & (NAND_BLK_SIZE(nand_info) - 1);
	U32 ret = ERROR_PARAM;
	bool flag = FALSE;

	if (block >= nand_info->block_num)
		goto EXIT;

	while (len) {
		ret = spi_nand_erase_process(search_reloc_tb(block) << 6);// blk lsh 6 is page addr
		if (ret) {
			flag = TRUE;
			ret = update_reloc_tb(block, NAND_INVALID_BLK);
			if (ret)
				goto EXIT;
		}
		block++;

		len -= MIN(NAND_BLK_SIZE(nand_info) - block_r, len);
		block_r = 0;
	}

	if (flag)
		return sync_reloc_tb();

EXIT:
	return ret;
}
#else
static inline int spi_nand_erase(U32 addr, U32 len)
{
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	U16 block = addr >> nand_info->block_shift;
	U32 block_r = addr & (NAND_BLK_SIZE(nand_info) - 1);
	U32 ret = ERROR_PARAM;

	if (block >= nand_info->block_num)
		goto EXIT;

	while (len) {
		ret = spi_nand_erase_process(block << 6); // blk lsh 6 is page addr
		if (ret) {
			goto EXIT;
		}
		block++;

		len -= MIN(NAND_BLK_SIZE(nand_info) - block_r, len);
		block_r = 0;
	}

EXIT:
	return ret;
}
#endif


int nand_flash_read(unsigned int addr, int size, char *buf)
{
	int ret = 0;

	FRTOS_mutex_lock(nand_mutex);
	candence_qspi_cs_open();
	ret = spi_nand_read_data(addr, size, buf);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nand_mutex);

	return ret;
}

int nand_flash_write(unsigned int addr, char *buf, int length)
{
	const struct spi_imi_flash_info *flash_info = res_get_nand_info();
	int ret = 0;

	FRTOS_mutex_lock(nand_mutex);
	candence_qspi_cs_open();

	if (flash_info->flags & SPI_NAND_USE_UNLOCK) {
		spi_nand_set_feature(0xa0, 0x00); //unlock
	}
	ret = spi_nand_write_data(addr, length, buf);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nand_mutex);

	return ret;
}

int nand_flash_erase(unsigned int addr, int length)
{
	const struct spi_imi_flash_info *flash_info = res_get_nand_info();
	int ret = 0;

	FRTOS_mutex_lock(nand_mutex);
	candence_qspi_cs_open();

	if (flash_info->flags & SPI_NAND_USE_UNLOCK) {
		spi_nand_set_feature(0xa0, 0x00); //unlock
	}

	ret = spi_nand_erase(addr, length);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nand_mutex);

	return ret;
}

int nand_flash_rawread(U32 row, U32 column, U32 numbyte, unsigned char *buffer)
{
	int ret = 0;

	FRTOS_mutex_lock(nand_mutex);
	candence_qspi_cs_open();

	ret = spi_nand_read_process(row, column, numbyte, buffer);

	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nand_mutex);

	return ret;
}

int nand_flash_rawwrite(U32 row, U32 column, U32 numbyte, unsigned char *buffer)
{
	const struct spi_imi_flash_info *flash_info = res_get_nand_info();
	int ret = 0;

	FRTOS_mutex_lock(nand_mutex);
	candence_qspi_cs_open();

	if (flash_info->flags & SPI_NAND_USE_UNLOCK) {
		spi_nand_set_feature(0xa0, 0x00); //unlock
	}

	ret = spi_nand_write_process(row, column, numbyte, buffer);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nand_mutex);

	return ret;
}

int nand_flash_rawerase(U32 row)
{
	const struct spi_imi_flash_info *flash_info = res_get_nand_info();
	int ret = 0;

	FRTOS_mutex_lock(nand_mutex);
	candence_qspi_cs_open();

	if (flash_info->flags & SPI_NAND_USE_UNLOCK) {
		spi_nand_set_feature(0xa0, 0x00); //unlock
	}

	ret = spi_nand_erase_process(row);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nand_mutex);

	return ret;
}

int spi_nand_flash_read_oob(U32 offset, void *data)
{
	const struct spi_imi_flash_info * nand_info = res_get_nand_info();
	int ret;

	FRTOS_mutex_lock(nand_mutex);
	candence_qspi_cs_open();

	ret = spi_nand_read_process(offset >> nand_info->page_shift,
			offset & ((1 << nand_info->page_shift) - 1), 0x880, (unsigned char*)data);

	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nand_mutex);

	return ret;
}

static const struct spi_imi_flash_info * spi_nand_read_id(void)
{
	U8 id[8] = { 0 };
	unsigned char cmd[4] = {0};

	cmd[0] = 0x9f;

	if (cadence_qspi_command_read(2, cmd, 2, id)) {
		log_e("read id fail\n");
		return NULL;
	}

	log_i("id: %x %x %x\r\n", id[0], id[1], id[2]);

	if (id[0] == 0xef) {
		cadence_qspi_command_read(2, cmd, 3, id);
		return spi_nand_probe(&id[0]);
	}

	return spi_nand_probe(&id[0]);
}

static void spi_nand_w25_flash_init(void)
{
	volatile unsigned char val = 0;

	val = spi_nand_get_feature(0xB0);
	val &= 0xfe; //close QE
	val |= 0x08;//set buf to 1
	spi_nand_set_feature(0xB0,  val);
}

static void spi_nand_check_busy(void)
{
	unsigned char busy = spi_nand_get_feature(0xC0);

	if (busy & 0x01) {
		spi_nand_reset();
		delayus(1000);
	}
}

static int spi_nand_init(unsigned char loopback_clk_bypass)
{
	const struct spi_imi_flash_info * nand_info = NULL;
	int ret = 0;
	unsigned int page_size;

	candence_qspi_controller_setup(QSPI_FLASH_CLK_FREQ, loopback_clk_bypass, 2048, 65536);
	candence_qspi_cs_open();

	spi_nand_check_busy();
	if (!spi_nand_read_id()) {
		log_i("r id fail\n");
		spi_nand_reset();
		delayus(1000);
		spi_nand_read_id();
	}

	nand_info = res_get_nand_info();

	if (nand_info->id[0] == 0xef)
		spi_nand_w25_flash_init();

	page_size = spi_nand_get_page_size();
	if (page_size != 2048) {
		candence_qspi_controller_setup(QSPI_FLASH_CLK_FREQ, loopback_clk_bypass, page_size, 65536);
	}

#ifdef SPI_FLASH_BBM
	ret = scan_reloc_tb();
#endif

	candence_qspi_cs_close();

	return ret;
}

void nand_init(void)
{
	int ret = 0;

	clock_enable("qspi");

	FRTOS_mutex_create(&nand_mutex);

	ret = spi_nand_init(1);
	if (ret) {
		log_e("spi nand init fail\n");
	}

#ifdef QSPI_USE_DMA
	ret = cadence_qspi_setup_dma();
	if (ret)
		log_e("cadence_qspi_setup_dma fail\n");
#endif
}

