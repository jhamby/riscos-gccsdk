# Generated automatically from Makefile.in by configure.
# Makefile for the GNU Compiler Collection for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000, 2001 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for the host compiler
# HOST_CC is a version of CC where optimisations are not turned off
HOST_CC = $(CC) -g -O2 $(OPTS) -D$(hostsystem)
HOST_INCLUDES = -I./ -I../../include -I./config
HOST_PREDEFS = -DIN_GCC -DHAVE_CONFIG_H -DGENERATOR_FILE -DTARGET_RISCOSAOF
INCLUDES = -I./ -I../../include -I./config

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
gexesfx = $(exesfx)
AR = $(CROSS_AR)
AR_FLAGS = $(CROSS_AR_FLAGS)
INCLUDES += -I$(unixlib_incl_dir)
endif

# Defs for GCC
CFLAGS += -DIN_GCC -DHAVE_CONFIG_H
GCC_AS = $(CC) -xassembler-with-cpp -c
LINK = $(CC) -o $@
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty
gccobjdir = $(objdir)/gcc-$(VERSION)/gcc/$(STAGE)

export prefix CC CXX CFLAGS bin_dir gcc_bin_dir gccobjdir gexesfx

LIBDEPS = $(gccobjdir)/prefix.o

ifeq (${STAGE},stage1)
RISC_OBJS=
RISCOS_OBJS=
else
RISC_OBJS = $(gccobjdir)/config/arm/riscos_fix.o
RISCOS_OBJS = $(RISC_OBJS) $(gccobjdir)/config/arm/riscos.o
endif


# Language-specific object files for C and Objective C
C_AND_OBJC_OBJS = \
	$(gccobjdir)/attribs.o \
	$(gccobjdir)/c-errors.o \
	$(gccobjdir)/c-lex.o \
	$(gccobjdir)/c-pragma.o \
	$(gccobjdir)/c-decl.o \
	$(gccobjdir)/c-typeck.o \
	$(gccobjdir)/c-convert.o \
	$(gccobjdir)/c-aux-info.o \
	$(gccobjdir)/c-common.o \
	$(gccobjdir)/c-format.o \
	$(gccobjdir)/c-semantics.o \
	$(gccobjdir)/c-objc-common.o \
	$(gccobjdir)/libcpp.a

# Language-specific object files for C
C_OBJS = $(gccobjdir)/c-parse.o \
	$(gccobjdir)/c-lang.o \
	$(C_AND_OBJC_OBJS)

