# Makefile for GNU libiberty for ARM/RISC OS
# Copyright (c) 1997-2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This Makefile gets run twice.  First libiberty must be built
# natively because the GCC cross-compiler uses routines from
# this library.
#
# The libiberty will be built natively for the RISC OS binaries
# that require routines from this library.
#

# Defs for GCC
CFLAGS += -DIN_GCC -DHAVE_CONFIG_H $(TARGET_DEFS)
INCLUDES = -I. -I../include

ifeq ($(TRG), native)

# Building libiberty for the host binaries
libname=libiberty
ifeq ($(crosscompile),yes)
libname=libiberty.a
endif
objtarget=$(objdir)/libiberty
else

# Building to target RISC OS
CC = $(standard_exec_prefix)/gcc $(CFLAGS)
objtarget=$(objdir)/libiberty/$(TRG)
endif


$(objtarget)/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)


LIBIBERTY = $(objtarget)/choose-temp.o $(objtarget)/cplus-dem.o \
	$(objtarget)/getopt.o $(objtarget)/getopt1.o \
	$(objtarget)/obstack.o $(objtarget)/mkstemps.o \
	$(objtarget)/pexecute.o $(objtarget)/splay-tree.o \
	$(objtarget)/xatexit.o $(objtarget)/xexit.o \
	$(objtarget)/xmalloc.o $(objtarget)/xstrerror.o

.PHONY: mkdirs

ifeq ($(TRG), native)
all:	mkdirs $(objdir)/libiberty/$(libname)

mkdirs:
	mkdir -p $(objdir)/libiberty

$(objdir)/libiberty/$(libname): $(LIBIBERTY)
	$(AR) $(ARFLAGS) $@ $(LIBIBERTY)
else
all:	mkdirs $(TBIN)/$(TRG)/libiberty.o

mkdirs:
	mkdir -p $(objtarget) $(TBIN)/$(TRG)

$(TBIN)/$(TRG)/libiberty.o: $(LIBIBERTY)
	$(standard_exec_prefix)/libfile -c $@ $(LIBIBERTY)
endif

$(objtarget)/choose-temp.o: choose-temp.c
$(objtarget)/cplus-dem.o: cplus-dem.c
$(objtarget)/getopt.o: getopt.c
$(objtarget)/getopt1.o: getopt1.c
$(objtarget)/mkstemps.o: mkstemps.c
$(objtarget)/obstack.o: obstack.c
$(objtarget)/pexecute.o: pexecute.c
$(objtarget)/splay-tree.o: splay-tree.c
$(objtarget)/xatexit.o: xatexit.c
$(objtarget)/xexit.o: xexit.c
$(objtarget)/xmalloc.o: xmalloc.c
$(objtarget)/xstrerror.o: xstrerror.c
 
