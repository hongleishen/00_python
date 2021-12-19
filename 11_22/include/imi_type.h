/***************************************************************************
  Copyright (c) 2020 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief    General types uased by all
  @author   Dejie Zhang

  @History
  When         Who           What, where, why
  ----------   -----------   ----------------------------------------------
  2020/04/12   Dejie Zhang   the initial version

***************************************************************************/
#ifndef __IMI_TYPE_H__
#define __IMI_TYPE_H__

#include <stddef.h>

typedef unsigned char       	u8;
typedef unsigned short      	u16;
typedef unsigned int        	u32;
typedef unsigned long long  	u64;

typedef char                	int8;
typedef short               	int16;
typedef int                 	int32;
typedef long long           	int64;

typedef unsigned char       	uint8;
typedef unsigned short      	uint16;
typedef unsigned int        	uint32;
typedef unsigned long long  	uint64;

typedef unsigned char       	uint8_t;
typedef unsigned short      	uint16_t;
typedef unsigned long long  	uint64_t;

typedef unsigned char       	U8;
typedef unsigned short      	U16;
typedef unsigned int        	U32;
typedef unsigned long long  	U64;

typedef char		        	S8;
typedef short		        	S16;
typedef int		        		S32;
typedef long long	        	S64;

typedef unsigned char	        uc;
//typedef unsigned int	        size_t;
typedef unsigned long	        ul;
typedef unsigned long long      ull;
typedef unsigned long volatile	ulv;
typedef unsigned char volatile	u8v;
typedef unsigned short volatile	u16v;

typedef char 					BOOL;
typedef long					INT32;
typedef short					INT16;
typedef char					INT8;

typedef unsigned char       	__u8;
typedef unsigned short      	__u16;
typedef unsigned int			__u32;
typedef unsigned long long		__u64;

typedef unsigned long			ulong;

#define bool					_Bool

#ifndef NULL
#define NULL					((void *)0)
#endif

#ifndef VOID
#define VOID					void
#endif

#define FAIL					(-1)
#define SUCCESS 				(0)

#define FALSE					(0)
#define TRUE					(1)

#ifndef true
#define true      				(1)
#endif

#ifndef false
#define false     				(0)
#endif

#define uswap_32(x)	\
	((((x) & 0xff000000) >>	24) | \
	 (((x) & 0x00ff0000) >>	 8) | \
	 (((x) & 0x0000ff00) <<	 8) | \
	 (((x) & 0x000000ff) <<	24))

#define ntohl(x)		uswap_32(x) /* change to machine byte order */

#ifndef __packed
#define __packed __attribute__((packed))
#endif

#define MIN(a, b)               (((a) < (b)) ? (a) : (b))

int memcmp(const void * cs,const void * ct,size_t count);

#define SETREG32(reg,val) ( (*(volatile unsigned int *) (reg)) = ((unsigned int) (val)) )
#define GETREG32(reg)     ( *((volatile unsigned int *) (reg)) )

typedef struct dev
{
	char*  dev_name;
	unsigned char   flag;
	unsigned int    clk_address;
	void*           ops;
}DEV;

#endif
