#ifndef __CANDENCE_QSPI_H__
#define __CANDENCE_QSPI_H__

#include "hellboy_map.h"
#include "imi_type.h"

#define QSPI_FLASH_CLK_FREQ 		 10		//for 2280 clk can up to 60M, delay is 3
#define QSPI_REF_CLK_FREQ            120


// ----------------------------------------------------------------------------
// ------------------------- QSPI Config Register List ------------------------
// ----------------------------------------------------------------------------
#define QSPI_CFG        (QSPI0_APB_BASE_ADDR+0x00)   // QSPI Configuration Register
#define QSPI_RDINS      (QSPI0_APB_BASE_ADDR+0x04)   // QSPI Device Read Instruction Register
#define QSPI_WRINS      (QSPI0_APB_BASE_ADDR+0x08)   // QSPI Device Write Instruction Register
#define QSPI_DELAY      (QSPI0_APB_BASE_ADDR+0x0C)   // QSPI Device Delay Register
#define QSPI_RDDATCAP   (QSPI0_APB_BASE_ADDR+0x10)   // QSPI Read Data Capture Register
#define QSPI_DSIZE      (QSPI0_APB_BASE_ADDR+0x14)   // QSPI Device Size Register
#define QSPI_SRAMPA     (QSPI0_APB_BASE_ADDR+0x18)   // QSPI SRAM Partition Register
#define QSPI_INDTRIG    (QSPI0_APB_BASE_ADDR+0x1C)   // QSPI Indirect AHB Address Trigger Register
#define QSPI_DMAPERI    (QSPI0_APB_BASE_ADDR+0x20)   // QSPI DMA Peripheral Register
#define QSPI_REMAP      (QSPI0_APB_BASE_ADDR+0x24)   // QSPI Remap Address Register
#define QSPI_MODEBIT    (QSPI0_APB_BASE_ADDR+0x28)   // QSPI Mode Bit Register
#define QSPI_SRAMFLVL   (QSPI0_APB_BASE_ADDR+0x2C)   // QSPI SRAM Fill Level Register
#define QSPI_TXTHRE     (QSPI0_APB_BASE_ADDR+0x30)   // QSPI TX Threshold Register
#define QSPI_RXTHRE     (QSPI0_APB_BASE_ADDR+0x34)   // QSPI RX Threshold Register
#define QSPI_WRCCTRL    (QSPI0_APB_BASE_ADDR+0x38)   // QSPI Write Completion Control Register
#define QSPI_POLLEXP    (QSPI0_APB_BASE_ADDR+0x3C)   // QSPI Polling Expiration Register
#define QSPI_IS         (QSPI0_APB_BASE_ADDR+0x40)   // QSPI Interrupt Status Register
#define QSPI_IM         (QSPI0_APB_BASE_ADDR+0x44)   // QSPI Interrupt Mask
#define QSPI_LOWERWP    (QSPI0_APB_BASE_ADDR+0x50)   // QSPI Lower Write Protection Register
#define QSPI_UPPERWP    (QSPI0_APB_BASE_ADDR+0x54)   // QSPI Upper Write Protection Register
#define QSPI_WP         (QSPI0_APB_BASE_ADDR+0x58)   // QSPI Write Protection Register
#define QSPI_INDRD      (QSPI0_APB_BASE_ADDR+0x60)   // QSPI Indirect Read Transfer Register
#define QSPI_INDRDWM    (QSPI0_APB_BASE_ADDR+0x64)   // QSPI Indirect Read Transfer Watermark Register
#define QSPI_INDRDSA    (QSPI0_APB_BASE_ADDR+0x68)   // QSPI Indirect Read Transfer Start Address Register
#define QSPI_INDRDNB    (QSPI0_APB_BASE_ADDR+0x6C)   // QSPI Indirect Read Transfer Number Bytes Register
#define QSPI_INDWR      (QSPI0_APB_BASE_ADDR+0x70)   // QSPI Indirect Write Transfer Register
#define QSPI_INDWRWM    (QSPI0_APB_BASE_ADDR+0x74)   // QSPI Indirect Write Transfer Watermark Register
#define QSPI_INDWRSA    (QSPI0_APB_BASE_ADDR+0x78)   // QSPI Indirect Write Transfer Start Address Register
#define QSPI_INDWRCT    (QSPI0_APB_BASE_ADDR+0x7C)   // QSPI Indirect Write Transfer Count Register
#define QSPI_INDTRIGAD  (QSPI0_APB_BASE_ADDR+0x80)   // QSPI Indirect AHB Trigger Address Range Register
#define QSPI_FCMD       (QSPI0_APB_BASE_ADDR+0x90)   // QSPI Flash Command Register
#define QSPI_FCMDADDR   (QSPI0_APB_BASE_ADDR+0x94)   // QSPI Flash Command Address Registers
#define QSPI_FCMDRDL    (QSPI0_APB_BASE_ADDR+0xA0)   // QSPI Flash Command Read Data Register (Lower)
#define QSPI_FCMDRDU    (QSPI0_APB_BASE_ADDR+0xA4)   // QSPI Flash Command Read Data Register (Upper)
#define QSPI_FCMDWDL    (QSPI0_APB_BASE_ADDR+0xA8)   // QSPI Flash Command Write Data Register (Lower)
#define QSPI_FCMDWDU    (QSPI0_APB_BASE_ADDR+0xAC)   // QSPI Flash Command Write Data Register (Upper)
#define QSPI_POLLFS     (QSPI0_APB_BASE_ADDR+0xB0)   // QSPI Polling Flash Status Register
#define QSPI_MODID      (QSPI0_APB_BASE_ADDR+0xFC)   // QSPI Module ID Register
// ----------------------------------------------------------------------------
// CTRLR0 - Control Register 0
// ----------------------------------------------------------------------------
#define QSPI_REG_CONFIG_DMA_MASK		BIT(15)
#define QSPI_REG_INDIRECTRD_START_MASK		BIT(0)

