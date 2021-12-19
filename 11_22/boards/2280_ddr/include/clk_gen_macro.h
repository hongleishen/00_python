#ifndef CLK_GEN_MACRO_H
#define CLK_GEN_MACRO_H

//#define CLK_GEN_BASE_ADDR 0x0
#define ENABLE_CLOCK(value) value|=0x80000000
#define DISABLE_CLOCK(value) value&=0x7FFFFFFF
#define POLARITY_NEG_CLOCK(value) value|=0x10000000
#define POLARITY_POS_CLOCK(value) value&=0xEFFFFFFF
#define CLOCK_SELECT(value,n) value=(value&0xFFFCFFFF)|((n&0x3)<<16)
#define CLOCK_DIVIDE(value,n) value=(value&0xFFFF0000)|(n&0xFFFF)
#define ENABLE_PD_CLOCK(value,n) value|=((uint32_t)1<<n)
#define DISABLE_PD_CLOCK(value,n) value&=~((uint32_t)1<<n)
#define a7_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000000
#define adc_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000004
#define adc_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000008
#define cif_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000000c
#define cif_clk_pre_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000010
#define clock_1mhz_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000014
#define cpu_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000018
#define cpu_clk_pre_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000001c
#define dpu_pix_clk_0_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000020
#define dpu_pix_clk_1_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000024
#define dpu_pix_clk_2_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000028
#define dsp0_intc_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000002c
#define dsp1_intc_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000030
#define fast_bus_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000034
#define fast_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000038
#define fpe_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000003c
#define fpe_cclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000040
#define fpe_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000044
#define i2s_mclk_i_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000048
#define main_root_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000004c
#define main_root_clk_pre_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000050
#define mbox0_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000054
#define mbox1_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000058
#define mbox2_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000005c
#define mipi_dphy0_esc_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000060
#define mipi_dphy1_esc_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000064
#define mipi_dphy2_esc_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000068
#define mipi_phy0_cfg_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000006c
#define mipi_phy0_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000070
#define mipi_phy1_cfg_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000074
#define mipi_phy1_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000078
#define mipi_phy2_cfg_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000007c
#define mipi_phy2_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000080
#define mst_bus_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000084
#define mst_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000088
#define nj_top_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000008c
#define nj_top_clk_arm0_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000090
#define nj_top_clk_arm1_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000094
#define nj_top_clk_dpu_slv_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000098
#define nj_top_clk_fast_bus_m_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000009c
#define nj_top_clk_fast_bus_s_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000a0
#define nj_top_clk_nj_apb_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000a4
#define pll0_out_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000a8
#define pll0_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000ac
#define pll1_out_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000b0
#define pll1_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000b4
#define pll1_ref_clk_pre_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000b8
#define pll2_out_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000bc
#define pll2_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000c0
#define pll2_ref_clk_pre_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000c4
#define skt_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000c8
#define skt_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000cc
#define skt_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000d0
#define slow_bus_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000d4
#define slow_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000d8
#define u0_ahb2apb_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000dc
#define u0_ahb_ram_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000e0
#define u0_ahb_rom_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000e4
#define u0_cfg_bus_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000e8
#define u0_clkgen_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000ec
#define u0_cmif_mclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000f0
#define u0_cmif_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000f4
#define u0_cqspi_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000f8
#define u0_cqspi_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000000fc
#define u0_cqspi_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000100
#define u0_csi_ipi_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000104
#define u0_ddr_aclk_0_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000108
#define u0_ddr_aclk_1_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000010c
#define u0_ddr_core_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000110
#define u0_ddr_ctl_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000114
#define u0_ddr_phy_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000118
#define u0_dmac_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000011c
#define u0_dmac_mst_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000120
#define u0_dmac_slv_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000124
#define u0_dsp_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000128
#define u0_dw_i2s_gclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000012c
#define u0_dw_i2s_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000130
#define u0_fast_bus_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000134
#define u0_gpio_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000138
#define u0_gpio_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000013c
#define u0_hs_device_utmi_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000140
#define u0_i2c_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000144
#define u0_i2c_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000148
#define u0_ircmif_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000014c
#define u0_ircmif_mclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000150
#define u0_isp_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000154
#define u0_isp_jpeg_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000158
#define u0_isp_sclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000015c
#define u0_mst_bus_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000160
#define u0_ocram_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000164
#define u0_pmu_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000168
#define u0_pwm_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000016c
#define u0_pwm_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000170
#define u0_rstgen_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000174
#define u0_sdio_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000178
#define u0_sdio_logic_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000017c
#define u0_slow_bus_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000180
#define u0_spi_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000184
#define u0_spi_sspclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000188
#define u0_sysbus_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000018c
#define u0_sysbus_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000190
#define u0_syscon_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000194
#define u0_timer64_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000198
#define u0_timer64_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000019c
#define u0_timer_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001a0
#define u0_timer_timer0_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001a4
#define u0_timer_timer1_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001a8
#define u0_timer_timer2_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001ac
#define u0_timer_timer3_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001b0
#define u0_timer_timer4_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001b4
#define u0_timer_timer5_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001b8
#define u0_timer_timer6_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001bc
#define u0_timer_wdog_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001c0
#define u0_uart_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001c4
#define u0_uart_uartclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001c8
#define u0_usb3_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001cc
#define u0_usb3_macro_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001d0
#define u0_usb3_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001d4
#define u0_usb3_ref_clk_pre_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001d8
#define u0_usb3_suspend_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001dc
#define u0_usb_dev_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001e0
#define u0_vi_domain_aclk_0_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001e4
#define u0_vi_domain_aclk_1_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001e8
#define u0_vi_domain_pclk_0_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001ec
#define u0_vi_domain_pclk_1_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001f0
#define u0_vi_domain_pclk_2_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001f4
#define u0_vi_domain_pclk_3_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001f8
#define u0_vi_domain_pclk_4_CLOCK_REG CLK_GEN_BASE_ADDR+0x000001fc
#define u0_vic_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000200
#define u1_ahb2apb_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000204
#define u1_cmif_mclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000208
#define u1_csi_ipi_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000020c
#define u1_dsp_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000210
#define u1_gpio_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000214
#define u1_gpio_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000218
#define u1_i2c_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000021c
#define u1_i2c_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000220
#define u1_pwm_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000224
#define u1_pwm_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000228
#define u1_spi_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000022c
#define u1_spi_sspclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000230
#define u1_timer64_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000234
#define u1_timer_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000238
#define u1_timer_timer0_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000023c
#define u1_timer_timer1_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000240
#define u1_timer_timer2_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000244
#define u1_timer_timer3_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000248
#define u1_timer_timer4_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000024c
#define u1_timer_timer5_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000250
#define u1_timer_timer6_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000254
#define u1_timer_wdog_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000258
#define u1_uart_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000025c
#define u1_uart_uartclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000260
#define u1_vic_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000264
#define u2_ahb2apb_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000268
#define u2_csi_ipi_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000026c
#define u2_gpio_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000270
#define u2_gpio_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000274
#define u2_i2c_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000278
#define u2_vic_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000027c
#define u3_ahb2apb_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000280
#define u3_vic_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000284
#define usb3_phy_ref_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000288
#define usb3_phy_ref_clk_pre_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000028c
#define vip8000_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000290
#define vip8000_core_clk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000294
#define vip8000_hclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x00000298
#define vpu_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x0000029c
#define vpu_cclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000002a0
#define vpu_fpe_aclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000002a4
#define vpu_fpe_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000002a8
#define vpu_pclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000002ac
#define vpu_rclk_CLOCK_REG CLK_GEN_BASE_ADDR+0x000002b0

