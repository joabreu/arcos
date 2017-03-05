/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bitops.h>
#include <arcOS/types.h>

inline unsigned long ffs(unsigned long word)
{
	int num = 0;

	if ((word & 0xffff) == 0) {
		num += 16;
		word >>= 16;
	}
	if ((word & 0xff) == 0) {
		num += 8;
		word >>= 8;
	}
	if ((word & 0xf) == 0) {
		num += 4;
		word >>= 4;
	}
	if ((word & 0x3) == 0) {
		num += 2;
		word >>= 2;
	}
	if ((word & 0x1) == 0)
		num += 1;
	return num;
}

inline bool test_and_clear_bit(int nr, unsigned long *addr)
{
	unsigned long mask = ~BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old = *p;

	*p = old & mask;
	return (old & ~mask) != 0;
}

inline bool test_and_set_bit(int nr, unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old = *p;

	*p = old | mask;
	return (old & mask) != 0;
}

inline bool test_bit(int nr, unsigned long *addr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}

static unsigned long __find_next_bit(const unsigned long *addr,
		unsigned long size, unsigned long start, unsigned long invert)
{
	unsigned long tmp;

	if (!size || start >= size)
		return size;

	tmp = addr[start / BITS_PER_LONG] ^ invert;
	tmp &= BITMAP_FIRST_WORD_MASK(start);
	start = round_down(start, BITS_PER_LONG);

	while (!tmp) {
		start += BITS_PER_LONG;
		if (start >= size)
			return size;

		tmp = addr[start / BITS_PER_LONG] ^ invert;
	}

	return min(start + ffs(tmp), size);
}

unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
		unsigned long offset)
{
	return __find_next_bit(addr, size, offset, 0UL);
}

unsigned long find_next_zero_bit(const unsigned long *addr,
		unsigned long size, unsigned long offset)
{
	return __find_next_bit(addr, size, offset, ~0UL);
}

