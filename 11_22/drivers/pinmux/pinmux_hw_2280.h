#if defined(CHIP_IMI_2280)
#include "hellboy_map.h"
#define PINMUX_BITMASK		0x7

#define U2			(0x80)
#define PINMUX_GAP	(0x2000000)

enum IMI_2280_PINPAD {
	GPIO0 = 0,				//2:pwm0_0 7:IR_MCLK
	GPIO1,					//2:pwm0_1 7:IR_MCLK
	GPIO2,					//2:pwm0_2 3:IR_MCLK	7:clkout2
	GPIO3,					//1:njtop_sensor_vs  2:pwm0_3 3:IR_MCLK	7:clkout3
	GPIO4,					//1:njtop_trigger_ir 2:pwm0_4 3:IR_MCLK	7:clkout4
	GPIO5,					//1:njtop_trigger_gs 2:pwm0_5 3:IR_MCLK	7:clkout5
	GPIO6,					//1:njtop_trigger_color 2:pwm0_6 3:IR_MCLK	7:clkout6
	GPIO7,					//1:GPIOB0 2:pwm0_7 3:IR_MCLK	7:pll_out
	GPIO8,					//1:PWM0_8
	GPIO9,					//1:PWM0_9
	GPIO10,					//1:PWM0_10
	GPIO11,					//1:PWM0_11
	GPIO12,					//1:dsp0_tck
	GPIO13,					//1:dsp0_tdi
	GPIO14,					//1:dsp0_tms
	GPIO15,					//1:dsp0_tdo
	GPIO16,					//2:dsp0_gp_out0
	GPIO17,					//2:dsp0_gp_out1
	GPIO18,					//1:dsp0_gpi_in2 2:dsp0_gp_out2
	GPIO19,					//1:dsp0_gpi_in3 2:dsp0_gp_out3
	GPIO20,					//1:dsp0_gpi_in4
	GPIO21,					//1:dsp0_gpi_in5
	GPIO22,					//1:dsp0_gpi_in6
	GPIO23,					//1:dsp0_gpi_in7
	GPIO24,
	GPIO25,
	GPIO26,
	GPIO27,
	GPIO28,					//7:PWM1_15
	GPIO29,					//1:isp_f1_trig	7:PWM1_14
	GPIO30,					//1:uart1_txd	7:PWM1_13
	GPIO31,					//1:uart1_rxd	7:PWM1_12
	GPIO32,					//1:dsp0_rtck	7:PWM1_11
	GPIO33,	  				//2:is20_sdo3	7:PWM1_10
	GPIO34,	  				//2:is20_sdo2	7:PWM1_9
	GPIO35,	  				//2:is20_sdo1	7:PWM1_8
	GPIO36,	  				//2:is20_sdo0	7:PWM1_7
	GPIO37,	  				//1:is20_sdi3	7:PWM1_6
	GPIO38,	  				//1:is20_sdi2	7:PWM1_5
	GPIO39,	  				//1:is20_sdi1	7:PWM1_4
	GPIO40,	  				//1:isp_flash_trig	7:PWM1_3
	GPIO41,  				//1:isp_prelight_trig	7:PWM1_2
	GPIO42,  				//1:isp_shutter_trig	6: i2c2_scl 7:PWM1_1
	GPIO43,  				//1:isp_shutter_open	6: i2c2_sda 7:PWM1_0
	GPIO44,  				//1:sdio_clk
	GPIO45,  				//1:sdio_cmd
	GPIO46,  				//1:sdio_dat0
	GPIO47,  				//1:sdio_dat1
	GPIO48,  				//1:sdio_dat2
	GPIO49,  				//1:sdio_dat3
	GPIO50,  				//1:sdio_dat4
	GPIO51,  				//1:sdio_dat5
	GPIO52,  				//1:sdio_dat6
	GPIO53,  				//1:sdio_dat7
	GPIO54,
	GPIO55,
	GPIO56,
	GPIO57,
	GPIO58,
	GPIO59,
	CM0_VREF,  				//2:GPIOB16		7:clkout2
	CM0_HREF,  				//2:GPIOB15		7:clkout3
	CM0_DAT0,  				//2:GPIOB7
	CM0_DAT1,  				//2:GPIOB8
	CM0_DAT2,  				//2:GPIOB9		7:clkout1
	CM0_DAT3,  				//2:GPIOB10		7:clkout0
	CM0_DAT4,  				//2:GPIOB3		7:pllout
	CM0_DAT5,  				//7:clkout6
	CM0_DAT6,  				//7:clkout5
	CM0_DAT7,  				//7:clkout4
	CM0_DAT8,  				//2:GPIOB11		7:pllout
	CM0_DAT9,  				//2:GPIOB12		7:clkout6
	CM0_DAT10,  			//2:GPIOB13		7:clkout5
	CM0_DAT11,  			//2:GPIOB14		7:clkout4
	CM0_PCLK,  				//2:GPIOB17
	CM0_CLK,  				//1: pll_lock 	2:GPIOB31
	CM1_VREF,
	CM1_HREF,
	CM1_DAT0,
	CM1_DAT1,
	CM1_DAT2,
	CM1_DAT3,
	CM1_DAT4,
	CM1_DAT5,
	CM1_DAT6,
	CM1_DAT7,
	CM1_DAT8,
	CM1_DAT9,
	CM1_DAT10,
	CM1_DAT11,
	CM1_PCLK,
	CM1_CLK,
	TCK,					//1:pwm0_12		2:gpioa12
	TMS,					//1:pwm0_13		2:gpioa13
	TDI,					//1:pwm0_14		2:gpioa14	3:dpu_pixen_1	4:dpu_pixen_0
	TDO,					//1:pwm0_15		2:gpioa15
	RTCK,					//2:gpioa16
	TRST,					//2:gpioa17
	UART0_TXD,				//1:gpioa18		2:IR_MCLK
	UART0_RXD,				//1:gpioa19	
	SPI0_FRM,				//1:gpioa20
	SPI0_CLK,				//1:gpioa21
	SPI0_TXD,				//1:gpioa22
	SPI0_RXD,				//1:gpioa23
	I2C0_SCL,				//1:gpioa24
	I2C0_SDA,				//1:gpioa25
	I2C1_SCL,				//1:gpioa26
	I2C1_SDA,				//1:gpioa27
	SPI1_FRM,
	SPI1_CLK,
	SPI1_TXD,
	SPI1_RXD,
	IR_PCLK,				//1:dpu_pixclk_1	3:dpu_pixclk_0
	IR_LINEV,				//1:dpu_hsync_1		3:dpu_hsync_0
	IR_FRMV,				//1:dpu_vsync_1		3:dpu_vsync_0
	IR_DOUT0,
	IR_DOUT1,
	IR_DOUT2,
	IR_DOUT3,
	IR_DOUT4,
	IR_DOUT5,
	IR_DOUT6,
	IR_DOUT7,
	IR_DOUT8,
	IR_DOUT9,
	IR_DOUT10,				//1:gpiob2
	IR_DOUT11,				//1:gpiob1
	I2S_MCLK,				//1:gpioa28		2:IR_MCLK
	I2S0_BCLK,				//1:gpioa29
	I2S0_LRCK,				//1:gpioa30
	I2S0_SDI,				//1:gpioa31
	PIN_MAX = 131,
};


#endif/*CHIP_IMI_2280*/
