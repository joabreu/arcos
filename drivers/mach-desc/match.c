/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/mach-desc.h>
#include <arcOS/params.h>
#include <arcOS/string.h>
#include <arcOS/types.h>

static const struct machine_desc *machdesc_find(char *param, char *val)
{
	const struct machine_desc *m;

	for (m = __machine_desc_table; m < __machine_desc_table_end; m++) {
		if (parameq(param, "machine") &&
		    parameq(val, m->name))
			return m;
	}

	return NULL;
}

const struct machine_desc *machdesc_match(char *cmdline)
{
	const struct machine_desc *match;

	cmdline = skip_spaces(cmdline);

	while (*cmdline) {
		char *param, *val;

		cmdline = next_arg(cmdline, &param, &val);
		match = machdesc_find(param, val);
		if (match)
			return match;
	}

	return NULL;
}
