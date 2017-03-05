/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/init.h>
#include <arcOS/io.h>
#include <arcOS/printk.h>
#include <arcOS/serial.h>
#include <arcOS/string.h>
#include <arcOS/types.h>
#include <arcOS/vsprintf.h>

static struct earlycon_device early_console = {
	.putchar = NULL,
};

static int earlycon_parse(char *args, struct earlycon_device *edev)
{
	edev->mapbase = simple_strtoull(args, NULL, 0);
	if (!edev->mapbase)
		return -EINVAL;

	edev->membase = ioremap(edev->mapbase, 64);
	if (!edev->membase)
		return -ENOMEM;

	args = strchr(args, ',');
	if (args)
		args++;

	edev->baud = simple_strtoul(args, NULL, 10);
	if (!edev->baud)
		return -EINVAL;

	while (*args >= '0' && *args <= '9')
		args++;
	if (*args)
		edev->parity = *args++;
	if (*args)
		edev->bits = *args++ - '0';
	if (*args)
		edev->flow = *args;

	return 0;
}

static int earlycon_register(char *args, const struct earlycon_id *eid)
{
	int ret;

	if (earlycon_parse(args, &early_console))
		return -EINVAL;
	if (!eid->setup)
		return -EINVAL;

	ret = eid->setup(&early_console, args);
	if (ret)
		return ret;
	if (!early_console.putchar)
		return -ENODEV;

	printk("\nearlycon: %s at 0x%x (options '%s')\n", eid->name,
			(unsigned int)early_console.membase, args);
	return 0;
}

static int earlycon_setup(char *args)
{
	const struct earlycon_id *eid;

	if (!args || !args[0])
		return 0; /* No earlycon supplied */

	for (eid = __earlycon_table; eid < __earlycon_table_end; eid++) {
		size_t len = strlen(eid->name);

		if (strncmp(args, eid->name, len))
			continue;

		if (args[len]) {
			if (args[len] != ',')
				continue;
			args += len + 1;
		} else {
			args = NULL;
		}

		return earlycon_register(args, eid);
	}

	return -ENODEV;
}
KEARLYPARAM_DECLARE("earlycon", earlycon_setup);

void write_earlycon(const unsigned char c)
{
	if (!early_console.putchar)
		return;
	early_console.putchar(&early_console, c);
}

