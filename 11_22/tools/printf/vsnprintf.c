#include "imi_io.h"
#include <stdlib.h>
#include "imi_type.h"
#include "printf.h"
#include "string.h"
#include "div64.h"
#include "ctype.h"


#define noinline __attribute__((noinline))
#define MAX_UTF8_PER_UTF16 3
size_t utf16_strlen(const u16 *in)
{
	size_t i;
	for (i = 0; in[i]; i++);
	return i;
}
size_t utf16_strnlen(const u16 *in, size_t count)
{
	size_t i;
	for (i = 0; count-- && in[i]; i++);
	return i;
}
u8 *utf16_to_utf8(u8 *dest, const u16 *src, size_t size)
{
	u32 code_high = 0;

	while (size--) {
		u32 code = *src++;

		if (code_high) {
			if (code >= 0xDC00 && code <= 0xDFFF) {
				/* Surrogate pair.  */
				code = ((code_high - 0xD800) << 10) + (code - 0xDC00) + 0x10000;

				*dest++ = (code >> 18) | 0xF0;
				*dest++ = ((code >> 12) & 0x3F) | 0x80;
				*dest++ = ((code >> 6) & 0x3F) | 0x80;
				*dest++ = (code & 0x3F) | 0x80;
			} else {
				/* Error...  */
				*dest++ = '?';
				/* *src may be valid. Don't eat it.  */
				src--;
			}

			code_high = 0;
		} else {
			if (code <= 0x007F) {
				*dest++ = code;
			} else if (code <= 0x07FF) {
				*dest++ = (code >> 6) | 0xC0;
				*dest++ = (code & 0x3F) | 0x80;
			} else if (code >= 0xD800 && code <= 0xDBFF) {
				code_high = code;
				continue;
			} else if (code >= 0xDC00 && code <= 0xDFFF) {
				/* Error... */
				*dest++ = '?';
			} else if (code < 0x10000) {
				*dest++ = (code >> 12) | 0xE0;
				*dest++ = ((code >> 6) & 0x3F) | 0x80;
				*dest++ = (code & 0x3F) | 0x80;
			} else {
				*dest++ = (code >> 18) | 0xF0;
				*dest++ = ((code >> 12) & 0x3F) | 0x80;
				*dest++ = ((code >> 6) & 0x3F) | 0x80;
				*dest++ = (code & 0x3F) | 0x80;
			}
		}
	}

	return dest;
}
u16 *utf8_to_utf16(u16 *dest, const u8 *src, size_t size)
{
	while (size--) {
		int extension_bytes;
		u32 code;

		extension_bytes = 0;
		if (*src <= 0x7f) {
			code = *src++;
			/* Exit on zero byte */
			if (!code)
				size = 0;
		} else if (*src <= 0xbf) {
			/* Illegal code */
			code = '?';
		} else if (*src <= 0xdf) {
			code = *src++ & 0x1f;
			extension_bytes = 1;
		} else if (*src <= 0xef) {
			code = *src++ & 0x0f;
			extension_bytes = 2;
		} else if (*src <= 0xf7) {
			code = *src++ & 0x07;
			extension_bytes = 3;
		} else {
			/* Illegal code */
			code = '?';
		}

		for (; extension_bytes && size; --size, --extension_bytes) {
			if ((*src & 0xc0) == 0x80) {
				code <<= 6;
				code |= *src++ & 0x3f;
			} else {
				/* Illegal code */
				code = '?';
				++src;
				--size;
				break;
			}
		}

		if (code < 0x10000) {
			*dest++ = code;
		} else {
			/*
			 * Simplified expression for
			 * (((code - 0x10000) >> 10) & 0x3ff) | 0xd800
			 */
			*dest++ = (code >> 10) + 0xd7c0;
			*dest++ = (code & 0x3ff) | 0xdc00;
		}
	}
	return dest;
}


#ifndef __div64_const32_is_OK
#define __div64_const32_is_OK (__GNUC__ >= 4)
#endif

