# Second-level Makefile for supporting binaries for a
# RISC OS GCC cross-compiler.
# Written by Nick Burrett <nick@dsvr.net>

export TLIB

bin_dir = $(ux_gccpkg)/$(gn_standard_exec_prefix)

export bin_dir

all:	arm-simulator

.PHONY: arm-simulator

arm-simulator:
	mkdir -p $(objdir)/riscos-progs/$@
	$(MAKE) -C $@

