/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bitops.h>
#include <arcOS/io.h>
#include <arcOS/serial.h>

static void arc_uart_putchar(struct earlycon_device *dev, const unsigned char c)
{
	while (!(readb(dev->membase + 20) & BIT(7)))
		;

	writeb(c, dev->membase + 16);
}

static int arc_uart_setup(struct earlycon_device *dev, const char *options)
{
	unsigned int l, h, hw_val;

	if (!dev->membase)
		return -ENODEV;

	hw_val = dev->baud / (dev->bits * 4) - 1;
	l = hw_val & GENMASK(7, 0);
	h = (hw_val >> 8) & GENMASK(7, 0);

	writeb(l, dev->membase + 24);
	writeb(h, dev->membase + 28);

	dev->putchar = arc_uart_putchar;
	return 0;
}
EARLYCON_DECLARE(arc_uart, arc_uart_setup);

