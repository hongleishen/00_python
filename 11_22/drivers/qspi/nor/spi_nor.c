/***************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    spi_nor support
  @creator

**************************************************************************/

#include "imi_type.h"
#include "elog.h"
#include "hellboy_map.h"
#include "candence_qspi.h"
#include "initcalls.h"
#include "delay.h"
#include "errno.h"
#include "spi_nor.h"
#include "linux_compat.h"
#include "clock.h"

static int nor_flash_index = 0;

enum nor_feature {
	QE_ENABLE = 0x0,
	QE_DISABLE,
};

#define FLASH_OIP               (0)
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static FRTOS_mutex nor_mutex;

#define IMI_NOR_INFO(_jedec_id, _page_shift, _sector_shift)\
	.id = {                         \
		((_jedec_id) >> 16) & 0xff,          \
		((_jedec_id) >> 8) & 0xff,          \
		(_jedec_id) & 0xff,             \
		},                      \
	.id_len = 3,\
	.page_shift = (_page_shift),                \
	.sector_shift = (_sector_shift),

static const struct spi_imi_flash_info spi_nor_flash_ids[] = {
	{"GD25LQ32ESIGR",	IMI_NOR_INFO(0xc86016, 8, 12)},
	{"GD25LQ64ESIGR",	IMI_NOR_INFO(0xc86017, 8, 12)},
	{"GD25Q20",		IMI_NOR_INFO(0xc84012, 8, 12)},
	{"GD25Q20",		IMI_NOR_INFO(0xc84013, 8, 12)},
	{"W25Q16SIQ",		IMI_NOR_INFO(0xEF6015, 8, 12)},
	{"W25Q64SIQ",		IMI_NOR_INFO(0xEF6017, 8, 12)},//page: 256 sector:4K
	{"W25Q64UUIG",		IMI_NOR_INFO(0xEF6017, 8, 12)},
	{"W25Qq64",		IMI_NOR_INFO(0xEF8017, 8, 12)},
	{"P25Q32L", 		IMI_NOR_INFO(0x856016, 8, 12)},
	{"P25Q64L",		IMI_NOR_INFO(0x856017, 8, 12)},
	{"MX25U32", 		IMI_NOR_INFO(0xc22536, 8, 12)},
	{"MX25U64",		IMI_NOR_INFO(0xc22537, 8, 12)},
	{"ZB25VQ32",		IMI_NOR_INFO(0x5E4016, 8, 12)},
	{"ZB25VQ128",		IMI_NOR_INFO(0x5E4018, 8, 12)}
};


// ----------------------------------------------------------------------------
// ------------------------- function -----------------------------------------
// ----------------------------------------------------------------------------
const struct spi_imi_flash_info * res_get_nor_info(void)
{
	return &spi_nor_flash_ids[nor_flash_index];
}

static unsigned int spi_nor_get_page_size(void)
{
	const struct spi_imi_flash_info *flash = res_get_nor_info();

	return (0x01 << flash->page_shift);
}

static inline const struct spi_imi_flash_info * spi_nor_probe(U8 * id)
{
	const struct spi_imi_flash_info * flash_info = spi_nor_flash_ids;
	U8 idx = 0;

	for (; idx < ARRAY_SIZE(spi_nor_flash_ids); idx++, flash_info++) {
		if (!memcmp(flash_info->id, id, flash_info->id_len)) {
			nor_flash_index = idx;
			return flash_info;
		}
	}

	return NULL;
}

static unsigned char spi_nor_read_sr1(void)
{
	unsigned char tmp[3];
	unsigned char val = 0;

	tmp[0] = 0x05; //read status register 1

	cadence_qspi_command_read(1, tmp, 1, &val);

	return val;
}

unsigned char spi_nor_read_sr2(void)
{
	unsigned char tmp[3];
	unsigned char val = 0;

	tmp[0] = 0x35; //read status register 2

	cadence_qspi_command_read(1, tmp, 1, &val);

	return val;
}

int spi_nor_write_s1_s2(unsigned char s1, unsigned char s2)
{
	unsigned char tmp[3];
	unsigned char val[2];

	tmp[0] = 0x01; //wr sr
	val[0] = s1;
	val[1] = s2;

	return cadence_qspi_command_write(1, tmp, 2, val);
}

int spi_nor_write_s1(unsigned char s1)
{
	unsigned char tmp[3];
	unsigned char val[2];

	tmp[0] = 0x01; //wr s1
	val[0] = s1;

	return cadence_qspi_command_write(1, tmp, 1, val);
}

