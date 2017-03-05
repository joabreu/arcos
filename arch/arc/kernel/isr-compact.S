/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

.cpu A7
.macro VECTOR lbl
	j	\lbl
.endm

.section .intvector
.align 4

VECTOR	panic_irq_entry		; 0x0: Reset vector
VECTOR	panic_irq_entry		; 0x8: Mem exception
VECTOR	panic_irq_entry		; 0x10: Instruction Error

.rept 29
VECTOR	l1_irq_entry		; Devices interrupts
.endr

VECTOR	panic_irq_entry		; 0x100: Fatal machine check
VECTOR	panic_irq_entry		; 0x108: Instruction TLB miss
VECTOR	panic_irq_entry		; 0x110: Data TLB miss
VECTOR	panic_irq_entry		; 0x118: Protection violation os misaligned
				; address
VECTOR	panic_irq_entry		; 0x120: Privilege violation
VECTOR	panic_irq_entry		; 0x128: Trap exception
VECTOR	panic_irq_entry		; 0x180: Extension instruction exception

.rept 24
VECTOR	reserved_irq_entry	; Reserved
.endr

.section .text

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

.macro ISR_PROLOGUE1
#ifdef CONFIG_FRAME_POINTER
	PUSH	ilink1
	PUSH	fp
	mov	fp, sp
#endif
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
.endm

.macro ISR_EPILOGUE1
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
#ifdef CONFIG_FRAME_POINTER
	mov	sp, fp
	POP	fp
	POP	ilink1
#endif
.endm

reserved_irq_entry:
	lr	r0, [icause1]
	and	r0, r0, 0x1f
	mov	r1, 0xdeadcafe
	flag	1

.global panic_irq_entry
.align 4
panic_irq_entry:
#ifdef CONFIG_FRAME_POINTER
	PUSHAX	efa
	PUSH	fp
	mov	fp, sp
#endif

	lr	r0, [ecr]

	bl.d	@arc_panic_irq
	mov	r1, sp

	flag	1

.global l1_irq_entry
.align 4
l1_irq_entry:
	ISR_PROLOGUE1

	lr	r0, [icause1]
	and	r0, r0, 0x1f

	bl.d	@arc_do_irq
	mov	r1, sp

	mov	r8, 0x1
	sr	r8, [0x43]

	ISR_EPILOGUE1
	rtie

