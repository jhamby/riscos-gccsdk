# Makefile for the GNU Compiler Collection for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for the host compiler
# HOST_CC is a version of CC where optimisations are not turned off
HOST_CC = $(CC) -O2 $(OPTS) -D$(hostsystem)
HOST_INCLUDES = -I./ -I../../include -I./config
HOST_PREDEFS = -DIN_GCC -DHAVE_CONFIG_H

ifeq (${STAGE},stage1)
prefix = $(unix_prefix)
CFLAGS += -DCROSS_COMPILE -D$(hostsystem)
bin_dir = $(ux_bin_dir)
gcc_bin_dir = $(ux_gcc_bin_dir)
else
prefix = $(riscos_prefix)
CC = $(CROSS_CC)
CXX = $(CROSS_CXX)
CFLAGS = $(CROSS_CFLAGS) -D$(targetsystem)
bin_dir = $(ro_bin_dir)
gcc_bin_dir = $(ro_gcc_bin_dir)
endif

# Defs for GCC
CFLAGS += -DIN_GCC -DHAVE_CONFIG_H
GCC_AS = $(CC) -xassembler-with-cpp -c
LINK = $(CC) -o $@
INCLUDES = -I./ -I../../include -I./config
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty
gccobjdir = $(objdir)/gcc-$(VERSION)/gcc/$(STAGE)

export prefix CC CXX CFLAGS bin_dir gcc_bin_dir gccobjdir


$(gccobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: c cp f77 gpc objc java docs

.PHONY: c
.PHONY: cp
.PHONY: f77
.PHONY: gpc
.PHONY: objc
.PHONY: java
.PHONY: docs

c: $(bin_dir)/gcc $(gcc_bin_dir)/cpp $(gcc_bin_dir)/cc1 $(bin_dir)/gcov

cp f77 gpc objc java:
	$(MAKE) -C $@

ifeq (${STAGE},stage1)
docs:
else
docs:	$(info_dir)/gcc $(info_dir)/cpp
endif

LIBDEPS = $(gccobjdir)/prefix.o

ifeq (${STAGE},stage1)
RISC_OBJS=
RISCOS_OBJS=
else
RISC_OBJS = $(gccobjdir)/config/arm/riscos_fix.o
RISCOS_OBJS = $(RISC_OBJS) $(gccobjdir)/config/arm/riscos.o
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
	$(gccobjdir)/cpp/cppmain.o \
	$(gccobjdir)/mbchar.o


# GEN files are listed separately, so they can be built before doing parallel
# makes for cc1 or cc1plus.  Otherwise sequent parallel make attempts to load
# them before rtl.o is compiled.
GEN = genemit gengenrtl genoutput genrecog genextract genflags \
	gencodes genconfig genpeep gencheck

# The real targets

$(bin_dir)/gcc:	$(gccobjdir)/gcc.o $(gccobjdir)/gccspec.o \
	$(gccobjdir)/version.o $(RISC_OBJS) \
	$(gccobjdir)/config/arm/rname.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/gcc.o $(gccobjdir)/gccspec.o \
	$(gccobjdir)/version.o $(RISC_OBJS) \
	$(gccobjdir)/config/arm/rname.o $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/cc1: $(C_OBJS) $(OBJS) $(OBJS1) $(RISCOS_OBJS)
	$(LINK) $(C_OBJS) $(OBJS) $(OBJS1) $(RISCOS_OBJS) $(LIBS)

$(gcc_bin_dir)/cpp: $(CPP_OBJS) $(gccobjdir)/version.o \
	$(gccobjdir)/config/arm/rname.o \
	$(RISCOS_OBJS) $(LIBDEPS)
	$(LINK) $(CPP_OBJS) $(gccobjdir)/version.o \
	$(gccobjdir)/config/arm/rname.o \
	$(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

$(bin_dir)/gcov: $(gccobjdir)/gcov.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/gcov.o $(LIBDEPS) $(LIBS)

#$(bin_dir)/demangle: $(LIBIBERTY)
#	$(LINK) $(LIBS)


ifeq (${STAGE},stage2)
$(info_dir)/gcc:	extend.texi gcc.texi gcov.texi install.texi \
	install1.texi invoke.texi md.texi rtl.texi tm.texi
	-makeinfo --no-split --number-sections --force gcc.texi -o $@

$(info_dir)/cpp:	cpp.texi
	makeinfo --no-split --number-sections --force cpp.texi -o $@
endif

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
	bison -d c-parse.y -o c-parse.c

# Perfect hash function generation stuff

#h.c-gperf: gperf/c-parse
#	gperf -p -j1 -i 1 -g -o -t -G -N is_reserved_word -k1,3,$$ <gperf.c-parse >h.c-gperf

# Bison object dependencies
$(gccobjdir)/c-parse.o:	c-parse.c c-parse.c c-parse.in
$(gccobjdir)/cexp.o:	cexp.c cexp.y

##############################################################################

ifeq (${STAGE},stage1)
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
endif

##############################################################################

$(gccobjdir)/cplus-dem.o: cplus-dem.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c cplus-dem.c -DMAIN

$(gccobjdir)/except.o: except.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c except.c -DDONT_USE_BUILTIN_SETJMP -DJMP_BUF_SIZE=24

$(gccobjdir)/optabs.o: optabs.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c optabs.c -DDONT_USE_BUILTIN_SETJMP -DJMP_BUF_SIZE=24

$(gccobjdir)/prefix.o: prefix.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c prefix.c -DPREFIX=\"rubbish\"

$(gccobjdir)/toplev.o: toplev.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c toplev.c -DTARGET_NAME=\"$(TARGET_NAME)\"

$(gccobjdir)/gcc.o: gcc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c gcc.c \
	-DDEFAULT_TARGET_VERSION=\"$(VERSION)\" \
	-DDEFAULT_TARGET_MACHINE=\"$(TARGET_NAME)\" \
	-DSTANDARD_STARTFILE_PREFIX=\"$(prefix)$(gn_standard_startfile_prefix)\" \
	-DSTANDARD_EXEC_PREFIX=\"$(prefix)$(gn_standard_exec_prefix)\"

$(gccobjdir)/cpp/cppinit.o: cpp/cppinit.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c cpp/cppinit.c \
	-DGCC_INCLUDE_DIR=\"$(prefix)$(gn_gcc_include_dir)\" \
	-DGPLUSPLUS_INCLUDE_DIR=\"$(prefix)$(gn_gplusplus_include_dir)\" \
	-DLOCAL_INCLUDE_DIR=\"$(prefix)$(gn_local_include_dir)\" \
	-DSYSTEM_INCLUDE_DIR=\"$(prefix)$(gn_system_include_dir)\" \
	-DCROSS_INCLUDE_DIR=\"$(prefix)$(gn_cross_include_dir)\"

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
