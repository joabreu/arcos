/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/errno.h>
#include <arcOS/printk.h>
#include <arcOS/types.h>
#include <asm/unwind.h>

#define FRAME_RETADDR_OFFSET		sizeof(u32)
#define FRAME_PREVSP_OFFSET		0

int arc_unwind(struct unwind_info *info)
{
	if (!is_stack(info->task, (void *)STACK_FP(info)))
		return -ENXIO;

	STACK_PC(info) = *((u32 *)(STACK_FP(info) + FRAME_RETADDR_OFFSET));
	STACK_SP(info) = *((u32 *)(STACK_FP(info) + FRAME_PREVSP_OFFSET));
	STACK_FP(info) = STACK_SP(info);
	return 0;
}

