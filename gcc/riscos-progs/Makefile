# Second-level Makefile for supporting binaries for RISC OS GCC
# Written by Nick Burrett <nick@dsvr.net>

export TLIB

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

