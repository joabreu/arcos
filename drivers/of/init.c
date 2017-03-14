/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bug.h>
#include <arcOS/bootmem.h>
#include <arcOS/of.h>
#include <arcOS/printk.h>
#include <arcOS/string.h>

struct of_info {
	struct ofdevice *devs;
	unsigned int devs_count;
	unsigned int devs_max_count;
};

static struct of_info of_info;

static const struct ofdevice_id *of_find_match(const struct ofnode *node)
{
	const struct ofdevice_id *oid;

	for_each_ofdevice_id(oid) {
		if (!oid->compatible)
			continue;
		if (!strcmp(node->compatible, oid->compatible) && oid->setup)
			return oid;
	}

	return NULL;
}

static unsigned int of_count_available(void)
{
	const struct ofnode *node;
	unsigned int count = 0;

	for_each_ofnode(node) {
		if (of_find_match(node))
			count++;
	}

	printk("of: found %d valid entries\n", count);
	return count;
}

static void of_populate_initial(struct of_info *info)
{
	const struct ofdevice_id *oid;
	const struct ofnode *node;

	for_each_ofnode(node) {
		struct ofdevice *dev = info->devs + info->devs_count;

		oid = of_find_match(node);
		if (!oid)
			continue;

		dev->node = node;
		dev->id = oid;
		dev->has_deps = node->deps && (node->deps_count > 0);
		dev->inited = false;

		info->devs_count++;
		printk("of: new dev 0x%x: %s\n", (unsigned int)dev,
				node->compatible);
	}
}

static struct ofdevice *of_find_dev(struct of_info *info, const struct ofnode *node)
{
	struct ofdevice *dev;
	int i;

	for_each_ofdevice(i, dev, info) {
		if (dev->node == node)
			return dev;
	}

	return NULL;
}

static int of_start_dev(struct of_info *info, struct ofdevice *dev)
{
	if (!dev)
		return -EINVAL;
	if (dev->inited)
		return 0;

	printk("of: starting dev 0x%x\n", (unsigned int)dev);

	if (dev->has_deps) {
		const struct ofnode *dep;
		struct ofdevice *dep_dev;
		int i;

		printk("of: %s has %d deps\n", dev->node->compatible,
				dev->node->deps_count);
		for (i = 0; i < dev->node->deps_count; i++) {
			dep = dev->node->deps[i];
			dep_dev = of_find_dev(info, dep);

			if (!dep_dev)
				continue;

			printk("of: %s starting dep %s\n",
					dev->node->compatible, dep->compatible);
			of_start_dev(info, dep_dev);
		}
	}

	dev->inited = true;
	return dev->id->setup(dev->node);
}

void of_init(void)
{
	unsigned int dev_count = of_count_available();
	struct ofdevice *dev;
	int i;

	if (!dev_count)
		return; /* Nothing to do here */

	of_info.devs = bootmem_zalloc(dev_count * sizeof(*of_info.devs));
	if (!of_info.devs)
		PANIC("Not enough memory to OF devices\n");

	of_info.devs_count = 0;
	of_info.devs_max_count = dev_count;

	of_populate_initial(&of_info);
	for_each_ofdevice(i, dev, &of_info)
		of_start_dev(&of_info, dev);
}

