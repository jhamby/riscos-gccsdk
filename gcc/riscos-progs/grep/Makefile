# Makefile for GNU grep
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I.  -I ./lib -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H 

$(objdir)/riscos-progs/grep/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

gobj=$(objdir)/riscos-progs/grep

OBJS = \
	$(gobj)/dfa.o \
	$(gobj)/grep.o \
	$(gobj)/grepmat.o \
	$(gobj)/kwset.o \
	$(gobj)/search.o

LIB = \
	$(gobj)/lib/closeout.o \
	$(gobj)/lib/error.o \
	$(gobj)/lib/exclude.o \
	$(gobj)/lib/hard-locale.o \
	$(gobj)/lib/isdir.o \
	$(gobj)/lib/quotearg.o \
	$(gobj)/lib/regex.o \
	$(gobj)/lib/savedir.o \
	$(gobj)/lib/xmalloc.o \
	$(gobj)/lib/xstrtol.o \
	$(gobj)/lib/xstrtoumax.o

all:	dirs $(bin_dir)/grep$(exesfx) $(info_dir)/grep

dirs:
	mkdir -p $(gobj) $(gobj)/lib

$(bin_dir)/grep$(exesfx):	$(OBJS) $(LIB)
	$(CROSS_CC) -o $@ $(OBJS) $(LIB) -L$(objdir)/libiberty/$(STAGE) -liberty

$(info_dir)/grep:	doc/grep.texi
	$(MAKEINFO) --no-split --number-sections doc/grep.texi -o $@ -I doc

$(gobj)/grep.o:	grep.c
$(gobj)/grepmat.o: grepmat.c
$(gobj)/dfa.o:	dfa.c
$(gobj)/kwset.o: kwset.c
$(gobj)/search.o: search.c

$(gobj)/lib/closeout.o: lib/closeout.c
$(gobj)/lib/error.o: lib/error.c
$(gobj)/lib/exclude.o: lib/exclude.c
$(gobj)/lib/hard-locale.o: lib/hard-locale.c
$(gobj)/lib/isdir.o: lib/isdir.c
$(gobj)/lib/quotearg.o: lib/quotearg.c
$(gobj)/lib/regex.o: lib/regex.c
$(gobj)/lib/savedir.o: lib/savedir.c
$(gobj)/lib/xmalloc.o: lib/xmalloc.c
$(gobj)/lib/xstrtol.o: lib/xstrtol.c
$(gobj)/lib/xstrtoumax.o: lib/xstrtoumax.c

