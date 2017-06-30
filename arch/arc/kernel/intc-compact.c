/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bitops.h>
#include <arcOS/bug.h>
#include <arcOS/irq.h>
#include <arcOS/of.h>
#include <arcOS/types.h>
#include <asm/arc-regs.h>
#include <asm/irq.h>

static void arc_intc_mask(struct irq_desc *desc)
{
	u32 val;

	if (WARN_ON(desc->hwirq > 31))
		return;

	val = arc_read_aux_reg(ARC_AUX_IENABLE);
	val &= ~BIT(desc->hwirq);
	arc_write_aux_reg(ARC_AUX_IENABLE, val);
}

static void arc_intc_unmask(struct irq_desc *desc)
{
	u32 val;

	if (WARN_ON(desc->hwirq > 31))
		return;

	val = arc_read_aux_reg(ARC_AUX_IENABLE);
	val |= BIT(desc->hwirq);
	arc_write_aux_reg(ARC_AUX_IENABLE, val);
}

static int arc_intc_map(struct irq_domain *d, unsigned int virq,
		unsigned int hwirq)
{
	return 0;
}

static const struct irq_domain_ops arc_intc_domain_ops = {
	.map = arc_intc_map,
	.mask = arc_intc_mask,
	.unmask = arc_intc_unmask,
};

static int arc_intc_init(const struct ofnode *node)
{
	struct irq_domain *domain;

	domain = irq_domain_add_linear(node->compatible, &arc_intc_domain_ops,
			NR_CPU_IRQS);
	if (!domain)
		PANIC("failed to create IRQ root domain\n");

	irq_set_default_domain(domain);
	return 0;
}
OFDEVICE_DEFINE(arc_intc, "snps,arc700-intc", arc_intc_init);
