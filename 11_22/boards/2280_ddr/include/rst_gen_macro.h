#ifndef RST_GEN_MACRO_H
#define RST_GEN_MACRO_H

//#define RST_GEN_BASE_ADDR 0x0
#include	"hellboy_map.h"

#define ACCESS_BIT(addr,n) { \
uint32_t i; \
uint32_t value=((uint32_t)1<<n); \
MA_OUTW(addr,value); \
for(i = 0; i < 10; i++); \
value=0; \
MA_OUTW(addr,value); \
}

#define GET_BIT(addr,n) ((MA_INW(addr)>>n)&0x1)

#define RESET_ASSERT_REG00 RST_GEN_BASE_ADDR+0x00
#define RESET_ASSERT_REG01 RST_GEN_BASE_ADDR+0x04
#define RESET_ASSERT_REG02 RST_GEN_BASE_ADDR+0x08
#define RESET_ASSERT_REG03 RST_GEN_BASE_ADDR+0x0c
#define RESET_ASSERT_REG04 RST_GEN_BASE_ADDR+0x10
#define RESET_CLEAR_REG00 RST_GEN_BASE_ADDR+0x100+0x00
#define RESET_CLEAR_REG01 RST_GEN_BASE_ADDR+0x100+0x04
#define RESET_CLEAR_REG02 RST_GEN_BASE_ADDR+0x100+0x08
#define RESET_CLEAR_REG03 RST_GEN_BASE_ADDR+0x100+0x0c
#define RESET_CLEAR_REG04 RST_GEN_BASE_ADDR+0x100+0x10
#define RESET_STATUS_REG00 RST_GEN_BASE_ADDR+0x200+0x00
#define RESET_STATUS_REG01 RST_GEN_BASE_ADDR+0x200+0x04
#define RESET_STATUS_REG02 RST_GEN_BASE_ADDR+0x200+0x08
#define RESET_STATUS_REG03 RST_GEN_BASE_ADDR+0x200+0x0c
#define RESET_STATUS_REG04 RST_GEN_BASE_ADDR+0x200+0x10

/********************************************************
* reset adc_presetn
*********************************************************/
#define ASSERT_RESET_adc_presetn ACCESS_BIT(RESET_ASSERT_REG00,0)
#define CLEAR_RESET_adc_presetn ACCESS_BIT(RESET_CLEAR_REG00,0)
#define RESET_ASSERTED_adc_presetn (GET_BIT(RESET_STATUS_REG00,0)==0)

/********************************************************
* reset cif_rstn
*********************************************************/
#define ASSERT_RESET_cif_rstn ACCESS_BIT(RESET_ASSERT_REG00,1)
#define CLEAR_RESET_cif_rstn ACCESS_BIT(RESET_CLEAR_REG00,1)
#define RESET_ASSERTED_cif_rstn (GET_BIT(RESET_STATUS_REG00,1)==0)

/********************************************************
* reset cpu_rstn
*********************************************************/
#define ASSERT_RESET_cpu_rstn ACCESS_BIT(RESET_ASSERT_REG00,2)
#define CLEAR_RESET_cpu_rstn ACCESS_BIT(RESET_CLEAR_REG00,2)
#define RESET_ASSERTED_cpu_rstn (GET_BIT(RESET_STATUS_REG00,2)==0)

/********************************************************
* reset cpu_rstn_a7_0
*********************************************************/
#define ASSERT_RESET_cpu_rstn_a7_0 ACCESS_BIT(RESET_ASSERT_REG00,3)
#define CLEAR_RESET_cpu_rstn_a7_0 ACCESS_BIT(RESET_CLEAR_REG00,3)
#define RESET_ASSERTED_cpu_rstn_a7_0 (GET_BIT(RESET_STATUS_REG00,3)==0)

/********************************************************
* reset cpu_rstn_a7_1
*********************************************************/
#define ASSERT_RESET_cpu_rstn_a7_1 ACCESS_BIT(RESET_ASSERT_REG00,4)
#define CLEAR_RESET_cpu_rstn_a7_1 ACCESS_BIT(RESET_CLEAR_REG00,4)
#define RESET_ASSERTED_cpu_rstn_a7_1 (GET_BIT(RESET_STATUS_REG00,4)==0)

/********************************************************
* reset dpu_pix_rstn_0
*********************************************************/
#define ASSERT_RESET_dpu_pix_rstn_0 ACCESS_BIT(RESET_ASSERT_REG00,5)
#define CLEAR_RESET_dpu_pix_rstn_0 ACCESS_BIT(RESET_CLEAR_REG00,5)
#define RESET_ASSERTED_dpu_pix_rstn_0 (GET_BIT(RESET_STATUS_REG00,5)==0)

/********************************************************
* reset dpu_pix_rstn_1
*********************************************************/
#define ASSERT_RESET_dpu_pix_rstn_1 ACCESS_BIT(RESET_ASSERT_REG00,6)
#define CLEAR_RESET_dpu_pix_rstn_1 ACCESS_BIT(RESET_CLEAR_REG00,6)
#define RESET_ASSERTED_dpu_pix_rstn_1 (GET_BIT(RESET_STATUS_REG00,6)==0)

/********************************************************
* reset dpu_pix_rstn_2
*********************************************************/
#define ASSERT_RESET_dpu_pix_rstn_2 ACCESS_BIT(RESET_ASSERT_REG00,7)
#define CLEAR_RESET_dpu_pix_rstn_2 ACCESS_BIT(RESET_CLEAR_REG00,7)
#define RESET_ASSERTED_dpu_pix_rstn_2 (GET_BIT(RESET_STATUS_REG00,7)==0)

/********************************************************
* reset dsp0_intc_presetn
*********************************************************/
#define ASSERT_RESET_dsp0_intc_presetn ACCESS_BIT(RESET_ASSERT_REG00,8)
#define CLEAR_RESET_dsp0_intc_presetn ACCESS_BIT(RESET_CLEAR_REG00,8)
#define RESET_ASSERTED_dsp0_intc_presetn (GET_BIT(RESET_STATUS_REG00,8)==0)

/********************************************************
* reset dsp1_intc_presetn
*********************************************************/
#define ASSERT_RESET_dsp1_intc_presetn ACCESS_BIT(RESET_ASSERT_REG00,9)
#define CLEAR_RESET_dsp1_intc_presetn ACCESS_BIT(RESET_CLEAR_REG00,9)
#define RESET_ASSERTED_dsp1_intc_presetn (GET_BIT(RESET_STATUS_REG00,9)==0)

/********************************************************
* reset fpe_aresetn
*********************************************************/
#define ASSERT_RESET_fpe_aresetn ACCESS_BIT(RESET_ASSERT_REG00,10)
#define CLEAR_RESET_fpe_aresetn ACCESS_BIT(RESET_CLEAR_REG00,10)
#define RESET_ASSERTED_fpe_aresetn (GET_BIT(RESET_STATUS_REG00,10)==0)

/********************************************************
* reset fpe_cresetn
*********************************************************/
#define ASSERT_RESET_fpe_cresetn ACCESS_BIT(RESET_ASSERT_REG00,11)
#define CLEAR_RESET_fpe_cresetn ACCESS_BIT(RESET_CLEAR_REG00,11)
#define RESET_ASSERTED_fpe_cresetn (GET_BIT(RESET_STATUS_REG00,11)==0)

/********************************************************
* reset fpe_presetn
*********************************************************/
#define ASSERT_RESET_fpe_presetn ACCESS_BIT(RESET_ASSERT_REG00,12)
#define CLEAR_RESET_fpe_presetn ACCESS_BIT(RESET_CLEAR_REG00,12)
#define RESET_ASSERTED_fpe_presetn (GET_BIT(RESET_STATUS_REG00,12)==0)

/********************************************************
* reset mbox0_presetn
*********************************************************/
#define ASSERT_RESET_mbox0_presetn ACCESS_BIT(RESET_ASSERT_REG00,13)
#define CLEAR_RESET_mbox0_presetn ACCESS_BIT(RESET_CLEAR_REG00,13)
#define RESET_ASSERTED_mbox0_presetn (GET_BIT(RESET_STATUS_REG00,13)==0)

/********************************************************
* reset mbox1_presetn
*********************************************************/
#define ASSERT_RESET_mbox1_presetn ACCESS_BIT(RESET_ASSERT_REG00,14)
#define CLEAR_RESET_mbox1_presetn ACCESS_BIT(RESET_CLEAR_REG00,14)
#define RESET_ASSERTED_mbox1_presetn (GET_BIT(RESET_STATUS_REG00,14)==0)

