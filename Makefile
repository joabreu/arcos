#
# arcOS
# Copyright (C) 2017 Jose Abreu <joabreu@synopsys.com>

# Version information
ARCOS_VERSION = 0
ARCOS_SUBVERSION = 1

# Variables
srcdir := .
outfile := $(srcdir)/arcOS
outtmp := $(srcdir)/.arcOS.tmp
outtmp-with-syms := $(srcdir)/.arcOS.tmp.2
outmap := $(srcdir)/.arcOS.map.tmp
finalmap := $(srcdir)/arcOS.map
export srcdir

# Tools
autoconf-bin = $(srcdir)/tools/autoconf/autoconf
autoconf-in = $(srcdir)/.config
autoconf-out = $(srcdir)/include/generated/autoconf.h
ksyms-bin = $(srcdir)/tools/ksyms/ksyms
ksyms-in = $(outmap)
ksyms-out = $(srcdir)/.ksyms.c
ksyms-obj-out = $(srcdir)/.ksyms.o

# Outputed objects in tools folder (to clean)
tools-bins = $(autoconf-bin) $(ksyms-bin)
tools-bins := $(strip $(tools-bins))

# Outputed objects in root folder (to clean)
outfiles = $(outfile) $(outtmp) $(outtmp-with-syms) $(outmap) $(ksyms-out)
outfiles += $(ksyms-obj-out) $(finalmap)
outfiles := $(strip $(outfiles))

# Flags
MAKEFLAGS += -rR --include-dir=$(CURDIR) --no-print-directory --quiet
CFLAGS = -Wall -Wmissing-prototypes -Wstrict-prototypes  -Wno-unused-value \
	 -Wno-unused-parameter -Wno-missing-field-initializers -std=gnu89
CFLAGS += -DARCOS_VERSION=$(ARCOS_VERSION) \
	  -DARCOS_SUBVERSION=$(ARCOS_SUBVERSION)
CFLAGS += -I$(srcdir)/include -include $(autoconf-out)
LDFLAGS = -Map $(finalmap)
HOSTCFLAGS = -g -Wall

export MAKEFLAGS
export CFLAGS
export LDFLAGS
export HOSTCFLAGS

# Executables
LD = $(patsubst "%",%,$(CONFIG_CROSS_COMPILE))ld
CC = $(patsubst "%",%,$(CONFIG_CROSS_COMPILE))gcc
AR = $(patsubst "%",%,$(CONFIG_CROSS_COMPILE))ar
NM = $(patsubst "%",%,$(CONFIG_CROSS_COMPILE))nm
LIBGCC = $(shell $(CC) --print-libgcc-file-name)
HOSTLD = ld
HOSTCC = gcc
export LD
export CC
export AR
export HOSTLD
export HOSTCC

# Build options
include $(srcdir)/.config
LDFLAGS += -T arch/$(patsubst "%",%,$(CONFIG_ARCH))/kernel/arch-linker.lds
CFLAGS += -I$(srcdir)/arch/$(patsubst "%",%,$(CONFIG_ARCH))/include

# Arch flags
-include arch/$(patsubst "%",%,$(CONFIG_ARCH))/Makefile.flags
CFLAGS += $(cflags-y)

# Build dependent gcc flags
ifdef CONFIG_FRAME_POINTER
CFLAGS += -fno-omit-frame-pointer -fno-optimize-sibling-calls
else
CFLAGS += -fomit-frame-pointer
endif

export LDFLAGS
export CFLAGS

# Directories
arch-y := arch/
kernel-y := kernel/
mm-y := mm/
drivers-y := drivers/
apps-$(CONFIG_APPS) := apps/
lib-y := lib/
tools-y := tools/
build-dirs := $(patsubst %/, %, $(filter %/, $(arch-y) $(kernel-y) \
	$(mm-y) $(drivers-y) $(apps-y) $(lib-y)))
tools-dirs := $(patsubst %/, %, $(filter %/, $(tools-y)))
all-dirs := $(sort $(build-dirs) $(patsubst %/,%,$(filter %/, $(arch-) \
	$(kernel-) $(mm-) $(drivers-) $(apps-) $(lib-))))
clean-dirs := $(addprefix _clean_, $(all-dirs) $(tools-y))

# Files
arch-y := $(patsubst %/, %/built-in.o, $(arch-y))
drivers-y := $(patsubst %/, %/built-in.o, $(drivers-y))
kernel-y := $(patsubst %/, %/built-in.o, $(kernel-y))
mm-y := $(patsubst %/, %/built-in.o, $(mm-y))
apps-y := $(patsubst %/, %/built-in.o, $(apps-y))
lib-y := $(patsubst %/, %/lib.a, $(lib-y))
lib-y += $(LIBGCC)
all-y := $(arch-y) $(kernel-y) $(mm-y) $(drivers-y) $(apps-y) $(lib-y)

# Compilation rules
PHONY := all
all: info prepare build prelink dosyms link

PHONY += info
info:
	@echo 'Compiling using gcc: $(CC)'
	@echo 'Compiling using ld: $(LD)'
	@echo 'Using libgcc: $(LIBGCC)'

PHONY += prepare
prepare: $(tools-dirs)
	$(autoconf-bin) $(autoconf-in) $(autoconf-out)

PHONY += $(tools-dirs)
$(tools-dirs):
	@$(MAKE) -f $(srcdir)/build/Makefile.tools dir=$@

PHONY += build
build: $(build-dirs)

PHONY += $(build-dirs)
$(build-dirs):
	@$(MAKE) -f $(srcdir)/build/Makefile.build dir=$@

PHONY += prelink
prelink:
	@echo '  LD      $(outtmp)'
	@$(LD) $(LDFLAGS) -o $(outtmp) $(strip $(all-y))
	@echo '  MAP     $(outtmp)'
	@$(NM) -n $(outtmp) > $(outmap)

PHONY += dosyms
dosyms:
	@echo '  SYMS    $(outtmp)'
	$(ksyms-bin) $(ksyms-in) $(ksyms-out)
	@echo '  CC      $(ksyms-out)'
	@$(CC) $(CFLAGS) -c -o $(ksyms-obj-out) $(ksyms-out)
	@echo '  LD      $(outtmp-with-syms)'
	@$(LD) $(LDFLAGS) -o $(outtmp-with-syms) $(strip $(all-y)) $(ksyms-obj-out)
	@echo '  MAP     $(outtmp-with-syms)'
	@$(NM) -n $(outtmp-with-syms) > $(outmap)
	@echo '  SYMS    $(outtmp-with-syms)'
	@$(ksyms-bin) $(ksyms-in) $(ksyms-out)
	@echo '  CC      $(ksyms-out)'
	@$(CC) $(CFLAGS) -c -o $(ksyms-obj-out) $(ksyms-out)

PHONY += link
link:
	@echo '  LD      $(outfile)'
	@$(LD) $(LDFLAGS) -o $(outfile) $(strip $(all-y)) $(ksyms-obj-out)

PHONY += clean
clean: $(clean-dirs)
	$(if $(wildcard $(tools-bins)), \
		@rm $(tools-bins))
	$(if $(wildcard $(outfiles)), \
		@rm $(wildcard $(outfiles)))

PHONY += clean-dirs
$(clean-dirs):
	@$(MAKE) -f $(srcdir)/build/Makefile.clean dir=$(patsubst _clean_%,%,$@)

PHONY += help
help:
	@echo 'arcOS make targets'
	@echo '------------------'
	@echo '  all 	- Make kernel and drivers'
	@echo '  clean	- Remove generated files'
	@echo ''
	@echo 'For more information see README.txt'

.PHONY: $(PHONY)

