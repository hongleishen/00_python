#include "elog_init.h"
#include "initcalls.h"

//#define LOG_TAG  "elog_init"


void imi_elog_init(void)
{
    /* initialize EasyLogger */
    elog_init();

	//elog_set_text_color_enabled(true);

    /* set EasyLogger log format */
    elog_set_fmt(ELOG_LVL_ASSERT,ELOG_FMT_ALL & ~(ELOG_FMT_T_INFO | ELOG_FMT_P_INFO|ELOG_FMT_DIR));
    elog_set_fmt(ELOG_LVL_ERROR,ELOG_FMT_ALL & ~( ELOG_FMT_T_INFO | ELOG_FMT_P_INFO|ELOG_FMT_DIR));
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_ALL & ~( ELOG_FMT_T_INFO | ELOG_FMT_P_INFO|ELOG_FMT_DIR));
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_ALL & ~( ELOG_FMT_T_INFO | ELOG_FMT_P_INFO|ELOG_FMT_DIR));
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~( ELOG_FMT_T_INFO | ELOG_FMT_P_INFO|ELOG_FMT_DIR));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_T_INFO | ELOG_FMT_P_INFO|ELOG_FMT_DIR));

	//elog_set_filter_lvl(ELOG_LVL_WARN);
   // elog_set_filter_kw("error");
    /* start EasyLogger */
    elog_start();
}

/**
 * EasyLogger demo
 */
void test_elog(void)
{
    /* test log output for all level */
    log_e("Hello EasyLogger\r\n");
    log_w("Hello EasyLogger\r\n");
    log_i("Hello EasyLogger\r\n");
    log_d("Hello EasyLogger\r\n");
    log_v("Hello EasyLogger\r\n");

}
#ifndef TARGET_FPGA_SIMULATION
EARLY_INIT(imi_elog_init);
#endif
