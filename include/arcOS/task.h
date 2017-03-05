/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_TASK_H__
#define __ARCOS_TASK_H__

#include <arcOS/printk.h>
#include <arcOS/types.h>
#include <asm/task-info.h> /* <-- Arch specific */

#define STACK_END_MAGIC		0xdeadbeef

struct task_struct {
	void *stack; /* <-- Must be first! */
};

union task_union {
	u32 stack[TASK_STACK_SIZE / sizeof(u32)];
};

static inline u32 *begin_of_stack(void *stack)
{
#ifdef CONFIG_STACK_GROWSUP
	return (u32 *)((u32)stack);
#else
	return (u32 *)((u32)stack + TASK_STACK_SIZE) - 1;
#endif
}

static inline u32 *end_of_stack(void *stack)
{
#ifdef CONFIG_STACK_GROWSUP
	return (u32 *)((u32)stack + TASK_STACK_SIZE) - 1;
#else
	return (u32 *)((u32)stack);
#endif
}

static inline bool is_stack(struct task_struct *task, void *addr)
{
#ifdef CONFIG_STACK_GROWSUP
	return ((u32 *)addr >= begin_of_stack(task->stack) &&
		(u32 *)addr <= end_of_stack(task->stack));
#else
	return ((u32 *)addr <= begin_of_stack(task->stack) &&
		(u32 *)addr >= end_of_stack(task->stack));
#endif
}

void task_set_end_magic(struct task_struct *task);
bool is_task_corrupted(struct task_struct *task);

#endif

