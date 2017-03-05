/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/of.h>
#include <arcOS/types.h>

OFNODE_DEFINE(null1) {
	.compatible = "snps,null-driver",
	.interrupt = 1,
	.memstart = 0xdeadbeef,
	.memend = 0xdeadcafe,
	.pdata = NULL,
	OFNODE_DEPS_NULL(),
};

OFNODE_DEFINE(null2) {
	.compatible = "snps,null-driver",
	.interrupt = 2,
	.memstart = 0xdeadcafe,
	.memend = 0xdeadbeef,
	.pdata = NULL,
	OFNODE_DEPS_NULL(),
};

OFNODE_DEFINE(void3) {
	.compatible = "snps,void-driver",
	.interrupt = 0,
	.memstart = 0x0,
	.memend = 0x0,
	.pdata = NULL,
	OFNODE_DEPS_NULL(),
};

OFNODE_DEFINE(arc_intc) {
	.compatible = "snps,arc700-intc",
	.interrupt = 0,
	.memstart = 0x0,
	.memend = 0x0,
	.pdata = NULL,
	OFNODE_DEPS_NULL(),
};

OFNODE_DEPS_DEFINE(arc_timer) {
	&OFNODE_FORMAT_NAME(arc_intc),
	&OFNODE_FORMAT_NAME(void3),
	&OFNODE_FORMAT_NAME(null2),
};

OFNODE_DEFINE(arc_timer) {
	.compatible = "snps,arc-timer",
	.interrupt = 3,
	.memstart = 0xdeadbeef,
	.memend = 0xdeadcafe,
	.pdata = NULL,
	OFNODE_DEPS(arc_timer),
};

