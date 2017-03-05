/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_PAGE_H__
#define __ARCOS_PAGE_H__

#include <arcOS/kernel.h>
#include <asm/page.h>

#define PAGE_ALIGN(addr)	ALIGN(addr, PAGE_SIZE)
#define PAGE_ALIGNED(addr)	IS_ALIGNED((unsigned long)(addr), PAGE_SIZE)

#endif