int spi_nor_write_s2(unsigned char s2)
{
	unsigned char tmp[3];
	unsigned char val[2];

	tmp[0] = 0x31; //wr s2
	val[0] = s2;

	return cadence_qspi_command_write(1, tmp, 1, val);
}

int spi_nor_set_feature(enum nor_feature feature)
{
	unsigned char s2 = 0, s1 = 0;

	switch (feature) {
		case QE_ENABLE:
			s2 = 0x02;
			break;

		case QE_DISABLE:
			s2 = 0x0;
			break;

		default:
			s2 = 0;
			break;
	}

	return spi_nor_write_s1_s2(s1, s2)  ;
}

int spi_nor_global_unlock(void)
{
	unsigned char tmp[3];

	tmp[0] = 0x98; //global_unlock

	return cadence_qspi_command_read(1, tmp, 0, NULL);
}

static int spi_nor_write_enable(void)
{
	unsigned char tmp[3];

	tmp[0] = 0x06; //write enable cmd

	return cadence_qspi_command_read(1, tmp, 0, NULL);
}

int spi_nor_write_volatile_sr_enable(void)
{
	unsigned char tmp[3];

	tmp[0] = 0x50; //write_volatile_sr_enable

	return cadence_qspi_command_read(1, tmp, 0, NULL);
}

//any addr inside the sector is ok
int spi_nor_sector_erase(unsigned int addr)
{
	unsigned char tmp[4];

	tmp[0] = 0x20; //sector erase
	tmp[1] = (addr >> 16) & 0xff;
	tmp[2] = (addr >> 8) & 0xff;
	tmp[3] = addr & 0xff;

	return cadence_qspi_command_read(4, tmp, 0, NULL);
}

//any addr inside the block is ok
int spi_nor_block_erase(unsigned int addr)
{
	unsigned char tmp[4];

	tmp[0] = 0x52; //block erase 32K cmd
	tmp[1] = (addr >> 16) & 0xff;
	tmp[2] = (addr >> 8) & 0xff;
	tmp[3] = addr & 0xff;

	return cadence_qspi_command_read(4, tmp, 0, NULL);
}

void spi_nor_reset(void)
{
	unsigned char tmp[4];

	tmp[0] = 0x66; //reset enable

	cadence_qspi_command_read(1, tmp, 0, NULL);

	tmp[0] = 0x99; //reset cmd

	cadence_qspi_command_read(1, tmp, 0, NULL);
}

static int spi_nor_status(U32 flag)
{
	unsigned char st;

	st = spi_nor_read_sr1();
	switch (flag) {
		case FLASH_OIP:
			if ((st & 0x01) != 0)
			{
				return -1;
			}
			break;

		default:
			return -1;
	}

	return 0;
}

static int spi_nor_wait_flash_idle(void)
{
	volatile unsigned int val;
	unsigned int time_out = 0xfffff;

	do {
		val = spi_nor_status(FLASH_OIP);
		time_out--;
	} while(val && time_out);

	if (!time_out) {
		log_e("wait flash timeout\n");
		return -EIO;
	}

	return 0;
}

int spi_nor_erase_process(U32 addr)
{
	U32  ret = 0;
	int retry;

	for (retry = 0; retry < (8); retry++)
	{
		ret = spi_nor_write_enable();
		if (ret) {
			log_e("wr enable fail\n");
			continue;
		}

		ret = spi_nor_sector_erase(addr);
		if (ret) {
			log_e("block_erase fail\n");
			continue;
		}

		ret = spi_nor_wait_flash_idle();
		if (ret)
			return ret;
	}

	return ret;
}

