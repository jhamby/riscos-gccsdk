# Second-level Makefile for supporting binaries for RISC OS GCC
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005, 2006 GCCSDK Developers

ifeq (${STAGE},stage1)
bin_dir=$(ux_gccpkg)/$(gn_standard_exec_prefix)
gcc_bin_dir=$(ux_gccpkg)/$(gn_gcc_bin_dir)
else
bin_dir=$(ro_gccpkg)/$(gn_standard_exec_prefix)
gcc_bin_dir=$(ro_gccpkg)/$(gn_gcc_bin_dir)
endif

export bin_dir gcc_bin_dir

all:	as drlink infozip ld nm libfile decaof cmunge resgen

riscos-dist:
	$(MAKE) -C as riscos-dist
	$(MAKE) -C cmunge riscos-dist
	$(MAKE) -C decaof riscos-dist
	$(MAKE) -C drlink riscos-dist
	$(MAKE) -C infozip riscos-dist
	$(MAKE) -C ld riscos-dist
	$(MAKE) -C libfile riscos-dist
	#$(MAKE) -C nm riscos-dist
	$(MAKE) -C resgen riscos-dist

.PHONY: riscos-dist
.PHONY: as
.PHONY: drlink
.PHONY: infozip
.PHONY: ld
.PHONY: libfile
.PHONY: nm
.PHONY: decaof
.PHONY: cmunge
.PHONY: resgen

as drlink infozip ld libfile nm decaof cmunge resgen:
	mkdir -p $(objdir)/riscos-aof/$@/$(STAGE)
	$(MAKE) -C $@ "STAGE=$(STAGE)"
