# Second-level Makefile for supporting binaries for RISC OS GCC
# Written by Nick Burrett <nick@dsvr.net>

export TLIB

CFLAGS += -D_GNU_SOURCE

ifeq (${STAGE},stage1)
bin_dir=$(ux_gccpkg)/$(gn_standard_exec_prefix)
gcc_bin_dir=$(ux_gccpkg)/$(gn_gcc_bin_dir)
else
bin_dir=$(ro_gccpkg)/$(gn_standard_exec_prefix)
gcc_bin_dir=$(ro_gccpkg)/$(gn_gcc_bin_dir)
endif

export bin_dir gcc_bin_dir

all:	as drlink infozip ld libfile nm rink

.PHONY: as
.PHONY: drlink
.PHONY: infozip
.PHONY: ld
.PHONY: libfile
.PHONY: nm
.PHONY: rink

as drlink infozip ld libfile nm rink:
	mkdir -p $(objdir)/riscos-aof/$@/$(STAGE)
	$(MAKE) -C $@ "STAGE=$(STAGE)"
