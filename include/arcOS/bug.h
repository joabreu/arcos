/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_BUG_H__
#define __ARCOS_BUG_H__

#include <arcOS/printk.h>
#include <arcOS/reboot.h>
#include <arcOS/types.h>
#include <asm/stacktrace.h>

struct task_struct;
extern struct task_struct init_task;

#define PANIC(args...) \
	do { \
		printk("\n --- KERNEL PANIC ---\n"); \
		printk(args); \
		show_stacktrace(&init_task); \
		machine_halt(); \
	} while(1);
#define EARLY_ASSERT(r) \
	do { \
		if(!(r)) { \
			break; \
		} else { \
			show_stacktrace(&init_task); \
			machine_halt(); \
		} \
	} while(1);
#define BUILD_BUG_ON_ZERO(e)	(sizeof(struct { int:-!!(e); }))

#endif /* __ARCOS_BUG_H__ */
