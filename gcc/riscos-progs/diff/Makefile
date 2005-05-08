# Makefile for GNU diffutils
# Written by Nick Burrett <nick@sqrt.co.uk>

INCLUDES = -I. -I ./lib -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H

DIFFSRC = analyze.c context.c diff.c dir.c ed.c ifdef.c io.c \
	normal.c side.c util.c version.c

DIFF3SRC = diff3.c version.c
SDIFFSRC = sdiff.c version.c
CMPSRC = cmp.c version.c

LIBSRC = lib/basename.c lib/cmpbuf.c lib/c-stack.c lib/error.c \
	lib/exclude.c lib/exitfail.c lib/freesoft.c lib/hard-locale.c \
	lib/offtostr.c lib/posixver.c lib/prepargs.c lib/quotesys.c \
	lib/regex.c lib/setmode.c lib/strftime.c lib/xmalloc.c \
	lib/xstrtol.c lib/xstrtoumax.c

LIBHDR = lib/cmpbuf.h lib/c-stack.h lib/dirname.h lib/error.h \
	lib/exclude.h lib/exitfail.h lib/freesoft.h lib/gettext.h \
	lib/hard-locale.h lib/inttostr.h lib/posixver.h lib/prepargs.h \
	lib/quotesys.h lib/regex.h lib/setmode.h lib/unlocked-io.h \
	lib/xalloc.h lib/xstrtol.h

HDRS = config.h diff.h system.h

all:	$(bin_dir)/diff$(exesfx) $(bin_dir)/diff3$(exesfx) \
	$(bin_dir)/sdiff$(exesfx) $(bin_dir)/cmp$(exesfx) \
	$(info_dir)/diff

$(objdir)/riscos-progs/diff.o:	$(DIFFSRC) $(HDRS) $(LIBSRC) $(LIBHDR)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(DIFFSRC) $(LIBSRC) $(INCLUDES)

$(objdir)/riscos-progs/diff3.o:	$(DIFF3SRC) $(HDRS) $(LIBSRC) $(LIBHDR)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(DIFF3SRC) $(LIBSRC) $(INCLUDES)

$(objdir)/riscos-progs/sdiff.o:	$(SDIFFSRC) $(HDRS) $(LIBSRC) $(LIBHDR)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SDIFFSRC) $(LIBSRC) $(INCLUDES)

$(objdir)/riscos-progs/cmp.o:	$(CMPSRC) $(HDRS) $(LIBSRC) $(LIBHDR)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(CMPSRC) $(LIBSRC) $(INCLUDES)

$(bin_dir)/diff$(exesfx):	$(objdir)/riscos-progs/diff.o 
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/diff.o 

$(bin_dir)/diff3$(exesfx):	$(objdir)/riscos-progs/diff3.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/diff3.o 

$(bin_dir)/sdiff$(exesfx):	$(objdir)/riscos-progs/sdiff.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/sdiff.o

$(bin_dir)/cmp$(exesfx):	$(objdir)/riscos-progs/cmp.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/cmp.o

$(info_dir)/diff:	doc/diff.texi doc/fdl.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections doc/diff.texi -o $@ -I doc
