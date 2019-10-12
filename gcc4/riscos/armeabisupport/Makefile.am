# Copyright (c) 2019 GCCSDK developers

bin_PROGRAMS=ARMEABISupport
ARMEABISupport_SOURCES=link_list.c memory.c swihandler.c init-fini.c \
			main.c swi.c swi-asm.s command.c abort.c abort-asm.s \
			stack.c stack-asm.s armeabisupport.cmhg
include_HEADERS=armeabi-support.h
BUILT_SOURCES=armeabisupport.h
ARMEABISupport_CFLAGS=-O3 -mmodule -Wall -std=gnu99
ARMEABISupport_CCASFLAGS=-O3 -mmodule -Wall -std=gnu99 -xassembler-with-cpp
ARMEABISupport_LDFLAGS=-mmodule

# CMunge invocation:
SUFFIXES=.cmhg
CMUNGEFLAGS = -tgcc -32bit -p -DPACKAGE_VERSION=$(PACKAGE_VERSION)
.cmhg.h:
	cmunge $(CMUNGEFLAGS) -d $@ $<
.cmhg.o:
	cmunge $(CMUNGEFLAGS) -o $@ $<

# Add ,ffa suffix to RISC OS Module filename during installation:
install-exec-hook:
	test -f "$(DESTDIR)$(bindir)/ARMEABISupport$(EXEEXT)" && mv $(DESTDIR)$(bindir)/ARMEABISupport$(EXEEXT) $(DESTDIR)$(bindir)/ARMEABISupport$(EXEEXT)\,ffa
