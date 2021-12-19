#ifndef __IMI_CONFIG__
#define __IMI_CONFIG__
#include "imi_vidomain.h"
#define  IMI_2280_RTOS_SYSTEM		(1)
//#define IN_SIMULATOR				(1)

#define PARA_USING_PRESET_TABLE			(1)
//#define PARA_USING_FS_FILE				(1)

//a larg msg queue to save more info
#define IMI_IPC_MSG_NUM				(64)

//#define PRJ_A300			1
#define PRJ_S300			1
#if defined(PRJ_A300)
#define GPIO_IRCIS_XCLK			22//34
#define GPIO_IRCIS_STROBE		36
#define GPIO_IRCIS_TRIGGER		37
#define GPIO_IRCIS_SHUTDOWN		35//33
#define GPIO_IRCIS_RESET		33//32

#define IMI_BM_IR_RES_WIDTH_MAX				(1280)
#define IMI_BM_IR_RES_HEIGHT_MAX			(960)
#define IMI_BM_RGB_RES_WIDTH_MAX			(1920)
#define IMI_BM_RGB_RES_HEIGHT_MAX			(1080)
#define IMI_BM_DEPTH_RES_WIDTH_MAX			(640)
#define IMI_BM_DEPTH_RES_HEIGHT_MAX			(480)



#define just_delay(a) do{for(int i=0; i<a; i++) { for(int j=0; j<1; j++);}}while(0)

//HAL_PRESET_INFO
#define SYS_DEF_IR_WIDTH				(1280)
#define SYS_DEF_IR_HEIGHT				(960)
#define SYS_DEF_DEPTH_ROTATE			(1)

#define SYS_DEF_DEPTH_WIDTH				(640)
#define SYS_DEF_DEPTH_HEIGHT			(480)

#define DPU_BUF_NUM						(4)

#define SYS_DEF_BL_VERTICAL				(1)

/*DTS Context*/
#define SS_EXT_TRIG_GPIO				(8)

/*Color Sensor*/
#define RGB_SNR_I2C_BUS					(1)
#define RGB_SNR_INTERFACE				(SRC_SEL_MIPI2)
#define RGB_SNR_DATA_BITS				(12)
#define RGB_SNR_MIPI_LANE				(2)
#define RGB_SNR_IO_RST					(34)
#define RGB_SNR_IO_AVDD					(18)
#define RGB_SNR_IO_DVDD					(8)
#define RGB_SNR_IO_DOVDD				(16)
#define RGB_SNR_EXT_SYNC				(0)
#define RGB_SNR_EXT_TRIG_GPIO			(0)

/*IR Sensor*/
#define IR_SNR_I2C_BUS					(1)
#define IR_SNR_INTERFACE				(SRC_SEL_MIPI2)
#define IR_SNR_DATA_BITS				(MIPI_10BIT)
#define IR_SNR_MIPI_LANE				(2)
//#define IR_SNR_MCLK						//mclk muxpin()
#define IR_SNR_IO_RST					(34)
#define IR_SNR_IO_AVDD					(18)
#define IR_SNR_IO_DVDD					(8)
#define IR_SNR_IO_DOVDD					(16)

/*DPU*/
#define DPU_BUS_FREQ					(200000000)
#define IMI_2280_DEP_IRQ_NO				(118)
#define IMI_2280_IR_IRQ_NO				(120)
#define IMI_2280_SS0_IRQ_NO				(121)

#define IMI_2280_SENSOR_TRIG			(0)
#define IMI_2280_FLOOD_TRIG				(0)
#define IMI_2280_DOT_TRIG				(0)
#define IMI_2280_EXT_TRIG				(0)
#elif defined(PRJ_S300)
#define GPIO_IRCIS_XCLK			22//34
#define GPIO_IRCIS_STROBE		36
#define GPIO_IRCIS_TRIGGER		37
#define GPIO_IRCIS_SHUTDOWN		35//33
#define GPIO_IRCIS_RESET		33//32

#define IMI_BM_IR_RES_WIDTH_MAX				(1280)
#define IMI_BM_IR_RES_HEIGHT_MAX			(960)
#define IMI_BM_RGB_RES_WIDTH_MAX			(1920)
#define IMI_BM_RGB_RES_HEIGHT_MAX			(1080)
#define IMI_BM_DEPTH_RES_WIDTH_MAX			(640)
#define IMI_BM_DEPTH_RES_HEIGHT_MAX			(480)



#define just_delay(a) do{for(int i=0; i<a; i++) { for(int j=0; j<1; j++);}}while(0)

//HAL_PRESET_INFO
#define SYS_DEF_IR_WIDTH				(1280)
#define SYS_DEF_IR_HEIGHT				(960)
#define SYS_DEF_DEPTH_ROTATE			(1)

#define SYS_DEF_DEPTH_WIDTH				(640)
#define SYS_DEF_DEPTH_HEIGHT			(480)

#define DPU_BUF_NUM						(4)

#define SYS_DEF_BL_VERTICAL				(0)

/*DTS Context*/
#define SS_EXT_TRIG_GPIO				(20)

/*Color Sensor*/
#define RGB_SNR_I2C_BUS					(1)
#define RGB_SNR_INTERFACE				(SRC_SEL_MIPI2)
#define RGB_SNR_DATA_BITS				(12)
#define RGB_SNR_MIPI_LANE				(2)
#define RGB_SNR_IO_RST					(34)
#define RGB_SNR_IO_AVDD					(18)
#define RGB_SNR_IO_DVDD					(8)
#define RGB_SNR_IO_DOVDD				(16)
#define RGB_SNR_EXT_SYNC				(0)
#define RGB_SNR_EXT_TRIG_GPIO			(0)

/*IR Sensor*/
#define IR_SNR_I2C_BUS					(1)
#define IR_SNR_INTERFACE				(SRC_SEL_MIPI2)
#define IR_SNR_DATA_BITS				(MIPI_10BIT)
#define IR_SNR_MIPI_LANE				(2)
//#define IR_SNR_MCLK						//mclk muxpin()
#define IR_SNR_IO_RST					(34)
#define IR_SNR_IO_AVDD					(18)
#define IR_SNR_IO_DVDD					(8)
#define IR_SNR_IO_DOVDD					(16)

/*DPU*/
#define DPU_BUS_FREQ					(200000000)
#define IMI_2280_DEP_IRQ_NO				(118)
#define IMI_2280_IR_IRQ_NO				(120)
#define IMI_2280_SS0_IRQ_NO				(121)

#define IMI_2280_SENSOR_TRIG			(0)
#define IMI_2280_FLOOD_TRIG				(0)
#define IMI_2280_DOT_TRIG				(0)
#define IMI_2280_EXT_TRIG				(0)
#endif
#endif/*__IMI_CONFIG__*/
