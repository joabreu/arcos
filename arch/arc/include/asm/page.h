/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_PAGE_H__
#define __ARC_PAGE_H__

#include <arcOS/bitops.h>

#define PAGE_SHIFT	13	/* 8k */
#define PAGE_SIZE	BIT(PAGE_SHIFT)

#ifndef __ASSEMBLY__

#define __pa(vaddr)		((unsigned long)(vaddr))
#define __va(paddr)		((void *)(unsigned long)(paddr))

#define virt_to_pfn(vaddr)	(__pa(vaddr) >> PAGE_SHIFT)

#define ARCH_PFN_OFFSET		virt_to_pfn(CONFIG_ARCOS_LINK_BASE)

#endif /* __ASSEMBLY__ */
#endif

