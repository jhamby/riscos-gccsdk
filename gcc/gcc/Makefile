# Second-level Makefile for RISC OS GCC v3.4
# Written by Nick Burrett <nick@dsvr.net>

# Passed in by top-level Makefile.  TRG is a path name for the 
export TRG

libpath=$(objdir)/gcc-$(VERSION)/$@
gccpath=$(objdir)/gcc-$(VERSION)/$@/$(STAGE)

all:	gcc

.PHONY: zlib
.PHONY: gcc
.PHONY: libgcc
.PHONY: libf2c
.PHONY: libobjc
.PHONY: libstdc

gcc:
	mkdir -p $(gccpath) $(gccpath)/cp $(gccpath)/f77 \
	$(gccpath)/objc $(gccpath)/config/arm
	$(MAKE) -C gcc "STAGE=$(STAGE)"

libgcc libgpc libf2c libobjc libstdc:
	mkdir -p $(libpath)/$(STAGE)/$(TRG)
	$(MAKE) -C $@ "STAGE=$(STAGE)"
