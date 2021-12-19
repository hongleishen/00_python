/******************************************************************************
  All rights reserved.

  @brief        chip init
  @creator      Qianyu Liu

  @History
      When          Who             What, where, why
      ----------    ------------    ----------------------------------------
      2020-02-14    Boxiao Ren      the modified version
******************************************************************************/
#include "imi_type.h"
#include "ddr.h"
#include "uart.h"
#include "imi_io.h"
#include "string.h"
#include "initcalls.h"

extern char __start[], __vectors_end[];

#if 0
void *memcpy(void *dest, const void *src, U32 n)
{
	U8 *destaddr = dest;
	U8 *srcaddr = (U8 *)src;

	while (n--) {
		*destaddr++ = *srcaddr++;
	}
	return dest;
}
#endif

void ddr_init(void)
{
	int ret = 0;
	long long key = 0;
	init_chip_ddr(993);

	ret = DDR3_init(0, &key);
	if (ret) {
		imi_printf(" ddr init fail\n");
	}
}

void ddr_init_fpga(void)
{
	writel(0xFFFFFFFF, 0x18000004);
	writel(0x3, 0x1800000c);
	writel(0x1, 0x18000018);
	writel(0, 0x18000000);   //reset open
	while(readl(0x18000020) != 1)
	{
		writel(0,0x18000000);
	}
}

void vector_addr_init(void)
{
	imi_printf("\nvector start:0x%x end:0x%x\r\n",
				__start, __vectors_end);

	memcpy((void*)0xffff0000, __start, __vectors_end - __start); //0x700009c
}
#if defined TARGET_SRAM
EARLY_INIT(ddr_init);
EARLY_INIT(vector_addr_init);
#elif defined TARGET_DDR
EARLY_INIT(vector_addr_init);
#else
EARLY_INIT(ddr_init_fpga);
EARLY_INIT(vector_addr_init);
#endif
