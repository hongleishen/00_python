#ifndef __HYDRA_MAP_H__
#define __HYDRA_MAP_H__

//====================================================================================================================================================================
#define ROM_BASE_ADDR				 0x00000000 
#define RAM_BASE_ADDR				 0x08000000
#define DDR_CONTROLLER_BASE_ADDR	 0x10000000  
#define DDR_PHY_BASE_ADDR		     0x14000000  
#define DDR_SYS_MANAG_BASE_ADDR		 0x18000000 
#define DDR_PM_BASE_ADDR		     0x1C000000 

//====================================================================================================================================================================
#define WATCHDOG_BASE_ADDR			 0x20000000	
#define GPIO_BASE_ADDR				 0x20100000	
#define COMMON_TIMER_BASE_ADDR		 0x20200000	
#define SPI_BASE_ADDR                0x20300000
#define I2C0_BASE_ADDR               0x20400000
#define I2C1_BASE_ADDR				 0x20401000  
#define I2C2_BASE_ADDR				 0x20402000  
#define I2C3_BASE_ADDR				 0x20403000  
#define I2C4_BASE_ADDR				 0x20404000 
#define UART0_BASE_ADDR              0x20500000
#define UART1_BASE_ADDR				 0x20501000	
#define MBOX0_BASE_ADDR              0x20600000
#define EFUSE_BASE_ADDR              0x20700000
#define ADC_BASE_ADDR                0x20800000
#define PWM_BASE_ADDR                0x20900000
#define CLK_GEN_BASE_ADDR            0x20f00000
#define USB_BASE_ADDR				 0x22000000	
#define CRYPTO_BASE_ADDR             0x22100000
#define IPP_BASE_ADDR                0x22200000
#define AXI_DMAC_BASE_ADDR           0x22500000
#define QSPI1_APB_BASE_ADDR          0x22540000
#define QSPI1_AHB_BASE_ADDR          0x22560000
#define QSPI0_APB_BASE_ADDR          0x22580000
#define QSPI0_AHB_BASE_ADDR          0x225C0000
#define MIPI_TX0_BASE_ADDR           0x22600000
#define MIPI_TX1_BASE_ADDR           0x22700000
#define BUS1_SYS_MANAG_BASE_ADDR     0x22f00000
#define ISP_BASE_ADDR                0x23000000
#define CVA_BASE_ADDR                0x23100000
#define BUS2_SYS_MANAG_BASE_ADDR     0x23f00000
#define VPU_BASE_ADDR				 0x24000000
#define TWO_D_BASE_ADDR              0x24100000
#define MIPI_RX0_BASE_ADDR           0x24200000
#define MIPI_RX1_BASE_ADDR           0x24300000
#define BUS3_SYS_MANAG_BASE_ADDR     0x24f00000
#define DPU_BASE_ADDR				 0x25000000
#define PNG_BASE_ADDR                0x25100000
#define RG_BASE_ADDR                 0x25200000
#define BUS4_SYS_MANAG_BASE_ADDR     0x25f00000

#define VIC0_BASE_ADDR				 0x2D000000	 
#define VIC1_BASE_ADDR				 0x2D100000	 
#define VIC2_BASE_ADDR				 0x2D200000	 
#define VIC3_BASE_ADDR				 0x2D300000	 

#define VIC4_BASE_ADDR				 0x2D400000	 
#define VIC5_BASE_ADDR				 0x2D500000	 
#define VIC6_BASE_ADDR				 0x2D600000	 
#define VIC7_BASE_ADDR				 0x2D700000

#define BOOT_MODE_REG_ADDR      0x20F08028

//====================================================================================================================================================================
// USB Register
#define USB_REG_FADDR				( USB_BASE_ADDR + 0x0   )
#define USB_REG_POWER				( USB_BASE_ADDR + 0x1   )
#define USB_REG_INTRIN				( USB_BASE_ADDR + 0x2   )
#define USB_REG_INTRINE      		( USB_BASE_ADDR + 0x6   )
#define USB_REG_INTR				( USB_BASE_ADDR + 0xa   )
#define USB_REG_INDEX        		( USB_BASE_ADDR + 0xe   )
#define USB_REG_TESTMODE     		( USB_BASE_ADDR + 0xf   )
#define USB_REG_EP_INMAP     		( USB_BASE_ADDR + 0x10  )
#define USB_REG_EP_INCSR     		( USB_BASE_ADDR + 0x12  )
#define USB_REG_EP_TXFIFOA   		( USB_BASE_ADDR + 0x1c  )
#define USB_REG_EP_TXFIFOS   		( USB_BASE_ADDR + 0x1a  )
#define USB_REG_EP_FIFO_DATA 		( USB_BASE_ADDR + 0x20  )
#define USB_REG_DELAY				( USB_BASE_ADDR + 0x100 )

//====================================================================================================================================================================

#endif//__HYDRA_MAP_H__

