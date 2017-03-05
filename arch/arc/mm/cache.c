/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/vsprintf.h>
#include <asm/cache.h>

unsigned long arc_perip_base = ARC_UNCACHED_ADDR_SPACE;
unsigned long arc_perip_end = 0xFFFFFFFF;

char *arc_format_cacheinfo(int cpu_id, char *buf, int len)
{
	int n = 0;

	n += scnprintf(buf + n, len - n, "Peripherals\t: 0x%x\n", arc_perip_base);
	n += scnprintf(buf + n, len - n, "\t\t: 0x%x\n", arc_perip_end);

	return buf;
}

