# Makefile for the GNU Fortran 77 compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty

$(gccobjdir)/f77/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: fortran77 docs

.PHONY: fortran77
.PHONY: docs

ifeq (${STAGE},stage1)
docs:
else
docs:	$(info_dir)/g77
endif

fortran77: $(gccobjdir)/f77/fini $(bin_dir)/g77$(gexesfx) \
  $(gcc_bin_dir)/f771$(gexesfx)

LIBDEPS = $(gccobjdir)/prefix.o

ifeq (${STAGE},stage1)
RISC_OBJS=
RISCOS_OBJS=
else
RISC_OBJS = $(gccobjdir)/config/arm/riscos_fix.o
RISCOS_OBJS = $(RISC_OBJS) $(gccobjdir)/config/arm/riscos.o
endif

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
	$(gccobjdir)/f77/where.o

STR_HEADERS = str-1t.h str-2t.h str-fo.h str-io.h str-nq.h str-op.h str-ot.h \
	str-1t.j str-2t.j str-fo.j str-io.j str-nq.j str-op.j str-ot.j


# The real targets
$(bin_dir)/g77$(gexesfx): $(gccobjdir)/f77/g77.o $(gccobjdir)/f77/g77spec.o \
	$(gccobjdir)/version.o $(gccobjdir)/config/arm/rname.o \
	$(RISC_OBJS) $(LIBDEPS)
	$(LINK) $(gccobjdir)/f77/g77.o $(gccobjdir)/f77/g77spec.o \
	$(gccobjdir)/version.o $(gccobjdir)/config/arm/rname.o \
	$(RISC_OBJS) $(LIBDEPS) $(LIBS)

$(gccobjdir)/f77/fini:	$(gccobjdir)/f77/fini.o $(LIBDEPS)
	$(LINK) $(gccobjdir)/f77/fini.o $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/f771$(gexesfx): $(F77_OBJS) $(BACKEND) $(RISCOS_OBJS) $(STR_HEADERS)
	$(LINK) $(F77_OBJS) $(BACKEND) $(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

ifeq (${STAGE},stage2)
$(info_dir)/g77:	bugs.texi bugs0.texi ffe.texi g77.texi \
	g77install.texi install0.texi intdoc.texi news.texi news0.texi \
	root.texi
	-makeinfo --no-split --number-sections --force g77.texi -o $@
endif

##############################################################################

# Fortran 77 make sources

ifeq (${STAGE},stage1)
# the j.str-1t.j equivalents are automatically updated with the fini program

str-1t.h str-1t.j:	fin/str-1t $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini fin/str-1t str-1t.j str-1t.h

str-2t.h str-2t.j:	fin/str-2t $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini fin/str-2t str-2t.j str-2t.h

str-fo.h str-fo.j:	fin/str-fo $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini fin/str-fo str-fo.j str-fo.h

str-io.h str-io.j:	fin/str-io $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini fin/str-io str-io.j str-io.h

str-nq.h str-nq.j:	fin/str-nq $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini fin/str-nq str-nq.j str-nq.h

str-op.h str-op.j:	fin/str-op $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini fin/str-op str-op.j str-op.h

str-ot.h str-ot.j:	fin/str-ot $(gccobjdir)/f77/fini
		$(gccobjdir)/f77/fini fin/str-ot str-ot.j str-ot.h
endif

##############################################################################

$(gccobjdir)/f77/g77.o: ../gcc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c ../gcc.c \
	-DDEFAULT_TARGET_VERSION=\"$(VERSION)\" \
	-DDEFAULT_TARGET_MACHINE=\"$(TARGET_NAME)\" -DLANG_SPECIFIC_DRIVER \
	-DSTANDARD_STARTFILE_PREFIX=\"$(prefix)$(gn_standard_startfile_prefix)\" \
	-DSTANDARD_EXEC_PREFIX=\"$(prefix)$(gn_standard_exec_prefix)\"

# Dependencies
$(gccobjdir)/f77/ansify.o: ansify.c
$(gccobjdir)/f77/bad.o: bad.c
$(gccobjdir)/f77/bit.o: bit.c
$(gccobjdir)/f77/bld.o: bld.c
$(gccobjdir)/f77/com.o: com.c
$(gccobjdir)/f77/data.o: data.c
$(gccobjdir)/f77/equiv.o: equiv.c
$(gccobjdir)/f77/expr.o: expr.c
$(gccobjdir)/f77/fini.o: fini.c
$(gccobjdir)/f77/g77spec.o: g77spec.c
$(gccobjdir)/f77/global.o: global.c
$(gccobjdir)/f77/implic.o: implic.c
$(gccobjdir)/f77/info.o: info.c
$(gccobjdir)/f77/intdoc.o: intdoc.c
$(gccobjdir)/f77/intrin.o: intrin.c
$(gccobjdir)/f77/lab.o: lab.c
$(gccobjdir)/f77/lex.o: lex.c
$(gccobjdir)/f77/malloc.o: malloc.c
$(gccobjdir)/f77/name.o: name.c
$(gccobjdir)/f77/parse.o: parse.c
$(gccobjdir)/f77/src.o: src.c
$(gccobjdir)/f77/st.o: st.c
$(gccobjdir)/f77/sta.o: sta.c
$(gccobjdir)/f77/stb.o: stb.c str.h str-1t.h str-fo.h \
	str-io.h str-nq.h str-ot.h str-op.h str-2t.h
$(gccobjdir)/f77/stc.o: stc.c
$(gccobjdir)/f77/std.o: std.c
$(gccobjdir)/f77/ste.o: ste.c
$(gccobjdir)/f77/storag.o: storag.c
$(gccobjdir)/f77/stp.o: stp.c
$(gccobjdir)/f77/str.o: str.c
$(gccobjdir)/f77/sts.o: sts.c
$(gccobjdir)/f77/stt.o: stt.c
$(gccobjdir)/f77/stu.o: stu.c
$(gccobjdir)/f77/stv.o: stv.c
$(gccobjdir)/f77/stw.o: stw.c
$(gccobjdir)/f77/symbol.o: symbol.c
$(gccobjdir)/f77/target.o: target.c
$(gccobjdir)/f77/top.o: top.c
$(gccobjdir)/f77/type.o: type.c
$(gccobjdir)/f77/where.o: where.c
