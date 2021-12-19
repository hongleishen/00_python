#ifndef _DELAY_H
#define _DELAY_H

#include "imi_type.h"

void delayus(unsigned long usec);
void delayms(unsigned int msdelay);
void imi_delay(unsigned long cycles);
void udelay(unsigned long cycles);
void mdelay(unsigned long cycles);

#endif
