# Makefile for the GNU Java compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
GCC_AS = $(CC) -xassembler-with-cpp -c
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config -I../../zlib
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty -L$(objdir)/gcc-$(VERSION)/zlib/$(STAGE)/$(TRG) -lz

$(gccobjdir)/java/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

$(gccobjdir)/%.o: ../%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: java

.PHONY: java

java: $(gcc_bin_dir)/gcjh$(gexesfx) $(gcc_bin_dir)/jvgenmain$(gexesfx) \
	$(gcc_bin_dir)/jcf-dump$(gexesfx) \
        $(gcc_bin_dir)/jv-scan$(gexesfx) $(gcc_bin_dir)/jc1$(gexesfx)

LIBDEPS = $(gccobjdir)/prefix.o

ifeq (${STAGE},stage1)
RISC_OBJS=
RISCOS_OBJS=
else
RISC_OBJS = $(gccobjdir)/config/arm/riscos_fix.o
RISCOS_OBJS = $(RISC_OBJS) $(gccobjdir)/config/arm/riscos.o
endif

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
	$(gccobjdir)/java/mangle_name.o \
	$(gccobjdir)/java/builtins.o \
	$(gccobjdir)/java/jcf-write.o \
	$(gccobjdir)/java/buffer.o \
	$(gccobjdir)/java/check-init.o \
	$(gccobjdir)/java/jcf-depend.o \
	$(gccobjdir)/java/jcf-path.o \
	$(gccobjdir)/java/xref.o \
	$(gccobjdir)/java/boehm.o \
	$(gccobjdir)/java/java-tree-inline.o \
	$(gccobjdir)/mkdeps.o

JVSCAN_OBJS = $(gccobjdir)/java/parse-scan.o \
	$(gccobjdir)/java/jv-scan.o \
	$(gccobjdir)/version.o

JCF_DUMP_OBJS = $(gccobjdir)/java/jcf-dump.o \
	$(gccobjdir)/java/jcf-io.o \
	$(gccobjdir)/java/jcf-depend.o \
	$(gccobjdir)/java/jcf-path.o \
	$(gccobjdir)/java/zextract.o \
	$(gccobjdir)/errors.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/mkdeps.o

JVGENMAIN_OBJS = $(gccobjdir)/java/jvgenmain.o \
	$(gccobjdir)/java/mangle_name.o \
	$(gccobjdir)/errors.o

GCJH_OBJS = $(gccobjdir)/java/gjavah.o \
	$(gccobjdir)/java/jcf-io.o \
	$(gccobjdir)/java/jcf-depend.o \
	$(gccobjdir)/java/jcf-path.o \
	$(gccobjdir)/java/zextract.o \
	$(gccobjdir)/version.o \
	$(gccobjdir)/mkdeps.o \
	$(gccobjdir)/errors.o


# The real targets
$(gcc_bin_dir)/jc1$(gexesfx): $(JAVA_OBJS) $(BACKEND) $(RISCOS_OBJS)
	$(LINK) $(JAVA_OBJS) $(BACKEND) $(RISCOS_OBJS) $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/jv-scan$(gexesfx): $(JVSCAN_OBJS)
	$(LINK) $(JVSCAN_OBJS) $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/jcf-dump$(gexesfx): $(JCF_DUMP_OBJS)
	$(LINK) $(JCF_DUMP_OBJS) $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/jvgenmain$(gexesfx): $(JVGENMAIN_OBJS)
	$(LINK) $(JVGENMAIN_OBJS) $(LIBDEPS) $(LIBS)

$(gcc_bin_dir)/gcjh$(gexesfx): $(GCJH_OBJS)
	$(LINK) $(GCJH_OBJS) $(LIBDEPS) $(LIBS)

##############################################################################

parse.c: parse.y
	bison -t --name-prefix=java_ -o $@ parse.y

parse-scan.c: parse-scan.y
	bison -o $@ parse-scan.y

keyword.h: keyword.gperf
	gperf -L C -C -F ', 0' -p -t -j1 -i 1 -g -o -N java_keyword -k1,4,$$ keyword.gperf > $@

$(gccobjdir)/java/jcf-path.o: jcf-path.c
	$(CC) $(CFLAGS) $(INCLUDES) -O2 -o $@ -c jcf-path.c \
	-DLIBGCJ_ZIP_FILE=\"libgcj.zip\" \
	-DDEFAULT_TARGET_VERSION=\"$(VERSION)\"

$(gccobjdir)/java/java-tree-inline.o: ../tree-inline.c ../tree-inline.h
	$(CC) $(CFLAGS) $(INCLUDES) -O2 -o $@ -c ../tree-inline.c \
	-DINLINER_FOR_JAVA=1

# Dependencies
$(gccobjdir)/errors.o: ../errors.c

$(gccobjdir)/java/boehm.o: boehm.c
$(gccobjdir)/java/buffer.o: buffer.c
$(gccobjdir)/java/builtins.o: builtins.c
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
$(gccobjdir)/java/mangle_name.o: mangle_name.c
$(gccobjdir)/java/parse.o: parse.c
$(gccobjdir)/java/parse-scan.o: parse-scan.c
$(gccobjdir)/java/typeck.o: typeck.c
$(gccobjdir)/java/verify.o: verify.c
$(gccobjdir)/java/xref.o: xref.c
$(gccobjdir)/java/zextract.o: zextract.c 
