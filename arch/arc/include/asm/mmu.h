/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_MMU_H__
#define __ARC_MMU_H__

#include <arcOS/bitops.h>

#define ARC_REG_MMU_BCR			0x06f
#define ARC_REG_TLBPD0			0x405
#define ARC_REG_TLBPD1			0x406
#define ARC_REG_TLBINDEX		0x407
#define ARC_REG_TLBCOMMAND		0x408
#define ARC_REG_PID			0x409

#define ARC_REG_PID_TLB_ENABLE		BIT(31)
#define ARC_TLB_LOOKUP_ERR		BIT(31)

/* Commands */
#define ARC_TLB_WRITE_CMD		0x1
#define ARC_TLB_READ_CMD		0x2
#define ARC_TLB_GETINDEX_CMD		0x3
#define ARC_TLB_PROBE_CMD		0x4
#define ARC_TLB_WRITENI_CMD		0x5
#define ARC_TLB_IVUTLB_CMD		0x6

void arc_mmu_init(void);
void arc_create_tlb(u32 vaddr, u32 paddr);

#endif /* __ARC_MMU_H__ */
