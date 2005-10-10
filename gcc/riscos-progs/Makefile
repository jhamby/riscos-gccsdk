# Second-level Makefile for supporting binaries for RISC OS GCC
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005 GCCSDK Developers

host_bin_dir = $(ux_gccpkg)/$(gn_standard_exec_prefix)
bin_dir = $(ro_gccpkg)/$(gn_standard_exec_prefix)
gcc_bin_dir = $(ro_gccpkg)/$(gn_gcc_bin_dir)
LIBIBERTY = $(gcc_bin_dir)/$(TRG)/o/libiberty

export host_bin_dir bin_dir gcc_bin_dir LIBIBERTY

all:	flex make diff bison fileutils gperf grep gzip patch \
	sed fortify

riscos-dist:
	$(MAKE) -C bison riscos-dist
	$(MAKE) -C diff riscos-dist
	$(MAKE) -C flex riscos-dist
	$(MAKE) -C gperf riscos-dist
	$(MAKE) -C grep riscos-dist
	$(MAKE) -C make riscos-dist
	$(MAKE) -C sed riscos-dist
	
.PHONY: riscos-dist
.PHONY: bison
.PHONY: diff
.PHONY: fileutils
.PHONY: flex
.PHONY: gperf
.PHONY: grep
.PHONY: gzip
.PHONY: make
.PHONY: patch
.PHONY: sed
.PHONY: fortify

flex fortify gperf:
	mkdir -p $(objdir)/riscos-progs/$@
	$(MAKE) -C $@

bison diff fileutils grep gzip make patch sed:
	$(MAKE) -C $@

