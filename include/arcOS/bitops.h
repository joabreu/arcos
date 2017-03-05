/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_BITOPS_H__
#define __ARCOS_BITOPS_H__

#define BITS_PER_LONG	32
#define BITS_PER_BYTE	8

#ifdef __ASSEMBLY__
#define BIT(n)		(1 << (n))
#define GENMASK(h, l)	(((~0) << (l)) & (~0 >> (BITS_PER_LONG - 1 - (h))))
#else

#include <arcOS/kernel.h>
#include <arcOS/types.h>

#define BIT(n)		(1UL << (n))
#define BIT_MASK(n)	(1UL << ((n) % BITS_PER_LONG))
#define BIT_WORD(n)	((n) / BITS_PER_LONG)
#define GENMASK(h, l)	(((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))
#define BITS_TO_LONGS(n)	DIV_ROUND_UP(n, BITS_PER_BYTE * sizeof(long))
#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))

inline unsigned long ffs(unsigned long word);
inline bool test_and_clear_bit(int nr, unsigned long *addr);
inline bool test_and_set_bit(int nr, unsigned long *addr);
inline bool test_bit(int nr, unsigned long *addr);
unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
		unsigned long offset);
unsigned long find_next_zero_bit(const unsigned long *addr,
		unsigned long size, unsigned long offset);

#endif /* __ASSEMBLY__ */
#endif

