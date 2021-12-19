#ifndef _IMI_CONSOLE_H
#define _IMI_CONSOLE_H

#define  CONS_CMD_SIZE		 (512)
#define CONS_OUT_SIZE		 (1024)
#define LOCAL_MAX_TASK_NUM	 (10)
#define RUN_STAT_BUF_SIZE	 (6)

extern int sprintf(char *buf, const char *fmt, ...);
void imi_CLIRegisterCommand(void);
void parse_cmd(const S8 *str, int32 len);

#endif
