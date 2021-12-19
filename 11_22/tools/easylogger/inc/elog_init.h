#ifndef __ELOG_INIT_H__
#define __ELOG_INIT_H__
#include "elog.h"
#include "imi_io.h"

void imi_elog_init(void);
void test_elog(void);
char* strcpy(char* des,const char* source);
char* retstring(char a[]);
size_t strlcpy(char *dest, const char *src, size_t size);

#endif