# Makefile for I'm bored of typing this stuff

INCLUDES = -I. -I../../include -I ../zlib

ifeq (${STAGE},stage1)
bin_dir = $(ux_bin_dir)
gcc_bin_dir = $(ux_gcc_bin_dir)
exesfx =
else
bin_dir = $(ro_bin_dir)
gcc_bin_dir = $(ro_gcc_bin_dir)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
INCLUDES += -I$(unixlib_incl_dir)
endif


jarobjdir=$(objdir)/gcc-$(VERSION)/fastjar/$(STAGE)

$(jarobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(jarobjdir)/jartool.o \
	$(jarobjdir)/dostime.o \
	$(jarobjdir)/compress.o \
	$(jarobjdir)/pushback.o

all:    $(bin_dir)/fastjar$(exesfx)

$(bin_dir)/fastjar$(exesfx):     $(OBJS)
	$(CC) -o $@ $(OBJS) -L$(objdir)/gcc-$(VERSION)/zlib/$(STAGE)/$(TRG) -L$(objdir)/libiberty/$(STAGE) -liberty -lz

$(jarobjdir)/jargrep.o: jargrep.c
$(jarobjdir)/jartool.o: jartool.c
$(jarobjdir)/dostime.o: dostime.c
$(jarobjdir)/compress.o: compress.c
$(jarobjdir)/pushback.o: pushback.c


