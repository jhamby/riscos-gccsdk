# Makefile for the GNU Compiler Collection for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for the host compiler
# HOST_CC is a version of CC where optimisations are not turned off
HOST_CC = gcc -O2 $(OPTS) -D$(hostsystem)
HOST_INCLUDES = -I./ -I../../include -I./config
HOST_PREDEFS = -DIN_GCC -DHAVE_CONFIG_H

# Defs for GCC
CFLAGS += -DIN_GCC -DHAVE_CONFIG_H -D$(hostsystem) $(TARGET_DEFS)
GCC_AS = gcc -xassembler-with-cpp -c

ifeq (${crosscompile},no)
LINK = gcc -Wl,-rescan -o $@
else
LINK = gcc -o $@
endif

INCLUDES = -I./ -I../../include -I./config -I./cp -I./f77 -I./java -I./objc
LIBS = -L$(objdir)/libiberty -liberty

gccobjdir = $(objdir)/gcc-$(VERSION)/gcc

$(gccobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: c cpp fortran77 pascal objc java

.PHONY: c
.PHONY: cpp
.PHONY: fortran
.PHONY: pascal
.PHONY: java
.PHONY: objc

c: $(BIN)/gcc $(TBIN)/cpp $(TBIN)/cc1 $(BIN)/gcov
cpp: $(TBIN)/cc1plus $(BIN)/g++
fortran77: $(gccobjdir)/f77/fini $(BIN)/g77 $(TBIN)/f771
pascal:	$(TBIN)/gpc1 $(TBIN)/gpcpp
java: $(TBIN)/gcjh $(TBIN)/jvgenmain $(TBIN)/jcf-dump $(TBIN)/jv-scan $(TBIN)/jc1
objc: $(TBIN)/cc1obj

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

CPPLIB_OBJS = $(gccobjdir)/cpp/cpperror.o \
	$(gccobjdir)/cpp/cppexp.o \
	$(gccobjdir)/cpp/cpphash.o \
	$(gccobjdir)/cpp/cpplib.o \
	$(gccobjdir)/cpp/cppspec.o \
	$(gccobjdir)/cpp/cppulp.o \
	$(gccobjdir)/cpp/cppinit.o \
	$(gccobjdir)/cpp/cppfiles.o

CPP_OBJS = $(CPPLIB_OBJS) \
	$(gccobjdir)/cpp/cppalloc.o \
	$(gccobjdir)/cpp/cppmain.o

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

# GEN files are listed separately, so they can be built before doing parallel
# makes for cc1 or cc1plus.  Otherwise sequent parallel make attempts to load
# them before rtl.o is compiled.
GEN = genemit gengenrtl genoutput genrecog genextract genflags \
	gencodes genconfig genpeep gencheck

F77_OBJS = $(gccobjdir)/f77/stb.o \
	$(gccobjdir)/f77/target.o \
	$(gccobjdir)/f77/symbol.o \
	$(gccobjdir)/f77/stu.o \
	$(gccobjdir)/f77/stt.o \
	$(gccobjdir)/f77/ste.o \
	$(gccobjdir)/f77/stc.o \
	$(gccobjdir)/f77/intrin.o \
	$(gccobjdir)/f77/implic.o \
	$(gccobjdir)/f77/expr.o \
	$(gccobjdir)/f77/data.o \
	$(gccobjdir)/f77/com.o \
	$(gccobjdir)/f77/bld.o \
	$(gccobjdir)/f77/bad.o \
	$(gccobjdir)/f77/bit.o \
	$(gccobjdir)/f77/equiv.o \
	$(gccobjdir)/f77/global.o \
	$(gccobjdir)/f77/info.o \
	$(gccobjdir)/f77/lab.o \
	$(gccobjdir)/f77/lex.o \
	$(gccobjdir)/f77/malloc.o \
	$(gccobjdir)/f77/name.o \
	$(gccobjdir)/f77/parse.o \
	$(gccobjdir)/f77/proj.o \
	$(gccobjdir)/f77/src.o \
	$(gccobjdir)/f77/st.o \
	$(gccobjdir)/f77/sta.o \
	$(gccobjdir)/f77/std.o \
	$(gccobjdir)/f77/storag.o \
	$(gccobjdir)/f77/stp.o \
	$(gccobjdir)/f77/str.o \
	$(gccobjdir)/f77/sts.o \
	$(gccobjdir)/f77/stv.o \
	$(gccobjdir)/f77/stw.o \
	$(gccobjdir)/f77/top.o \
	$(gccobjdir)/f77/type.o \
	$(gccobjdir)/f77/where.o \
	$(gccobjdir)/f77/version.o

STR_HEADERS = f77/str-1t.h f77/str-2t.h f77/str-fo.h f77/str-io.h \
	f77/str-nq.h f77/str-op.h f77/str-ot.h \
	f77/str-1t.j f77/str-2t.j f77/str-fo.j f77/str-io.j \
	f77/str-nq.j f77/str-op.j f77/str-ot.j


# The real targets

$(BIN)/gcc:	$(gccobjdir)/gcc.o $(gccobjdir)/gccspec.o \
	$(gccobjdir)/version.o $(RISC_OBJS) \
	$(gccobjdir)/config/arm/rname.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/gcc.o $(gccobjdir)/gccspec.o \
	$(gccobjdir)/version.o $(RISC_OBJS) \
	$(gccobjdir)/config/arm/rname.o $(LIBDEPS) $(LIBS)

$(TBIN)/cc1: $(C_OBJS) $(OBJS) $(OBJS1) $(RISCOS_OBJS)
	$(LINK) $(C_OBJS) $(OBJS) $(OBJS1) $(RISCOS_OBJS) $(LIBS)

$(TBIN)/cpp: $(CPP_OBJS) $(gccobjdir)/version.o \
	$(gccobjdir)/config/arm/rname.o \
	$(RISCOS_OBJS) $(LIBDEPS)
	$(LINK) $(CPP_OBJS) $(gccobjdir)/version.o \
	$(gccobjdir)/config/arm/rname.o \
	$(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

$(BIN)/gcov: $(gccobjdir)/gcov.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/gcov.o $(LIBDEPS) $(LIBS)

$(TBIN)/cc1plus: $(C++OBJS) $(OBJS) $(OBJS1) $(gccobjdir)/c-common.o \
	$(gccobjdir)/c-pragma.o $(RISCOS_OBJS) $(LIBDEPS)
	$(LINK) $(C++OBJS) $(OBJS) $(OBJS1) $(gccobjdir)/c-common.o \
	$(gccobjdir)/c-pragma.o $(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

#$(BIN)/demangle: $(LIBIBERTY)
#	$(LINK) $(LIBS)

$(BIN)/g++: $(gccobjdir)/cp/g++.o $(gccobjdir)/cp/g++spec.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/config/arm/rname.o $(RISC_OBJS) $(LIBDEPS)
	$(LINK) $(gccobjdir)/cp/g++.o $(gccobjdir)/cp/g++spec.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/config/arm/rname.o $(RISC_OBJS) $(LIBDEPS) $(LIBS)

$(TBIN)/cc1obj:	$(OBJS) $(OBJS1) $(OBJC_OBJS) $(RISCOS_OBJS) $(LIBDEPS)
	$(LINK) $(OBJS) $(OBJS1) $(OBJC_OBJS) $(RISCOS_OBJS) $(LIBS)

$(BIN)/g77:	$(gccobjdir)/f77/g77.o $(gccobjdir)/f77/g77spec.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/f77/version.o $(gccobjdir)/config/arm/rname.o \
	$(RISC_OBJS) $(LIBDEPS)
	$(LINK) $(gccobjdir)/f77/g77.o $(gccobjdir)/f77/g77spec.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/f77/version.o $(gccobjdir)/config/arm/rname.o \
	$(RISC_OBJS) $(LIBDEPS) $(LIBS)

$(gccobjdir)/f77/fini:	$(gccobjdir)/f77/fini.o $(gccobjdir)/f77/proj.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/f77/fini.o $(gccobjdir)/f77/proj.o \
	$(LIBDEPS) $(LIBS)

$(TBIN)/f771:	$(F77_OBJS) $(OBJS) $(OBJS1) $(BC_OBJS) $(RISCOS_OBJS) $(STR_HEADERS)
	$(LINK) $(F77_OBJS) $(OBJS) $(OBJS1) $(BC_OBJS) $(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

$(TBIN)/gpc1:	$(GPC_OBJS)
	$(LINK) $(GPC_OBJS) $(OBJS) $(LIBS)

$(TBIN)/gpcpp:	$(gccobjdir)/gpc/gpcpp.o $(gccobjdir)/cexp.o \
	$(gccobjdir)/mbchar.o $(gccobjdir)/version.o
	$(LINK) $(gccobjdir)/gpc/gpcpp.o $(gccobjdir)/cexp.o \
	$(gccobjdir)/mbchar.o $(gccobjdir)/version.o $(LIBDEPS) $(LIBS)

$(TBIN)/jc1: $(JAVA_OBJS) $(OBJS) $(OBJS1) $(RISCOS_OBJS)
	$(LINK) $(JAVA_OBJS) $(OBJS) $(OBJS1) $(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

$(TBIN)/jv-scan: $(JAVA_OBJS_LITE)
	$(LINK) $(JAVA_OBJS_LITE) $(LIBDEPS) $(LIBS)


$(TBIN)/jcf-dump: $(JCF_DUMP_OBJS)
	$(LINK) $(JCF_DUMP_OBJS) $(LIBDEPS) $(LIBS)

$(TBIN)/jvgenmain: $(JVGENMAIN_OBJS)
	$(LINK) $(JVGENMAIN_OBJS) $(LIBDEPS) $(LIBS)

$(TBIN)/gcjh: $(GCJH_OBJS)
	$(LINK) $(GCJH_OBJS) $(LIBDEPS) $(LIBS)

##############################################################################


# Make Bison sources

c-parse.y:	c-parse.in
	sed -e "/^ifobjc$/,/^end ifobjc$/d" -e "/^ifc$/d" -e "/^end ifc$/d" $? > c-parse.y

# Bison sources
# There's no point having a suffix thing for these becuase
# they're all built differently.

cexp.c: cexp.y
	bison -o cexp.c cexp.y

c-parse.c: c-parse.y
	bison -d $? -o c-parse.c

cp/parse.c: cp/parse.y
	cd cp ; bison -d parse.y -o parse.c ; cd .. ; grep "^#define[ 	]*YYEMPTY" cp/parse.c >>cp/parse.h

gpc/parse.c gpc/parse.h: gpc/parse.y
	cd gpc ; bison -d parse.y -o parse.c ; cd ..

objc/objc-parse.c: objc/objc-parse.y
	cd objc ; bison objc-parse.y -o objc-parse.c ; cd ..

# Perfect hash function generation stuff

#h.c-gperf: gperf/c-parse
#	gperf -p -j1 -i 1 -g -o -t -G -N is_reserved_word -k1,3,$$ <gperf.c-parse >h.c-gperf

cp/hash.h: gperf/gxx
	gperf -p -j1 -g -o -t -N is_reserved_word -k1,4,$$,7 <gxx.gperf >cp/hash.h

gpc/hash.h: gperf/gpc
	gperf -p -j1 -G -o -t -N is_reserved_word -k1,2,$$ <gperf/gpc >gpc/hash.h

# Bison object dependencies
$(gccobjdir)/c-parse.o:	c-parse.c c-parse.c c-parse.in
$(gccobjdir)/cexp.o:	cexp.c cexp.y
$(gccobjdir)/cp/parse.o:	cp/parse.c cp/parse.y

##############################################################################

# Derived files. Created from config/arm/md

MD = config/arm/md
GENOBJS = $(gccobjdir)/gen/obstack.o $(gccobjdir)/gen/rtl.o \
	$(gccobjdir)/bitmap.o

gen-derived:	insn-config.h insn-flags.h insn-codes.h \
	insn-emit.c insn-recog.c insn-extract.c \
	insn-opinit.c insn-output.c insn-peep.c \
        insn-attr.h insn-attrtab.c genrtl.c genrtl.h

gen-prog:	$(gccobjdir)/gen/genemit $(gccobjdir)/gen/genoutput \
	$(gccobjdir)/gen/genrecog $(gccobjdir)/gen/genextract \
	$(gccobjdir)/gen/genflags $(gccobjdir)/gen/gencodes \
	$(gccobjdir)/gen/genconfig $(gccobjdir)/gen/genpeep \
	$(gccobjdir)/gen/gengenrtl $(gccobjdir)/gen/gencheck

# Stuff to make generate files to generate insn-*.[ch] files.

# Our gen versions are subtly different. We include hconfig.h instead
# of config.h.


$(gccobjdir)/gen/obstack.c:	../../libiberty/obstack.c
	sed -e "s/config[.]h/hconfig.h/" ../../libiberty/obstack.c >$@
$(gccobjdir)/gen/obstack.o:  $(gccobjdir)/gen/obstack.c
	$(HOST_CC) -c $(gccobjdir)/gen/obstack.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/rtl.c:	rtl.c
	sed -e "s/config[.]h/hconfig.h/" rtl.c >$@
$(gccobjdir)/gen/rtl.o:	$(gccobjdir)/gen/rtl.c 
	$(HOST_CC) -c $(gccobjdir)/gen/rtl.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/print-rtl.c:	print-rtl.c
	sed -e "s/config[.]h/hconfig.h/" print-rtl.c >$@
$(gccobjdir)/gen/print-rtl.o:	$(gccobjdir)/gen/print-rtl.c
	$(HOST_CC) -c $(gccobjdir)/gen/print-rtl.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/rtlanal.c:	rtlanal.c
	sed -e "s/config[.]h/hconfig.h/" rtlanal.c >$@
$(gccobjdir)/gen/rtlanal.o:	$(gccobjdir)/gen/rtlanal.c
	$(HOST_CC) -c $(gccobjdir)/gen/rtlanal.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)



$(gccobjdir)/gen/genattr.o:	gen/genattr.c
	$(HOST_CC) -c gen/genattr.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genattrtab.o:	gen/genattrtab.c
	$(HOST_CC) -c gen/genattrtab.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/gencheck.o:	gen/gencheck.c tree.def
	$(HOST_CC) -c gen/gencheck.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genemit.o:	gen/genemit.c
	$(HOST_CC) -c gen/genemit.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/gengenrtl.o:	gen/gengenrtl.c
	$(HOST_CC) -c gen/gengenrtl.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genoutput.o:	gen/genoutput.c
	$(HOST_CC) -c gen/genoutput.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genopinit.o:	gen/genopinit.c
	$(HOST_CC) -c gen/genopinit.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genrecog.o:	gen/genrecog.c
	$(HOST_CC) -c gen/genrecog.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genextract.o:	gen/genextract.c
	$(HOST_CC) -c gen/genextract.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genflags.o:	gen/genflags.c
	$(HOST_CC) -c gen/genflags.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/gencodes.o:	gen/gencodes.c
	$(HOST_CC) -c gen/gencodes.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genconfig.o:	gen/genconfig.c
	$(HOST_CC) -c gen/genconfig.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gen/genpeep.o:	gen/genpeep.c
	$(HOST_CC) -c gen/genpeep.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)


$(gccobjdir)/gen/genattr:	$(gccobjdir)/gen/genattr.o $(GENOBJS)
	$(LINK) $(gccobjdir)/gen/genattr.o $(GENOBJS) $(LIBS)
$(gccobjdir)/gen/genattrtab:	$(gccobjdir)/gen/genattrtab.o $(GENOBJS) $(gccobjdir)/gen/rtlanal.o
	$(LINK) $(gccobjdir)/gen/genattrtab.o $(GENOBJS) $(gccobjdir)/gen/rtlanal.o $(LIBS)
$(gccobjdir)/gen/gencheck:	$(gccobjdir)/gen/gencheck.o tree.def
	$(LINK) $(gccobjdir)/gen/gencheck.o $(LIBS)
$(gccobjdir)/gen/genemit:	$(gccobjdir)/gen/genemit.o $(GENOBJS)
	$(LINK) $(gccobjdir)/gen/genemit.o $(GENOBJS) $(LIBS)
$(gccobjdir)/gen/gengenrtl:	$(gccobjdir)/gen/gengenrtl.o $(gccobjdir)/gen/obstack.o
	$(LINK) $(gccobjdir)/gen/gengenrtl.o $(gccobjdir)/gen/obstack.o $(LIBS)
$(gccobjdir)/gen/genoutput:	 $(gccobjdir)/gen/genoutput.o $(GENOBJS)
	$(LINK) $(gccobjdir)/gen/genoutput.o $(GENOBJS) $(LIBS)
$(gccobjdir)/gen/genopinit:	 $(gccobjdir)/gen/genopinit.o $(GENOBJS)
	$(LINK) $(gccobjdir)/gen/genopinit.o $(GENOBJS) $(LIBS)
$(gccobjdir)/gen/genrecog:	  $(gccobjdir)/gen/genrecog.o $(GENOBJS) $(gccobjdir)/gen/print-rtl.o
	$(LINK) $(gccobjdir)/gen/genrecog.o $(GENOBJS) $(gccobjdir)/gen/print-rtl.o $(LIBS)
$(gccobjdir)/gen/genextract:	$(gccobjdir)/gen/genextract.o $(GENOBJS) insn-config.h
	$(LINK) $(gccobjdir)/gen/genextract.o $(GENOBJS) $(LIBS)
$(gccobjdir)/gen/genflags:	  $(gccobjdir)/gen/genflags.o $(GENOBJS)
	$(LINK) $(gccobjdir)/gen/genflags.o $(GENOBJS) $(LIBS)
$(gccobjdir)/gen/gencodes:	  $(gccobjdir)/gen/gencodes.o $(GENOBJS)
	$(LINK) $(gccobjdir)/gen/gencodes.o $(GENOBJS) $(LIBS)
$(gccobjdir)/gen/genconfig:	 $(gccobjdir)/gen/genconfig.o $(GENOBJS) genrtl.h
	$(LINK) $(gccobjdir)/gen/genconfig.o $(GENOBJS) $(LIBS)
$(gccobjdir)/gen/genpeep:	   $(gccobjdir)/gen/genpeep.o $(GENOBJS)
	$(LINK) $(gccobjdir)/gen/genpeep.o $(GENOBJS) $(LIBS)


insn-config.h:	$(MD) $(gccobjdir)/gen/genconfig
	$(gccobjdir)/gen/genconfig $(MD) >$@
insn-flags.h:	$(MD) gen/genflags
	$(gccobjdir)/gen/genflags $(MD) >$@
insn-codes.h:	$(MD) gen/gencodes
	$(gccobjdir)/gen/gencodes $(MD) >$@
insn-attr.h:	$(MD) gen/genattr
	$(gccobjdir)/gen/genattr $(MD) >$@
tree-check.h:	gen/gencheck tree.def
	$(gccobjdir)/gen/gencheck >tree-check.h

insn-emit.c:	$(MD) $(gccobjdir)/gen/genemit
	$(gccobjdir)/gen/genemit $(MD) >$@
insn-recog.c:	$(MD) $(gccobjdir)/gen/genrecog
	$(gccobjdir)/gen/genrecog $(MD) >$@
insn-extract.c:	$(MD) $(gccobjdir)/gen/genextract
	$(gccobjdir)/gen/genextract $(MD) >$@
insn-opinit.c:	$(MD) $(gccobjdir)/gen/genopinit
	$(gccobjdir)/gen/genopinit $(MD) >$@
insn-output.c:	$(MD) $(gccobjdir)/gen/genoutput
	$(gccobjdir)/gen/genoutput $(MD) >$@
insn-peep.c:	$(MD) $(gccobjdir)/gen/genpeep
	$(gccobjdir)/gen/genpeep $(MD) >$@
insn-attrtab.c:	$(MD) $(gccobjdir)/gen/genattrtab
	$(gccobjdir)/gen/genattrtab $(MD) >$@

genrtl.c genrtl.h:	$(gccobjdir)/gen/gengenrtl
	$(gccobjdir)/gen/gengenrtl genrtl.h genrtl.c

#############################################################################

# Fortran 77 make sources

# the j.str-1t.j equivalents are automatically updated with the fini program

f77/str-1t.h f77/str-1t.j:	f77/fin/str-1t $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini f77/fin/str-1t f77/str-1t.j f77/str-1t.h

f77/str-2t.h f77/str-2t.j:	f77/fin/str-2t $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini f77/fin/str-2t f77/str-2t.j f77/str-2t.h

f77/str-fo.h f77/str-fo.j:	f77/fin/str-fo $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini f77/fin/str-fo f77/str-fo.j f77/str-fo.h

f77/str-io.h f77/str-io.j:	f77/fin/str-io $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini f77/fin/str-io f77/str-io.j f77/str-io.h

f77/str-nq.h f77/str-nq.j:	f77/fin/str-nq $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini f77/fin/str-nq f77/str-nq.j f77/str-nq.h

f77/str-op.h f77/str-op.j:	f77/fin/str-op $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini f77/fin/str-op f77/str-op.j f77/str-op.h

f77/str-ot.h f77/str-ot.j:	f77/fin/str-ot $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini f77/fin/str-ot f77/str-ot.j f77/str-ot.h

##############################################################################

$(gccobjdir)/cplus-dem.o: cplus-dem.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c cplus-dem.c -DMAIN

$(gccobjdir)/except.o: except.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c except.c -DDONT_USE_BUILTIN_SETJMP -DJMP_BUF_SIZE=24

$(gccobjdir)/optabs.o: optabs.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c optabs.c -DDONT_USE_BUILTIN_SETJMP -DJMP_BUF_SIZE=24

$(gccobjdir)/prefix.o: prefix.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c prefix.c -DPREFIX=\"rubbish\"

$(gccobjdir)/java/jcf-path.o: java/jcf-path.c
	$(CC) $(CFLAGS) $(INCLUDES) -O2 -o $@ -c java/jcf-path.c -DLIBGCJ_ZIP_FILE=\"libgcj.zip\"


$(gccobjdir)/toplev.o: toplev.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c toplev.c -DTARGET_NAME=\"$(TARGET_NAME)\"


$(gccobjdir)/gcc.o: gcc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c gcc.c \
	-DDEFAULT_TARGET_VERSION=\"$(VERSION)\" \
	-DDEFAULT_TARGET_MACHINE=\"$(TARGET_NAME)\" \
	-DSTANDARD_STARTFILE_PREFIX=\"$(standard_startfile_prefix)\" \
	-DSTANDARD_EXEC_PREFIX=\"$(standard_exec_prefix)\"

$(gccobjdir)/cp/g++.o: gcc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c gcc.c \
	-DDEFAULT_TARGET_VERSION=\"$(VERSION)\" \
	-DDEFAULT_TARGET_MACHINE=\"$(TARGET_NAME)\" -DLANG_SPECIFIC_DRIVER \
	-DSTANDARD_STARTFILE_PREFIX=\"$(standard_startfile_prefix)\" \
	-DSTANDARD_EXEC_PREFIX=\"$(standard_exec_prefix)\"

$(gccobjdir)/f77/g77.o: gcc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c gcc.c \
	-DDEFAULT_TARGET_VERSION=\"$(VERSION)\" \
	-DDEFAULT_TARGET_MACHINE=\"$(TARGET_NAME)\" -DLANG_SPECIFIC_DRIVER \
	-DSTANDARD_STARTFILE_PREFIX=\"$(standard_startfile_prefix)\" \
	-DSTANDARD_EXEC_PREFIX=\"$(standard_exec_prefix)\"

$(gccobjdir)/cpp/cppinit.o: cpp/cppinit.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c cpp/cppinit.c \
	-DGCC_INCLUDE_DIR=\"$(gcc_include_dir)\" \
	-DGPLUSPLUS_INCLUDE_DIR=\"$(gplusplus_include_dir)\" \
	-DLOCAL_INCLUDE_DIR=\"$(local_include_dir)\" \
	-DSYSTEM_INCLUDE_DIR=\"$(system_include_dir)\" \
	-DCROSS_INCLUDE_DIR=\"$(cross_include_dir)\"

$(gccobjdir)/gpc/circle-buf.o: gpc/circle-buf.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/gpc.o: gpc/gpc.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/gpcpp.o: gpc/gpcpp.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95 \
	-DGCC_INCLUDE_DIR=\"$(gcc_include_dir)\" \
	-DGPLUSPLUS_INCLUDE_DIR=\"$(gplusplus_include_dir)\" \
	-DLOCAL_INCLUDE_DIR=\"$(local_include_dir)\" \
	-DSYSTEM_INCLUDE_DIR=\"$(system_include_dir)\" \
	-DCROSS_INCLUDE_DIR=\"$(cross_include_dir)\"

$(gccobjdir)/gpc/gpc-common.o: gpc/gpc-common.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/gpc-convert.o: gpc/gpc-convert.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/gpc-decl.o: gpc/gpc-decl.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/lang.o: gpc/lang.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/gpc-lex.o: gpc/gpc-lex.c gpc/hash.h gpc/parse.h
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/gpc-typeck.o: gpc/gpc-typeck.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/gpi-hash.o: gpc/gpi-hash.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/memfiles.o: gpc/memfiles.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/module.o: gpc/module.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/objects.o: gpc/objects.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/parse.o: gpc/parse.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/rts.o: gpc/rts.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/setop.o: gpc/setop.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/types.o: gpc/types.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/util.o: gpc/util.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/version.o: gpc/version.c 
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/convert.o: convert.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/expr.o: expr.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/fold-const.o: fold-const.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/function.o: function.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/dbxout.o: dbxout.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/optabs.o: optabs.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/stor-layout.o: stor-layout.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95

$(gccobjdir)/gpc/toplev.o: toplev.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95 -DTARGET_NAME=\"$(TARGET_NAME)\"

$(gccobjdir)/gpc/tree.o: tree.c
	$(CC) $(CFLAGS) -o $@ -c $< -I./gpc $(INCLUDES) -DGPC -DEGCS92 -DEGCS -DEGCS95


# Dependencies
$(gccobjdir)/alias.o: alias.c
$(gccobjdir)/aux-output.o: aux-output.c
$(gccobjdir)/bitmap.o: bitmap.c
$(gccobjdir)/c-aux-info.o: c-aux-info.c
$(gccobjdir)/c-common.o: c-common.c
$(gccobjdir)/c-convert.o: c-convert.c
$(gccobjdir)/c-decl.o: c-decl.c
$(gccobjdir)/c-iterate.o: c-iterate.c
$(gccobjdir)/c-lang.o: c-lang.c
$(gccobjdir)/c-lex.o: c-lex.c rtl.h genrtl.h
$(gccobjdir)/c-parse.o: c-parse.c
$(gccobjdir)/c-pragma.o: c-pragma.c
$(gccobjdir)/c-typeck.o: c-typeck.c
$(gccobjdir)/caller-save.o: caller-save.c
$(gccobjdir)/calls.o: calls.c
$(gccobjdir)/combine.o: combine.c
$(gccobjdir)/convert.o: convert.c
$(gccobjdir)/cse.o: cse.c
$(gccobjdir)/dbxout.o: dbxout.c
$(gccobjdir)/dwarf2out.o: dwarf2out.c
$(gccobjdir)/dyn-string.o: dyn-string.c
$(gccobjdir)/emit-rtl.o: emit-rtl.c
$(gccobjdir)/except.o: except.c
$(gccobjdir)/explow.o: explow.c
$(gccobjdir)/expmed.o: expmed.c
$(gccobjdir)/expr.o: expr.c
$(gccobjdir)/final.o: final.c
$(gccobjdir)/flow.o: flow.c
$(gccobjdir)/fold-const.o: fold-const.c
$(gccobjdir)/function.o: function.c
$(gccobjdir)/gcc.o: gcc.c
$(gccobjdir)/gccspec.o: gccspec.c
$(gccobjdir)/gcov.o: gcov.c
$(gccobjdir)/gcse.o: gcse.c
$(gccobjdir)/genrtl.o: genrtl.c
$(gccobjdir)/getpwd.o: getpwd.c
$(gccobjdir)/global.o: global.c
$(gccobjdir)/graph.o: graph.c
$(gccobjdir)/haifa-sched.o: haifa-sched.c
$(gccobjdir)/hash.o: hash.c
$(gccobjdir)/integrate.o: integrate.c
$(gccobjdir)/jump.o: jump.c
$(gccobjdir)/lcm.o: lcm.c
$(gccobjdir)/libgcc2.o: libgcc2.c
$(gccobjdir)/local-alloc.o: local-alloc.c
$(gccobjdir)/loop.o: loop.c
$(gccobjdir)/mbchar.o: mbchar.c
$(gccobjdir)/optabs.o: optabs.c
$(gccobjdir)/prefix.o: prefix.c
$(gccobjdir)/print-rtl.o: print-rtl.c
$(gccobjdir)/print-tree.o: print-tree.c
$(gccobjdir)/profile.o: profile.c
$(gccobjdir)/real.o: real.c
$(gccobjdir)/recog.o: recog.c
$(gccobjdir)/reg-stack.o: reg-stack.c
$(gccobjdir)/regclass.o: regclass.c
$(gccobjdir)/regmove.o: regmove.c
$(gccobjdir)/reload.o: reload.c
$(gccobjdir)/reload1.o: reload1.c
$(gccobjdir)/reorg.o: reorg.c
$(gccobjdir)/resource.o: resource.c
$(gccobjdir)/rtl.o: rtl.c
$(gccobjdir)/rtlanal.o: rtlanal.c
$(gccobjdir)/sbitmap.o: sbitmap.c
$(gccobjdir)/sched.o: sched.c
$(gccobjdir)/stmt.o: stmt.c insn-config.h
$(gccobjdir)/stor-layout.o: stor-layout.c
$(gccobjdir)/stupid.o: stupid.c
$(gccobjdir)/tlink.o: tlink.c
$(gccobjdir)/toplev.o: toplev.c
$(gccobjdir)/tree.o: tree.c
$(gccobjdir)/unroll.o: unroll.c
$(gccobjdir)/varasm.o: varasm.c
$(gccobjdir)/varray.o: varray.c
$(gccobjdir)/version.o: version.c

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

$(gccobjdir)/objc/objc-act.c: objc/objc-act.c
$(gccobjdir)/objc/objc-parse.c: objc/objc-parse.c objc/objc-parse.y

$(gccobjdir)/f77/ansify.o: f77/ansify.c
$(gccobjdir)/f77/bad.o: f77/bad.c
$(gccobjdir)/f77/bit.o: f77/bit.c
$(gccobjdir)/f77/bld.o: f77/bld.c
$(gccobjdir)/f77/com.o: f77/com.c
$(gccobjdir)/f77/data.o: f77/data.c
$(gccobjdir)/f77/equiv.o: f77/equiv.c
$(gccobjdir)/f77/expr.o: f77/expr.c
$(gccobjdir)/f77/fini.o: f77/fini.c
$(gccobjdir)/f77/g77spec.o: f77/g77spec.c
$(gccobjdir)/f77/global.o: f77/global.c
$(gccobjdir)/f77/implic.o: f77/implic.c
$(gccobjdir)/f77/info.o: f77/info.c
$(gccobjdir)/f77/intdoc.o: f77/intdoc.c
$(gccobjdir)/f77/intrin.o: f77/intrin.c
$(gccobjdir)/f77/lab.o: f77/lab.c
$(gccobjdir)/f77/lex.o: f77/lex.c
$(gccobjdir)/f77/malloc.o: f77/malloc.c
$(gccobjdir)/f77/name.o: f77/name.c
$(gccobjdir)/f77/parse.o: f77/parse.c
$(gccobjdir)/f77/proj.o: f77/proj.c
$(gccobjdir)/f77/src.o: f77/src.c
$(gccobjdir)/f77/st.o: f77/st.c
$(gccobjdir)/f77/sta.o: f77/sta.c
$(gccobjdir)/f77/stb.o: f77/stb.c
$(gccobjdir)/f77/stc.o: f77/stc.c
$(gccobjdir)/f77/std.o: f77/std.c
$(gccobjdir)/f77/ste.o: f77/ste.c
$(gccobjdir)/f77/storag.o: f77/storag.c
$(gccobjdir)/f77/stp.o: f77/stp.c
$(gccobjdir)/f77/str.o: f77/str.c
$(gccobjdir)/f77/sts.o: f77/sts.c
$(gccobjdir)/f77/stt.o: f77/stt.c
$(gccobjdir)/f77/stu.o: f77/stu.c
$(gccobjdir)/f77/stv.o: f77/stv.c
$(gccobjdir)/f77/stw.o: f77/stw.c
$(gccobjdir)/f77/symbol.o: f77/symbol.c
$(gccobjdir)/f77/target.o: f77/target.c
$(gccobjdir)/f77/top.o: f77/top.c
$(gccobjdir)/f77/type.o: f77/type.c
$(gccobjdir)/f77/version.o: f77/version.c
$(gccobjdir)/f77/where.o: f77/where.c

$(gccobjdir)/java/buffer.o: java/buffer.c
$(gccobjdir)/java/check-init.o: java/check-init.c
$(gccobjdir)/java/class.o: java/class.c
$(gccobjdir)/java/constants.o: java/constants.c
$(gccobjdir)/java/decl.o: java/decl.c
$(gccobjdir)/java/except.o: java/except.c
$(gccobjdir)/java/expr.o: java/expr.c
$(gccobjdir)/java/gjavah.o: java/gjavah.c
$(gccobjdir)/java/jcf-depend.o: java/jcf-depend.c
$(gccobjdir)/java/jcf-dump.o: java/jcf-dump.c
$(gccobjdir)/java/jcf-io.o: java/jcf-io.c
$(gccobjdir)/java/jcf-parse.o: java/jcf-parse.c
$(gccobjdir)/java/jcf-path.o: java/jcf-path.c
$(gccobjdir)/java/jcf-reader.o: java/jcf-reader.c
$(gccobjdir)/java/jcf-write.o: java/jcf-write.c
$(gccobjdir)/java/jv-scan.o: java/jv-scan.c
$(gccobjdir)/java/jvgenmain.o: java/jvgenmain.c
$(gccobjdir)/java/jvspec.o: java/jvspec.c
$(gccobjdir)/java/lang.o: java/lang.c
$(gccobjdir)/java/lex.o: java/lex.c
$(gccobjdir)/java/mangle.o: java/mangle.c
$(gccobjdir)/java/parse.o: java/parse.c
$(gccobjdir)/java/parse-scan.o: java/parse-scan.c
$(gccobjdir)/java/typeck.o: java/typeck.c
$(gccobjdir)/java/verify.o: java/verify.c
$(gccobjdir)/java/xref.o: java/xref.c
$(gccobjdir)/java/zextract.o: java/zextract.c 

$(gccobjdir)/gen/genattr.o: gen/genattr.c
$(gccobjdir)/gen/genattrtab.o: gen/genattrtab.c
$(gccobjdir)/gen/gencheck.o: gen/gencheck.c
$(gccobjdir)/gen/gencodes.o: gen/gencodes.c
$(gccobjdir)/gen/genconfig.o: gen/genconfig.c
$(gccobjdir)/gen/genemit.o: gen/genemit.c
$(gccobjdir)/gen/genextract.o: gen/genextract.c
$(gccobjdir)/gen/genflags.o: gen/genflags.c
$(gccobjdir)/gen/gengenrtl.o: gen/gengenrtl.c
$(gccobjdir)/gen/genopinit.o: gen/genopinit.c
$(gccobjdir)/gen/genoutput.o: gen/genoutput.c
$(gccobjdir)/gen/genpeep.o: gen/genpeep.c
$(gccobjdir)/gen/genrecog.o: gen/genrecog.c
$(gccobjdir)/gen/obstack.o: gen/obstack.c
$(gccobjdir)/gen/print-rtl.o: gen/print-rtl.c
$(gccobjdir)/gen/rtl.o: gen/rtl.c
$(gccobjdir)/gen/rtlanal.o: gen/rtlanal.c 

$(gccobjdir)/insn-attrtab.o: insn-attrtab.c
$(gccobjdir)/insn-emit.o: insn-emit.c
$(gccobjdir)/insn-extract.o: insn-extract.c
$(gccobjdir)/insn-opinit.o: insn-opinit.c
$(gccobjdir)/insn-output.o: insn-output.c
$(gccobjdir)/insn-peep.o: insn-peep.c
$(gccobjdir)/insn-recog.o: insn-recog.c 

$(gccobjdir)/config/arm/arm.o: config/arm/arm.c
$(gccobjdir)/config/arm/riscos.o: config/arm/riscos.c
$(gccobjdir)/config/arm/riscos_fix.o: config/arm/riscos_fix.c
$(gccobjdir)/config/arm/rname.o: config/arm/rname.c 

$(gccobjdir)/cpp/cppalloc.o: cpp/cppalloc.c
$(gccobjdir)/cpp/cpperror.o: cpp/cpperror.c
$(gccobjdir)/cpp/cppexp.o: cpp/cppexp.c
$(gccobjdir)/cpp/cppfiles.o: cpp/cppfiles.c
$(gccobjdir)/cpp/cpphash.o: cpp/cpphash.c
$(gccobjdir)/cpp/cppinit.o: cpp/cppinit.c
$(gccobjdir)/cpp/cpplib.o: cpp/cpplib.c
$(gccobjdir)/cpp/cppmain.o: cpp/cppmain.c
$(gccobjdir)/cpp/cppspec.o: cpp/cppspec.c
$(gccobjdir)/cpp/cppulp.o: cpp/cppulp.c 
