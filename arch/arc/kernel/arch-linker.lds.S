/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <asm/page.h>
#include <asm-generic/arch-linker.lds.h>

ENTRY(__arc_entry)
SECTIONS
{
	. = CONFIG_ARCOS_LINK_BASE;

	__intvector_base = .;
	.intvector : {
		*(.intvector)
		. = ALIGN(PAGE_SIZE);
	}

	__text_start = .;
	.text : {
		*(.text)
		. = ALIGN(PAGE_SIZE);
	}
	__text_end = .;

	__data_start = .;
	.data : {
		*(.data)
		*(.rodata)

		INIT_DATA

		. = ALIGN(PAGE_SIZE);
	}
	__data_end = .;

	__bss_start = .;
	.bss : {
		*(.bss)
		. = ALIGN(PAGE_SIZE);
	}
	__bss_end = .;

#ifdef CONFIG_ARC_UNWIND
	__unwind_start = .;
	.eh_frame : {
		*(.eh_frame)
		. = ALIGN(PAGE_SIZE);
	}
	__unwind_end = .;
#else
	/DISCARD/ : { *(.eh_frame) }
#endif

	/DISCARD/ : {
		*(.comment)
		*(.note)
	}

	__end = .;
}
