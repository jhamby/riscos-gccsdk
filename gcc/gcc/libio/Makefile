# Makefile for GNU Libio for the ARM/RISC OS GCC port
# Copyright (c) 1997, 1998, 1999, 2000, 2002 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
#

INCLUDES = -I. -I../libstdc/include -I../libstdc/include/arm-riscos-aof
ioobjdir=$(objdir)/gcc-$(VERSION)/libio/$(TRG)

CROSS_CFLAGS += -D_GNU_SOURCE
CROSS_CXXFLAGS += -D_GNU_SOURCE

$(ioobjdir)/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

$(ioobjdir)/%.o: %.cc
	$(CROSS_CXX) $(CROSS_CXXFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

IO_OBJECTS = $(ioobjdir)/filedoalloc.o \
	$(ioobjdir)/fileops.o \
	$(ioobjdir)/genops.o \
	$(ioobjdir)/iofclose.o \
	$(ioobjdir)/iofopen.o \
	$(ioobjdir)/iofwide.o \
	$(ioobjdir)/stdfiles.o \
	$(ioobjdir)/stdio.o \
	$(ioobjdir)/wfiledoalloc.o \
	$(ioobjdir)/wfileops.o \
	$(ioobjdir)/wgenops.o

all:	$(ux_gcc_bin_dir)/$(TRG)/libio.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libio

install_headers:
	$(insthdr) . "*.h" . $(ux_gccpkg)/$(gn_gplusplus_include_dir) $(ro_gccpkg)/$(gn_gplusplus_include_dir)

$(ux_gcc_bin_dir)/$(TRG)/libio.o: $(IO_OBJECTS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(IO_OBJECTS)

$(ro_gcc_bin_dir)/$(TRG)/o/libio: $(IO_OBJECTS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(IO_OBJECTS)

$(ioobjdir)/filedoalloc.o: filedoalloc.c
$(ioobjdir)/fileops.o: fileops.c
$(ioobjdir)/genops.o: genops.c
$(ioobjdir)/iofclose.o: iofclose.c
$(ioobjdir)/iofopen.o: iofopen.c
$(ioobjdir)/iofwide.o: iofwide.c
$(ioobjdir)/stdfiles.o: stdfiles.c
$(ioobjdir)/stdio.o: stdio.c
$(ioobjdir)/wfiledoalloc.o: wfiledoalloc.c
$(ioobjdir)/wfileops.o: wfileops.c
$(ioobjdir)/wgenops.o: wgenops.c
