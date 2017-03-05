/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __SOC_ARC_TIMERS_H__
#define __SOC_ARC_TIMERS_H__

#include <arcOS/bitops.h>

#define ARC_TIMER0_CNT		0x21
#define ARC_TIMER0_CTRL		0x22
#define ARC_TIMER0_LIMIT	0x23
#define ARC_TIMER1_CNT		0x100
#define ARC_TIMER1_CTRL		0x101
#define ARC_TIMER1_LIMIT	0x102
#define ARC_TIMER_CTRL_IE	BIT(0)
#define ARC_TIMER_CTRL_NH	BIT(1)
#define ARC_TIMERN_MAX		0xFFFFFFFF
#define ARC_TIMER_BUILD		0x75 /* BCR register */

struct arc_bcr_timer {
#ifdef CONFIG_CPU_BIG_ENDIAN
	unsigned int pad2:15, rtsc:1, pad1:5, rtc:1, t1:1, t0:1, ver:8;
#else
	unsigned int ver:8, t0:1, t1:1, rtc:1, pad1:5, rtsc:1, pad2:15;
#endif
};

#endif

