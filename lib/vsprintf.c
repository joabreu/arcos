/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <stdarg.h>
#include <arcOS/bitops.h>
#include <arcOS/ctype.h>
#include <arcOS/hex.h>
#include <arcOS/page.h>
#include <arcOS/string.h>
#include <arcOS/types.h>
#include <arcOS/vsprintf.h>

static const char *parse_integer_fixup_radix(const char *str, unsigned int *b)
{
	if (*b == 0) {
		if (str[0] == '0') {
			if (tolower(str[1]) == 'x' && isxdigit(str[2]))
				*b = 16;
			else
				*b = 8;
		} else {
			*b = 10;
		}
	}

	if (*b == 16 && str[0] == '0' && tolower(str[1]) == 'x')
		str += 2;
	return str;
}

static unsigned int parse_integer(const char *str, unsigned int b, unsigned long long *result)
{
	unsigned long long tmp = 0;
	unsigned int digits;

	while (*str) {
		unsigned int val;

		if ('0' <= *str && *str <= '9')
			val = *str - '0';
		else if ('a' <= tolower(*str) && tolower(*str) <= 'f')
			val = tolower(*str) - 'a' + 10;
		else
			break;

		if (val >= b)
			break;
		if (tmp & (~0ULL << 60)) {
			*result = 0;
			return 0; /* Assume a overflow will happen */
		}

		tmp = tmp * b + val;
		str++;
		digits++;
	}

	*result = tmp;
	return digits;
}

unsigned long long simple_strtoull(const char *str, char **end, unsigned int b)
{
	unsigned long long result;
	unsigned int ret;

	str = parse_integer_fixup_radix(str, &b);
	ret = parse_integer(str, b, &result);
	str += ret;

	if (end)
		*end = (char *)str;
	return result;
}

unsigned long simple_strtoul(const char *str, char **end, unsigned int b)
{
	return simple_strtoull(str, end, b);
}

#define BUF_WRITE(__b,__e,__c)	\
{ \
	if ((__b) < (__e)) \
		*(__b) = (__c); \
	(__b)++; \
}

#define SIGN		BIT(0) /* Fixed */
#define LEFT		BIT(1)
#define PLUS		BIT(2)
#define SPACE		BIT(3)
#define ZEROPAD		BIT(4) /* Fixed */
#define SMALL		BIT(5) /* Fixed */
#define SPECIAL		BIT(6)

enum format_type {
	FORMAT_TYPE_NONE,
	FORMAT_TYPE_WIDTH,
	FORMAT_TYPE_PRECISION,
	FORMAT_TYPE_STRING,
	FORMAT_TYPE_PERCENT_CHAR,
	FORMAT_TYPE_CHAR,
	FORMAT_TYPE_UBYTE,
	FORMAT_TYPE_BYTE,
	FORMAT_TYPE_USHORT,
	FORMAT_TYPE_SHORT,
	FORMAT_TYPE_ULONG,
	FORMAT_TYPE_LONG,
	FORMAT_TYPE_UINT,
	FORMAT_TYPE_INT,
	FORMAT_TYPE_LONG_LONG,
	FORMAT_TYPE_SIZE_T,
	FORMAT_TYPE_PTRDIFF,
	FORMAT_TYPE_INVALID,
};

struct format_spec {
	u8 type;
	u8 flags;
	u8 base;
	s32 width;
	s16 precision;
};

#define WIDTH_MAX	((1 << 31) - 1)
#define PRECISION_MAX	((1 << 15) - 1)

static int skip_atoi(const char **s)
{
	int i = 0;

	do {
		i = i*10 + *((*s)++) - '0';
	} while (isdigit(**s));

	return i;
}

