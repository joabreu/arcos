/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/of.h>
#include <arcOS/types.h>

OFNODE_DEFINE(arc_intc) {
	.compatible = "snps,arc700-intc",
	.interrupt = 0,
	.memstart = 0x0,
	.memend = 0x0,
	.pdata = NULL,
	OFNODE_DEPS_NULL(),
};

OFNODE_DEPS_DEFINE(arc_uart) {
	&OFNODE_FORMAT_NAME(arc_intc),
};

/*
 * uart1: 0xe0020000, int=17
 * uart2: 0xe0021000, int=18
 */

OFNODE_DEFINE(arc_uart3) {
	.compatible = "snps,dw-apb-uart",
	.interrupt = 19,
	.memstart = 0xe0022000,
	.memend = 0xe00220ff,
	.pdata = NULL,
	OFNODE_DEPS(arc_uart),
};
