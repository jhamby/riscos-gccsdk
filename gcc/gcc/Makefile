# Second-level Makefile for RISC OS GCC v2.95.2
# Written by Nick Burrett <nick@dsvr.net>

# Passed in by top-level Makefile.  TRG is a path name for the 
export TRG

all:	gcc

.PHONY: gcc
.PHONY: libgcc
.PHONY: libio
.PHONY: libf2c
.PHONY: libstdc
.PHONY: libgpc

gccpath=$(objdir)/gcc-$(VERSION)/$@

gcc:
	mkdir -p $(gccpath) $(gccpath)/cp $(gccpath)/cpp $(gccpath)/f77 $(gccpath)/gpc
	mkdir -p $(gccpath)/config/arm $(gccpath)/gen $(gccpath)/insn
	$(MAKE) -C gcc

libgcc:
	mkdir -p $(gccpath)/$(TRG) $(TBIN)/$(TRG)
	$(MAKE) -C gcc -f Makelibs

libgpc libio libf2c libstdc:
	mkdir -p $(gccpath)/$(TRG)
	$(MAKE) -C $@

