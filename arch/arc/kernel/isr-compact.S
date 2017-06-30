/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

 #include <asm/asm-helpers.h>

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

reserved_irq_entry:
	lr	r0, [icause1]
	and	r0, r0, 0x1f
	mov	r1, 0xdeadcafe
	flag	1

.global panic_irq_entry
.align 4
panic_irq_entry:
	FRAME_POINTER_SAVE

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
