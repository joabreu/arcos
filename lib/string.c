/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 * Copyright (C) 1991, 1992 Linus Torvalds
 */

#include <arcOS/ctype.h>
#include <arcOS/string.h>
#include <arcOS/types.h>

void *memset(void *s, int c, size_t count)
{
	volatile char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}

void *memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = dest;
	const char *s = src;

	while (count--)
		*tmp++ = *s++;
	return dest;
}

char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		;
	return tmp;
}

char *strncpy(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	while (count) {
		if ((*tmp = *src) != 0)
			src++;
		tmp++;
		count--;
	}

	return dest;
}

size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t ret = strlen(src);

	if (size) {
		size_t len = (ret >= size) ? size - 1 : ret;
		memcpy(dest, src, len);
		dest[len] = '\0';
	}

	return ret;
}

char *strcat(char *dest, const char *src)
{
	char *tmp = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++) != '\0')
		;
	return tmp;
}

char *strncat(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++) != 0) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}

	return tmp;
}

int strcmp(const char *cs, const char *ct)
{
	unsigned char c1, c2;

	while (1) {
		c1 = *cs++;
		c2 = *ct++;

		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}

	return 0;
}

int strncmp(const char *cs, const char *ct, size_t count)
{
	unsigned char c1, c2;

	while (count--) {
		c1 = *cs++;
		c2 = *ct++;

		if (c1 != c2)
			return c1 < c2 ? -1: 1;
		if (!c1)
			break;
	}

	return 0;
}

char *strchr(const char *s, int c)
{
	for (; *s != (char)c; s++)
		if (*s == '\0')
			return NULL;
	return (char *)s;
}

char *skip_spaces(const char *str)
{
	while (isspace(*str))
		++str;
	return (char *)str;
}

size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; sc++)
		;
	return sc -s;
}