static int format_decode(const char *fmt, struct format_spec *spec)
{
	const char *start = fmt;
	char qualifier;

	if (spec->type == FORMAT_TYPE_WIDTH) {
		if (spec->width < 0) {
			spec->width = -spec->width;
			spec->flags |= LEFT;
		}
		spec->type = FORMAT_TYPE_NONE;
		goto precision;
	} else if (spec->type == FORMAT_TYPE_PRECISION) {
		if (spec->precision < 0)
			spec->precision = 0;
		spec->type = FORMAT_TYPE_NONE;
		goto qualifier;
	}

	spec->type = FORMAT_TYPE_NONE;

	for (; *fmt; fmt++) {
		if (*fmt == '%')
			break;
	}

	if (fmt != start || !*fmt)
		return fmt - start;

	/* --- Found modifier --- */

	/* Process flags */
	spec->flags = 0;
	while (1) {
		bool found = true;

		fmt++;

		switch (*fmt) {
		case '-': spec->flags |= LEFT;break;
		case '+': spec->flags |= PLUS;break;
		case ' ': spec->flags |= SPACE;break;
		case '#': spec->flags |= SPECIAL;break;
		case '0': spec->flags |= ZEROPAD;break;
		default: found = false;break;
		}

		if (!found)
			break;
	}

	/* Process width */
	spec->width = -1;
	if (isdigit(*fmt)) {
		spec->width = skip_atoi(&fmt);
	} else if (*fmt == '*') {
		/* it's the next argument */
		spec->type = FORMAT_TYPE_WIDTH;
		return ++fmt - start;
	}

precision:
	/* Process precision */
	spec->precision = -1;
	if (*fmt == '.') {
		++fmt;
		if (isdigit(*fmt)) {
			spec->precision = skip_atoi(&fmt);
			if (spec->precision < 0)
				spec->precision = 0;
		} else if (*fmt == '*') {
			/* it's the next argument */
			spec->type = FORMAT_TYPE_PRECISION;
			return ++fmt - start;
		}
	}

qualifier:
	/* Process qualifier */
	qualifier = 0;
	if (*fmt == 'h' || tolower(*fmt) == 'l' ||
	    tolower(*fmt) == 'z' || *fmt == 't') {
		qualifier = *fmt++;
		if (qualifier == *fmt) {
			if (qualifier == 'l') {
				qualifier = 'L';
				fmt++;
			} else if (qualifier == 'h') {
				qualifier = 'H';
				fmt++;
			}
		}
	}

	/* Process conversion chars */
	spec->base = 10;
	switch (*fmt) {
	case 's':
		spec->type = FORMAT_TYPE_STRING;
		return ++fmt - start;
	case '%':
		spec->type = FORMAT_TYPE_PERCENT_CHAR;
		return ++fmt - start;
	case 'c':
		spec->type = FORMAT_TYPE_CHAR;
		return ++fmt - start;
	case 'o':
		spec->base = 8;
		break;
	case 'x':
		spec->flags |= SMALL;
		/* Fall through */
	case 'X':
		spec->base = 16;
		break;
	case 'd':
	case 'i':
		spec->flags |= SIGN;
	case 'u':
		break;
	default:
		spec->type = FORMAT_TYPE_INVALID;
		return fmt - start;
	}

	if (qualifier == 'L')
		spec->type = FORMAT_TYPE_LONG_LONG;
	else if (qualifier == 'l')
		spec->type = FORMAT_TYPE_ULONG + (spec->flags & SIGN);
	else if (tolower(qualifier) == 'z')
		spec->type = FORMAT_TYPE_SIZE_T;
	else if (qualifier == 't')
		spec->type = FORMAT_TYPE_PTRDIFF;
	else if (qualifier == 'H')
		spec->type = FORMAT_TYPE_UBYTE + (spec->flags & SIGN);
	else if (qualifier == 'h')
		spec->type = FORMAT_TYPE_USHORT + (spec->flags & SIGN);
	else
		spec->type = FORMAT_TYPE_UINT + (spec->flags & SIGN);

	return ++fmt - start;
}

static unsigned char *string(unsigned char *buf, unsigned char *end,
		const char *str, struct format_spec spec)
{
	size_t prec = spec.precision;

	if ((unsigned long)str < PAGE_SIZE)
		str = "(null)";

	while (prec--) {
		char c = *str++;
		if (!c)
			break;
		BUF_WRITE(buf, end, c);
	}

	return buf;
}