/********************************************************
* reset mbox2_presetn
*********************************************************/
#define ASSERT_RESET_mbox2_presetn ACCESS_BIT(RESET_ASSERT_REG00,15)
#define CLEAR_RESET_mbox2_presetn ACCESS_BIT(RESET_CLEAR_REG00,15)
#define RESET_ASSERTED_mbox2_presetn (GET_BIT(RESET_STATUS_REG00,15)==0)

/********************************************************
* reset musbhsfc_NRST
*********************************************************/
#define ASSERT_RESET_musbhsfc_NRST ACCESS_BIT(RESET_ASSERT_REG00,16)
#define CLEAR_RESET_musbhsfc_NRST ACCESS_BIT(RESET_CLEAR_REG00,16)
#define RESET_ASSERTED_musbhsfc_NRST (GET_BIT(RESET_STATUS_REG00,16)==0)

/********************************************************
* reset musbhsfc_NRSTA
*********************************************************/
#define ASSERT_RESET_musbhsfc_NRSTA ACCESS_BIT(RESET_ASSERT_REG00,17)
#define CLEAR_RESET_musbhsfc_NRSTA ACCESS_BIT(RESET_CLEAR_REG00,17)
#define RESET_ASSERTED_musbhsfc_NRSTA (GET_BIT(RESET_STATUS_REG00,17)==0)

/********************************************************
* reset musbhsfc_NRSTX
*********************************************************/
#define ASSERT_RESET_musbhsfc_NRSTX ACCESS_BIT(RESET_ASSERT_REG00,18)
#define CLEAR_RESET_musbhsfc_NRSTX ACCESS_BIT(RESET_CLEAR_REG00,18)
#define RESET_ASSERTED_musbhsfc_NRSTX (GET_BIT(RESET_STATUS_REG00,18)==0)

/********************************************************
* reset nj_top_aresetn
*********************************************************/
#define ASSERT_RESET_nj_top_aresetn ACCESS_BIT(RESET_ASSERT_REG00,19)
#define CLEAR_RESET_nj_top_aresetn ACCESS_BIT(RESET_CLEAR_REG00,19)
#define RESET_ASSERTED_nj_top_aresetn (GET_BIT(RESET_STATUS_REG00,19)==0)

/********************************************************
* reset nj_top_resetn_ahbsram0
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_ahbsram0 ACCESS_BIT(RESET_ASSERT_REG00,20)
#define CLEAR_RESET_nj_top_resetn_ahbsram0 ACCESS_BIT(RESET_CLEAR_REG00,20)
#define RESET_ASSERTED_nj_top_resetn_ahbsram0 (GET_BIT(RESET_STATUS_REG00,20)==0)

/********************************************************
* reset nj_top_resetn_ahbsram1
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_ahbsram1 ACCESS_BIT(RESET_ASSERT_REG00,21)
#define CLEAR_RESET_nj_top_resetn_ahbsram1 ACCESS_BIT(RESET_CLEAR_REG00,21)
#define RESET_ASSERTED_nj_top_resetn_ahbsram1 (GET_BIT(RESET_STATUS_REG00,21)==0)

/********************************************************
* reset nj_top_resetn_arm0
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_arm0 ACCESS_BIT(RESET_ASSERT_REG00,22)
#define CLEAR_RESET_nj_top_resetn_arm0 ACCESS_BIT(RESET_CLEAR_REG00,22)
#define RESET_ASSERTED_nj_top_resetn_arm0 (GET_BIT(RESET_STATUS_REG00,22)==0)

/********************************************************
* reset nj_top_resetn_arm1
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_arm1 ACCESS_BIT(RESET_ASSERT_REG00,23)
#define CLEAR_RESET_nj_top_resetn_arm1 ACCESS_BIT(RESET_CLEAR_REG00,23)
#define RESET_ASSERTED_nj_top_resetn_arm1 (GET_BIT(RESET_STATUS_REG00,23)==0)

/********************************************************
* reset nj_top_resetn_dpu_slv
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_dpu_slv ACCESS_BIT(RESET_ASSERT_REG00,24)
#define CLEAR_RESET_nj_top_resetn_dpu_slv ACCESS_BIT(RESET_CLEAR_REG00,24)
#define RESET_ASSERTED_nj_top_resetn_dpu_slv (GET_BIT(RESET_STATUS_REG00,24)==0)

/********************************************************
* reset nj_top_resetn_fast_bus_m
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_fast_bus_m ACCESS_BIT(RESET_ASSERT_REG00,25)
#define CLEAR_RESET_nj_top_resetn_fast_bus_m ACCESS_BIT(RESET_CLEAR_REG00,25)
#define RESET_ASSERTED_nj_top_resetn_fast_bus_m (GET_BIT(RESET_STATUS_REG00,25)==0)

/********************************************************
* reset nj_top_resetn_fast_bus_s
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_fast_bus_s ACCESS_BIT(RESET_ASSERT_REG00,26)
#define CLEAR_RESET_nj_top_resetn_fast_bus_s ACCESS_BIT(RESET_CLEAR_REG00,26)
#define RESET_ASSERTED_nj_top_resetn_fast_bus_s (GET_BIT(RESET_STATUS_REG00,26)==0)

/********************************************************
* reset nj_top_resetn_nj_apb
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_nj_apb ACCESS_BIT(RESET_ASSERT_REG00,27)
#define CLEAR_RESET_nj_top_resetn_nj_apb ACCESS_BIT(RESET_CLEAR_REG00,27)
#define RESET_ASSERTED_nj_top_resetn_nj_apb (GET_BIT(RESET_STATUS_REG00,27)==0)

/********************************************************
* reset nj_top_resetn_refsram
*********************************************************/
#define ASSERT_RESET_nj_top_resetn_refsram ACCESS_BIT(RESET_ASSERT_REG00,28)
#define CLEAR_RESET_nj_top_resetn_refsram ACCESS_BIT(RESET_CLEAR_REG00,28)
#define RESET_ASSERTED_nj_top_resetn_refsram (GET_BIT(RESET_STATUS_REG00,28)==0)

/********************************************************
* reset skt_aresetn
*********************************************************/
#define ASSERT_RESET_skt_aresetn ACCESS_BIT(RESET_ASSERT_REG00,29)
#define CLEAR_RESET_skt_aresetn ACCESS_BIT(RESET_CLEAR_REG00,29)
#define RESET_ASSERTED_skt_aresetn (GET_BIT(RESET_STATUS_REG00,29)==0)

/********************************************************
* reset skt_presetn
*********************************************************/
#define ASSERT_RESET_skt_presetn ACCESS_BIT(RESET_ASSERT_REG00,30)
#define CLEAR_RESET_skt_presetn ACCESS_BIT(RESET_CLEAR_REG00,30)
#define RESET_ASSERTED_skt_presetn (GET_BIT(RESET_STATUS_REG00,30)==0)

/********************************************************
* reset skt_resetn
*********************************************************/
#define ASSERT_RESET_skt_resetn ACCESS_BIT(RESET_ASSERT_REG00,31)
#define CLEAR_RESET_skt_resetn ACCESS_BIT(RESET_CLEAR_REG00,31)
#define RESET_ASSERTED_skt_resetn (GET_BIT(RESET_STATUS_REG00,31)==0)

/********************************************************
* reset u0_ahb2apb_hresetn
*********************************************************/
#define ASSERT_RESET_u0_ahb2apb_hresetn ACCESS_BIT(RESET_ASSERT_REG01,0)
#define CLEAR_RESET_u0_ahb2apb_hresetn ACCESS_BIT(RESET_CLEAR_REG01,0)
#define RESET_ASSERTED_u0_ahb2apb_hresetn (GET_BIT(RESET_STATUS_REG01,0)==0)

/********************************************************
* reset u0_cfg_bus_presetn
*********************************************************/
#define ASSERT_RESET_u0_cfg_bus_presetn ACCESS_BIT(RESET_ASSERT_REG01,1)
#define CLEAR_RESET_u0_cfg_bus_presetn ACCESS_BIT(RESET_CLEAR_REG01,1)
#define RESET_ASSERTED_u0_cfg_bus_presetn (GET_BIT(RESET_STATUS_REG01,1)==0)

