/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_KSYMBOL_H__
#define __ARCOS_KSYMBOL_H__

#include <arcOS/compiler-gcc.h>

#define KSYMBOL_BUFFER_LEN		512 

struct ksymbol {
	unsigned long addr;
	char type;
	char *name;
};

extern const struct ksymbol __ksymbols_table[];
extern const struct ksymbol __ksymbols_table_end[];

#define KSYMBOL_TABLE() \
	static const struct ksymbol __ksymbols_real_table[] \
		__used __section(__ksymbols_table)

void print_all_ksymbols(void);
void print_ksymbol(const char *fmt, unsigned long address);

#endif

