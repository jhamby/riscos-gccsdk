# Makefile for GNU flex - a lexical analyser
# Written by Nick Burrett <nick@dsvr.net>

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

all:	$(bin_dir)/flex$(exesfx) $(gcc_bin_dir)/$(TRG)/o/libfl \
	$(info_dir)/flex

$(objdir)/riscos-progs/flex.o:  $(SRCS) $(HDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS)

$(bin_dir)/flex$(exesfx):	$(objdir)/riscos-progs/flex.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/flex.o

$(gcc_bin_dir)/$(TRG)/o/libfl:	$(LIBFL_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBFL_OBJS)

$(info_dir)/flex:	doc/flex.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections doc/flex -o $@ -I doc

parse.h parse.c: parse.y
	$(BISON) -d parse.y -o parse.c

scan.c: scan.l
	$(FLEX) -t -o$@ scan.l > $@

