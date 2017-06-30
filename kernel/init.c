/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bug.h>
#include <arcOS/compiler-gcc.h>
#include <arcOS/device.h>
#include <arcOS/init.h>
#include <arcOS/init-task.h>
#include <arcOS/irq.h>
#include <arcOS/ksymbol.h>
#include <arcOS/params.h>
#include <arcOS/printk.h>
#include <arcOS/serial.h>
#include <arcOS/spinlock.h>
#include <arcOS/string.h>
#include <arcOS/task.h>
#include <arcOS/types.h>
#include <asm/setup.h>
#include <asm/stacktrace.h>

char *boot_command_line = CONFIG_BOOT_COMMAND_LINE;
static int early_params_done = 0;

static int do_early_param(char *param, char *val)
{
	const struct kearlyparam_id *p;

	for (p = __kearlyparam_table; p < __kearlyparam_table_end; p++) {
		if (parameq(param, p->str) && p->setup)
			p->setup(val);
	}

	return 0;
}

void parse_early_params(void)
{
	char tmp_cmdline[COMMAND_LINE_SIZE];

	if (early_params_done)
		return;

	strlcpy(tmp_cmdline, boot_command_line, COMMAND_LINE_SIZE);
	parse_args(tmp_cmdline, do_early_param);
	early_params_done = 1;
}

void start_kernel(void)
{
	spinlock_t lock;
	char *cmdline;

	task_set_end_magic(&init_task);
	local_irq_disable();
	setup_arch(&cmdline);
	parse_early_params();
	driver_init();

	/* All done */
	printk("\narcOS v%d.%d\n\n", ARCOS_VERSION, ARCOS_SUBVERSION);
	arch_local_irq_enable();

	while (!is_task_corrupted(&init_task))
		;
	PANIC("Init task stack got corrupted!\n");
}
