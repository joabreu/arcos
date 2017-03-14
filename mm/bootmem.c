/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/errno.h>
#include <arcOS/bitops.h>
#include <arcOS/bootmem.h>
#include <arcOS/bug.h>
#include <arcOS/io.h>
#include <arcOS/kernel.h>
#include <arcOS/page.h>
#include <arcOS/pfn.h>
#include <arcOS/printk.h>
#include <arcOS/string.h>

struct bootmem {
	unsigned long start_pfn;
	unsigned long end_pfn;
	void *map;
	unsigned long curr_pfn;
};

struct bootmem bootmem;

static unsigned long bootmap_bytes(unsigned long pages)
{
	pages = DIV_ROUND_UP(pages, 8); /* each byte accomodates 8 pages */

	return ALIGN(pages, sizeof(long));
}

static void *bootmem_alloc_data(struct bootmem *binfo, size_t size)
{
	unsigned long min, max;
	unsigned long free_idx;

	if (!binfo || !binfo->map)
		return NULL;

	min = binfo->start_pfn;
	max = binfo->end_pfn;

	while(1) {
		/* find free block */
		free_idx = find_next_zero_bit(binfo->map, max, min);
		if (free_idx >= max)
			return NULL;
	}
}

static void *bootmem_alloc_flags(size_t size, int flags)
{
	unsigned long size_pfn = PFN_UP(size);
	unsigned long i, idx;
	void *tmp;

again:
	tmp = phys_to_virt(PFN_PHYS(bootmem.curr_pfn));

	if ((bootmem.curr_pfn + size_pfn) >= bootmem.end_pfn)
		PANIC("Bootmem memory got exausted\n");

	for (i = 0; i < size_pfn; i++) {
		idx = bootmem.curr_pfn + i - bootmem.start_pfn;

		if (test_and_set_bit(idx, bootmem.map)) {
			bootmem.curr_pfn += i + 1;
			goto again;
		}
	}

	bootmem.curr_pfn += size_pfn;

	if (flags & BOOTMEM_FLAGS_ZERO)
		memset(tmp, 0, size);

	printk("bootmem: alloc=0x%x, size=%d, flags=0x%x, curr_pfn=%ld\n",
			(unsigned int)tmp, size, flags, bootmem.curr_pfn);
	return tmp;
}

static bool bootmem_mark_region(unsigned long start_pfn, unsigned long end_pfn,
		bool reserve)
{
	unsigned long i;

	if (end_pfn < start_pfn)
		return false;
	if (start_pfn < bootmem.start_pfn || end_pfn > bootmem.end_pfn)
		return false;

	end_pfn -= bootmem.start_pfn;
	start_pfn -= bootmem.start_pfn;

	for (i = start_pfn; i <= end_pfn; i++) {
		if (reserve) {
			if (test_and_set_bit(i, bootmem.map))
				return false;
		} else {
			if (!test_and_clear_bit(i, bootmem.map))
				return false;
		}
	}

	return true;
}

void bootmem_mark_free_region(unsigned long start_pfn, unsigned long end_pfn)
{
	if (!bootmem_mark_region(start_pfn, end_pfn, false))
		PANIC("bootmem: Tried to free already freed region\n");
}

void bootmem_mark_reserved_region(unsigned long start_pfn, unsigned long end_pfn)
{
	if (!bootmem_mark_region(start_pfn, end_pfn, true))
		PANIC("bootmem: Tried to reserve already reserved region\n");
}

void *bootmem_zalloc(size_t size)
{
	return bootmem_alloc_flags(size, BOOTMEM_FLAGS_ZERO);
}

int init_bootmem(unsigned long map_pfn, unsigned long start_pfn,
		unsigned long end_pfn)
{
	unsigned long mapsize;

	bootmem.start_pfn = start_pfn;
	bootmem.end_pfn = end_pfn;
	bootmem.map = phys_to_virt(PFN_PHYS(map_pfn));

	mapsize = bootmap_bytes(end_pfn - start_pfn);
	memset(bootmem.map, 0x00, mapsize);

	bootmem.curr_pfn = PFN_UP(virt_to_phys(bootmem.map) + mapsize);
	if (bootmem.curr_pfn >= end_pfn)
		PANIC("Not enough bootmem memory\n");

	bootmem_mark_reserved_region(map_pfn, bootmem.curr_pfn - 1);

	printk("bootmem: start_pfn=%ld, end_pfn=%ld, mapaddr=0x%x (%ld)\n",
			start_pfn, end_pfn, (unsigned int)bootmem.map, mapsize);
	printk("bootmem: map_pfn=%ld, curr_pfn=%ld\n",
			map_pfn, bootmem.curr_pfn);
	return 0;
}

