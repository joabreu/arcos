/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_BOOTMEM_H__
#define __ARCOS_BOOTMEM_H__

#include <arcOS/types.h>

void *bootmem_alloc(size_t size);
void bootmem_mark_free_region(unsigned long start_pfn, unsigned long end_pfn);
void bootmem_mark_reserved_region(unsigned long start_pfn, unsigned long end_pfn);
int init_bootmem(unsigned long map_pfn, unsigned long start_pfn,
		unsigned long end_pfn);

#endif

