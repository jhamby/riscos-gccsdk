# Makefile for GNU diffutils
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I. -I ./lib -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/diff/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

dobj = $(objdir)/riscos-progs/diff


DIFFOBJS = \
	$(dobj)/analyze.o \
	$(dobj)/context.o \
	$(dobj)/diff.o \
	$(dobj)/dir.o \
	$(dobj)/ed.o \
	$(dobj)/ifdef.o \
	$(dobj)/io.o \
	$(dobj)/normal.o \
	$(dobj)/side.o \
	$(dobj)/util.o \
	$(dobj)/version.o

DIFF3OBJS = $(dobj)/diff3.o \
	$(dobj)/version.o


SDIFFOBJS = $(dobj)/sdiff.o \
	$(dobj)/version.o

CMPOBJS = $(dobj)/cmp.o \
	$(dobj)/version.o

LIB = \
	$(dobj)/lib/basename.o \
	$(dobj)/lib/cmpbuf.o \
	$(dobj)/lib/c-stack.o \
	$(dobj)/lib/error.o \
	$(dobj)/lib/exclude.o \
	$(dobj)/lib/exitfail.o \
	$(dobj)/lib/freesoft.o \
	$(dobj)/lib/hard-locale.o \
	$(dobj)/lib/offtostr.o \
	$(dobj)/lib/posixver.o \
	$(dobj)/lib/prepargs.o \
	$(dobj)/lib/quotesys.o \
	$(dobj)/lib/regex.o \
	$(dobj)/lib/setmode.o \
	$(dobj)/lib/strftime.o \
	$(dobj)/lib/xmalloc.o \
	$(dobj)/lib/xstrtol.o \
	$(dobj)/lib/xstrtoumax.o

all:	dirs \
	$(bin_dir)/diff$(exesfx) $(bin_dir)/diff3$(exesfx) \
	$(bin_dir)/sdiff$(exesfx) $(bin_dir)/cmp$(exesfx) \
	$(info_dir)/diff

dirs:
	mkdir -p $(dobj) $(dobj)/lib

$(bin_dir)/diff$(exesfx):	$(DIFFOBJS) $(LIB)
	$(CROSS_CC) -o $@ $(DIFFOBJS) $(LIB)

$(bin_dir)/diff3$(exesfx):	$(DIFF3OBJS) $(LIB)
	$(CROSS_CC) -o $@ $(DIFF3OBJS) $(LIB)

$(bin_dir)/sdiff$(exesfx):	$(SDIFFOBJS) $(LIB)
	$(CROSS_CC) -o $@ $(SDIFFOBJS) $(LIB)

$(bin_dir)/cmp$(exesfx):	$(CMPOBJS) $(LIB)
	$(CROSS_CC) -o $@ $(CMPOBJS) $(LIB)

$(info_dir)/diff:	doc/diff.texi doc/fdl.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections doc/diff.texi -o $@ -I doc
