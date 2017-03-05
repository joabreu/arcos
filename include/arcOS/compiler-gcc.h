/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_COMPILER_GCC_H__
#define __ARCOS_COMPILER_GCC_H__

#define ASM			__asm__ __volatile__
#define __noinline		__attribute__((noinline))
#define __always_inline		inline __attribute__((always_inline))
#define __printf(a, b)		__attribute__((format(printf, a, b)))
#define __section(s)		__attribute__((__section__(#s)))
#define __used			__attribute__((__used__))
#define __aligned(n)		__attribute__((aligned(n)))
#define __same_type(a, b)	__builtin_types_compatible_p(typeof(a), typeof(b))

#include <arcOS/bug.h>

#define __must_be_array(a)	BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

#endif

