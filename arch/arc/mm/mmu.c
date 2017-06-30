/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bug.h>
#include <arcOS/types.h>
#include <asm/arc-regs.h>
#include <asm/mmu.h>
#include <asm/page.h>

static u8 arc_get_mmu_version(void)
{
	u32 reg = arc_read_aux_reg(ARC_REG_MMU_BCR);

	return reg >> 24 & 0xff;
}

static inline unsigned int tlb_entry_lookup(u32 vaddr)
{
	arc_write_aux_reg(ARC_REG_TLBPD0, vaddr);
	arc_write_aux_reg(ARC_REG_TLBCOMMAND, ARC_TLB_PROBE_CMD);

	return arc_read_aux_reg(ARC_REG_TLBINDEX);
}

static void tlb_entry_insert(u32 pd0, u32 pd1)
{
	u32 idx = tlb_entry_lookup(pd0);

	if (idx & ARC_TLB_LOOKUP_ERR)
		arc_write_aux_reg(ARC_REG_TLBCOMMAND, ARC_TLB_GETINDEX_CMD);

	arc_write_aux_reg(ARC_REG_TLBPD1, pd1);
	arc_write_aux_reg(ARC_REG_TLBCOMMAND, ARC_TLB_WRITE_CMD);
}

void arc_create_tlb(u32 vaddr, u32 paddr)
{
	u32 pd0, pd1;

	vaddr &= PAGE_MASK;
	pd0 = (vaddr << PAGE_VPN_OFFSET) & PAGE_VPN_MASK;
	pd0 |= PAGE_FLAG_GLOBAL | PAGE_FLAG_VALID;

	paddr &= PAGE_MASK;
	pd1 = (paddr << PAGE_PFN_OFFSET) & PAGE_PFN_MASK;
	pd1 |= PAGE_FLAG_RK | PAGE_FLAG_WK | PAGE_FLAG_EK;

	tlb_entry_insert(pd0, pd1);
}

void arc_mmu_init(void)
{
	if (arc_get_mmu_version() != CONFIG_ARC_MMU_VER) {
		PANIC("MMU ver:%d does not match kernel built for ver:%d\n",
				arc_get_mmu_version(), CONFIG_ARC_MMU_VER);
	}

	arc_write_aux_reg(ARC_REG_PID, ARC_REG_PID_TLB_ENABLE);
}
