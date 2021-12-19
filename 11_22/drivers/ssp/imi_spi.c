#if !((defined TARGET_SRAM) || (defined TARGET_DDR))

#include "imi_spi.h"
#include "imi_io.h"
#include "gpio.h"

#define bits_per_word_to_dss(bpw)	((bpw) - 1)

#define clamp(val, low, high) \
	(val> high ? high : (val < low ? low : val));

static U32 reg_base1;
static U32 reg_base2;

//static U32 cs_base;

void imi_udelay(U32 cycles)
{
	volatile U32 i;
	for(i = 0; i < 7 * cycles; i++);
}

void imi_mdelay(U32 cycles)
{
	volatile U32 i;
	for(i = 0; i < cycles; i++)
		imi_udelay(1000);
}

void spi_clear_irq(void)
{
	writel(0, (reg_base1 + SSPICR));

}
static void imi2280_spi0_hw_prepare(void)
{
	U32 val;

	val = readl(reg_base1 + SSPCR1);
	val |= SSPCR1_SSE;
	writel(val, reg_base1 + SSPCR1);
}

static void imi2280_spi1_hw_prepare(void)
{
	U32 val;

	val = readl(reg_base2 + SSPCR1);
	val |= SSPCR1_SSE;
	writel(val, reg_base2 + SSPCR1);
}

static void imi2280_spi0_hw_unprepare(void)
{
	U32 val;

	val = readl(reg_base1 + SSPCR1);
	val &= ~SSPCR1_SSE;
	writel(val, (reg_base1 + SSPCR1));
}


static void imi2280_spi1_hw_unprepare(void)
{
	U32 val;

	val = readl(reg_base2 + SSPCR1);
	val &= ~SSPCR1_SSE;
	writel(val, (reg_base2 + SSPCR1));
}

static void imi2280_spi_cs_activate(void)
{
	//writel(0x01, cs_base);
	//    imi_printf("imi2280_spi_cs_activate\n");
	gpio_set_value(8,0);
	//gpio_set_value(33,0);
}

static void imi2280_spi_cs_deactivate(void)
{
	//s   imi_printf("imi2280_spi_cs_deactivate\n");
	gpio_set_value(8,1);
	//gpio_set_value(33,1);
	//writel(0x03, cs_base);
}
#if 0
static void imi2280_spi_global_reset(void)
{
	U32 val;

	imi_printf("1111 imi2280_spi_global_reset\n");
	val = readl(SPI0_REG_RST);
	val |= SSPRST;
	writel(val, SPI0_REG_RST);
	imi_printf("222 imi2280_spi_global_reset\n");
	imi_udelay(100);
	imi_printf("333 imi2280_spi_global_reset\n");
	val = readl(SPI0_REG_RST);
	val &= ~SSPRST;
	writel(val, SPI0_REG_RST);
	imi_udelay(100);

	val = readl(SPI0_REG_CLR);
	val |= SSPRST;
	writel(val, SPI0_REG_CLR);
	imi_udelay(100);
	val = readl(SPI0_REG_CLR);
	val &= ~SSPRST;
	writel(val, SPI0_REG_CLR);
	imi_udelay(100);
}
#endif
static int imi2280_spi_calc_divisors(U32 rate, U8 *div_cpsr, U8 *div_scr)
{
	unsigned long spi_clk_rate = SPI_CLK_INPUT;
	int cpsr, scr;
	U32 min_speed, max_speed;

	max_speed = SPI_CLK_INPUT / 2;
	min_speed = SPI_CLK_INPUT / (254 * 256);
	/*
	 * Make sure that max value is between values supported by the
	 * controller.
	 */
	rate = clamp(rate, min_speed, max_speed);

	/*
	 * Calculate divisors so that we can get speed according the
	 * following formula:
	 *	rate = spi_clock_rate / (cpsr * (1 + scr))
	 *
	 * cpsr must be even number and starts from 2, scr can be any number
	 * between 0 and 255.
	 */
	for (cpsr = 2; cpsr <= 254; cpsr += 2) {
		for (scr = 0; scr <= 255; scr++) {
			if ((spi_clk_rate / (cpsr * (scr + 1))) <= rate) {
				*div_scr = (U8)scr;
				*div_cpsr = (U8)cpsr;
				return 0;
			}
		}
	}

	return -1;
}

