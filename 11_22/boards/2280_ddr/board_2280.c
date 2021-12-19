#include "board.h"
#include "spi_nand.h"
#include "spi_nor.h"
#include "initcalls.h"
#include "elog.h"

int get_bootmode(void)
{
	//return NAND_BOOT;
	return NOR_BOOT;
}

int get_secboot_flag(void)
{
	return NO_SEC_BOOT;
}

int flash_read(unsigned int addr, char *buf, int size)
{
	int boot_mode = get_bootmode();

	if (boot_mode == NOR_BOOT || boot_mode == NOR_BOOT_DISABLE_LOOPBACK)
		return nor_flash_read(addr, size, buf);
	else
		return nand_flash_read(addr, size, buf);
}

int flash_write(unsigned int addr, char *buf, int size)
{
	int boot_mode = get_bootmode();

	if (boot_mode == NOR_BOOT || boot_mode == NOR_BOOT_DISABLE_LOOPBACK)
		return nor_flash_write(addr, buf, size);
	else
		return nand_flash_write(addr, buf, size);
}

void flash_erase(unsigned int addr, int len)
{
	int boot_mode = get_bootmode();

	if (boot_mode == NOR_BOOT || boot_mode == NOR_BOOT_DISABLE_LOOPBACK)
		nor_flash_erase(addr,  len);
	else
		nand_flash_erase(addr, len);
}

void board_init(void)
{

	int boot_mode = get_bootmode();
	log_e("board init\r\n");

	if (boot_mode == NOR_BOOT || boot_mode == NOR_BOOT_DISABLE_LOOPBACK)
		nor_init();
	else
		nand_init();

}

DEV_INIT(board_init);
