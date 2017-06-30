/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/init-task.h>
#include <arcOS/task.h>

struct task_struct init_task = INIT_TASK(init_task);
union task_union init_task_union;
