/***************************************************************************
  Copyright (c) 2019 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    CLI
  @author   RongFeng shao

  @History
  When         Who             What, where, why
  ----------   -------------   ----------------------------------------------
  2020/09/15   RongFeng shao   the initial version

===========================================================================
***************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "FreeRTOS_CLI.h"
#include "imi_type.h"
#include "vic_int.h"
#include <string.h>
#include <stdlib.h>
#include <imi_console.h>
#include "printf.h"

char cli_output[CONS_OUT_SIZE];

static void runstat_getstat(void);
static BaseType_t testcmd(S8 *out, size_t outlen, const S8 *in);
static BaseType_t querymem(S8 *out, size_t outlen, const S8 *in);
static BaseType_t runtimestats(S8 *out, size_t outlen, const S8 *in);
static BaseType_t memdump(S8 *out, size_t outlen, const S8 *in);
static BaseType_t memrd(S8 *out, size_t outlen, const S8 *in);
static BaseType_t memwr(S8 *out, size_t outlen, const S8 *in);
static BaseType_t ar_top(S8 *out, size_t outlen, const S8 *in);
static BaseType_t irq_statistics(S8 *out, size_t outlen, const S8 *in);
static BaseType_t print_time(S8 *out, size_t outlen, const S8 *in);


static const CLI_Command_Definition_t cmd_test =
{
	"test",
	"\r\ntest:\r\n this is test information\r\n",
	testcmd,
	0
};

static const CLI_Command_Definition_t mem_info =
{
	"meminfo",
	"\r\nmeminfo:\r\n meminfo about heap.\r\n",
	querymem,
	0
};

static const CLI_Command_Definition_t runtime_stats =
{
	"stat",
	"\r\nstat:\r\n sys run time stat\r\n",
	runtimestats,
	0
};

static const CLI_Command_Definition_t mem_dump =
{
	"dump",
	"\r\ndump:\r\n dump <addr> <len>.\r\n",
	memdump,
	2
};

static const CLI_Command_Definition_t mem32 =
{
	"mem32",
	"\r\nmem32:\r\n mem32 <addr>.\r\n",
	memrd,
	1
};

static const CLI_Command_Definition_t w4 =
{
	"w4",
	"\r\nw4:\r\n w4 <addr> <val>.\r\n",
	memwr,
	2
};

static const CLI_Command_Definition_t top =
{
	"top",
	"\r\ntop:\r\ntop info.\r\n",
	ar_top,
	0
};

static const CLI_Command_Definition_t cmd_irq_statistics =
{
	"irq",
	"\r\nirq:\r\n this is irq statistics\r\n",
	irq_statistics,
	0
};

static const CLI_Command_Definition_t cmd_print_time =
{
	"time",
	"\r\ntime:\r\n return sec : msec\r\n",
	print_time,
	0
};



static BaseType_t testcmd(S8 *out, size_t outlen, const S8 *in)
{
	const S8 *coreptr;
	long plen;

	/* check arg. */
	coreptr = FreeRTOS_CLIGetParameter( in, 0, &plen);
	//imi_printf("%s\r\n",coreptr);
	if ( (coreptr != NULL)) {
		sprintf(out, "%s", cmd_test.pcHelpString);
		return -1;
	}
	sprintf(out, "this is test command !\n");
	return 0;
}


static BaseType_t querymem(S8 *out, size_t outlen, const S8 *in)
{
	const S8 *coreptr;
	long plen;
	/* check arg. */
	coreptr = FreeRTOS_CLIGetParameter( in, 0, &plen);
	if ((coreptr != NULL)) {
		sprintf(out, "%s", mem_info.pcHelpString);
		return -1;
	}

	sprintf( out, "Current free heap %d bytes, minimum ever free heap %d bytes\r\n",
		( int ) xPortGetFreeHeapSize(), ( int ) xPortGetMinimumEverFreeHeapSize() );

	return 0;
}

static BaseType_t runtimestats(S8 *out, size_t outlen, const S8 *in)
{
	const S8 *coreptr;
	long plen;
	//const s8 * const pcHeader =
	//  "\033[1mTask    Abs Time        % Time\r\n**********************************\r\n\033[0m";

	/* check arg. */
	coreptr = FreeRTOS_CLIGetParameter( in, 0, &plen);
	if ( (coreptr != NULL)) {
		sprintf(out, "%s", runtime_stats.pcHelpString);
		return -1;
	}

	runstat_getstat();
	return 0;
}

static void runstat_getstat(void)
{
	imi_printf("this is tast run state\r\n");
}

static BaseType_t memdump(S8 *out, size_t outlen, const S8 *in)
{
	const S8 *coreptr;
	S8 tmp[CONS_CMD_SIZE];
	long plen;
	u32 addr, iaddr;
	u32 len, ilen;
	int32 i, j;
	const S8 * const header =
		"\033[1m             0x00   0x01   0x02   0x03   0x04   0x05   0x06"
		"   0x07   0x08   0x09   0x0A   0x0B   0x0C   0x0D   0x0E   0x0F"
		"\r\n-----------------------------------------------"
		"---------------------------------------------------"
		"------------------------\r\n\033[0m";
	/* check arg. */

	coreptr = FreeRTOS_CLIGetParameter( in, 1, &plen);
	memcpy(tmp, coreptr, plen);
	addr = (u32)strtoul(tmp, NULL, 0);
	memset(tmp, 0, CONS_CMD_SIZE);

	coreptr = FreeRTOS_CLIGetParameter( in, 2, &plen);
	memcpy(tmp, coreptr, plen);
	len = (u32)strtoul(tmp, NULL, 0);

	iaddr = addr & (~0xF);
	ilen = (len >> 4) + 1;
	imi_printf("%s", header);

	for (i = 0 ; i < ilen ; i++) {
		printf("\033[1m0x%08x\033[0m", (iaddr + i * 0x10));

		for (j = 0 ; j < 16 ; j++) {
			printf("   0x%02x", (u8)read8(iaddr + i * 0x10 + j));
			if (j == 15) {
				imi_printf("\n");
			}
		}
	}
	return 0;
}

