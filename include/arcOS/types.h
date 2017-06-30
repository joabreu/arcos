/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_TYPES_H__
#define __ARCOS_TYPES_H__

#ifndef __ASSEMBLY__

#include <arcOS/compiler-gcc.h>
#include <arcOS/errno.h>
#include <arcOS/stddef.h>

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef unsigned int size_t;
typedef signed int ssize_t;

typedef _Bool bool;

typedef u32 resource_size_t;
typedef u32 phys_addr_t;
#endif

#endif /* __ARCOS_TYPES_H__ */
