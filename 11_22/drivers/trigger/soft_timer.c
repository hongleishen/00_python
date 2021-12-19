#if 0//defined(CHIP_IMI_2280)
#include "sensor_sync_2280.h"
#include "trigger.h"
#include "hellboy_map.h"
#include "gpio.h"
#include "pinmux.h"

#define SS_2280_DEV_NAME	"ss_2280"

static int ss_2280_power(void *ctrl, int onoff)
{
	struct snr_sync_hw *hw = (struct snr_sync_hw *)ctrl;
	//clk ops
	//reset ops

	//donot enable global trigger untill valid_sync
	if (!onoff) {
		write32(hw->base + 0x18, !!onoff);
	}
	return 0;
}

/*
static int ss_2280_reset(void *ctrl)
{
//	struct snr_sync_hw *hw = (struct snr_sync_hw *)ctrl;
	//reset ops
	return 0;
}
*/

static int ss_2280_enable(void *ctrl, int chan, int onoff)
{
	struct snr_sync_hw *hw = (struct snr_sync_hw *)ctrl;
	unsigned int base;
	if (chan & (1 << TRIG_CHAN_IR)) {
		base = (u32)hw->base + SS_2280_OFFSET(ss_2280_hw_snr);
	} else if (chan & (1 << TRIG_CHAN_FLOOD)) {
		base = (u32)hw->base + SS_2280_OFFSET(ss_2280_hw_flood);
	} else if (chan & (1 << TRIG_CHAN_DOT)) {
		base = (u32)hw->base + SS_2280_OFFSET(ss_2280_hw_dot);
	} else {
		log_e("chan %x invalid\n", chan);
		return -EINVAL;
	}

	write32(base + 0x8, !!onoff);
	return 0;
}

static int ss_2280_init(void *ctrl, void *data)
{	
	int ret = 0;
	struct snr_sync_hw *hw = (struct snr_sync_hw *)ctrl;
	int mode;
	memcpy((void*)&hw->tm, data, sizeof(struct trigger_timing));
	mode = hw->tm.trig_mode;
	ret |= ss_2280_chan_mode(ctrl, 1 << TRIG_CHAN_IR, mode);
	ret |= ss_2280_chan_mode(ctrl, 1 << TRIG_CHAN_FLOOD, mode);
	ret |= ss_2280_chan_mode(ctrl, 1 << TRIG_CHAN_DOT, mode);

	ret |= ss_2280_config(ctrl, 1 << TRIG_CHAN_IR,
			&hw->tm.path[TRIG_CHAN_IR]);
	ret |= ss_2280_config(ctrl, 1 << TRIG_CHAN_FLOOD,
			&hw->tm.path[TRIG_CHAN_FLOOD]);
	ret |= ss_2280_config(ctrl, 1 << TRIG_CHAN_DOT,
			&hw->tm.path[TRIG_CHAN_DOT]);

	return ret;
}

static int ss_2280_config(void *ctrl, int chan,
		struct trigger_period *pd)
{
	struct snr_sync_hw *hw = (struct snr_sync_hw *)ctrl;
	int reg;
	unsigned int base;
	//suntongce
//	if (pd->mode > TRIG_MODE_SINGLE) {
//
//	}

	if (!hw->tm.freq) {
		log_e("para freq invalid %d\n", hw->tm.freq);
		configASSERT(0);
		return -EINVAL;
	}

	if (chan & (1 << TRIG_CHAN_IR)) {
		base = (u32)hw->base + SS_2280_OFFSET(ss_2280_hw_snr);
	} else if (chan & (1 << TRIG_CHAN_FLOOD)) {
		base = (u32)hw->base + SS_2280_OFFSET(ss_2280_hw_flood);
	} else if (chan & (1 << TRIG_CHAN_DOT)) {
		base = (u32)hw->base + SS_2280_OFFSET(ss_2280_hw_dot);
	}
	write32((u32)hw->base + SS_2280_GLOBAL_EN(ss_2280_hw_snr),
			(GLOBAL_TRIG_CLK(hw->busclk) / hw->tm.freq - 1) &0xFFFFFF);
	write32(base + 0x8, 0);
	reg = 0;
	if (pd->T0.pulse) {
		reg = SS_PCLK_DIV | ((pd->T0.pulse * TU(hw->busclk) / 1000 -1) << 16) |
				(TRG_ATTR << 30);
	}
	write32(base, reg);
	reg = 0;
	if (pd->T0.delay) {
		reg = pd->T0.delay * TU(hw->busclk) / 1000 -1;
	}
	write32(base + 0x4, reg);
	return 0;
}

