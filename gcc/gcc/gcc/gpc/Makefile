# Makefile for the GNU Pascal compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
CFLAGS += -DGPC -DEGCS92 -DEGCS -DEGCS95 -MMD
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty

$(gccobjdir)/gpc/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: gpc docs

.PHONY: gpc
.PHONY: docs

gpc:	$(bin_dir)/gpc $(gcc_bin_dir)/gpc1 $(gcc_bin_dir)/gpcpp

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


$(bin_dir)/gpc: $(gccobjdir)/gpc/gpc.o $(gccobjdir)/gpc/gpcspec.o \
	$(gccobjdir)/gpc/version.o $(RISC_OBJS) \
	$(gccobjdir)/config/arm/rname.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/gpc/gpc.o $(gccobjdir)/gpc/gpcspec.o \
	$(gccobjdir)/gpc/version.o $(RISC_OBJS) \
	$(gccobjdir)/config/arm/rname.o $(LIBDEPS) $(LIBS)

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

$(gccobjdir)/gpc/gpc.o: gpc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c gpc.c \
	-DDEFAULT_TARGET_VERSION=\"$(VERSION)\" \
	-DDEFAULT_TARGET_MACHINE=\"$(TARGET_NAME)\" \
	-DSTANDARD_STARTFILE_PREFIX=\"$(prefix)$(gn_standard_startfile_prefix)\" \
	-DSTANDARD_EXEC_PREFIX=\"$(prefix)$(gn_standard_exec_prefix)\"

$(gccobjdir)/gpc/cexp.o: ../cexp.c ../config.h ../auto-host.h ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h ../intl.h
$(gccobjdir)/gpc/circle-buf.o: circle-buf.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h ../../../include/obstack.h \
 circle-buf.h
$(gccobjdir)/gpc/convert.o: ../convert.c ../config.h ../auto-host.h ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../tree.h \
 ../tree.def ../tree-check.h ../real.h ../flags.h ../convert.h \
 ../toplev.h
$(gccobjdir)/gpc/dbxout.o: ../dbxout.c ../config.h ../auto-host.h ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h ../tree.h ../tree.def ../tree-check.h \
 ../real.h ../rtl.h ../rtl.def ../genrtl.h ../flags.h ../regs.h \
 ../varray.h ../insn-config.h ../reload.h ../defaults.h ../output.h \
 ../dbxout.h ../toplev.h
$(gccobjdir)/gpc/expr.o: ../expr.c ../config.h ../auto-host.h ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h ../rtl.h ../rtl.def ../genrtl.h \
 ../tree.h ../tree.def ../tree-check.h ../real.h \
 ../../../include/obstack.h ../flags.h ../regs.h ../varray.h \
 ../hard-reg-set.h ../except.h ../function.h ../insn-flags.h \
 ../insn-codes.h ../insn-config.h ../expr.h ../recog.h ../output.h \
 ../typeclass.h ../defaults.h ../toplev.h
$(gccobjdir)/gpc/fold-const.o: ../fold-const.c ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h ../flags.h ../tree.h \
 ../tree.def ../tree-check.h ../real.h ../rtl.h ../rtl.def ../genrtl.h \
 ../toplev.h
$(gccobjdir)/gpc/function.o: ../function.c ../config.h ../auto-host.h ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h ../rtl.h ../rtl.def ../genrtl.h \
 ../tree.h ../tree.def ../tree-check.h ../real.h ../flags.h \
 ../except.h ../function.h ../insn-flags.h ../expr.h ../insn-codes.h \
 ../regs.h ../varray.h ../hard-reg-set.h ../insn-config.h ../recog.h \
 ../output.h ../basic-block.h ../bitmap.h ../sbitmap.h \
 ../../../include/obstack.h ../toplev.h ../hash.h
$(gccobjdir)/gpc/gpc-common.o: gpc-common.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h ../c-lex.h types.h gpc-defs.h lang.h \
 parse.h rts.h util.h
$(gccobjdir)/gpc/gpc-convert.o: gpc-convert.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h ../toplev.h gbe.h ../tree.h \
 ../tree.def ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h \
 ../rtl.def ../genrtl.h ../../../include/obstack.h ../flags.h \
 ../output.h ../expr.h ../insn-codes.h gpc-defs.h lang.h types.h \
 util.h ../convert.h
$(gccobjdir)/gpc/gpc-decl.o: gpc-decl.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h ../c-lex.h ../except.h ../function.h \
 /usr/lib/gcc-lib/i386-redhat-linux/egcs-2.91.66/include/float.h \
 gpc-defs.h lang.h module.h memfiles.h objects.h types.h util.h \
 gpc-options.h
$(gccobjdir)/gpc/gpc-lex.o: gpc-lex.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h ../c-lex.h gpc-defs.h lang.h parse.h \
 module.h memfiles.h util.h types.h gpc-options.h ../dwarfout.h hash.h \
 lang-options.h
