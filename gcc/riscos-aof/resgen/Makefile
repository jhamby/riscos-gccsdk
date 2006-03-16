# Makefile for the RISC OS Resource generator
# Copyright (c) 2006 GCCSDK Developers

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE -D_GNU_SOURCE
exesfx =
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -D_GNU_SOURCE -D__RISCOSIFY
endif

INCLUDES = -I. -I../../
CFLAGS += -Wall -W

resgenobjdir = $(objdir)/riscos-aof/resgen/$(STAGE)

all:	$(bin_dir)/resgen$(exesfx)

riscos-dist:
	mkdir -p $(info_dir)/resgen
	cp -p COPYING $(info_dir)/resgen
	cp -p Resgen $(info_dir)/resgen

SRCS = resgen.c

HDRS = ../../sdk-config.h

ifeq (${STAGE},stage1)
# This funky line tells make to change all '.c' suffixes to '.o' and add
# '$(resgenobjdir)/' to each object.
OBJS = $(addprefix $(resgenobjdir)/, $(SRCS:.c=.o))

$(resgenobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(bin_dir)/resgen$(exesfx):	$(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
endif

ifeq (${STAGE},stage2)
# Since we know that we are compiling with GCC 3.4, we can compile the
# application using inter-module analysis, effectivly building as if all
# functions are contained within one source file.
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib

$(resgenobjdir)/resgen.o: $(SRCS) $(DEPS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/resgen$(exesfx):	$(resgenobjdir)/resgen.o $(DEPS)
	$(CC) -o $@ $(resgenobjdir)/resgen.o
endif
