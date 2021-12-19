#ifndef __BOARD_H__
#define __BOARD_H__

enum SEC_BOOT_FLG {
	NO_SEC_BOOT = 0x0,
	SEC_BOOT,
};

enum BOOTMODE {
	NOR_BOOT = 0x0,
	IRAM_BOOT,
	USB_BOOT,
	NAND_BOOT,
	UART_BOOT,
	NOR_BOOT_DISABLE_LOOPBACK, // read cap delay decided by cal
	NAND_BOOT_DISABLE_LOOPBACK, //read cap delay decided by cal
};

int get_secboot_flag(void);
int get_bootmode(void);
int flash_read(unsigned int addr, char *buf, int size);
int flash_write(unsigned int addr, char *buf, int size);
void flash_erase(unsigned int addr, int len);

#endif
