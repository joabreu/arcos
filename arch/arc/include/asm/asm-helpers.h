/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_ASM_HELPERS_H__
#define __ARC_ASM_HELPERS_H__

.macro PUSH reg
	st.a	\reg, [sp, -4]
.endm

.macro PUSHAX aux
	lr	r9, [\aux]
	PUSH	r9
.endm

.macro POP reg
	ld.ab	\reg, [sp, 4]
.endm

.macro POPAX aux
	POP	r9
	sr	r9, [\aux]
.endm

.macro FRAME_POINTER_SAVE
#ifdef CONFIG_FRAME_POINTER
	PUSH	blink
	PUSH	fp
	mov	fp, sp
#endif
.endm

.macro FRAME_POINTER_RESTORE
#ifdef CONFIG_FRAME_POINTER
	mov	sp, fp
	POP	fp
	POP	blink
#endif
.endm

.macro FRAME_POINTER_SAVE_ISR
#ifdef CONFIG_FRAME_POINTER
	PUSH	ilink1
	mov	r0, 0x0
	PUSH	r0
	mov	fp, sp
#endif
.endm

.macro FRAME_POINTER_RESTORE_ISR
#ifdef CONFIG_FRAME_POINTER
	mov	sp, fp
	POP	r0
	POP	ilink1
#endif
.endm

.macro ISR_PROLOGUE1
	PUSH sp

	PUSH r0
	PUSH r1
	PUSH r2
	PUSH r3
	PUSH r4
	PUSH r5
	PUSH r6
	PUSH r7
	PUSH r8
	PUSH r9
	PUSH r10
	PUSH r11
	PUSH r12

	PUSH gp
	PUSH fp
	PUSH blink
	PUSH ilink1
	PUSHAX status32_l1
	PUSH lp_count
	PUSHAX lp_end
	PUSHAX lp_start
	PUSHAX bta_l1

	FRAME_POINTER_SAVE_ISR
.endm

.macro ISR_EPILOGUE1
	FRAME_POINTER_RESTORE_ISR

	POPAX	bta_l1
	POPAX	lp_start
	POPAX	lp_end

	POP	r9
	mov	lp_count, r9

	POPAX	status32_l1
	POP	ilink1
	POP	blink
	POP	fp
	POP	gp

	POP	r12
	POP	r11
	POP	r10
	POP	r9
	POP	r8
	POP	r7
	POP	r6
	POP	r5
	POP	r4
	POP	r3
	POP	r2
	POP	r1
	POP	r0

	POP	sp
.endm

#endif /* __ARC_ASM_HELPERS_H__ */
