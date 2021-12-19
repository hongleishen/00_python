#ifndef __INITCALLS_H__
#define __INITCALLS_H__

#define _INIT_START  			    __attribute__((section("._INIT_START")))
#define _INIT_EARLY  			    __attribute__((section("._INIT_EARLY")))
#define _INIT_CORE  			    __attribute__((section("._INIT_CORE")))
#define _INIT_POST_CORE  			__attribute__((section("._INIT_POST_CORE")))
#define _INIT_ARCH  			    __attribute__((section("._INIT_ARCH")))
#define _INIT_SUBSY				    __attribute__((section("._INIT_SUBSYS")))
#define _INIT_FS				    __attribute__((section("._INIT_FS")))
#define _INIT_DEV				    __attribute__((section("._INIT_DEV")))
#define _INIT_LATE				    __attribute__((section("._INIT_LATE")))
#define _INIT_END				    __attribute__((section("._INIT_END")))

#define EARLY_INIT(func)			_INIT_EARLY void (* _early_##func)(void) = func
//core : clk /reset /power config
#define CORE_INIT(func)			    _INIT_CORE void (* _core_##func)(void) = func
#define POST_CORE_INIT(func)		_INIT_POST_CORE void (* _post_core_##func)(void) = func
//arch controller init
#define ARCH_INIT(func)			    _INIT_ARCH void (* _arch_##func)(void) = func
#define SUBSYS_INIT(func)			_INIT_SUBSY void (* _subsys_##func)(void) = func
#define FS_INIT(func)			    _INIT_FS void (* _fs_##func)(void) = func
#define DEV_INIT(func)			    _INIT_DEV void (* _dev_##func)(void) = func
#define LATE_INIT(func)			    _INIT_LATE void (* _late_##func)(void) = func

void InitCalls(void);

#endif/* __INITCALLS_H__*/