static BaseType_t memrd(S8 *out, size_t outlen, const S8 *in)
{
	S8 tmp[CONS_CMD_SIZE];
	const S8 *coreptr;
	long plen;
	u32 addr;
	/* check arg. */
	coreptr = FreeRTOS_CLIGetParameter( in, 0, &plen);
	if ( (coreptr != NULL)) {
		sprintf(out, "%s", mem32.pcHelpString);
		return -1;
	}

	coreptr = FreeRTOS_CLIGetParameter( in, 1, &plen);
	memcpy(tmp, coreptr, plen);
	addr = (u32)strtoul(tmp, NULL, 0);

	sprintf( out, "0x%x\r\n", readl(addr));
	return 0;
}

static BaseType_t memwr(S8 *out, size_t outlen, const S8 *in)
{
	S8 tmp[CONS_CMD_SIZE];
	const S8 *coreptr;
	long plen;
	u32 addr, val;
	/* check arg. */
	coreptr = FreeRTOS_CLIGetParameter( in, 0, &plen);
	if ( (coreptr != NULL)) {
		sprintf(out, "%s", mem32.pcHelpString);
		return -1;
	}

	coreptr = FreeRTOS_CLIGetParameter( in, 1, &plen);
	memcpy(tmp, coreptr, plen);
	addr = (u32)strtoul(tmp, NULL, 0);
	memset(tmp, 0, CONS_CMD_SIZE);

	coreptr = FreeRTOS_CLIGetParameter( in, 2, &plen);
	memcpy(tmp, coreptr, plen);
	val = (u32)strtoul(tmp, NULL, 0);
	writel(val, addr);
	sprintf( out, "0x%x <-- 0x%x\r\n", addr, val);

	return 0;
}

void _ar_top(void)
{
	const char task_state[]={'r','R','B','S','D'};
	volatile UBaseType_t uxArraySize, x;
	unsigned int ulTotalRunTime;
	unsigned int ulStatsAsPercentage;
	TaskStatus_t pxTaskStatusArray[LOCAL_MAX_TASK_NUM];

	/* require the total tasks */
	uxArraySize = uxTaskGetNumberOfTasks();

	if(uxArraySize>LOCAL_MAX_TASK_NUM)
	{
		imi_printf("too many tasks\n");
		return;
	}
	/* require each task struct information */
	uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);

	imi_printf("name                            status ID Priority   stack   used\n");

	if( ulTotalRunTime > 0 )
	{
		for( x = 0; x < uxArraySize; x++ )
		{
				char tmp[128];

			/* caculate the total run time and total percent time */
			ulStatsAsPercentage =(u64)(pxTaskStatusArray[x].ulRunTimeCounter)*100 /
			ulTotalRunTime;

			if( ulStatsAsPercentage > 0UL )
			{
				sprintf(tmp,"%-32s%-7c%-6ld%-8ld%-8d%d%%",pxTaskStatusArray[x].pcTaskName,
					task_state[pxTaskStatusArray[x].eCurrentState],
					pxTaskStatusArray[x].xTaskNumber,pxTaskStatusArray[x].uxCurrentPriority,
					pxTaskStatusArray[x].usStackHighWaterMark,ulStatsAsPercentage);
			}
			else
			{
				/* cpu used not more than 1 */
				sprintf(tmp,"%-32s%-7c%-6ld%-8ld%-8dt<1%%",pxTaskStatusArray[x].pcTaskName,
						task_state[pxTaskStatusArray[x].eCurrentState],
						pxTaskStatusArray[x].xTaskNumber,pxTaskStatusArray[x].uxCurrentPriority,
						pxTaskStatusArray[x].usStackHighWaterMark);
			}
			imi_printf("%s\n",tmp);
		}
	}
	return;
}

static BaseType_t ar_top(S8 *out, size_t outlen, const S8 *in)
{
	_ar_top();
	return 0;
}

BaseType_t irq_statistics(S8 *out, size_t outlen, const S8 *in)
{
	vic_print_irq_statistics();
	return 0;
}

BaseType_t print_time(S8 *out, size_t outlen, const S8 *in)
{
	print_cur_time();
	return 0;
}

void parse_cmd(const S8 *str, int32 len)
{
	S8 tmp[CONS_CMD_SIZE] = {0};
	const S8 *coreptr;
	long plen;

	memcpy(tmp, str, len);
	coreptr = FreeRTOS_CLIGetParameter( tmp, 0, &plen);
	if ( (coreptr != NULL)) {
		return ;
	}
	FreeRTOS_CLIProcessCommand(tmp, cli_output,
		sizeof(cli_output));
	imi_printf("%s\n", cli_output);
	memset(cli_output, 0, sizeof(cli_output));

}

void imi_CLIRegisterCommand(void)
{
	FreeRTOS_CLIRegisterCommand(&cmd_test);
	FreeRTOS_CLIRegisterCommand(&mem_info);
	//FreeRTOS_CLIRegisterCommand(&runtime_stats);
	FreeRTOS_CLIRegisterCommand(&mem_dump);
	FreeRTOS_CLIRegisterCommand(&mem32);
	FreeRTOS_CLIRegisterCommand(&w4);
	FreeRTOS_CLIRegisterCommand(&top);
	FreeRTOS_CLIRegisterCommand(&cmd_irq_statistics);
	FreeRTOS_CLIRegisterCommand(&cmd_print_time);
}


