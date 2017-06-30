/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bitops.h>
#include <arcOS/io.h>
#include <arcOS/serial.h>

static void uart_arc_putchar(struct earlycon_device *edev, const unsigned char c)
{
	struct uart_port *port = &edev->port;

	while (!(readb(port->membase + 20) & BIT(7)))
		;

	writeb(c, port->membase + 16);
}

static int uart_arc_setup(struct earlycon_device *edev, const char *options)
{
	struct uart_port *port = &edev->port;
	unsigned int l, h, hw_val;

	if (!port->membase)
		return -ENODEV;

	hw_val = port->clk / (edev->baud * 4) - 1;
	l = hw_val & GENMASK(7, 0);
	h = (hw_val >> 8) & GENMASK(7, 0);

	writeb(l, port->membase + 24);
	writeb(h, port->membase + 28);

	port->putchar = uart_arc_putchar;
	return 0;
}
EARLYCON_DECLARE(uart_arc, uart_arc_setup);