static void imi2280_spi_clear_fifo(void)
{
	//	u32 val;
	//	imi_printf("imi2280_spi_clear_fifo\n");
	while (readl(reg_base1 + SSPSR) & SSPSR_RNE) {
		//val=readl(reg_base1 + SSPDR);
		readl(reg_base1 + SSPDR);
		//imi_printf("imi2280_spi_clear_fifo val is 0x%0x\n",val);
	}
}

void spi_gpio_set_func(void)
{
	gpad_request_one(8,1);
	gpad_request_one(9,0);
	gpad_request_one(10,0);
	gpad_request_one(11,0);
	gpad_request_one(33,0);
	gpad_request_one(34,0);
	gpad_request_one(35,0);
	gpad_request_one(36,0);

}

int imi2280_spi_master_init(struct spi_info *info)
{
	U8 dss = bits_per_word_to_dss(BITS_PER_WORD);
	U8 div_cpsr = 0;
	U8 div_scr = 0;
	int err;
	U16 cr0;
	U32 val;

	//imi_printf("entry imi2280_spi_master_init\n");
	/*pll0/12 = 600/10 = 60M*/
	//writel(0x8001000a, (U32)(0x30100188));

	gpio_set_value(4,0);
	gpio_set_value(5,1);
	spi_gpio_set_func();

	if (info->id == SPI_ID0)
		reg_base1 = SPI0_REG_BASE;
	else
		reg_base2 = SPI1_REG_BASE;


	err = imi2280_spi_calc_divisors(info->rate, &div_cpsr, &div_scr);
	if (err) {
		imi_printf("spi calc rate err!\n");
		return err;
	}


	cr0 = div_scr << SSPCR0_SCR_SHIFT;
	cr0 |= ((info->mode & 0x3) << SSPCR0_MODE_SHIFT);
	cr0 |= (SSI_MOTO_SPI << SSPCR0_FRF_SHIFT);
#ifdef  bit8_test

	cr0 |= dss;
#else
	cr0 |= 0xf;
#endif


	/*ssp reset*/
	//imi2280_spi_global_reset();

	/*ssp operation disabled*/
	imi2280_spi0_hw_unprepare();

	/*set spi clk rate*/
	writel(div_cpsr, (reg_base1 + SSPCPSR));
	writel(cr0, (reg_base1 + SSPCR0));

	/*enable RORIE/TIE/RIE interrupts*/
	val = readl(reg_base1 + SSPCR1);
	val |= (SSPCR1_RORIE | SSPCR1_TIE | SSPCR1_RIE);
	writel(val, (reg_base1 + SSPCR1));

	/*spi master mode*/
	val = readl(reg_base1 + SSPCR1);
	val &= ~SSPCR1_MS;
	writel(val, (reg_base1 + SSPCR1));

	/*clear receive overrun interrupt*/
	writel(0, (reg_base1 + SSPICR));

	//	writel(3, (reg_base1 + 0x0024));

#if 0
	/* Set CS Pin as GPIO */
	val = readl(CS_REG_BASE);
	val |= 0x10;
	writel(val, CS_REG_BASE);
#endif
	//cs_base = BASE_GPIO_CTRL(0, 1);

	imi2280_spi_cs_deactivate();

	/*ssp operation enabled*/
	imi2280_spi0_hw_prepare();

	return 0;
}

