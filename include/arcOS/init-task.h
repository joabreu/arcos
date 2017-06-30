/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_INIT_TASK_H__
#define __ARCOS_INIT_TASK_H__

#include <arcOS/task.h>
#include <asm/task-info.h> /* <-- Arch specific */

#define INIT_TASK(task) \
{ \
	.stack = init_stack, \
}; \

extern struct task_struct init_task;
extern union task_union init_task_union;

#endif /* __ARCOS_INIT_TASK_H__ */
