# Makefile for the GNU Compiler Collection for ARM/RISC OS
# Copyright (c) 1997-2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for the host compiler
# HOST_CC is a version of CC where optimisations are not turned off
HOST_CC = gcc -O2 $(OPTS)
HOST_INCLUDES = -I./ -I./include -I./config
HOST_PREDEFS = -DIN_GCC -DHAVE_CONFIG_H

# Defs for GCC
CC = gcc $(CFLAGS)
CFLAGS = -O2 $(OPTS) -DIN_GCC -DHAVE_CONFIG_H $(TARGET_DEFS)
GCC_AS = gcc -xassembler-with-cpp -c

ifeq (${crosscompile},no)
LINK = gcc -Wl,-rescan -o $@
else
LINK = gcc -o $@
endif

#INCLUDES = -I@ -Iinclude -Idef -Iconfig -Icp -If -If/j -If/def -Ip -Ijava
INCLUDES = -I./ -I../../include -I./config -I./cp -I./f77 -I./gpc -I./java
LIBS = -L$(objdir)/libiberty -liberty
BISON = $(BIN)/bison
BISONFLAGS =
CC = gcc $(CFLAGS)

gccobjdir = $(objdir)/gcc-$(VERSION)/gcc

$(gccobjdir)/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES)


all:	$(BIN)/gcc $(TBIN)/cpp $(TBIN)/cc1 $(TBIN)/cc1plus \
	$(BIN)/g++ $(BIN)/gcov $(BIN)/fini $(BIN)/g77 \
	$(TBIN)/f771

# Pascal: $(TBIN)/gpc1
# Java:  $(TBIN)/gcjh $(TBIN)/jvgenmain $(TBIN)/jcf-dump $(TBIN)/jv-scan $(TBIN)/jc1

################################################################################

LIBDEPS = $(gccobjdir)/prefix.o

ifeq (${crosscompile},no)
RISC_OBJS = $(gccobjdir)/config/arm/riscos_fix.o
RISCOS_OBJS = $(RISC_OBJS) $(gccobjdir)/config/arm/riscos.o
else
RISC_OBJS=
RISCOS_OBJS=
endif

# Language-specific object files for C
C_OBJS = $(gccobjdir)/c-parse.o \
	$(gccobjdir)/c-lang.o \
	$(gccobjdir)/c-lex.o \
	$(gccobjdir)/c-pragma.o \
	$(gccobjdir)/c-decl.o \
	$(gccobjdir)/c-typeck.o \
	$(gccobjdir)/c-convert.o \
	$(gccobjdir)/c-aux-info.o \
	$(gccobjdir)/c-common.o \
	$(gccobjdir)/c-iterate.o

# Language-independent object files

OBJS =	$(gccobjdir)/version.o \
	$(gccobjdir)/tree.o \
	$(gccobjdir)/print-tree.o \
	$(gccobjdir)/stor-layout.o \
	$(gccobjdir)/fold-const.o \
	$(gccobjdir)/genrtl.o \
	$(gccobjdir)/function.o \
	$(gccobjdir)/stmt.o \
	$(gccobjdir)/except.o \
	$(gccobjdir)/expr.o \
	$(gccobjdir)/calls.o \
	$(gccobjdir)/expmed.o \
	$(gccobjdir)/explow.o \
	$(gccobjdir)/optabs.o \
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
	$(gccobjdir)/insn/insn-peep.o \
	$(gccobjdir)/reorg.o \
	$(gccobjdir)/sched.o \
	$(gccobjdir)/final.o \
	$(gccobjdir)/recog.o \
	$(gccobjdir)/reg-stack.o \
	$(gccobjdir)/dbxout.o \
	$(gccobjdir)/insn/insn-opinit.o \
	$(gccobjdir)/insn/insn-recog.o \
	$(gccobjdir)/insn/insn-extract.o \
	$(gccobjdir)/insn/insn-output.o \
	$(gccobjdir)/insn/insn-emit.o \
	$(gccobjdir)/profile.o \
	$(gccobjdir)/insn/insn-attrtab.o \
	$(gccobjdir)/aux-output.o \
	$(gccobjdir)/getpwd.o \
	$(gccobjdir)/convert.o \
	$(gccobjdir)/toplev.o \
	$(gccobjdir)/lcm.o \
	$(gccobjdir)/gcse.o \
	$(gccobjdir)/sbitmap.o \
	$(gccobjdir)/varray.o \
	$(gccobjdir)/graph.o \
	$(gccobjdir)/hash.o

# C++ specific files

