/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/compiler-gcc.h>
#include <arcOS/reboot.h>

void machine_halt(void)
{
	ASM("flag 1\n");
}
