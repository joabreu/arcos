/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_UNWIND_H__
#define __ARC_UNWIND_H__

#include <arcOS/errno.h>
#include <arcOS/kernel.h>
#include <arcOS/task.h>
#include <arcOS/types.h>

struct arc700_regs {
	/* r27 = fp, r28 = sp, r31 = blink, r63 = pc */
	u32 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10;
	u32 r11, r12, r13, r14, r15, r16, r17, r18, r19, r20;
	u32 r21, r22, r23, r24, r25, r26, r27, r28, r29, r30;
	u32 r31, r63;
};

struct unwind_info {
	struct arc700_regs regs;
	struct task_struct *task;
};

#define STACK_FP(info)		((info)->regs.r27)
#define STACK_SP(info)		((info)->regs.r28)
#define STACK_PC(info)		((info)->regs.r63)

#if IS_ENABLED(CONFIG_FRAME_POINTER) && IS_ENABLED(CONFIG_ARC_UNWIND)
#error "Select between CONFIG_FRAMER_POINTER or CONFIG_ARC_UNWIND"
#elif IS_ENABLED(CONFIG_FRAME_POINTER) || IS_ENABLED(CONFIG_ARC_UNWIND)
int arc_unwind(struct unwind_info *info);
#else
static inline int arc_unwind(struct unwind_info *info)
{
	return -ENXIO;
}
#endif

#endif

