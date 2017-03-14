/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARC_SERIAL_H__
#define __ARC_SERIAL_H__

#include <arcOS/kernel.h>

#if IS_ENABLED(CONFIG_MACHINE_AXS10X)
#define BASE_BAUD	50000000
#else
#define BASE_BAUD	33333333
#endif

#endif