#define QSPI_FRF_STD        (0x0<<22)   //       Standard SPI Format
#define QSPI_FRF_DUAL       (0x1<<22)   //       Dual SPI Format
#define QSPI_FRF_QUAD       (0x2<<22)   //       Quad SPI Format
#define QSPI_FRF_OCTAL      (0x3<<22)   //       Octal SPI Format
										// 19:16 Microwire Control Frame Size
#define QSPI_CFS_01B        (0x0<<16)   //       01-bit Control Word
#define QSPI_CFS_02B        (0x1<<16)   //       02-bit Control Word
#define QSPI_CFS_03B        (0x2<<16)   //       03-bit Control Word
#define QSPI_CFS_04B        (0x3<<16)   //       04-bit Control Word
#define QSPI_CFS_05B        (0x4<<16)   //       05-bit Control Word
#define QSPI_CFS_06B        (0x5<<16)   //       06-bit Control Word
#define QSPI_CFS_07B        (0x6<<16)   //       07-bit Control Word
#define QSPI_CFS_08B        (0x7<<16)   //       08-bit Control Word
#define QSPI_CFS_09B        (0x8<<16)   //       09-bit Control Word
#define QSPI_CFS_10B        (0x9<<16)   //       10-bit Control Word
#define QSPI_CFS_11B        (0xa<<16)   //       11-bit Control Word
#define QSPI_CFS_12B        (0xb<<16)   //       12-bit Control Word
#define QSPI_CFS_13B        (0xc<<16)   //       13-bit Control Word
#define QSPI_CFS_14B        (0xd<<16)   //       14-bit Control Word
#define QSPI_CFS_15B        (0xe<<16)   //       15-bit Control Word
#define QSPI_CFS_16B        (0xf<<16)   //       16-bit Control Word
#define QSPI_SSTE           (0x1<<14)   // 14    Slave Select Toggle Enable
#define QSPI_SRL            (0x1<<13)   // 13    Shift Register Loop
#define QSPI_SLV_OE         (0x1<<12)   // 12    Slave Output Enable
										// 11:10 Transfer Mode
#define QSPI_TMOD_TXRX      (0x0<<10)   //       (TX_AND_RX)  : Transmit & Receive; Not Applicable in enhanced SPI operating mode
#define QSPI_TMOD_TX        (0x1<<10)   //       (TX_ONLY)    : Transmit only mode; Or Write in enhanced SPI operating mode
#define QSPI_TMOD_RX        (0x2<<10)   //       (RX_ONLY)    : Receive only mode; Or Read in enhanced SPI operating mode
#define QSPI_TMOD_EEP       (0x3<<10)   //       (EEPROM_READ): EEPROM Read mode; Not Applicable in enhanced SPI operating mode
#define QSPI_SCPOL          (0x1<<9)    // 9     Serial Clock Polarity
#define QSPI_SCPH           (0x1<<8)    // 8     Serial Clock Phase
										// 7:6   Frame Format
