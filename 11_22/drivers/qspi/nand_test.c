/*
#include "utils.h"
#include "spi_nor.h"
#include "spi_nand.h"
#include "my_printf.h"
*/

/*
#include "comdef.h"
#include "utils.h"
#include "comdef.h"
#include "spi_bbm.h"
#include "my_printf.h"
#include "timer.h"
#include "global.h"
*/

#include "spi_nand.h"
#include "uart.h"
#include "printf.h"
#include "imi_type.h"
#include "linux_compat.h"

void print_nand_int(int *buf, int n)
{
	for (int i = 0; i < n; i++) {

		imi_printf("%x ", buf[i]);

		if ( (i + 1)%16 == 0) {
			uart_puts("\r\n");
		}
	}
}

void print_nand_char(u8 *buf, int n)
{
	for (int i = 0; i < n; i++) {
		if (i % 16 == 0) {
			imi_printf("[%x]	", i);
		}

		imi_printf("%x ", buf[i]);

		if ( (i + 1)%16 == 0) {
			uart_puts("\r\n");
		}
	}
}


#define TEST_SIZE	4096
//test with world
static u32 wbuf[TEST_SIZE] = {0};
static u32 rbuf[TEST_SIZE] = {0};

u32 test_nand_data(void)
{
	int i = 0;
	int ret;

	u32 addr = 	0X100000;
	u32 numbyte = TEST_SIZE;
	u32 num_world = TEST_SIZE / 4;

	for (i = 0; i < numbyte/4; i++) {
		wbuf[i] = i;
		//wbuf[i] = numbyte/4 - 1 - i;
	}

	#if 1
	uart_puts("nand_flash_write\r\n");
	ret = nand_flash_write(addr, (char *)wbuf, numbyte);
	if (ret != 0)
		imi_printf("write error, ret = %d\n", ret);
	#endif

	uart_puts("nand_flash_read\r\n");
	ret = nand_flash_read( addr, numbyte, (char *)rbuf);
	if (ret != 0)
		imi_printf("read error, ret = %d\n", ret);

	uart_puts("rdata:\r\n");
	print_nand_int((int *)rbuf, num_world);
	//print_nand_char((u8 *)rbuf, num_world*4);


	//check data --------------------------
	for(i = 0; i < num_world; i++) {
		if (rbuf[i] != wbuf[i])
			break;
	}

	if (i == num_world) {
		uart_puts("data right!!\r\n");
		return 0;
	}
	else {
		uart_puts("data wrong\r\n");
		imi_printf("wrong index 0x%x\r\n", i);
		return -1;
	}

	/*
	//print page data
	//addr = 0;
	if (addr & PAGE_MASK) {
		imi_printf("\n\n  print integer page:\n");

		ret = nand_flash_read( addr & (~PAGE_MASK), 2048, (char *)rbuf);
		if (ret != 0)
			imi_printf("read error, ret = %d\n", ret);
		print_nand_int((int *)rbuf, 512);

		ret = nand_flash_read( (addr & (~PAGE_MASK)) + 2048, 2048, (char *)rbuf);
		if (ret != 0)
			imi_printf("read error, ret = %d\n", ret);
		imi_printf("\n\n");
		print_nand_int((int *)rbuf, 512);
	}
	*/

	uart_puts("test end\r\n");
	return 0;
}




//test byte -------------------------------------
void print_nand_byte(u8 *buf, int n)
{
	uart_puts("\r\n");
	for (int i = 0; i < n; i++) {
		imi_printf("%x ", buf[i]);

		if ( (i + 1)%16 == 0) {
			uart_puts("\r\n");
		}
	}
}

u32 test_nand_byte(void)
{
	u8 wbuf[512] = {0};
	u8 rbuf[512] = {0};

	int i = 0;
	int ret;

	//u32 addr = 0xCA800;
	u32 addr = 0x100000;
	u32 numbyte = 16;


	for (i = 0; i < numbyte; i++) {
		wbuf[i] = i;
	}


	//write -----------------------------
	ret = nand_flash_write(addr, (char *)wbuf, numbyte);
	if (ret != 0)
		imi_printf("write error, ret = %d\n", ret);

	//read ------------------------------
	memset(rbuf, 0 , numbyte);
	ret = nand_flash_read( addr, numbyte, (char *)rbuf);
	if (ret != 0)
		imi_printf("read error, ret = %d\n", ret);

	imi_printf("rdata:%d\n", ret);
	print_nand_byte(rbuf, numbyte);


	//check data --------------------------
	for(i = 0; i < numbyte; i++) {
		if (rbuf[i] != wbuf[i])
			break;
	}

	if (i == numbyte) {
		uart_puts("data right!!\r\n");
		uart_puts("test successful\r\n");
		return 0;
	}
	else {
		imi_printf("data wrong %d\n", ret);

	}

	uart_puts("test end\r\n");
	return 0;
}

void find_nand_clk_delay(void)
{
	int div = 0;
	int delay = 0;
	int ret = -1;


	for (div = 0; div < 16; div++) {
		imi_printf("try div = %d, clk = %d\n", div, 120/(2 * (div + 1)));
		for (delay = 0; delay < 6; delay ++) {
			imi_printf("\n	try delay = %d\n", delay);

			candence_clk_delay_config(div, delay);
			ret = test_nand_data();
			if (ret == 0) {
				imi_printf("successfull: div = %d, delay = %d\n", div, delay);
				candence_clk_delay_config(div, delay);
				return;
			}
		}
	}

	imi_printf("fail find div delay\n");
	return;
}

void nand_test(void)
{
	uart_puts("\r\nNand test^^^^^^^^\n");
	//test_nand_byte();
	for(int i; i < 1000; i++);

	//find_nand_clk_delay();

	imi_printf("\n\n\n");
	test_nand_data();
	//test_nand_byte();
	imi_printf("Nand test end ========\n");
	while(1);
}

