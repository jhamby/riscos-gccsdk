# Second-level Makefile for UnixLib for RISC OS GCC
# Written by Nick Burrett <nick@dsvr.net>

export TRG CFL1

all:	unixlib

.PHONY: unixlib

upath=$(objdir)/$@/$(TRG)

unixlib:
	mkdir -p $(upath)/o $(upath)/complex/o $(upath)/math/o $(upath)/math-asm/o
	mkdir -p $(upath)/common/o $(upath)/gcccompat/o $(upath)/locale/o
	mkdir -p $(upath)/netlib/o $(upath)/pwd/o $(upath)/resource/o $(upath)/signal/o
	mkdir -p $(upath)/stdio/o $(upath)/scl/o $(upath)/sys/o $(upath)/time/o
	mkdir -p $(upath)/termios/o $(upath)/unix/o $(upath)/wchar/o
	mkdir -p $(TBIN)/$(TRG)
	$(MAKE) -C source

