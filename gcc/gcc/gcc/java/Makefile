# Makefile for the GNU Java compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
GCC_AS = $(CC) -xassembler-with-cpp -c
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty

$(gccobjdir)/java/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: java

.PHONY: java

java: $(gcc_bin_dir)/gcjh $(gcc_bin_dir)/jvgenmain \
	$(gcc_bin_dir)/jcf-dump $(gcc_bin_dir)/jv-scan $(gcc_bin_dir)/jc1

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

# Language specific object files for GCC/Java
JAVA_OBJS = $(gccobjdir)/java/parse.o \
	$(gccobjdir)/java/class.o \
	$(gccobjdir)/java/decl.o \
	$(gccobjdir)/java/expr.o \
	$(gccobjdir)/java/constants.o \
	$(gccobjdir)/java/lang.o \
	$(gccobjdir)/java/typeck.o \
	$(gccobjdir)/java/except.o \
	$(gccobjdir)/java/verify.o \
	$(gccobjdir)/java/zextract.o \
	$(gccobjdir)/java/jcf-io.o \
	$(gccobjdir)/java/jcf-parse.o \
	$(gccobjdir)/java/mangle.o \
	$(gccobjdir)/java/jcf-write.o \
	$(gccobjdir)/java/buffer.o \
	$(gccobjdir)/java/check-init.o \
	$(gccobjdir)/java/jcf-depend.o \
	$(gccobjdir)/java/jcf-path.o \
	$(gccobjdir)/java/xref.o

JAVA_OBJS_LITE = $(gccobjdir)/java/parse-scan.o \
	$(gccobjdir)/java/jv-scan.o

JCF_DUMP_OBJS = $(gccobjdir)/java/jcf-dump.o \
	$(gccobjdir)/java/jcf-io.o \
	$(gccobjdir)/java/jcf-depend.o \
	$(gccobjdir)/java/jcf-path.o \
	$(gccobjdir)/java/zextract.o

JVGENMAIN_OBJS = $(gccobjdir)/java/jvgenmain.o \
	$(gccobjdir)/java/mangle.o

GCJH_OBJS = $(gccobjdir)/java/gjavah.o \
	$(gccobjdir)/java/jcf-io.o \
	$(gccobjdir)/java/jcf-depend.o \
	$(gccobjdir)/java/jcf-path.o \
	$(gccobjdir)/java/zextract.o


# The real targets
$(gcc_bin_dir)/jc1: $(JAVA_OBJS) $(OBJS) $(OBJS1) $(RISCOS_OBJS)
	$(LINK) $(JAVA_OBJS) $(OBJS) $(OBJS1) $(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/jv-scan: $(JAVA_OBJS_LITE)
	$(LINK) $(JAVA_OBJS_LITE) $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/jcf-dump: $(JCF_DUMP_OBJS)
	$(LINK) $(JCF_DUMP_OBJS) $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/jvgenmain: $(JVGENMAIN_OBJS)
	$(LINK) $(JVGENMAIN_OBJS) $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/gcjh: $(GCJH_OBJS)
	$(LINK) $(GCJH_OBJS) $(LIBDEPS) $(LIBS)

##############################################################################

$(gccobjdir)/java/jcf-path.o: jcf-path.c
	$(CC) $(CFLAGS) $(INCLUDES) -O2 -o $@ -c jcf-path.c -DLIBGCJ_ZIP_FILE=\"libgcj.zip\"


# Dependencies
$(gccobjdir)/java/buffer.o: buffer.c
$(gccobjdir)/java/check-init.o: check-init.c
$(gccobjdir)/java/class.o: class.c
$(gccobjdir)/java/constants.o: constants.c
$(gccobjdir)/java/decl.o: decl.c
$(gccobjdir)/java/except.o: except.c
$(gccobjdir)/java/expr.o: expr.c
$(gccobjdir)/java/gjavah.o: gjavah.c
$(gccobjdir)/java/jcf-depend.o: jcf-depend.c
$(gccobjdir)/java/jcf-dump.o: jcf-dump.c
$(gccobjdir)/java/jcf-io.o: jcf-io.c
$(gccobjdir)/java/jcf-parse.o: jcf-parse.c
$(gccobjdir)/java/jcf-path.o: jcf-path.c
$(gccobjdir)/java/jcf-reader.o: jcf-reader.c
$(gccobjdir)/java/jcf-write.o: jcf-write.c
$(gccobjdir)/java/jv-scan.o: jv-scan.c
$(gccobjdir)/java/jvgenmain.o: jvgenmain.c
$(gccobjdir)/java/jvspec.o: jvspec.c
$(gccobjdir)/java/lang.o: lang.c
$(gccobjdir)/java/lex.o: lex.c
$(gccobjdir)/java/mangle.o: mangle.c
$(gccobjdir)/java/parse.o: parse.c
$(gccobjdir)/java/parse-scan.o: parse-scan.c
$(gccobjdir)/java/typeck.o: typeck.c
$(gccobjdir)/java/verify.o: verify.c
$(gccobjdir)/java/xref.o: xref.c
$(gccobjdir)/java/zextract.o: zextract.c 
