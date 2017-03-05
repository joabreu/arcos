/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#define ALIGN_STRUCT()		. = ALIGN(32)

#define KEARLYPARAM_TABLE() \
	ALIGN_STRUCT(); \
	__kearlyparam_table = .; \
	*(__kearlyparam_table) \
	__kearlyparam_table_end = .;

#define OFNODE_TABLE() \
	ALIGN_STRUCT(); \
	__ofnode_table = .; \
	*(__ofnode_table) \
	__ofnode_table_end = .;

#define OFDEVICE_TABLE() \
	ALIGN_STRUCT(); \
	__ofdevice_table = .; \
	*(__ofdevice_table) \
	__ofdevice_table_end = .;

#define MACHDESC_TABLE() \
	ALIGN_STRUCT(); \
	__machine_desc_table = .; \
	*(__machine_desc_table) \
	__machine_desc_table_end = .;

#define KSYMBOLS_TABLE() \
	ALIGN_STRUCT(); \
	__ksymbols_table = .; \
	*(__ksymbols_table) \
	__ksymbols_table_end = .;

#define EARLYCON_TABLE() \
	ALIGN_STRUCT(); \
	__earlycon_table = .; \
	*(__earlycon_table) \
	__earlycon_table_end = .;

#define INIT_DATA \
	KEARLYPARAM_TABLE() \
	OFNODE_TABLE() \
	OFDEVICE_TABLE() \
	MACHDESC_TABLE() \
	KSYMBOLS_TABLE() \
	EARLYCON_TABLE()