static int ss_2280_valid_sync(void *ctrl, int chan)
{
	struct snr_sync_hw *hw = (struct snr_sync_hw *)ctrl;
	switch (hw->tm.trig_mode) {
		case TRIG_MODE_AUTO:
			write32(hw->base + 0x18, 1);
			return 0;
		case TRIG_MODE_SINGLE:
			gpio_set_value(hw->ext_gpio, 1);
			gpio_set_value(hw->ext_gpio, 0);
			gpio_set_value(hw->ext_gpio, 1);
			gpio_set_value(hw->ext_gpio, 0);
			break;
		case TRIG_MODE_SW_SEPERATE:
			
			break;
	}
	return 0;
}

static int ss_2280_chan_mode(void *ctrl, int chan, int mode)
{
	struct snr_sync_hw *hw = (struct snr_sync_hw *)ctrl;
	if (chan & (1 << TRIG_CHAN_IR)) {
		if (mode < TRIG_MODE_SW_SEPERATE) {
			pinmux_set_mode(hw->ss0_gpio, 1);
		}
		else {
			pinmux_set_mode(hw->ss0_gpio, 0);
		}
//		hw->tm.path[TRIG_CHAN_IR].mode = mode;
	}
	if (chan & (1 << TRIG_CHAN_FLOOD)) {
		if (mode < TRIG_MODE_SW_SEPERATE) {
			pinmux_set_mode(hw->ss1_gpio, 1);
		}
		else {
			pinmux_set_mode(hw->ss1_gpio, 0);
		}
//		hw->tm.path[TRIG_CHAN_FLOOD].mode = mode;
	}
	if (chan & (1 << TRIG_CHAN_DOT)) {
		if (mode < TRIG_MODE_SW_SEPERATE) {
			pinmux_set_mode(hw->ss2_gpio, 1);
		}
		else {
			pinmux_set_mode(hw->ss2_gpio, 0);
		}
//		hw->tm.path[TRIG_CHAN_DOT].mode = mode;
	}
	return 0;
}

struct trigger_dev_ops trig_2280_ops = {
	.power = ss_2280_power,
	.enable = ss_2280_enable,
	.init = ss_2280_init,
	.config = ss_2280_config,
	.valid_sync = ss_2280_valid_sync,
	.chan_mode = ss_2280_chan_mode,
};

void ss_2280_probe(void)
{
	struct snr_sync_hw *sshw;
	struct trigger_handle *handle;

	sshw = pvPortMalloc(sizeof(*sshw));
	if (!sshw) {
		log_e("%s malloc failed\n", __func__);
		return;
	}
	memset(sshw, 0, sizeof(*sshw));
	sshw->base = (void __iomem *)DPU_BASE_ADDR;
	sshw->ss0_irq = 121;
	sshw->ss1_irq = 122;
	sshw->ss2_irq = 123;

	sshw->ss0_gpio = GPIO3;
	sshw->ss1_gpio = GPIO4;
	sshw->ss2_gpio = GPIO5;
	sshw->ext_gpio = GPIO6;

	sshw->busclk = 200000000;
	sshw->channel_bit = (1 << TRIG_CHAN_DOT) |
			(1 << TRIG_CHAN_FLOOD) | (1 << TRIG_CHAN_IR);
	sshw->trig_mode = (1 << TRIG_MODE_AUTO) |
					(1 << TRIG_MODE_SINGLE) |
					(1 << TRIG_MODE_SW_SEPERATE);	

	handle = trigger_register(&trig_2280_ops);
	handle->priv = (void*)sshw;
	handle->name = SS_2280_DEV_NAME;
}
DEV_INIT(ss_2280_probe);
#endif/*CHIP_IMI_2280*/