# Language-independent object files
OBJS =	$(gccobjdir)/alias.o \
	$(gccobjdir)/bb-reorder.o \
	$(gccobjdir)/bitmap.o \
	$(gccobjdir)/builtins.o \
	$(gccobjdir)/caller-save.o \
	$(gccobjdir)/calls.o \
	$(gccobjdir)/cfg.o \
	$(gccobjdir)/cfganal.o \
	$(gccobjdir)/cfgbuild.o \
	$(gccobjdir)/cfgcleanup.o \
	$(gccobjdir)/cfglayout.o \
	$(gccobjdir)/cfgloop.o \
	$(gccobjdir)/cfgrtl.o \
	$(gccobjdir)/combine.o \
	$(gccobjdir)/conflict.o \
	$(gccobjdir)/convert.o \
	$(gccobjdir)/cse.o \
	$(gccobjdir)/cselib.o \
	$(gccobjdir)/dbxout.o \
	$(gccobjdir)/debug.o \
	$(gccobjdir)/dependence.o \
	$(gccobjdir)/df.o \
	$(gccobjdir)/diagnostic.o \
	$(gccobjdir)/doloop.o \
	$(gccobjdir)/dominance.o \
	$(gccobjdir)/dwarf2asm.o \
	$(gccobjdir)/dwarf2out.o \
	$(gccobjdir)/dwarfout.o \
	$(gccobjdir)/emit-rtl.o \
	$(gccobjdir)/except.o \
	$(gccobjdir)/explow.o \
	$(gccobjdir)/expmed.o \
	$(gccobjdir)/expr.o \
	$(gccobjdir)/final.o \
	$(gccobjdir)/flow.o \
	$(gccobjdir)/fold-const.o \
	$(gccobjdir)/function.o \
	$(gccobjdir)/gcse.o \
	$(gccobjdir)/genrtl.o \
	$(gccobjdir)/ggc-common.o \
	$(gccobjdir)/global.o \
	$(gccobjdir)/graph.o \
	$(gccobjdir)/haifa-sched.o \
	$(gccobjdir)/hash.o \
	$(gccobjdir)/hashtable.o \
	$(gccobjdir)/ifcvt.o \
	$(gccobjdir)/insn-attrtab.o \
	$(gccobjdir)/insn-emit.o \
	$(gccobjdir)/insn-extract.o \
	$(gccobjdir)/insn-opinit.o \
	$(gccobjdir)/insn-output.o \
	$(gccobjdir)/insn-peep.o \
	$(gccobjdir)/insn-recog.o \
	$(gccobjdir)/integrate.o \
	$(gccobjdir)/jump.o \
	$(gccobjdir)/langhooks.o \
	$(gccobjdir)/lcm.o \
	$(gccobjdir)/lists.o \
	$(gccobjdir)/local-alloc.o \
	$(gccobjdir)/loop.o \
	$(gccobjdir)/mbchar.o \
	$(gccobjdir)/optabs.o \
	$(gccobjdir)/params.o \
	$(gccobjdir)/predict.o \
	$(gccobjdir)/print-rtl.o \
	$(gccobjdir)/print-tree.o \
	$(gccobjdir)/profile.o \
	$(gccobjdir)/real.o \
	$(gccobjdir)/recog.o \
	$(gccobjdir)/reg-stack.o \
	$(gccobjdir)/regclass.o \
	$(gccobjdir)/regmove.o \
	$(gccobjdir)/regrename.o \
	$(gccobjdir)/reload.o \
	$(gccobjdir)/reload1.o \
	$(gccobjdir)/reorg.o \
	$(gccobjdir)/resource.o \
	$(gccobjdir)/rtl.o \
	$(gccobjdir)/rtlanal.o \
	$(gccobjdir)/rtl-error.o \
	$(gccobjdir)/sbitmap.o \
	$(gccobjdir)/sched-deps.o \
	$(gccobjdir)/sched-ebb.o \
	$(gccobjdir)/sched-rgn.o \
	$(gccobjdir)/sched-vis.o \
	$(gccobjdir)/sdbout.o \
	$(gccobjdir)/sibcall.o \
	$(gccobjdir)/simplify-rtx.o \
	$(gccobjdir)/ssa.o \
	$(gccobjdir)/ssa-ccp.o \
	$(gccobjdir)/ssa-dce.o \
	$(gccobjdir)/stmt.o \
	$(gccobjdir)/stor-layout.o \
	$(gccobjdir)/stringpool.o \
	$(gccobjdir)/timevar.o \
	$(gccobjdir)/toplev.o \
	$(gccobjdir)/tree.o \
	$(gccobjdir)/tree-dump.o \
	$(gccobjdir)/tree-inline.o \
	$(gccobjdir)/unroll.o \
	$(gccobjdir)/varasm.o \
	$(gccobjdir)/varray.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/xcoffout.o \
	$(gccobjdir)/ggc-simple.o \
	$(gccobjdir)/arm.o

#	$(gccobjdir)/intl.o \

BACKEND = $(gccobjdir)/main.o $(gccobjdir)/libbackend.a

LIBCPP_OBJS = 	$(gccobjdir)/cppdefault.o \
	$(gccobjdir)/cpperror.o \
	$(gccobjdir)/cppexp.o \
	$(gccobjdir)/cppfiles.o \
	$(gccobjdir)/cpphash.o \
	$(gccobjdir)/cppinit.o \
	$(gccobjdir)/cpplib.o \
	$(gccobjdir)/cpplex.o \
	$(gccobjdir)/cppmacro.o \
	$(gccobjdir)/cppspec.o \
	$(gccobjdir)/hashtable.o \
	$(gccobjdir)/line-map.o \
	$(gccobjdir)/mkdeps.o \
	$(gccobjdir)/prefix.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/mbchar.o

