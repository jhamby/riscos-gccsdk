# Makefile for GNU gzip - a file compression utility
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I.

$(objdir)/riscos-progs/gzip/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/gzip/bits.o \
	$(objdir)/riscos-progs/gzip/crypt.o \
	$(objdir)/riscos-progs/gzip/deflate.o \
	$(objdir)/riscos-progs/gzip/getopt.o \
	$(objdir)/riscos-progs/gzip/gzip.o \
	$(objdir)/riscos-progs/gzip/inflate.o \
	$(objdir)/riscos-progs/gzip/lzw.o \
	$(objdir)/riscos-progs/gzip/trees.o \
	$(objdir)/riscos-progs/gzip/unlzh.o \
	$(objdir)/riscos-progs/gzip/unlzw.o \
	$(objdir)/riscos-progs/gzip/unpack.o \
	$(objdir)/riscos-progs/gzip/unzip.o \
	$(objdir)/riscos-progs/gzip/util.o \
	$(objdir)/riscos-progs/gzip/zip.o

all:	$(bin_dir)/gzip$(exesfx)

$(bin_dir)/gzip$(exesfx):	$(OBJS)
	$(CROSS_CC) -o $@ $(OBJS)

$(objdir)/riscos-progs/gzip/bits.o: bits.c
$(objdir)/riscos-progs/gzip/crypt.o: crypt.c
$(objdir)/riscos-progs/gzip/deflate.o: deflate.c
$(objdir)/riscos-progs/gzip/getopt.o: getopt.c
$(objdir)/riscos-progs/gzip/gzip.o: gzip.c
$(objdir)/riscos-progs/gzip/inflate.o: inflate.c
$(objdir)/riscos-progs/gzip/lzw.o: lzw.c
$(objdir)/riscos-progs/gzip/trees.o: trees.c
$(objdir)/riscos-progs/gzip/unlzh.o: unlzh.c
$(objdir)/riscos-progs/gzip/unlzw.o: unlzw.c
$(objdir)/riscos-progs/gzip/unpack.o: unpack.c
$(objdir)/riscos-progs/gzip/unzip.o: unzip.c
$(objdir)/riscos-progs/gzip/util.o: util.c
$(objdir)/riscos-progs/gzip/zip.o: zip.c
