/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bitops.h>
#include <arcOS/io.h>
#include <arcOS/serial.h>

static void uart_8250_putchar(struct earlycon_device *dev, const unsigned char c)
{
	/* */
}

static int uart_8250_setup(struct earlycon_device *dev, const char *options)
{
	dev->port.putchar = uart_8250_putchar;
	return 0;
}
EARLYCON_DECLARE(uart_8250, uart_8250_setup);