#define QSPI_FRF_SPI        (0x0<<6)    //       (SPI): Motorola SPI Frame Format
#define QSPI_FRF_SSP        (0x1<<6)    //       (SSP): Texas Instruments SSP Frame Format
#define QSPI_FRF_MW         (0x2<<6)    //       (MICROWIRE): National Semiconductors Microwire Frame Format
#define QSPI_FRF_RESV       (0x3<<6)    //       (RESERVED): Reserved
										// 4:0   Data Frame Size
#define QSPI_DFS_01B        (0x0)       //       (DFS_01_BIT): Reserved
#define QSPI_DFS_02B        (0x1)       //       (DFS_02_BIT): Reserved
#define QSPI_DFS_03B        (0x2)       //       (DFS_03_BIT): Reserved
#define QSPI_DFS_04B        (0x3)       //       (DFS_04_BIT): 04-bit serial data transfer
#define QSPI_DFS_05B        (0x4)       //       (DFS_05_BIT): 05-bit serial data transfer
#define QSPI_DFS_06B        (0x5)       //       (DFS_06_BIT): 06-bit serial data transfer
#define QSPI_DFS_07B        (0x6)       //       (DFS_07_BIT): 07-bit serial data transfer
#define QSPI_DFS_08B        (0x7)       //       (DFS_08_BIT): 08-bit serial data transfer
#define QSPI_DFS_09B        (0x8)       //       (DFS_09_BIT): 09-bit serial data transfer
#define QSPI_DFS_10B        (0x9)       //       (DFS_10_BIT): 10-bit serial data transfer
#define QSPI_DFS_11B        (0xa)       //       (DFS_11_BIT): 11-bit serial data transfer
#define QSPI_DFS_12B        (0xb)       //       (DFS_12_BIT): 12-bit serial data transfer
#define QSPI_DFS_13B        (0xc)       //       (DFS_13_BIT): 13-bit serial data transfer
#define QSPI_DFS_14B        (0xd)       //       (DFS_14_BIT): 14-bit serial data transfer
#define QSPI_DFS_15B        (0xe)       //       (DFS_15_BIT): 15-bit serial data transfer
#define QSPI_DFS_16B        (0xf)       //       (DFS_16_BIT): 16-bit serial data transfer
#define QSPI_DFS_17B        (0x10)      //       (DFS_17_BIT): 17-bit serial data transfer
#define QSPI_DFS_18B        (0x11)      //       (DFS_18_BIT): 18-bit serial data transfer
#define QSPI_DFS_19B        (0x12)      //       (DFS_19_BIT): 19-bit serial data transfer
#define QSPI_DFS_20B        (0x13)      //       (DFS_20_BIT): 20-bit serial data transfer
#define QSPI_DFS_21B        (0x14)      //       (DFS_21_BIT): 21-bit serial data transfer
#define QSPI_DFS_22B        (0x15)      //       (DFS_22_BIT): 22-bit serial data transfer
#define QSPI_DFS_23B        (0x16)      //       (DFS_23_BIT): 23-bit serial data transfer
#define QSPI_DFS_24B        (0x17)      //       (DFS_24_BIT): 24-bit serial data transfer
#define QSPI_DFS_25B        (0x18)      //       (DFS_25_BIT): 25-bit serial data transfer
#define QSPI_DFS_26B        (0x19)      //       (DFS_26_BIT): 26-bit serial data transfer
#define QSPI_DFS_27B        (0x1a)      //       (DFS_27_BIT): 27-bit serial data transfer
#define QSPI_DFS_28B        (0x1b)      //       (DFS_28_BIT): 28-bit serial data transfer
#define QSPI_DFS_29B        (0x1c)      //       (DFS_29_BIT): 29-bit serial data transfer
#define QSPI_DFS_30B        (0x1d)      //       (DFS_30_BIT): 30-bit serial data transfer
#define QSPI_DFS_31B        (0x1e)      //       (DFS_31_BIT): 31-bit serial data transfer
#define QSPI_DFS_32B        (0x1f)      //       (DFS_32_BIT): 32-bit serial data transfer
// ----------------------------------------------------------------------------
// SPI_CTRLR0 - SPI Control Register
// ----------------------------------------------------------------------------
#define QSPI_XIP_CONTX_EN   (0x1<<21)   // 21    Enable continuous transfer in XIP mode
#define QSPI_XIP_INST_EN    (0x1<<20)   // 20    XIP instruction enable bit.
#define QSPI_XIP_DFS_HC     (0x1<<19)   // 19    Fix DFS for XIP transfers
#define QSPI_SPI_RXDS_EN    (0x1<<18)   // 18    Read data strobe enable bit
#define QSPI_INST_DDR_EN    (0x1<<17)   // 17    Instruction DDR Enable bit
#define QSPI_SPI_DDR_EN     (0x1<<16)   // 16    SPI DDR Enable bit
										// 15:11 WAIT_CYCLES Wait cycles in Dual/Quad/Octal mode
