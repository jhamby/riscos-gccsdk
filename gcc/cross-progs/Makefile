# Second-level Makefile for supporting binaries for a
# RISC OS GCC cross-compiler.
# Written by Nick Burrett <nick@dsvr.net>

export TLIB

bin_dir = $(ux_gccpkg)/$(gn_standard_exec_prefix)

export bin_dir

# ARM Simulator doesn't build in Cygwin
ifneq (${hostsystem},HOST_I386_CYGWIN)
CROSS_TARGETS=arm-simulator
endif

all:	$(CROSS_TARGETS)

.PHONY: arm-simulator

arm-simulator:
	mkdir -p $(objdir)/riscos-progs/$@
	$(MAKE) -C $@
