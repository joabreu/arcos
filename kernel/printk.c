/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <stdarg.h>
#include <arcOS/printk.h>
#include <arcOS/serial.h>
#include <arcOS/vsprintf.h>

int printk(const char *fmt, ...)
{
	char buff[512];
	va_list args;
	int i, ret;

	va_start(args, fmt);
	ret = vscnprintf(buff, sizeof(buff), fmt, args);
	va_end(args);

	for (i = 0; i < ret; i++) {
		write_earlycon(buff[i]);
		if (buff[i] == '\n')
			write_earlycon('\r');
	}

	return ret;
}
