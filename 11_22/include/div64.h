

#ifndef _ASM_GENERIC_DIV64_H
#define _ASM_GENERIC_DIV64_H


#include "imi_type.h"


#ifndef __arch_xprod_64
/*
 * Default C implementation for __arch_xprod_64()
 *
 * Prototype: uint64_t __arch_xprod_64(const uint64_t m, uint64_t n, bool bias)
 * Semantic:  retval = ((bias ? m : 0) + m * n) >> 64
 *
 * The product is a 128-bit value, scaled down to 64 bits.
 * Assuming constant propagation to optimize away unused conditional code.
 * Architectures may provide their own optimized assembly implementation.
 */
static inline uint64_t __arch_xprod_64(const uint64_t m, uint64_t n, bool bias)
{
	uint32 m_lo = m;
	uint32 m_hi = m >> 32;
	uint32 n_lo = n;
	uint32 n_hi = n >> 32;
	uint64_t res, tmp;

	if (!bias) {
		res = ((uint64_t)m_lo * n_lo) >> 32;
	} else if (!(m & ((1ULL << 63) | (1ULL << 31)))) {
		/* there can't be any overflow here */
		res = (m + (uint64_t)m_lo * n_lo) >> 32;
	} else {
		res = m + (uint64_t)m_lo * n_lo;
		tmp = (res < m) ? (1ULL << 32) : 0;
		res = (res >> 32) + tmp;
	}

	if (!(m & ((1ULL << 63) | (1ULL << 31)))) {
		/* there can't be any overflow here */
		res += (uint64_t)m_lo * n_hi;
		res += (uint64_t)m_hi * n_lo;
		res >>= 32;
	} else {
		tmp = res += (uint64_t)m_lo * n_hi;
		res += (uint64_t)m_hi * n_lo;
		tmp = (res < tmp) ? (1ULL << 32) : 0;
		res = (res >> 32) + tmp;
	}

	res += (uint64_t)m_hi * n_hi;

	return res;
}
#endif



#define __div64_const32(n, ___b)							\
({															\
															\
	uint64_t ___res, ___x, ___t, ___m, ___n = (n);			\
	uint32 ___p, ___bias;									\
															\
	/* determine MSB of b */								\
	___p = 1 << ilog2(___b);								\
															\
	/* compute m = ((p << 64) + b - 1) / b */				\
	___m = (~0ULL / ___b) * ___p;							\
	___m += (((~0ULL % ___b + 1) * ___p) + ___b - 1) / ___b;	\
															\
	/* one less than the dividend with highest result */	\
	___x = ~0ULL / ___b * ___b - 1;							\
									\
	/* test our ___m with res = m * x / (p << 64) */		\
	___res = ((___m & 0xffffffff) * (___x & 0xffffffff)) >> 32;	\
	___t = ___res += (___m & 0xffffffff) * (___x >> 32);		\
	___res += (___x & 0xffffffff) * (___m >> 32);			\
	___t = (___res < ___t) ? (1ULL << 32) : 0;				\
	___res = (___res >> 32) + ___t;							\
	___res += (___m >> 32) * (___x >> 32);					\
	___res /= ___p;											\
															\
	/* Now sanitize and optimize what we've got. */			\
	if (~0ULL % (___b / (___b & -___b)) == 0) {				\
		/* special case, can be simplified to ... */		\
		___n /= (___b & -___b);								\
		___m = ~0ULL / (___b / (___b & -___b));				\
		___p = 1;											\
		___bias = 1;										\
	} else if (___res != ___x / ___b) {						\
															\
		___bias = 1;										\
		/* Compute m = (p << 64) / b */						\
		___m = (~0ULL / ___b) * ___p;						\
		___m += ((~0ULL % ___b + 1) * ___p) / ___b;			\
	} else {												\
		/*													\
		 * Reduce m / p, and try to clear bit 31 of m when	\
		 * possible, otherwise that'll need extra overflow	\
		 * handling later.									\
		 */													\
		uint32 ___bits = -(___m & -___m);					\
		___bits |= ___m >> 32;								\
		___bits = (~___bits) << 1;							\
		if (!___bits) {										\
			___p /= (___m & -___m);							\
			___m /= (___m & -___m);							\
		} else {											\
			___p >>= ilog2(___bits);						\
			___m >>= ilog2(___bits);						\
		}													\
		___bias = 0;										\
	}														\
	___res = __arch_xprod_64(___m, ___n, ___bias);			\
															\
	___res /= ___p;											\
})




#endif