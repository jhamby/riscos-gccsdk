# Second-level Makefile for supporting binaries for RISC OS GCC
# Written by Nick Burrett <nick@dsvr.net>

export TLIB

host_bin_dir = $(ux_gccpkg)/$(gn_standard_exec_prefix)
bin_dir = $(ro_gccpkg)/$(gn_standard_exec_prefix)
gcc_bin_dir = $(ro_gccpkg)/$(gn_gcc_bin_dir)
LIBIBERTY = $(gcc_bin_dir)/$(TRG)/libiberty.o

export host_bin_dir bin_dir gcc_bin_dir LIBIBERTY

all:	make bison fileutils flex gperf grep gzip patch \
	sed

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

bison diff fileutils flex gperf grep gzip make patch sed:
	mkdir -p $(objdir)/riscos-progs/$@
	$(MAKE) -C $@

