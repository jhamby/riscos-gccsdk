# Second-level Makefile for RISC OS GCC v3.3
# Written by Nick Burrett <nick@dsvr.net>

# Passed in by top-level Makefile.  TRG is a path name for the 
export TRG

libpath=$(objdir)/gcc-$(VERSION)/$@
gccpath=$(objdir)/gcc-$(VERSION)/$@/$(STAGE)

all:	zlib gcc
# libstdc++-v3

.PHONY: zlib
.PHONY: gcc
.PHONY: libgcc
.PHONY: libf2c
.PHONY: libstdc++-v3

gcc:
	mkdir -p $(gccpath) $(gccpath)/ada $(gccpath)/cp $(gccpath)/f77 \
	$(gccpath)/gpc $(gccpath)/objc $(gccpath)/java $(gccpath)/config/arm
	$(MAKE) -C gcc "STAGE=$(STAGE)"

zlib:
	mkdir -p $(gccpath)/zlib
	$(MAKE) -C $@

libgcc:
	mkdir -p $(libpath)/$(TRG)
	$(MAKE) -C libgcc "STAGE=$(STAGE)"

libgpc libf2c libstdc++-v3:
	mkdir -p $(libpath)/$(TRG)
	$(MAKE) -C $@
