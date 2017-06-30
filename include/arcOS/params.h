/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#ifndef __ARCOS_PARAMS_H__
#define __ARCOS_PARAMS_H__

#include <arcOS/types.h>

bool parameqn(const char *a, const char *b, size_t n);
bool parameq(const char *a, const char *b);
char *next_arg(char *args, char **param, char **val);
int parse_args(char *args, int (*fn) (char *param, char *val));

#endif /* __ARCOS_PARAMS_H__ */
