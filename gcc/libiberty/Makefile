# Makefile for GNU libiberty for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000, 2001 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This Makefile gets run twice.  First libiberty must be built
# natively because the GCC cross-compiler uses routines from
# this library.
#
# The libiberty will be built natively for the RISC OS binaries
# that require routines from this library.
#

INCLUDES = -I. -I../include

ifeq (${STAGE},stage1)
bin_dir = $(ux_bin_dir)
gcc_bin_dir = $(ux_gcc_bin_dir)
else
bin_dir = $(ro_bin_dir)
gcc_bin_dir = $(ro_gcc_bin_dir)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
INCLUDES += -I$(unixlib_incl_dir)
endif

CFLAGS += -DHAVE_CONFIG_H $(TARGET_DEFS)
objtarget = $(objdir)/libiberty/$(STAGE)

$(objtarget)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)


OBJS = $(objtarget)/asprintf.o \
	$(objtarget)/choose-temp.o \
	$(objtarget)/cp-demangle.o \
	$(objtarget)/concat.o \
	$(objtarget)/cplus-dem.o \
	$(objtarget)/dyn-string.o \
	$(objtarget)/fibheap.o \
	$(objtarget)/getruntime.o \
	$(objtarget)/getopt.o \
	$(objtarget)/getopt1.o \
	$(objtarget)/getpwd.o \
	$(objtarget)/hashtab.o \
	$(objtarget)/hex.o \
	$(objtarget)/lbasename.o \
	$(objtarget)/obstack.o \
	$(objtarget)/make-temp-file.o \
	$(objtarget)/make-relative-prefix.o \
	$(objtarget)/md5.o \
	$(objtarget)/mkstemps.o \
	$(objtarget)/partition.o \
	$(objtarget)/pexecute.o \
	$(objtarget)/physmem.o \
	$(objtarget)/safe-ctype.o \
	$(objtarget)/splay-tree.o \
	$(objtarget)/stpcpy.o \
	$(objtarget)/strsignal.o \
	$(objtarget)/vasprintf.o \
	$(objtarget)/xatexit.o \
	$(objtarget)/xexit.o \
	$(objtarget)/xmalloc.o \
	$(objtarget)/xmemdup.o \
	$(objtarget)/xstrdup.o \
	$(objtarget)/xstrerror.o

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

$(objtarget)/asprintf.o: asprintf.c
$(objtarget)/choose-temp.o: choose-temp.c
$(objtarget)/concat.o: concat.c
$(objtarget)/cp-demangle.o: cp-demangle.c
$(objtarget)/cplus-dem.o: cplus-dem.c
$(objtarget)/dyn-string.o: dyn-string.c
$(objtarget)/fibheap.o: fibheap.c
$(objtarget)/getruntime.o: getruntime.c
$(objtarget)/getopt.o: getopt.c
$(objtarget)/getopt1.o: getopt1.c
$(objtarget)/getpwd.o: getpwd.c
$(objtarget)/hashtab.o: hashtab.c
$(objtarget)/hex.o: hex.c
$(objtarget)/lbasename.o: lbasename.c
$(objtarget)/make-temp-file.o: make-temp-file.c
$(objtarget)/make-relative-prefix.o: make-relative-prefix.c
$(objtarget)/md5.o: md5.c
$(objtarget)/mkstemps.o: mkstemps.c
$(objtarget)/obstack.o: obstack.c
$(objtarget)/partition.o: partition.c
$(objtarget)/pexecute.o: pexecute.c
$(objtarget)/safe-ctype.o: safe-ctype.c
$(objtarget)/splay-tree.o: splay-tree.c
$(objtarget)/stpcpy.o: stpcpy.c
$(objtarget)/vasprintf.o: vasprintf.c
$(objtarget)/xatexit.o: xatexit.c
$(objtarget)/xexit.o: xexit.c
$(objtarget)/xmalloc.o: xmalloc.c
$(objtarget)/xmemdup.o: xmemdup.c
$(objtarget)/xstrdup.o: xstrdup.c
$(objtarget)/xstrerror.o: xstrerror.c
