/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_IRQ_COMPACT_H__
#define __ARC_IRQ_COMPACT_H__

#include <asm/arc-regs.h>
#include <arcOS/compiler-gcc.h>

#define ARC_AUX_IRQ_LEV		0x200
#define ARC_AUX_IENABLE		0x40C

struct pt_regs {
	u32 bta_l1;
	u32 lp_start, lp_end, lp_count;
	u32 status32_l1;
	u32 ilink1, blink;
	u32 fp;
	u32 gp;
	u32 r12, r11, r10, r9, r8, r7, r6, r5, r4, r3, r2, r1, r0;
	u32 sp;
};

static inline long arch_local_irq_save(void)
{
	unsigned long tmp, flags;

	ASM(
		"lr %1, [status32]\n"
		"bic %0, %1, %2\n"
		"and.f 0, %1, %2\n"
		"flag.nz %0\n"
		: "=r"(tmp), "=r"(flags)
		: "n"((ARC_STATUS32_E1_MASK | ARC_STATUS32_E2_MASK))
		: "memory", "cc");

	return flags;
}

static inline void arch_local_irq_restore(unsigned long flags)
{
	ASM(
		"flag %0\n"
		:
		: "r"(flags)
		: "memory");
}

static inline void arch_local_irq_enable(void)
{
	unsigned long tmp;

	ASM(
		"lr %0, [status32]\n"
		"or %0, %0, %1\n"
		"flag %0\n"
		: "=&r"(tmp)
		: "n"((ARC_STATUS32_E1_MASK | ARC_STATUS32_E2_MASK))
		: "cc", "memory");
}

static inline void arch_local_irq_disable(void)
{
	unsigned long tmp;

	ASM(
		"lr %0, [status32]\n"
		"and %0, %0, %1\n"
		"flag %0"
		: "=&r"(tmp)
		: "n"(~(ARC_STATUS32_E1_MASK | ARC_STATUS32_E2_MASK))
		: "memory");
}

#endif /* __ARC_IRQ_COMPACT_H__ */