static unsigned char *number(unsigned char *buf, unsigned char *end,
		unsigned long num, struct format_spec spec)
{
	char tmp[3 * sizeof(num)];
	char sign, locase;
	int width = spec.width;
	int precision = spec.precision;
	int need_pfx = ((spec.flags & SPECIAL) && spec.base != 10);
	bool is_zero = (num == 0LL);
	int i;

	locase = (spec.flags & SMALL);
	if (spec.flags & LEFT)
		spec.flags &= ~ZEROPAD;

	sign = 0;
	if (spec.flags & SIGN) {
		if ((signed long long)num < 0) {
			sign = '-';
			num = -(signed long long)num;
			width--;
		} else if (spec.flags & PLUS) {
			sign = '+';
			width--;
		} else if (spec.flags & SPACE) {
			sign = ' ';
			width--;
		}
	}

	if (need_pfx) {
		if (spec.base == 16)
			width -= 2;
		else if (!is_zero)
			width--;
	}

	i = 0;
	if (num < spec.base) {
		tmp[i++] = hex_chars_upper[num] | locase;
	} else if (spec.base != 10) {
		int mask = spec.base - 1;
		int shift = 3;

		if (spec.base == 16)
			shift = 4;
		do {
			tmp[i++] = (hex_chars_upper[((unsigned char)num) & mask] | locase);
			num >>= shift;
		} while (num);
	} else {
		do {
			tmp[i++] = '0' + (num % 10);
			num /= 10;
		} while (num);
	}

	if (i > precision)
		precision = i;
	width -= precision;

	if (!(spec.flags & (ZEROPAD | LEFT))) {
		while (--width >= 0)
			BUF_WRITE(buf, end, ' ');
	}

	if (sign)
		BUF_WRITE(buf, end, sign);

	if (need_pfx) {
		if (spec.base == 16 || !is_zero)
			BUF_WRITE(buf, end, '0');
		if (spec.base == 16)
			BUF_WRITE(buf, end, ('X' | locase));
	}

	if (!(spec.flags & LEFT)) {
		char c = ' ' + (spec.flags & ZEROPAD);
		while (--width >= 0)
			BUF_WRITE(buf, end, c);
	}

	while (i <= --precision)
		BUF_WRITE(buf, end, '0');
	while (--i >= 0)
		BUF_WRITE(buf, end, tmp[i]);
	while (--width >= 0)
		BUF_WRITE(buf, end, ' ');

	return buf;
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	struct format_spec spec = {0};
	unsigned char *pos, *end;
	unsigned long num;

	pos = (unsigned char *)buf;
	end = (unsigned char *)buf + size;

	if (!size)
		return 0;

	while (*fmt) {
		const char *old_fmt = fmt;
		int read = format_decode(fmt, &spec);

		fmt += read;

		switch (spec.type) {
		case FORMAT_TYPE_NONE: { /* Normal string */
			int copy = read;
			if (pos < end) {
				if (copy > end - pos)
					copy = end - pos;
				memcpy(pos, old_fmt, copy);
			}
			pos += read;
			break;
		}
		case FORMAT_TYPE_WIDTH: /* Not yet supported */
		case FORMAT_TYPE_PRECISION: /* Not yet supported */
			goto out;
		case FORMAT_TYPE_STRING: /* %s */
			pos = string(pos, end, va_arg(args, char *), spec);
			break;
		case FORMAT_TYPE_PERCENT_CHAR: /* %% */
			BUF_WRITE(pos, end, '%');
			break;
		case FORMAT_TYPE_CHAR: { /* %c */
			char c;

			if (!(spec.flags & LEFT)) {
				while (--spec.width > 0)
					BUF_WRITE(pos, end, ' ');
			}

			c = (unsigned char)va_arg(args, int);
			BUF_WRITE(pos, end, c);

			while (--spec.width > 0)
				BUF_WRITE(pos, end, ' ');
			break;
		}
		case FORMAT_TYPE_PTRDIFF: /* Not yet supported */
		case FORMAT_TYPE_LONG_LONG: /* Not yet supported */
		case FORMAT_TYPE_INVALID:
			goto out;
		default:
			switch (spec.type) {
			case FORMAT_TYPE_UBYTE:
				num = (unsigned char)va_arg(args, int);
				break;
			case FORMAT_TYPE_BYTE:
				num = (signed char)va_arg(args, int);
				break;
			case FORMAT_TYPE_USHORT:
				num = (unsigned short)va_arg(args, int);
				break;
			case FORMAT_TYPE_SHORT:
				num = (signed short)va_arg(args, int);
				break;
			case FORMAT_TYPE_ULONG:
				num = va_arg(args, unsigned long);
				break;
			case FORMAT_TYPE_LONG:
				num = va_arg(args, long);
				break;
			case FORMAT_TYPE_INT:
				num = (int)va_arg(args, int);
				break;
			case FORMAT_TYPE_SIZE_T:
				if (spec.flags & SIGN)
					num = va_arg(args, ssize_t);
				else
					num = va_arg(args, size_t);
				break;
			default:
				num = va_arg(args, unsigned int);
				break;
			}

			pos = number(pos, end, num, spec);
			break;
		}
	}

out:
	if (pos < end)
		*pos = '\0';
	else
		end[-1] = '\0';

	return pos - (unsigned char *)buf;
}

int vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	int i;

	i = vsnprintf(buf, size, fmt, args);

	if (i < size)
		return i;
	if (size != 0)
		return size - 1;
	return 0;
}

int scnprintf(char *buf, size_t size, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = vscnprintf(buf, size, fmt, args);
	va_end(args);

	return i;
}
