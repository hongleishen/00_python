#ifndef _QSPI_NOR_CTRL_H__
#define _QSPI_NOR_CTRL_H__

#include "candence_qspi.h"

const struct spi_imi_flash_info * res_get_nor_info(void);

int nor_flash_read(unsigned int addr, int size, char *buf);
int nor_flash_write(unsigned int addr, char *buf, int length);
int nor_flash_erase(unsigned int addr, int length);
void nor_init(void);

int nor_flash_rawread(U32 row, U32 column, U32 numbyte, unsigned char *buffer);
int nor_flash_rawwrite(U32 row, U32 column, U32 numbyte, unsigned char *buffer);
int nor_flash_rawerase(U32 row);

#endif // _QSPI_CTRL_H_INCLUDED_

