# Makefile for GNU Libio for the ARM/RISC OS GCC port
# Copyright (c) 1997, 1998, 1999, 2000, 2002 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
#

INCLUDES = -I include
ffiobjdir=$(objdir)/gcc-$(VERSION)/libffi/$(STAGE)/$(TRG)


FFI_OBJS = $(ffiobjdir)/debug.o \
	$(ffiobjdir)/ffitest.o \
	$(ffiobjdir)/java_raw_api.o \
	$(ffiobjdir)/prep_cif.o \
	$(ffiobjdir)/raw_api.o \
	$(ffiobjdir)/types.o \
	$(ffiobjdir)/ffi.o \
	$(ffiobjdir)/sysv.o

all:	$(ux_gcc_bin_dir)/$(TRG)/libffi.o \
	$(ro_gcc_bin_dir)/$(TRG)/o/libffi

$(ux_gcc_bin_dir)/$(TRG)/libffi.o: $(FFI_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(FFI_OBJS)

$(ro_gcc_bin_dir)/$(TRG)/o/libffi: $(FFI_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(FFI_OBJS)

$(ffiobjdir)/debug.o: src/debug.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)
$(ffiobjdir)/ffitest.o: src/ffitest.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)
$(ffiobjdir)/java_raw_api.o: src/java_raw_api.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)
$(ffiobjdir)/prep_cif.o: src/prep_cif.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)
$(ffiobjdir)/raw_api.o: src/raw_api.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)
$(ffiobjdir)/types.o: src/types.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)
$(ffiobjdir)/ffi.o: src/arm/ffi.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)
$(ffiobjdir)/sysv.o: src/arm/sysv.s
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES) $(CFL)

