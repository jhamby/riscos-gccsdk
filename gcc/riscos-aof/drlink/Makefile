# Makefile for the ARM Object Format linker - DRLINK
# Written by Nick Burrett <nick@dsvr.net>

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE
exesfx =
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
endif

INCLUDES = -I. -I../../

drlinkobjdir = $(objdir)/riscos-aof/drlink/$(STAGE)

$(drlinkobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS =	$(drlinkobjdir)/aofiles.o \
	$(drlinkobjdir)/areas.o \
	$(drlinkobjdir)/debug.o \
	$(drlinkobjdir)/drlmain.o \
	$(drlinkobjdir)/files.o \
	$(drlinkobjdir)/heap.o \
	$(drlinkobjdir)/libraries.o \
	$(drlinkobjdir)/linkdebug.o \
	$(drlinkobjdir)/linkedit.o \
	$(drlinkobjdir)/linkfiles.o \
	$(drlinkobjdir)/messages.o \
	$(drlinkobjdir)/stdcode.o \
	$(drlinkobjdir)/symbols.o

ifeq (${STAGE},stage2)
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib
endif


all:	$(bin_dir)/drlink$(exesfx)

$(bin_dir)/drlink$(exesfx): $(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS)

$(drlinkobjdir)/aofiles.o: aofiles.c
$(drlinkobjdir)/areas.o: areas.c
$(drlinkobjdir)/c++names.o: c++names.c
$(drlinkobjdir)/debug.o: debug.c
$(drlinkobjdir)/drlmain.o: drlmain.c
$(drlinkobjdir)/files.o: files.c
$(drlinkobjdir)/heap.o: heap.c
$(drlinkobjdir)/libraries.o: libraries.c
$(drlinkobjdir)/linkdebug.o: linkdebug.c
$(drlinkobjdir)/linkedit.o: linkedit.c
$(drlinkobjdir)/linkfiles.o: linkfiles.c
$(drlinkobjdir)/messages.o: messages.c
$(drlinkobjdir)/stdcode.o: stdcode.c
$(drlinkobjdir)/symbols.o: symbols.c
