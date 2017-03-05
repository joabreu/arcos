/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_IRQ_H__
#define __ARC_IRQ_H__

#include <arcOS/kernel.h>

#if IS_ENABLED(CONFIG_ISA_ARCOMPACT)
#include <asm/irq-compact.h>
#else
#error "ISA not supported"
#endif

#define NR_CPU_IRQS	32
#define NR_IRQS		128

void arc_do_irq(unsigned int hwirq, struct pt_regs *regs);
void arc_panic_irq(unsigned int hwirq, struct pt_regs *regs);
void arc_init_irq(void);

#endif

