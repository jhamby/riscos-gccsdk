# Second-level Makefile for RISC OS GCC v2.95.2
# Written by Nick Burrett <nick@dsvr.net>

# Passed in by top-level Makefile.  TRG is a path name for the 
export TRG

libpath=$(objdir)/gcc-$(VERSION)/$@
gccpath=$(objdir)/gcc-$(VERSION)/$@/$(STAGE)

all:	gcc

.PHONY: gcc
.PHONY: libgcc
.PHONY: libio
.PHONY: libf2c
.PHONY: libstdc
.PHONY: libgpc

gcc:
	mkdir -p $(gccpath) $(gccpath)/cp $(gccpath)/cpp $(gccpath)/f77 \
	$(gccpath)/gpc $(gccpath)/objc $(gccpath)/java $(gccpath)/config/arm \
	$(gccpath)/gen $(gccpath)/insn
	$(MAKE) -C gcc "STAGE=$(STAGE)"

libgcc:
	mkdir -p $(libpath)/$(TRG)
	$(MAKE) -C gcc -f Makelibs "STAGE=$(STAGE)"

libgpc libio libf2c libstdc:
	mkdir -p $(libpath)/$(TRG)
	$(MAKE) -C $@