static int spi_nor_read_from_flash(unsigned int addr, unsigned char *buf, unsigned int len)
{
	unsigned char cmd[6];
	unsigned char opcode = 0x03; //read cmd
	unsigned int cmd_len = 4;//opcode + 3 addr byte
	int ret;

#ifdef  QSPI_INST_TYPE_QUAD_MODE
	opcode = 0x6B; //read x 4  cmd
#endif

	//cadence_qspi_controller_init(256, 65536);

	cmd[0] = opcode;
	cmd[1] = (addr >> 16) & 0xff;
	cmd[2] = (addr >> 8) & 0xff;
	cmd[3] = addr & 0xff;

	ret = cadence_qspi_indirect_read_setup(cmd, cmd_len); //opcode + 3 addr byte
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

int spi_nor_read_process(U32 start, U32 numbyte, unsigned char* buffer)
{
		int ret = 0;

#ifdef  QSPI_INST_TYPE_QUAD_MODE
		/* open quad mode */
		spi_nor_write_enable();
		spi_nor_write_volatile_sr_enable();
		spi_nor_set_feature(QE_ENABLE);
		spi_nor_wait_flash_idle();
#endif
		ret = spi_nor_read_from_flash(start, buffer, numbyte); //start is relative addr in one page
		if (ret) {
			log_e("read fail\n");
		}

#ifdef  QSPI_INST_TYPE_QUAD_MODE
		/* close quad mode */
		spi_nor_write_enable();
		spi_nor_write_volatile_sr_enable();
		spi_nor_set_feature(QE_DISABLE);
#endif

	return ret;
}

//row is relative addr in one page
int spi_nor_write_to_flash(unsigned int row, unsigned char *buf, unsigned int len)
{
	unsigned char cmd[4];
	unsigned char opcode = 0x02; //page program
	unsigned int page_size = spi_nor_get_page_size();
	int ret;

#ifdef  QSPI_INST_TYPE_QUAD_MODE
	opcode = 0x32; //page program x 4
#endif

	//cadence_qspi_controller_init(256, 65536);

	cmd[0] = opcode;
	cmd[1] = (row >> 16) & 0xff;
	cmd[2] = (row >> 8) & 0xff;
	cmd[3] = row & 0xff;

	ret = cadence_qspi_indirect_write_setup(cmd, 4); //cmd
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

int spi_nor_write_process(U32 addr, U32 numbyte, unsigned char *buffer)
{
	int ret = 0;

#ifdef  QSPI_INST_TYPE_QUAD_MODE
	ret = spi_nor_write_enable();
	if (ret) {
		log_e("wr enable fail\n");
		return ret;
	}

	spi_nor_write_volatile_sr_enable();
	spi_nor_set_feature(QE_ENABLE);
	spi_nor_wait_flash_idle();
#endif

	spi_nor_write_enable();
	spi_nor_write_to_flash(addr, buffer, numbyte);

	ret = spi_nor_wait_flash_idle();
	if (ret) {
		log_e("wait_flash_idle fail\n");
		return ret;
	}

#ifdef  QSPI_INST_TYPE_QUAD_MODE
	spi_nor_write_volatile_sr_enable();
	ret = spi_nor_write_enable(); //WEL will be reset, so open it again
	if (ret) {
		log_e("wr enable fail\n");
		return ret;
	}
	spi_nor_set_feature(QE_DISABLE);
#endif

	return 0;
}

int spi_nor_read_data(U32 addr, U32 length, char* buf)
{
	const struct spi_imi_flash_info *flash_info = res_get_nor_info();
	U32 len = length;
	U32 start_addr, page_size;
	U32 read_bytes, readed_bytes = 0;
	int ret = 0;

	page_size = 0x01 << flash_info->page_shift;

	if ((length == 0) || (buf == NULL))
	{
		return -EINVAL;
	}

	start_addr = addr;
	read_bytes = MIN(page_size, len);

	do {
		ret = spi_nor_read_process(start_addr,
				read_bytes, (unsigned char *)(buf + readed_bytes));
		if(ret)
			goto log_eor;

		readed_bytes += read_bytes;
		start_addr += read_bytes;
		len -= read_bytes;

		read_bytes = MIN(len, page_size);
	} while (len > 0);

	return 0;
log_eor:
	return ret;
}

//flash data can not span pages once a time
int spi_nor_write_data(U32 addr, U32 length, char * buf)
{
	const struct spi_imi_flash_info *flash_info = res_get_nor_info();
	U32 len = length;
	U32 start_addr, tmp;
	U32 write_bytes, written_bytes;
	U32 page_size, sector_size;
	int ret = 0;

	page_size = 0x01 << flash_info->page_shift;
	sector_size = 0x01 << flash_info->sector_shift;

	if ((length == 0) || (buf == NULL))
	{
		return -EINVAL;
	}

	tmp = addr & (page_size -1);
	write_bytes = MIN((page_size - tmp), len);

	written_bytes = 0;
	start_addr = addr;

#if 0 //we suppose sector has been erased if start addr is not aligned to sector size
	if ((start_addr % sector_size) != 0) {
		spi_nor_erase_process(start_addr); //any addr inside a sector(block) is ok
	}
#endif

	for (; written_bytes < length; )
	{
		if ((start_addr % sector_size) == 0) {
			spi_nor_erase_process(start_addr);
		}

		ret = spi_nor_write_process(start_addr,
				write_bytes, (unsigned char *)(buf + written_bytes));
		if (ret) {
			log_e("spi_nor_write_process err\n");
			break;
		}

		written_bytes += write_bytes;
		start_addr = start_addr + write_bytes;
		len -= write_bytes;

		write_bytes = MIN(page_size, len);
	}

	return ret;
}

static inline int spi_nor_erase(U32 addr, U32 length)
{
	const struct spi_imi_flash_info * flash_info = res_get_nor_info();
	U32 sector_size = 0x01 << flash_info->sector_shift;
	U32 ret = -EIO;
	int len = length;

	if ((addr % sector_size) != 0) {
		log_e("addr is not aligned\n");
		return -EINVAL;
	}

	while (len > 0) {
		ret = spi_nor_erase_process(addr);
		if (ret) {
			log_e("erase fail\n");
			break;
		}
		len -= sector_size;
	}

	return ret;
}


int nor_flash_read(unsigned int addr, int size, char *buf)
{
	int ret = 0;

	FRTOS_mutex_lock(nor_mutex);
	candence_qspi_cs_open();
	ret = spi_nor_read_data(addr, size, buf);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nor_mutex);

	return ret;
}

int nor_flash_write(unsigned int addr, char *buf, int length)
{
	int ret = 0;

	FRTOS_mutex_lock(nor_mutex);
	candence_qspi_cs_open();
	ret = spi_nor_write_data(addr, length, buf);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nor_mutex);

	return ret;
}

