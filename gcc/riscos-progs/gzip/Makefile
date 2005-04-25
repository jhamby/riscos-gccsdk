# Makefile for GNU gzip - a file compression utility
# Written by Nick Burrett <nick@sqrt.co.uk>

INCLUDES = -I.

SRCS = bits.c crypt.c deflate.c getopt.c gzip.c inflate.c \
	lzw.c trees.c unlzh.c unlzw.c unpack.c unzip.c util.c zip.c
HDRS = crypt.h getopt.h gzip.h lzw.h revision.h tailor.h

all:	$(bin_dir)/gzip$(exesfx)

$(objdir)/riscos-progs/gzip.o:	$(SRCS) $(HDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/gzip$(exesfx):	$(objdir)/riscos-progs/gzip.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/gzip.o
