#ifndef __PINMUX_H__
#define __PINMUX_H__
#include "imi_io.h"
#if defined(CHIP_IMI_2280)
#include "pinmux_hw_2280.h"
#elif defined(CHIP_IMI_1680)
#include "pinmux_hw_1680.h"
#endif

int pinmux_get_addr(int pin);
int pinmux_get_mask(int pin);
int pinmux_set_mode(int pin, int mode);
#endif
