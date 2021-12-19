#include "initcalls.h"

typedef void (* func_call)(void);
_INIT_START func_call func_start;
_INIT_END func_call func_end;

void InitCalls(void)
{
	func_call *func_ptr = &func_start + 1;

	for (; func_ptr < &func_end ; func_ptr++) {
		(*func_ptr)();
	}
}