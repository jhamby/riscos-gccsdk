# Second-level Makefile for RISC OS GCC v3.3
# Written by Nick Burrett <nick@dsvr.net>

# Passed in by top-level Makefile.  TRG is a path name for the 
export TRG

libpath=$(objdir)/gcc-$(VERSION)/$@
gccpath=$(objdir)/gcc-$(VERSION)/$@/$(STAGE)

all:	gcc

.PHONY: zlib
.PHONY: fastjar
.PHONY: gcc
.PHONY: libgcc
.PHONY: libf2c
.PHONY: libstdc
.PHONY: libio
.PHONY: libsupc++

gcc:
	mkdir -p $(gccpath) $(gccpath)/ada $(gccpath)/cp $(gccpath)/f77 \
	$(gccpath)/gpc $(gccpath)/objc $(gccpath)/java $(gccpath)/config/arm
	$(MAKE) -C gcc "STAGE=$(STAGE)"

fastjar libgcc libgpc libf2c libstdc libio libsupc++ zlib:
	mkdir -p $(libpath)/$(STAGE)/$(TRG)
	$(MAKE) -C $@ "STAGE=$(STAGE)"

libjava/.rules_class: libjava/Makefile
	$(MAKE) -C libjava class_deps GEN="gen"

libjava/.rules_headers: libjava/Makefile
	$(MAKE) -C libjava header_deps GEN="gen"


libjava: libjava/.rules_class libjava/.rules_headers
	mkdir -p $(libpath)/$(STAGE)/$(TRG)
	$(MAKE) -C $@ "STAGE=$(STAGE)"
