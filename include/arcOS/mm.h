/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_MM_H__
#define __ARCOS_MM_H__

struct mm_info {
	unsigned long start_code, end_code;
	unsigned long start_data, end_data;
};

extern struct mm_info init_mm;

#endif /* __ARCOS_MM_H__ */
