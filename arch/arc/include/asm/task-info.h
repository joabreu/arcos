/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_TASK_INFO_H__
#define __ARC_TASK_INFO_H__

#include <asm/page.h>

#define TASK_STACK_SIZE		PAGE_SIZE
#define init_stack		(init_task_union.stack)

#endif

