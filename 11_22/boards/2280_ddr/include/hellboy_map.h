#ifndef __HYDRA_MAP_H__
#define __HYDRA_MAP_H__

//====================================================================================================================================================================
#define ROM_BASE_ADDR				0x00000000  // default: 0x00000000 ~ 0x00007FFF (32KB); remap: 0x07000000 ~ 0x071FFFFF (2MB)  (Allocated 2MB address )
#define RAM_BASE_ADDR				0x07000000  // default: 0x07000000 ~ 0x070FFFFF (1MB);  remap: 0x00000000 ~ 0x003FFFFF (32KB) (Allocated 4MB address)
#define DDRC_PORT0_BASE_ADDR		0x40000000  // 0x40000000 ~ 0x7FFFFFFF (1024MB ) DDRC Port0
#define DDRC_PORT1_BASE_ADDR		0x80000000  // 0x80000000 ~ 0xFFFFFFFF (2048MB ) DDRC Port1
#define DDRC_PORT2_BASE_ADDR		0x00000000  // 0x00000000 ~ 0x7FFFFFFF (2048MB ) DDRC Port2
#define DDRC_PORT3_BASE_ADDR		0x00000000  // 0x00000000 ~ 0x7FFFFFFF (2048MB ) DDRC Port3

//====================================================================================================================================================================
#define NJ_TOP_BASE_ADDR			0x10000000  // 0x10000000 ~ 0x1FFFFFFF (256MKB)
#define DPU_REF_BASE_ADDR			0x10300000	// 0x10300000 ~ 		
#define DPU_DY_RAM0					0x10A10000	// 0x10A10000 ~ 0x10A1FFFF (64KB)
#define DPU_DY_RAM1					0x10A20000	// 0x10A20000 ~ 0x10A2FFFF (64KB)
#define DPU_BASE_ADDR				0x11000000	// 0x11000000 ~ 0x11100000 (1MB)
#define USB_BASE_ADDR				0x20000000  // 0x20000000 ~ 0x2000FFFF (64KB)
#define VIC0_BASE_ADDR				0x20010000  // 0x20010000 ~ 0x2001FFFF (64KB)
#define VIC1_BASE_ADDR				0x20020000  // 0x20020000 ~ 0x2002FFFF (64KB)
#define VIC2_BASE_ADDR    		    0x20030000  // 0x20030000 ~ 0x2003FFFF (64KB)
#define VIC3_BASE_ADDR				0x20400000  // 0x20400000 ~ 0x207FFFFF (4MB)
#define FAST_BUS_CFG				0x20B00000  // 0x20B00000 ~ 0x20BFFFFF (1MB)
#define MST_BUS_CFG					0x20C00000  // 0x20C00000 ~ 0x20CFFFFF (1MB)
#define DDR_PHY_BASE_ADDR			0x22000000  // 0x22000000 ~ 0x23FFFFFF (32MB)
 
