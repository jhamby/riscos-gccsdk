# Makefile for the RISC OS hacked InfoZIP

ifeq (${STAGE},stage1)
CFLAGS += -DCROSS_COMPILE -DFORRISCOS -DUNIX
exesfx =
else
CC = $(CROSS_CC) -mlibscl
CFLAGS = $(CROSS_CFLAGS) -DRISCOS -DASMV
endif

INCLUDES = -I. -I../../

infozipobjdir = $(objdir)/riscos-aof/infozip/$(STAGE)

$(infozipobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(infozipobjdir)/%.o: acorn/%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(infozipobjdir)/%.o: acorn/%.s
	$(CC) -o $@ -c $< $(INCLUDES)

OBJS =	$(infozipobjdir)/crc32.o \
	$(infozipobjdir)/crctab.o \
	$(infozipobjdir)/crypt.o \
	$(infozipobjdir)/deflate.o \
	$(infozipobjdir)/fileio.o \
	$(infozipobjdir)/globals.o \
	$(infozipobjdir)/trees.o \
	$(infozipobjdir)/ttyio.o \
	$(infozipobjdir)/util.o \
	$(infozipobjdir)/zip.o \
	$(infozipobjdir)/zipfile.o \
	$(infozipobjdir)/zipup.o

ifeq (${STAGE},stage1)
OBJS +=	$(infozipobjdir)/unix.o
endif

ifeq (${STAGE},stage2)
OBJS += $(infozipobjdir)/acornzip.o
OBJS += $(infozipobjdir)/riscos.o
OBJS += $(infozipobjdir)/match.o
OBJS += $(infozipobjdir)/sendbits.o
OBJS += $(infozipobjdir)/swiven.o
endif


all:	$(bin_dir)/zip$(exesfx)

$(bin_dir)/zip$(exesfx): $(OBJS)
	$(CC) -o $@ $(OBJS)

$(infozipobjdir)/crc32.o: crc32.c
$(infozipobjdir)/crctab.o: crctab.c
$(infozipobjdir)/crypt.o: crypt.c
$(infozipobjdir)/deflate.o: deflate.c
$(infozipobjdir)/fileio.o: fileio.c
$(infozipobjdir)/globals.o: globals.c
$(infozipobjdir)/trees.o: trees.c
$(infozipobjdir)/ttyio.o: ttyio.c
$(infozipobjdir)/unix.o: unix.c
$(infozipobjdir)/util.o: util.c
$(infozipobjdir)/zip.o: zip.c
$(infozipobjdir)/zipfile.o: zipfile.c
$(infozipobjdir)/zipup.o: zipup.c

ifeq (${STAGE},stage2)
$(infozipobjdir)/acornzip.o: acorn/acornzip.c
$(infozipobjdir)/riscos.o: acorn/riscos.c
$(infozipobjdir)/match.o: acorn/match.s
$(infozipobjdir)/sendbits.o: acorn/sendbits.s
$(infozipobjdir)/swiven.o: acorn/swiven.s
endif
