# Makefile for the SharedCLibrary stubs
# Copyright (c) 2000, 2001, 2003 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the RISC OS GCCSDK
#

CROSS_CFLAGS += -mlibscl
INCLUDES = -isystem .

libsclobj=$(objdir)/libscl/$(TRG)

$(libsclobj)/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

$(libsclobj)/%.o: %.s
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(CFL)

STDLIB = $(libsclobj)/stdlib/div.o \
	$(libsclobj)/stdlib/strtof.o \
	$(libsclobj)/stdlib/strtold.o \
	$(libsclobj)/stdlib/strtoll.o \


LIBC = $(libsclobj)/unistd.o \
	$(libsclobj)/stubs.o \
	$(libsclobj)/uname.o

all:	mkdirs \
	$(ux_gcc_bin_dir)/$(TRG)/libscl.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libscl \
	$(ro_libsclpkg)/o/stubs

.PHONY: mkdirs

mkdirs:
	mkdir -p $(libsclobj)
	mkdir -p $(libsclobj)/stdlib

install_headers:
	$(insthdr) . "*.h" libscl $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) sys "*.h" libscl/sys $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)

	$(instsrc) . "*.h" . $(ro_libsclpkg)
	$(instsrc) sys "*.h" sys $(ro_libsclpkg)

$(ux_gcc_bin_dir)/$(TRG)/libscl.o $(ro_gcc_bin_dir)/$(TRG)/o/libscl $(ro_libsclpkg)/o/stubs: $(LIBC) $(STDLIB)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBC) $(STDLIB)

$(libsclobj)/unistd.o: unistd.c unistd.h
$(libsclobj)/fcntl.o: fcntl.c fcntl.h
$(libsclobj)/stubs.o: stubs.s
$(libsclobj)/uname.o: uname.c

$(libsclobj)/stdlib/div.o: stdlib/div.c
$(libsclobj)/stdlib/strtof.o: stdlib/strtof.c
$(libsclobj)/stdlib/strtold.o: stdlib/strtold.c
$(libsclobj)/stdlib/strtoll.o: stdlib/strtoll.c
