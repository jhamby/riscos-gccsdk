# Second-level Makefile for UnixLib for RISC OS GCC
# Written by Nick Burrett <nick@dsvr.net>

export TRG CFL1

all:	unixlib

.PHONY: unixlib

upath=$(objdir)/$@/$(TRG)

install_headers:
	$(MAKE) -C source install_headers

install_source:
	$(MAKE) -C source install_source

unixlib:
	mkdir -p $(upath) $(upath)/complex $(upath)/crypt $(upath)/grp
	mkdir -p $(upath)/math $(upath)/math-asm $(upath)/signal
	mkdir -p $(upath)/common $(upath)/gcccompat $(upath)/locale
	mkdir -p $(upath)/netlib $(upath)/pwd $(upath)/resource
	mkdir -p $(upath)/stat $(upath)/stdio
	mkdir -p $(upath)/stdlib $(upath)/scl
	mkdir -p $(upath)/string $(upath)/sys $(upath)/time
	mkdir -p $(upath)/termios $(upath)/unistd $(upath)/unix
	mkdir -p $(upath)/wchar $(upath)/module
	$(MAKE) -C source

