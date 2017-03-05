/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_KERNEL_H__
#define __ARCOS_KERNEL_H__

#include <arcOS/compiler-gcc.h>
#include <arcOS/stddef.h>

#define __ARG_PLACEHOLDER_1	0,
#define __take_second_arg(__ignored, val, ...)	val

#define __is_defined(x)		___is_defined(x)
#define ___is_defined(x)	____is_defined(__ARG_PLACEHOLDER_##x)
#define ____is_defined(x)	__take_second_arg(x 1, 0)

#define IS_ENABLED(option)	(__is_defined(option))
#define ARRAY_SIZE(a)		(sizeof(a) / sizeof((a)[0]) + __must_be_array(a))
#define ALIGN(x, a)		__ALIGN_MASK(x, (typeof(x))(a) - 1)
#define __ALIGN_MASK(x, mask)	(((x) + (mask)) & ~(mask))
#define IS_ALIGNED(x, a)	(((x) & ((typeof(x))(a) - 1)) == 0)
#define DIV_ROUND_UP(n, d)	(((n) + (d) - 1) / (d))

#define container_of(ptr, type, member)	({ \
	const typeof(((type *)0)->member) *__mptr = (ptr); \
	(type *)((char *)__mptr - offsetof(type, member)); })

#define max(x, y)		(((x) > (y)) ? (x) : (y))
#define min(x, y)		(((x) < (y)) ? (x) : (y))

#define __round_mask(x, y)	((typeof(x))((y) - 1))
#define round_up(x, y)		((((x) - 1) | __round_mask(x, y)) + 1)
#define round_down(x, y)	((x) & ~__round_mask(x, y))

#endif

