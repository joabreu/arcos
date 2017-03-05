/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_STACKTRACE_H__
#define __ARC_STACKTRACE_H__

struct task_struct *task;

void show_stacktrace(struct task_struct *task);

#endif

