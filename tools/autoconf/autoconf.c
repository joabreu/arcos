/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "ini.h"

struct autoconf {
	FILE *f;
};

static int autoconf_handler(void *user, const char *section, const char *name,
		const char *value)
{
	struct autoconf *ac = user;
	int ivalue = 0;

	if (!strcmp(value, "y")) {
		/* Y/N value */
		ivalue = 1;
		fprintf(ac->f, "#define %s	%d\n", name, ivalue);
	} else {
		/* Other values: print as it is */
		fprintf(ac->f, "#define %s	%s\n", name, value);
	}

	return 1;
}

int main(int argc, char **argv)
{
	struct autoconf ac;

	memset(&ac, 0, sizeof(ac));

	if (argc < 3) {
		printf("usage: %s <.config> <out.h>\n", argv[0]);
		exit(1);
	}

	ac.f = fopen(argv[2], "w");
	if (!ac.f) {
		printf("failed to open '%s'\n", argv[2]);
		exit(1);
	}

	if (ini_parse(argv[1], autoconf_handler, &ac) < 0) {
		printf("failed to parse '%s'\n", argv[1]);
		exit(1);
	}

	fclose(ac.f);
	return 0;
}