/********************************************************
* reset u0_clkgen_presetn
*********************************************************/
#define ASSERT_RESET_u0_clkgen_presetn ACCESS_BIT(RESET_ASSERT_REG01,2)
#define CLEAR_RESET_u0_clkgen_presetn ACCESS_BIT(RESET_CLEAR_REG01,2)
#define RESET_ASSERTED_u0_clkgen_presetn (GET_BIT(RESET_STATUS_REG01,2)==0)

/********************************************************
* reset u0_cqspi_hresetn
*********************************************************/
#define ASSERT_RESET_u0_cqspi_hresetn ACCESS_BIT(RESET_ASSERT_REG01,3)
#define CLEAR_RESET_u0_cqspi_hresetn ACCESS_BIT(RESET_CLEAR_REG01,3)
#define RESET_ASSERTED_u0_cqspi_hresetn (GET_BIT(RESET_STATUS_REG01,3)==0)

/********************************************************
* reset u0_cqspi_presetn
*********************************************************/
#define ASSERT_RESET_u0_cqspi_presetn ACCESS_BIT(RESET_ASSERT_REG01,4)
#define CLEAR_RESET_u0_cqspi_presetn ACCESS_BIT(RESET_CLEAR_REG01,4)
#define RESET_ASSERTED_u0_cqspi_presetn (GET_BIT(RESET_STATUS_REG01,4)==0)

/********************************************************
* reset u0_cqspi_ref_rstn
*********************************************************/
#define ASSERT_RESET_u0_cqspi_ref_rstn ACCESS_BIT(RESET_ASSERT_REG01,5)
#define CLEAR_RESET_u0_cqspi_ref_rstn ACCESS_BIT(RESET_CLEAR_REG01,5)
#define RESET_ASSERTED_u0_cqspi_ref_rstn (GET_BIT(RESET_STATUS_REG01,5)==0)

/********************************************************
* reset u0_csi_ipi_rstn
*********************************************************/
#define ASSERT_RESET_u0_csi_ipi_rstn ACCESS_BIT(RESET_ASSERT_REG01,6)
#define CLEAR_RESET_u0_csi_ipi_rstn ACCESS_BIT(RESET_CLEAR_REG01,6)
#define RESET_ASSERTED_u0_csi_ipi_rstn (GET_BIT(RESET_STATUS_REG01,6)==0)

/********************************************************
* reset u0_ddr_aresetn_0
*********************************************************/
#define ASSERT_RESET_u0_ddr_aresetn_0 ACCESS_BIT(RESET_ASSERT_REG01,7)
#define CLEAR_RESET_u0_ddr_aresetn_0 ACCESS_BIT(RESET_CLEAR_REG01,7)
#define RESET_ASSERTED_u0_ddr_aresetn_0 (GET_BIT(RESET_STATUS_REG01,7)==0)

/********************************************************
* reset u0_ddr_aresetn_1
*********************************************************/
#define ASSERT_RESET_u0_ddr_aresetn_1 ACCESS_BIT(RESET_ASSERT_REG01,8)
#define CLEAR_RESET_u0_ddr_aresetn_1 ACCESS_BIT(RESET_CLEAR_REG01,8)
#define RESET_ASSERTED_u0_ddr_aresetn_1 (GET_BIT(RESET_STATUS_REG01,8)==0)

/********************************************************
* reset u0_ddr_ctl_presetn
*********************************************************/
#define ASSERT_RESET_u0_ddr_ctl_presetn ACCESS_BIT(RESET_ASSERT_REG01,9)
#define CLEAR_RESET_u0_ddr_ctl_presetn ACCESS_BIT(RESET_CLEAR_REG01,9)
#define RESET_ASSERTED_u0_ddr_ctl_presetn (GET_BIT(RESET_STATUS_REG01,9)==0)

/********************************************************
* reset u0_ddr_ctl_resetn
*********************************************************/
#define ASSERT_RESET_u0_ddr_ctl_resetn ACCESS_BIT(RESET_ASSERT_REG01,10)
#define CLEAR_RESET_u0_ddr_ctl_resetn ACCESS_BIT(RESET_CLEAR_REG01,10)
#define RESET_ASSERTED_u0_ddr_ctl_resetn (GET_BIT(RESET_STATUS_REG01,10)==0)

/********************************************************
* reset u0_ddr_phy_PwrOkIn
*********************************************************/
#define ASSERT_RESET_u0_ddr_phy_PwrOkIn ACCESS_BIT(RESET_ASSERT_REG01,11)
#define CLEAR_RESET_u0_ddr_phy_PwrOkIn ACCESS_BIT(RESET_CLEAR_REG01,11)
#define RESET_ASSERTED_u0_ddr_phy_PwrOkIn (GET_BIT(RESET_STATUS_REG01,11)==0)

/********************************************************
* reset u0_ddr_phy_Reset
*********************************************************/
#define ASSERT_RESET_u0_ddr_phy_Reset ACCESS_BIT(RESET_ASSERT_REG01,12)
#define CLEAR_RESET_u0_ddr_phy_Reset ACCESS_BIT(RESET_CLEAR_REG01,12)
#define RESET_ASSERTED_u0_ddr_phy_Reset (GET_BIT(RESET_STATUS_REG01,12)==1)

/********************************************************
* reset u0_ddr_phy_presetn
*********************************************************/
#define ASSERT_RESET_u0_ddr_phy_presetn ACCESS_BIT(RESET_ASSERT_REG01,13)
#define CLEAR_RESET_u0_ddr_phy_presetn ACCESS_BIT(RESET_CLEAR_REG01,13)
#define RESET_ASSERTED_u0_ddr_phy_presetn (GET_BIT(RESET_STATUS_REG01,13)==0)

/********************************************************
* reset u0_dmac_aresetn
*********************************************************/
#define ASSERT_RESET_u0_dmac_aresetn ACCESS_BIT(RESET_ASSERT_REG01,14)
#define CLEAR_RESET_u0_dmac_aresetn ACCESS_BIT(RESET_CLEAR_REG01,14)
#define RESET_ASSERTED_u0_dmac_aresetn (GET_BIT(RESET_STATUS_REG01,14)==0)

/********************************************************
* reset u0_dsp_core_rstn
*********************************************************/
#define ASSERT_RESET_u0_dsp_core_rstn ACCESS_BIT(RESET_ASSERT_REG01,15)
#define CLEAR_RESET_u0_dsp_core_rstn ACCESS_BIT(RESET_CLEAR_REG01,15)
#define RESET_ASSERTED_u0_dsp_core_rstn (GET_BIT(RESET_STATUS_REG01,15)==0)

/********************************************************
* reset u0_dsp_global_rstn
*********************************************************/
#define ASSERT_RESET_u0_dsp_global_rstn ACCESS_BIT(RESET_ASSERT_REG01,16)
#define CLEAR_RESET_u0_dsp_global_rstn ACCESS_BIT(RESET_CLEAR_REG01,16)
#define RESET_ASSERTED_u0_dsp_global_rstn (GET_BIT(RESET_STATUS_REG01,16)==0)

/********************************************************
* reset u0_dsp_ocem_rstn
*********************************************************/
#define ASSERT_RESET_u0_dsp_ocem_rstn ACCESS_BIT(RESET_ASSERT_REG01,17)
#define CLEAR_RESET_u0_dsp_ocem_rstn ACCESS_BIT(RESET_CLEAR_REG01,17)
#define RESET_ASSERTED_u0_dsp_ocem_rstn (GET_BIT(RESET_STATUS_REG01,17)==0)

/********************************************************
* reset u0_dsp_rstn
*********************************************************/
#define ASSERT_RESET_u0_dsp_rstn ACCESS_BIT(RESET_ASSERT_REG01,18)
#define CLEAR_RESET_u0_dsp_rstn ACCESS_BIT(RESET_CLEAR_REG01,18)
#define RESET_ASSERTED_u0_dsp_rstn (GET_BIT(RESET_STATUS_REG01,18)==0)

/********************************************************
* reset u0_dsp_sys_rstn
*********************************************************/
#define ASSERT_RESET_u0_dsp_sys_rstn ACCESS_BIT(RESET_ASSERT_REG01,19)
#define CLEAR_RESET_u0_dsp_sys_rstn ACCESS_BIT(RESET_CLEAR_REG01,19)
#define RESET_ASSERTED_u0_dsp_sys_rstn (GET_BIT(RESET_STATUS_REG01,19)==0)

