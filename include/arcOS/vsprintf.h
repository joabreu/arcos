/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_VSPRINTF_H__
#define __ARCOS_VSPRINTF_H__

#include <stdarg.h>
#include <arcOS/types.h>

unsigned long long simple_strtoull(const char *str, char **end, unsigned int b);
unsigned long simple_strtoul(const char *str, char **end, unsigned int b);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int vscnprintf(char *buf, size_t size, const char *fmt, va_list args);
int scnprintf(char *buf, size_t size, const char *fmt, ...);

#endif /* __ARCOS_VSPRINTF_H__ */