#define QSPI_WAIT_4C        (0x4<<11)
#define QSPI_WAIT_8C        (0x8<<11)
										// 9:8   INST_L Dual/Quad/Octal mode instruction length in bits
#define QSPI_INST_L0        (0x0<<8)    //       (INST_L0): No Instruction
#define QSPI_INST_L4        (0x1<<8)    //       (INST_L4): 4-bit Instruction length
#define QSPI_INST_L8        (0x2<<8)    //       (INST_L8): 8-bit Instruction length
#define QSPI_INST_L16       (0x3<<8)    //       (INST_L16): 16-bit Instruction length
#define QSPI_XIP_MD_BIT_EN  (0x1<<7)    // 7     Varies Mode bits enable in XIP mode
										// 5:2   ADDR_L This bit defines Address Length to be transmitted
#define QSPI_ADDR_L0        (0x0<<2)    //       (ADDR_L0): No Address
#define QSPI_ADDR_L4        (0x1<<2)    //       (ADDR_L4): 4-bit Address length
#define QSPI_ADDR_L8        (0x2<<2)    //       (ADDR_L8): 8-bit Address length
#define QSPI_ADDR_L12       (0x3<<2)    //       (ADDR_L12): 12-bit Address length
#define QSPI_ADDR_L16       (0x4<<2)    //       (ADDR_L16): 16-bit Address length
#define QSPI_ADDR_L20       (0x5<<2)    //       (ADDR_L20): 20-bit Address length
#define QSPI_ADDR_L24       (0x6<<2)    //       (ADDR_L24): 24-bit Address length
#define QSPI_ADDR_L28       (0x7<<2)    //       (ADDR_L28): 28-bit Address length
#define QSPI_ADDR_L32       (0x8<<2)    //       (ADDR_L32): 32-bit Address length
#define QSPI_ADDR_L36       (0x9<<2)    //       (ADDR_L36): 36-bit Address length
#define QSPI_ADDR_L40       (0xa<<2)    //       (ADDR_L40): 40-bit Address length
#define QSPI_ADDR_L44       (0xb<<2)    //       (ADDR_L44): 44-bit Address length
#define QSPI_ADDR_L48       (0xc<<2)    //       (ADDR_L48): 48-bit Address length
#define QSPI_ADDR_L52       (0xd<<2)    //       (ADDR_L52): 52-bit Address length
#define QSPI_ADDR_L56       (0xe<<2)    //       (ADDR_L56): 56-bit Address length
#define QSPI_ADDR_L60       (0xf<<2)    //       (ADDR_L60): 60-bit Address length
										// 1:0   TRANS_TYPE Address and instruction transfer format