# GEN files are listed separately, so they can be built before doing parallel
# makes for cc1 or cc1plus.  Otherwise sequent parallel make attempts to load
# them before rtl.o is compiled.
GEN = genemit gengenrtl genoutput genrecog genextract genflags \
	gencodes genconfig genpeep gencheck genpreds genconstants

export BACKEND RISCOS_OBJS C_AND_OBJC_OBJS LIBS LIBDEPS


$(gccobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)


all: c cp ada f77 objc java

# cp f77 gpc objc java docs

.PHONY: c
.PHONY: cp
.PHONY: ada
.PHONY: f77
.PHONY: gpc
.PHONY: objc
.PHONY: java
.PHONY: docs

c: $(bin_dir)/gcc$(gexesfx) $(gcc_bin_dir)/cpp0$(gexesfx) \
   $(gcc_bin_dir)/cc1$(gexesfx) $(bin_dir)/gcov$(gexesfx)

ada cp f77 gpc objc java:
	$(MAKE) -C $@


ifeq (${STAGE},stage1)
docs:
else
docs:	$(info_dir)/gcc $(info_dir)/cpp
endif


$(gccobjdir)/libbackend.a: $(OBJS)
	$(AR) $(AR_FLAGS) $@ $(OBJS)

$(gccobjdir)/libcpp.a:	$(LIBCPP_OBJS)
	$(AR) $(AR_FLAGS) $@ $(LIBCPP_OBJS)

# The real targets
$(bin_dir)/gcc$(gexesfx): $(gccobjdir)/gcc.o $(gccobjdir)/gccspec.o \
	$(gccobjdir)/version.o $(RISC_OBJS) \
	$(gccobjdir)/config/arm/rname.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/gcc.o $(gccobjdir)/gccspec.o \
	$(gccobjdir)/version.o $(RISC_OBJS) \
	$(gccobjdir)/config/arm/rname.o $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/cc1$(gexesfx): $(C_OBJS) $(BACKEND) $(RISCOS_OBJS)
	$(LINK) $(C_OBJS) $(BACKEND) $(RISCOS_OBJS) $(LIBS)

$(gcc_bin_dir)/cpp0$(gexesfx): $(gccobjdir)/cppmain.o \
	$(gccobjdir)/libcpp.a $(gccobjdir)/config/arm/rname.o
	$(LINK) $(gccobjdir)/cppmain.o $(gccobjdir)/libcpp.a \
	$(gccobjdir)/config/arm/rname.o $(LIBS)

$(bin_dir)/gcov$(gexesfx): $(gccobjdir)/gcov.o $(gccobjdir)/version.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/gcov.o $(gccobjdir)/version.o $(LIBDEPS) $(LIBS)

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
	sed -e "/^ifobjc$$/,/^end ifobjc$$/d" -e "/^ifc$$/d" -e "/^end ifc$$/d" $? > c-parse.y

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
$(gccobjdir)/c-parse.o:	c-parse.c c-parse.c c-parse.in tree-check.h
$(gccobjdir)/cexp.o:	cexp.c cexp.y

##############################################################################

ifeq (${STAGE},stage1)
# Derived files. Created from config/arm/md

MD = config/arm/arm.md
GENOBJS = $(gccobjdir)/_obstack.o $(gccobjdir)/_rtl.o \
	$(gccobjdir)/_bitmap.o $(gccobjdir)/ggc-none.o \
	$(gccobjdir)/_errors.o $(gccobjdir)/gensupport.o