$(gccobjdir)/gpc/gpc-typeck.o: gpc-typeck.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h ../toplev.h gbe.h ../tree.h \
 ../tree.def ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h \
 ../rtl.def ../genrtl.h ../../../include/obstack.h ../flags.h \
 ../output.h ../expr.h ../insn-codes.h gpc-defs.h lang.h module.h \
 memfiles.h rts.h types.h objects.h util.h
$(gccobjdir)/gpc/gpc.o: gpc.c gcc-version.h version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h ../../../include/obstack.h \
 ../multilib.h ../specs.h ../cp/lang-specs.h ../objc/lang-specs.h \
 ../f77/lang-specs.h ../java/lang-specs.h ../gpc/lang-specs.h \
 ../gpc/version.h
$(gccobjdir)/gpc/gpcpp.o: gpcpp.c ../config.h ../auto-host.h ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../pcp.h \
 gpc-options.h lang-options.h gcc-version.h
$(gccobjdir)/gpc/gpcspec.o: gpcspec.c ../config.h ../auto-host.h ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h version.h
$(gccobjdir)/gpc/gpi-hash.o: gpi-hash.c gcc-version.h ../config.h \
 ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h gpc-defs.h lang.h util.h module.h \
 memfiles.h circle-buf.h gpi-hash.h nodes.def
$(gccobjdir)/gpc/lang.o: lang.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h gbe.h ../tree.h \
 ../tree.def ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h \
 ../rtl.def ../genrtl.h ../../../include/obstack.h ../flags.h \
 ../output.h ../expr.h ../insn-codes.h util.h gpc-defs.h lang.h
$(gccobjdir)/gpc/memfiles.o: memfiles.c ../config.h ../auto-host.h \
 ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h gbe.h ../tree.h \
 ../tree.def ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h \
 ../rtl.def ../genrtl.h ../../../include/obstack.h ../flags.h \
 ../output.h ../expr.h ../insn-codes.h ../system.h \
 ../../../include/libiberty.h memfiles.h
$(gccobjdir)/gpc/module.o: module.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h gpc-defs.h lang.h objects.h util.h module.h \
 memfiles.h gpi-hash.h nodes.def
$(gccobjdir)/gpc/objects.o: objects.c gcc-version.h ../config.h \
 ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h gpc-defs.h lang.h module.h memfiles.h \
 types.h util.h rts/types.h objects.h
$(gccobjdir)/gpc/optabs.o: ../optabs.c ../config.h ../auto-host.h \
 ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h ../toplev.h ../insn-config.h ../rtl.h \
 ../rtl.def ../genrtl.h ../tree.h ../tree.def ../tree-check.h \
 ../real.h ../flags.h ../insn-flags.h ../insn-codes.h ../expr.h \
 ../recog.h ../reload.h
$(gccobjdir)/gpc/parse.o: parse.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h gpc-defs.h lang.h module.h memfiles.h \
 objects.h rts.h types.h util.h
$(gccobjdir)/gpc/rts.o: rts.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h gpc-defs.h lang.h module.h memfiles.h \
 parse.h types.h util.h rts/types.h rts.h
$(gccobjdir)/gpc/setop.o: setop.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h ../insn-flags.h setop.h
$(gccobjdir)/gpc/stor-layout.o: ../stor-layout.c ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h ../rtl.h ../rtl.def ../genrtl.h ../flags.h \
 ../except.h ../function.h ../expr.h ../insn-codes.h ../toplev.h
$(gccobjdir)/gpc/toplev.o: ../toplev.c ../config.h ../auto-host.h \
 ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h ../input.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h ../rtl.h ../rtl.def ../genrtl.h ../flags.h \
 ../insn-attr.h ../insn-codes.h ../insn-config.h ../recog.h \
 ../defaults.h ../output.h ../except.h ../toplev.h ../expr.h \
 ../basic-block.h ../bitmap.h ../sbitmap.h ../varray.h ../intl.h \
 ../options.h ../cp/lang-options.h ../f77/lang-options.h \
 ../java/lang-options.h ../gpc/lang-options.h ../gpc/gcc-version.h
$(gccobjdir)/gpc/tree.o: ../tree.c ../config.h ../auto-host.h ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h ../flags.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h ../except.h ../function.h \
 ../../../include/obstack.h ../toplev.h
$(gccobjdir)/gpc/types.o: types.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h ../../../include/ansidecl.h ../config/i386/xm-linux.h \
 ../system.h ../../../include/libiberty.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h gpc-defs.h lang.h parse.h objects.h rts.h \
 util.h types.h
$(gccobjdir)/gpc/util.o: util.c gcc-version.h ../config.h ../auto-host.h \
 ../gansidecl.h \
 ../../../include/ansidecl.h ../config/i386/xm-linux.h ../system.h \
 ../../../include/libiberty.h ../toplev.h gbe.h ../tree.h ../tree.def \
 ../tree-check.h ../real.h gpc-tree.h ../input.h ../rtl.h ../rtl.def \
 ../genrtl.h ../../../include/obstack.h ../flags.h ../output.h \
 ../expr.h ../insn-codes.h gpc-defs.h lang.h module.h memfiles.h \
 parse.h rts.h ../except.h ../function.h types.h rts/types.h util.h
$(gccobjdir)/gpc/version.o: version.c ../version.c
