#if defined(CHIP_IMI_1680)
#include "cpu.h"
#include "printf.h"
#include "stdio.h"
#include "imi_io.h"

void cpu_open(unsigned int id)
{
	imi_printf("set CPU %d open.\n\r",id);
	write32(0x20f08024, 0x80000000);
	write32(0x20f08004, 0xffffffff);
	write32(0x20f08000, 0x0);
}

void cpu_close(unsigned int id)
{
	imi_printf("set CPU %d close.\n\r",id);
	write32(0x20f08004, 0x0);
	write32(0x20f08000, 0x1);
}
#endif/*CHIP_IMI_1680*/
