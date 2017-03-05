/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_OF_H__
#define __ARCOS_OF_H__

#include <arcOS/kernel.h>
#include <arcOS/types.h>

struct ofnode {
	const char *compatible;
	unsigned int interrupt;
	u32 memstart;
	u32 memend;
	u32 *pdata;
	const struct ofnode **deps;
	unsigned int deps_count;
} __aligned(32);

struct ofdevice_id {
	const char *compatible;
	int (*setup)(const struct ofnode *node);
} __aligned(32);

struct ofdevice {
	const struct ofnode *node;
	const struct ofdevice_id *id;
	bool has_deps;
	bool inited;
};

/* From linker script */
extern const struct ofnode __ofnode_table[];
extern const struct ofnode __ofnode_table_end[];
extern const struct ofdevice_id __ofdevice_table[];
extern const struct ofdevice_id __ofdevice_table_end[];

#define OFDEVICE_DEFINE(n, c, f) \
	static const struct ofdevice_id __ofdevice_##n \
		__used __section(__ofdevice_table) = { \
			.compatible = c, \
			.setup = f, \
		}

#define OFNODE_FORMAT_NAME(n)		__ofnode_##n
#define OFNODE_FORMAT_DEPS_NAME(n)	__ofnode_deps_##n

#define OFNODE_DEFINE(n) \
	static const struct ofnode OFNODE_FORMAT_NAME(n) \
		__used __section(__ofnode_table) =
#define OFNODE_DEPS_DEFINE(n) \
	static const struct ofnode *OFNODE_FORMAT_DEPS_NAME(n)[] =

#define OFNODE_DEPS(n) \
	.deps = OFNODE_FORMAT_DEPS_NAME(n), \
	.deps_count = ARRAY_SIZE(OFNODE_FORMAT_DEPS_NAME(n))
#define OFNODE_DEPS_NULL() \
	.deps = NULL, \
	.deps_count = 0

#define for_each_ofnode(__node) \
	for ((__node) = __ofnode_table; (__node) < __ofnode_table_end; \
			(__node)++)
#define for_each_ofdevice_id(__oid) \
	for ((__oid) = __ofdevice_table; (__oid) < __ofdevice_table_end; \
			(__oid)++)
#define for_each_ofdevice(__i, __dev, __info) \
	for ((__i) = 0, (__dev) = (__info)->devs; \
			(__i) < (__info)->devs_max_count; \
			(__i)++, (__dev)++)

void of_init(void);

#endif

