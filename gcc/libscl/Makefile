# Makefile for the SharedCLibrary stubs
# Copyright (c) 2000, 2001 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the RISC OS GCCSDK
#

CROSS_CFLAGS += -mlibscl
INCLUDES = -idirafter .

$(objdir)/libscl/$(TRG)/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

$(objdir)/libscl/$(TRG)/%.o: %.s
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(CFL)

LIBC = $(objdir)/libscl/$(TRG)/unistd.o \
	$(objdir)/libscl/$(TRG)/stubs.o \
	$(objdir)/libscl/$(TRG)/uname.o

all:	mkdirs \
	$(ux_gcc_bin_dir)/$(TRG)/libscl.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libscl \
	$(ro_libsclpkg)/o/stubs

.PHONY: mkdirs

mkdirs:
	mkdir -p $(objdir)/libscl/$(TRG)

install_headers:
	$(insthdr) . "*.h" libscl $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)
	$(insthdr) sys "*.h" libscl/sys $(ux_gccpkg)/$(gn_cross_include_dir) $(ro_gccpkg)/$(gn_cross_include_dir)

	$(instsrc) . "*.h" . $(ro_libsclpkg)
	$(instsrc) sys "*.h" sys $(ro_libsclpkg)

$(ux_gcc_bin_dir)/$(TRG)/libscl.o $(ro_gcc_bin_dir)/$(TRG)/o/libscl $(ro_libsclpkg)/o/stubs: $(LIBC)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBC)

$(objdir)/libscl/$(TRG)/unistd.o: unistd.c unistd.h
$(objdir)/libscl/$(TRG)/fcntl.o: fcntl.c fcntl.h
$(objdir)/libscl/$(TRG)/stubs.o: stubs.s
$(objdir)/libscl/$(TRG)/uname.o: uname.c

