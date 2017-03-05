/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/errno.h>
#include <arcOS/printk.h>
#include <arcOS/string.h>
#include <arcOS/types.h>
#include <asm/unwind.h>

/* From linker script */
extern const char __unwind_start[], __unwind_end[];

static bool is_cie(u32 id)
{
	return id == 0x0;
}

int arc_unwind(struct unwind_info *info)
{
	unsigned long i, size = __unwind_end - __unwind_start;
	u32 length, id;
	u8 *ptr;

	for (i = 0; i < size; ) {
		ptr = (u8 *)&__unwind_start[i];

		memcpy(&length, ptr, sizeof(length));
		ptr += sizeof(length);

		memcpy(&id, ptr, sizeof(id));
		ptr += sizeof(id);

		if ((u32)~0UL == length)
			return -EINVAL; /* 64-bit DWARF format not supported */
		if (is_cie(id)) {
			u8 version = *ptr++;
			const char *str = (const char *)ptr;
			printk("unwind: found CIE: length=%d, version=0x%x, str=%s\n",
					length, version, str);
		} else {
			s32 fde_offset = (s32)id;
			printk("unwind: found FDE: length=%d, offset=%d\n",
					length, fde_offset);
		}

		i += length + 4;
	}

	return 0;
}