HOST_RTL =	$(gccobjdir)/_rtl.o \
	$(gccobjdir)/_read-rtl.o \
	$(gccobjdir)/_bitmap.o \
	$(gccobjdir)/_ggc-none.o \
	$(gccobjdir)/gensupport.o \
	$(gccobjdir)/_hashtab.o \
	$(gccobjdir)/_safe-ctype.o

HOST_PRINT = $(gccobjdir)/print-rtl.o

HOST_ERRORS = $(gccobjdir)/errors.o

gen-derived:	insn-config.h insn-flags.h insn-codes.h \
	insn-emit.c insn-recog.c insn-extract.c \
	insn-opinit.c insn-output.c insn-peep.c \
        insn-attr.h insn-attrtab.c genrtl.c genrtl.h tm-preds.h

gen-prog:	$(gccobjdir)/genemit $(gccobjdir)/genoutput \
	$(gccobjdir)/genrecog $(gccobjdir)/genextract \
	$(gccobjdir)/genflags $(gccobjdir)/gencodes \
	$(gccobjdir)/genconfig $(gccobjdir)/genpeep \
	$(gccobjdir)/genconstants $(gccobjdir)/gengenrtl \
	$(gccobjdir)/gencheck $(gccobjdir)/genpreds

# Stuff to make generate files to generate insn-*.[ch] files.

# Our gen versions are subtly different. We include hconfig.h instead
# of config.h.

$(gccobjdir)/bitmap.c:	bitmap.c
	sed -e "s/config[.]h/hconfig.h/" bitmap.c >$@
$(gccobjdir)/_bitmap.o:  $(gccobjdir)/bitmap.c
	$(HOST_CC) -c $(gccobjdir)/bitmap.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/errors.c:	errors.c
	sed -e "s/config[.]h/hconfig.h/" errors.c >$@
$(gccobjdir)/_errors.o:  $(gccobjdir)/errors.c
	$(HOST_CC) -c $(gccobjdir)/errors.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/ggc-none.c:	ggc-none.c
	sed -e "s/config[.]h/hconfig.h/" ggc-none.c >$@
$(gccobjdir)/_ggc-none.o:  $(gccobjdir)/ggc-none.c
	$(HOST_CC) -c $(gccobjdir)/ggc-none.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/obstack.c:	../../libiberty/obstack.c
	sed -e "s/config[.]h/hconfig.h/" ../../libiberty/obstack.c >$@
$(gccobjdir)/_obstack.o:  $(gccobjdir)/obstack.c
	$(HOST_CC) -c $(gccobjdir)/obstack.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/rtl.c:	rtl.c
	sed -e "s/config[.]h/hconfig.h/" rtl.c >$@
$(gccobjdir)/_rtl.o:	$(gccobjdir)/rtl.c 
	$(HOST_CC) -c $(gccobjdir)/rtl.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/print-rtl.c:	print-rtl.c
	sed -e "s/config[.]h/hconfig.h/" print-rtl.c >$@
$(gccobjdir)/_print-rtl.o:	$(gccobjdir)/print-rtl.c
	$(HOST_CC) -c $(gccobjdir)/print-rtl.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/_read-rtl.o:	read-rtl.c
	$(HOST_CC) -c read-rtl.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/_hashtab.o:	../../libiberty/hashtab.c
	$(HOST_CC) -c ../../libiberty/hashtab.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/_safe-ctype.o:	../../libiberty/safe-ctype.c
	$(HOST_CC) -c ../../libiberty/safe-ctype.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)




