/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_STRING_H__
#define __ARCOS_STRING_H__

#include <arcOS/types.h>

void *memset(void *s, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
size_t strlcpy(char *dest, const char *src, size_t size);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t count);
int strcmp(const char *cs, const char *ct);
int strncmp(const char *cs, const char *ct, size_t count);
char *strchr(const char *s, int c);
char *skip_spaces(const char *str);
size_t strlen(const char *s);

#endif