/********************************************************
* reset u0_dw_i2s_gresetn
*********************************************************/
#define ASSERT_RESET_u0_dw_i2s_gresetn ACCESS_BIT(RESET_ASSERT_REG01,20)
#define CLEAR_RESET_u0_dw_i2s_gresetn ACCESS_BIT(RESET_CLEAR_REG01,20)
#define RESET_ASSERTED_u0_dw_i2s_gresetn (GET_BIT(RESET_STATUS_REG01,20)==0)

/********************************************************
* reset u0_dw_i2s_presetn
*********************************************************/
#define ASSERT_RESET_u0_dw_i2s_presetn ACCESS_BIT(RESET_ASSERT_REG01,21)
#define CLEAR_RESET_u0_dw_i2s_presetn ACCESS_BIT(RESET_CLEAR_REG01,21)
#define RESET_ASSERTED_u0_dw_i2s_presetn (GET_BIT(RESET_STATUS_REG01,21)==0)

/********************************************************
* reset u0_fast_bus_resetn
*********************************************************/
#define ASSERT_RESET_u0_fast_bus_resetn ACCESS_BIT(RESET_ASSERT_REG01,22)
#define CLEAR_RESET_u0_fast_bus_resetn ACCESS_BIT(RESET_CLEAR_REG01,22)
#define RESET_ASSERTED_u0_fast_bus_resetn (GET_BIT(RESET_STATUS_REG01,22)==0)

/********************************************************
* reset u0_gpio_gresetn
*********************************************************/
#define ASSERT_RESET_u0_gpio_gresetn ACCESS_BIT(RESET_ASSERT_REG01,23)
#define CLEAR_RESET_u0_gpio_gresetn ACCESS_BIT(RESET_CLEAR_REG01,23)
#define RESET_ASSERTED_u0_gpio_gresetn (GET_BIT(RESET_STATUS_REG01,23)==0)

/********************************************************
* reset u0_gpio_presetn
*********************************************************/
#define ASSERT_RESET_u0_gpio_presetn ACCESS_BIT(RESET_ASSERT_REG01,24)
#define CLEAR_RESET_u0_gpio_presetn ACCESS_BIT(RESET_CLEAR_REG01,24)
#define RESET_ASSERTED_u0_gpio_presetn (GET_BIT(RESET_STATUS_REG01,24)==0)

/********************************************************
* reset u0_i2c_presetn
*********************************************************/
#define ASSERT_RESET_u0_i2c_presetn ACCESS_BIT(RESET_ASSERT_REG01,25)
#define CLEAR_RESET_u0_i2c_presetn ACCESS_BIT(RESET_CLEAR_REG01,25)
#define RESET_ASSERTED_u0_i2c_presetn (GET_BIT(RESET_STATUS_REG01,25)==0)

/********************************************************
* reset u0_i2c_resetn
*********************************************************/
#define ASSERT_RESET_u0_i2c_resetn ACCESS_BIT(RESET_ASSERT_REG01,26)
#define CLEAR_RESET_u0_i2c_resetn ACCESS_BIT(RESET_CLEAR_REG01,26)
#define RESET_ASSERTED_u0_i2c_resetn (GET_BIT(RESET_STATUS_REG01,26)==0)

/********************************************************
* reset u0_isp_hresetn
*********************************************************/
#define ASSERT_RESET_u0_isp_hresetn ACCESS_BIT(RESET_ASSERT_REG01,27)
#define CLEAR_RESET_u0_isp_hresetn ACCESS_BIT(RESET_CLEAR_REG01,27)
#define RESET_ASSERTED_u0_isp_hresetn (GET_BIT(RESET_STATUS_REG01,27)==0)

/********************************************************
* reset u0_isp_jpeg_resetn
*********************************************************/
#define ASSERT_RESET_u0_isp_jpeg_resetn ACCESS_BIT(RESET_ASSERT_REG01,28)
#define CLEAR_RESET_u0_isp_jpeg_resetn ACCESS_BIT(RESET_CLEAR_REG01,28)
#define RESET_ASSERTED_u0_isp_jpeg_resetn (GET_BIT(RESET_STATUS_REG01,28)==0)

/********************************************************
* reset u0_isp_resetn
*********************************************************/
#define ASSERT_RESET_u0_isp_resetn ACCESS_BIT(RESET_ASSERT_REG01,29)
#define CLEAR_RESET_u0_isp_resetn ACCESS_BIT(RESET_CLEAR_REG01,29)
#define RESET_ASSERTED_u0_isp_resetn (GET_BIT(RESET_STATUS_REG01,29)==0)

/********************************************************
* reset u0_mst_bus_resetn
*********************************************************/
#define ASSERT_RESET_u0_mst_bus_resetn ACCESS_BIT(RESET_ASSERT_REG01,30)
#define CLEAR_RESET_u0_mst_bus_resetn ACCESS_BIT(RESET_CLEAR_REG01,30)
#define RESET_ASSERTED_u0_mst_bus_resetn (GET_BIT(RESET_STATUS_REG01,30)==0)

/********************************************************
* reset u0_ocram_aresetn
*********************************************************/
#define ASSERT_RESET_u0_ocram_aresetn ACCESS_BIT(RESET_ASSERT_REG01,31)
#define CLEAR_RESET_u0_ocram_aresetn ACCESS_BIT(RESET_CLEAR_REG01,31)
#define RESET_ASSERTED_u0_ocram_aresetn (GET_BIT(RESET_STATUS_REG01,31)==0)

/********************************************************
* reset u0_pmu_presetn
*********************************************************/
#define ASSERT_RESET_u0_pmu_presetn ACCESS_BIT(RESET_ASSERT_REG02,0)
#define CLEAR_RESET_u0_pmu_presetn ACCESS_BIT(RESET_CLEAR_REG02,0)
#define RESET_ASSERTED_u0_pmu_presetn (GET_BIT(RESET_STATUS_REG02,0)==0)

/********************************************************
* reset u0_pwm_presetn
*********************************************************/
#define ASSERT_RESET_u0_pwm_presetn ACCESS_BIT(RESET_ASSERT_REG02,1)
#define CLEAR_RESET_u0_pwm_presetn ACCESS_BIT(RESET_CLEAR_REG02,1)
#define RESET_ASSERTED_u0_pwm_presetn (GET_BIT(RESET_STATUS_REG02,1)==0)

/********************************************************
* reset u0_pwm_rstn
*********************************************************/
#define ASSERT_RESET_u0_pwm_rstn ACCESS_BIT(RESET_ASSERT_REG02,2)
#define CLEAR_RESET_u0_pwm_rstn ACCESS_BIT(RESET_CLEAR_REG02,2)
#define RESET_ASSERTED_u0_pwm_rstn (GET_BIT(RESET_STATUS_REG02,2)==0)

/********************************************************
* reset u0_rom_hresetn
*********************************************************/
#define ASSERT_RESET_u0_rom_hresetn ACCESS_BIT(RESET_ASSERT_REG02,3)
#define CLEAR_RESET_u0_rom_hresetn ACCESS_BIT(RESET_CLEAR_REG02,3)
#define RESET_ASSERTED_u0_rom_hresetn (GET_BIT(RESET_STATUS_REG02,3)==0)

/********************************************************
* reset u0_rstgen_presetn
*********************************************************/
#define ASSERT_RESET_u0_rstgen_presetn ACCESS_BIT(RESET_ASSERT_REG02,4)
#define CLEAR_RESET_u0_rstgen_presetn ACCESS_BIT(RESET_CLEAR_REG02,4)
#define RESET_ASSERTED_u0_rstgen_presetn (GET_BIT(RESET_STATUS_REG02,4)==0)

/********************************************************
* reset u0_sdio_rstn
*********************************************************/
#define ASSERT_RESET_u0_sdio_rstn ACCESS_BIT(RESET_ASSERT_REG02,5)
#define CLEAR_RESET_u0_sdio_rstn ACCESS_BIT(RESET_CLEAR_REG02,5)
#define RESET_ASSERTED_u0_sdio_rstn (GET_BIT(RESET_STATUS_REG02,5)==0)

