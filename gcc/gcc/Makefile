# Second-level Makefile for RISC OS GCC v3.4
# Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005 Nick Burrett
# Copyright (c) 2005 GCCSDK Developers
# Written by Nick Burrett <nick@dsvr.net>

# Passed in by top-level Makefile.  TRG is a path name for the 
export TRG

libpath=$(objdir)/gcc-$(VERSION)/$@
gccpath=$(objdir)/gcc-$(VERSION)/$@/$(STAGE)

all:	gcc

.PHONY: gcc
.PHONY: libgcc
.PHONY: libf2c
.PHONY: libobjc
.PHONY: libstdc
.PHONY: libgcov

gcc:
	mkdir -p $(gccpath) $(gccpath)/cp $(gccpath)/f77 \
	$(gccpath)/objc $(gccpath)/config/arm
	$(MAKE) -C gcc "STAGE=$(STAGE)"

libgcc libf2c libobjc libstdc libgcov:
	mkdir -p $(libpath)/$(STAGE)/$(TRG)
	$(MAKE) -C $@ "STAGE=$(STAGE)"
