/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <stdio.h>
#include <stdlib.h>

void write_header(FILE *fout)
{
	fprintf(fout, "#include <arcOS/ksymbol.h>\n");
	fprintf(fout, "\n");
	fprintf(fout, "KSYMBOL_TABLE() = {\n");
}

void write_footer(FILE *fout)
{
	fprintf(fout, "};\n");
	fprintf(fout, "\n");
}

void parse_map(FILE *fin, FILE *fout)
{
	unsigned long long addr;
	char type, str[500];
	int ret;

	while (!feof(fin)) {
		ret = fscanf(fin, "%llx %c %499s\n", &addr, &type, str);
		if (ret != 3) {
			printf("ERROR: invalid .map file\n");
			break;
		}

		fprintf(fout, "{ .addr = 0x%llx, .type = '%c', .name = \"%s\" },\n",
				addr, type, str);
	}
}

int main(int argc, char **argv)
{
	FILE *fin, *fout;

	if (argc < 3) {
		printf("usage: %s <in.map> <out.h>\n", argv[0]);
		exit(1);
	}

	fin = fopen(argv[1], "r");
	if (!fin) {
		printf("failed to open '%s'\n", argv[1]);
		exit(1);
	}

	fout = fopen(argv[2], "w");
	if (!fout) {
		printf("failed to open '%s'\n", argv[2]);
		exit(1);
	}

	write_header(fout);
	parse_map(fin, fout);
	write_footer(fout);
	return 0;
}
