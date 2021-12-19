/***************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    camera driver
  @author   Dejie Zhang

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------
  2021/01/15   Dejie Zhang   the initial version

***************************************************************************/
#if defined(CHIP_IMI_1680)
#include "csi_rx.h"
#include "sensor_def.h"
#include "sensor_list.h"
#include "vic_int.h"
#include "imgsensor.h"
#include "i2c.h"
#include "mipi_gpio.h"
#include "delay.h"
#include "elog.h"
#include "timer_imi.h"

#define GPIO_IMGSEN_XCLK		22//34
#define GPIO_IMGSEN_STROBE		36
#define GPIO_IMGSEN_TRIGGER		37
#define GPIO_IMGSEN_SHUTDOWN	35//33
#define GPIO_IMGSEN_RESET		33//32

static PIMI_CAM_FUNCS gl_irsnr_drv;
static PIMI_CAM_FUNCS gl_rgbsnr_drv;

/***********************************************************************
 *
 * Common funcs
 *
 ***********************************************************************/
#if 1
void sensor_pinmux_init(IMI_CAMPW_T pw)
{
	pinmux_select_gpio(pw->strbp);
	pinmux_select_gpio(pw->shutdnp);
	pinmux_select_gpio(pw->trigp);
	pinmux_select_gpio(pw->rstp);
	pinmux_select_func(pw->mclkp);
}

int power_up_sensor(IMI_CAMPW_T pw)
{
	log_d("sensor power up ...\n");
	gpio_bus_enable_old();
	sensor_pinmux_init(pw);
	extclk_dir_out_old();

	imi_printf("strobe = %d, shutdownz = 0x%d\n", pw->strbp, pw->shutdnp);
	imi_printf("trigger = %d, reset = 0x%d\n", pw->trigp, pw->rstp);
	imi_printf("mclk = 0x%d\n", pw->mclkp);

	udelay(1);
	gpio_out_pulldown(pw->shutdnp);
	udelay(5);
	gpio_out_pullup(pw->shutdnp);

	udelay(1);
	gpio_out_pulldown(pw->rstp);
	udelay(5);
	gpio_out_pullup(pw->rstp);

	gpio_out_pulldown(pw->trigp);
	log_d("sensor power'ed up\n");

	return 0;
}
#else

void sensor_pinmux_init(void)
{
	pinmux_select_gpio(GPIO_IMGSEN_STROBE);
	pinmux_select_gpio(GPIO_IMGSEN_TRIGGER);
	pinmux_select_gpio(GPIO_IMGSEN_SHUTDOWN);
	pinmux_select_gpio(GPIO_IMGSEN_RESET);
	pinmux_select_func(GPIO_IMGSEN_XCLK);
}
int power_up_sensor(IMI_CAMPW_T pw)
{
	imi_printf("sensor power up ...\n");
	gpio_bus_enable_old();
	sensor_pinmux_init();
	extclk_dir_out_old();
	udelay(1);
	gpio_out_pulldown(GPIO_IMGSEN_SHUTDOWN);
	udelay(5);
	gpio_out_pullup(GPIO_IMGSEN_SHUTDOWN);
	udelay(1);
	gpio_out_pulldown(GPIO_IMGSEN_RESET);
	udelay(5);
	gpio_out_pullup(GPIO_IMGSEN_RESET);
	imi_printf("sensor power'ed up\n");

	return 0;
}
#endif

void trig_sensor_stream(IMI_CAMPW_T pw)
{
	gpio_direction_output(27, 0);
	gpio_out_pulldown(pw->trigp);
	gpio_out_pullup(pw->trigp);
	udelay(1000);
	gpio_out_pulldown(pw->trigp);

	gpio_direction_output(27, 1);
}

irqreturn_t common_timer1_handler(int irq, void *arg)
{
	int timer = TIMER1;

	imi_timer_int_clr(timer);
	trig_sensor_stream((IMI_CAMPW_T)arg);
	//log_d("common_timer1_handler, trig = %d\n", ((IMI_CAMPW_T)arg)->trigp);

	return IRQ_HANDLED;
}