/********************************************************
* reset u0_slow_bus_resetn
*********************************************************/
#define ASSERT_RESET_u0_slow_bus_resetn ACCESS_BIT(RESET_ASSERT_REG02,6)
#define CLEAR_RESET_u0_slow_bus_resetn ACCESS_BIT(RESET_CLEAR_REG02,6)
#define RESET_ASSERTED_u0_slow_bus_resetn (GET_BIT(RESET_STATUS_REG02,6)==0)

/********************************************************
* reset u0_spi_BnRES
*********************************************************/
#define ASSERT_RESET_u0_spi_BnRES ACCESS_BIT(RESET_ASSERT_REG02,7)
#define CLEAR_RESET_u0_spi_BnRES ACCESS_BIT(RESET_CLEAR_REG02,7)
#define RESET_ASSERTED_u0_spi_BnRES (GET_BIT(RESET_STATUS_REG02,7)==0)

/********************************************************
* reset u0_spi_nRES
*********************************************************/
#define ASSERT_RESET_u0_spi_nRES ACCESS_BIT(RESET_ASSERT_REG02,8)
#define CLEAR_RESET_u0_spi_nRES ACCESS_BIT(RESET_CLEAR_REG02,8)
#define RESET_ASSERTED_u0_spi_nRES (GET_BIT(RESET_STATUS_REG02,8)==0)

/********************************************************
* reset u0_spi_nSSRST
*********************************************************/
#define ASSERT_RESET_u0_spi_nSSRST ACCESS_BIT(RESET_ASSERT_REG02,9)
#define CLEAR_RESET_u0_spi_nSSRST ACCESS_BIT(RESET_CLEAR_REG02,9)
#define RESET_ASSERTED_u0_spi_nSSRST (GET_BIT(RESET_STATUS_REG02,9)==0)

/********************************************************
* reset u0_syscon_presetn
*********************************************************/
#define ASSERT_RESET_u0_syscon_presetn ACCESS_BIT(RESET_ASSERT_REG02,10)
#define CLEAR_RESET_u0_syscon_presetn ACCESS_BIT(RESET_CLEAR_REG02,10)
#define RESET_ASSERTED_u0_syscon_presetn (GET_BIT(RESET_STATUS_REG02,10)==0)

/********************************************************
* reset u0_timer64_presetn
*********************************************************/
#define ASSERT_RESET_u0_timer64_presetn ACCESS_BIT(RESET_ASSERT_REG02,11)
#define CLEAR_RESET_u0_timer64_presetn ACCESS_BIT(RESET_CLEAR_REG02,11)
#define RESET_ASSERTED_u0_timer64_presetn (GET_BIT(RESET_STATUS_REG02,11)==0)

/********************************************************
* reset u0_timer64_rstn
*********************************************************/
#define ASSERT_RESET_u0_timer64_rstn ACCESS_BIT(RESET_ASSERT_REG02,12)
#define CLEAR_RESET_u0_timer64_rstn ACCESS_BIT(RESET_CLEAR_REG02,12)
#define RESET_ASSERTED_u0_timer64_rstn (GET_BIT(RESET_STATUS_REG02,12)==0)

/********************************************************
* reset u0_timer_presetn
*********************************************************/
#define ASSERT_RESET_u0_timer_presetn ACCESS_BIT(RESET_ASSERT_REG02,13)
#define CLEAR_RESET_u0_timer_presetn ACCESS_BIT(RESET_CLEAR_REG02,13)
#define RESET_ASSERTED_u0_timer_presetn (GET_BIT(RESET_STATUS_REG02,13)==0)

/********************************************************
* reset u0_timer_timer0_resetn
*********************************************************/
#define ASSERT_RESET_u0_timer_timer0_resetn ACCESS_BIT(RESET_ASSERT_REG02,14)
#define CLEAR_RESET_u0_timer_timer0_resetn ACCESS_BIT(RESET_CLEAR_REG02,14)
#define RESET_ASSERTED_u0_timer_timer0_resetn (GET_BIT(RESET_STATUS_REG02,14)==0)

/********************************************************
* reset u0_timer_timer1_resetn
*********************************************************/
#define ASSERT_RESET_u0_timer_timer1_resetn ACCESS_BIT(RESET_ASSERT_REG02,15)
#define CLEAR_RESET_u0_timer_timer1_resetn ACCESS_BIT(RESET_CLEAR_REG02,15)
#define RESET_ASSERTED_u0_timer_timer1_resetn (GET_BIT(RESET_STATUS_REG02,15)==0)

/********************************************************
* reset u0_timer_timer2_resetn
*********************************************************/
#define ASSERT_RESET_u0_timer_timer2_resetn ACCESS_BIT(RESET_ASSERT_REG02,16)
#define CLEAR_RESET_u0_timer_timer2_resetn ACCESS_BIT(RESET_CLEAR_REG02,16)
#define RESET_ASSERTED_u0_timer_timer2_resetn (GET_BIT(RESET_STATUS_REG02,16)==0)

/********************************************************
* reset u0_timer_timer3_resetn
*********************************************************/
#define ASSERT_RESET_u0_timer_timer3_resetn ACCESS_BIT(RESET_ASSERT_REG02,17)
#define CLEAR_RESET_u0_timer_timer3_resetn ACCESS_BIT(RESET_CLEAR_REG02,17)
#define RESET_ASSERTED_u0_timer_timer3_resetn (GET_BIT(RESET_STATUS_REG02,17)==0)

/********************************************************
* reset u0_timer_timer4_resetn
*********************************************************/
#define ASSERT_RESET_u0_timer_timer4_resetn ACCESS_BIT(RESET_ASSERT_REG02,18)
#define CLEAR_RESET_u0_timer_timer4_resetn ACCESS_BIT(RESET_CLEAR_REG02,18)
#define RESET_ASSERTED_u0_timer_timer4_resetn (GET_BIT(RESET_STATUS_REG02,18)==0)

/********************************************************
* reset u0_timer_timer5_resetn
*********************************************************/
#define ASSERT_RESET_u0_timer_timer5_resetn ACCESS_BIT(RESET_ASSERT_REG02,19)
#define CLEAR_RESET_u0_timer_timer5_resetn ACCESS_BIT(RESET_CLEAR_REG02,19)
#define RESET_ASSERTED_u0_timer_timer5_resetn (GET_BIT(RESET_STATUS_REG02,19)==0)

/********************************************************
* reset u0_timer_timer6_resetn
*********************************************************/
#define ASSERT_RESET_u0_timer_timer6_resetn ACCESS_BIT(RESET_ASSERT_REG02,20)
#define CLEAR_RESET_u0_timer_timer6_resetn ACCESS_BIT(RESET_CLEAR_REG02,20)
#define RESET_ASSERTED_u0_timer_timer6_resetn (GET_BIT(RESET_STATUS_REG02,20)==0)

/********************************************************
* reset u0_timer_wdog_rstn
*********************************************************/
#define ASSERT_RESET_u0_timer_wdog_rstn ACCESS_BIT(RESET_ASSERT_REG02,21)
#define CLEAR_RESET_u0_timer_wdog_rstn ACCESS_BIT(RESET_CLEAR_REG02,21)
#define RESET_ASSERTED_u0_timer_wdog_rstn (GET_BIT(RESET_STATUS_REG02,21)==0)

/********************************************************
* reset u0_uart_presetn
*********************************************************/
#define ASSERT_RESET_u0_uart_presetn ACCESS_BIT(RESET_ASSERT_REG02,22)
#define CLEAR_RESET_u0_uart_presetn ACCESS_BIT(RESET_CLEAR_REG02,22)
#define RESET_ASSERTED_u0_uart_presetn (GET_BIT(RESET_STATUS_REG02,22)==0)

/********************************************************
* reset u0_uart_resetn
*********************************************************/
#define ASSERT_RESET_u0_uart_resetn ACCESS_BIT(RESET_ASSERT_REG02,23)
#define CLEAR_RESET_u0_uart_resetn ACCESS_BIT(RESET_CLEAR_REG02,23)
#define RESET_ASSERTED_u0_uart_resetn (GET_BIT(RESET_STATUS_REG02,23)==0)

/********************************************************
* reset u0_usb3_aresetn
*********************************************************/
#define ASSERT_RESET_u0_usb3_aresetn ACCESS_BIT(RESET_ASSERT_REG02,24)
#define CLEAR_RESET_u0_usb3_aresetn ACCESS_BIT(RESET_CLEAR_REG02,24)
#define RESET_ASSERTED_u0_usb3_aresetn (GET_BIT(RESET_STATUS_REG02,24)==0)