#define QSPI_TRANS_TT0      (0x0)       //       (TT0): Instruction and Address are sent in Standard SPI Mode.
#define QSPI_TRANS_TT1      (0x1)       //       (TT1): Instruction is sent in Standard SPI Mode and Address is sent in the mode specified by CTRLR0.SPI_FRF.
#define QSPI_TRANS_TT2      (0x2)       //       (TT2): Both Instruction and Address are sent in the mode specified by SPI_FRF.
#define QSPI_TRANS_TT3      (0x3)       //       (TT3): Reserved.
// ----------------------------------------------------------------------------
// SR - Status register
// ----------------------------------------------------------------------------
#define QSPI_ST_DCOL        (0x1<<6)    // 6     DCOL Data Collision Error
#define QSPI_ST_TXE         (0x1<<5)    // 5     TXE Transmission Error
#define QSPI_ST_RFF         (0x1<<4)    // 4     RFF Receive FIFO Full
#define QSPI_ST_RFNE        (0x1<<3)    // 3     RFNE Receive FIFO Not Empty
#define QSPI_ST_TFE         (0x1<<2)    // 2     TFE Transmit FIFO Empty
#define QSPI_ST_TFNF        (0x1<<1)    // 1     TFNF Transmit FIFO Not Full
#define QSPI_ST_BUSY        (0x1<<0)    // 0     BUSY SSI Busy Flag
// ----------------------------------------------------------------------------
// QSPI Configuration Register
// ----------------------------------------------------------------------------
#define QSPI_CFG_IDLE    (0x80000000)   //    [31] Serial Interface and QSPI pipeline is IDLE.  (0x1<<31)
#define QSPI_CFG_PIPEPHYME  (0x1<<25)   //    [25] Pipeline PHY Mode enable
#define QSPI_CFG_DTREN      (0x1<<24)   //    [24] Enable DTR Protocol
#define QSPI_CFG_DECEN      (0x1<<23)   //    [23] Enable AHB Decoder
										// [22:19] Master mode baud rate divisor
#define QSPI_CFG_BUAD2      (0x0<<19)   //         0000 = /2
#define QSPI_CFG_BUAD4      (0x1<<19)   //         0001 = /4
#define QSPI_CFG_BUAD6      (0x2<<19)   //         0010 = /6
#define QSPI_CFG_BUAD8      (0x3<<19)   //         0011 = /8
#define QSPI_CFG_BUAD10     (0x4<<19)   //         0100 = /10
#define QSPI_CFG_BUAD12     (0x5<<19)   //         0101 = /12
#define QSPI_CFG_BUAD14     (0x6<<19)   //         0110 = /14
#define QSPI_CFG_BUAD16     (0x7<<19)   //         0111 = /16
#define QSPI_CFG_BUAD18     (0x8<<19)   //         1000 = /18
#define QSPI_CFG_BUAD20     (0x9<<19)   //         1001 = /20
#define QSPI_CFG_BUAD22     (0xA<<19)   //         1010 = /22
#define QSPI_CFG_BUAD24     (0xB<<19)   //         1011 = /24
#define QSPI_CFG_BUAD26     (0xC<<19)   //         1100 = /26
#define QSPI_CFG_BUAD28     (0xD<<19)   //         1101 = /28
#define QSPI_CFG_BUAD30     (0xE<<19)   //         1110 = /30
#define QSPI_CFG_BUAD32     (0xF<<19)   //         1111 = /32
#define QSPI_CFG_XIPIM      (0x1<<18)   // [18]    Enter XIP Mode immediately
#define QSPI_CFG_XIPNR      (0x1<<17)   // [17]    Enter XIP Mode on next READ
#define QSPI_CFG_REMAP      (0x1<<16)   // [16]    Enable AHB Address Re-mapping
#define QSPI_CFG_DMAEN      (0x1<<15)   // [15]    Enable DMA Peripheral Interface
#define QSPI_CFG_WPDRV      (0x1<<14)   // [14]    Set to drive the Write Protect pin of the FLASH device.
										// [13:10] Peripheral chip select lines If bit 9 of this register
