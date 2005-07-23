# Makefile for the ARM Object Format linker - DrLink
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005 GCCSDK Developers

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE -D_GNU_SOURCE
exesfx =
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -D_GNU_SOURCE
endif

INCLUDES = -I. -I../../
CFLAGS += -Wall

drlinkobjdir = $(objdir)/riscos-aof/drlink/$(STAGE)

all:	$(bin_dir)/drlink$(exesfx)

SRCS =	aofiles.c \
	areas.c \
	debug.c \
	drlmain.c \
	files.c \
	heap.c \
	libraries.c \
	linkdebug.c \
	linkedit.c \
	linkfiles.c \
	messages.c \
	stdcode.c \
	symbols.c

HDRS =	../../sdk-config.h \
	areahdr.h \
	chunkhdr.h \
	config.h \
	debugprocs.h \
	drlhdr.h \
	edithdr.h \
	filehdr.h \
	libraries.h \
	procdefs.h \
	symbolhdr.h

ifeq (${STAGE},stage1)
# This funky line tells make to change all '.c' suffixes to '.o' and add
# '$(drlinkobjdir)/' to each object.
OBJS = $(addprefix $(drlinkobjdir)/, $(SRCS:.c=.o))

$(drlinkobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(bin_dir)/drlink$(exesfx): $(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS)
endif

ifeq (${STAGE},stage2)
# Since we know that we are compiling with GCC 3.4, we can compile the
# application using inter-module analysis, effectivly building as if all
# functions are contained within one source file.
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib

$(drlinkobjdir)/drlink.o:	$(SRCS) $(DEPS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/drlink$(exesfx): $(drlinkobjdir)/drlink.o $(DEPS)
	$(CC) -o $@ $(drlinkobjdir)/drlink.o
endif
