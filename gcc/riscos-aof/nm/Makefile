# Makefile for `nm'
# Copyright (c) 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005 GCCSDK Developers

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE -D_GNU_SOURCE
exesfx = 
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -D_GNU_SOURCE
endif

INCLUDES = -I. -I../../ -I../../include
CFLAGS += -Wall

nmobjdir = $(objdir)/riscos-aof/nm/$(STAGE)
all:	$(bin_dir)/nm$(exesfx)

OBJS = $(nmobjdir)/nm.o

ifeq (${STAGE},stage2)
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib
endif

$(bin_dir)/nm$(exesfx):	$(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS) -L$(objdir)/libiberty/$(STAGE) -liberty

$(nmobjdir)/nm.o: nm.c
	$(CC) $(CFLAGS) -o $@ -c nm.c $(INCLUDES)