//====================================================================================================================================================================
// AHB2APB Bridge 0 : 0x30000000 ~ 0x30FFFFFF (16MB)
#define SYS_CON_BASE_ADDR 			0x30000000  // 0x30000000 ~ 0x300FFFFF (1MB)  Peripheral 0
#define CLK_GEN_BASE_ADDR      		0x30100000  // 0x30100000 ~ 0x301FFFFF (1MB)  Peripheral 1
#define RST_GEN_BASE_ADDR      		0x30200000  // 0x30200000 ~ 0x302FFFFF (1MB)  Peripheral 2
#define ADC_CTL_BASE_ADDR      		0x30300000  // 0x30300000 ~ 0x303FFFFF (1MB)  Peripheral 3
#define TIMER0_BASE_ADDR       		0x30400000  // 0x30400000 ~ 0x304FFFFF (1MB)  Peripheral 4
#define TIMER1_BASE_ADDR       		0x30500000  // 0x30500000 ~ 0x305FFFFF (1MB)  Peripheral 5
#define GPIO0_BASE_ADDR        		0x30600000  // 0x30600000 ~ 0x306FFFFF (1MB)  Peripheral 6
#define GPIO1_BASE_ADDR        		0x30700000  // 0x30700000 ~ 0x307FFFFF (1MB)  Peripheral 7
#define GPIO2_BASE_ADDR        		0x30800000  // 0x30800000 ~ 0x308FFFFF (1MB)  Peripheral 8
#define PMU_BASE_ADDR          		0x30900000  // 0x30900000 ~ 0x309FFFFF (1MB)  Peripheral 9
#define PWM0_BASE_ADDR         		0x30A00000  // 0x30A00000 ~ 0x30AFFFFF (1MB)  Peripheral 10
#define PWM1_BASE_ADDR         		0x30B00000  // 0x30B00000 ~ 0x30BFFFFF (1MB)  Peripheral 11
#define MBOX0_BASE_ADDR        		0x30C00000  // 0x30C00000 ~ 0x30CFFFFF (1MB)  Peripheral 12
#define MBOX1_BASE_ADDR        		0x30D00000  // 0x30D00000 ~ 0x30DFFFFF (1MB)  Peripheral 13
#define SYS_CON_SV_BASE_ADDR   		0x30E00000  // 0x30E00000 ~ 0x30EFFFFF (1MB)  Peripheral 14
#define VPU_BASE_ADDR          		0x30F00000  // 0x30F00000 ~ 0x30FFFFFF (1MB)  Peripheral 15
#define QSPI0_APB_BASE_ADDR         0x35600000
#define QSPI0_AHB_BASE_ADDR         0x38000000


//====================================================================================================================================================================
// AHB2APB Bridge 1 : 0x31000000 ~ 0x31FFFFFF (16MB)
#define DSP0_IRQC_BASE_ADDR         0x31000000  // 0x31000000 ~ 0x310FFFFF (1MB)  Peripheral 0
#define UART0_BASE_ADDR             0x31100000  // 0x31100000 ~ 0x311FFFFF (1MB)  Peripheral 1
#define UART1_BASE_ADDR             0x31200000  // 0x31200000 ~ 0x312FFFFF (1MB)  Peripheral 2
#define SPI0_BASE_ADDR              0x31300000  // 0x31300000 ~ 0x313FFFFF (1MB)  Peripheral 3
#define SPI1_BASE_ADDR              0x31400000  // 0x31400000 ~ 0x314FFFFF (1MB)  Peripheral 4
#define I2C0_BASE_ADDR              0x31500000  // 0x31500000 ~ 0x315FFFFF (1MB)  Peripheral 5
#define I2C1_BASE_ADDR              0x31600000  // 0x31600000 ~ 0x316FFFFF (1MB)  Peripheral 6
#define I2C2_BASE_ADDR              0x31700000  // 0x31700000 ~ 0x317FFFFF (1MB)  Peripheral 7
#define DW_I2S_BASE_ADDR            0x31800000  // 0x31800000 ~ 0x318FFFFF (1MB)  Peripheral 8
#define MBOX2_BASE_ADDR             0x31900000  // 0x31900000 ~ 0x319FFFFF (1MB)  Peripheral 9
#define TIMER64_0_BASE_ADDR         0x31A00000  // 0x31A00000 ~ 0x31AFFFFF (1MB)  Peripheral 10
#define TIMER64_1_BASE_ADDR			0x31B00000  // 0x31B00000 ~ 0x31BFFFFF (1MB)  Peripheral 11
//RESERVED                        	0x31C00000  // 0x31C00000 ~ 0x31CFFFFF (1MB)  Peripheral 12
#define DSP1_IRQC_BASE_ADDR			0x31D00000  // 0x31D00000 ~ 0x31DFFFFF (1MB)  Peripheral 13
#define VPU_FPE_BUS_CFG_BASE_ADDR	0x31E00000  // 0x30E00000 ~ 0x30EFFFFF (1MB)  Peripheral 14
#define AXI_DMAC_BASE_ADDR			0x31F00000  // 0x31F00000 ~ 0x31FFFFFF (1MB)  Peripheral 15

