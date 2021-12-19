#include "imi_io.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include "imi_type.h"
#include "printf.h"
#include "timer_imi.h"

#define OUTBUFSIZE  1024
static S8 log_sem = 0;

enum {
	FREE,
	IN_USE,
};

static char log_buf[OUTBUFSIZE];

static int outc(int c)
{
	uart_putc(c);
	return 0;
}

static int outs (const char *s)
{
	while (*s != '\0')
		outc(*s++);
	return 0;
}

volatile int printf(const char *fmt, ...)
{
	int len;
	va_list args;

	if (log_sem == IN_USE) {
		return -1;
	}
	log_sem = IN_USE;

	va_start(args, fmt);
	len = imi_vsnprintf(log_buf, OUTBUFSIZE - 1, fmt, args);
	va_end(args);
	log_buf[len] = '\0';

	outs(log_buf);

	log_sem = FREE;

	return len;
}

int imi_printf(const char *fmt, ...)
{
	va_list ap;
	int len;

	if (log_sem == IN_USE) {
		return -1;
	}
	log_sem = IN_USE;

	va_start(ap, fmt);
	len = imi_vsnprintf(log_buf, OUTBUFSIZE - 1, fmt, ap);
	va_end(ap);
	log_buf[len] = '\0';
	outs(log_buf);

	log_sem = FREE;
	return 0;
}

void print_cur_time(void)
{
	struct timeval_t cur;

	imi_get_cur_time(&cur);
	imi_printf("%ld:%ld  ", cur.tv_sec, cur.tv_usec);
}

int sprintf(char *buf, const char *fmt, ...)
{
	int len;
	va_list args;

	va_start(args, fmt);
	len = vsprintf(buf,fmt,args);
	va_end(args);

	return len;
}