void triger_start(IMI_CAMPW_T pw)
{
	u32 timer = TIMER1;
	u32 mode = MODE_RELOAD;
	u32 intr = WDOG_INT_EN;
	u32 period = 0x2625A00;//1s

	request_irq(IRQ_TIMER_1, common_timer1_handler, "timer1", pw);
	imi_timer_en(timer, mode, intr, period);
}

/***********************************************************************
 *
 * Sim SENSOR driver funcs
 *
 ***********************************************************************/

int sim_cam_config(PIMI_CAM_PARAMS simcam)
{
	simcam->again       = 1;
	simcam->fps         = 30;
	simcam->width       = 1280;
	simcam->height      = 800;
	simcam->sysclk      = 80;
	simcam->exposure    = 1000;
	simcam->vts         = 0xFA0;
	simcam->hts         = 0x4A0;
	simcam->trigen      = 0;
	simcam->hdren       = SNR_HDR_DISABLE;

	simcam->pw.mclkp    = 22;
	simcam->pw.strbp    = 23;
	simcam->pw.trigp    = 8;
	simcam->pw.shutdnp  = 26;
	simcam->pw.rstp     = 32;

	simcam->mipi_cfg.id      = 1;
	simcam->mipi_cfg.mbps    = 270;
	simcam->mipi_cfg.height  = simcam->height;
	simcam->mipi_cfg.width   = simcam->width;
	simcam->mipi_cfg.data_t  = MIPI_RAW_10BIT;
	simcam->mipi_cfg.cfg_clk = 24000000;
	simcam->mipi_cfg.lanes   = 2;
	switch (simcam->hdren) {
	case SNR_HDR_3_FRAME:
		simcam->mipi_cfg.vcno    = 3;
		break;
	case SNR_HDR_2_FRAME:
		simcam->mipi_cfg.vcno    = 2;
		break;
	case SNR_HDR_DISABLE:
	default:
		simcam->mipi_cfg.vcno    = 1;
		break;
	}
	simcam->mipi_cfg.flags   = 0;

#if (VERISLC_DPHY_SIMULAT == 1)
	simcam->mipi_cfg.addr[0] = 0x90000000;
	simcam->mipi_cfg.addr[1] = 0x92000000;
	simcam->mipi_cfg.addr[2] = 0x94000000;
#else
	simcam->mipi_cfg.addr[0] = 0xA0000000;
	simcam->mipi_cfg.addr[1] = 0xA2000000;
	simcam->mipi_cfg.addr[2] = 0xA4000000;
#endif

	return 0;
}

/***********************************************************************
 *
 * IR SENSOR driver funcs
 *
 ***********************************************************************/

int ir_cam_config(PIMI_CAM_PARAMS ircam)
{
	ircam->again       = 1;
	ircam->fps         = 30;
	ircam->width       = 1280;
#if (SYNPSYS_DPHY_TESTCHIP == 1)
	ircam->height      = 960;
#else
	ircam->height      = 800;
#endif
	ircam->sysclk      = 80;
	ircam->exposure    = 1000;
	ircam->vts         = 0xFA0;
	ircam->hts         = 0x4A0;
	ircam->trigen      = 0;
	ircam->hdren       = SNR_HDR_DISABLE;

	ircam->pw.mclkp    = 22;
	ircam->pw.strbp    = 23;
	ircam->pw.trigp    = 11;//8;
	ircam->pw.shutdnp  = 26;
	ircam->pw.rstp     = 32;

	ircam->mipi_cfg.id      = 0;
	ircam->mipi_cfg.mbps    = 270;
	ircam->mipi_cfg.height  = ircam->height;
	ircam->mipi_cfg.width   = ircam->width;
	ircam->mipi_cfg.data_t  = MIPI_RAW_10BIT;
	ircam->mipi_cfg.cfg_clk = 24000000;
	ircam->mipi_cfg.lanes   = 2;
	switch (ircam->hdren) {
	case SNR_HDR_3_FRAME:
		ircam->mipi_cfg.vcno    = 3;
		break;
	case SNR_HDR_2_FRAME:
		ircam->mipi_cfg.vcno    = 2;
		break;
	case SNR_HDR_DISABLE:
	default:
		ircam->mipi_cfg.vcno    = 1;
		break;
	}
	ircam->mipi_cfg.flags   = 0;

#if (VERISLC_DPHY_SIMULAT == 1)
	ircam->mipi_cfg.addr[0] = 0x90000000;
	ircam->mipi_cfg.addr[1] = 0x92000000;
	ircam->mipi_cfg.addr[1] = 0x94000000;
#else
	ircam->mipi_cfg.addr[0] = 0xA0000000;
	ircam->mipi_cfg.addr[1] = 0xA2000000;
	ircam->mipi_cfg.addr[2] = 0xA4000000;
#endif

	return 0;
}

