/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bootmem.h>
#include <arcOS/irq.h>
#include <arcOS/printk.h>
#include <asm/irq.h>

static struct irq_domain *irq_default_domain = NULL;

static unsigned int get_hwirq(struct irq_domain *domain, unsigned int irq)
{
	return irq;
}

static unsigned int get_irq(struct irq_domain *domain, unsigned int hwirq)
{
	return hwirq;
}

static struct irq_entry *get_irqentry(struct irq_domain *domain,
		unsigned int irq)
{
	irq = get_hwirq(domain, irq);
	if (irq >= domain->hwirq_max)
		return NULL;
	if (!domain->entries)
		return NULL;
	return domain->entries + irq;
}

void irq_set_default_domain(struct irq_domain *domain)
{
	printk("irq: default domain is now 0x%x\n", (unsigned int)domain);
	irq_default_domain = domain;
}

struct irq_domain *irq_domain_add_linear(const char *name,
		const struct irq_domain_ops *ops, unsigned int hwirq_max)
{
	struct irq_domain *domain;

	domain = bootmem_zalloc(sizeof(*domain));
	if (!domain)
		return NULL;

	domain->entries = bootmem_zalloc(sizeof(*domain->entries) * hwirq_max);
	if (!domain->entries)
		return NULL;

	domain->name = name;
	domain->ops = ops;
	domain->hwirq_max = hwirq_max;
	printk("irq: created domain at 0x%x [%d irqs]\n", (unsigned int)domain,
			hwirq_max);
	return domain;
}

void handle_domain_irq(struct irq_domain *domain, unsigned int hwirq,
		struct pt_regs *regs)
{
	struct irq_entry *entry;

	if (!domain)
		domain = irq_default_domain;
	if (!domain)
		return;

	entry = get_irqentry(domain, get_irq(domain, hwirq));
	if (!entry || !entry->handler) {
		printk("irq: unmapped irq entry [hwirq=%d]\n", hwirq);
		return;
	}

	printk("irq: hwirq[%d]:\n", hwirq);
	if (regs) {
		printk("\tbta_l1=0x%x\n", regs->bta_l1);
		printk("\tlp_start=0x%x, lp_end=0x%x, lp_count=0x%x\n",
				regs->lp_start, regs->lp_end, regs->lp_count);
		printk("\tstatus32_l1=0x%x\n", regs->status32_l1);
		printk("\tilink1=0x%x, blink=0x%x\n", regs->ilink1, regs->blink);
		printk("\tfp=0x%x, gp=0x%x, sp=0x%x\n", regs->fp, regs->gp,
				regs->sp);
		printk("\tr12=0x%x, r11=0x%x, r10=0x%x, r9=0x%x, r8=0x%x\n",
				regs->r12, regs->r11, regs->r10, regs->r9,
				regs->r8);
		printk("\tr7=0x%x, r6=0x%x, r5=0x%x, r4=0x%x, r3=0x%x\n",
				regs->r7, regs->r6, regs->r5, regs->r4,
				regs->r3);
		printk("\tr2=0x%x, r1=0x%x, r0=0x%x\n", regs->r2, regs->r1,
				regs->r0);
	}

	entry->handler(entry->desc.irq, entry->devid);
}

int request_irq(unsigned int irq, int (*handler)(int irq, void *devid),
		const char *devname, void *devid)
{
	struct irq_domain *domain = irq_default_domain;
	struct irq_entry *entry;

	if (!domain || !handler)
		return -EINVAL;

	entry = get_irqentry(domain, irq);
	if (!entry)
		return -EINVAL;

	entry->devname = devname;
	entry->desc.irq = irq;
	entry->desc.hwirq = get_hwirq(domain, irq);
	entry->handler = handler;
	entry->devid = devid;

	if (domain->ops->unmask)
		domain->ops->unmask(&entry->desc);

	printk("irq: registered hwirq[%d] at 0x%x\n", get_hwirq(domain, irq),
			(unsigned int)entry);
	return 0;
}
