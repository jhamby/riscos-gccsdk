# Makefile for GNU bison
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/bison/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/bison/allocate.o \
	$(objdir)/riscos-progs/bison/closure.o \
	$(objdir)/riscos-progs/bison/conflicts.o \
	$(objdir)/riscos-progs/bison/derives.o \
	$(objdir)/riscos-progs/bison/files.o \
	$(objdir)/riscos-progs/bison/getargs.o \
	$(objdir)/riscos-progs/bison/gram.o \
	$(objdir)/riscos-progs/bison/lalr.o \
	$(objdir)/riscos-progs/bison/lex.o \
	$(objdir)/riscos-progs/bison/LR0.o \
	$(objdir)/riscos-progs/bison/main.o \
	$(objdir)/riscos-progs/bison/nullable.o \
	$(objdir)/riscos-progs/bison/output.o \
	$(objdir)/riscos-progs/bison/print.o \
	$(objdir)/riscos-progs/bison/reader.o \
	$(objdir)/riscos-progs/bison/reduce.o \
	$(objdir)/riscos-progs/bison/symtab.o \
	$(objdir)/riscos-progs/bison/warshall.o


all:	$(bin_dir)/bison$(exesfx)

$(bin_dir)/bison$(exesfx):	$(OBJS)
	$(CROSS_CC) -o $@ $(OBJS) -L$(objdir)/libiberty/$(STAGE) -liberty

$(objdir)/riscos-progs/bison/allocate.o: allocate.c
$(objdir)/riscos-progs/bison/closure.o: closure.c
$(objdir)/riscos-progs/bison/conflicts.o: conflicts.c
$(objdir)/riscos-progs/bison/derives.o: derives.c
$(objdir)/riscos-progs/bison/files.o: files.c
$(objdir)/riscos-progs/bison/getargs.o: getargs.c
$(objdir)/riscos-progs/bison/gram.o: gram.c
$(objdir)/riscos-progs/bison/lalr.o: lalr.c
$(objdir)/riscos-progs/bison/lex.o: lex.c
$(objdir)/riscos-progs/bison/LR0.o: LR0.c
$(objdir)/riscos-progs/bison/main.o: main.c
$(objdir)/riscos-progs/bison/nullable.o: nullable.c
$(objdir)/riscos-progs/bison/output.o: output.c
$(objdir)/riscos-progs/bison/print.o: print.c
$(objdir)/riscos-progs/bison/reader.o: reader.c
$(objdir)/riscos-progs/bison/reduce.o: reduce.c
$(objdir)/riscos-progs/bison/symtab.o: symtab.c
$(objdir)/riscos-progs/bison/warshall.o: warshall.c