/********************************************************
* reset u0_usb3_macro_presetn
*********************************************************/
#define ASSERT_RESET_u0_usb3_macro_presetn ACCESS_BIT(RESET_ASSERT_REG02,25)
#define CLEAR_RESET_u0_usb3_macro_presetn ACCESS_BIT(RESET_CLEAR_REG02,25)
#define RESET_ASSERTED_u0_usb3_macro_presetn (GET_BIT(RESET_STATUS_REG02,25)==0)

/********************************************************
* reset u0_usb_dev_phy_rst
*********************************************************/
#define ASSERT_RESET_u0_usb_dev_phy_rst ACCESS_BIT(RESET_ASSERT_REG02,26)
#define CLEAR_RESET_u0_usb_dev_phy_rst ACCESS_BIT(RESET_CLEAR_REG02,26)
#define RESET_ASSERTED_u0_usb_dev_phy_rst (GET_BIT(RESET_STATUS_REG02,26)==1)

/********************************************************
* reset u0_vi_domain_aresetn_0
*********************************************************/
#define ASSERT_RESET_u0_vi_domain_aresetn_0 ACCESS_BIT(RESET_ASSERT_REG02,27)
#define CLEAR_RESET_u0_vi_domain_aresetn_0 ACCESS_BIT(RESET_CLEAR_REG02,27)
#define RESET_ASSERTED_u0_vi_domain_aresetn_0 (GET_BIT(RESET_STATUS_REG02,27)==0)

/********************************************************
* reset u0_vi_domain_aresetn_1
*********************************************************/
#define ASSERT_RESET_u0_vi_domain_aresetn_1 ACCESS_BIT(RESET_ASSERT_REG02,28)
#define CLEAR_RESET_u0_vi_domain_aresetn_1 ACCESS_BIT(RESET_CLEAR_REG02,28)
#define RESET_ASSERTED_u0_vi_domain_aresetn_1 (GET_BIT(RESET_STATUS_REG02,28)==0)

/********************************************************
* reset u0_vi_domain_presetn_0
*********************************************************/
#define ASSERT_RESET_u0_vi_domain_presetn_0 ACCESS_BIT(RESET_ASSERT_REG02,29)
#define CLEAR_RESET_u0_vi_domain_presetn_0 ACCESS_BIT(RESET_CLEAR_REG02,29)
#define RESET_ASSERTED_u0_vi_domain_presetn_0 (GET_BIT(RESET_STATUS_REG02,29)==0)

/********************************************************
* reset u0_vi_domain_presetn_1
*********************************************************/
#define ASSERT_RESET_u0_vi_domain_presetn_1 ACCESS_BIT(RESET_ASSERT_REG02,30)
#define CLEAR_RESET_u0_vi_domain_presetn_1 ACCESS_BIT(RESET_CLEAR_REG02,30)
#define RESET_ASSERTED_u0_vi_domain_presetn_1 (GET_BIT(RESET_STATUS_REG02,30)==0)

/********************************************************
* reset u0_vi_domain_presetn_2
*********************************************************/
#define ASSERT_RESET_u0_vi_domain_presetn_2 ACCESS_BIT(RESET_ASSERT_REG02,31)
#define CLEAR_RESET_u0_vi_domain_presetn_2 ACCESS_BIT(RESET_CLEAR_REG02,31)
#define RESET_ASSERTED_u0_vi_domain_presetn_2 (GET_BIT(RESET_STATUS_REG02,31)==0)

/********************************************************
* reset u0_vi_domain_presetn_3
*********************************************************/
#define ASSERT_RESET_u0_vi_domain_presetn_3 ACCESS_BIT(RESET_ASSERT_REG03,0)
#define CLEAR_RESET_u0_vi_domain_presetn_3 ACCESS_BIT(RESET_CLEAR_REG03,0)
#define RESET_ASSERTED_u0_vi_domain_presetn_3 (GET_BIT(RESET_STATUS_REG03,0)==0)

/********************************************************
* reset u0_vi_domain_presetn_4
*********************************************************/
#define ASSERT_RESET_u0_vi_domain_presetn_4 ACCESS_BIT(RESET_ASSERT_REG03,1)
#define CLEAR_RESET_u0_vi_domain_presetn_4 ACCESS_BIT(RESET_CLEAR_REG03,1)
#define RESET_ASSERTED_u0_vi_domain_presetn_4 (GET_BIT(RESET_STATUS_REG03,1)==0)

/********************************************************
* reset u0_vic_hresetn
*********************************************************/
#define ASSERT_RESET_u0_vic_hresetn ACCESS_BIT(RESET_ASSERT_REG03,2)
#define CLEAR_RESET_u0_vic_hresetn ACCESS_BIT(RESET_CLEAR_REG03,2)
#define RESET_ASSERTED_u0_vic_hresetn (GET_BIT(RESET_STATUS_REG03,2)==0)

/********************************************************
* reset u1_ahb2apb_hresetn
*********************************************************/
#define ASSERT_RESET_u1_ahb2apb_hresetn ACCESS_BIT(RESET_ASSERT_REG03,3)
#define CLEAR_RESET_u1_ahb2apb_hresetn ACCESS_BIT(RESET_CLEAR_REG03,3)
#define RESET_ASSERTED_u1_ahb2apb_hresetn (GET_BIT(RESET_STATUS_REG03,3)==0)

/********************************************************
* reset u1_csi_ipi_rstn
*********************************************************/
#define ASSERT_RESET_u1_csi_ipi_rstn ACCESS_BIT(RESET_ASSERT_REG03,4)
#define CLEAR_RESET_u1_csi_ipi_rstn ACCESS_BIT(RESET_CLEAR_REG03,4)
#define RESET_ASSERTED_u1_csi_ipi_rstn (GET_BIT(RESET_STATUS_REG03,4)==0)

/********************************************************
* reset u1_dsp_core_rstn
*********************************************************/
#define ASSERT_RESET_u1_dsp_core_rstn ACCESS_BIT(RESET_ASSERT_REG03,5)
#define CLEAR_RESET_u1_dsp_core_rstn ACCESS_BIT(RESET_CLEAR_REG03,5)
#define RESET_ASSERTED_u1_dsp_core_rstn (GET_BIT(RESET_STATUS_REG03,5)==0)

/********************************************************
* reset u1_dsp_global_rstn
*********************************************************/
#define ASSERT_RESET_u1_dsp_global_rstn ACCESS_BIT(RESET_ASSERT_REG03,6)
#define CLEAR_RESET_u1_dsp_global_rstn ACCESS_BIT(RESET_CLEAR_REG03,6)
#define RESET_ASSERTED_u1_dsp_global_rstn (GET_BIT(RESET_STATUS_REG03,6)==0)

/********************************************************
* reset u1_dsp_ocem_rstn
*********************************************************/
#define ASSERT_RESET_u1_dsp_ocem_rstn ACCESS_BIT(RESET_ASSERT_REG03,7)
#define CLEAR_RESET_u1_dsp_ocem_rstn ACCESS_BIT(RESET_CLEAR_REG03,7)
#define RESET_ASSERTED_u1_dsp_ocem_rstn (GET_BIT(RESET_STATUS_REG03,7)==0)

/********************************************************
* reset u1_dsp_rstn
*********************************************************/
#define ASSERT_RESET_u1_dsp_rstn ACCESS_BIT(RESET_ASSERT_REG03,8)
#define CLEAR_RESET_u1_dsp_rstn ACCESS_BIT(RESET_CLEAR_REG03,8)
#define RESET_ASSERTED_u1_dsp_rstn (GET_BIT(RESET_STATUS_REG03,8)==0)

/********************************************************
* reset u1_dsp_sys_rstn
*********************************************************/
#define ASSERT_RESET_u1_dsp_sys_rstn ACCESS_BIT(RESET_ASSERT_REG03,9)
#define CLEAR_RESET_u1_dsp_sys_rstn ACCESS_BIT(RESET_CLEAR_REG03,9)
#define RESET_ASSERTED_u1_dsp_sys_rstn (GET_BIT(RESET_STATUS_REG03,9)==0)

/********************************************************
* reset u1_gpio_gresetn
*********************************************************/
#define ASSERT_RESET_u1_gpio_gresetn ACCESS_BIT(RESET_ASSERT_REG03,10)
#define CLEAR_RESET_u1_gpio_gresetn ACCESS_BIT(RESET_CLEAR_REG03,10)
#define RESET_ASSERTED_u1_gpio_gresetn (GET_BIT(RESET_STATUS_REG03,10)==0)

