/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_IRQ_H__
#define __ARCOS_IRQ_H__

#include <asm/irq.h>

#define local_irq_disable()		arch_local_irq_disable()

struct irq_domain;

struct irq_desc {
	unsigned int irq;
	unsigned int hwirq;
};

struct irq_entry {
	const char *devname;
	struct irq_desc desc;
	int (*handler)(int irq, void *devid);
	void *devid;
};

struct irq_domain_ops {
	int (*map)(struct irq_domain *d, unsigned int virq, unsigned int hwirq);
	void (*mask)(struct irq_desc *desc);
	void (*unmask)(struct irq_desc *desc);
};

struct irq_domain {
	const char *name;
	const struct irq_domain_ops *ops;
	unsigned int hwirq_max;
	struct irq_entry *entries;
};

void irq_set_default_domain(struct irq_domain *domain);
struct irq_domain *irq_domain_add_linear(const char *name,
		const struct irq_domain_ops *ops, unsigned int hwirq_max);
void handle_domain_irq(struct irq_domain *domain, unsigned int hwirq,
		struct pt_regs *regs);
int request_irq(unsigned int irq, int (*handler)(int irq, void *devid),
		const char *devname, void *devid);

#endif /* __ARCOS_IRQ_H__ */
