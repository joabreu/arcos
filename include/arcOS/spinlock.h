/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_SPINLOCK_H__
#define __ARCOS_SPINLOCK_H__

#include <asm/spinlock.h>

typedef struct spinlock {
	struct arch_spinlock raw_lock;
} spinlock_t;

#define spin_lock(lock) \
	arch_spin_lock(&((lock)->raw_lock))
#define spin_unlock(lock) \
	arch_spin_unlock(&((lock)->raw_lock))
#define spin_lock_init(lock) \
	arch_spin_lock_init(&((lock)->raw_lock))

#endif