/********************************************************
* reset u1_gpio_presetn
*********************************************************/
#define ASSERT_RESET_u1_gpio_presetn ACCESS_BIT(RESET_ASSERT_REG03,11)
#define CLEAR_RESET_u1_gpio_presetn ACCESS_BIT(RESET_CLEAR_REG03,11)
#define RESET_ASSERTED_u1_gpio_presetn (GET_BIT(RESET_STATUS_REG03,11)==0)

/********************************************************
* reset u1_i2c_presetn
*********************************************************/
#define ASSERT_RESET_u1_i2c_presetn ACCESS_BIT(RESET_ASSERT_REG03,12)
#define CLEAR_RESET_u1_i2c_presetn ACCESS_BIT(RESET_CLEAR_REG03,12)
#define RESET_ASSERTED_u1_i2c_presetn (GET_BIT(RESET_STATUS_REG03,12)==0)

/********************************************************
* reset u1_i2c_resetn
*********************************************************/
#define ASSERT_RESET_u1_i2c_resetn ACCESS_BIT(RESET_ASSERT_REG03,13)
#define CLEAR_RESET_u1_i2c_resetn ACCESS_BIT(RESET_CLEAR_REG03,13)
#define RESET_ASSERTED_u1_i2c_resetn (GET_BIT(RESET_STATUS_REG03,13)==0)

/********************************************************
* reset u1_pwm_presetn
*********************************************************/
#define ASSERT_RESET_u1_pwm_presetn ACCESS_BIT(RESET_ASSERT_REG03,14)
#define CLEAR_RESET_u1_pwm_presetn ACCESS_BIT(RESET_CLEAR_REG03,14)
#define RESET_ASSERTED_u1_pwm_presetn (GET_BIT(RESET_STATUS_REG03,14)==0)

/********************************************************
* reset u1_pwm_rstn
*********************************************************/
#define ASSERT_RESET_u1_pwm_rstn ACCESS_BIT(RESET_ASSERT_REG03,15)
#define CLEAR_RESET_u1_pwm_rstn ACCESS_BIT(RESET_CLEAR_REG03,15)
#define RESET_ASSERTED_u1_pwm_rstn (GET_BIT(RESET_STATUS_REG03,15)==0)

/********************************************************
* reset u1_spi_BnRES
*********************************************************/
#define ASSERT_RESET_u1_spi_BnRES ACCESS_BIT(RESET_ASSERT_REG03,16)
#define CLEAR_RESET_u1_spi_BnRES ACCESS_BIT(RESET_CLEAR_REG03,16)
#define RESET_ASSERTED_u1_spi_BnRES (GET_BIT(RESET_STATUS_REG03,16)==0)

/********************************************************
* reset u1_spi_nRES
*********************************************************/
#define ASSERT_RESET_u1_spi_nRES ACCESS_BIT(RESET_ASSERT_REG03,17)
#define CLEAR_RESET_u1_spi_nRES ACCESS_BIT(RESET_CLEAR_REG03,17)
#define RESET_ASSERTED_u1_spi_nRES (GET_BIT(RESET_STATUS_REG03,17)==0)

/********************************************************
* reset u1_spi_nSSRST
*********************************************************/
#define ASSERT_RESET_u1_spi_nSSRST ACCESS_BIT(RESET_ASSERT_REG03,18)
#define CLEAR_RESET_u1_spi_nSSRST ACCESS_BIT(RESET_CLEAR_REG03,18)
#define RESET_ASSERTED_u1_spi_nSSRST (GET_BIT(RESET_STATUS_REG03,18)==0)

/********************************************************
* reset u1_timer64_presetn
*********************************************************/
#define ASSERT_RESET_u1_timer64_presetn ACCESS_BIT(RESET_ASSERT_REG03,19)
#define CLEAR_RESET_u1_timer64_presetn ACCESS_BIT(RESET_CLEAR_REG03,19)
#define RESET_ASSERTED_u1_timer64_presetn (GET_BIT(RESET_STATUS_REG03,19)==0)

/********************************************************
* reset u1_timer_presetn
*********************************************************/
#define ASSERT_RESET_u1_timer_presetn ACCESS_BIT(RESET_ASSERT_REG03,20)
#define CLEAR_RESET_u1_timer_presetn ACCESS_BIT(RESET_CLEAR_REG03,20)
#define RESET_ASSERTED_u1_timer_presetn (GET_BIT(RESET_STATUS_REG03,20)==0)

/********************************************************
* reset u1_timer_timer0_resetn
*********************************************************/
#define ASSERT_RESET_u1_timer_timer0_resetn ACCESS_BIT(RESET_ASSERT_REG03,21)
#define CLEAR_RESET_u1_timer_timer0_resetn ACCESS_BIT(RESET_CLEAR_REG03,21)
#define RESET_ASSERTED_u1_timer_timer0_resetn (GET_BIT(RESET_STATUS_REG03,21)==0)

/********************************************************
* reset u1_timer_timer1_resetn
*********************************************************/
#define ASSERT_RESET_u1_timer_timer1_resetn ACCESS_BIT(RESET_ASSERT_REG03,22)
#define CLEAR_RESET_u1_timer_timer1_resetn ACCESS_BIT(RESET_CLEAR_REG03,22)
#define RESET_ASSERTED_u1_timer_timer1_resetn (GET_BIT(RESET_STATUS_REG03,22)==0)

/********************************************************
* reset u1_timer_timer2_resetn
*********************************************************/
#define ASSERT_RESET_u1_timer_timer2_resetn ACCESS_BIT(RESET_ASSERT_REG03,23)
#define CLEAR_RESET_u1_timer_timer2_resetn ACCESS_BIT(RESET_CLEAR_REG03,23)
#define RESET_ASSERTED_u1_timer_timer2_resetn (GET_BIT(RESET_STATUS_REG03,23)==0)

/********************************************************
* reset u1_timer_timer3_resetn
*********************************************************/
#define ASSERT_RESET_u1_timer_timer3_resetn ACCESS_BIT(RESET_ASSERT_REG03,24)
#define CLEAR_RESET_u1_timer_timer3_resetn ACCESS_BIT(RESET_CLEAR_REG03,24)
#define RESET_ASSERTED_u1_timer_timer3_resetn (GET_BIT(RESET_STATUS_REG03,24)==0)

/********************************************************
* reset u1_timer_timer4_resetn
*********************************************************/
#define ASSERT_RESET_u1_timer_timer4_resetn ACCESS_BIT(RESET_ASSERT_REG03,25)
#define CLEAR_RESET_u1_timer_timer4_resetn ACCESS_BIT(RESET_CLEAR_REG03,25)
#define RESET_ASSERTED_u1_timer_timer4_resetn (GET_BIT(RESET_STATUS_REG03,25)==0)

/********************************************************
* reset u1_timer_timer5_resetn
*********************************************************/
#define ASSERT_RESET_u1_timer_timer5_resetn ACCESS_BIT(RESET_ASSERT_REG03,26)
#define CLEAR_RESET_u1_timer_timer5_resetn ACCESS_BIT(RESET_CLEAR_REG03,26)
#define RESET_ASSERTED_u1_timer_timer5_resetn (GET_BIT(RESET_STATUS_REG03,26)==0)

/********************************************************
* reset u1_timer_timer6_resetn
*********************************************************/
#define ASSERT_RESET_u1_timer_timer6_resetn ACCESS_BIT(RESET_ASSERT_REG03,27)
#define CLEAR_RESET_u1_timer_timer6_resetn ACCESS_BIT(RESET_CLEAR_REG03,27)
#define RESET_ASSERTED_u1_timer_timer6_resetn (GET_BIT(RESET_STATUS_REG03,27)==0)

/********************************************************
* reset u1_timer_wdog_rstn
*********************************************************/
#define ASSERT_RESET_u1_timer_wdog_rstn ACCESS_BIT(RESET_ASSERT_REG03,28)
#define CLEAR_RESET_u1_timer_wdog_rstn ACCESS_BIT(RESET_CLEAR_REG03,28)
#define RESET_ASSERTED_u1_timer_wdog_rstn (GET_BIT(RESET_STATUS_REG03,28)==0)

