/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_INIT_H__
#define __ARCOS_INIT_H__

#include <arcOS/compiler-gcc.h>

struct kearlyparam_id {
	const char *str;
	int (*setup)(char *args);
} __aligned(32);

/* From linker script */
extern const struct kearlyparam_id __kearlyparam_table[];
extern const struct kearlyparam_id __kearlyparam_table_end[];

#define KEARLYPARAM_DECLARE(n, f) \
	static const struct kearlyparam_id __kearlyparam_##f \
		__used __section(__kearlyparam_table) = { \
			.str = n, \
			.setup = f, \
		}

/* From kernel/init.c */
extern char *boot_command_line;
void parse_early_params(void);
void start_kernel(void);

/* Arch dependent */
void setup_arch(char **cmdline);

#endif /* __ARCOS_INIT_H__ */
