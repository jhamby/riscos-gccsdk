# Makefile for LD - frontend to DRLINK
# Written by Nick Burrett <nick@dsvr.net>

ifeq (${STAGE},stage1)
prefix = $(unix_prefix)
CFLAGS += -DCROSS_COMPILE
else
prefix = $(riscos_prefix)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
endif

INCLUDES = -I. -I../../ -I../../include
CFLAGS += -DHAVE_CONFIG_H

ldobjdir = $(objdir)/riscos-aof/ld/$(STAGE)

$(ldobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(ldobjdir)/ld.o $(ldobjdir)/rname.o

all:	$(gcc_bin_dir)/ld

$(gcc_bin_dir)/ld:	$(OBJS)
	$(CC) -o $@ $(OBJS) -L$(objdir)/libiberty/$(STAGE) -liberty

$(ldobjdir)/ld.o: ld.c
	$(CC) $(CFLAGS) -o $@ -c ld.c $(INCLUDES) -DSTANDARD_EXEC_PREFIX=\"$(prefix)$(gn_standard_exec_prefix)\"

$(ldobjdir)/rname.o: rname.c
