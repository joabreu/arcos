/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bug.h>
#include <arcOS/cpu.h>
#include <arcOS/init.h>
#include <arcOS/mach-desc.h>
#include <arcOS/printk.h>
#include <arcOS/vsprintf.h>
#include <asm/arc-regs.h>
#include <asm/cache.h>
#include <asm/irq.h>
#include <asm/mmu.h>
#include <asm/setup.h>
#include <soc/arc/timers.h>

#define IS_AVAIL1(v, s)		((v) ? (s) : "")

const struct machine_desc *machine_desc;
struct arc_cpuinfo arc_cpuinfo[NR_CPUS];

struct arc_id_desc {
	int id;
	const char *desc;
};

static const struct arc_id_desc arc_cpu_release[] = {
	{0x31, "V1"},
	{0x32, "V2"},
	{0x33, "V3"},
	{0x34, "R4.10"},
	{0x35, "R4.11"},
	{0x51, "R2.0"},
	{0x52, "R2.1"},
	{0x53, "R3.0"},
	{0x00, "Unknown"},
};

static const struct arc_id_desc arc_cpu_name[] = {
	{0x20, "ARC 600"},
	{0x30, "ARC 700"},
	{0x40, "ARC EM"},
	{0x50, "ARC HS38"},
	{0x00, "Unknown"},
};

static char *arc_format_cpuinfo(int cpu_id, char *buf, int len)
{
	struct arc_cpuinfo *cpu = &arc_cpuinfo[cpu_id];
	struct arc_bcr_identity *core = &cpu->core;
	int n = 0;

#define BUFPR(fmt, arg...)	n += scnprintf(buf + n, len - n, fmt, ##arg)

	BUFPR("\nIDENTITY\t: ARCVER [0x%x] ARCNUM [0x%x] CHIPID [0x%x]\n",
			core->family, core->cpu_id, core->chip_id);
	BUFPR("processor [%d]\t: %s %s (%s ISA) %s\n", cpu_id,
			cpu->name, cpu->details,
			is_isa_arcompact() ? "ARCompact" : "ARCv2",
			IS_AVAIL1(cpu->isa.be, "[Big-Endian]"));
	return buf;
}

static void arc_read_cpuinfo(int cpu_id)
{
	struct arc_cpuinfo *cpu = &arc_cpuinfo[cpu_id];
	const struct arc_id_desc *desc;
	struct arc_bcr_timer timer;

	ARC_READ_BCR(ARC_IDENTITY, cpu->core);
	ARC_READ_BCR(ARC_ARC700_BUILD, cpu->isa);

	for (desc = &arc_cpu_release[0]; desc->id != 0x00; desc++) {
		if (cpu->core.family == desc->id)
			break;
	}
	cpu->details = desc->desc;

	for (desc = &arc_cpu_name[0]; desc->id != 0x00; desc++) {
		if ((cpu->core.family & 0xF0) == desc->id)
			break;
	}
	cpu->name = desc->desc;

	ARC_READ_BCR(ARC_TIMER_BUILD, timer);
	cpu->extn.timer0 = timer.t0;
	cpu->extn.timer1 = timer.t1;
	cpu->extn.rtc = timer.rtc;
}

static void arc_chk_core_config(int cpu_id)
{
	struct arc_cpuinfo *cpu = &arc_cpuinfo[cpu_id];

	PANIC_ON(!cpu->extn.timer0, "timer0 not present!\n");
	PANIC_ON(!cpu->extn.timer1, "timer1 not present!\n");
}

static void setup_processor(void)
{
	char str[512];

	arc_read_cpuinfo(0);
	arc_init_irq();

	printk(arc_format_cpuinfo(0, str, sizeof(str)));
	printk(arc_format_cacheinfo(0, str, sizeof(str)));
	printk("\n");

	arc_chk_core_config(0);
}

void setup_arch(char **cmdline)
{
	*cmdline = boot_command_line;

	parse_early_params();

	machine_desc = machdesc_match(*cmdline);
	if (!machine_desc) {
		PANIC("No machine description found!\n");
	} else if (machine_desc->setup_early) {
		machine_desc->setup_early(machine_desc);
	}

	setup_processor();
	setup_arch_memory();
	arc_mmu_init();
}
