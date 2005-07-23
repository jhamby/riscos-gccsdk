# Second-level Makefile for supporting binaries for RISC OS GCC
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005 GCCSDK Developers

export TLIB

ifeq (${STAGE},stage1)
bin_dir=$(ux_gccpkg)/$(gn_standard_exec_prefix)
gcc_bin_dir=$(ux_gccpkg)/$(gn_gcc_bin_dir)
else
bin_dir=$(ro_gccpkg)/$(gn_standard_exec_prefix)
gcc_bin_dir=$(ro_gccpkg)/$(gn_gcc_bin_dir)
endif

export bin_dir gcc_bin_dir

all:	as drlink infozip ld nm rink libfile decaof cmunge

.PHONY: as
.PHONY: drlink
.PHONY: infozip
.PHONY: ld
.PHONY: libfile
.PHONY: nm
.PHONY: rink
.PHONY: decaof
.PHONY: cmunge

as drlink infozip ld libfile nm rink decaof cmunge:
	mkdir -p $(objdir)/riscos-aof/$@/$(STAGE)
	$(MAKE) -C $@ "STAGE=$(STAGE)"
