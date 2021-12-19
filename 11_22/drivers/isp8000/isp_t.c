/*******************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief        isp test driver
  @creator      rain zhou

  @History
  When          Who             What, where, why
  ----------    ------------    ----------------------------------------
  2021/01/26    rain zhou       the initial version
 *******************************************************************************/
 #define LOG_TAG "isp_t"
 #include "isp_t.h"
 #include "imi_type.h"
 #include "imi_io.h"
 #include "dev.h"
 #include "clock.h"
 #include "daemon.h"
 #include "isp_data.h"
 #include "hellboy_map.h"
 #include "isp.h"
 #include "elog_init.h"
 #include "sensor_def.h"
 #include "vic_int.h"
 #include "delay.h"
 #include "i2c.h"
 #include "imgsensor.h"
 #include "isp_data1.h"

 extern uint32_t ulPortInterruptNesting;
 extern uint32_t ulPortYieldRequired;

 IMI_CAM_PARAMS hdrsensor;
 c_type	isp_case[3]={
 	{
 		.name	= "isp mcm test:",
		.num	= MCM1,
 	},{
 		.name	= "isp hdr test:",
		.num	= HDR6,
 	},{
		.name	= "isp linear test:",
		.num	= LI,
 	},
 };

 c_isp_conf isp_config = {
 		.irq	= 0,
		.sensor	= 0,
		.mipi	= 0,
		.tpg	= 0,
		.hdr	= 6,
		.ret	= 0,
 };


 void isp_t_IOCTL(unsigned int flag);

void isp_init(void)
{
	imi_printf("isp_init start. \n\r");
	log_d("isp init start.\n\r");
	clock_enable("isp");
}


int isp_t_init(int gid)
{
	imi_printf("isp_t init start.\n\r");
	test_group_register("isp_t", gid);
	test_case_register("isp_mcm", gid, isp_test_main, NULL);
	test_case_register("isp_hdr", gid, isp_test_main, NULL);
	test_case_register("isp_linear", gid, isp_test_main, NULL);
	return 0;
}

/*
static void isp_3D_config(void)
{
	denoise3d_enable = 1; // 0: Disable Denoise3D; 1: Enable Denoise3D; 
	denoise3d_dilate_en = 1; // 0: Disable dilate function; 1: Enable dilate function; 
	denoise3d_temperal_en = 1; // 0: Disable temporal filter 1: Enable temporal filter; 
	denoise3d_vertical_en = 1; // 0: Disable vertical filter; 1: Enable vertical filter; 
	denoise3d_horizontal_en = 1; // 0: Disable horizontal filter; 1: Enable horizontal filter;
	
	denoise3d_strength = 128; // Set general strength of denoise3D
	denoise3d_update_spatial = 900; // Set update parameter for spatial filter
	denoise3d_update_temporal = 900; // Set update parameter for temporal filter
	denoise3d_strength_curve_spatial = 64; // Set parameter for spatial curve
	denoise3d_strength_curve_temporal = 64; // Set parameter for temporal curve
	denoise3d_thr_edge_v_inv = 1024; // Set parameter for vertical edge detection
	denoise3d_thr_edge_h_inv = 1024; // Set parameter for horizontal edge detection
	denoise3d_range_s_inv = 1024; // Set parameter for spatial range calculation
	denoise3d_thr_motion_inv = 1024; // Set parameter for motion detection
	denoise3d_delta_t_inv = 1024; // Set parameter for temporal delta 
	denoise3d_delta_v_inv = 1024; // Set parameter for spatial vertical delta 
	denoise3d_delta_h_inv = 1024; // Set parameter for spatial horizontal delta
}
*/

static void isp_config_init(void)
{
	unsigned int temp;
	if(isp_config.tpg == 1){
		write32((ISP_BASE_ADDR+0x0700), 0x00000083);
	}
	if(isp_config.sensor == 1){
		temp = read32((ISP_BASE_ADDR+0x1200))|0x2;
		write32((ISP_BASE_ADDR+0x1200), temp);
	}else{
		temp = read32((ISP_BASE_ADDR+0x1200))&0xfffffffd;
		write32((ISP_BASE_ADDR+0x1200), temp);
	}

	if(isp_config.ret == 1){
		write32((ISP_BASE_ADDR+0x5900), 0x0000014a);
	}else{
		write32((ISP_BASE_ADDR+0x5900), 0x0000010a);
	}

	write32((ISP_BASE_ADDR+0x336c), isp_config.hdr);
			
}

