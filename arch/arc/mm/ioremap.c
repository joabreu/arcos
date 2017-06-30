/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/io.h>
#include <arcOS/types.h>
#include <asm/cache.h>

static bool arc_uncached_addr_space(phys_addr_t paddr)
{
	if (paddr >= arc_perip_base && paddr <= arc_perip_end)
		return true;
	return false;
}

void *ioremap(phys_addr_t paddr, size_t size)
{
	phys_addr_t pend;

	pend = paddr + size - 1;
	if (!size || (pend < paddr))
		return NULL;

	if (arc_uncached_addr_space(paddr))
		return (void *)(u32)paddr;
	return NULL;
}
