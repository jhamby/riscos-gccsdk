# Makefile for GNU flex - a lexical analyser
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/flex/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/flex/parse.o \
	$(objdir)/riscos-progs/flex/ccl.o \
	$(objdir)/riscos-progs/flex/dfa.o \
	$(objdir)/riscos-progs/flex/ecs.o \
	$(objdir)/riscos-progs/flex/gen.o \
	$(objdir)/riscos-progs/flex/initscan.o \
	$(objdir)/riscos-progs/flex/main.o \
	$(objdir)/riscos-progs/flex/misc.o \
	$(objdir)/riscos-progs/flex/nfa.o \
	$(objdir)/riscos-progs/flex/skel.o \
	$(objdir)/riscos-progs/flex/sym.o \
	$(objdir)/riscos-progs/flex/tblcmp.o \
	$(objdir)/riscos-progs/flex/yylex.o

LIBFL_OBJS = $(objdir)/riscos-progs/flex/libmain.o \
	$(objdir)/riscos-progs/flex/libyywrap.o

all:	$(bin_dir)/flex$(exesfx) $(gcc_bin_dir)/$(TRG)/o/libfl

$(bin_dir)/flex$(exesfx):	$(OBJS)
	$(CROSS_CC) -o $@ $(OBJS)

$(gcc_bin_dir)/$(TRG)/o/libfl:	$(LIBFL_OBJS)
	$(CROSS_AR) $(CROSS_AR_FLAGS) $@ $(LIBFL_OBJS)

parse.h parse.c: parse.y
	bison -d parse.y -o parse.c

$(objdir)/riscos-progs/flex/parse.o: parse.c parse.y parse.h
$(objdir)/riscos-progs/flex/ccl.o: ccl.c
$(objdir)/riscos-progs/flex/dfa.o: dfa.c
$(objdir)/riscos-progs/flex/ecs.o: ecs.c
$(objdir)/riscos-progs/flex/gen.o: gen.c
$(objdir)/riscos-progs/flex/initscan.o: initscan.c
$(objdir)/riscos-progs/flex/main.o: main.c
$(objdir)/riscos-progs/flex/misc.o: misc.c
$(objdir)/riscos-progs/flex/nfa.o: nfa.c
$(objdir)/riscos-progs/flex/skel.o: skel.c
$(objdir)/riscos-progs/flex/sym.o: sym.c
$(objdir)/riscos-progs/flex/tblcmp.o: tblcmp.c
$(objdir)/riscos-progs/flex/yylex.o: yylex.c
$(objdir)/riscos-progs/flex/libmain.o: libmain.c
$(objdir)/riscos-progs/flex/libyywrap.o: libyywrap.c