static void isp_mcm(void)
{
	imi_printf("isp_t mcm test start.\n\r");
	for(int i=0; i<(sizeof(config_mcm0)/sizeof(config_mcm0[0])); i++){
		write32((ISP_BASE_ADDR+config_mcm0[i][0]), config_mcm0[i][1]);
	}


	while(1){
		imi_printf("ISP_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x05c0));
		imi_printf("MI_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16e0));
		imi_printf("MI_ISR1 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16cc));
		if(read32(ISP_BASE_ADDR+0x16e0) == 0x1000301)
			break;

	}
#ifdef TARGET_FPGA_SIMULATION
	log_d("SIM dump start.\n\r");
	write32(0x08010004, 0x91000000);	//dump Y data
	write32(0x08010008, 0x0000c000);
	write32(0x0801000c, 0x0);
	write32(0x08010000, 0x75);
	write32(0x08010004, 0x9100c000);	//dump Cb data
	write32(0x08010008, 0x00006000);
	write32(0x0801000c, 0x1);
	write32(0x08010000, 0x75);
#endif
}

static void cam_config(SNR_HDR_EN hdrmode)
{
	hdrsensor.again        = 1;
	hdrsensor.fps          = 30;
	hdrsensor.width        = 1920;
	hdrsensor.height       = 1080;
	hdrsensor.sysclk       = 80;
	hdrsensor.exposure     = 1000;
	hdrsensor.vts          = 0xFA0;
	hdrsensor.hts          = 0x4A0;
	hdrsensor.trigen       = 0;
	hdrsensor.hdren        = hdrmode;

	hdrsensor.pw.mclkp     = 22;
	hdrsensor.pw.strbp     = 36;
	hdrsensor.pw.trigp     = 37;
	hdrsensor.pw.shutdnp   = 35;
	hdrsensor.pw.rstp      = 33;

	hdrsensor.mipi_cfg.id      = isp_config.mipi;
	hdrsensor.mipi_cfg.mbps    = 270;
	hdrsensor.mipi_cfg.height  = hdrsensor.height;
	hdrsensor.mipi_cfg.width   = hdrsensor.width;
	hdrsensor.mipi_cfg.data_t  = MIPI_RAW_12BIT;
	hdrsensor.mipi_cfg.cfg_clk = 24000000;
	hdrsensor.mipi_cfg.lanes   = 2;
	switch (hdrsensor.hdren) {
	case SNR_HDR_3_FRAME:
		hdrsensor.mipi_cfg.vcno    = 3;
		break;
	case SNR_HDR_2_FRAME:
		hdrsensor.mipi_cfg.vcno    = 2;
		break;
	case SNR_HDR_DISABLE:
	default:
		hdrsensor.mipi_cfg.vcno    = 1;
		break;
	}
	hdrsensor.mipi_cfg.flags   = 0;

#if (VERISLC_DPHY_SIMULAT == 1)
	hdrsensor.mipi_cfg.addr[0] = 0x90000000;
	hdrsensor.mipi_cfg.addr[1] = 0x92000000;
	hdrsensor.mipi_cfg.addr[1] = 0x94000000;
#else
	hdrsensor.mipi_cfg.addr[0] = 0xA0000000;
	hdrsensor.mipi_cfg.addr[1] = 0xA2000000;
	hdrsensor.mipi_cfg.addr[2] = 0xA4000000;
#endif


}

static void isp_hdr(void)
{
	imi_printf("isp_t hdr test start.\n\r");

	isp_config_init();
	for(int i=0; i<(sizeof(config_hdr)/sizeof(config_hdr[0])); i++){
		write32((ISP_BASE_ADDR+config_hdr[i][0]), config_hdr[i][1]);
	}	
	write32((ISP_BASE_ADDR+0x00000400),0x80100697);

	cam_config(SNR_HDR_2_FRAME);
	hdrsensor.dev = imi_i2c_init(I2C_4);
	imi_printf("MIPI%x init start.\n\r",hdrsensor.mipi_cfg.id);
	mipi_init(&(hdrsensor.mipi_cfg));
#ifndef TARGET_FPGA_SIMULATION
	rgb_sensor_init(&hdrsensor);
	imi_printf("sensor_init\n");
	start_rgbsnr_stream(&hdrsensor);
#endif
#ifdef TARGET_FPGA_SIMULATION
	write32(0x08010060,1);
#endif
	imi_printf("HDR mode is 0x%x.\n\r",read32(ISP_BASE_ADDR+0x336c));
	while(1){
		imi_printf("ISP_STITCHING0_RIS is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x000033E8));
		imi_printf("ISP_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x05c0));
		imi_printf("MI_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16e0));
		imi_printf("MI_ISR1 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16cc));
		imi_printf("ISP_REG_FE_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+ 0x00003D7C));
		imi_printf("MI_RIS3 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x000056e0));
		if(read32(ISP_BASE_ADDR+0x16e0) == 0x0301)
			break;
	#ifndef TARGET_FPGA_SIMULATION
		udelay(100000);
	#endif
	}
	#ifdef TARGET_FPGA_SIMULATION
	log_d("SIM dump start.\n\r");
	write32(0x08010004, MI_MP_Y_BA);	//dump Y data
	write32(0x08010008, (MI_MP_Y_SE*2));
	write32(0x0801000c, 0x0);
	write32(0x08010000, 0x75);
	write32(0x08010004, HDR_RAW_L_BA);	//dump Cb data
	write32(0x08010008, 0x600000);
	write32(0x0801000c, 0x1);
	write32(0x08010000, 0x75);
	#endif
}

void isp_linear(void)
{
	imi_printf("isp_t linear test start.\n\r");
	write32(MI_IMSC,0x1); //mask YCbCr frame end
	write32((ISP_BASE_ADDR+0x05bc),0x2); //mask isp fame put out
	for(int i=0; i<(sizeof(config_lin)/sizeof(config_lin[0])); i++){
		write32((ISP_BASE_ADDR+config_lin[i][0]), config_lin[i][1]);
	}
	isp_config_init();
	write32((ISP_BASE_ADDR+0x00001310), 0x0000003a);
	write32((ISP_BASE_ADDR+0x00000400), 0x80100697);
	cam_config(SNR_HDR_DISABLE);

	hdrsensor.dev = imi_i2c_init(I2C_4);
	imi_printf("MIPI%x init start.\n\r",hdrsensor.mipi_cfg.id);
	mipi_init(&(hdrsensor.mipi_cfg));
#ifndef TARGET_FPGA_SIMULATION
	rgb_sensor_init(&hdrsensor);
	imi_printf(" ==== sensor_init over\n");
	start_rgbsnr_stream(&hdrsensor);
#endif
#ifdef TARGET_FPGA_SIMULATION
	//write32(0x08010060,1);  //start sensor1
	write32(0x08010064,1);  //start sensor2
#endif
	if(isp_config.irq == 1)
		request_irq(IRQ_ISP_INT0, ISP_IRQHandler, "ISP_MI_IRQ", NULL);
	if(isp_config.irq == 3)
		request_irq(IRQ_ISP_INT2, ISP_IRQHandler, "ISP_MI_IRQ", NULL);


	while(1){
		imi_printf("ISP_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x05c0));
		imi_printf("MI_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16e0));
		imi_printf("MI_ISR1 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16cc));
		imi_printf("ISP_REG_FE_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+ 0x00003D7C));
		imi_printf("MI_RIS3 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x000056e0));
		if(read32(ISP_BASE_ADDR+0x16e0) == 0x0301) {
			//write32((ISP_BASE_ADDR+0x400), 0x0);
			break;
		}
	#ifndef TARGET_FPGA_SIMULATION
		udelay(100000);
	#endif
	}
	#ifdef TARGET_FPGA_SIMULATION
	log_d("SIM dump start.\n\r");
	write32(0x08010004, MI_MP_Y_BA);
	write32(0x08010008, (MI_MP_Y_SE*2));
	write32(0x0801000c, 0x0);
	write32(0x08010000, 0x75);
	write32(0x08010004, (MI_MP_Y_BA+MI_MP_Y_SE));	//dump Cb data
	write32(0x08010008, MI_MP_Y_SE);
	write32(0x0801000c, 0x1);
	write32(0x08010000, 0x75);
	#endif
	while(1){
		if(read32(ISP_BASE_ADDR+0x16d0) == 0x0){
			break;
		}else{
			imi_printf("interrupt is close or fail.\n\r");
			break;
		}

	}

}

void isp_linear_IR(void)
{
	imi_printf("isp_t IR test start.\n\r");
	write32(0x23f00020,0x1); //change to DPU input
	
	for(int i=0; i<(sizeof(config_lin)/sizeof(config_lin[0])); i++){
		write32((ISP_BASE_ADDR+config_lin[i][0]), config_lin[i][1]);
	}
	hdrsensor.again 	   = 1;
	hdrsensor.fps		   = 30;
	hdrsensor.width 	   = 1280;
	hdrsensor.height	   = 960;
	hdrsensor.sysclk	   = 80;
	hdrsensor.exposure	   = 1000;
	hdrsensor.vts		   = 0xFA0;
	hdrsensor.hts		   = 0x4A0;
	hdrsensor.trigen	   = 0;
	
	hdrsensor.pw.mclkp	   = 22;
	hdrsensor.pw.strbp	   = 36;
	hdrsensor.pw.trigp	   = 37;
	hdrsensor.pw.shutdnp   = 35;
	hdrsensor.pw.rstp	   = 33;
	
	hdrsensor.mipi_cfg.id	   = 0;
	hdrsensor.mipi_cfg.mbps    = 270;
	hdrsensor.mipi_cfg.height  = hdrsensor.height;
	hdrsensor.mipi_cfg.width   = hdrsensor.width;
	hdrsensor.mipi_cfg.data_t  = MIPI_RAW_10BIT;
	hdrsensor.mipi_cfg.cfg_clk = 24000000;
	hdrsensor.mipi_cfg.lanes   = 2;
	hdrsensor.mipi_cfg.vcno    = 1;
	hdrsensor.mipi_cfg.flags   = 0;
	
#if (VERISLC_DPHY_SIMULAT == 1)
	hdrsensor.mipi_cfg.addr[0] = 0x90000000;
	hdrsensor.mipi_cfg.addr[1] = 0x92000000;
	hdrsensor.mipi_cfg.addr[1] = 0x94000000;
#else
	hdrsensor.mipi_cfg.addr[0] = 0xB0000000;
	hdrsensor.mipi_cfg.addr[1] = 0xB2000000;
	hdrsensor.mipi_cfg.addr[2] = 0xB4000000;
#endif
	mipi_init(&(hdrsensor.mipi_cfg));
#ifdef TARGET_FPGA_SIMULATION
	write32(0x08010060,1);
#endif
	
	while(1){
		imi_printf("ISP_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x05c0));
		imi_printf("MI_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16e0));
		imi_printf("MI_ISR1 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16cc));
		imi_printf("ISP_REG_FE_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+ 0x00003D7C));
		imi_printf("MI_RIS3 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x000056e0));
		if(read32(ISP_BASE_ADDR+0x16e0) == 0x0301)			
			break;
		
	}
	#ifdef TARGET_FPGA_SIMULATION
	log_d("SIM dump start.\n\r");
	write32(0x08010004, IR_MP_Y_BA);	//dump Y data
	write32(0x08010008, (IR_MP_Y_SE*2));
	write32(0x0801000c, 0x0);
	write32(0x08010000, 0x75);
	write32(0x08010004, (IR_MP_Y_BA+IR_MP_Y_SE));	//dump Cb data
	write32(0x08010008, IR_MP_Y_SE);
	write32(0x0801000c, 0x1);
	write32(0x08010000, 0x75);
	#endif
	while(1){
		if(read32(ISP_BASE_ADDR+0x16d0) == 0x0){	
			break;
		}else{
			imi_printf("interrupt is close or error.\n\r");
		}
		
	}
		
}


static void isp_mcm_p(void)
{
	imi_printf("isp_t mcm_p test start.\n\r");
	for(int i=0; i<(sizeof(config_mcm_p)/sizeof(config_mcm_p[0])); i++){
		write32((ISP_BASE_ADDR+config_mcm_p[i][0]), config_mcm_p[i][1]);
	}
	cam_config(SNR_HDR_DISABLE);
	hdrsensor.dev = imi_i2c_init(I2C_4);

	mipi_init(&(hdrsensor.mipi_cfg));

	rgb_sensor_init(&hdrsensor);
	imi_printf("sensor_init\n");
	start_rgbsnr_stream(&hdrsensor);
#ifdef TARGET_FPGA_SIMULATION
	write32(0x08010060,1);
#endif

	
	while(1){
		//imi_printf("MI MIS is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x000016d0));
		imi_printf("ISP_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x05c0));
		imi_printf("MI_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16e0));
		imi_printf("MI_ISR1 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x16cc));
		imi_printf("ISP_REG_FE_RIS value is 0x%x. \n\r",read32(ISP_BASE_ADDR+ 0x00003D7C));
		imi_printf("MI_RIS3 value is 0x%x. \n\r",read32(ISP_BASE_ADDR+0x000056e0));
		unsigned int value =read32(ISP_BASE_ADDR+0x16e0) & 0x40;
		if(value== 0x40)
			break;
	#ifndef TARGET_FPGA_SIMULATION
		udelay(100000);
	#endif
	}
	#ifdef TARGET_FPGA_SIMULATION
	log_d("SIM dump start.\n\r");
	write32(0x08010004, HDR_RAW_L_BA);	//dump raw data
	write32(0x08010008, 0x003f4800);
	write32(0x0801000c, 0x0);
	write32(0x08010000, 0x75);
	write32(0x08010004, HDR_RAW_S_BA);	//dump raw data
	write32(0x08010008, 0x003f4800);
	write32(0x0801000c, 0x1);
	write32(0x08010000, 0x75);
	#endif
}


void isp_test_main(void *select ,int n)
{

	log_d("isp_t main start.\n\r");
	//isp_t_IOCTL(MCM1);
//#ifdef TARGET_FPGA_SIMULATION
//	isp_t_IOCTL(RET);
//#else
	int *param = (int *)select;
	//imi_printf("%s\n\r",select->name);
	isp_t_IOCTL(param[0]);
//#endif
}

void isp_t_IOCTL(unsigned int flag)
{
	imi_printf("isp test case is 0x%x.\n\r",flag);
	switch(flag){
	case MCM1:
		isp_mcm();
		break;
	case HDR6:
		isp_config.hdr = 6;
		isp_hdr();
		break;
	case RET:
		isp_config.ret = 1;
		isp_hdr();
		break;
	case LI:
		isp_linear();
		break;
	case Bypass:
		isp_mcm_p();
		break;
	case IR:
		isp_linear_IR();
		break;
	case TPG:
		isp_config.tpg = 1;
		isp_linear();
		break;
	case IRQ0:
		isp_config.irq = 1;
		isp_linear();
		break;
	case IRQ1:
		isp_config.irq = 2;
		imi_printf("isp IRQ1 test case is empty.\n\r");
		break;
	case IRQ2:
		isp_config.irq = 3;
		isp_linear();
		break;
	case THDN:
		imi_printf("isp 3DNosie test case is empty.\n\r");
		break;
	case DSensor:
		imi_printf("isp DSensor test case is empty.\n\r");
		break;
	case Sensor1:
		isp_config.sensor = 1;
		isp_config.mipi   = 1;
		isp_linear();
		//isp_hdr();
		break;
	case HDR0:
		isp_config.hdr = 0;
		isp_hdr();
		break;
	case HDR1:
		isp_config.hdr = 1;
		isp_hdr();
		break;
	case HDR2:
		isp_config.hdr = 2;
		isp_hdr();
		break;
	case HDR3:
		isp_config.hdr = 3;
		isp_hdr();
		break;
	case HDR4:
		isp_config.hdr = 4;
		isp_hdr();
		break;
	case HDR5:
		isp_config.hdr = 5;
		isp_hdr();
		break;
	default:
		log_e("ISP test flag is error.\n\r");
		break;

	}

}

irqreturn_t ISP_IRQHandler(int irq, void *arg)
{
	switch (irq) {
		case IRQ_ISP_INT0:
			imi_printf("ISP_MIS: 0x%x\n\r",read32(ISP_BASE_ADDR+0x05c4));
			write32((ISP_BASE_ADDR+0x05c8),0x2); //clear YCbCr frame end
			imi_printf("IRQ_ISP_INT0 start. \n\r");
			break;
		case IRQ_ISP_INT1:
			imi_printf("IRQ_ISP_INT1 start. \n\r");
			break;
		case IRQ_ISP_INT2:
			imi_printf("MI_MIS: 0x%x\n\r",read32(MI_MIS));
			imi_printf("MI_MIS1: 0x%x\n\r",read32(MI_MIS1));
			write32(MI_ICR,0x1);
			//write32((ISP_BASE_ADDR+0x400), 0x0);
		#ifdef TARGET_FPGA_SIMULATION
			write32(0x08020000,0x50);
		#endif
			imi_printf("IRQ_ISP_INT2 start. \n\r");
			break;
		default:
			imi_printf("IRQ_num is error. \n\r");
			break;
	}
	ulPortYieldRequired = pdFALSE;
	ulPortInterruptNesting = 1;
	return IRQ_HANDLED;

}




DEV_INIT(isp_init);