$(gccobjdir)/genattr.o:	genattr.c
	$(HOST_CC) -c genattr.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genattrtab.o:	genattrtab.c
	$(HOST_CC) -c genattrtab.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gencheck.o:	gencheck.c tree.def
	$(HOST_CC) -c gencheck.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genconstants.o: genconstants.c
	$(HOST_CC) -c genconstants.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genemit.o:	genemit.c
	$(HOST_CC) -c genemit.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gengenrtl.o:	gengenrtl.c
	$(HOST_CC) -c gengenrtl.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genoutput.o:	genoutput.c
	$(HOST_CC) -c genoutput.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genopinit.o:	genopinit.c
	$(HOST_CC) -c genopinit.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genrecog.o:	genrecog.c
	$(HOST_CC) -c genrecog.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genextract.o:	genextract.c
	$(HOST_CC) -c genextract.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genflags.o:	genflags.c
	$(HOST_CC) -c genflags.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gencodes.o:	gencodes.c rtl.h genrtl.h
	$(HOST_CC) -c gencodes.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genconfig.o:	genconfig.c
	$(HOST_CC) -c genconfig.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genpeep.o:	genpeep.c
	$(HOST_CC) -c genpeep.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/gensupport.o:	gensupport.c
	$(HOST_CC) -c gensupport.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genpreds.o:	genpreds.c
	$(HOST_CC) -c genpreds.c -o $@ $(HOST_INCLUDES) $(HOST_PREDEFS)

$(gccobjdir)/genattr:	$(gccobjdir)/genattr.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genattr.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genattrtab:	$(gccobjdir)/genattrtab.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genattrtab.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/gencheck:	$(gccobjdir)/gencheck.o tree.def
	$(LINK) $(gccobjdir)/gencheck.o $(LIBS)

$(gccobjdir)/gencodes:	  $(gccobjdir)/gencodes.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/gencodes.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genconstants:	  $(gccobjdir)/genconstants.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genconstants.o $(HOST_RTL) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genconfig:	 $(gccobjdir)/genconfig.o genrtl.h $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genconfig.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genemit:	$(gccobjdir)/genemit.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genemit.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genextract:	$(gccobjdir)/genextract.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) insn-config.h
	$(LINK) $(gccobjdir)/genextract.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genflags:	  $(gccobjdir)/genflags.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genflags.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)


$(gccobjdir)/gengenrtl:	$(gccobjdir)/gengenrtl.o $(gccobjdir)/_obstack.o
	$(LINK) $(gccobjdir)/gengenrtl.o $(gccobjdir)/_obstack.o $(LIBS)

$(gccobjdir)/genoutput:	 $(gccobjdir)/genoutput.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genoutput.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genopinit:	 $(gccobjdir)/genopinit.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genopinit.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genrecog:	  $(gccobjdir)/genrecog.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genrecog.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genpeep:	   $(gccobjdir)/genpeep.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genpeep.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

$(gccobjdir)/genpreds:	$(gccobjdir)/genpreds.c $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS)
	$(LINK) $(gccobjdir)/genpreds.o $(HOST_RTL) $(HOST_PRINT) $(HOST_ERRORS) $(LIBS)

insn-config.h:	$(MD) $(gccobjdir)/genconfig
	$(gccobjdir)/genconfig $(MD) >$@
insn-flags.h:	$(MD) $(gccobjdir)/genflags
	$(gccobjdir)/genflags $(MD) >$@
insn-codes.h:	$(MD) $(gccobjdir)/gencodes
	$(gccobjdir)/gencodes $(MD) >$@
insn-constants.h:	$(MD) $(gccobjdir)/genconstants
	$(gccobjdir)/genconstants $(MD) >$@
insn-attr.h:	$(MD) $(gccobjdir)/genattr
	$(gccobjdir)/genattr $(MD) >$@
tree-check.h:	$(gccobjdir)/gencheck tree.def
	$(gccobjdir)/gencheck >tree-check.h

insn-emit.c:	$(MD) $(gccobjdir)/genemit
	$(gccobjdir)/genemit $(MD) >$@
insn-recog.c:	$(MD) $(gccobjdir)/genrecog
	$(gccobjdir)/genrecog $(MD) >$@
insn-extract.c:	$(MD) $(gccobjdir)/genextract
	$(gccobjdir)/genextract $(MD) >$@
insn-opinit.c:	$(MD) $(gccobjdir)/genopinit
	$(gccobjdir)/genopinit $(MD) >$@
insn-output.c:	$(MD) $(gccobjdir)/genoutput
	$(gccobjdir)/genoutput $(MD) >$@
