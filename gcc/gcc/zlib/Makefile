# Makefile for GNU Libio for the ARM/RISC OS GCC port
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
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


zlibobjdir=$(objdir)/gcc-$(VERSION)/zlib/$(STAGE)/$(TRG)

$(zlibobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = 	$(zlibobjdir)/adler32.o \
	$(zlibobjdir)/compress.o \
	$(zlibobjdir)/crc32.o \
	$(zlibobjdir)/deflate.o \
	$(zlibobjdir)/gzio.o \
	$(zlibobjdir)/infblock.o \
	$(zlibobjdir)/infcodes.o \
	$(zlibobjdir)/inffast.o \
	$(zlibobjdir)/inflate.o \
	$(zlibobjdir)/inftrees.o \
	$(zlibobjdir)/infutil.o \
	$(zlibobjdir)/trees.o \
	$(zlibobjdir)/uncompr.o \
	$(zlibobjdir)/zutil.o

ifeq (${STAGE},stage1)
all:	$(zlibobjdir)/libz.a

$(zlibobjdir)/libz.a: $(OBJS)
	$(AR) $(AR_FLAGS) $@ $(OBJS)
else
all:    $(zlibobjdir)/libz.o

$(zlibobjdir)/libz.o: $(OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(OBJS)
endif

$(zlibobjdir)/adler32.o: adler32.c
$(zlibobjdir)/compress.o: compress.c
$(zlibobjdir)/crc32.o: crc32.c
$(zlibobjdir)/deflate.o: deflate.c
$(zlibobjdir)/gzio.o: gzio.c
$(zlibobjdir)/infblock.o: infblock.c
$(zlibobjdir)/infcodes.o: infcodes.c
$(zlibobjdir)/inffast.o: inffast.c
$(zlibobjdir)/inflate.o: inflate.c
$(zlibobjdir)/inftrees.o: inftrees.c
$(zlibobjdir)/infutil.o: infutil.c
$(zlibobjdir)/maketree.o: maketree.c
$(zlibobjdir)/minigzip.o: minigzip.c
$(zlibobjdir)/trees.o: trees.c
$(zlibobjdir)/uncompr.o: uncompr.c
$(zlibobjdir)/zutil.o: zutil.c
