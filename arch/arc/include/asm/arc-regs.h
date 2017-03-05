/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_REGS__
#define __ARC_REGS__

#include <arcOS/bitops.h>

#define ARC_IDENTITY			0x04
#define ARC_INT_VECTOR_BASE		0x25
#define ARC_ARC700_BUILD		0xC1

/* status32 field values */
#define ARC_STATUS32_E1_MASK		BIT(1)
#define ARC_STATUS32_E2_MASK		BIT(2)
#define ARC_STATUS32_A1_MASK		BIT(3)
#define ARC_STATUS32_A2_MASK		BIT(4)

#ifndef __ASSEMBLY__

#include <arcOS/kernel.h>
#include <arcOS/types.h>

#define arc_read_aux_reg(reg)		__builtin_arc_lr(reg)
#define arc_write_aux_reg(reg, val)	__builtin_arc_sr((unsigned int)(val), (reg))
#define ARC_READ_BCR(reg, dest) \
{ \
	unsigned int tmp; \
	tmp = arc_read_aux_reg(reg); \
	if (sizeof(tmp) == sizeof(dest)) { \
		(dest) = *((typeof(dest) *)&tmp); \
	} else { \
		extern void undefined_func(void); \
		undefined_func(); \
	} \
}
#define ARC_WRITE_BCR(reg, dest) \
{ \
	unsigned int tmp; \
	if (sizeof(tmp) == sizeof(dest)) { \
		tmp = (*(unsigned int *)&(dest)); \
		arc_write_aux_reg(reg, tmp); \
	} else { \
		extern void undefined_func(void); \
		undefined_func(); \
	} \
}

struct arc_cache_info {
	unsigned int sz_k:14, line_len:8, assoc:4, ver:4, alias:1, vipt:1;
};

struct arc_ccm_info {
	unsigned int base_addr, sz;
};

struct arc_mmu_info {
	unsigned int ver:4, pg_sz_k:8, s_pg_sz_m:8, pad:10, sasid:1, pae:1;
	unsigned int sets:12, ways:4, u_dtlb:8, u_itlb:8;
};

struct arc_bpu_info {
	unsigned int ver, full, num_cache, num_pred;
};

struct arc_bcr_identity {
#ifdef CONFIG_CPU_BIG_ENDIAN
	unsigned int chip_id:16, cpu_id:8, family:8;
#else
	unsigned int family:8, cpu_id:8, chip_id:16;
#endif
};

struct arc_bcr_isa {
#ifdef CONFIG_CPU_BIG_ENDIAN
	unsigned int div_rem:4, pad2:4, ldd:1, unalign:1, atomic:1, be:1,
		     pad1:11, atomic1:1, ver:8;
#else
	unsigned int ver:8, atomic1:1, pad1:11, be:1, atomic:1, unalign:1,
		     ldd:1, pad2:4, div_rem:4;
#endif
};

struct arc_bcr_extn_xymem {
#ifdef CONFIG_CPU_BIG_ENDIAN
	unsigned int ram_org:2, num_banks:4, bank_sz:4, ver:8;
#else
	unsigned int ver:8, bank_sz:4, num_banks:4, ram_org:2;
#endif
};

struct arc_cpuinfo {
	struct arc_cache_info icache, dcache, slc;
	struct arc_ccm_info iccm, dccm;
	struct arc_mmu_info mmu;
	struct arc_bpu_info bpu;
	struct arc_bcr_identity core;
	struct arc_bcr_isa isa;
	const char *details, *name;
	unsigned int vec_base;
	struct {
		unsigned int swap:1, norm:1, minmax:1, barrel:1, crc:1,
			     swape:1, pad1:2, fpu_sp:1, fpu_dp:1, pad2:6,
			     debug:1, ap:1, smart:1, rtt:1, pad3:4,
			     timer0:1, timer1:1, rtc:1, gfrc:1, pad4:4;
	} extn;
};

static inline bool is_isa_arcompact(void)
{
	return IS_ENABLED(CONFIG_ISA_ARCOMPACT);
}

#endif /* __ASSEMBLY__ */
#endif

