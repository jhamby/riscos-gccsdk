# Second-level Makefile for supporting binaries for RISC OS GCC
# Written by Nick Burrett <nick@dsvr.net>

export TLIB

all:	as drlink ld libfile

.PHONY: as
.PHONY: drlink
.PHONY: ld
.PHONY: libfile

as drlink ld libfile:
	mkdir -p $(objdir)/riscos-aof/$@
	$(MAKE) -C $@

