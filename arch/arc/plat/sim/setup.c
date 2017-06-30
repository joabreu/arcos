/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/printk.h>
#include <arcOS/mach-desc.h>

static void setup_early(const struct machine_desc *mdesc)
{
	printk("%s: %s\n", mdesc->name, __func__);
}
MACHDESC_DECLARE(SIMULATION, "snps,nsim", setup_early);
