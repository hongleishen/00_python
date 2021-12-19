/***************************************************************************
  Copyright (c) 2020 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    General io functions
  @author   Dejie Zhang

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------
  2020/04/12   Dejie Zhang   the initial version

***************************************************************************/
#ifndef __IMI_IO_H___
#define __IMI_IO_H___

#include "stdarg.h"
#include "imi_type.h"

#define BIT(x) (1UL << (x))

#define read8(addr)			(*((volatile unsigned char  *)(addr)))
#define read16(addr)		(*((volatile unsigned short *)(addr)))
#define read32(addr)		(*((volatile unsigned int   *)(addr)))
#define readl(addr)			(*((volatile unsigned int   *)(addr)))

#define write8(addr,data)	do{*((volatile unsigned char  *)(addr)) = (data);}while(0)
#define write16(addr,data)	do{*((volatile unsigned short *)(addr)) = (data);}while(0)
#define write32(addr,data)	do{*((volatile unsigned int   *)(addr)) = (data);}while(0)
#define writel(data,addr)	do{*((volatile unsigned int   *)(addr)) = (data);}while(0)

#define __arch_getb(a)          (*(volatile unsigned char *)(a))
#define __arch_getw(a)          (*(volatile unsigned short *)(a))
#define __arch_getl(a)          (*(volatile unsigned int *)(a))
#define __arch_getq(a)          (*(volatile unsigned long long *)(a))

#define __arch_putb(v,a)        (*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)        (*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)        (*(volatile unsigned int *)(a) = (v))
#define __arch_putq(v,a)        (*(volatile unsigned long long *)(a) = (v))

static inline void __raw_writesb(unsigned long addr, const void *data,
				 int bytelen)
{
	unsigned char *buf = (unsigned char *)data;
	while(bytelen--)
		__arch_putb(*buf++, addr);
}

static inline void __raw_writesw(unsigned long addr, const void *data,
				 int wordlen)
{
	unsigned short *buf = (unsigned short *)data;
	while(wordlen--)
		__arch_putw(*buf++, addr);
}

static inline void __raw_writesl(unsigned long addr, const void *data,
				 int longlen)
{
	unsigned int *buf = (unsigned int *)data;
	while(longlen--)
		__arch_putl(*buf++, addr);
}

static inline void __raw_readsb(unsigned long addr, void *data, int bytelen)
{
	unsigned char *buf = (unsigned char *)data;
	while(bytelen--)
		*buf++ = __arch_getb(addr);
}

static inline void __raw_readsw(unsigned long addr, void *data, int wordlen)
{
	unsigned short *buf = (unsigned short *)data;
	while(wordlen--)
		*buf++ = __arch_getw(addr);
}

static inline void __raw_readsl(unsigned long addr, void *data, int longlen)
{
	unsigned int *buf = (unsigned int *)data;
	while(longlen--)
		*buf++ = __arch_getl(addr);
}

#define __raw_writeb(v,a)   __arch_putb(v,a)
#define __raw_writew(v,a)   __arch_putw(v,a)
#define __raw_writel(v,a)   __arch_putl(v,a)
#define __raw_writeq(v,a)   __arch_putq(v,a)

#define __raw_readb(a)      __arch_getb(a)
#define __raw_readw(a)      __arch_getw(a)
#define __raw_readl(a)      __arch_getl(a)
#define __raw_readq(a)      __arch_getq(a)

#define outsb(p,d,l)            __raw_writesb((p),d,l)
#define outsw(p,d,l)            __raw_writesw((p),d,l)
#define outsl(p,d,l)            __raw_writesl((p),d,l)

#define insb(p,d,l)         __raw_readsb((p),d,l)
#define insw(p,d,l)         __raw_readsw((p),d,l)
#define insl(p,d,l)         __raw_readsl((p),d,l)

static inline void readsl(const void  *addr, void *buf, int len)
	{ insl((unsigned long)addr, buf, len); }
static inline void readsw(const void  *addr, void *buf, int len)
	{ insw((unsigned long)addr, buf, len); }
static inline void readsb(const void  *addr, void *buf, int len)
	{ insb((unsigned long)addr, buf, len); }

static inline void writesl(const void  *addr, const void *buf, int len)
	{ outsl((unsigned long)addr, buf, len); }
static inline void writesw(const void  *addr, const void *buf, int len)
	{ outsw((unsigned long)addr, buf, len); }
static inline void writesb(const void  *addr, const void *buf, int len)
	{ outsb((unsigned long)addr, buf, len); }


int printf(const char *fmt, ...);
int imi_printf(const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int snprintf(char *str, size_t size, const char *format, ...);
int imi_vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
void ddr_init(void);
void vector_addr_init(void);

#if DEBUG_OPEN
#define dbg(format, ...) imi_printf("[dbg: %s, %d ]" format, __func__, __LINE__, ##__VA_ARGS__)
#else
#define dbg(format, ...)
#endif


#endif