#ifndef __div64_32
u32 __attribute__((weak)) __div64_32(u64 *n, u32 base)
{
	u64 rem = *n;
	u64 b = base;
	u64 res, d = 1;
	u64 high = rem >> 32;

	/* Reduce the thing a bit first */
	res = 0;
	if (high >= base) {
		high /= base;
		res = (u64) high << 32;
		rem -= (u64) (high*base) << 32;
	}

	while ((int64)b > 0 && b < rem) {
		b = b+b;
		d = d+d;
	}

	do {
		if (rem >= b) {
			rem -= b;
			res += d;
		}
		b >>= 1;
		d >>= 1;
	} while (d);

	*n = res;
	return rem;
}

#endif

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)

#ifndef is_power_of_2
bool inline is_power_of_2(int n) 
{
	if (n < 1)
		return false;
	return ((n & (n -1)) == 0);
}
#endif

#ifndef ilog2
inline unsigned int ilog2(unsigned int v)
{
	unsigned int r;
	unsigned int shift;
	r = (v > 0xffff) << 4; v >>= r;
	shift = (v > 0xff) << 3; v >>= shift; r |= shift;
	shift = (v > 0xf) << 2; v >>= shift; r |= shift;
	shift = (v > 0x3) << 1; v >>= shift; r |= shift;
	r |= (v >> 1);
	return r;
}
#endif

#define do_div(n,base) ({				\
	U32 __base = (base);			\
	u32 __rem;					\
	(void)(((typeof((n)) *)0) == ((u64 *)0));	\
	if (__builtin_constant_p(__base) &&		\
	    is_power_of_2(__base)) {			\
		__rem = (n) & (__base - 1);		\
		(n) >>= ilog2(__base);			\
	} else if (__div64_const32_is_OK &&		\
		   __builtin_constant_p(__base) &&	\
		   __base != 0) {			\
		u32 __res_lo, __n_lo = (n);	\
		(n) = __div64_const32(n, __base);	\
		/* the remainder can be computed with 32-bit regs */ \
		__res_lo = (n);				\
		__rem = __n_lo - __res_lo * __base;	\
	} else if (likely(((n) >> 32) == 0)) {		\
		__rem = (u32)(n) % __base;		\
		(n) = (u32)(n) / __base;		\
	} else 						\
		__rem = __div64_32(&(n), __base);	\
	__rem;						\
 })

