/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_STDDEF_H__
#define __ARCOS_STDDEF_H__

#include <arcOS/types.h>

#undef NULL
#define NULL		((void *)0)

enum {
	false = 0,
	true = 1
};

#undef offsetof
#ifdef __compiler_offsetof
#define offsetof(type, member)		__compiler_offsetof(type, member)
#else
#define offsetof(type, member)		((size_t)&((type *)0)->member)
#endif

#define offsetofend(type, member) \
	(offsetof(type, member) + sizeof(((type *)0)->member))

#endif