C++OBJS = $(gccobjdir)/cp/call.o \
	$(gccobjdir)/cp/class.o \
	$(gccobjdir)/cp/cvt.o \
	$(gccobjdir)/cp/decl.o \
	$(gccobjdir)/cp/decl2.o \
	$(gccobjdir)/cp/errfn.o \
	$(gccobjdir)/cp/error.o \
	$(gccobjdir)/cp/except.o \
	$(gccobjdir)/cp/expr.o \
	$(gccobjdir)/cp/friend.o \
	$(gccobjdir)/cp/init.o \
	$(gccobjdir)/cp/lex.o \
	$(gccobjdir)/cp/method.o \
	$(gccobjdir)/cp/parse.o \
	$(gccobjdir)/cp/pt.o \
	$(gccobjdir)/cp/ptree.o \
	$(gccobjdir)/cp/repo.o \
	$(gccobjdir)/cp/rtti.o \
	$(gccobjdir)/cp/search.o \
	$(gccobjdir)/cp/semantics.o \
	$(gccobjdir)/cp/sig.o \
	$(gccobjdir)/cp/spew.o \
	$(gccobjdir)/cp/tree.o \
	$(gccobjdir)/cp/typeck.o \
	$(gccobjdir)/cp/typeck2.o \
	$(gccobjdir)/cp/xref.o

# The real targets
$(TBIN)/cc1plus: $(C++OBJS) $(OBJS) $(gccobjdir)/c-common.o \
	$(gccobjdir)/c-pragma.o $(RISCOS_OBJS) $(LIBDEPS)
	$(LINK) $(C++OBJS) $(OBJS) $(gccobjdir)/c-common.o \
	$(gccobjdir)/c-pragma.o $(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

#$(BIN)/demangle: $(LIBIBERTY)
#	$(LINK) $(LIBS)

$(BIN)/g++: $(gccobjdir)/cp/g++.o $(gccobjdir)/cp/g++spec.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/config/arm/rname.o $(RISC_OBJS) $(LIBDEPS)
	$(LINK) $(gccobjdir)/cp/g++.o $(gccobjdir)/g++spec.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/config/arm/rname.o $(RISC_OBJS) $(LIBDEPS) $(LIBS)

# Bison sources
parse.c: parse.y
	bison -d parse.y -o parse.c
	grep "^#define[ 	]*YYEMPTY" cp/parse.c >>cp/parse.h

# Perfect hash function generation stuff
hash.h: gxx.gperf
	gperf -p -j1 -g -o -t -N is_reserved_word -k1,4,$$,7 <gxx.gperf >hash.h

# Bison object dependencies
$(gccobjdir)/cp/parse.o:	parse.c parse.y

$(gccobjdir)/cp/call.o: cp/call.c
$(gccobjdir)/cp/class.o: cp/class.c
$(gccobjdir)/cp/cvt.o: cp/cvt.c
$(gccobjdir)/cp/decl.o: cp/decl.c
$(gccobjdir)/cp/decl2.o: cp/decl2.c
$(gccobjdir)/cp/errfn.o: cp/errfn.c
$(gccobjdir)/cp/error.o: cp/error.c
$(gccobjdir)/cp/except.o: cp/except.c
$(gccobjdir)/cp/expr.o: cp/expr.c
$(gccobjdir)/cp/friend.o: cp/friend.c
$(gccobjdir)/cp/g++spec.o: cp/g++spec.c
$(gccobjdir)/cp/init.o: cp/init.c
$(gccobjdir)/cp/input.o: cp/input.c
$(gccobjdir)/cp/lex.o: cp/lex.c
$(gccobjdir)/cp/method.o: cp/method.c
$(gccobjdir)/cp/parse.o: cp/parse.c
$(gccobjdir)/cp/pt.o: cp/pt.c
$(gccobjdir)/cp/ptree.o: cp/ptree.c
$(gccobjdir)/cp/repo.o: cp/repo.c
$(gccobjdir)/cp/rtti.o: cp/rtti.c
$(gccobjdir)/cp/search.o: cp/search.c
$(gccobjdir)/cp/semantics.o: cp/semantics.c
$(gccobjdir)/cp/sig.o: cp/sig.c
$(gccobjdir)/cp/spew.o: cp/spew.c
$(gccobjdir)/cp/tree.o: cp/tree.c
$(gccobjdir)/cp/typeck.o: cp/typeck.c
$(gccobjdir)/cp/typeck2.o: cp/typeck2.c
$(gccobjdir)/cp/xref.o: cp/xref.c 
