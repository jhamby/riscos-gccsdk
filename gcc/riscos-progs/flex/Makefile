INCLUDES = -I.
CFLAGS = -O2 $(OPTS) $(TARGET_DEFS) -DHAVE_CONFIG_H
CC = $(standard_exec_prefix)/gcc $(CFLAGS)

$(objdir)/riscos-progs/flex/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)

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

all:	$(BIN)/flex $(TBIN)/$(TRG)/libfl.o

$(BIN)/flex:	$(OBJS) $(TBIN)/$(TRG)/unixlib.o
	$(standard_exec_prefix)/gcc -o $@ $(OBJS)

$(TBIN)/$(TRG)/libfl.o:	$(LIBFL_OBJS)
	$(BIN)/libfile -c $@ $(LIBFL_OBJS)

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
