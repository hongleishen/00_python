#include "imi_io.h"
#include <string.h>
#include <ctype.h>

#define LONG_MAX     2147483647L  /*0x7FFFFFFF*/
#define LONG_MIN     (-2147483647L-1L) /*-0x80000000*/
char * ___strtok;

void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = s;
	while (n-- != 0) {
		if ((unsigned char)c == *p++) {
			return (void *)(p-1);
		}
	}
	return NULL;
}

int memcmp(const void * cs,const void * ct,size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

#ifdef TARGET_FPGA_SIMULATION
//#define cmd_set 			0x08020000
//#define memset_addr			0x08020004
//#define memset_value		0x08020008
//#define memset_len			0x0802000C
//#define memset_set_value	0x08020010
//#define memcpy_addr			0x08020004
//#define memcpy_source_addr	0x08020008
//#define memcpy_len			0x0802000C
//#define memdump_addr		0x08020004
//#define memdump_len			0x08020008
//#define memdump_set			0x0802000C

#define cmd_set 			0x08010000
#define memset_addr			(cmd_set + 0x04)
#define memset_value		(cmd_set + 0x08)
#define memset_len			(cmd_set + 0x0c)
#define memset_set_value	(cmd_set + 0x10)
#define memcpy_addr			(cmd_set + 0x04)
#define memcpy_source_addr	(cmd_set + 0x08)
#define memcpy_len			(cmd_set + 0x0c)
#define memdump_addr		(cmd_set + 0x04)
#define memdump_len			(cmd_set + 0x08)
#define memdump_set			(cmd_set + 0x0c)


void * memcpy(void *dest, const void *src, size_t count)
{
	writel((int)dest, memcpy_addr);
	writel((int)src, memcpy_source_addr);
	writel(count, memcpy_len);
	writel(0x77, cmd_set);
	return dest;
}

//Memory dump
void memory_dump(void * s, size_t count, size_t number)
{
	writel((int)s, memdump_addr);
	writel(count, memdump_len);
	writel(number, memdump_set);
	writel(0x75, cmd_set);
}

void simu_start(void)
{
	writel(0x85, cmd_set);
}

void program_pass(void)
{
	writel(0x50, cmd_set);
}

void program_fail(void)
{
	writel(0x46, cmd_set);
}

void program_need_check(void)
{
	writel(0x26, cmd_set);
}
#else
void * memcpy(void *dest, const void *src, size_t count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* while all data is aligned (common case), copy a word at a time */
	if ( (((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
		while (count >= sizeof(*dl)) {
			*dl++ = *sl++;
			count -= sizeof(*dl);
		}
	}
	/* copy the reset one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (count--)
		*d8++ = *s8++;

	return dest;
}
#endif

void * memmove(void * dest,const void *src,size_t count)
{
	char *tmp, *s;

	if (dest <= src) {
		memcpy(dest, src, count);
	} else {
		tmp = (char *) dest + count;
		s = (char *) src + count;
		while (count--)
			*--tmp = *--s;
		}

	return dest;
}

#ifdef TARGET_FPGA_SIMULATION
void * memset(void * s,int c,size_t count)
{
	writel((int)s, memset_addr);
	writel(count, memset_len);
	writel(c, memset_value);
	writel(0x0, memset_set_value);
	writel(0x70, cmd_set);

	return s;
}
#else
void * memset(void * s,int c,size_t count)
{
	char *tmps= (char *)s;

	while (count--) {
		*tmps++ = (char)c;
	}

	return s;
}
#endif

char * strcat(char * dest, const char * src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;

	return tmp;
}

char * strchr(const char * s, int c)
{
	for(; *s != (char) c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *) s;
}

int strcmp(const char * cs,const char * ct)
{
	register signed char __res;

	while (1) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
	}

	return __res;
}

char * strcpy(char * dest,const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}
	return ret;
}

size_t strcspn(const char *s, const char *reject)
{
	const char *p;
	const char *r;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (r = reject; *r != '\0'; ++r) {
			if (*p == *r)
				return count;
		}
		++count;
	}
	return count;
}

size_t strlen(const char * s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

char * strncat(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++)) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}

	return tmp;
}

int strncmp(const char * cs,const char * ct,size_t count)
{
	register signed char __res = 0;

	while (count) {
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
		count--;
	}

	return __res;
}

char * strncpy(char * dest,const char *src,size_t count)
{
	char *tmp = dest;

	while (count-- && (*dest++ = *src++) != '\0')
		/* nothing */;

	return tmp;
}

char * strpbrk(const char * cs,const char * ct)
{
	const char *sc1,*sc2;

	for( sc1 = cs; *sc1 != '\0'; ++sc1) {
		for( sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *) sc1;
		}
	}
	return NULL;
}

char * strrchr(const char * s, int c)
{
       const char *p = s + strlen(s);
       do {
	   if (*p == (char)c)
	       return (char *)p;
       } while (--p >= s);
       return NULL;
}

size_t strspn(const char *s, const char *accept)
{
	const char *p;
	const char *a;
	size_t count = 0;

	for (p = s; *p != '\0'; ++p) {
		for (a = accept; *a != '\0'; ++a) {
			if (*p == *a)
				break;
		}
		if (*a == '\0')
			return count;
		++count;
	}

	return count;
}

char * strstr(const char * s1,const char * s2)
{
	int l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *) s1;
	l1 = strlen(s1);
	while (l1 >= l2) {
		l1--;
		if (!memcmp(s1,s2,l2))
			return (char *) s1;
		s1++;
	}
	return NULL;
}

char * strtok(char * s,const char * ct)
{
	char *sbegin, *send;

	sbegin  = s ? s : ___strtok;
	if (!sbegin) {
		return NULL;
	}
	sbegin += strspn(sbegin,ct);
	if (*sbegin == '\0') {
		___strtok = NULL;
		return( NULL );
	}
	send = strpbrk( sbegin, ct);
	if (send && *send != '\0')
		*send++ = '\0';
	___strtok = send;
	return (sbegin);
}

char *strtok_r(char *s, const char *delim, char **save_ptr)
{
	char *token;

	if (s == NULL)
		s = *save_ptr;

	/* Scan leading delimiters.  */
	s += strspn(s, delim);
	if (*s == '\0') {
		*save_ptr = s;
		return NULL;
	}

	/* Find the end of the token.  */
	token = s;
	s = strpbrk (token, delim);
	if (s == NULL) {
		/* This token finishes the string.  */
		*save_ptr = memchr(token, '\0', strlen(token));
	} else {
		/* Terminate the token and make *SAVE_PTR point past it.  */
		*s = '\0';
		*save_ptr = s + 1;
	}
	return token;
}

#define TOLOWER(x) ((x) | 0x20)
#if 0
#define isxdigit(c)    (('0' <= (c) && (c) <= '9') \
					|| ('a' <= (c) && (c) <= 'f') \
					|| ('A' <= (c) && (c) <= 'F'))
#define isdigit(c)    (('0' <= (c) && (c) <= '9'))
#endif
static unsigned int simple_guess_base(const char *cp)
{
	if (cp[0] == '0') {
		if (TOLOWER(cp[1]) == 'x' && isxdigit(cp[2]))
			return 16;
		else
			return 8;
	} else {
		return 10;
	}
}

ulong simple_strtoul(const char *cp, char **endp, unsigned int base)
{
	ulong result = 0;

	if (!base)
		base = simple_guess_base(cp);

	if (base == 16 && cp[0] == '0' && TOLOWER(cp[1]) == 'x')
		cp += 2;

	while (isxdigit(*cp)) {
		unsigned int value;

		value = isdigit(*cp) ? *cp - '0' : TOLOWER(*cp) - 'a' + 10;
		if (value >= base)
			break;
		result = result * base + value;
		cp++;
	}

	if (endp)
		*endp = (char *)cp;
	return result;
}

size_t strnlen(const char * s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}

long strtol(const char *nptr, char **endptr, int base)
{
   const char *p = nptr;
   unsigned long ret;
   unsigned char ch;
   unsigned long Overflow;
   int sign = 0, flag, LimitRemainder;
   do
   {
      ch = *p++;
   } while (isspace(ch));
   if (ch == '-')
   {
      sign = 1;
      ch = *p++;
   }
   else if (ch == '+')
      ch = *p++;
   if ((base == 0 || base == 16) && ch == '0' && (*p == 'x' || *p == 'X'))
   {
      ch = p[1];
      p += 2;
      base = 16;
   }
   if (base == 0)
      base = ch == '0' ? 8 : 10;
   Overflow = sign ? -(unsigned long)LONG_MIN : LONG_MAX;
   LimitRemainder = Overflow % (unsigned long)base;
   Overflow /= (unsigned long)base;
   for (ret = 0, flag = 0;; ch = *p++)
   {
      if (isdigit(ch))
        ch -= '0';
      else if (isalpha(ch))
        ch -= isupper(ch) ? 'A' - 10 : 'a' - 10;
      else
        break;
      if (ch >= base)
        break;
      if (flag < 0 || ret > Overflow || (ret == Overflow && ch > LimitRemainder))
        flag = -1;
      else
      {
        flag = 1;
        ret *= base;
        ret += ch;
      }
   }
   if (flag < 0)
      ret = sign ? LONG_MIN : LONG_MAX;
   else if (sign)
      ret = -ret;
   if (endptr != 0)
      *endptr = (char *)(flag ? (p - 1) : nptr);
   return ret;
}

int atoi(const char *src)
{
	int s = 0;
	bool isMinus = false;
	while(*src == ' ')
	{
		src++;
	}
	if(*src == '+' || *src == '-')
	{
		if(*src == '-')
		{
			isMinus = true;
		}
		src++;
	}
	else if(*src < '0' || *src > '9')
	{
		s = 2147483647;
		return s;
	}
	while(*src != '\0' && *src >= '0' && *src <= '9')
	{
		s = s * 10 + *src - '0';
		src++;
	}
	return s * (isMinus ? -1 : 1);
 }
