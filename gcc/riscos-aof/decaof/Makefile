# Makefile for DecAOF
# Copyright (c) 2004, 2005 GCCSDK Developers

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE -D_GNU_SOURCE
exesfx =
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -D_GNU_SOURCE
endif

INCLUDES = -I. -I ../../
CFLAGS += -Wall

decaofobjdir = $(objdir)/riscos-aof/decaof/$(STAGE)

all:	$(bin_dir)/decaof$(exesfx)

riscos-dist:
	mkdir -p $(info_dir)/decaof
	cp -p COPYING $(info_dir)/decaof
	cp -p README $(info_dir)/decaof

SRCS = decode.c io.c error.c main.c misc.c

HDRS = ../../sdk-config.h config.h decaof.h error.h io.h main.h misc.h

ifeq (${STAGE},stage1)
# This funky line tells make to change all '.c' suffixes to '.o' and add
# '$(decaofobjdir)/' to each object.
OBJS = $(addprefix $(decaofobjdir)/, $(SRCS:.c=.o))

$(decaofobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(bin_dir)/decaof$(exesfx): $(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS)
endif

ifeq (${STAGE},stage2)
# Since we know that we are compiling with GCC 3.4, we can compile the
# application using inter-module analysis, effectivly building as if all
# functions are contained within one source file.
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib

$(decaofobjdir)/decaof.o:	$(SRCS) $(DEPS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/decaof$(exesfx): $(decaofobjdir)/decaof.o $(DEPS)
	$(CC) -o $@ $(decaofobjdir)/decaof.o
endif