int imi2280_spi_slave_init(struct spi_info *info)
{
	U8 dss = bits_per_word_to_dss(BITS_PER_WORD);
	U32 cr0 = 0;
	U32 val;

	/*pll0/12 = 600/4 = 150M*/
	//writel(0x80010004, 0x30100188);

	if (info->id == SPI_ID0)
		reg_base1 = SPI0_REG_BASE;
	else
		reg_base2 = SPI1_REG_BASE;

	cr0 |= ((info->mode & 0x3) << SSPCR0_MODE_SHIFT);
	cr0 |= (SSI_MOTO_SPI << SSPCR0_FRF_SHIFT);
#ifdef  bit8_test
	cr0 |= dss;
#else
	cr0 |= 0xf;
#endif

	/*ssp reset*/
	//imi2280_spi_global_reset();

	/*ssp operation disabled*/
	imi2280_spi1_hw_unprepare();

	writel(cr0, reg_base2 + SSPCR0);

	/*enable RORIE/TIE/RIE interrupts*/
	val = readl(reg_base2 + SSPCR1);
	val |= (SSPCR1_RORIE | SSPCR1_TIE | SSPCR1_RIE);
	writel(val, reg_base2 + SSPCR1);

	/*spi slave mode*/
	val = readl(reg_base2 + SSPCR1);
	val |= SSPCR1_MS;
	writel(val, reg_base2 + SSPCR1);

	/*clear receive overrun interrupt*/
	writel(0, reg_base2 + SSPICR);

	/*ssp operation enabled*/
	imi2280_spi1_hw_prepare();

	return 0;
}
void imi2280_spi_dma_tx_test(U32 bytelen,  void *dout, void *din)
{
#if 0

	U32 toread;
	int i;
	u16 val = 0;
	//	const U16 *out = dout;
	//	U16 *in = din;
	//	static int delay = 18;
	toread= bytelen;
	//	int count=0;
	imi2280_spi_clear_fifo();
	//imi_printf("imi2280_spi_dma_tx_test entry dout is 0x%0x din is 0x%0x\n",dout,din);

	imi2280_spi_cs_activate();

	while(toread) {
		dma_mem_transfer((u32)dout,(u32)(reg_base2 + SSPDR),8,12,config);
		for(i=0;i<8;i++){

			writel(2, (reg_base1 + SSPDR));
			//	if (toread && (readl(reg_base1 + SSPSR) & SSPSR_RNE)) {
			val = readl(reg_base1 + SSPDR);
			if (din != NULL) {
				*(u8 *)din = val;
				(u8 *)din++;
				(u8 *)dout++;
				// imi_printf("#### read val is %0x\n" ,val);
			}
			toread--;
			// }
		}

	}
	imi2280_spi_cs_deactivate();
#endif
}

void imi2280_spi_dma_rx_test(U32 bytelen,  void *dout, void *din)
{
#if 0
	U32 towrite;// toread;
	u8 val = 0;
	u32 i;
	//	const U16 *out = dout;
	//	U16 *in = din;
	//	static int delay = 18;
	towrite = bytelen;
	u32 *p =(u32 *)0x47000000;

	//(int *)din =*p;
	/*clear fifo first*/
	imi2280_spi_clear_fifo();
	//imi_printf("imi2280_spi_dma_rx_test entry dout is 0x%0x din is 0x%0x\n",dout,din);
	/*CS low*/
	imi2280_spi_cs_activate();
	i=0;
	while(towrite) {
		if (towrite && (readl(reg_base2 + SSPSR) & SSPSR_TNF)) {
			if (dout != NULL) {
				val = *(u8 *)dout;
				(u8 *)dout++;
			}
			writel(val, (reg_base2 + SSPDR));
			writel(0, (reg_base1 + SSPDR));
			// imi_printf("#### write val is %0x\n" ,val);
			val = readl(reg_base2 + SSPDR);
			towrite--;
			i++;
		}
		if(i==8)
		{
			dma_mem_transfer((U32)(reg_base1 + SSPDR),(u32 )p,8,5,config);
			p += 8;
			//*p++;
			//dma_mem_transfer(reg_base1 + SSPDR,din,8,5,config);
			//imi_printf("dst is %0x,%0x\n",*p++,*p++);
			//imi_printf("dst is %0x,%0x %0x, %0x\n",(u8 *)din++,(u8 *)din++,(u8 *)din++,(u8 *)din++);
			// imi_printf("dst is %0x,%0x\n",*(int *)din++,*(int *)din++);
			i=0;
		}


	}
	/*CS high*/

	//imi_printf("imi2280_spi_master_xfer exit\n");
	imi2280_spi_cs_deactivate();
#endif
}



