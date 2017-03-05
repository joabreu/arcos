/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bug.h>
#include <arcOS/irq.h>
#include <arcOS/printk.h>
#include <asm/arc-regs.h>
#include <asm/irq.h>
#include <soc/arc/timers.h>

void arc_do_irq(unsigned int hwirq, struct pt_regs *regs)
{
	handle_domain_irq(NULL, hwirq, regs);
}

void arc_panic_irq(unsigned int hwirq, struct pt_regs *regs)
{
	PANIC("\nUnexpected HWIRQ=0x%x", hwirq);
}

void arc_init_irq(void)
{
	arc_write_aux_reg(ARC_AUX_IRQ_LEV, 0x0);
}

