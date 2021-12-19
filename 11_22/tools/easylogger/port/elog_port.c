/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2015-04-28
 */

#include "elog.h"
#include "imi_io.h"
#include "portmacro.h"
#include "imi_type.h"
#include "elog_init.h"
#include "printf.h"
/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
	ElogErrCode result = ELOG_NO_ERR;

	/* add your code here */



	return result;
}

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 */
void elog_port_output(const char *log, size_t size) {

	/* add your code here */
	char print_log[size+1];
	int i=0;
	for(i=0;i<size;i++)
	{
		print_log[i] = log[i];
	}
	print_log[size] = '\0';
	#ifndef TARGET_FPGA_SIMULATION
		print_cur_time();
		printf("%s",print_log);
	#else
		imi_printf("%s",print_log);
	#endif // !
}

/**
 * output lock
 */
void elog_port_output_lock(void) {

	/* add your code here */
	//portDISABLE_INTERRUPTS();
}

/**
 * output unlock
 */
void elog_port_output_unlock(void) {

	/* add your code here */
	//portENABLE_INTERRUPTS();
}

/**
 * get current time interface
 *
 * @return current time
 */

const char *elog_port_get_time(void) {

	/* add your code here */
	 return "";
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) {

	/* add your code here */
	//return "pid:1008";
	return " ";

}

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) {

	/* add your code here */
	//return "tid:24";
	return " ";

}
#if 0
char* strcpy(char* des,const char* source)
{
	char* r=des;
	assert((des != NULL) && (source != NULL));
	while((*r++ = *source++)!='\0');
	return des;
}
#endif
char* retstring(char a[])
{
	static char b[20];
	strcpy(b,a);
	return b;
}
