/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/


/**
 * The STR9 port can use either the watchdog or the timer 2 interrupt to generate
 * the system tick.  Set configUSE_WATCHDOG_TICK to 1 to use the watchdog, or
 * 0 to use timer 2.
 */
#define vAssertCalled(char,int) imi_printf("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)


#define configUSE_WATCHDOG_TICK			0


#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
#define configCPU_CLOCK_HZ				( ( unsigned long ) 600000000 )
#define configCPU_PERIPH_HZ				( ( unsigned long ) 24000000 )
#define configTICK_RATE_HZ				( ( TickType_t ) 1000)
#define configMAX_PRIORITIES			( 8 ) //refer to projdefs.h
#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 180 )
//#define configTOTAL_HEAP_SIZE			( ( size_t ) 52000 )
#define configMAX_TASK_NAME_LEN			( 16 )
#define configUSE_TRACE_FACILITY		1
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			1
#define configUSE_MUTEXES				1
//#define configUSE_TIME_SLICING          0
#define portTICK_TYPE_IS_ATOMIC         1
//suntongce
#define configTIMER_TASK_PRIORITY		(configMAX_PRIORITIES-1)
#define configTIMER_QUEUE_LENGTH		8
#define configTIMER_TASK_STACK_DEPTH	(4096)
#define INCLUDE_xTimerPendFunctionCall 	1
#define configUSE_TIMERS				1

#define configQUEUE_REGISTRY_SIZE	    8

#define configSUPPORT_DYNAMIC_ALLOCATION	1
#define configTOTAL_HEAP_SIZE			((size_t)(300*1024))


/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 				0
#define configMAX_CO_ROUTINE_PRIORITIES 	( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_xTaskGetCurrentTaskHandle 		1

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS	1

#if ((defined TARGET_SRAM) || (defined TARGET_DDR))
#define configUSE_TIME_SLICING						1
#define configGENERATE_RUN_TIME_STATS				1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()	do{;}while(0)
#define portGET_RUN_TIME_COUNTER_VALUE()			xTaskGetTickCount()
#elif defined TARGET_FPGA
#define configUSE_TIME_SLICING						1
#define configGENERATE_RUN_TIME_STATS				1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()	do{;}while(0)
#define portGET_RUN_TIME_COUNTER_VALUE()			xTaskGetTickCount()
#endif

#define configCHECK_FOR_STACK_OVERFLOW          1

#endif /* FREERTOS_CONFIG_H */
