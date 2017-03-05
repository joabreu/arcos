/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */
#include <asm/arc-regs.h>
#include <asm/task-info.h>

.macro CPU_EARLY_SETUP
	; debug
	mov	r0, 0xdeadcafe

	; store interrupt vector address
	sr	@__intvector_base, [ARC_INT_VECTOR_BASE]
.endm

.section .text
.global __arc_entry
.align 4
__arc_entry:
	; CPU setup
	CPU_EARLY_SETUP	

	; Clear BSS
	mov	r5, __bss_start
	sub	r6, __bss_end, r5
	lsr.f	lp_count, r6, 2
	lpnz	1f
	st.ab	0, [r5, 4]

1:
	; setup stack
	mov	fp, 0
	mov	r9, @init_task
	ld	sp, [r9, 0]			; load stack var
	add2	sp, sp, (TASK_STACK_SIZE)/4	; move to end of stack

	; "C" entry point
	j	start_kernel

	; halt
	flag 1