#define QSPI_CFG_PSDEC      (0x1<<9)    // [9]     Peripheral select decode
#define QSPI_CFG_LEGACY     (0x1<<8)    // [8]     Legacy IP Mode Enable
#define QSPI_CFG_DACEN      (0x1<<7)    // [7]     Enable Direct Access Controller
#define QSPI_CFG_PHYME      (0x1<<3)    // [3]     PHY Mode enable
#define QSPI_CFG_CPHA       (0x1<<2)    // [2]     Clock phase ¨C This maps to the standard SPI CPHA transfer format.
#define QSPI_CFG_CPOL       (0x1<<1)    // [1]     Clock polarity outside SPI word. This maps to the standard SPI CPOL transfer format.
#define QSPI_CFG_QSPIEN     (0x1<<0)    // [0]     QSPI Enable
// ----------------------------------------------------------------------------
// Device Read Instruction Register
// ----------------------------------------------------------------------------
#define QSPI_RDINS_DUMMY0   (0x0<<24)
#define QSPI_RDINS_DUMMY1   (0x1<<24)
#define QSPI_RDINS_DUMMY2   (0x2<<24)
#define QSPI_RDINS_DUMMY3   (0x3<<24)
#define QSPI_RDINS_DUMMY4   (0x4<<24)
#define QSPI_RDINS_DUMMY8   (0x8<<24)
#define QSPI_RDINS_MODEBE   (0x1<<20)   // [20]    Mode Bit Enable
										// [17:16] Data Transfer Type for Standard SPI modes
#define QSPI_RDINS_DTTSIO   (0x0<<16)   //         0: SIO mode ¨C data is shifted to the device on DQ0 only and from the device on DQ1 only
#define QSPI_RDINS_DTTDIO   (0x1<<16)   //         1: Used for Dual Input/Output instructions. For data transfers, DQ0 and DQ1 are used as both inputs and outputs.
#define QSPI_RDINS_DTTQIO   (0x2<<16)   //         2: Used for Quad Input/Output instructions. For data transfers, DQ0, DQ1, DQ2 and DQ3 are used as both inputs and outputs.
										// [13:12] Address Transfer Type for Standard SPI modes
#define QSPI_RDINS_ATTSIO   (0x0<<12)   //         0: Addresses can be shifted to the device on DQ0 only
#define QSPI_RDINS_ATTDIO   (0x1<<12)   //         1: Addresses can be shifted to the device on DQ0 and DQ1 only
#define QSPI_RDINS_ATTQIO   (0x2<<12)   //         2: Addresses can be shifted to the device on DQ0, DQ1, DQ2 and DQ3
#define QSPI_RDINS_DDRBE    (0x1<<10)   // [10]    DDR Bit Enable
										// [9:8]   Instruction Type
#define QSPI_RDINS_ITTSIO   (0x0<<8)    //         0 : Use Standard SPI mode (instruction always shifted into the device on DQ0 only)
#define QSPI_RDINS_ITTDIO   (0x1<<8)    //         1 : Use DIO-SPI mode (Instructions, Address and Data always sent on DQ0 and DQ1)
#define QSPI_RDINS_ITTQIO   (0x2<<8)    //         2 : Use QIO-SPI mode ((Instructions, Address and Data always sent on DQ0,DQ1,DQ2 and DQ3)
										// [7:0]   Read Opcode to use when not in XIP mode

// Read Data Capture Register
#define QSPI_RDDC_EDGE      (0x1<<5)    // [5]     Sample edge selection (of the flash memory data outputs
#define QSPI_RDDC_ALCC      (0x1<<0)    // [0]     Bypass of the adapted loopback clock circuit

// Device Write Instruction Configuration Register
										// [28:24] Number of Dummy Clock Cycles required by device for Write Instruction
										// [17:16] Data Transfer Type for Standard SPI modes
#define QSPI_WRINS_DTTSIO   (0x0<<16)   //         0: SIO mode ¨C data is shifted to the device on DQ0 only and from the device on DQ1 only
#define QSPI_WRINS_DTTDIO   (0x1<<16)   //         1: Used for Dual Input/Output instructions. For data transfers, DQ0 and DQ1 are used as both inputs and outputs
#define QSPI_WRINS_DTTQIO   (0x2<<16)   //         2: Used for Quad Input/Output instructions. For data transfers, DQ0, DQ1, DQ2 and DQ3 are used as both inputs and outputs
										// [13:12] Address Transfer Type for Standard SPI modes
#define QSPI_WRINS_ATTSIO   (0x0<<12)   //         0 : Addresses can be shifted to the device on DQ0 only
#define QSPI_WRINS_ATTDIO   (0x1<<12)   //         1 : Addresses can be shifted to the device on DQ0 and DQ1 only
#define QSPI_WRINS_ATTQIO   (0x2<<12)   //         2 : Addresses can be shifted to the device on DQ0, DQ1, DQ2 and DQ3
#define QSPI_WRINS_WELDIS   (0x1<<8)    // [8]     WEL Disable

