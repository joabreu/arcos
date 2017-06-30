/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/ctype.h>
#include <arcOS/params.h>
#include <arcOS/string.h>
#include <arcOS/types.h>

static char dash2underscore(char c)
{
	if (c == '-')
		return '_';
	return c;
}

bool parameqn(const char *a, const char *b, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		if (dash2underscore(a[i]) != dash2underscore(b[i]))
			return false;
	}

	return true;
}

bool parameq(const char *a, const char *b)
{
	return parameqn(a, b, strlen(a) + 1);
}

char *next_arg(char *args, char **param, char **val)
{
	int i, equal_pos = 0;
	char *next;

	for (i = 0; args[i]; i++) {
		if (isspace(args[i]))
			break;
		if (!equal_pos) {
			if (args[i] == '=')
				equal_pos = i;
		}
	}

	*param = args;
	if (!equal_pos) {
		*val = NULL;
	} else {
		args[equal_pos] = '\0';
		*val = args + equal_pos + 1;
	}

	if (args[i]) {
		args[i] = '\0';
		next = args + i + 1;
	} else {
		next = args + i;
	}

	return skip_spaces(next);
}

int parse_args(char *args, int (*fn) (char *param, char *val))
{
	int err = 0;

	args = skip_spaces(args);

	while (*args) {
		char *param, *val;

		args = next_arg(args, &param, &val);
		if (fn)
			err |= fn(param, val);
	}

	return err;
}
