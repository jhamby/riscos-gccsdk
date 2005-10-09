# Makefile for GNU gperf - a perfect hash generator
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005 GCCSDK Developers

INCLUDES = -I. -I../../include -I../../gcc/libstdc/include
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/gperf/%.o: %.cc
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = 	$(objdir)/riscos-progs/gperf/bool-array.o \
	$(objdir)/riscos-progs/gperf/hash-table.o \
	$(objdir)/riscos-progs/gperf/hash.o \
	$(objdir)/riscos-progs/gperf/getline.o \
	$(objdir)/riscos-progs/gperf/input.o \
	$(objdir)/riscos-progs/gperf/keyword.o \
	$(objdir)/riscos-progs/gperf/keyword-list.o \
	$(objdir)/riscos-progs/gperf/main.o \
	$(objdir)/riscos-progs/gperf/options.o \
	$(objdir)/riscos-progs/gperf/output.o \
	$(objdir)/riscos-progs/gperf/positions.o \
	$(objdir)/riscos-progs/gperf/search.o \
	$(objdir)/riscos-progs/gperf/version.o

all:	$(bin_dir)/gperf$(exesfx)

riscos-dist: $(info_dir)/gperf

$(bin_dir)/gperf$(exesfx):	$(OBJS)
	$(CROSS_CXX) -o $@ $(OBJS) -L$(objdir)/libiberty/stage2 -liberty

$(info_dir)/gperf:	doc/gperf.texi
	$(MAKEINFO) --no-split --number-sections doc/gperf -o $@ -I doc

$(objdir)/riscos-progs/gperf/bool-array.o: bool-array.cc bool-array.icc options.icc
$(objdir)/riscos-progs/gperf/getline.o: getline.cc getline.h
$(objdir)/riscos-progs/gperf/hash.o: hash.cc
$(objdir)/riscos-progs/gperf/hash-table.o: hash-table.cc options.icc
$(objdir)/riscos-progs/gperf/input.o: input.cc
$(objdir)/riscos-progs/gperf/keyword.o: keyword.cc
$(objdir)/riscos-progs/gperf/keyword-list.o: keyword-list.cc
$(objdir)/riscos-progs/gperf/main.o: main.cc
$(objdir)/riscos-progs/gperf/options.o: options.cc
$(objdir)/riscos-progs/gperf/output.o: output.cc
$(objdir)/riscos-progs/gperf/positions.o: positions.cc
$(objdir)/riscos-progs/gperf/search.o: search.cc
$(objdir)/riscos-progs/gperf/version.o: version.cc
