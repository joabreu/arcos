/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bootmem.h>
#include <arcOS/io.h>
#include <arcOS/mm.h>
#include <arcOS/pfn.h>
#include <asm/setup.h>
#include <asm-generic/sections.h>

void setup_arch_memory(void)
{
	unsigned long mapstart;
	unsigned long start_pfn, end_pfn;

	init_mm.start_code = (unsigned long)__text_start;
	init_mm.end_code = (unsigned long)__text_end;
	init_mm.start_data = (unsigned long)__data_start;
	init_mm.end_data = (unsigned long)__data_end;

	mapstart = PFN_UP((unsigned long)__end);

	start_pfn = ARCH_PFN_OFFSET;
	end_pfn = PFN_DOWN(CONFIG_MEM_SIZE) + start_pfn;

	init_bootmem(mapstart, start_pfn, end_pfn);
	bootmem_mark_reserved_region(start_pfn, mapstart - 1);

	bootmem_alloc(0x100);
	bootmem_alloc(0x123);
	bootmem_alloc(0x320);
	bootmem_alloc(0x400);
	bootmem_alloc(0x600);

	printk("Memory @ %lx [%ldM]\n", __pa(PFN_PHYS(start_pfn)),
			__pa(PFN_PHYS(end_pfn - start_pfn)) >> 20);
}