void imi2280_spi_loop_test(U32 bytelen,  void *dout, void *din)
{
	U32 towrite, toread;
	u16 val = 0;
	//	u32 i;
	//	const U16 *out = dout;
	//	U16 *in = din;
	//	static int delay = 18;
	towrite = toread = bytelen;

	/*clear fifo first*/
	imi2280_spi_clear_fifo();
	// imi_printf("imi2280_spi_master_xfer entry dout is 0x%0x din is 0x%0x\n",dout,din);
	/*CS low*/
	imi2280_spi_cs_activate();

	while(towrite || toread) {
		if (towrite && (readl(reg_base1 + SSPSR) & SSPSR_TNF)) {
			if (dout != 0) {
				val = *(u8 *)dout;
				(u8 *)dout++;
			}
			towrite--;
			writel(val, (reg_base2 + SSPDR));
			writel(0, (reg_base1 + SSPDR));
		}
		if (toread && (readl(reg_base1 + SSPSR) & SSPSR_RNE)) {
			val = readl(reg_base1 + SSPDR);
			if (din != 0) {
				*(u8 *)din = val;
				(u8 *)din++;
				//imi_printf("#### read val is %0x\n" ,val);
			}
			toread--;
		}
	}

	/*CS high*/

	// imi_printf("imi2280_spi_master_xfer exit\n");
	imi2280_spi_cs_deactivate();
	// imi_printf("val is %0x\n",&din);
}


void imi2280_spi_master_xfer(U32 bytelen,  void *dout, void *din)
{
	U32 towrite, toread;
	u16 val = 0;
	//	u32 i;
	//	const U16 *out = dout;
	//	U16 *in = din;
	//	static int delay = 18;
	towrite = toread = bytelen;

	/*clear fifo first*/
	imi2280_spi_clear_fifo();
	//imi_printf("imi2280_spi_master_xfer entry dout is 0x%0x din is 0x%0x\n",dout,din);
	/*CS low*/
	imi2280_spi_cs_activate();
#if 1
	while(towrite || toread) { 
		if (towrite && (readl(reg_base1 + SSPSR) & SSPSR_TNF)) {
			if (dout != 0) {
				val = *(u8 *)dout;
				(u8 *)dout++;
				//imi_printf("$$$$$$$$$$$ write val is %0x\n" ,val);
			}
			towrite--;
			writel(val, (reg_base1 + SSPDR));
		}
		if (toread && (readl(reg_base1 + SSPSR) & SSPSR_RNE)) {
			val = readl(reg_base1 + SSPDR);
			if (din != 0) {
				*(u8 *)din = val;
				(u8 *)din++;
				//imi_printf("#### read val is %0x\n" ,val);
			}
			toread--;
		}
	}
#else

	while(towrite || toread) {
		if (towrite && (readl(reg_base1 + SSPSR) & SSPSR_TNF)) {
			writel(out ? *out++ : 0, reg_base1 + SSPDR);
			//imi_printf("$$$$$$$$$$$out is %0x\n",*out);
			towrite--;
		}

		if (toread && (readl(reg_base1 + SSPSR) & SSPSR_RNE)) {
#ifdef  bit8_test
			val = read8(reg_base1 + SSPDR);
#else
			val = readl(reg_base1 + SSPDR);
#endif
			imi_printf("####val is %0x\n",val);
			if (in)
				*in++ = val;
			toread--;
		}
	}
#endif
	/*CS high*/

	// imi_printf("imi2280_spi_master_xfer exit\n");
	imi2280_spi_cs_deactivate();
	// imi_printf("val is %0x\n",&din);
}

void imi2280_spi_slave_xfer(U32 xfer_len, U8 *tx, U8 *rx)
{
	U32 tx_cnt = 0;
	U32	rx_cnt = 0;
	U32 fill_len;

	/*clear fifo first*/
	imi2280_spi_clear_fifo();

	if (tx) {
		if (xfer_len < 4)
			fill_len = xfer_len;
		else
			fill_len = 4;

		for (tx_cnt = 0; tx_cnt < fill_len; tx_cnt++) {
			writel(tx[tx_cnt], reg_base2 + SSPDR);
		}
	}

	while (rx_cnt < xfer_len) {

		while (readl(reg_base2 + SSPSR) & SSPSR_RNE) {

			rx[rx_cnt++] = readl(reg_base2 + SSPDR);
			if (tx) {
				if (tx_cnt < xfer_len)
					writel(tx[tx_cnt++], reg_base2 + SSPDR);
			}
		}
	}
}


#endif

