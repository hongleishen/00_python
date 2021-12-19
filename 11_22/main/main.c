/***************************************************************************
  Copyright (c) 2019 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    main.c
  @author

  @History
  When         Who             What, where, why
  ----------   -------------   ----------------------------------------------
  2020/07/13   RongFeng shao   the initial version

===========================================================================
***************************************************************************/
#include "uart.h"
#include "FreeRTOS.h"
#include "imi_type.h"
#include "task.h"
#include "queue.h"
#include "uart_task.h"
#include "initcalls.h"
#include "string.h"
#include "cpu.h"
#include "clock.h"
#include "imi_ipc.h"
#include "imi_bm.h"
#include "board.h"

#define JTAG			0

extern void icache_enable(void);
extern void dcache_enable(void);

//UART task parameter
#define      UART_TASK_PRIO         1
#define      UART_TASK_STK_SIZE     1024
TaskHandle_t UART_Handler;

//qspi task parameter
#define      QSPI_TASK_PRIO         1
#define      QSPI_TASK_STK_SIZE     1024
TaskHandle_t QSPI_Handler;


void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* If configCHECK_FOR_STACK_OVERFLOW is set to either 1 or 2 then this
	function will automatically get called if a task overflows its stack. */
	( void ) pxTask;

	imi_printf("tsk: %s stack over flow\n",pcTaskName);
}

void InitSysCtrl(void)
{
	clock_dev_init();
}

extern int imi_vendor_device_init(void);


#if JTAG
int *breakpoint = (int*)0x98000000;
int *breakpoint2 = (int*)0x98000010;
#endif
#if defined(CHIP_IMI_2280)
extern void imi_task_init(void);
#endif




extern void nand_test(void);
extern void nor_test(void);
void xl_test( void *pvParameters )
{
	int bmode = get_bootmode();
	imi_printf("\n\nhello shl ,bootmode = %d\n", bmode);

	if (bmode == NAND_BOOT) {
		nand_test();
	} else {
		nor_test();
	}

	while(1);
}


int main(int value,char *agrv[])
{
	icache_enable();
	dcache_enable();

#if JTAG
	*breakpoint = 0;
	*breakpoint2 = 0;
	while(!*breakpoint);
#endif

	InitSysCtrl();
	InitCalls();

	xTaskCreate(xl_test, "xl_test", QSPI_TASK_STK_SIZE, NULL, QSPI_TASK_PRIO, &QSPI_Handler);

	xTaskCreate(uart_task, "uart_task", UART_TASK_STK_SIZE, NULL, UART_TASK_PRIO, &UART_Handler);  //who send mesg must the last create

#if defined(CHIP_IMI_2280)
//  imi_task_init();
#endif
	imi_vendor_device_init();
	vTaskStartScheduler();

	return 0;
}