insn-peep.c:	$(MD) $(gccobjdir)/genpeep
	$(gccobjdir)/genpeep $(MD) >$@
insn-attrtab.c:	$(MD) $(gccobjdir)/genattrtab
	$(gccobjdir)/genattrtab $(MD) >$@

genrtl.c:	$(gccobjdir)/gengenrtl
	$(gccobjdir)/gengenrtl >$@

genrtl.h:	$(gccobjdir)/gengenrtl
	$(gccobjdir)/gengenrtl -h >$@

tmp-preds.h:	$(gccobjdir)/genpreds
	$(gccobjdir)/genpreds >$@
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

$(gccobjdir)/cppdefault.o: cppdefault.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c cppdefault.c \
	-DGCC_INCLUDE_DIR=\"$(prefix)$(gn_gcc_include_dir)\" \
	-DGPLUSPLUS_INCLUDE_DIR=\"$(prefix)$(gn_gplusplus_include_dir)\" \
	-DLOCAL_INCLUDE_DIR=\"$(prefix)$(gn_local_include_dir)\" \
	-DSYSTEM_INCLUDE_DIR=\"$(prefix)$(gn_system_include_dir)\" \
	-DCROSS_INCLUDE_DIR=\"$(prefix)$(gn_cross_include_dir)\"

# Dependencies
$(gccobjdir)/alias.o: alias.c
$(gccobjdir)/arm.o: config/arm/arm.c config.h system.h rtl.h tree.h \
	regs.h hard-reg-set.h real.h insn-config.h conditions.h \
	output.h insn-attr.h flags.h reload.h function.h expr.h toplev.h \
	recog.h ggc.h except.h c-pragma.h tm_p.h
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/bb-reorder.o: bb-reorder.c insn-config.h
$(gccobjdir)/bitmap.o: bitmap.c insn-codes.h insn-flags.h
$(gccobjdir)/builtins.o: builtins.c
$(gccobjdir)/caller-save.o: caller-save.c
$(gccobjdir)/calls.o: calls.c
$(gccobjdir)/combine.o: combine.c insn-attr.h
$(gccobjdir)/conflict.o: conflict.c
$(gccobjdir)/convert.o: convert.c
$(gccobjdir)/cse.o: cse.c
$(gccobjdir)/dbxout.o: dbxout.c
$(gccobjdir)/debug.o: debug.c debug.h
$(gccobjdir)/dependence.o: dependence.c
$(gccobjdir)/df.o: df.c
$(gccobjdir)/diagnostic.o: diagnostic.c
$(gccobjdir)/doloop.o: doloop.c
$(gccobjdir)/dominance.o: dominance.c
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
$(gccobjdir)/gcse.o: gcse.c
$(gccobjdir)/genrtl.o: genrtl.c
$(gccobjdir)/ggc-common.o: ggc-common.c
$(gccobjdir)/ggc-simple.o: ggc-simple.c
$(gccobjdir)/global.o: global.c
$(gccobjdir)/graph.o: graph.c
$(gccobjdir)/haifa-sched.o: haifa-sched.c
$(gccobjdir)/hash.o: hash.c
$(gccobjdir)/hashtable.o: hashtable.c hashtable.h
$(gccobjdir)/ifcvt.o: ifcvt.c
$(gccobjdir)/integrate.o: integrate.c
$(gccobjdir)/jump.o: jump.c
$(gccobjdir)/lcm.o: lcm.c
$(gccobjdir)/langhooks.o: langhooks.c
$(gccobjdir)/lists.o: lists.c
$(gccobjdir)/local-alloc.o: local-alloc.c
$(gccobjdir)/loop.o: loop.c
$(gccobjdir)/mbchar.o: mbchar.c
$(gccobjdir)/optabs.o: optabs.c
$(gccobjdir)/params.o: params.c
$(gccobjdir)/predict.o: predict.c
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
$(gccobjdir)/sched-deps.o: sched-deps.c
$(gccobjdir)/sched-ebb.o: sched-ebb.c
$(gccobjdir)/sched-rgn.o: sched-rgn.c
$(gccobjdir)/sched-vis.o: sched-vis.c
$(gccobjdir)/sibcall.o: sibcall.c
$(gccobjdir)/simplify-rtx.o: simplify-rtx.c
$(gccobjdir)/ssa.o: ssa.c
$(gccobjdir)/ssa-ccp.o: ssa-ccp.c
$(gccobjdir)/ssa-dce.o: ssa-dce.c
$(gccobjdir)/stmt.o: stmt.c insn-config.h
$(gccobjdir)/stor-layout.o: stor-layout.c
$(gccobjdir)/string-pool.o: stringpool.c
$(gccobjdir)/timevar.o: timevar.c
$(gccobjdir)/toplev.o: toplev.c
$(gccobjdir)/tree.o: tree.c
$(gccobjdir)/unroll.o: unroll.c
$(gccobjdir)/varasm.o: varasm.c
$(gccobjdir)/varray.o: varray.c
$(gccobjdir)/version.o: version.c

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

