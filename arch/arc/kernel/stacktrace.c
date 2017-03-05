/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/compiler-gcc.h>
#include <arcOS/irq.h>
#include <arcOS/ksymbol.h>
#include <arcOS/printk.h>
#include <arcOS/task.h>
#include <arcOS/types.h>
#include <asm/stacktrace.h>
#include <asm/unwind.h>

static __always_inline void arc_unwind_seed(struct task_struct *task,
		struct pt_regs *regs, struct unwind_info *uinfo)
{
	if (!regs) {
		u32 fp, sp, blink, ret;

		ASM(
			"mov %0, r27\n"
			"mov %1, r28\n"
			"mov %2, r31\n"
			"mov %3, r63\n"
			: "=r"(fp), "=r"(sp), "=r"(blink), "=r"(ret)
		);

		uinfo->regs.r27 = fp;
		uinfo->regs.r28 = sp;
		uinfo->regs.r31 = blink;
		uinfo->regs.r63 = ret;
	}

	uinfo->task = task;
}

static __noinline void arc_unwind_core(struct task_struct *task,
		void (*consumer_fn) (unsigned long address))
{
	struct unwind_info uinfo;
	u32 address;
	int ret;

	arc_unwind_seed(task, NULL, &uinfo);

	while (1) {
		address = STACK_PC(&uinfo);
		if (address)
			consumer_fn(address);

		ret = arc_unwind(&uinfo);
		if (ret)
			break;
	}
}

static void arc_print_symbol(unsigned long address)
{
	print_ksymbol("\t%s\n", address);
}

__noinline void show_stacktrace(struct task_struct *task)
{
	printk("\nStack Trace:\n");
	arc_unwind_core(task, arc_print_symbol);
}

