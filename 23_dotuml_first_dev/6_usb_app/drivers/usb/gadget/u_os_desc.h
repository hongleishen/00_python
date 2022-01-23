/*
 * u_os_desc.h
 *
 * Utility definitions for "OS Descriptors" support
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *      http://www.samsung.com
 *
 * Author: Andrzej Pietrasiewicz <andrzej.p@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __U_OS_DESC_H__
#define __U_OS_DESC_H__
#include <linux_compat.h>

#define USB_EXT_PROP_DW_SIZE            0
#define USB_EXT_PROP_DW_PROPERTY_DATA_TYPE  4
#define USB_EXT_PROP_W_PROPERTY_NAME_LENGTH 8
#define USB_EXT_PROP_B_PROPERTY_NAME        10
#define USB_EXT_PROP_DW_PROPERTY_DATA_LENGTH    10
#define USB_EXT_PROP_B_PROPERTY_DATA        14

#define USB_EXT_PROP_RESERVED           0
#define USB_EXT_PROP_UNICODE            1
#define USB_EXT_PROP_UNICODE_ENV        2
#define USB_EXT_PROP_BINARY         3
#define USB_EXT_PROP_LE32           4
#define USB_EXT_PROP_BE32           5
#define USB_EXT_PROP_UNICODE_LINK       6
#define USB_EXT_PROP_UNICODE_MULTI      7

#define UNICODE_MAX 0x0010ffff
#define PLANE_SIZE  0x00010000

#define SURROGATE_MASK  0xfffff800
#define SURROGATE_PAIR  0x0000d800
#define SURROGATE_LOW   0x00000400
#define SURROGATE_BITS  0x000003ff

struct utf8_table {
	int     cmask;
	int     cval;
	int     shift;
	long    lmask;
	long    lval;
};
static const struct utf8_table utf8_table[] =
{
	{0x80,  0x00,   0*6,    0x7F,           0,         /* 1 byte sequence */},
	{0xE0,  0xC0,   1*6,    0x7FF,          0x80,      /* 2 byte sequence */},
	{0xF0,  0xE0,   2*6,    0xFFFF,         0x800,     /* 3 byte sequence */},
	{0xF8,  0xF0,   3*6,    0x1FFFFF,       0x10000,   /* 4 byte sequence */},
	{0xFC,  0xF8,   4*6,    0x3FFFFFF,      0x200000,  /* 5 byte sequence */},
	{0xFE,  0xFC,   5*6,    0x7FFFFFFF,     0x4000000, /* 6 byte sequence */},
	{0,                            /* end of table    */}
};

int utf8_to_utf32(const u8 *s, int inlen, unicode_t *pu)
{
	unsigned long l;
	int c0, c, nc;
	const struct utf8_table *t;

	nc = 0;
	c0 = *s;
	l = c0;
	for (t = utf8_table; t->cmask; t++) {
		nc++;
		if ((c0 & t->cmask) == t->cval) {
			l &= t->lmask;
			if (l < t->lval || l > UNICODE_MAX ||
					(l & SURROGATE_MASK) == SURROGATE_PAIR)
				return -1;
			*pu = (unicode_t) l;
			return nc;
		}
		if (inlen <= nc)
			return -1;
		s++;
		c = (*s ^ 0x80) & 0xFF;
		if (c & 0xC0)
			return -1;
		l = (l << 6) | c;
	}
	return -1;
}

static inline void put_utf16(wchar_t *s, unsigned c, enum utf16_endian endian)
{
	switch (endian) {
		default:
			*s = (wchar_t) c;
			break;
		case UTF16_LITTLE_ENDIAN:
			*s = cpu_to_le16(c);
			break;
		case UTF16_BIG_ENDIAN:
			//*s = cpu_to_be16(c);
			break;
	}
}

