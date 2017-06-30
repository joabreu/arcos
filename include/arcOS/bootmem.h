/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_BOOTMEM_H__
#define __ARCOS_BOOTMEM_H__

#include <arcOS/bitops.h>
#include <arcOS/types.h>

#define BOOTMEM_FLAGS_ZERO	BIT(0)

void *bootmem_zalloc(size_t size);
void bootmem_mark_free_region(unsigned long start_pfn, unsigned long end_pfn);
void bootmem_mark_reserved_region(unsigned long start_pfn, unsigned long end_pfn);
int init_bootmem(unsigned long map_pfn, unsigned long start_pfn,
		unsigned long end_pfn);

#endif /* __ARCOS_BOOTMEM_H__ */