$(gccobjdir)/gcc.o: gcc.c config.h insn-codes.h insn-flags.h multilib.h insn-constants.h
$(gccobjdir)/gccspec.o: gccspec.c
$(gccobjdir)/gcov.o: gcov.c

$(gccobjdir)/getpwd.o: getpwd.c
$(gccobjdir)/libgcc2.o: libgcc2.c
$(gccobjdir)/prefix.o: prefix.c

$(gccobjdir)/genattr.o: genattr.c
$(gccobjdir)/genattrtab.o: genattrtab.c
$(gccobjdir)/gencheck.o: gencheck.c
$(gccobjdir)/gencodes.o: gencodes.c
$(gccobjdir)/genconfig.o: genconfig.c
$(gccobjdir)/genconstants.o: genconstants.c
$(gccobjdir)/genemit.o: genemit.c
$(gccobjdir)/genextract.o: genextract.c
$(gccobjdir)/genflags.o: genflags.c
$(gccobjdir)/gengenrtl.o: gengenrtl.c
$(gccobjdir)/genopinit.o: genopinit.c
$(gccobjdir)/genoutput.o: genoutput.c
$(gccobjdir)/genpeep.o: genpeep.c
$(gccobjdir)/genrecog.o: genrecog.c
$(gccobjdir)/_obstack.o: $(gccobjdir)/obstack.c
$(gccobjdir)/_print-rtl.o: $(gccobjdir)/print-rtl.c
$(gccobjdir)/_rtl.o: $(gccobjdir)/rtl.c
$(gccobjdir)/_rtlanal.o: $(gccobjdir)/rtlanal.c 

$(gccobjdir)/insn-attrtab.o: insn-attrtab.c
$(gccobjdir)/insn-emit.o: insn-emit.c
$(gccobjdir)/insn-extract.o: insn-extract.c
$(gccobjdir)/insn-opinit.o: insn-opinit.c
$(gccobjdir)/insn-output.o: insn-output.c
$(gccobjdir)/insn-peep.o: insn-peep.c
$(gccobjdir)/insn-recog.o: insn-recog.c 

$(gccobjdir)/config/arm/riscos.o: config/arm/riscos.c
$(gccobjdir)/config/arm/riscos_fix.o: config/arm/riscos_fix.c
$(gccobjdir)/config/arm/rname.o: config/arm/rname.c 

$(gccobjdir)/cpperror.o: cpperror.c
$(gccobjdir)/cppexp.o: cppexp.c
$(gccobjdir)/cppfiles.o: cppfiles.c
$(gccobjdir)/cpphash.o: cpphash.c
$(gccobjdir)/cpplib.o: cpplib.c
$(gccobjdir)/cpplex.o: cpplex.c
$(gccobjdir)/cppmacro.o: cppmacro.c
$(gccobjdir)/cppmain.o: cppmain.c
$(gccobjdir)/cppspec.o: cppspec.c