int nor_flash_erase(unsigned int addr, int length)
{
	int ret = 0;

	FRTOS_mutex_lock(nor_mutex);
	candence_qspi_cs_open();
	ret = spi_nor_erase(addr, length);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nor_mutex);

	return ret;
}


int nor_flash_rawread(U32 row, U32 column, U32 numbyte, unsigned char *buffer)
{
	const struct spi_imi_flash_info *flash = res_get_nor_info();
	int ret = 0;

	FRTOS_mutex_lock(nor_mutex);
	candence_qspi_cs_open();
	ret = spi_nor_read_process(row << flash->page_shift | column, numbyte, buffer);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nor_mutex);

	return ret;
}

int nor_flash_rawwrite(U32 row, U32 column, U32 numbyte, unsigned char *buffer)
{
	const struct spi_imi_flash_info *flash = res_get_nor_info();
	int ret = 0;

	FRTOS_mutex_lock(nor_mutex);
	candence_qspi_cs_open();
	ret = spi_nor_write_process(row << flash->page_shift | column, numbyte, buffer);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nor_mutex);

	return ret;
}

int nor_flash_rawerase(U32 row)
{
	const struct spi_imi_flash_info *flash = res_get_nor_info();
	int ret = 0;

	FRTOS_mutex_lock(nor_mutex);
	candence_qspi_cs_open();
	ret = spi_nor_erase_process(row << flash->page_shift);
	candence_qspi_cs_close();
	FRTOS_mutex_unlock(nor_mutex);

	return ret;
}


static const struct spi_imi_flash_info * spi_nor_read_id(void)
{
	U8 id[8] = { 0 };
	unsigned char cmd[4] = {0};

	cmd[0] = 0x9f;

	if (cadence_qspi_command_read(1, cmd, 3, id)) {
		log_e("read id fail\n");
		return NULL;
	}

	log_i("id: %x %x %x\r\n", id[0], id[1], id[2]);

	return spi_nor_probe(&id[0]);
}

static void spi_nor_check_busy(void)
{
	unsigned st = spi_nor_status(FLASH_OIP);

	if (st) {
		spi_nor_reset();
		delayus(1000);
	}
}

static int spi_nor_init(unsigned char loopback_clk_bypass)
{
	int ret = 0;
	unsigned int page_size;

	candence_qspi_controller_setup(QSPI_FLASH_CLK_FREQ, loopback_clk_bypass, 256, 65536);
	candence_qspi_cs_open();

	spi_nor_check_busy();
	if (!spi_nor_read_id()) {
		log_i("read id fail\n");
		spi_nor_reset();
		delayus(1000);
		spi_nor_read_id();
	}

//  spi_nor_global_unlock();

	page_size = spi_nor_get_page_size();
	if (page_size != 256) {
		candence_qspi_controller_setup(QSPI_FLASH_CLK_FREQ, loopback_clk_bypass, page_size, 65536);
	}

	candence_qspi_cs_close();

	return ret;
}

void nor_init(void)
{
	int ret;

	clock_enable("qspi");

	FRTOS_mutex_create(&nor_mutex);
	ret = spi_nor_init(1);
	if (ret) {
		log_e("spi nor init fail\n");
	}

#ifdef QSPI_USE_DMA
	ret = cadence_qspi_setup_dma();
	if (ret)
		log_e("cadence_qspi_setup_dma fail\n");
#endif
}


