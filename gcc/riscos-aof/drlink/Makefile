# Makefile for the ARM Object Format linker - DrLink
# Written by Nick Burrett <nick@sqrt.co.uk>

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE
exesfx =
else
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
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

HDRS =	areahdr.h chunkhdr.h debugprocs.h drlhdr.h \
	edithdr.h filehdr.h procdefs.h symbolhdr.h

ifeq (${STAGE},stage1)
OBJS = $(addprefix $(drlinkobjdir)/, $(SRCS:.c=.o))

$(drlinkobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

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
endif

ifeq (${STAGE},stage2)
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib

$(drlinkobjdir)/drlink.o:	$(SRCS) $(DEPS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/drlink$(exesfx): $(drlinkobjdir)/drlink.o $(DEPS)
	$(CC) -o $@ $(drlinkobjdir)/drlink.o
endif
