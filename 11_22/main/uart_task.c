#define LOG_TAG     "uart_task"
#include "FreeRTOS.h"
#include "task.h"
#include "vic_int.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include "uart_task.h"
#include "queue.h"
#include "elog_init.h"
//#include "stdbool.h"
#include "imi_console.h"

int uart_buf_processed  = 0;
int send_bytes  = 0;
char queue_uart_buff[BUF_SIZE];
char queue_uart_cmd[BUF_SIZE];
char recieve_msg_info[BUF_SIZE]={0};

static TaskHandle_t  uart_tsk_handle;

#define UART_DEBUG                      0

#define  UART_FIFO_BIT_ASSRET(n)        (read32(UART_REG_FR)&(1<<(n)))
#define  UART_ECR_BIT_ASSRET(n)         (read32(UART_REG_ECR)&(1<<(n)))
#define  UART_RIS_BIT_ASSRET(n)         (read32(UART_REG_RIS)&(1<<(n)))

u32 uart_fifo_is_empty(void)
{
	return  (read32(UART_REG_FR)&(1<<(4)));
}

u32 uart_fifo_is_error(void)
{
	return  (UART_RIS_BIT_ASSRET(10) || UART_ECR_BIT_ASSRET(3));
}

int uart_get_char (char *ch)
{
	char data;

	/* Wait until there is data in the FIFO */
	while (read32(UART_REG_FR) & UART_PL01x_FR_RXFE);

	data = read32(UART_REG_DR);
	if (data & 0xFFFFFF00) {
		/* Check for an error flag */
		write32(UART_REG_ECR, 0x0F);
		imi_printf("error!\n");
		return (char)-1;
	}
	*ch = data&0xff;

	return 0;
}

/* warning don't print in irq ,without no printf */
irqreturn_t uart_irq_handle(int irq, void *arg)
{
#define MAX_DELAY  0x10000
	char data;
	int wait_count = 0;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	/*sometimes the read is faster than uart, the fifo is empty,
	  but  '\r' is still remian in fifo  */

	while(wait_count < MAX_DELAY) {
		while(!uart_fifo_is_empty())
		{
			uart_get_char(&data);
			imi_printf("%c",data);
			if( data != '\r') {
				recieve_msg_info[send_bytes] = data;
				if(send_bytes < BUF_SIZE - 1)
					send_bytes++;
			}
			if(data == '\r') {
				if(!uart_fifo_is_error()) {
					vTaskNotifyGiveFromISR(uart_tsk_handle, &xHigherPriorityTaskWoken);
					recieve_msg_info[send_bytes] = '\0';
				}
				wait_count = MAX_DELAY;
				break;
			}
		}
		wait_count++;
		/*if recieve '\r'  don't need loop*/
	}
	/*clear the overrun buf transmit*/
	if(uart_fifo_is_error()) {
		imi_printf("overrun\r\n");
		write32(UART_REG_ECR, (1<<3));
		write32(UART_REG_ICR,(1<<10));
	}
	return IRQ_HANDLED;
}

void uart_task( void *pvParameters )
{
	memset (recieve_msg_info, 0 ,BUF_SIZE);

	uart_tsk_handle = xTaskGetCurrentTaskHandle();

#if ((defined TARGET_SRAM) || (defined TARGET_DDR))
	request_irq(IRQ_U0_UART, uart_irq_handle, NULL, NULL);
#elif defined TARGET_FPGA
#if USE_UART1
	request_irq(IRQ_UART1_RX, uart_irq_handle, NULL, NULL);
#else
	request_irq(IRQ_UART0_RX, uart_irq_handle, NULL, NULL);
#endif
#else

#endif

	imi_CLIRegisterCommand();
	imi_printf("Welcome To FreeRtos Cli Interface, Print 'help' For Cmd List\n");
	while (1) {
		imi_printf("hjimi_1680#:");
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if ((recieve_msg_info[0] >= 'a' && recieve_msg_info[0] <= 'z') ||
			(recieve_msg_info[0] >= 'A' && recieve_msg_info[0] <= 'Z')) {
			parse_cmd(recieve_msg_info, sizeof(recieve_msg_info));
		}
		memset(recieve_msg_info, 0,BUF_SIZE);
		imi_printf("\r\n");
		send_bytes = 0;
	}
}
