/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/ctype.h>
#include <arcOS/kernel.h>
#include <arcOS/ksymbol.h>
#include <arcOS/printk.h>
#include <arcOS/vsprintf.h>

static const struct ksymbol *get_ksymbol(unsigned long address)
{
	const struct ksymbol *sym, *last_sym = NULL;

	for (sym = __ksymbols_table; sym < __ksymbols_table_end; sym++) {
		if ((sym->addr > address) && (tolower(last_sym->type) == 't'))
			break;
		last_sym = sym;
	}

	return last_sym;
}

void print_all_ksymbols(void)
{
	const struct ksymbol *sym;

	for (sym = __ksymbols_table; sym < __ksymbols_table_end; sym++) {
		printk("addr=0x%lx, type=%x, name=%s\n",
				sym->addr, sym->type, sym->name);
	}
}

void print_ksymbol(const char *fmt, unsigned long address)
{
	const struct ksymbol *sym = get_ksymbol(address);
	char buf[KSYMBOL_BUFFER_LEN];
	int n = 0, len = KSYMBOL_BUFFER_LEN;

	if (sym) {
		n += scnprintf(buf + n, len - n, "%s (0x%lx+0x%lx)",
				sym->name, sym->addr, address - sym->addr);
	} else {
		n += scnprintf(buf + n, len - n, "<unknown> (0x%lx)",
				address);
	}

	printk(fmt, buf);
}

