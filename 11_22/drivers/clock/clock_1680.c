#if defined (CHIP_IMI_1680)
#define LOG_TAG		"clock"
#include "clock.h"
#include "string.h"
#include "dev.h"
#include "elog.h"
#include "imi_type.h"
#include "initcalls.h"
#include "linux_compat.h"
#include "errno.h"

#define PLL_CLK_FREQ                    360  //360M
#define PAPB_CLK_FREQ                   90
#define BUS1_CLK_FREQ                   120
#define UART_EXT_CLK_FREQ               20
#define USB_REF_CLK_FREQ                24
#define QSPI_REF_CLK_FREQ               120
#define QSPI_FLASH_CLK_FREQ             10

#define UART_CLK            (UART_EXT_CLK_FREQ * 1000000)

static struct clk peri_dev[]= {
        {
                .devname	= "wtimer",
                .address	= WTimer_C,
        },{
                .devname	= "gpio",
                .address	= GPIO_C,
        },{
                .devname	= "ctimer",
                .address	= CTimer_C,
        },{
                .devname	= "spi",
                .address	= SPI_C,
        },{
                .devname	= "i2c",
                .address	= I2C_C,
        },{
                .devname	= "uart",
                .address	= UART_C,
        },{
                .devname	= "isp",
                .address	= ISP_C,
        },{
                .devname	= "mipi_rx0",
                .address	= MIPIRX0_C,
        },{
                .devname	= "mipi_rx1",
                .address	= MIPIRX1_C,
        },{
                .devname	= "vpu",
                .address	= VPU_C,
        },{
                .devname	= "dd",
                .address	= DD_C,
        },{
                .devname	= "mipi_tx0",
                .address	= MIPITX0_C,
        },{
                .devname	= "mipi_tx1",
                .address	= MIPITX1_C,
        },{
                .devname	= "dpu",
                .address	= DPU_C,
        },{
                .devname	= "rvl",
                .address	= RVL_C,
        },{
                .devname	= "mailbox",
                .address	= MAILBOX_C,
        },
};

static struct clk bus_dev[]= {
	{
                .devname	= "usb",
                .address	= USB2_C,
    },{
                .devname	= "dma",
                .address	= DMAQ_C,
    },{
                .devname	= "qspi",
                .address	= DMAQ_C,
    },
};

 void clock_enable_bus(u32 adress, unsigned int val)
{
	writel(0x01,   (adress + 0x1c));
	writel(val,    (adress + 0x04));
	writel(0x3,    (adress + 0x0c));
	writel(0x1,    (adress + 0x18));
	writel(0x0,    (adress + 0x00));
	writel(0x0,   (adress + 0x1c));
}

void clock_enable_dev(u32 adress, unsigned int val)
{
	writel(0x01,   (adress + 0x1c));
	writel(val,    (adress + 0x04));
	writel(0x1,    (adress + 0x18));
	writel(0x0,    (adress + 0x00));
	writel(0x0,   (adress + 0x1c));
}

int clock_set_freq(char *name, unsigned int freq)
{
	//todo
	return 0;
}

int clock_enable(char* name)
{
	int dev_index = 0, bus_index = 0;
	unsigned int flags;

	flags = local_irq_save();

	for (dev_index = 0; dev_index < ARRAY_SIZE(peri_dev); dev_index++) {
		if (!strcmp(name, peri_dev[dev_index].devname)) {
			clock_enable_bus(peri_dev[dev_index].address, 0xff);
			local_irq_restore(flags);
			return 0;
		}
	}

	for (bus_index = 0; bus_index < ARRAY_SIZE(bus_dev); bus_index++) {
		if (!strcmp(name, bus_dev[bus_index].devname)) {
			clock_enable_bus(bus_dev[bus_index].address, 0xff);
			local_irq_restore(flags);
			return 0;
		}
	}

	local_irq_restore(flags);

	return -EINVAL;
}

/******************************pll           clk********************************/

static void pll_set_freq(unsigned int addr, unsigned int freq)
{
//OSC default is 24M
	unsigned int val, timeout = 0xffff;

	val = freq / 3;

	writel(0x00, addr + 0x24); //use OSC as pll reference clk, output pll reference clk
	writel(0x00, addr + 0x20); //disable pll
	writel(0x04, addr);     //bypass PLL, Integer mode

	writel(0x02, addr + 0x04);  //M
	writel(val,    addr + 0x08);  //N
	writel(0x04, addr + 0x0C);  //P

	writel(0x02, addr + 0x1C);   //update and enable clk output
	writel(0x01, addr + 0x20); //enable pll

	while((val != 0x1) && (timeout--)){
		val = readl(addr + 0x28); //wait lock
	}

	if (!timeout) {
		log_i("pll timeout\n");
	}

	writel(0x0, addr);      //pll normal operation
	writel(0x2, addr + 0x24);//use OSC as pll reference clk, output pll clk
}

void imi_set_pll_freq(void)
{
	pll_set_freq(APLL, PLL_CLK_FREQ);
}

/******************************bus           clk********************************/

static void set_papb_freq(unsigned int freq, unsigned int source)
{
	unsigned int ratio = 1;

	writel(0x0, PAPB_CLK + 0xc); //load disable
	writel(source, PAPB_CLK);

	ratio = (PLL_CLK_FREQ / freq) - 1;

	writel(ratio, PAPB_CLK + 0x4);

	writel(0x1, PAPB_CLK + 0xc); //load enable
}

static void set_bus_freq(unsigned int base, unsigned int freq, unsigned int source)
{
	unsigned int ratio = 1;

	writel(0x1, base + 0xc); //load disable
	writel(source, base);

	ratio = (PLL_CLK_FREQ / freq) - 1;

	writel(ratio, base + 0x4);

	writel(0x0, base + 0xc); //load enable
}

static void set_usb_ref_clk(unsigned int base, unsigned int source)
{
	writel(0x1, base + 0xc); //load disable
	writel(source, base); //osc = 24M
	writel(0, base + 0x4); //ratio = 0, output = src / (ratio + 1), out = 24M
	writel(0x0, base + 0xc); //load enable
}

 void imi_set_bus_freq(void)
{
	set_papb_freq(PAPB_CLK_FREQ, SOURCE_APLL);
	set_usb_ref_clk(USB_REFCLK, SOURCE_OSC); //USB use OSC 24M
	set_bus_freq(BUS1_CLK, BUS1_CLK_FREQ, SOURCE_APLL);
	set_bus_freq(UART_ECLK, UART_EXT_CLK_FREQ, SOURCE_APLL);
	set_bus_freq(QSPI_REFCLK, QSPI_REF_CLK_FREQ, SOURCE_APLL);
}

void clock_dev_init(void)
{
	imi_set_pll_freq();
	imi_set_bus_freq();
}
#endif /*CHIP_IMI_1680*/
