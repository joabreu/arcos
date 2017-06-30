/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_IO_H__
#define __ARC_IO_H__

#include <arcOS/types.h>

#define ioremap ioremap
void *ioremap(phys_addr_t paddr, size_t size);

#endif /* __ARC_IO_H__ */
