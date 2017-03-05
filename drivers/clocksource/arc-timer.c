/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/irq.h>
#include <arcOS/of.h>
#include <arcOS/printk.h>
#include <asm/arc-regs.h>
#include <soc/arc/timers.h>

static int arc_clockevent_irq_handler(int irq, void *devid)
{
	arc_write_aux_reg(ARC_TIMER0_CTRL, ARC_TIMER_CTRL_IE | ARC_TIMER_CTRL_NH);
	return 0;
}

static int arc_clockevent_setup(const struct ofnode *node)
{
	int ret;

	printk("%s: int=%d, mem@0x%x-0x%x, pdata=0x%x\n",
			node->compatible, node->interrupt,
			node->memstart, node->memend,
			(unsigned int)node->pdata);

	ret = request_irq(node->interrupt, arc_clockevent_irq_handler,
			node->compatible, NULL);
	if (ret) {
		printk("%s: failed to request irq[%d]\n", __func__, ret);
		return ret;
	}

	arc_write_aux_reg(ARC_TIMER0_LIMIT, 0x01000000);
	arc_write_aux_reg(ARC_TIMER0_CNT, 0);
	arc_write_aux_reg(ARC_TIMER0_CTRL, ARC_TIMER_CTRL_IE | ARC_TIMER_CTRL_NH);
	return 0;
}
OFDEVICE_DEFINE(arc_clksource, "snps,arc-timer", arc_clockevent_setup);

