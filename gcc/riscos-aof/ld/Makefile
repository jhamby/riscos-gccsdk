# Makefile for LD - frontend to DRLINK
# Written by Nick Burrett <nick@dsvr.net>

ifeq (${STAGE},stage1)
prefix = $(unix_prefix)
CFLAGS += -DCROSS_COMPILE -DLIBDIR=\"$(ux_gcc_bin_dir)/\"
exesfx =
else
prefix = $(riscos_prefix)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
endif

INCLUDES = -I. -I../../ -I../../include
CFLAGS += -DHAVE_CONFIG_H
ldobjdir = $(objdir)/riscos-aof/ld/$(STAGE)
OBJS = $(ldobjdir)/ld.o

ifeq (${STAGE},stage2)
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib
endif

all:	$(gcc_bin_dir)/ld$(exesfx)

$(gcc_bin_dir)/ld$(exesfx):	$(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS) -L$(objdir)/libiberty/$(STAGE) -liberty

$(ldobjdir)/ld.o: ld.c ../../sdk-config.h ../../include/obstack.h \
	../../include/demangle.h
	$(CC) $(CFLAGS) -o $@ -c ld.c $(INCLUDES) \
	-DSTANDARD_EXEC_PREFIX=\"$(prefix)$(gn_standard_exec_prefix)\"

