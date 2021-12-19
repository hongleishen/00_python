#ifndef __CLK_UTILS_H__ 
#define __CLK_UTILS_H__ 

#define  reg_pll0_REFDIV        0x301800c4
#define  reg_pll0_FBDIV         0x301800c8
#define  reg_pll0_FRAC          0x301800cc
#define  reg_pll0_POSTDIV1      0x301800d0
#define  reg_pll0_POSTDIV2      0x301800d4
#define  reg_pll0_PD            0x301800d8
#define  reg_pll0_DACPD         0x301800dc
#define  reg_pll0_DSMPD         0x301800e0
#define  reg_pll0_FOUTPOSTDIVPD 0x301800e4
#define  reg_pll0_FOUT4PHASEPD  0x301800e8
#define  reg_pll0_FOUTVCOPD     0x301800ec
#define  reg_pll0_BYPASS        0x301800f0
#define  reg_pll0_lock          0x301800f4

#define  reg_pll1_REFDIV        0x301800f8
#define  reg_pll1_FBDIV         0x301800fc
#define  reg_pll1_FRAC          0x30180100
#define  reg_pll1_POSTDIV1      0x30180104
#define  reg_pll1_POSTDIV2      0x30180108
#define  reg_pll1_PD            0x3018010c
#define  reg_pll1_DACPD         0x30180110
#define  reg_pll1_DSMPD         0x30180114
#define  reg_pll1_FOUTPOSTDIVPD 0x30180118
#define  reg_pll1_FOUT4PHASEPD  0x3018011c
#define  reg_pll1_FOUTVCOPD     0x30180120
#define  reg_pll1_BYPASS        0x30180124
#define  reg_pll1_lock          0x30180128

#define  reg_pll2_REFDIV        0x3018012c
#define  reg_pll2_FBDIV         0x30180130
#define  reg_pll2_FRAC          0x30180134
#define  reg_pll2_POSTDIV1      0x30180138
#define  reg_pll2_POSTDIV2      0x3018013c
#define  reg_pll2_PD            0x30180140
#define  reg_pll2_DACPD         0x30180144
#define  reg_pll2_DSMPD         0x30180148
#define  reg_pll2_FOUTPOSTDIVPD 0x3018014c
#define  reg_pll2_FOUT4PHASEPD  0x30180150
#define  reg_pll2_FOUTVCOPD     0x30180154
#define  reg_pll2_BYPASS        0x30180158
#define  reg_pll2_lock          0x3018015c


//void cfg_pll0()
// Pll will be 600Mhz
#define cfg_pll0_1(pll_freq)		\
do {								\
  MA_OUTW(reg_pll0_POSTDIV1, 2);	\
  MA_OUTW(reg_pll0_POSTDIV2, 1);	\
  MA_OUTW(reg_pll0_REFDIV, 12);		\
  MA_OUTW(reg_pll0_DSMPD, 1);		\
  MA_OUTW(reg_pll0_FBDIV, pll_freq);\
  MA_OUTW(reg_pll0_PD,  0);			\
} while (0)

#define wait_for_pll0_lock_1()		\
do {								\
   volatile u32 val;				\
   val = MA_INW(reg_pll0_lock);		\
   while (!val) {					\
     val = MA_INW(reg_pll0_lock);	\
   }								\
} while (0)

//void cfg_pll1()
// Pll will be 800Mhz
#define cfg_pll1_1(pll_freq)		\
do {								\
  MA_OUTW(reg_pll1_POSTDIV1, 2);	\
  MA_OUTW(reg_pll1_POSTDIV2, 1);	\
  MA_OUTW(reg_pll1_REFDIV, 12);		\
  MA_OUTW(reg_pll1_DSMPD, 1);		\
  MA_OUTW(reg_pll1_FBDIV, pll_freq);\
  MA_OUTW(reg_pll1_PD,  0);			\
} while (0)

#define wait_for_pll1_lock_1()		\
do {								\
   volatile u32 val;				\
   val = MA_INW(reg_pll1_lock);		\
   while (!val) {					\
     val = MA_INW(reg_pll1_lock);	\
   }								\
} while (0)

