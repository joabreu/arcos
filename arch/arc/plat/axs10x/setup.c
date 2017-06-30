/*
 * arcOS
 * Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>
 */

#include <arcOS/bitops.h>
#include <arcOS/io.h>
#include <arcOS/kernel.h>
#include <arcOS/mach-desc.h>
#include <arcOS/printk.h>
#include <arcOS/vsprintf.h>

#define AXS_MB_CGU		0xE0010000
#define AXS_MB_CREG		0xE0011000

#define AXS_CREG_IRQ_MUX	(AXS_MB_CREG + 0x214)
#define AXS_CREG_SW_RESET	(AXS_MB_CREG + 0x220)
#define AXS_CREG_VER		(AXS_MB_CREG + 0x230)
#define AXS_CREG_CONFIG		(AXS_MB_CREG + 0x234)

static void axs10x_enable_gpio_intc(void)
{
	/* */
}

static void axs10x_print_board_info(u32 ver_reg, const char *str)
{
	union ver {
		struct {
#if IS_ENABLED(CONFIG_CPU_BIG_ENDIAN)
			u32 pad:11, y:12, m:4, d:5;
#else
			u32 d:5, m:4, y:12, pad:11;
#endif
		};
		u32 val;
	} board;

	board.val = readl((void *)ver_reg);
	printk("AXS: %s FPGA Date: %u-%u-%u\n", str, board.d, board.m, board.y);
}

static void axs10x_setup_early(void)
{
	int mb_rev;
	char buf[32];

	if (readl((void *)AXS_CREG_CONFIG) & BIT(28))
		mb_rev = 3;
	else
		mb_rev = 2;

	axs10x_enable_gpio_intc();

	scnprintf(buf, 32, "MainBoard v%d", mb_rev);
	axs10x_print_board_info(AXS_CREG_VER, buf);
}

#if IS_ENABLED(CONFIG_AXS101)
static void setup_early(const struct machine_desc *mdesc)
{
	printk("%s: %s\n", mdesc->name, __func__);
	axs10x_setup_early();
}
MACHDESC_DECLARE(AXS10X, "snps,axs101", setup_early);
#endif /* IS_ENABLED(CONFIG_AXS101) */

#if IS_ENABLED(CONFIG_AXS103)
static void setup_early(const struct machine_desc *mdesc)
{
	printk("%s: %s\n", mdesc->name, __func__);
}
MACHDESC_DECLARE(AXS10X, "snps,axs103", setup_early);
#endif /* IS_ENABLED(CONFIG_AXS103) */
