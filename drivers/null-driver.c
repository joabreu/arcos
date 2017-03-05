/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/of.h>
#include <arcOS/printk.h>

static int null_driver_setup(const struct ofnode *node)
{
	printk("%s: int=%d, mem@0x%x-0x%x, pdata=0x%x\n",
			node->compatible, node->interrupt,
			node->memstart, node->memend,
			(unsigned int)node->pdata);

	return 0;
}
OFDEVICE_DEFINE(null_driver, "snps,null-driver", null_driver_setup);

