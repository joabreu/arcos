/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_BUG_H__
#define __ARCOS_BUG_H__

#include <arcOS/printk.h>
#include <arcOS/reboot.h>
#include <arcOS/stddef.h>
#include <arcOS/types.h>
#include <asm/stacktrace.h>

struct task_struct;
extern struct task_struct init_task;

#define __WARN(__condition__) \
	do { \
		char *__holder__ = (char *)__condition__; \
		printk("\n --- WARNING: %s:%d ---\n", __FILE__, __LINE__); \
		if (__holder__) \
			printk(" --- condition='%s'\n", __holder__); \
		show_stacktrace(&init_task); \
	} while (0);
#define PANIC(args...) \
	do { \
		printk("\n --- KERNEL PANIC: %s:%d ---\n", __FILE__, __LINE__); \
		printk(" --- Description: "); \
		printk(args); \
		show_stacktrace(&init_task); \
		machine_halt(); \
	} while(1);
#define PANIC_ON(__condition__, __args__...) \
	do { \
		if (!(__condition__)) { \
			break; \
		} else { \
			PANIC(__args__); \
		} \
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
#define WARN_ON(condition) ({ \
	int __ret__ = !!(condition); \
	if (__ret__) \
		__WARN(#condition); \
	__ret__; \
})
#define WARN()		__WARN(NULL)

#endif /* __ARCOS_BUG_H__ */
