# Makefile for GNU libiberty for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This Makefile gets run twice.  First libiberty must be built
# natively because the GCC cross-compiler uses routines from
# this library.
#
# The libiberty will be built natively for the RISC OS binaries
# that require routines from this library.
#

ifeq (${STAGE},stage1)
bin_dir = $(ux_bin_dir)
gcc_bin_dir = $(ux_gcc_bin_dir)
else
bin_dir = $(ro_bin_dir)
gcc_bin_dir = $(ro_gcc_bin_dir)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
endif

CFLAGS += -DIN_GCC -DHAVE_CONFIG_H $(TARGET_DEFS)
INCLUDES = -I. -I../include

objtarget = $(objdir)/libiberty/$(STAGE)

$(objtarget)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)


OBJS = $(objtarget)/choose-temp.o $(objtarget)/cplus-dem.o \
	$(objtarget)/getopt.o $(objtarget)/getopt1.o \
	$(objtarget)/obstack.o $(objtarget)/mkstemps.o \
	$(objtarget)/pexecute.o $(objtarget)/splay-tree.o \
	$(objtarget)/stpcpy.o \
	$(objtarget)/xatexit.o $(objtarget)/xexit.o \
	$(objtarget)/xmalloc.o $(objtarget)/xstrerror.o

.PHONY: mkdirs


ifeq (${STAGE},stage1)
all:	mkdirs $(objtarget)/libiberty.a

mkdirs:
	mkdir -p $(objtarget)

$(objtarget)/libiberty.a: $(OBJS)
	$(AR) $(AR_FLAGS) $@ $(OBJS)
else
all:	mkdirs $(gcc_bin_dir)/$(TRG)/o/libiberty $(objtarget)/libiberty.o

mkdirs:
	mkdir -p $(objtarget)

$(objtarget)/libiberty.o: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)

$(gcc_bin_dir)/$(TRG)/o/libiberty: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)
endif

$(objtarget)/choose-temp.o: choose-temp.c
$(objtarget)/cplus-dem.o: cplus-dem.c
$(objtarget)/getopt.o: getopt.c
$(objtarget)/getopt1.o: getopt1.c
$(objtarget)/mkstemps.o: mkstemps.c
$(objtarget)/obstack.o: obstack.c
$(objtarget)/pexecute.o: pexecute.c
$(objtarget)/splay-tree.o: splay-tree.c
$(objtarget)/stpcpy.o: stpcpy.c
$(objtarget)/xatexit.o: xatexit.c
$(objtarget)/xexit.o: xexit.c
$(objtarget)/xmalloc.o: xmalloc.c
$(objtarget)/xstrerror.o: xstrerror.c
