# Makefile for GNU flex - a lexical analyser
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005 GCCSDK Developers

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/flex/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

fobj = $(objdir)/riscos-progs/flex

SRCS = buf.c ccl.c dfa.c ecs.c filter.c gen.c main.c misc.c nfa.c \
	options.c parse.c regex.c scan.c scanopt.c skel.c sym.c \
	tables.c tables_shared.c tblcmp.c yylex.c

HDRS = config.h flexdef.h flexint.h FlexLexer.h gettext.h \
	options.h parse.h scanopt.h tables.h tables_shared.h version.h

LIBFL_OBJS = $(fobj)/libmain.o \
	$(fobj)/libyywrap.o

all:	$(bin_dir)/flex$(exesfx) $(gcc_bin_dir)/$(TRG)/o/libfl

riscos-dist:	$(info_dir)/flex,faf

$(info_dir)/flex,faf: doc/flex.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections --html doc/flex -o $@ -I doc

$(objdir)/riscos-progs/flex.o:  $(SRCS) $(HDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS)

$(bin_dir)/flex$(exesfx):	$(objdir)/riscos-progs/flex.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/flex.o

$(gcc_bin_dir)/$(TRG)/o/libfl:	$(LIBFL_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBFL_OBJS)

parse.h parse.c: parse.y
	$(BISON) -d parse.y -o parse.c

scan.c: scan.l
	$(FLEX) -t -o$@ scan.l > $@