/********************************************************
* reset u1_uart_presetn
*********************************************************/
#define ASSERT_RESET_u1_uart_presetn ACCESS_BIT(RESET_ASSERT_REG03,29)
#define CLEAR_RESET_u1_uart_presetn ACCESS_BIT(RESET_CLEAR_REG03,29)
#define RESET_ASSERTED_u1_uart_presetn (GET_BIT(RESET_STATUS_REG03,29)==0)

/********************************************************
* reset u1_uart_resetn
*********************************************************/
#define ASSERT_RESET_u1_uart_resetn ACCESS_BIT(RESET_ASSERT_REG03,30)
#define CLEAR_RESET_u1_uart_resetn ACCESS_BIT(RESET_CLEAR_REG03,30)
#define RESET_ASSERTED_u1_uart_resetn (GET_BIT(RESET_STATUS_REG03,30)==0)

/********************************************************
* reset u1_vic_hresetn
*********************************************************/
#define ASSERT_RESET_u1_vic_hresetn ACCESS_BIT(RESET_ASSERT_REG03,31)
#define CLEAR_RESET_u1_vic_hresetn ACCESS_BIT(RESET_CLEAR_REG03,31)
#define RESET_ASSERTED_u1_vic_hresetn (GET_BIT(RESET_STATUS_REG03,31)==0)

/********************************************************
* reset u2_ahb2apb_hresetn
*********************************************************/
#define ASSERT_RESET_u2_ahb2apb_hresetn ACCESS_BIT(RESET_ASSERT_REG04,0)
#define CLEAR_RESET_u2_ahb2apb_hresetn ACCESS_BIT(RESET_CLEAR_REG04,0)
#define RESET_ASSERTED_u2_ahb2apb_hresetn (GET_BIT(RESET_STATUS_REG04,0)==0)

/********************************************************
* reset u2_csi_ipi_rstn
*********************************************************/
#define ASSERT_RESET_u2_csi_ipi_rstn ACCESS_BIT(RESET_ASSERT_REG04,1)
#define CLEAR_RESET_u2_csi_ipi_rstn ACCESS_BIT(RESET_CLEAR_REG04,1)
#define RESET_ASSERTED_u2_csi_ipi_rstn (GET_BIT(RESET_STATUS_REG04,1)==0)

/********************************************************
* reset u2_gpio_gresetn
*********************************************************/
#define ASSERT_RESET_u2_gpio_gresetn ACCESS_BIT(RESET_ASSERT_REG04,2)
#define CLEAR_RESET_u2_gpio_gresetn ACCESS_BIT(RESET_CLEAR_REG04,2)
#define RESET_ASSERTED_u2_gpio_gresetn (GET_BIT(RESET_STATUS_REG04,2)==0)

/********************************************************
* reset u2_gpio_presetn
*********************************************************/
#define ASSERT_RESET_u2_gpio_presetn ACCESS_BIT(RESET_ASSERT_REG04,3)
#define CLEAR_RESET_u2_gpio_presetn ACCESS_BIT(RESET_CLEAR_REG04,3)
#define RESET_ASSERTED_u2_gpio_presetn (GET_BIT(RESET_STATUS_REG04,3)==0)

/********************************************************
* reset u2_vic_hresetn
*********************************************************/
#define ASSERT_RESET_u2_vic_hresetn ACCESS_BIT(RESET_ASSERT_REG04,4)
#define CLEAR_RESET_u2_vic_hresetn ACCESS_BIT(RESET_CLEAR_REG04,4)
#define RESET_ASSERTED_u2_vic_hresetn (GET_BIT(RESET_STATUS_REG04,4)==0)

/********************************************************
* reset u3_ahb2apb_hresetn
*********************************************************/
#define ASSERT_RESET_u3_ahb2apb_hresetn ACCESS_BIT(RESET_ASSERT_REG04,5)
#define CLEAR_RESET_u3_ahb2apb_hresetn ACCESS_BIT(RESET_CLEAR_REG04,5)
#define RESET_ASSERTED_u3_ahb2apb_hresetn (GET_BIT(RESET_STATUS_REG04,5)==0)

/********************************************************
* reset u3_vic_hresetn
*********************************************************/
#define ASSERT_RESET_u3_vic_hresetn ACCESS_BIT(RESET_ASSERT_REG04,6)
#define CLEAR_RESET_u3_vic_hresetn ACCESS_BIT(RESET_CLEAR_REG04,6)
#define RESET_ASSERTED_u3_vic_hresetn (GET_BIT(RESET_STATUS_REG04,6)==0)

/********************************************************
* reset vip8000_aresetn
*********************************************************/
#define ASSERT_RESET_vip8000_aresetn ACCESS_BIT(RESET_ASSERT_REG04,7)
#define CLEAR_RESET_vip8000_aresetn ACCESS_BIT(RESET_CLEAR_REG04,7)
#define RESET_ASSERTED_vip8000_aresetn (GET_BIT(RESET_STATUS_REG04,7)==0)

/********************************************************
* reset vip8000_core_rstn
*********************************************************/
#define ASSERT_RESET_vip8000_core_rstn ACCESS_BIT(RESET_ASSERT_REG04,8)
#define CLEAR_RESET_vip8000_core_rstn ACCESS_BIT(RESET_CLEAR_REG04,8)
#define RESET_ASSERTED_vip8000_core_rstn (GET_BIT(RESET_STATUS_REG04,8)==0)

/********************************************************
* reset vip8000_hresetn
*********************************************************/
#define ASSERT_RESET_vip8000_hresetn ACCESS_BIT(RESET_ASSERT_REG04,9)
#define CLEAR_RESET_vip8000_hresetn ACCESS_BIT(RESET_CLEAR_REG04,9)
#define RESET_ASSERTED_vip8000_hresetn (GET_BIT(RESET_STATUS_REG04,9)==0)

/********************************************************
* reset vpu_aresetn
*********************************************************/
#define ASSERT_RESET_vpu_aresetn ACCESS_BIT(RESET_ASSERT_REG04,10)
#define CLEAR_RESET_vpu_aresetn ACCESS_BIT(RESET_CLEAR_REG04,10)
#define RESET_ASSERTED_vpu_aresetn (GET_BIT(RESET_STATUS_REG04,10)==0)

/********************************************************
* reset vpu_cresetn
*********************************************************/
#define ASSERT_RESET_vpu_cresetn ACCESS_BIT(RESET_ASSERT_REG04,11)
#define CLEAR_RESET_vpu_cresetn ACCESS_BIT(RESET_CLEAR_REG04,11)
#define RESET_ASSERTED_vpu_cresetn (GET_BIT(RESET_STATUS_REG04,11)==0)

/********************************************************
* reset vpu_fpe_aresetn
*********************************************************/
#define ASSERT_RESET_vpu_fpe_aresetn ACCESS_BIT(RESET_ASSERT_REG04,12)
#define CLEAR_RESET_vpu_fpe_aresetn ACCESS_BIT(RESET_CLEAR_REG04,12)
#define RESET_ASSERTED_vpu_fpe_aresetn (GET_BIT(RESET_STATUS_REG04,12)==0)

/********************************************************
* reset vpu_fpe_presetn
*********************************************************/
#define ASSERT_RESET_vpu_fpe_presetn ACCESS_BIT(RESET_ASSERT_REG04,13)
#define CLEAR_RESET_vpu_fpe_presetn ACCESS_BIT(RESET_CLEAR_REG04,13)
#define RESET_ASSERTED_vpu_fpe_presetn (GET_BIT(RESET_STATUS_REG04,13)==0)

/********************************************************
* reset vpu_presetn
*********************************************************/
#define ASSERT_RESET_vpu_presetn ACCESS_BIT(RESET_ASSERT_REG04,14)
#define CLEAR_RESET_vpu_presetn ACCESS_BIT(RESET_CLEAR_REG04,14)
#define RESET_ASSERTED_vpu_presetn (GET_BIT(RESET_STATUS_REG04,14)==0)

/********************************************************
* reset vpu_rresetn
*********************************************************/
#define ASSERT_RESET_vpu_rresetn ACCESS_BIT(RESET_ASSERT_REG04,15)
#define CLEAR_RESET_vpu_rresetn ACCESS_BIT(RESET_CLEAR_REG04,15)
#define RESET_ASSERTED_vpu_rresetn (GET_BIT(RESET_STATUS_REG04,15)==0)

#endif //RST_GEN_MACRO_H