// Device Size Configuration Register
										// [28:27] Size of Flash Device connected to CS[3] pin
										// [26:25] Size of Flash Device connected to CS[2] pin
										// [24:23] Size of Flash Device connected to CS[1] pin
										// [22:21] Size of Flash Device connected to CS[0] pin
										// [20:16] Number of bytes per block.
#define QSPI_DSIZE_NB_32K   (16<<16)    //         16 - 32768 bytes
										// [15:4]  Number of bytes per device page
#define QSPI_DSIZE_NP_256   (256<<4)    //         256 bytes
										// [3:0]   Number of address bytes
#define QSPI_DSIZE_NA_1     (0)         //         1 byte
#define QSPI_DSIZE_NA_2     (1)         //         2 bytes
#define QSPI_DSIZE_NA_3     (2)         //         3 bytes
#define QSPI_DSIZE_NA_4     (3)         //         4 bytes

// Write Protection Register
#define QSPI_WP_EN          (0x1<<1)    // [1]     Write Protection Enable Bit
#define QSPI_WP_INV         (0x1<<0)    // [0]     Write Protection Inversion Bit

// Flash Command Control Register (Using STIG)
										// [31:24] Command Opcode
#define QSPI_FCMD_RDEN      (0x1<<23)   // [23]    Read Data Enable
										// [22:20] Number of Read Data Bytes
#define QSPI_FCMD_CMDAE     (0x1<<19)   // [19]    Command Address Enable
#define QSPI_FCMD_MODEBE    (0x1<<18)   // [18]    Mode Bit Enable
										// [17:16] Number of Address Bytes
#define QSPI_FCMD_WDEN      (0x1<<15)   // [15]    Write Data Enable
										// [14:12] Number of Write Data Bytes
										// [11:7]  Number of Dummy Bytes
#define QSPI_FCMD_CMDEIP    (0x1<<1)    // [1]     Command execution in progress.
#define QSPI_FCMD_EXEC      (0x1<<0)    // [0]     Execute the command.


typedef enum opt_e {
	E_PAGE = 0x20,
	E_BLOCK = 0xD8,
	E_WHOLE = 0xc7,
}OPT_E;

#define SPI_IMI_FLASH_MAX_ID_LEN    (4)
struct spi_imi_flash_info {
	const char *name;

	U8 id[SPI_IMI_FLASH_MAX_ID_LEN]; //0xffc8c2 or 0xffc8c1
	unsigned int id_len;                       //0x0

	unsigned int page_shift;         //11
	unsigned int sector_shift;
	unsigned int block_shift;        //17
	unsigned int block_num;          //2048 or 1024
	unsigned int spare_size;         //128 or 256
	unsigned int flags;
};

//////////////////////////////////////////api interface/////////////////////////////////
/* For command RDID, RDSR. */
int cadence_qspi_command_read(unsigned int cmdlen, const u8 *cmdbuf,
	 unsigned int rxlen, u8 *rxbuf);

/* For commands: WRSR, WREN, WRDI, CHIP_ERASE, BE, etc. */
int cadence_qspi_command_write(unsigned int cmdlen,
		 const u8 *cmdbuf, unsigned int txlen,  const u8 *txbuf);


/* Opcode + Address (3/4 bytes) */
int cadence_qspi_indirect_write_setup(const u8 *cmdbuf, unsigned int cmdlen);
int cadence_qspi_indirect_write_execute(const unsigned char *txbuf, int n_tx, unsigned int page_size);

/* Opcode + Address (3/4 bytes) + dummy bytes (0-4 bytes) */
int  cadence_qspi_indirect_read_setup(const u8 *cmdbuf, unsigned int cmdlen);
int  cadence_qspi_indirect_read_execute(unsigned int n_rx,               unsigned char *rxbuf);

void  candence_qspi_controller_setup(U32 sclk_mhz, unsigned char loopback_clk_bypass, unsigned int page_size, unsigned int blk_size);
void cadence_qspi_controller_init(unsigned int page_size, unsigned int blk_size);

void candence_qspi_cs_open(void);
void candence_qspi_cs_close(void);
void candence_clk_delay_config(int div, int delay);
int cadence_qspi_setup_dma(void);

#endif // __QSPI_REG_H__

