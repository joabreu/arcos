/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_PRINTK_H__
#define __ARCOS_PRINTK_H__

#include <stdarg.h>
#include <arcOS/compiler-gcc.h>

__printf(1, 2) int printk(const char *fmt, ...);

#endif

