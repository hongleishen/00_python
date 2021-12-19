#ifndef __UART_TASK_H__
#define __UART_TASK_H__
#include "vic_int.h"

#define  BUF_SIZE  64 // for no use uart queue

void uart_task( void *pvParameters );

int uart_get_char (char *ch);
irqreturn_t uart_irq_handle(int irq, void *arg);

#endif
