/*
#include "utils.h"
#include "spi_nor.h"
#include "spi_nand.h"
#include "my_imi_printf.h"
*/
//#include "comdef.h"

/*
//#include "utils.h"
//#include "comdef.h"
#include "spi_nor.h"
#include "spi_bbm.h"
//#include "my_imi_printf.h"
//#include "timer.h"
#include "global.h"
#include "uart.h"
*/

#include "candence_qspi.h"
#include "spi_nor.h"
#include "uart.h"
#include "printf.h"
#include "imi_type.h"
#include "linux_compat.h"

void print_nor_int(int *buf, int n)
{
	for (int i = 0; i < n; i++) {
		imi_printf("%d ", buf[i]);

		if ( (i + 1)%16 == 0) {
			uart_puts("\r\n");
		}
	}
}

void print_buf_32(u32 *buf, int n)
{
	//imi_printf("\n");
	uart_puts("[0x00] [0x00]:	");

	for (int i = 0; i < n; i++) {
		imi_printf("%x ", (u32)buf[i]);

		if ( i < n-1 && (i + 1)%16 == 0) {
			uart_puts("\r\n");

			if ((i+1)%0x400 == 0) {		//sector: 0x400 world, 0x1000 byte = 4k byte, 1 sector
				uart_puts("\r\n\r\n");
				imi_printf("sector %d:\n", (i+1)/0x400 + 1);
			}

			if ((i+1)%0x100 == 0) {		//1k: 0x100 world, 0x400 = 1k byte
				//uart_puts("\r\n");
				imi_printf("-- %dk byte:\r\n", (i+1)/0x100 +1);
			}

			if ((i+1)%0x40 == 0) {		//page:	0x40 world, 0x100 byte = 256 byte, 1 page
				imi_printf("-page: %d\r\n", (i+1)/0x40 +1);
			}

			imi_printf("[0x%x] [0x%x]:	", i+1, (i+1)*4);
		}
	}
	uart_puts("\r\n");

	return;

}



#define TEST_SIZE	1024
//test with world
static u32 wbuf[TEST_SIZE] = {0};
static u32 rbuf[TEST_SIZE] = {0};
int quad_mode_s = 0;

int quad_mode_w = 1;
int quad_mode_r = 0;

u32 test_nor_data(void)
{
	int i = 0;
	int ret;

	u32 addr = 	0X100000;
	u32 numbyte = TEST_SIZE;
	u32 num_world = TEST_SIZE / 4;

	//spi_nor_write_volatile_sr_enable();


	//clk_config(4, 0);    //--------------------------------

	// 1.data 0922
	for (i = 0; i < numbyte/4; i++) {
		wbuf[i] = i;
	}

#if 1
	// 2.write
	ret = nor_flash_write(addr, (char *)wbuf, numbyte);
	if (ret != 0)
		imi_printf("write error, ret = %d\n", ret);

	//nor_flash_write(addr + 0x100000, (char *)wbuf, numbyte);
#endif

#if 1
	//2m data
	//nor_flash_read( addr + 0x100000, numbyte, (char *)rbuf);
	//uart_puts("rdata + 2M:\r\n");
	//print_buf_32((U32 *)rbuf, num_world);
	uart_puts("\r\n\r\n");

	// 3. read
	//1M
	//memset((unsigned char *)rbuf, 0, TEST_SIZE);
	for (i = 0; i < TEST_SIZE; i++)
		rbuf[i] = 0;
	ret = nor_flash_read( addr, numbyte, (char *)rbuf);
	if (ret != 0)
		imi_printf("read error, ret = %d\n", ret);

	uart_puts("rdata 1M:\r\n");
	print_buf_32((U32 *)rbuf, num_world);
#endif



	//4. check data --------------------------
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
		imi_printf("wrong index is: %d\r\n", i);
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
void print_nor_byte(u8 *buf, int n)
{
	uart_puts("\r\n");
	for (int i = 0; i < n; i++) {
		imi_printf("%d ", buf[i]);

		if ( (i + 1)%16 == 0) {
			uart_puts("\r\n");
		}
	}
}

u32 test_nor_byte(void)
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

#if 0
	//write -----------------------------
	ret = nor_flash_write(addr, (char *)wbuf, numbyte);
	if (ret != 0)
		imi_printf("write error, ret = %d\n", ret);
#endif

	//read ------------------------------
	memset(rbuf, 0 , numbyte);
	ret = nor_flash_read( addr, numbyte, (char *)rbuf);
	if (ret != 0)
		imi_printf("read error, ret = %d\n", ret);

	imi_printf("rdata:%d\n", ret);
	print_nor_byte(rbuf, numbyte);


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

//calibration
//find_write_div_delay();
//find_read_div_delay();
//QSPI_INST_TYPE_QUAD_MODE

u32 set_nor_max_clk(void)
{
	int div = 0;
	int delay = 0;
	int ret = -1;


	for (div = 0; div < 16; div++) {
		imi_printf("try div = %d, clk = %d\n", div, 120/(2 * (div + 1)));
		for (delay = 0; delay < 6; delay ++) {
			imi_printf("\n	try delay = %d\n", delay);

			candence_clk_delay_config(div, delay);
			ret = test_nor_data();
			if (ret == 0) {
				imi_printf("successfull: div = %d, delay = %d\n", div, delay);
				candence_clk_delay_config(div, delay);
				return 0;
			}
		}
	}

	imi_printf("fail find div delay\n");
	return -1;
}



void nor_test(void)
{
	uart_puts("\r\nNor test-----------\n");
	//test_nor_byte();
	for(int i = 0; i < 1000; i++);

	//set_nor_max_clk();

	imi_printf("\n\n\n");
	test_nor_data();

	imi_printf("Nor test end ========\n");
	while(1);
}


