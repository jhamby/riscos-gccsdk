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

OBJS =	$(drlinkobjdir)/Aoffiles.o \
	$(drlinkobjdir)/Areas.o \
	$(drlinkobjdir)/C++names.o \
	$(drlinkobjdir)/Debug.o \
	$(drlinkobjdir)/Drlmain.o \
	$(drlinkobjdir)/Files.o \
	$(drlinkobjdir)/Heap.o \
	$(drlinkobjdir)/Libraries.o \
	$(drlinkobjdir)/Linkdebug.o \
	$(drlinkobjdir)/Linkedit.o \
	$(drlinkobjdir)/Linkfiles.o \
	$(drlinkobjdir)/Messages.o \
	$(drlinkobjdir)/Stdcode.o \
	$(drlinkobjdir)/Symbols.o

all:	$(bin_dir)/drlink$(exesfx)

$(bin_dir)/drlink$(exesfx): $(OBJS)
	$(CC) -o $@ $(OBJS)

$(drlinkobjdir)/Aoffiles.o: Aoffiles.c
$(drlinkobjdir)/Areas.o: Areas.c
$(drlinkobjdir)/C++names.o: C++names.c
$(drlinkobjdir)/Debug.o: Debug.c
$(drlinkobjdir)/Drlmain.o: Drlmain.c
$(drlinkobjdir)/Files.o: Files.c
$(drlinkobjdir)/Heap.o: Heap.c
$(drlinkobjdir)/Libraries.o: Libraries.c
$(drlinkobjdir)/Linkdebug.o: Linkdebug.c
$(drlinkobjdir)/Linkedit.o: Linkedit.c
$(drlinkobjdir)/Linkfiles.o: Linkfiles.c
$(drlinkobjdir)/Messages.o: Messages.c
$(drlinkobjdir)/Stdcode.o: Stdcode.c
$(drlinkobjdir)/Symbols.o: Symbols.c
