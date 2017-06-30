/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/task.h>

void task_set_end_magic(struct task_struct *task)
{
	u32 *end = end_of_stack(task->stack);

	*end = STACK_END_MAGIC;
}

bool is_task_corrupted(struct task_struct *task)
{
	u32 *end = end_of_stack(task->stack);

	if (!end)
		return true;
	return *end != STACK_END_MAGIC;
}
