/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_MACH_DESC_H__
#define __ARCOS_MACH_DESC_H__

#include <arcOS/compiler-gcc.h>

struct machine_desc {
	const char *name;
	void (*setup_early)(const struct machine_desc *mdesc);
} __aligned(32);

/* From linker script */
extern const struct machine_desc __machine_desc_table[];
extern const struct machine_desc __machine_desc_table_end[];

#define MACHDESC_DECLARE(t, n, f) \
	static const struct machine_desc __machine_desc_##t \
		__used __section(__machine_desc_table) = { \
			.name = n, \
			.setup_early = f, \
		}

const struct machine_desc *machdesc_match(char *cmdline);

#endif /* __ARCOS_MACH_DESC_H__ */