//void cfg_pll2()
// Pll will be 533Mhz
#define cfg_pll2_1(pll_freq)		\
do {								\
  MA_OUTW(reg_pll2_POSTDIV1, 2);	\
  MA_OUTW(reg_pll2_POSTDIV2, 1);	\
  MA_OUTW(reg_pll2_REFDIV, 12);		\
  MA_OUTW(reg_pll2_DSMPD, 1);		\
  MA_OUTW(reg_pll2_FBDIV, pll_freq);\
  MA_OUTW(reg_pll2_PD,  0);			\
} while (0)

#define wait_for_pll2_lock_1()		\
do {								\
   volatile u32 val;				\
   val = MA_INW(reg_pll2_lock);		\
   while (!val) {					\
     val = MA_INW(reg_pll2_lock);	\
   }								\
} while (0)

/*
void switch_to_pll_clk()
{
	volatile CLK_GEN_T *bl_clkgen;
	volatile MAIN_ROOT_CLK_CTRL_U main_root_clk_ctrl;
	
	bl_clkgen = (volatile CLK_GEN_T*) CLK_GEN_BASE_ADDR;
  
     main_root_clk_ctrl.v = bl_clkgen->main_root_clk_ctrl.v;
	
	main_root_clk_ctrl.bit_info.main_root_clk_sel   =0x1;
    main_root_clk_ctrl.bit_info.bus_clk_switch      =0x1;
	
	bl_clkgen->main_root_clk_ctrl.v = main_root_clk_ctrl.v;
	
}

void switch_to_fast_bus_clk()
{
    volatile CLK_GEN_T *bl_clkgen;
    volatile FAST_CLK_SWITCH_U fast_clk_switch;
    volatile MST_CLK_SWITCH_U  mst_clk_switch;
    volatile SLOW_CLK_SWITCH_U slow_clk_switch;
	
	bl_clkgen = (volatile CLK_GEN_T*) CLK_GEN_BASE_ADDR;
    fast_clk_switch.bit_info.fast_clk_switch = 0x1;
    mst_clk_switch.bit_info.mst_clk_switch   = 0x1;
    slow_clk_switch.bit_info.slow_clk_switch = 0x1;

    bl_clkgen->fast_clk_switch.v = fast_clk_switch.v;
    bl_clkgen->mst_clk_switch.v  = mst_clk_switch.v;
    bl_clkgen->slow_clk_switch.v = slow_clk_switch.v;
	
}


void init_bus_clk_div()
{
  volatile CLK_GEN_T *bl_clkgen;
  volatile FAST_SRC_SEL_U fast_src_sel;
  volatile FAST_CLK_DIV_U fast_clk_div;
  volatile MST_SRC_SEL_U mst_src_sel;
  volatile MST_CLK_DIV_U mst_clk_div;
  volatile SLOW_SRC_SEL_U slow_src_sel;
  volatile SLOW_CLK_DIV_U slow_clk_div;

  bl_clkgen=(volatile CLK_GEN_T*) CLK_GEN_BASE_ADDR;
 
  
  fast_clk_div.bit_info.fast_clk_div = 0x2;
  mst_clk_div.bit_info.mst_clk_div   = 0x2;
  slow_clk_div.bit_info.slow_clk_div = 0x4;
 

  bl_clkgen->fast_clk_div.v = fast_clk_div.v;
  bl_clkgen->mst_clk_div.v  = mst_clk_div.v;
  bl_clkgen->slow_clk_div.v = slow_clk_div.v;
}

void init_bus_clk_switch()
{
  volatile CLK_GEN_T *bl_clkgen;
  volatile FAST_SRC_SEL_U fast_src_sel;
  volatile FAST_CLK_DIV_U fast_clk_div;
  volatile MST_SRC_SEL_U mst_src_sel;
  volatile MST_CLK_DIV_U mst_clk_div;
  volatile SLOW_SRC_SEL_U slow_src_sel;
  volatile SLOW_CLK_DIV_U slow_clk_div;

  bl_clkgen=(volatile CLK_GEN_T*) CLK_GEN_BASE_ADDR;
 
  fast_src_sel.bit_info.fast_src_sel = 0x2;
  mst_src_sel.bit_info.mst_src_sel   = 0x3; // Modified by vli@2017/08/24
  slow_src_sel.bit_info.slow_src_sel = 0x3;
  
 
  bl_clkgen->fast_src_sel.v =  fast_src_sel.v;
  bl_clkgen->mst_src_sel.v  =  mst_src_sel.v;
  bl_clkgen->slow_src_sel.v =  slow_src_sel.v;

}
*/
#endif