//====================================================================================================================================================================
// AHB2APB Bridge 2 : 0x32000000 ~ 0x32FFFFFF (16MB)
 #define SYS_CON3_BASE_ADDR			0x32000000  // 0x32000000 ~ 0x320FFFFF (1MB)  Peripheral 0
//RESERVED                     		0x32100000  // 0x32100000 ~ 0x321FFFFF (1MB)  Peripheral 1
#define USB3_APB_BASE_ADDR			0x32200000  // 0x32200000 ~ 0x322FFFFF (1MB)  Peripheral 2
#define PERFMON_APB_BASE_ADDR		0x32300000  // 0x32300000 ~ 0x323FFFFF (1MB)  Peripheral 3
#define EFUSE_APB_BASE_ADDR			0x32400000  // 0x32400000 ~ 0x324FFFFF (1MB)  Peripheral 4
//RESERVED                    		0x32500000  // 0x32500000 ~ 0x325FFFFF (1MB)  Peripheral 5
//RESERVED                     		0x32600000  // 0x32600000 ~ 0x326FFFFF (1MB)  Peripheral 6
//RESERVED                    		0x32700000  // 0x32700000 ~ 0x327FFFFF (1MB)  Peripheral 7
#define MIPI_DPHY0_BASE_ADDR   		0x32800000  // 0x32800000 ~ 0x328FFFFF (1MB)  Peripheral 8
#define MIPI_DPHY1_BASE_ADDR   		0x32900000  // 0x32900000 ~ 0x329FFFFF (1MB)  Peripheral 9
#define VI_BASE_ADDR           		0x32A00000  // 0x32A00000 ~ 0x32AFFFFF (1MB)  Peripheral 10
#define CMIF_BASE_ADDR         		0x32B00000  // 0x32B00000 ~ 0x32BFFFFF (1MB)  Peripheral 11
#define MIPI_DPHY2_BASE_ADDR   		0x32C00000  // 0x32C00000 ~ 0x32CFFFFF (1MB)  Peripheral 12
#define SYS_CON2_BASE_ADDR     		0x32D00000  // 0x32D00000 ~ 0x32DFFFFF (1MB)  Peripheral 13
//RESERVED                     		0x32E00000  // 0x32E00000 ~ 0x32EFFFFF (1MB)  Peripheral 14
#define DDR_CTL_BASE_ADDR      		0x32F00000  // 0x32F00000 ~ 0x32FFFFFF (1MB)  Peripheral 15

//====================================================================================================================================================================
#define ISP_BASE_ADDR          		0x33F00000  // 0x33F00000 ~ 0x33FFFFFF (1MB)  ISP
#define NN_BASE_ADDR           		0x34000000  // 0x34000000 ~ 0x340FFFFF (1MB)  VIP8000
#define DSP0_OCEM_BASE_ADDR    		0x34100000  // 0x34100000 ~ 0x341FFFFF (1MB)  DSP0_OCEM
#define DSP1_OCEM_BASE_ADDR    		0x34200000  // 0x34200000 ~ 0x342FFFFF (1MB)  DSP1_OCEM

