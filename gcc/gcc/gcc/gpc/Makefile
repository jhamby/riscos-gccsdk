# Makefile for the GNU Pascal compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
CFLAGS += -DGPC -DEGCS92 -DEGCS -DEGCS95
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty

$(gccobjdir)/gpc/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: gpc docs

.PHONY: gpc
.PHONY: docs

gpc:	$(gcc_bin_dir)/gpc1 $(gcc_bin_dir)/gpcpp

ifeq (${STAGE},stage1)
docs:
else
docs:	$(info_dir)/gpc
endif

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

# Language specific object files for GCC/Pascal
GPC_OBJS = $(gccobjdir)/c-aux-info.o \
	$(gccobjdir)/c-iterate.o \
	$(gccobjdir)/c-pragma.o \
	$(gccobjdir)/gpc/gpc-common.o \
	$(gccobjdir)/gpc/gpc-convert.o \
	$(gccobjdir)/gpc/gpc-decl.o \
	$(gccobjdir)/gpc/lang.o \
	$(gccobjdir)/gpc/gpc-lex.o \
	$(gccobjdir)/gpc/gpc-typeck.o \
	$(gccobjdir)/gpc/module.o \
	$(gccobjdir)/gpc/gpi-hash.o \
	$(gccobjdir)/gpc/circle-buf.o \
	$(gccobjdir)/gpc/memfiles.o \
	$(gccobjdir)/gpc/objects.o \
	$(gccobjdir)/gpc/parse.o \
	$(gccobjdir)/gpc/rts.o \
	$(gccobjdir)/gpc/types.o \
	$(gccobjdir)/gpc/util.o \
	$(gccobjdir)/gpc/setop.o \
	$(gccobjdir)/gpc/version.o \
	$(gccobjdir)/gpc/convert.o \
	$(gccobjdir)/gpc/expr.o \
	$(gccobjdir)/gpc/fold-const.o \
	$(gccobjdir)/gpc/function.o \
	$(gccobjdir)/gpc/dbxout.o \
	$(gccobjdir)/gpc/optabs.o \
	$(gccobjdir)/gpc/stor-layout.o \
	$(gccobjdir)/gpc/toplev.o \
	$(gccobjdir)/gpc/tree.o


$(gcc_bin_dir)/gpc1:	$(GPC_OBJS) $(OBJS) $(RISCOS_OBJS)
	$(LINK) $(GPC_OBJS) $(OBJS) $(RISCOS_OBJS) $(LIBS)

$(gcc_bin_dir)/gpcpp:	$(gccobjdir)/gpc/gpcpp.o $(gccobjdir)/gpc/cexp.o \
	$(gccobjdir)/mbchar.o $(gccobjdir)/version.o
	$(LINK) $(gccobjdir)/gpc/gpcpp.o $(gccobjdir)/gpc/cexp.o \
	$(gccobjdir)/mbchar.o $(gccobjdir)/version.o $(LIBDEPS) $(LIBS)


ifeq (${STAGE},stage2)
$(info_dir)/gpc:
	makeinfo --no-split --number-sections -I ./doc doc/gpc.texi -o $@
endif

##############################################################################

parse.c parse.h: parse.y
	bison -d parse.y -o parse.c

hash.h: gpc.gperf
	gperf -p -j1 -G -o -t -N is_reserved_word -k1,2,$$ <gpc.gperf > $@

##############################################################################

$(gccobjdir)/gpc/gpcpp.o: gpcpp.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) \
	-DGCC_INCLUDE_DIR=\"$(prefix)$(gn_gcc_include_dir)\" \
	-DGPLUSPLUS_INCLUDE_DIR=\"$(prefix)$(gn_gplusplus_include_dir)\" \
	-DLOCAL_INCLUDE_DIR=\"$(prefix)$(gn_local_include_dir)\" \
	-DSYSTEM_INCLUDE_DIR=\"$(prefix)$(gn_system_include_dir)\" \
	-DCROSS_INCLUDE_DIR=\"$(prefix)$(gn_cross_include_dir)\"

$(gccobjdir)/gpc/circle-buf.o: circle-buf.c
$(gccobjdir)/gpc/gpc.o: gpc.c
$(gccobjdir)/gpc/gpc-common.o: gpc-common.c parse.c
$(gccobjdir)/gpc/gpc-convert.o: gpc-convert.c
$(gccobjdir)/gpc/gpc-decl.o: gpc-decl.c
$(gccobjdir)/gpc/lang.o: lang.c
$(gccobjdir)/gpc/gpc-lex.o: gpc-lex.c hash.h parse.h
$(gccobjdir)/gpc/gpc-typeck.o: gpc-typeck.c
$(gccobjdir)/gpc/gpi-hash.o: gpi-hash.c
$(gccobjdir)/gpc/memfiles.o: memfiles.c
$(gccobjdir)/gpc/module.o: module.c
$(gccobjdir)/gpc/objects.o: objects.c
$(gccobjdir)/gpc/parse.o: parse.c
$(gccobjdir)/gpc/rts.o: rts.c
$(gccobjdir)/gpc/setop.o: setop.c
$(gccobjdir)/gpc/types.o: types.c
$(gccobjdir)/gpc/util.o: util.c
$(gccobjdir)/gpc/version.o: version.c

$(gccobjdir)/gpc/cexp.o: ../cexp.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/gpc/convert.o: ../convert.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/gpc/expr.o: ../expr.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/gpc/fold-const.o: ../fold-const.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/gpc/function.o: ../function.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/gpc/dbxout.o: ../dbxout.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/gpc/optabs.o: ../optabs.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/gpc/stor-layout.o: ../stor-layout.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/gpc/toplev.o: ../toplev.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) -DTARGET_NAME=\"$(TARGET_NAME)\"

$(gccobjdir)/gpc/tree.o: ../tree.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

