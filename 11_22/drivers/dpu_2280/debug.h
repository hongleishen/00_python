#ifndef  __DEBUG_H__
#define  __DEBUG_H__
#include "dpu_dev.h"

#define LOG_LVL_ERR		(0x1)
#define LOG_LVL_INFO	(0x2)
#define LOG_LVL_TIMING	(0x4)
#define LOG_LVL_PARA	(0x8)
#define LOG_LVL_REPORT	(0x10)
#define LOG_LVL_COMMAND (0x20)
#define LOG_LVL_VERBOSE	(0x40)
#define LOG_LVL_DEBUG	(0x80)
#define LOG_LVL_GEOMETRICAL	(0x100)

extern int dbg_lvl;


#define DPU_LOGE(fmt,...)		do {\
								if (likely(dbg_lvl & LOG_LVL_ERR)) {\
									imi_printf("DPU_ERR "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)
#define DPU_LOGI(fmt,...)		do {\
								if (unlikely(dbg_lvl & LOG_LVL_INFO)) {\
									imi_printf("DPU_INFO "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)
#define DPU_LOGT(fmt,...)		do {\
								if (unlikely(dbg_lvl & LOG_LVL_TIMING)) {\
									imi_printf("DPU_TIMING "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)
#define DPU_LOGP(fmt,...)		do {\
								if (unlikely(dbg_lvl & LOG_LVL_PARA)) {\
									imi_printf("DPU_PARA "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)
#define DPU_LOGR(fmt,...)		do {\
								if (unlikely(dbg_lvl & LOG_LVL_REPORT)) {\
									imi_printf("DPU_REPORT "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)
#define DPU_LOGC(fmt,...)		do {\
								if (unlikely(dbg_lvl & LOG_LVL_COMMAND)) {\
									imi_printf("DPU_COMMAND "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)
#define DPU_LOGV(fmt,...)		do {\
								if (unlikely(dbg_lvl & LOG_LVL_VERBOSE)) {\
									imi_printf("DPU_VERBOSE "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)
#define DPU_LOGD(fmt,...)		do {\
								if (unlikely(dbg_lvl & LOG_LVL_DEBUG)) {\
									imi_printf("DPU_DEBUG "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)
#define DPU_LOGG(fmt,...)		do {\
								if (unlikely(dbg_lvl & LOG_LVL_GEOMETRICAL)) {\
									imi_printf("DPU_GEOMETRICAL "fmt"\n",##__VA_ARGS__);\
									}\
								} while(0)

void dump_dpu_setting(struct imi_dpu *idpu);
#endif
