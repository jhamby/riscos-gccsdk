# Makefile for GNU bison
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I. -I ./lib -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H -DPKGDATADIR=\"GCCsys:bison\"

$(objdir)/riscos-progs/bison/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

bobj = $(objdir)/riscos-progs/bison

OBJS = $(bobj)/assoc.o \
	$(bobj)/closure.o \
	$(bobj)/complain.o \
	$(bobj)/conflicts.o \
	$(bobj)/derives.o \
	$(bobj)/files.o \
	$(bobj)/getargs.o \
	$(bobj)/gram.o \
	$(bobj)/lalr.o \
	$(bobj)/location.o \
	$(bobj)/LR0.o \
	$(bobj)/main.o \
	$(bobj)/muscle_tab.o \
	$(bobj)/nullable.o \
	$(bobj)/output.o \
	$(bobj)/parse-gram.o \
	$(bobj)/print.o \
	$(bobj)/print_graph.o \
	$(bobj)/reader.o \
	$(bobj)/reduce.o \
	$(bobj)/relation.o \
	$(bobj)/scan-gram.o \
	$(bobj)/scan-skel.o \
	$(bobj)/state.o \
	$(bobj)/symlist.o \
	$(bobj)/symtab.o \
	$(bobj)/tables.o \
	$(bobj)/uniqstr.o \
	$(bobj)/vcg.o

LIB = \
	$(bobj)/lib/abitset.o \
	$(bobj)/lib/argmatch.o \
	$(bobj)/lib/basename.o \
	$(bobj)/lib/bitset.o \
	$(bobj)/lib/bitset_stats.o \
	$(bobj)/lib/bitsetv.o \
	$(bobj)/lib/bitsetv-print.o \
	$(bobj)/lib/dirname.o \
	$(bobj)/lib/ebitset.o \
	$(bobj)/lib/error.o \
	$(bobj)/lib/get-errno.o \
	$(bobj)/lib/hash.o \
	$(bobj)/lib/lbitset.o \
	$(bobj)/lib/mbswidth.o \
	$(bobj)/lib/quotearg.o \
	$(bobj)/lib/quote.o \
	$(bobj)/lib/subpipe.o \
	$(bobj)/lib/timevar.o \
	$(bobj)/lib/xmalloc.o \
	$(bobj)/lib/xstrndup.o

all:	dirs $(bin_dir)/bison$(exesfx)

dirs:
	mkdir -p $(bobj) $(bobj)/lib

$(bin_dir)/bison$(exesfx):	$(OBJS) $(LIB)
	$(CROSS_CC) -o $@ $(OBJS) $(LIB) -L$(objdir)/libiberty/$(STAGE) -liberty

scan-gram.c: scan-gram.l
	flex -t -o$@ scan-gram.l > $@

scan-skel.c: scan-skel.l
	flex -t -o$@ scan-skel.l > $@

$(bobj)/scan-gram.o: scan-gram.c scan-gram.l
$(bobj)/scan-skel.o: scan-skel.c scan-skel.l
