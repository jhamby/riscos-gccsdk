# Makefile for `nm'
# Written by Nick Burrett <nick@dsvr.net>

ifeq (${STAGE},stage1)
prefix = $(unix_prefix)
CFLAGS += -DCROSS_COMPILE
exesfx = 
else
prefix = $(riscos_prefix)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
endif

INCLUDES = -I. -I../../ -I../../include
CFLAGS += -DHAVE_CONFIG_H
nmobjdir = $(objdir)/riscos-aof/nm/$(STAGE)
OBJS = $(nmobjdir)/nm.o

ifeq (${STAGE},stage2)
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib
endif

all:	$(bin_dir)/nm$(exesfx)

$(bin_dir)/nm$(exesfx):	$(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS) -L$(objdir)/libiberty/$(STAGE) -liberty

$(nmobjdir)/nm.o: nm.c ../../sdk-config.h
	$(CC) $(CFLAGS) -o $@ -c nm.c $(INCLUDES)

