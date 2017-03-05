--- arcOS
--- Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>


1. Description

arcOS is a heavly Linux based operating system that was designed to run in
Synopsys ARC processors. Although the system is designed to ARC the destination
architecture can easly be changed, just like Linux.

The system borrowed Linux way of using a general .config file to enable/disable
system features, unfortunatelly this file needs to be edited manually (i.e.
there is no graphical interface).

The OS was validated using Synopsys NSIM simulator. The intentation is to run
it in the future in an ARC SDP platform.


2. .config File

A simple .config file for usage with Synopsys NSIM simulator could be:
=========================
CONFIG_CROSS_COMPILE="/BUILDROOT/output/host/usr/bin/arc-buildroot-linux-uclibc-"
CONFIG_ARCOS_LINK_BASE=0x80000000
CONFIG_ARCH="arc"
CONFIG_BOOT_COMMAND_LINE="machine=snps,nsim earlycon=arc_uart,0xc0fc1000,115200n8"
CONFIG_MACHINE_NSIM=y
CONFIG_NRCPUS=1
CONFIG_ISA_ARCOMPACT=y
CONFIG_MEM_SIZE=0x10000000
CONFIG_FRAME_POINTER=y
=========================

Some notes about this .config file:
	1. CONFIG_CROSS_COMPILE should point to the cross compile toolset for
	ARC processors. Currently I am using the toolchain available with
	Buildroot.
	2. If you want to have stacktrace's with stack unwinding then
	CONFIG_FRAME_POINTER should be enabled BUT you will need to patch GCC
	in order for this to work correctly [1].
	3. Remaining configuration should not be changed for NSIM simulator
	(which is the only platform currently supported).


3. Compilation

Type 'make' in main folder. This will compile tools, parse .config file and
generate a binary image (called 'arcOS') which can be used with NSIM simulator.


4. Usage

You need to have Synopsys NSIM simulator installed. Currently I'm also using it
with Metaware so that I can easly see disassembly code and registers values. The
command line to use it with Metaware can for example be:

mdb -a7 -nsim -Xlib -prop=nsim_mmu=3 -Xtimer0 -Xtimer1 -icache=16384,64,2,o \
-dcache=32768,64,4,o -prop=cpunum=0 -prop=nsim_mem-dev=uart0 \
-prop=nsim_sc_mem_range_end=0xc0fbffff -prop=nsim_isa_atomic_option=1 \
-prop=nsim_isa_num_actionpoints=8 -prop=nsim_isa_aps_feature=1 -OK \
-toggle=deadbeef=1 -toggle=program_zeros_bss=1 arcOS


5. Appendix

[1] GCC patch in order to use stack unwinding:
=========================
--- a/gcc/config/arc/arc.c	2017-03-05 16:03:17.099432115 +0000
+++ b/gcc/config/arc/arc.c	2017-03-02 11:31:48.839881675 +0000
@@ -2345,7 +2345,7 @@ arc_compute_frame_size (void)
 	<return addr reg size> (if required) + <fp size> (if required).  */
   frame_info->save_return_addr
     = (!crtl->is_leaf || df_regs_ever_live_p (RETURN_ADDR_REGNUM)
-       || crtl->calls_eh_return);
+       || crtl->calls_eh_return || !flag_omit_frame_pointer);
   /* Saving blink reg in case of leaf function for millicode thunk calls.  */
   if (optimize_size
       && !TARGET_NO_MILLICODE_THUNK_SET
=========================

This patch was applied in Buildroot package 'host-gcc-final-arc-2016.09-eng010',
you may need to adjust it to the GCC version you are using.