/********************************************************************************
* clock a7_clk 
********************************************************************************/

#define ENABLE_CLOCK_a7_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(a7_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(a7_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_a7_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(a7_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(a7_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_a7_clk_SOURCE_pll5_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(a7_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(a7_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_a7_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(a7_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(a7_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_a7_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(a7_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(a7_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_a7_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(a7_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(a7_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_a7_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(a7_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(a7_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock adc_clk 
********************************************************************************/

#define ENABLE_CLOCK_adc_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(adc_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(adc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_adc_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(adc_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(adc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_adc_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(adc_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(adc_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock adc_pclk 
********************************************************************************/

#define ENABLE_CLOCK_adc_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(adc_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(adc_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_adc_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(adc_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(adc_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock cif_clk 
********************************************************************************/

#define ENABLE_CLOCK_cif_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(cif_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(cif_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_cif_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(cif_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(cif_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_cif_clk_SOURCE_cif_clk_pre { \
uint32_t _clk_gen_reg_value_=MA_INW(cif_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(cif_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_cif_clk_SOURCE_ext_clk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(cif_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(cif_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock cif_clk_pre 
********************************************************************************/

#define DIVIDE_cif_clk_pre(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(cif_clk_pre_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(cif_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock clock_1mhz 
********************************************************************************/

#define DIVIDE_clock_1mhz(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(clock_1mhz_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(clock_1mhz_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock cpu_clk 
********************************************************************************/

#define SWITCH_cpu_clk_SOURCE_cpu_clk_pre { \
uint32_t _clk_gen_reg_value_=MA_INW(cpu_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(cpu_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_cpu_clk_SOURCE_main_root_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(cpu_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(cpu_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock cpu_clk_pre 
********************************************************************************/

#define DIVIDE_cpu_clk_pre(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(cpu_clk_pre_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(cpu_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock dpu_pix_clk_0 
********************************************************************************/

#define ENABLE_CLOCK_dpu_pix_clk_0 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_0_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dpu_pix_clk_0_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_dpu_pix_clk_0 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_0_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dpu_pix_clk_0_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_0_SOURCE_pad_pixclk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_0_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(dpu_pix_clk_0_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_0_SOURCE_u0_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_0_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(dpu_pix_clk_0_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_0_SOURCE_u1_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_0_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(dpu_pix_clk_0_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_0_SOURCE_u2_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_0_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(dpu_pix_clk_0_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock dpu_pix_clk_1 
********************************************************************************/

#define ENABLE_CLOCK_dpu_pix_clk_1 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_1_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dpu_pix_clk_1_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_dpu_pix_clk_1 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_1_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dpu_pix_clk_1_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_1_SOURCE_u0_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_1_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(dpu_pix_clk_1_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_1_SOURCE_u1_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_1_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(dpu_pix_clk_1_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_1_SOURCE_u2_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_1_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(dpu_pix_clk_1_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_1_SOURCE_pad_pixclk1 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_1_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(dpu_pix_clk_1_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock dpu_pix_clk_2 
********************************************************************************/

#define ENABLE_CLOCK_dpu_pix_clk_2 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_2_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dpu_pix_clk_2_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_dpu_pix_clk_2 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_2_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dpu_pix_clk_2_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_2_SOURCE_u1_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_2_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(dpu_pix_clk_2_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_2_SOURCE_u2_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_2_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(dpu_pix_clk_2_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_2_SOURCE_pad_pixclk1 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_2_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(dpu_pix_clk_2_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_dpu_pix_clk_2_SOURCE_pad_pixclk2 { \
uint32_t _clk_gen_reg_value_=MA_INW(dpu_pix_clk_2_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(dpu_pix_clk_2_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock dsp0_intc_pclk 
********************************************************************************/

#define ENABLE_CLOCK_dsp0_intc_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(dsp0_intc_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dsp0_intc_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_dsp0_intc_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(dsp0_intc_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dsp0_intc_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock dsp1_intc_pclk 
********************************************************************************/

#define ENABLE_CLOCK_dsp1_intc_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(dsp1_intc_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dsp1_intc_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_dsp1_intc_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(dsp1_intc_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(dsp1_intc_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock fast_bus_clk 
********************************************************************************/

#define SWITCH_fast_bus_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_bus_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(fast_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_fast_bus_clk_SOURCE_fast_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_bus_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(fast_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock fast_clk 
********************************************************************************/

#define ENABLE_CLOCK_fast_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(fast_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_fast_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(fast_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_fast_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(fast_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_fast_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(fast_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_fast_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(fast_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_fast_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(fast_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_fast_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(fast_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(fast_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock fpe_aclk 
********************************************************************************/

#define ENABLE_CLOCK_fpe_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(fpe_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_fpe_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(fpe_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock fpe_cclk 
********************************************************************************/

#define ENABLE_CLOCK_fpe_cclk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_cclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(fpe_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_fpe_cclk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_cclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(fpe_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_fpe_cclk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_cclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(fpe_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_fpe_cclk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_cclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(fpe_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_fpe_cclk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_cclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(fpe_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_fpe_cclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_cclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(fpe_cclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock fpe_pclk 
********************************************************************************/

#define ENABLE_CLOCK_fpe_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(fpe_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_fpe_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(fpe_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(fpe_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock i2s_mclk_i 
********************************************************************************/

#define ENABLE_CLOCK_i2s_mclk_i { \
uint32_t _clk_gen_reg_value_=MA_INW(i2s_mclk_i_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(i2s_mclk_i_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_i2s_mclk_i { \
uint32_t _clk_gen_reg_value_=MA_INW(i2s_mclk_i_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(i2s_mclk_i_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_i2s_mclk_i_SOURCE_main_root_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(i2s_mclk_i_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(i2s_mclk_i_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_i2s_mclk_i_SOURCE_pad_audio_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(i2s_mclk_i_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(i2s_mclk_i_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_i2s_mclk_i_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(i2s_mclk_i_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(i2s_mclk_i_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_i2s_mclk_i(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(i2s_mclk_i_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(i2s_mclk_i_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock main_root_clk 
********************************************************************************/

#define SWITCH_main_root_clk_SOURCE_main_root_clk_pre { \
uint32_t _clk_gen_reg_value_=MA_INW(main_root_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(main_root_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_main_root_clk_SOURCE_ext_clk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(main_root_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(main_root_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock main_root_clk_pre 
********************************************************************************/

#define SWITCH_main_root_clk_pre_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(main_root_clk_pre_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(main_root_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_main_root_clk_pre_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(main_root_clk_pre_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(main_root_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_main_root_clk_pre_SOURCE_fast_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(main_root_clk_pre_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(main_root_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_main_root_clk_pre_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(main_root_clk_pre_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(main_root_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mbox0_pclk 
********************************************************************************/

#define ENABLE_CLOCK_mbox0_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(mbox0_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mbox0_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mbox0_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(mbox0_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mbox0_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mbox1_pclk 
********************************************************************************/

#define ENABLE_CLOCK_mbox1_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(mbox1_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mbox1_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mbox1_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(mbox1_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mbox1_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mbox2_pclk 
********************************************************************************/

#define ENABLE_CLOCK_mbox2_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(mbox2_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mbox2_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mbox2_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(mbox2_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mbox2_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_dphy0_esc_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_dphy0_esc_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy0_esc_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_dphy0_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_dphy0_esc_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy0_esc_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_dphy0_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_dphy0_esc_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy0_esc_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_dphy0_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_dphy0_esc_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy0_esc_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_dphy0_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_dphy0_esc_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy0_esc_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_dphy0_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_dphy1_esc_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_dphy1_esc_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy1_esc_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_dphy1_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_dphy1_esc_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy1_esc_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_dphy1_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_dphy1_esc_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy1_esc_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_dphy1_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_dphy1_esc_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy1_esc_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_dphy1_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_dphy1_esc_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy1_esc_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_dphy1_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_dphy2_esc_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_dphy2_esc_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy2_esc_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_dphy2_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_dphy2_esc_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy2_esc_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_dphy2_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_dphy2_esc_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy2_esc_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_dphy2_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_dphy2_esc_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy2_esc_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_dphy2_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_dphy2_esc_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_dphy2_esc_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_dphy2_esc_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_phy0_cfg_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_phy0_cfg_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_cfg_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy0_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_phy0_cfg_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_cfg_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy0_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy0_cfg_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_cfg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_phy0_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy0_cfg_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_cfg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_phy0_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_phy0_cfg_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_cfg_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_phy0_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_phy0_ref_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_phy0_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_ref_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy0_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_phy0_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_ref_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy0_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy0_ref_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_phy0_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy0_ref_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_phy0_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_phy0_ref_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy0_ref_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_phy0_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_phy1_cfg_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_phy1_cfg_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_cfg_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy1_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_phy1_cfg_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_cfg_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy1_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy1_cfg_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_cfg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_phy1_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy1_cfg_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_cfg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_phy1_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_phy1_cfg_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_cfg_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_phy1_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_phy1_ref_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_phy1_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_ref_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_phy1_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_ref_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy1_ref_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_phy1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy1_ref_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_phy1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_phy1_ref_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy1_ref_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_phy1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_phy2_cfg_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_phy2_cfg_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_cfg_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy2_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_phy2_cfg_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_cfg_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy2_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy2_cfg_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_cfg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_phy2_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy2_cfg_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_cfg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_phy2_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_phy2_cfg_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_cfg_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_phy2_cfg_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mipi_phy2_ref_clk 
********************************************************************************/

#define ENABLE_CLOCK_mipi_phy2_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_ref_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mipi_phy2_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_ref_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mipi_phy2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy2_ref_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mipi_phy2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mipi_phy2_ref_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mipi_phy2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mipi_phy2_ref_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mipi_phy2_ref_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mipi_phy2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mst_bus_clk 
********************************************************************************/

#define SWITCH_mst_bus_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_bus_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mst_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mst_bus_clk_SOURCE_mst_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_bus_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mst_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock mst_clk 
********************************************************************************/

#define ENABLE_CLOCK_mst_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mst_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_mst_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(mst_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mst_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(mst_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mst_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(mst_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mst_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(mst_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_mst_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(mst_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_mst_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(mst_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(mst_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock nj_top_aclk 
********************************************************************************/

#define ENABLE_CLOCK_nj_top_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_nj_top_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_aclk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_aclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(nj_top_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_aclk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_aclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(nj_top_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_aclk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_aclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(nj_top_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_nj_top_aclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_aclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(nj_top_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock nj_top_clk_arm0 
********************************************************************************/

#define ENABLE_CLOCK_nj_top_clk_arm0 { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm0_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_arm0_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_nj_top_clk_arm0 { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm0_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_arm0_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_arm0_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm0_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(nj_top_clk_arm0_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_arm0_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm0_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(nj_top_clk_arm0_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_arm0_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm0_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(nj_top_clk_arm0_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_nj_top_clk_arm0(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm0_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(nj_top_clk_arm0_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock nj_top_clk_arm1 
********************************************************************************/

#define ENABLE_CLOCK_nj_top_clk_arm1 { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm1_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_arm1_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_nj_top_clk_arm1 { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm1_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_arm1_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_arm1_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm1_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(nj_top_clk_arm1_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_arm1_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm1_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(nj_top_clk_arm1_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_arm1_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm1_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(nj_top_clk_arm1_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_nj_top_clk_arm1(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_arm1_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(nj_top_clk_arm1_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock nj_top_clk_dpu_slv 
********************************************************************************/

#define ENABLE_CLOCK_nj_top_clk_dpu_slv { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_dpu_slv_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_dpu_slv_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_nj_top_clk_dpu_slv { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_dpu_slv_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_dpu_slv_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_dpu_slv_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_dpu_slv_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(nj_top_clk_dpu_slv_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_dpu_slv_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_dpu_slv_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(nj_top_clk_dpu_slv_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_nj_top_clk_dpu_slv_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_dpu_slv_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(nj_top_clk_dpu_slv_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_nj_top_clk_dpu_slv(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_dpu_slv_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(nj_top_clk_dpu_slv_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock nj_top_clk_fast_bus_m 
********************************************************************************/

#define ENABLE_CLOCK_nj_top_clk_fast_bus_m { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_fast_bus_m_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_fast_bus_m_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_nj_top_clk_fast_bus_m { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_fast_bus_m_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_fast_bus_m_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock nj_top_clk_fast_bus_s 
********************************************************************************/

#define ENABLE_CLOCK_nj_top_clk_fast_bus_s { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_fast_bus_s_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_fast_bus_s_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_nj_top_clk_fast_bus_s { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_fast_bus_s_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_fast_bus_s_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock nj_top_clk_nj_apb 
********************************************************************************/

#define ENABLE_CLOCK_nj_top_clk_nj_apb { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_nj_apb_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_nj_apb_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_nj_top_clk_nj_apb { \
uint32_t _clk_gen_reg_value_=MA_INW(nj_top_clk_nj_apb_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(nj_top_clk_nj_apb_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock pll0_out 
********************************************************************************/

#define ENABLE_CLOCK_pll0_out { \
uint32_t _clk_gen_reg_value_=MA_INW(pll0_out_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(pll0_out_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_pll0_out { \
uint32_t _clk_gen_reg_value_=MA_INW(pll0_out_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(pll0_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll0_out_SOURCE_pll0_out_wire { \
uint32_t _clk_gen_reg_value_=MA_INW(pll0_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(pll0_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll0_out_SOURCE_ext_clk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll0_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(pll0_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll0_out_SOURCE_ext_clk1 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll0_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(pll0_out_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock pll0_ref_clk 
********************************************************************************/

#define SWITCH_pll0_ref_clk_SOURCE_pad_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(pll0_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(pll0_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll0_ref_clk_SOURCE_ext_clk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll0_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(pll0_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll0_ref_clk_SOURCE_ext_clk1 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll0_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(pll0_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock pll1_out 
********************************************************************************/

#define ENABLE_CLOCK_pll1_out { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_out_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(pll1_out_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_pll1_out { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_out_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(pll1_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll1_out_SOURCE_pll1_out_wire { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(pll1_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll1_out_SOURCE_ext_clk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(pll1_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll1_out_SOURCE_ext_clk1 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(pll1_out_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock pll1_ref_clk 
********************************************************************************/

#define SWITCH_pll1_ref_clk_SOURCE_pad_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(pll1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll1_ref_clk_SOURCE_ext_clk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(pll1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll1_ref_clk_SOURCE_ext_clk1 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(pll1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll1_ref_clk_SOURCE_pll1_ref_clk_pre { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(pll1_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock pll1_ref_clk_pre 
********************************************************************************/

#define DIVIDE_pll1_ref_clk_pre(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(pll1_ref_clk_pre_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(pll1_ref_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock pll2_out 
********************************************************************************/

#define ENABLE_CLOCK_pll2_out { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_out_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(pll2_out_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_pll2_out { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_out_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(pll2_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll2_out_SOURCE_pll2_out_wire { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(pll2_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll2_out_SOURCE_ext_clk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(pll2_out_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll2_out_SOURCE_ext_clk1 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_out_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(pll2_out_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock pll2_ref_clk 
********************************************************************************/

#define SWITCH_pll2_ref_clk_SOURCE_pad_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(pll2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll2_ref_clk_SOURCE_ext_clk0 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(pll2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll2_ref_clk_SOURCE_ext_clk1 { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(pll2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_pll2_ref_clk_SOURCE_pll2_ref_clk_pre { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(pll2_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock pll2_ref_clk_pre 
********************************************************************************/

#define DIVIDE_pll2_ref_clk_pre(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(pll2_ref_clk_pre_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(pll2_ref_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock skt_aclk 
********************************************************************************/

#define ENABLE_CLOCK_skt_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(skt_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(skt_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_skt_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(skt_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(skt_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock skt_clk 
********************************************************************************/

#define ENABLE_CLOCK_skt_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(skt_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(skt_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_skt_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(skt_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(skt_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock skt_pclk 
********************************************************************************/

#define ENABLE_CLOCK_skt_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(skt_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(skt_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_skt_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(skt_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(skt_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock slow_bus_clk 
********************************************************************************/

#define SWITCH_slow_bus_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_bus_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(slow_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_slow_bus_clk_SOURCE_slow_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_bus_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(slow_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock slow_clk 
********************************************************************************/

#define ENABLE_CLOCK_slow_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(slow_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_slow_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(slow_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_slow_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(slow_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_slow_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(slow_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_slow_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(slow_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_slow_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(slow_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_slow_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(slow_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(slow_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ahb2apb_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ahb2apb_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ahb2apb_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ahb2apb_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ahb2apb_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ahb2apb_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ahb2apb_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ahb_ram_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ahb_ram_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ahb_ram_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ahb_ram_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ahb_ram_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ahb_ram_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ahb_ram_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ahb_rom_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ahb_rom_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ahb_rom_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ahb_rom_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ahb_rom_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ahb_rom_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ahb_rom_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_cfg_bus_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_cfg_bus_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cfg_bus_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cfg_bus_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_cfg_bus_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cfg_bus_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cfg_bus_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_clkgen_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_clkgen_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_clkgen_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_clkgen_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_clkgen_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_clkgen_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_clkgen_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_cmif_mclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_cmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_mclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_cmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_mclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define POLARITY_NEG_CLOCK_u0_cmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_mclk_CLOCK_REG); \
POLARITY_NEG_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define POLARITY_POS_CLOCK_u0_cmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_mclk_CLOCK_REG); \
POLARITY_POS_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_cmif_mclk_SOURCE_main_root_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_mclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_cmif_mclk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_mclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_cmif_mclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_mclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_cmif_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_cmif_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cmif_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_cmif_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cmif_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cmif_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_cqspi_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_cqspi_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cqspi_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_cqspi_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cqspi_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_cqspi_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_cqspi_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cqspi_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_cqspi_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cqspi_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_cqspi_ref_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_cqspi_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_ref_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cqspi_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_cqspi_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_ref_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_cqspi_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_cqspi_ref_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_cqspi_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_cqspi_ref_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_cqspi_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_cqspi_ref_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u0_cqspi_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_cqspi_ref_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_cqspi_ref_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_cqspi_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_csi_ipi_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_csi_ipi_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_csi_ipi_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_csi_ipi_clk_SOURCE_pll4_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_csi_ipi_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_csi_ipi_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u0_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_csi_ipi_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u0_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_csi_ipi_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_csi_ipi_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ddr_aclk_0 
********************************************************************************/

#define ENABLE_CLOCK_u0_ddr_aclk_0 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_aclk_0_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_aclk_0_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ddr_aclk_0 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_aclk_0_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_aclk_0_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ddr_aclk_1 
********************************************************************************/

#define ENABLE_CLOCK_u0_ddr_aclk_1 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_aclk_1_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_aclk_1_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ddr_aclk_1 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_aclk_1_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_aclk_1_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ddr_core_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ddr_core_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_core_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ddr_core_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_core_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_ddr_core_clk_SOURCE_pll5_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_core_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_ddr_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_ddr_core_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_core_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_ddr_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_ddr_core_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_core_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u0_ddr_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_ddr_core_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_core_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u0_ddr_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_ddr_core_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_core_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_ddr_core_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ddr_ctl_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ddr_ctl_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_ctl_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_ctl_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ddr_ctl_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_ctl_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_ctl_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ddr_phy_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ddr_phy_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_phy_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_phy_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ddr_phy_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ddr_phy_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ddr_phy_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_dmac_aclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_dmac_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dmac_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dmac_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_dmac_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dmac_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dmac_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_dmac_mst_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_dmac_mst_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dmac_mst_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dmac_mst_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_dmac_mst_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dmac_mst_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dmac_mst_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_dmac_slv_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_dmac_slv_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dmac_slv_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dmac_slv_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_dmac_slv_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dmac_slv_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dmac_slv_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_dsp_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_dsp_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dsp_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_dsp_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dsp_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_dsp_clk_SOURCE_pll3_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dsp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_dsp_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dsp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_dsp_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dsp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u0_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_dsp_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dsp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u0_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_dsp_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dsp_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_dw_i2s_gclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_dw_i2s_gclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dw_i2s_gclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dw_i2s_gclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_dw_i2s_gclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dw_i2s_gclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dw_i2s_gclk_CLOCK_REG,_clk_gen_reg_value_); }

#define POLARITY_NEG_CLOCK_u0_dw_i2s_gclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dw_i2s_gclk_CLOCK_REG); \
POLARITY_NEG_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dw_i2s_gclk_CLOCK_REG,_clk_gen_reg_value_); }

#define POLARITY_POS_CLOCK_u0_dw_i2s_gclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dw_i2s_gclk_CLOCK_REG); \
POLARITY_POS_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dw_i2s_gclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_dw_i2s_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_dw_i2s_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dw_i2s_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dw_i2s_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_dw_i2s_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_dw_i2s_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_dw_i2s_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_fast_bus_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_fast_bus_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_fast_bus_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_fast_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_fast_bus_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_fast_bus_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_fast_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_gpio_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_gpio_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_gpio_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_gpio_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_gpio_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_gpio_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_gpio_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_gpio_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_gpio_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_gpio_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_gpio_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_gpio_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_gpio_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_gpio_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_hs_device_utmi_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_hs_device_utmi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_hs_device_utmi_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_hs_device_utmi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_hs_device_utmi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_hs_device_utmi_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_hs_device_utmi_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_i2c_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_i2c_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_i2c_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_i2c_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_i2c_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_i2c_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_i2c_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_i2c_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_i2c_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_i2c_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_i2c_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_i2c_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_i2c_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_i2c_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_i2c_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_i2c_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_i2c_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_i2c_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ircmif_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ircmif_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ircmif_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ircmif_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ircmif_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ircmif_mclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ircmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_mclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ircmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ircmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_mclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ircmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define POLARITY_NEG_CLOCK_u0_ircmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_mclk_CLOCK_REG); \
POLARITY_NEG_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ircmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define POLARITY_POS_CLOCK_u0_ircmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_mclk_CLOCK_REG); \
POLARITY_POS_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ircmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_ircmif_mclk_SOURCE_main_root_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_mclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_ircmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_ircmif_mclk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_mclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_ircmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_ircmif_mclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ircmif_mclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_ircmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_isp_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_isp_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_isp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_isp_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_isp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_isp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_isp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u0_isp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u0_isp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_isp_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_isp_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_isp_jpeg_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_isp_jpeg_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_jpeg_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_isp_jpeg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_isp_jpeg_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_jpeg_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_isp_jpeg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_jpeg_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_jpeg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_isp_jpeg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_jpeg_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_jpeg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_isp_jpeg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_jpeg_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_jpeg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u0_isp_jpeg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_jpeg_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_jpeg_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u0_isp_jpeg_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_isp_jpeg_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_jpeg_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_isp_jpeg_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_isp_sclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_isp_sclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_sclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_isp_sclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_isp_sclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_sclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_isp_sclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_sclk_SOURCE_u0_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_sclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_isp_sclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_sclk_SOURCE_u1_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_sclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_isp_sclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_sclk_SOURCE_u2_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_sclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u0_isp_sclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_isp_sclk_SOURCE_pad_pixclk2 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_isp_sclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u0_isp_sclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_mst_bus_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_mst_bus_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_mst_bus_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_mst_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_mst_bus_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_mst_bus_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_mst_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_ocram_aclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_ocram_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ocram_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ocram_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_ocram_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_ocram_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_ocram_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_pmu_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_pmu_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pmu_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_pmu_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_pmu_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pmu_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_pmu_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_pwm_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_pwm_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pwm_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_pwm_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pwm_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_pwm_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pwm_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_pwm_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pwm_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_pwm_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pwm_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_pwm_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_pwm_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pwm_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_pwm_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_pwm_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_pwm_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_pwm_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_rstgen_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_rstgen_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_rstgen_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_rstgen_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_rstgen_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_rstgen_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_rstgen_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_sdio_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_sdio_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sdio_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_sdio_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_sdio_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sdio_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_sdio_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_sdio_logic_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_sdio_logic_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sdio_logic_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_sdio_logic_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_sdio_logic_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sdio_logic_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_sdio_logic_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_sdio_logic_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sdio_logic_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_sdio_logic_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_sdio_logic_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sdio_logic_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_sdio_logic_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_sdio_logic_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sdio_logic_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u0_sdio_logic_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_sdio_logic_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sdio_logic_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_sdio_logic_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_slow_bus_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_slow_bus_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_slow_bus_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_slow_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_slow_bus_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_slow_bus_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_slow_bus_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_spi_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_spi_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_spi_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_spi_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_spi_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_spi_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_spi_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_spi_sspclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_spi_sspclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_spi_sspclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_spi_sspclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_spi_sspclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_spi_sspclk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_spi_sspclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_spi_sspclk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_spi_sspclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_spi_sspclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_spi_sspclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_sysbus_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_sysbus_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sysbus_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_sysbus_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_sysbus_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sysbus_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_sysbus_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_sysbus_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_sysbus_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sysbus_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_sysbus_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_sysbus_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_sysbus_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_sysbus_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_syscon_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_syscon_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_syscon_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_syscon_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_syscon_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_syscon_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_syscon_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer64_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer64_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer64_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer64_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer64_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer64_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer64_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_timer64_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer64_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_timer64_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_timer64_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer64_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_timer64_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_timer64_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer64_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_timer64_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer64_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer64_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer64_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer64_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer64_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer64_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer64_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_timer0_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_timer0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer0_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer0_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_timer0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer0_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer0_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_timer1_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_timer1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer1_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer1_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_timer1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer1_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer1_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_timer2_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_timer2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer2_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer2_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_timer2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer2_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer2_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_timer3_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_timer3_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer3_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer3_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_timer3_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer3_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer3_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_timer4_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_timer4_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer4_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer4_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_timer4_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer4_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer4_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_timer5_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_timer5_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer5_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer5_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_timer5_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer5_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer5_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_timer6_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_timer6_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer6_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer6_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_timer6_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_timer6_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_timer6_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_timer_wdog_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_timer_wdog_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_wdog_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_wdog_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_timer_wdog_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_timer_wdog_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_timer_wdog_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_uart_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_uart_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_uart_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_uart_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_uart_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_uart_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_uart_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_uart_uartclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_uart_uartclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_uart_uartclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_uart_uartclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_uart_uartclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_uart_uartclk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_uart_uartclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_uart_uartclk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_uart_uartclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_uart_uartclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_uart_uartclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_usb3_aclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_usb3_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb3_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_usb3_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb3_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_usb3_macro_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_usb3_macro_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_macro_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb3_macro_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_usb3_macro_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_macro_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb3_macro_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_usb3_ref_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_usb3_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_ref_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb3_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_usb3_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_ref_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb3_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_usb3_ref_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_usb3_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_usb3_ref_clk_SOURCE_u0_usb3_ref_clk_pre { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_usb3_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_usb3_ref_clk_pre 
********************************************************************************/

#define DIVIDE_u0_usb3_ref_clk_pre(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_ref_clk_pre_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_usb3_ref_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_usb3_suspend_clk 
********************************************************************************/

#define ENABLE_CLOCK_u0_usb3_suspend_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_suspend_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb3_suspend_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_usb3_suspend_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_suspend_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb3_suspend_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_usb3_suspend_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_suspend_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u0_usb3_suspend_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u0_usb3_suspend_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_suspend_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u0_usb3_suspend_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u0_usb3_suspend_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb3_suspend_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u0_usb3_suspend_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_usb_dev_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_usb_dev_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb_dev_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb_dev_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_usb_dev_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_usb_dev_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_usb_dev_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_vi_domain_aclk_0 
********************************************************************************/

#define ENABLE_CLOCK_u0_vi_domain_aclk_0 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_aclk_0_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_aclk_0_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_vi_domain_aclk_0 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_aclk_0_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_aclk_0_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_vi_domain_aclk_1 
********************************************************************************/

#define ENABLE_CLOCK_u0_vi_domain_aclk_1 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_aclk_1_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_aclk_1_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_vi_domain_aclk_1 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_aclk_1_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_aclk_1_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_vi_domain_pclk_0 
********************************************************************************/

#define ENABLE_CLOCK_u0_vi_domain_pclk_0 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_0_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_0_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_vi_domain_pclk_0 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_0_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_0_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_vi_domain_pclk_1 
********************************************************************************/

#define ENABLE_CLOCK_u0_vi_domain_pclk_1 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_1_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_1_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_vi_domain_pclk_1 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_1_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_1_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_vi_domain_pclk_2 
********************************************************************************/

#define ENABLE_CLOCK_u0_vi_domain_pclk_2 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_2_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_2_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_vi_domain_pclk_2 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_2_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_2_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_vi_domain_pclk_3 
********************************************************************************/

#define ENABLE_CLOCK_u0_vi_domain_pclk_3 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_3_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_3_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_vi_domain_pclk_3 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_3_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_3_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_vi_domain_pclk_4 
********************************************************************************/

#define ENABLE_CLOCK_u0_vi_domain_pclk_4 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_4_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_4_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_vi_domain_pclk_4 { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vi_domain_pclk_4_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vi_domain_pclk_4_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u0_vic_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u0_vic_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vic_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vic_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u0_vic_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u0_vic_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u0_vic_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_ahb2apb_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_ahb2apb_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_ahb2apb_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_ahb2apb_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_ahb2apb_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_ahb2apb_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_ahb2apb_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_cmif_mclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_cmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_cmif_mclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_cmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_cmif_mclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define POLARITY_NEG_CLOCK_u1_cmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_cmif_mclk_CLOCK_REG); \
POLARITY_NEG_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define POLARITY_POS_CLOCK_u1_cmif_mclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_cmif_mclk_CLOCK_REG); \
POLARITY_POS_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_cmif_mclk_SOURCE_main_root_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_cmif_mclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u1_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_cmif_mclk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_cmif_mclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u1_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u1_cmif_mclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_cmif_mclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u1_cmif_mclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_csi_ipi_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_csi_ipi_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_csi_ipi_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_csi_ipi_clk_SOURCE_pll4_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u1_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_csi_ipi_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u1_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_csi_ipi_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u1_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_csi_ipi_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u1_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u1_csi_ipi_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_csi_ipi_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u1_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_dsp_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_dsp_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_dsp_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_dsp_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_dsp_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_dsp_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_dsp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u1_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_dsp_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_dsp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u1_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_dsp_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_dsp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u1_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_dsp_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_dsp_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u1_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u1_dsp_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_dsp_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u1_dsp_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_gpio_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_gpio_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_gpio_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_gpio_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_gpio_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_gpio_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_gpio_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_gpio_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_gpio_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_gpio_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_gpio_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_gpio_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_gpio_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_gpio_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_i2c_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_i2c_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_i2c_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_i2c_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_i2c_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_i2c_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_i2c_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u1_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_i2c_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_i2c_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u1_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u1_i2c_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_i2c_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u1_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_i2c_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_i2c_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_i2c_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_i2c_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_i2c_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_i2c_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_i2c_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_pwm_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_pwm_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_pwm_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_pwm_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_pwm_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_pwm_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_pwm_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u1_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_pwm_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_pwm_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u1_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u1_pwm_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_pwm_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u1_pwm_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_pwm_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_pwm_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_pwm_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_pwm_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_pwm_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_pwm_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_pwm_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_spi_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_spi_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_spi_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_spi_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_spi_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_spi_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_spi_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_spi_sspclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_spi_sspclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_spi_sspclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_spi_sspclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_spi_sspclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_spi_sspclk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_spi_sspclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u1_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_spi_sspclk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_spi_sspclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u1_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u1_spi_sspclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_spi_sspclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u1_spi_sspclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer64_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer64_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer64_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer64_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer64_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer64_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer64_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_timer0_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_timer0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer0_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer0_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_timer0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer0_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer0_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_timer1_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_timer1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer1_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer1_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_timer1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer1_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer1_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_timer2_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_timer2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer2_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer2_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_timer2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer2_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer2_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_timer3_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_timer3_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer3_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer3_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_timer3_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer3_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer3_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_timer4_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_timer4_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer4_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer4_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_timer4_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer4_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer4_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_timer5_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_timer5_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer5_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer5_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_timer5_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer5_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer5_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_timer6_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_timer6_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer6_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer6_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_timer6_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_timer6_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_timer6_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_timer_wdog_clk 
********************************************************************************/

#define ENABLE_CLOCK_u1_timer_wdog_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_wdog_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_wdog_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_timer_wdog_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_timer_wdog_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_timer_wdog_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_uart_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_uart_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_uart_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_uart_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_uart_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_uart_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_uart_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_uart_uartclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_uart_uartclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_uart_uartclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_uart_uartclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_uart_uartclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_uart_uartclk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_uart_uartclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u1_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u1_uart_uartclk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_uart_uartclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u1_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u1_uart_uartclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_uart_uartclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u1_uart_uartclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u1_vic_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u1_vic_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_vic_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_vic_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u1_vic_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u1_vic_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u1_vic_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u2_ahb2apb_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u2_ahb2apb_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_ahb2apb_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_ahb2apb_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u2_ahb2apb_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_ahb2apb_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_ahb2apb_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u2_csi_ipi_clk 
********************************************************************************/

#define ENABLE_CLOCK_u2_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_csi_ipi_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u2_csi_ipi_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_csi_ipi_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u2_csi_ipi_clk_SOURCE_pll4_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u2_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u2_csi_ipi_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u2_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u2_csi_ipi_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(u2_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u2_csi_ipi_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_csi_ipi_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,3); \
MA_OUTW(u2_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u2_csi_ipi_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_csi_ipi_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u2_csi_ipi_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u2_gpio_clk 
********************************************************************************/

#define ENABLE_CLOCK_u2_gpio_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_gpio_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_gpio_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u2_gpio_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_gpio_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_gpio_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u2_gpio_pclk 
********************************************************************************/

#define ENABLE_CLOCK_u2_gpio_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_gpio_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_gpio_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u2_gpio_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_gpio_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_gpio_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u2_i2c_clk 
********************************************************************************/

#define ENABLE_CLOCK_u2_i2c_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_i2c_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u2_i2c_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_i2c_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u2_i2c_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_i2c_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(u2_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_u2_i2c_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_i2c_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(u2_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_u2_i2c_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_i2c_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(u2_i2c_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u2_vic_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u2_vic_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_vic_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_vic_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u2_vic_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u2_vic_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u2_vic_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u3_ahb2apb_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u3_ahb2apb_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u3_ahb2apb_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u3_ahb2apb_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u3_ahb2apb_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u3_ahb2apb_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u3_ahb2apb_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock u3_vic_hclk 
********************************************************************************/

#define ENABLE_CLOCK_u3_vic_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u3_vic_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u3_vic_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_u3_vic_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(u3_vic_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(u3_vic_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock usb3_phy_ref_clk 
********************************************************************************/

#define ENABLE_CLOCK_usb3_phy_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(usb3_phy_ref_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(usb3_phy_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_usb3_phy_ref_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(usb3_phy_ref_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(usb3_phy_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_usb3_phy_ref_clk_SOURCE_osc_clock { \
uint32_t _clk_gen_reg_value_=MA_INW(usb3_phy_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(usb3_phy_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_usb3_phy_ref_clk_SOURCE_usb3_phy_ref_clk_pre { \
uint32_t _clk_gen_reg_value_=MA_INW(usb3_phy_ref_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(usb3_phy_ref_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock usb3_phy_ref_clk_pre 
********************************************************************************/

#define DIVIDE_usb3_phy_ref_clk_pre(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(usb3_phy_ref_clk_pre_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(usb3_phy_ref_clk_pre_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vip8000_aclk 
********************************************************************************/

#define ENABLE_CLOCK_vip8000_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vip8000_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vip8000_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vip8000_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vip8000_core_clk 
********************************************************************************/

#define ENABLE_CLOCK_vip8000_core_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_core_clk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vip8000_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vip8000_core_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_core_clk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vip8000_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vip8000_core_clk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_core_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(vip8000_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vip8000_core_clk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_core_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(vip8000_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vip8000_core_clk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_core_clk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(vip8000_core_clk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_vip8000_core_clk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_core_clk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(vip8000_core_clk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vip8000_hclk 
********************************************************************************/

#define ENABLE_CLOCK_vip8000_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_hclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vip8000_hclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vip8000_hclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vip8000_hclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vip8000_hclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vpu_aclk 
********************************************************************************/

#define ENABLE_CLOCK_vpu_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vpu_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vpu_cclk 
********************************************************************************/

#define ENABLE_CLOCK_vpu_cclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_cclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vpu_cclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_cclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vpu_cclk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_cclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(vpu_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vpu_cclk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_cclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(vpu_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vpu_cclk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_cclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(vpu_cclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_vpu_cclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_cclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(vpu_cclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vpu_fpe_aclk 
********************************************************************************/

#define ENABLE_CLOCK_vpu_fpe_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_fpe_aclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_fpe_aclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vpu_fpe_aclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_fpe_aclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_fpe_aclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vpu_fpe_pclk 
********************************************************************************/

#define ENABLE_CLOCK_vpu_fpe_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_fpe_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_fpe_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vpu_fpe_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_fpe_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_fpe_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vpu_pclk 
********************************************************************************/

#define ENABLE_CLOCK_vpu_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_pclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_pclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vpu_pclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_pclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_pclk_CLOCK_REG,_clk_gen_reg_value_); }


/********************************************************************************
* clock vpu_rclk 
********************************************************************************/

#define ENABLE_CLOCK_vpu_rclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_rclk_CLOCK_REG); \
ENABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_rclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DISABLE_CLOCK_vpu_rclk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_rclk_CLOCK_REG); \
DISABLE_CLOCK(_clk_gen_reg_value_); \
MA_OUTW(vpu_rclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vpu_rclk_SOURCE_pll0_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_rclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,0); \
MA_OUTW(vpu_rclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vpu_rclk_SOURCE_pll1_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_rclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,1); \
MA_OUTW(vpu_rclk_CLOCK_REG,_clk_gen_reg_value_); }

#define SWITCH_vpu_rclk_SOURCE_pll2_clk { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_rclk_CLOCK_REG); \
CLOCK_SELECT(_clk_gen_reg_value_,2); \
MA_OUTW(vpu_rclk_CLOCK_REG,_clk_gen_reg_value_); }

#define DIVIDE_vpu_rclk(div) { \
uint32_t _clk_gen_reg_value_=MA_INW(vpu_rclk_CLOCK_REG); \
CLOCK_DIVIDE(_clk_gen_reg_value_,div); \
MA_OUTW(vpu_rclk_CLOCK_REG,_clk_gen_reg_value_); }

#endif //CLK_GEN_MACRO_H
