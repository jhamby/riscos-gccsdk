# Makefile for GNU grep
# Written by Nick Burrett <nick@sqrt.co.uk>

INCLUDES = -I.  -I ./lib -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H 

SRCS = dfa.c grep.c grepmat.c kwset.c search.c
HDRS = config.h dfa.h getpagesize.h grep.h kwset.h system.h

LIBSRC = lib/closeout.c lib/error.c lib/exclude.c lib/hard-locale.c \
	lib/isdir.c lib/quotearg.c lib/regex.c lib/savedir.c \
	lib/xmalloc.c lib/xstrtol.c lib/xstrtoumax.c

LIBHDR = lib/closeout.h lib/error.h lib/exclude.h lib/hard-locale.h \
	lib/quotearg.h lib/regex.h lib/savedir.h \
	lib/xalloc.h lib/xstrtol.h

all:	$(bin_dir)/grep$(exesfx) $(info_dir)/grep

$(objdir)/riscos-progs/grep.o:  $(SRCS) $(HDRS) $(LIBSRC) $(LIBHDR)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS) $(LIBSRC) $(INCLUDES)

$(bin_dir)/grep$(exesfx):	$(objdir)/riscos-progs/grep.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/grep.o -L$(objdir)/libiberty/$(STAGE) -liberty

$(info_dir)/grep:	doc/grep.texi
	$(MAKEINFO) --no-split --number-sections doc/grep.texi -o $@ -I doc
