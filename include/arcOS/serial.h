/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_SERIAL_H__
#define __ARCOS_SERIAL_H__

#include <arcOS/types.h>

struct earlycon_device {
	void *membase;
	resource_size_t mapbase;
	unsigned int baud;
	unsigned int bits;
	unsigned int parity;
	unsigned int flow;
	void (*putchar)(struct earlycon_device *dev, const unsigned char c);
};

struct earlycon_id {
	const char *name;
	int (*setup)(struct earlycon_device *dev, const char *options);
} __aligned(32);

/* From linker script */
extern const struct earlycon_id __earlycon_table[];
extern const struct earlycon_id __earlycon_table_end[];

#define EARLYCON_DECLARE(n, f) \
	static const struct earlycon_id __earlycon_##n \
		__used __section(__earlycon_table) = { \
			.name = #n, \
			.setup = f, \
		}

void write_earlycon(const unsigned char c);

#endif