inline int utf8s_to_utf16s(const u8 *s, int inlen, enum utf16_endian endian,
		wchar_t *pwcs, int maxout)
{
	wchar_t *op;
	int size;
	unicode_t u;

	op = pwcs;
	while (inlen > 0 && maxout > 0 && *s) {
		if (*s & 0x80) {
			size = utf8_to_utf32(s, inlen, &u);
			if (size < 0)
				return -EINVAL;
			s += size;
			inlen -= size;

			if (u >= PLANE_SIZE) {
				if (maxout < 2)
					break;
				u -= PLANE_SIZE;
				put_utf16(op++, SURROGATE_PAIR |
						((u >> 10) & SURROGATE_BITS),
						endian);
				put_utf16(op++, SURROGATE_PAIR |
						SURROGATE_LOW |
						(u & SURROGATE_BITS),
						endian);
				maxout -= 2;
			} else {
				put_utf16(op++, u, endian);
				maxout--;
			}
		} else {
			put_utf16(op++, *s++, endian);
			inlen--;
			maxout--;
		}
	}
	return (int)(op - pwcs);
}


static inline u8 *__usb_ext_prop_ptr(u8 *buf, size_t offset)
{
	return buf + offset;
}

static inline u8 *usb_ext_prop_size_ptr(u8 *buf)
{
	return __usb_ext_prop_ptr(buf, USB_EXT_PROP_DW_SIZE);
}

static inline u8 *usb_ext_prop_type_ptr(u8 *buf)
{
	return __usb_ext_prop_ptr(buf, USB_EXT_PROP_DW_PROPERTY_DATA_TYPE);
}

static inline u8 *usb_ext_prop_name_len_ptr(u8 *buf)
{
	return __usb_ext_prop_ptr(buf, USB_EXT_PROP_W_PROPERTY_NAME_LENGTH);
}

static inline u8 *usb_ext_prop_name_ptr(u8 *buf)
{
	return __usb_ext_prop_ptr(buf, USB_EXT_PROP_B_PROPERTY_NAME);
}

static inline u8 *usb_ext_prop_data_len_ptr(u8 *buf, size_t off)
{
	return __usb_ext_prop_ptr(buf,
				  USB_EXT_PROP_DW_PROPERTY_DATA_LENGTH + off);
}

static inline u8 *usb_ext_prop_data_ptr(u8 *buf, size_t off)
{
	return __usb_ext_prop_ptr(buf, USB_EXT_PROP_B_PROPERTY_DATA + off);
}

static inline void usb_ext_prop_put_size(u8 *buf, int dw_size)
{
	put_unaligned_le32(dw_size, usb_ext_prop_size_ptr(buf));
}

static inline void usb_ext_prop_put_type(u8 *buf, int type)
{
	put_unaligned_le32(type, usb_ext_prop_type_ptr(buf));
}

static inline int usb_ext_prop_put_name(u8 *buf, const char *name, int pnl)
{
	int result;

	put_unaligned_le16(pnl, usb_ext_prop_name_len_ptr(buf));
	result = utf8s_to_utf16s((const u8 *)name, strlen(name), UTF16_LITTLE_ENDIAN,
		(wchar_t *) usb_ext_prop_name_ptr(buf), pnl - 2);
	if (result < 0)
		return result;

	put_unaligned_le16(0, &buf[USB_EXT_PROP_B_PROPERTY_NAME + pnl - 2]);

	return pnl;
}

static inline void usb_ext_prop_put_binary(u8 *buf, int pnl, const u8 *data,
					   int data_len)
{
	put_unaligned_le32(data_len, usb_ext_prop_data_len_ptr(buf, pnl));
	memcpy(usb_ext_prop_data_ptr(buf, pnl), data, data_len);
}

static inline int usb_ext_prop_put_unicode(u8 *buf, int pnl, const char *string,
					   int data_len)
{
	int result;
	put_unaligned_le32(data_len, usb_ext_prop_data_len_ptr(buf, pnl));
	result = utf8s_to_utf16s((const u8 *)string, data_len >> 1, UTF16_LITTLE_ENDIAN,
			(wchar_t *) usb_ext_prop_data_ptr(buf, pnl),
			data_len - 2);
	if (result < 0)
		return result;

	put_unaligned_le16(0,
		&buf[USB_EXT_PROP_B_PROPERTY_DATA + pnl + data_len - 2]);

	return data_len;
}

#endif /* __U_OS_DESC_H__ */