//====================================================================================================================================================================
// AHB2APB Bridge 3 : 0x35000000 ~ 0x35FFFFFF (16MB)
#define SKT_BLOB_BASE_ADDR     		0x35000000  // 0x35000000 ~ 0x350FFFFF (1MB)  Peripheral 0
#define SKT_BPC_BASE_ADDR      		0x35100000  // 0x35100000 ~ 0x351FFFFF (1MB)  Peripheral 1
#define SKT_CENTD_BASE_ADDR    		0x35200000  // 0x35200000 ~ 0x352FFFFF (1MB)  Peripheral 2
#define SKT_FLOOR_BASE_ADDR    		0x35300000  // 0x35300000 ~ 0x353FFFFF (1MB)  Peripheral 3
#define SKT_FDIFF_BASE_ADDR    		0x35400000  // 0x35400000 ~ 0x354FFFFF (1MB)  Peripheral 4
#define FPE_BASE_ADDR          		0x35500000  // 0x35500000 ~ 0x355FFFFF (1MB)  Peripheral 5
#define QSPI_BASE_ADDR         		0x35600000  // 0x35600000 ~ 0x356FFFFF (1MB)  Peripheral 6
//RESERVED                     		0x35700000  // 0x35700000 ~ 0x357FFFFF (1MB)  Peripheral 7
//RESERVED                     		0x35800000  // 0x35800000 ~ 0x358FFFFF (1MB)  Peripheral 8
//RESERVED                     		0x35900000  // 0x35900000 ~ 0x359FFFFF (1MB)  Peripheral 9
//RESERVED                     		0x35A00000  // 0x35A00000 ~ 0x35AFFFFF (1MB)  Peripheral 10
//RESERVED                     		0x35B00000  // 0x35B00000 ~ 0x35BFFFFF (1MB)  Peripheral 11
//RESERVED                     		0x35C00000  // 0x35C00000 ~ 0x35CFFFFF (1MB)  Peripheral 12
//RESERVED                     		0x35D00000  // 0x35D00000 ~ 0x35DFFFFF (1MB)  Peripheral 13
//RESERVED                     		0x35E00000  // 0x35E00000 ~ 0x35EFFFFF (1MB)  Peripheral 14
#define SKT_BASE_ADDR          		0x35F00000  // 0x35F00000 ~ 0x35FFFFFF (1MB)  Peripheral 15

//====================================================================================================================================================================
#define SDIO_BASE_ADDR         		0x36000000  // 0x36000000 ~ 0x37FFFFFF (32MB   ) SDIO
#define QSPI_MEM_ADDR          		0x38000000  // 0x38000000 ~ 0x3FFFFFFF (128MB  ) QSPI
#define DSP0_EDAP_BASE_ADDR    		0x01000000  // 0x01000000 ~ 0x01FFFFFF (16MB   ) DSP0_EDAP
#define DSP0_SLV0_BASE_ADDR    		0x02000000  // 0x02000000 ~ 0x02FFFFFF (16MB   ) DSP0_SLV0
#define DSP1_EDAP_BASE_ADDR    		0x03000000  // 0x03000000 ~ 0x03FFFFFF (16MB   ) DSP0_EDAP
#define DSP1_SLV0_BASE_ADDR    		0x04000000  // 0x04000000 ~ 0x04FFFFFF (16MB   ) DSP0_SLV0
#define USB3_BASE_ADDR         		0x05000000  // 0x05000000 ~ 0x06FFFFFF (32MB   )
#define SKT_MEM_ADDR           		0x08000000  // 0x08000000 ~ 0x0FFFFFFF (128MB  ) SKT_MEM

//====================================================================================================================================================================
// USB Register
#define USB_REG_FADDR        		( USB_BASE_ADDR + 0x0   )
#define USB_REG_POWER        		( USB_BASE_ADDR + 0x1   )
#define USB_REG_INTRIN       		( USB_BASE_ADDR + 0x2   )
#define USB_REG_INTRINE      		( USB_BASE_ADDR + 0x6   )
#define USB_REG_INTR         		( USB_BASE_ADDR + 0xa   )
#define USB_REG_INDEX       	 	( USB_BASE_ADDR + 0xe   )
#define USB_REG_TESTMODE     		( USB_BASE_ADDR + 0xf   )
#define USB_REG_EP_INMAP     		( USB_BASE_ADDR + 0x10  )
#define USB_REG_EP_INCSR     		( USB_BASE_ADDR + 0x12  )
#define USB_REG_EP_TXFIFOA   		( USB_BASE_ADDR + 0x1c  )
#define USB_REG_EP_TXFIFOS   		( USB_BASE_ADDR + 0x1a  )
#define USB_REG_EP_FIFO_DATA 		( USB_BASE_ADDR + 0x20  )
#define USB_REG_DELAY        		( USB_BASE_ADDR + 0x100 )

//====================================================================================================================================================================

#endif//__HYDRA_MAP_H__