static char *put_dec_trunc(char *buf, unsigned q)
{
	unsigned d3, d2, d1, d0;
	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	d0 = 6*(d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0'; /* least significant digit */
	d1 = q + 9*d3 + 5*d2 + d1;
	if (d1 != 0) {
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10*q;
		*buf++ = d1 + '0'; /* next digit */

		d2 = q + 2*d2;
		if ((d2 != 0) || (d3 != 0)) {
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10*q;
			*buf++ = d2 + '0'; /* next digit */

			d3 = q + 4*d3;
			if (d3 != 0) {
				q = (d3 * 0xcd) >> 11;
				d3 = d3 - 10*q;
				*buf++ = d3 + '0';  /* next digit */
				if (q != 0)
					*buf++ = q + '0'; /* most sign. digit */
			}
		}
	}
	return buf;
}

static char *put_dec_full(char *buf, unsigned q)
{
	/* BTW, if q is in [0,9999], 8-bit ints will be enough, */
	/* but anyway, gcc produces better code with full-sized ints */
	unsigned d3, d2, d1, d0;
	d1 = (q>>4) & 0xf;
	d2 = (q>>8) & 0xf;
	d3 = (q>>12);

	/*
	 * Possible ways to approx. divide by 10
	 * gcc -O2 replaces multiply with shifts and adds
	 * (x * 0xcd) >> 11: 11001101 - shorter code than * 0x67 (on i386)
	 * (x * 0x67) >> 10:  1100111
	 * (x * 0x34) >> 9:    110100 - same
	 * (x * 0x1a) >> 8:     11010 - same
	 * (x * 0x0d) >> 7:      1101 - same, shortest code (on i386)
	 */

	d0 = 6*(d3 + d2 + d1) + (q & 0xf);
	q = (d0 * 0xcd) >> 11;
	d0 = d0 - 10*q;
	*buf++ = d0 + '0';
	d1 = q + 9*d3 + 5*d2 + d1;
		q = (d1 * 0xcd) >> 11;
		d1 = d1 - 10*q;
		*buf++ = d1 + '0';

		d2 = q + 2*d2;
			q = (d2 * 0xd) >> 7;
			d2 = d2 - 10*q;
			*buf++ = d2 + '0';

			d3 = q + 4*d3;
				q = (d3 * 0xcd) >> 11; /* - shorter code */
				/* q = (d3 * 0x67) >> 10; - would also work */
				d3 = d3 - 10*q;
				*buf++ = d3 + '0';
					*buf++ = q + '0';
	return buf;
}

static noinline char *put_dec(char *buf, u64 num)
{
	while (1) {
		unsigned rem;
		if (num < 100000)
			return put_dec_trunc(buf, num);
		rem = do_div(num, 100000);
		buf = put_dec_full(buf, rem);
	}
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SMALL	32		/* Must be 32 == 0x20 */
#define SPECIAL	64		/* 0x */
#define is_digit(c)	((c) >= '0' && (c) <= '9')

#define ADDCH(str, ch) do { \
	if ((str) < end) \
		*(str) = (ch); \
	++str; \
	} while (0)

typedef __signed__ long long s64;
static char *number(char *buf, char *end, u64 num,
		int base, int size, int precision, int type)
{
	/* we are called with base 8, 10 or 16, only, thus don't need "G..."  */
	static const char digits[16] = "0123456789ABCDEF";

	char tmp[66];
	char sign;
	char locase;
	int need_pfx = ((type & SPECIAL) && base != 10);
	int i;

	/* locase = 0 or 0x20. ORing digits or letters with 'locase'
	 * produces same digits or (maybe lowercased) letters */
	locase = (type & SMALL);
	if (type & LEFT)
		type &= ~ZEROPAD;
	sign = 0;
	if (type & SIGN) {
		if ((s64) num < 0) {
			sign = '-';
			num = -(s64) num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (need_pfx) {
		size--;
		if (base == 16)
			size--;
	}

	/* generate full string in tmp[], in reverse order */
	i = 0;
	if (num == 0)
		tmp[i++] = '0';
	/* Generic code, for any base:
	else do {
		tmp[i++] = (digits[do_div(num,base)] | locase);
	} while (num != 0);
	*/
	else if (base != 10) { /* 8 or 16 */
		int mask = base - 1;
		int shift = 3;

		if (base == 16)
			shift = 4;

		do {
			tmp[i++] = (digits[((unsigned char)num) & mask]
					| locase);
			num >>= shift;
		} while (num);
	} else { /* base 10 */
		i = put_dec(tmp, num) - tmp;
	}

	/* printing 100 using %2d gives "100", not "00" */
	if (i > precision)
		precision = i;
	/* leading space padding */
	size -= precision;
	if (!(type & (ZEROPAD + LEFT))) {
		while (--size >= 0)
			ADDCH(buf, ' ');
	}
	/* sign */
	if (sign)
		ADDCH(buf, sign);
	/* "0x" / "0" prefix */
	if (need_pfx) {
		ADDCH(buf, '0');
		if (base == 16)
			ADDCH(buf, 'X' | locase);
	}
	/* zero or space padding */
	if (!(type & LEFT)) {
		char c = (type & ZEROPAD) ? '0' : ' ';

		while (--size >= 0)
			ADDCH(buf, c);
	}
	/* hmm even more zero padding? */
	while (i <= --precision)
		ADDCH(buf, '0');
	/* actual digits of result */
	while (--i >= 0)
		ADDCH(buf, tmp[i]);
	/* trailing space padding */
	while (--size >= 0)
		ADDCH(buf, ' ');
	return buf;
}


static int skip_atoi(const char **s)
{
	int i = 0;

	while (is_digit(**s))
		i = i * 10 + *((*s)++) - '0';

	return i;
}

static char *string(char *buf, char *end, char *s, int field_width,
		int precision, int flags)
{
	int len, i;

	if (s == NULL)
		s = "<NULL>";

	len = strnlen(s, precision);

	if (!(flags & LEFT))
		while (len < field_width--)
			ADDCH(buf, ' ');
	for (i = 0; i < len; ++i)
		ADDCH(buf, *s++);
	while (len < field_width--)
		ADDCH(buf, ' ');
	return buf;
}


static char *string16(char *buf, char *end, u16 *s, int field_width,
		int precision, int flags)
{
	u16 *null_str = (u16 *)"NULL";
	u16 *str = s ? s : null_str;
	int utf16_len = utf16_strnlen(str, precision);
	u8 utf8[utf16_len * MAX_UTF8_PER_UTF16];
	int utf8_len, i;

	utf8_len = utf16_to_utf8(utf8, str, utf16_len) - utf8;

	if (!(flags & LEFT))
		while (utf8_len < field_width--)
			ADDCH(buf, ' ');
	for (i = 0; i < utf8_len; ++i)
		ADDCH(buf, utf8[i]);
	while (utf8_len < field_width--)
		ADDCH(buf, ' ');
	return buf;
}

#define config_enabled(cfg) _config_enabled(cfg)

#define IS_ENABLED(option) \
	(config_enabled(option) || config_enabled(option##_MODULE))
/*
#if defined(CONFIG_EFI_LOADER) && \
	!defined(CONFIG_SPL_BUILD) && !defined(API_BUILD)
*/
#define MAX_ERRNO	4095

#define IS_ERR_VALUE(x) unlikely((x) >= (unsigned long)-MAX_ERRNO)

static inline long IS_ERR(const void *ptr)
{
	return IS_ERR_VALUE((unsigned long)ptr);
}

static inline long PTR_ERR(const void *ptr)
{
	return (long) ptr;
}

typedef unsigned long int uintptr_t;

static char *pointer(const char *fmt, char *buf, char *end, void *ptr,
		int field_width, int precision, int flags)
{
	u64 num = (uintptr_t)ptr;

	switch (*fmt) {
#if defined(CONFIG_EFI_LOADER) && \
	!defined(CONFIG_SPL_BUILD) && !defined(API_BUILD)
	case 'D':
		return device_path_string(buf, end, ptr, field_width,
					  precision, flags);
#endif
#ifdef CONFIG_CMD_NET
	case 'a':
		flags |= SPECIAL | ZEROPAD;

		switch (fmt[1]) {
		case 'p':
		default:
			field_width = sizeof(phys_addr_t) * 2 + 2;
			num = *(phys_addr_t *)ptr;
			break;
		}
		break;
	case 'm':
		flags |= SPECIAL;
		/* Fallthrough */
	case 'M':
		return mac_address_string(buf, end, ptr, field_width,
					  precision, flags);
	case 'i':
		flags |= SPECIAL;
		/* Fallthrough */
	case 'I':
		if (fmt[1] == '6')
			return ip6_addr_string(buf, end, ptr, field_width,
					       precision, flags);
		if (fmt[1] == '4')
			return ip4_addr_string(buf, end, ptr, field_width,
					       precision, flags);
		flags &= ~SPECIAL;
		break;
#endif
#ifdef CONFIG_LIB_UUID
	case 'U':
		return uuid_string(buf, end, ptr, field_width, precision,
				   flags, fmt);
#endif
	default:
		break;
	}
	flags |= SMALL;
	if (field_width == -1) {
		field_width = 2*sizeof(void *);
		flags |= ZEROPAD;
	}
	return number(buf, end, num, 16, field_width, precision, flags);
}


static int vsnprintf_internal(char *buf, size_t size, const char *fmt,
			      va_list args)
{
	u64 num;
	int base;
	char *str;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */
				/* 'z' support added 23/7/1999 S.H.    */
				/* 'z' changed to 'Z' --davidm 1/25/99 */
				/* 't' added for ptrdiff_t */
	char *end = buf + size;

	/* Make sure end is always >= buf - do we want this in U-Boot? */
	if (end < buf) {
		end = ((void *)-1);
		size = end - buf;
	}
	str = buf;

	for (; *fmt ; ++fmt) {
		if (*fmt != '%') {
			ADDCH(str, *fmt);
			continue;
		}

		/* process flags */
		flags = 0;
repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
			case '-':
				flags |= LEFT;
				goto repeat;
			case '+':
				flags |= PLUS;
				goto repeat;
			case ' ':
				flags |= SPACE;
				goto repeat;
			case '#':
				flags |= SPECIAL;
				goto repeat;
			case '0':
				flags |= ZEROPAD;
				goto repeat;
			}

		/* get field width */
		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' ||
		    *fmt == 'Z' || *fmt == 'z' || *fmt == 't') {
			qualifier = *fmt;
			++fmt;
			if (qualifier == 'l' && *fmt == 'l') {
				qualifier = 'L';
				++fmt;
			}
		}

		/* default base */
		base = 10;

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT)) {
				while (--field_width > 0)
					ADDCH(str, ' ');
			}
			ADDCH(str, (unsigned char) va_arg(args, int));
			while (--field_width > 0)
				ADDCH(str, ' ');
			continue;

		case 's':
			//if (qualifier == 'l' && !IS_ENABLED(CONFIG_SPL_BUILD)) {
			if (qualifier == 'l') {
				str = string16(str, end, va_arg(args, u16 *),
					       field_width, precision, flags);
			} else {
				str = string(str, end, va_arg(args, char *),
					     field_width, precision, flags);
			}
			continue;

		case 'p':
			str = pointer(fmt + 1, str, end,
					va_arg(args, void *),
					field_width, precision, flags);
			if (IS_ERR(str))
				return PTR_ERR(str);
			/* Skip all alphanumeric pointer suffixes */
			while (isalnum(fmt[1]))
				fmt++;
			continue;

		case 'n':
			if (qualifier == 'l') {
				long *ip = va_arg(args, long *);
				*ip = (str - buf);
			} else {
				int *ip = va_arg(args, int *);
				*ip = (str - buf);
			}
			continue;

		case '%':
			ADDCH(str, '%');
			continue;

		/* integer number formats - set up the flags and "break" */
		case 'o':
			base = 8;
			break;

		case 'x':
			flags |= SMALL;
		case 'X':
			base = 16;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			break;

		default:
			ADDCH(str, '%');
			if (*fmt)
				ADDCH(str, *fmt);
			else
				--fmt;
			continue;
		}
		if (qualifier == 'L')  /* "quad" for 64 bit variables */
			num = va_arg(args, unsigned long long);
		else if (qualifier == 'l') {
			num = va_arg(args, unsigned long);
			if (flags & SIGN)
				num = (signed long) num;
		} else if (qualifier == 'Z' || qualifier == 'z') {
			num = va_arg(args, size_t);
		} else if (qualifier == 't') {
			num = va_arg(args, ptrdiff_t);
		} else if (qualifier == 'h') {
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (signed short) num;
		} else {
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int) num;
		}
		str = number(str, end, num, base, field_width, precision,
			     flags);
	}

	if (size > 0) {
		ADDCH(str, '\0');
		if (str > end)
			end[-1] = '\0';
		--str;
	}
	/* the trailing null byte doesn't count towards the total */
	return str - buf;
}

int imi_vsnprintf(char *buf, size_t size, const char *fmt,
			      va_list args)
{
	return vsnprintf_internal(buf, size, fmt, args);
}


int snprintf(char *str, size_t size, const char *format, ...)
{
    va_list ap;
    int n = 0;
    va_start(ap, format); 
    n=imi_vsnprintf (str, size, format, ap); 
    va_end(ap); 
    return n;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
	return imi_vsnprintf(buf, (~0U)>>1, fmt, args);
}
