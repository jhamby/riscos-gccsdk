# Makefile for GNU flex - a lexical analyser
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/flex/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

fobj = $(objdir)/riscos-progs/flex

OBJS = \
	$(fobj)/buf.o \
	$(fobj)/ccl.o \
	$(fobj)/dfa.o \
	$(fobj)/ecs.o \
	$(fobj)/filter.o \
	$(fobj)/gen.o \
	$(fobj)/main.o \
	$(fobj)/misc.o \
	$(fobj)/nfa.o \
	$(fobj)/options.o \
	$(fobj)/parse.o \
	$(fobj)/regex.o \
	$(fobj)/scan.o \
	$(fobj)/scanopt.o \
	$(fobj)/skel.o \
	$(fobj)/sym.o \
	$(fobj)/tables.o \
	$(fobj)/tables_shared.o \
	$(fobj)/tblcmp.o \
	$(fobj)/yylex.o

LIBFL_OBJS = $(fobj)/libmain.o \
	$(fobj)/libyywrap.o

all:	$(bin_dir)/flex$(exesfx) $(gcc_bin_dir)/$(TRG)/o/libfl \
	$(info_dir)/flex

$(bin_dir)/flex$(exesfx):	$(OBJS)
	$(CROSS_CC) -o $@ $(OBJS)

$(gcc_bin_dir)/$(TRG)/o/libfl:	$(LIBFL_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBFL_OBJS)

$(info_dir)/flex:	doc/flex.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections doc/flex -o $@ -I doc

parse.h parse.c: parse.y
	$(BISON) -d parse.y -o parse.c

scan.c: scan.l
	$(FLEX) -t -o$@ scan.l > $@

$(fobj)/parse.o: parse.c parse.y parse.h
$(fobj)/buf.o: buf.c
$(fobj)/ccl.o: ccl.c
$(fobj)/dfa.o: dfa.c
$(fobj)/ecs.o: ecs.c
$(fobj)/filter.o: filter.c
$(fobj)/gen.o: gen.c
$(fobj)/main.o: main.c
$(fobj)/misc.o: misc.c
$(fobj)/nfa.o: nfa.c
$(fobj)/options.o: options.c
$(fobj)/regex.o: regex.c
$(fobj)/scan.o: scan.c flexdef.h parse.h
$(fobj)/scanopt.o: scanopt.c
$(fobj)/skel.o: skel.c
$(fobj)/sym.o: sym.c
$(fobj)/tables.o: tables.c
$(fobj)/tables_shared.o: tables_shared.c
$(fobj)/tblcmp.o: tblcmp.c
$(fobj)/yylex.o: yylex.c

