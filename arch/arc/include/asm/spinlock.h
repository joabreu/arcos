/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_SPINLOCK_H__
#define __ARC_SPINLOCK_H__

#include <arcOS/compiler-gcc.h>
#include <arcOS/types.h>

#define ARCH_SPINLOCK_LOCKED		1
#define ARCH_SPINLOCK_UNLOCKED		0

struct arch_spinlock {
	volatile u32 lock_status;
};

static inline void arch_spin_lock(struct arch_spinlock *lock)
{
	u32 val = ARCH_SPINLOCK_LOCKED;

	ASM(
		"1:	ex %0, [%1]\n"
		"	breq %0, %2, 1b\n"
		: "+&r"(val)
		: "r"(&(lock->lock_status)), "ir"(ARCH_SPINLOCK_LOCKED)
		: "memory");
}

static inline void arch_spin_unlock(struct arch_spinlock *lock)
{
	u32 val = ARCH_SPINLOCK_UNLOCKED;

	ASM(
		"ex %0, [%1]\n"
		: "+r"(val)
		: "r"(&(lock->lock_status))
		: "memory");
}

static inline void arch_spin_lock_init(struct arch_spinlock *lock)
{
	lock->lock_status = ARCH_SPINLOCK_UNLOCKED;
}

#endif