int ircam_setup_driver(PIMI_CAM_PARAMS ircam)
{
	int ret = 0;
	IMI_SENSOR_LIST *entry;
	unsigned int tmp_id = 0;

	log_d("Enter %s\n", __func__);
	for ( entry = &ir_sensor_list[0]; ; entry ++ ) {

		if ((entry->id == 0) && (entry->name == NULL) \
			&& (entry->sensor_init == NULL)) {
			log_e("[ERR]: No matching imgsensor driver\n");
			ret = -1;
			break;
		}

		if(entry->sensor_init(&gl_irsnr_drv)) {
			log_e("[ERR]: Get Sensor Funcs \n");
			return ret;
		}

		tmp_id = gl_irsnr_drv->sensor_open(ircam);
		if ( tmp_id != (unsigned int)entry->id ) {
			continue;
		} else {
			log_i("Sensor name: %s, sensor id: 0x%x\n", entry->name, entry->id);
			gl_irsnr_drv->sensor_close(ircam);
			break;
		}
	}
	log_d("------------- ircam_setup_driver ok, id = 0x%x\n", tmp_id);

	log_d("Exit %s\n", __func__);
	return ret;
}

int ir_sensor_init(PIMI_CAM_PARAMS ircam)
{
	int ret = 0;
	log_d("===== sensor init Enter =====\n");
	power_up_sensor(&(ircam->pw));
	ret = ircam_setup_driver(ircam);
	if (ret) {
		log_e("[ERR]: Can't find the driver\n");
		return -1;
	}
	log_d("Find IR sensor driver\n");
	
	gl_irsnr_drv->sensor_open(ircam);
	gl_irsnr_drv->sensor_regs_init(ircam);

	log_d("===== sensor init OK =====\n");

	return 0;
}

int start_irsnr_stream(PIMI_CAM_PARAMS ircam)
{
	gl_irsnr_drv->sensor_start_stream(ircam);
	return 0;
}

int ir_cam_test_init(void)
{
	static IMI_CAMPW_T g_campw;
	static IMI_CAM_PARAMS ircam;
	log_d("Enter ir_cam_init\n");

	ir_cam_config(&ircam);
	ircam.trigen = 1;
	g_campw = &(ircam.pw);

	mipi_init(&(ircam.mipi_cfg));

	ircam.dev = imi_i2c_init(I2C_1);

	ir_sensor_init(&ircam);

	start_irsnr_stream(&ircam);
	//trig_sensor_stream();
	triger_start(g_campw);
	return 0;
}

/***********************************************************************
 *
 * IR SENSOR driver funcs
 *
 ***********************************************************************/

