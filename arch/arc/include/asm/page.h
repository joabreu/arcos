/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_PAGE_H__
#define __ARC_PAGE_H__

#include <arcOS/bitops.h>

#define PAGE_SHIFT	13	/* 8k */
#define PAGE_SIZE	BIT(PAGE_SHIFT)
#define PAGE_MASK	(~(PAGE_SIZE-1))

#ifndef __ASSEMBLY__

#define __pa(vaddr)		((unsigned long)(vaddr))
#define __va(paddr)		((void *)(unsigned long)(paddr))

#define virt_to_pfn(vaddr)	(__pa(vaddr) >> PAGE_SHIFT)

#define ARCH_PFN_OFFSET		virt_to_pfn(CONFIG_ARCOS_LINK_BASE)

/* TLBPD0 fields */
#define PAGE_VPN_OFFSET		13
#define PAGE_VPN_MASK		GENMASK(30,13)
#define PAGE_FLAG_VALID		BIT(10)
#define PAGE_FLAG_GLOBAL	BIT(8)
#define PAGE_ASID_OFFSET	0
#define PAGE_ASIC_MASK		GENMASK(7,0)

/* TLBPD1 fields */
#define PAGE_PFN_OFFSET		13
#define PAGE_PFN_MASK		GENMASK(31,13)
#define PAGE_FLAG_RK		BIT(8)
#define PAGE_FLAG_WK		BIT(7)
#define PAGE_FLAG_EK		BIT(6)
#define PAGE_FLAG_RU		BIT(5)
#define PAGE_FLAG_WU		BIT(4)
#define PAGE_FLAG_EU		BIT(3)
#define PAGE_FLAG_FC		BIT(2)

#endif /* __ASSEMBLY__ */
#endif /* __ARC_PAGE_H__ */
