# Makefile for the GNU C++ compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000, 2004 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty

$(gccobjdir)/cp/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: cpp

.PHONY: cpp

cpp: $(gcc_bin_dir)/cc1plus$(gexesfx) $(bin_dir)/g++$(gexesfx)

# Shared with C front end
CXX_C_OBJS = \
	$(gccobjdir)/attribs.o \
	$(gccobjdir)/c-common.o \
	$(gccobjdir)/c-format.o \
	$(gccobjdir)/c-pragma.o \
	$(gccobjdir)/c-semantics.o \
	$(gccobjdir)/c-lex.o \
	$(gccobjdir)/c-dump.o \
	$(gccobjdir)/c-pretty-print.o \
	$(gccobjdir)/c-opts.o \
	$(gccobjdir)/c-pch.o \
	$(gccobjdir)/c-incpath.o \
	$(gccobjdir)/cppdefault.o \
	$(gccobjdir)/c-ppoutput.o \
	$(gccobjdir)/c-cppbuiltin.o

# C++ specific files
C++OBJS = $(gccobjdir)/cp/call.o \
	$(gccobjdir)/cp/class.o \
	$(gccobjdir)/cp/cp-lang.o \
	$(gccobjdir)/cp/cvt.o \
	$(gccobjdir)/cp/cxx-pretty-print.o \
	$(gccobjdir)/cp/decl.o \
	$(gccobjdir)/cp/decl2.o \
	$(gccobjdir)/cp/dump.o \
	$(gccobjdir)/cp/error.o \
	$(gccobjdir)/cp/except.o \
	$(gccobjdir)/cp/expr.o \
	$(gccobjdir)/cp/friend.o \
	$(gccobjdir)/cp/init.o \
	$(gccobjdir)/cp/lex.o \
	$(gccobjdir)/cp/mangle.o \
	$(gccobjdir)/cp/method.o \
	$(gccobjdir)/cp/name-lookup.o \
	$(gccobjdir)/cp/optimize.o \
	$(gccobjdir)/cp/parser.o \
	$(gccobjdir)/cp/pt.o \
	$(gccobjdir)/cp/ptree.o \
	$(gccobjdir)/cp/repo.o \
	$(gccobjdir)/cp/rtti.o \
	$(gccobjdir)/cp/search.o \
	$(gccobjdir)/cp/semantics.o \
	$(gccobjdir)/cp/tree.o \
	$(gccobjdir)/cp/typeck.o \
	$(gccobjdir)/cp/typeck2.o


$(gcc_bin_dir)/cc1plus$(gexesfx): $(C++OBJS) $(BACKEND) $(CXX_C_OBJS) \
        $(gccobjdir)/c-common.o $(gccobjdir)/c-pragma.o $(RISCOS_OBJS) $(gccobjdir)/libcpp.a $(LIBDEPS)
	$(LINK) $(C++OBJS) $(BACKEND) $(CXX_C_OBJS) \
	$(RISCOS_OBJS) $(LIBDEPS) $(gccobjdir)/libcpp.a $(LIBS)

$(bin_dir)/g++$(gexesfx): $(gccobjdir)/cp/g++.o $(gccobjdir)/cp/g++spec.o \
	$(gccobjdir)/version.o $(RISCOS_OBJS) $(LIBDEPS)
	$(LINK) $(gccobjdir)/cp/g++.o $(gccobjdir)/cp/g++spec.o \
	$(gccobjdir)/version.o $(RISCOS_OBJS) $(LIBDEPS) $(LIBS)


##############################################################################


# Make Bison sources
# Bison sources
# There's no point having a suffix thing for these becuase
# they're all built differently.

cfns.h: cfns.gperf
	$(GPERF) -o -C -E -k 1-6,$ -j1 -D -N libc_name_p < cfns.gperf >$@

DRIVER_DEFINES = \
  -DSTANDARD_STARTFILE_PREFIX=\"$(prefix)$(gn_standard_startfile_prefix)\" \
  -DSTANDARD_EXEC_PREFIX=\"$(prefix)$(gn_standard_exec_prefix)\" \
  -DSTANDARD_LIBEXEC_PREFIX=\"$(libexecdir)/gcc/\" \
  -DDEFAULT_TARGET_VERSION=\"$(VERSION)\" \
  -DDEFAULT_TARGET_MACHINE=\"$(TARGET_NAME)\" \
  -DSTANDARD_BINDIR_PREFIX=\"$(prefix)/$(gn_standard_exec_prefix)\" \
  -DTOOLDIR_BASE_PREFIX=\"$(prefix)/$(gn_standard_exec_prefix)\"


$(gccobjdir)/cp/g++.o: ../gcc.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c ../gcc.c $(DRIVER_DEFINES)


# Dependencies
$(gccobjdir)/cp/call.o: call.c
$(gccobjdir)/cp/class.o: class.c
$(gccobjdir)/cp/cp-lang.o: cp-lang.c
$(gccobjdir)/cp/cvt.o: cvt.c
$(gccobjdir)/cp/decl.o: decl.c
$(gccobjdir)/cp/decl2.o: decl2.c
$(gccobjdir)/cp/dump.o: dump.c
$(gccobjdir)/cp/error.o: error.c
$(gccobjdir)/cp/except.o: except.c
$(gccobjdir)/cp/expr.o: expr.c
$(gccobjdir)/cp/friend.o: friend.c
$(gccobjdir)/cp/g++spec.o: g++spec.c
$(gccobjdir)/cp/init.o: init.c
$(gccobjdir)/cp/lex.o: lex.c
$(gccobjdir)/cp/mangle.o: mangle.c
$(gccobjdir)/cp/method.o: method.c
$(gccobjdir)/cp/optimize.o: optimize.c
$(gccobjdir)/cp/parser.o: parser.c
$(gccobjdir)/cp/pt.o: pt.c
$(gccobjdir)/cp/ptree.o: ptree.c
$(gccobjdir)/cp/repo.o: repo.c
$(gccobjdir)/cp/rtti.o: rtti.c
$(gccobjdir)/cp/search.o: search.c
$(gccobjdir)/cp/semantics.o: semantics.c
$(gccobjdir)/cp/tree.o: tree.c
$(gccobjdir)/cp/typeck.o: typeck.c
$(gccobjdir)/cp/typeck2.o: typeck2.c

