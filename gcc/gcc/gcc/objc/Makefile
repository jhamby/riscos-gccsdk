# Makefile for the GNU Objective-C compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty

$(gccobjdir)/objc/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: objc

.PHONY: objc

objc: $(gcc_bin_dir)/cc1obj

LIBDEPS = $(gccobjdir)/prefix.o

ifeq (${STAGE},stage1)
RISC_OBJS=
RISCOS_OBJS=
else
RISC_OBJS = $(gccobjdir)/config/arm/riscos_fix.o
RISCOS_OBJS = $(RISC_OBJS) $(gccobjdir)/config/arm/riscos.o
endif

# Language-independent object files
OBJS =	$(gccobjdir)/print-tree.o \
	$(gccobjdir)/genrtl.o \
	$(gccobjdir)/stmt.o \
	$(gccobjdir)/except.o \
	$(gccobjdir)/calls.o \
	$(gccobjdir)/expmed.o \
	$(gccobjdir)/explow.o \
	$(gccobjdir)/varasm.o \
	$(gccobjdir)/rtl.o \
	$(gccobjdir)/print-rtl.o \
	$(gccobjdir)/rtlanal.o \
	$(gccobjdir)/emit-rtl.o \
	$(gccobjdir)/real.o \
	$(gccobjdir)/regmove.o \
	$(gccobjdir)/bitmap.o \
	$(gccobjdir)/alias.o \
	$(gccobjdir)/integrate.o \
	$(gccobjdir)/dyn-string.o \
	$(gccobjdir)/dwarf2out.o \
	$(gccobjdir)/jump.o \
	$(gccobjdir)/cse.o \
	$(gccobjdir)/loop.o \
	$(gccobjdir)/unroll.o \
	$(gccobjdir)/flow.o \
	$(gccobjdir)/stupid.o \
	$(gccobjdir)/combine.o \
	$(gccobjdir)/regclass.o \
	$(gccobjdir)/local-alloc.o \
	$(gccobjdir)/global.o \
	$(gccobjdir)/reload.o \
	$(gccobjdir)/reload1.o \
	$(gccobjdir)/caller-save.o \
	$(gccobjdir)/insn-peep.o \
	$(gccobjdir)/reorg.o \
	$(gccobjdir)/sched.o \
	$(gccobjdir)/final.o \
	$(gccobjdir)/recog.o \
	$(gccobjdir)/reg-stack.o \
	$(gccobjdir)/insn-opinit.o \
	$(gccobjdir)/insn-recog.o \
	$(gccobjdir)/insn-extract.o \
	$(gccobjdir)/insn-output.o \
	$(gccobjdir)/insn-emit.o \
	$(gccobjdir)/profile.o \
	$(gccobjdir)/insn-attrtab.o \
	$(gccobjdir)/aux-output.o \
	$(gccobjdir)/getpwd.o \
	$(gccobjdir)/lcm.o \
	$(gccobjdir)/gcse.o \
	$(gccobjdir)/sbitmap.o \
	$(gccobjdir)/varray.o \
	$(gccobjdir)/graph.o \
	$(gccobjdir)/hash.o

OBJS1 = $(gccobjdir)/convert.o \
	$(gccobjdir)/expr.o \
	$(gccobjdir)/fold-const.o \
	$(gccobjdir)/function.o \
	$(gccobjdir)/dbxout.o \
	$(gccobjdir)/optabs.o \
	$(gccobjdir)/stor-layout.o \
	$(gccobjdir)/toplev.o \
	$(gccobjdir)/tree.o \
	$(gccobjdir)/version.o

OBJC_OBJS = $(gccobjdir)/objc/objc-parse.o \
	$(gccobjdir)/objc/objc-act.o \
	$(gccobjdir)/c-lex.o \
	$(gccobjdir)/c-pragma.o \
	$(gccobjdir)/c-decl.o \
	$(gccobjdir)/c-typeck.o \
	$(gccobjdir)/c-aux-info.o \
	$(gccobjdir)/c-convert.o \
	$(gccobjdir)/c-common.o \
	$(gccobjdir)/c-iterate.o


# The real targets
$(gcc_bin_dir)/cc1obj:	$(OBJS) $(OBJS1) $(OBJC_OBJS) $(RISCOS_OBJS) $(LIBDEPS)
	$(LINK) $(OBJS) $(OBJS1) $(OBJC_OBJS) $(RISCOS_OBJS) $(LIBS)

##############################################################################

objc-parse.c: objc-parse.y
	bison objc-parse.y -o objc-parse.c

##############################################################################


# Dependencies
$(gccobjdir)/objc/objc-act.c: objc-act.c
$(gccobjdir)/objc/objc-parse.c: objc-parse.c objc-parse.y
