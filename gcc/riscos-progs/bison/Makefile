# Makefile for GNU bison
# Written by Nick Burrett <nick@sqrt.co.uk>

INCLUDES = -I. -I ./lib -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H -DPKGDATADIR=\"GCCsys:bison\"

SRCS = assoc.c closure.c complain.c conflicts.c derives.c files.c getargs.c \
	gram.c lalr.c location.c LR0.c main.c muscle_tab.c \
	nullable.c output.c parse-gram.c print.c print_graph.c \
	reader.c reduce.c relation.c scan-gram.c scan-skel.c \
	state.c symlist.c symtab.c tables.c uniqstr.c vcg.c

HDRS = assoc.h closure.h complain.h config.h conflicts.h derives.h \
	files.h getargs.h gram.h lalr.h location.h LR0.h \
	muscle_tab.h nullable.h output.h parse-gram.h print_graph.h \
	print.h reader.h reduce.h relation.h state.h symlist.h \
	symtab.h system.h tables.h uniqstr.h vcg_defaults.h vcg.h

LIBSRCS = lib/abitset.c lib/argmatch.c lib/basename.c lib/bitset.c \
	lib/bitset_stats.c lib/bitsetv.c lib/bitsetv-print.c \
	lib/dirname.c lib/ebitset.c lib/error.c lib/get-errno.c \
	lib/hash.c lib/lbitset.c lib/mbswidth.c lib/quotearg.c \
	lib/quote.c lib/subpipe.c lib/timevar.c lib/xmalloc.c \
	lib/xstrndup.c

LIBHDRS = lib/abitset.h lib/argmatch.h lib/bitset.h lib/bbitset.h \
	lib/bitset_stats.h lib/bitsetv.h lib/bitsetv-print.h \
	lib/dirname.h lib/ebitset.h lib/error.h lib/get-errno.h \
	lib/gettext.h lib/hash.h lib/lbitset.h lib/mbswidth.h lib/quotearg.h \
	lib/quote.h lib/subpipe.h lib/timevar.h lib/unlocked-io.h \
	lib/xalloc.h

all:	$(bin_dir)/bison$(exesfx) $(info_dir)/bison

$(objdir)/riscos-progs/bison.o:  $(SRCS) $(HDRS) $(LIBSRCS) $(LIBHDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS) $(LIBSRCS) $(INCLUDES)

$(bin_dir)/bison$(exesfx):	$(objdir)/riscos-progs/bison.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/bison.o -L$(objdir)/libiberty/$(STAGE) -liberty

$(info_dir)/bison:	doc/bison.texinfo doc/fdl.texi doc/gpl.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections doc/bison.texinfo -o $@ -I doc

#scan-gram.c: scan-gram.l
#	$(FLEX) -t -o$@ scan-gram.l > $@

#scan-skel.c: scan-skel.l
#	$(FLEX) -t -o$@ scan-skel.l > $@

#$(bobj)/scan-gram.o: scan-gram.c scan-gram.l
#$(bobj)/scan-skel.o: scan-skel.c scan-skel.l