int rgb_cam_config(PIMI_CAM_PARAMS rgbcam)
{
	rgbcam->again        = 1;
	rgbcam->fps          = 30;
	rgbcam->width        = 1920;
	rgbcam->height       = 1080;
	rgbcam->sysclk       = 80;
	rgbcam->exposure     = 1000;
	rgbcam->vts          = 0xFA0;
	rgbcam->hts          = 0x4A0;
	rgbcam->trigen       = 0;
	rgbcam->hdren        = SNR_HDR_DISABLE;

	rgbcam->pw.mclkp     = 22;
	rgbcam->pw.strbp     = 36;
	rgbcam->pw.trigp     = 37;
	rgbcam->pw.shutdnp   = 35;
	rgbcam->pw.rstp      = 33;

	rgbcam->mipi_cfg.id      = 0;
	rgbcam->mipi_cfg.mbps    = 270;
	rgbcam->mipi_cfg.height  = rgbcam->height;
	rgbcam->mipi_cfg.width   = rgbcam->width;
	rgbcam->mipi_cfg.data_t  = MIPI_RAW_12BIT;
	rgbcam->mipi_cfg.cfg_clk = 24000000;
	rgbcam->mipi_cfg.lanes   = 2;
	switch (rgbcam->hdren) {
	case SNR_HDR_3_FRAME:
		rgbcam->mipi_cfg.vcno    = 3;
		break;
	case SNR_HDR_2_FRAME:
		rgbcam->mipi_cfg.vcno    = 2;
		break;
	case SNR_HDR_DISABLE:
	default:
		rgbcam->mipi_cfg.vcno    = 1;
		break;
	}

	rgbcam->mipi_cfg.flags   = 0;

#if (VERISLC_DPHY_SIMULAT == 1)
	rgbcam->mipi_cfg.addr[0] = 0x90000000;
	rgbcam->mipi_cfg.addr[1] = 0x92000000;
	rgbcam->mipi_cfg.addr[1] = 0x94000000;
#else
	rgbcam->mipi_cfg.addr[0] = 0xB0000000;
	rgbcam->mipi_cfg.addr[1] = 0xB2000000;
	rgbcam->mipi_cfg.addr[2] = 0xB4000000;
#endif
	imi_printf("rgb camera config\n");
	return 0;
}

int rgbcam_setup_driver(PIMI_CAM_PARAMS rgbcam)
{
	int ret = 0;
	IMI_SENSOR_LIST *entry;
	unsigned int tmp_id = 0;

	log_d("Enter %s\n", __func__);
	for ( entry = &rgb_sensor_list[0]; ; entry ++ ) {

		if ((entry->id == 0) && (entry->name == NULL) \
			&& (entry->sensor_init == NULL)) {
			log_e("[ERR]: No matching rgb sensor driver\n");
			ret = -1;
			break;
		}

		if(entry->sensor_init(&gl_rgbsnr_drv)) {
			log_e("[ERR]: Get Sensor Funcs \n");
			return ret;
		}

		tmp_id = gl_rgbsnr_drv->sensor_open(rgbcam);
		if ( tmp_id != (unsigned int)entry->id ) {
			continue;
		} else {
			log_i("Sensor name: %s, sensor id: 0x%x\n", entry->name, entry->id);
			gl_rgbsnr_drv->sensor_close(rgbcam);
			break;
		}
	}
	log_d("------------- rgbcam_setup_driver ok, id = 0x%x\n", tmp_id);
	log_d("Exit %s\n", __func__);
	return ret;
}

int rgb_sensor_init(PIMI_CAM_PARAMS rgbcam)
{
	int ret = 0;
	imi_printf("===== sensor init Enter =====\n");
	power_up_sensor(&(rgbcam->pw));
	ret = rgbcam_setup_driver(rgbcam);
	if (ret) {
		log_e("[ERR]: Can't find the driver\n");
		return -1;
	}
	imi_printf("open sensor\n");
	gl_rgbsnr_drv->sensor_open(rgbcam);
	// do some important regs init
	imi_printf("Sensor regs init\n");
	gl_rgbsnr_drv->sensor_regs_init(rgbcam);
	imi_printf("===== sensor init OK =====\n");

	return 0;
}

int start_rgbsnr_stream(PIMI_CAM_PARAMS rgbcam)
{
	gl_rgbsnr_drv->sensor_start_stream(rgbcam);
	return 0;
}

int rgb_cam_test_init(void)
{
	IMI_CAM_PARAMS rgbcam;

	//log_d("Enter ir_cam_init\n");
	imi_printf("Enter ir_cam_init, &rgbcam = 0x%x\n", (unsigned int)&rgbcam);

	rgb_cam_config(&rgbcam);
	
	rgbcam.dev = imi_i2c_init(I2C_1);

	mipi_init(&(rgbcam.mipi_cfg));

	rgb_sensor_init(&rgbcam);
	imi_printf("sensor_init\n");
	start_rgbsnr_stream(&rgbcam);

	return 0;
}
#endif
