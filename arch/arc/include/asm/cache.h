/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_CACHE_H__
#define __ARC_CACHE_H__

#define ARC_UNCACHED_ADDR_SPACE		0xc0000000

extern unsigned long arc_perip_base, arc_perip_end;

char *arc_format_cacheinfo(int cpu_id, char *buf, int len);

#endif /* __ARC_CACHE_H__ */
