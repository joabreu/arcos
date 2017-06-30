/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/init.h>
#include <arcOS/io.h>
#include <arcOS/kernel.h>
#include <arcOS/printk.h>
#include <arcOS/serial.h>
#include <arcOS/string.h>
#include <arcOS/types.h>
#include <arcOS/vsprintf.h>

static struct earlycon_device early_console = {
	.name = "earlycon",
};

static int earlycon_parse_mem(char *args, unsigned char *iotype,
		resource_size_t *addr, char **options)
{
	if (strncmp(args, "mmio,", 5) == 0) {
		*iotype = UART_IOTYPE_MEM;
		args += 5;
	} else if (strncmp(args, "mmio16,", 7) == 0) {
		*iotype = UART_IOTYPE_MEM16;
		args += 7;
	} else if (strncmp(args, "mmio32,", 7) == 0) {
		*iotype = UART_IOTYPE_MEM32;
		args += 7;
	} else if (strncmp(args, "0x", 2) == 0) {
		*iotype = UART_IOTYPE_MEM;
	} else {
		return -EINVAL;
	}

	*addr = simple_strtoul(args, NULL, 0);
	args = strchr(args, ',');
	if (args)
		args++;

	*options = args;
	return 0;
}

static void earlycon_parse_options(char *options, unsigned int *baud,
		unsigned int *parity, unsigned int *bits,
		unsigned int *flow)
{
	char *str = options;

	*baud = simple_strtoul(str, NULL, 10);
	while (*str >= '0' && *str <= '9')
		str++;
	if (*str)
		*parity = *str++;
	if (*str)
		*bits = *str++ - '0';
	if (*str)
		*flow = *str;
}

static int earlycon_parse(struct earlycon_device *edev, char *args)
{
	struct uart_port *port = &edev->port;
	int ret, len;

	ret = earlycon_parse_mem(args, &port->iotype, &port->mapbase, &args);
	if (ret)
		return ret;

	switch (port->iotype) {
	case UART_IOTYPE_MEM:
		port->regshift = 0;
		break;
	case UART_IOTYPE_MEM16:
		port->regshift = 1;
		break;
	case UART_IOTYPE_MEM32:
		port->regshift = 2;
		break;
	default:
		return -EINVAL;
	}

	port->membase = ioremap(port->mapbase, 64);
	if (!port->membase)
		return -ENOMEM;

	if (args) {
		earlycon_parse_options(args, &edev->baud, &edev->parity,
				&edev->bits, &edev->flow);
		len = min(strlen(args) + 1, sizeof(edev->options));
		strlcpy(edev->options, args, len);
	}

	return 0;
}

static void *earlycon_map(resource_size_t paddr, size_t size)
{
	return ioremap(paddr, size);
}

static int earlycon_register(char *args, const struct earlycon_id *eid)
{
	struct uart_port *port = &early_console.port;
	int ret;

	if (!eid->setup)
		return -EINVAL;
	if (args && earlycon_parse(&early_console, args))
		return -EINVAL;

	if (port->mapbase) {
		port->membase = earlycon_map(port->mapbase, 64);
		if (!port->membase)
			return -ENOMEM;
	}

	ret = eid->setup(&early_console, args);
	if (ret)
		return ret;
	if (!port->putchar)
		return -ENODEV;

	printk("\nearlycon: %s mapped at 0x%x (options '%s')\n", eid->name,
			(unsigned int)port->membase, early_console.options);
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
	if (!early_console.port.putchar)
		return;
	early_console.port.putchar(&early_console, c);
}
