# Makefile for GNU gperf - a perfect hash generator
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I. -I../../include -I../../gcc-2_95_2/libio
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/gperf/%.o: %.cc
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = 	$(objdir)/riscos-progs/gperf/bool-array.o \
	$(objdir)/riscos-progs/gperf/gen-perf.o \
	$(objdir)/riscos-progs/gperf/hash.o \
	$(objdir)/riscos-progs/gperf/hash-table.o \
	$(objdir)/riscos-progs/gperf/iterator.o \
	$(objdir)/riscos-progs/gperf/key-list.o \
	$(objdir)/riscos-progs/gperf/list-node.o \
	$(objdir)/riscos-progs/gperf/main.o \
	$(objdir)/riscos-progs/gperf/new.o \
	$(objdir)/riscos-progs/gperf/options.o \
	$(objdir)/riscos-progs/gperf/read-line.o \
	$(objdir)/riscos-progs/gperf/trace.o \
	$(objdir)/riscos-progs/gperf/vectors.o \
	$(objdir)/riscos-progs/gperf/version.o

all:	$(bin_dir)/gperf$(exesfx)

$(bin_dir)/gperf$(exesfx):	$(OBJS)
	$(CROSS_CC) -o $@ $(OBJS) -L$(objdir)/libiberty/stage2 -liberty -lio

$(objdir)/riscos-progs/gperf/bool-array.o: bool-array.cc icc/bool-array icc/options
$(objdir)/riscos-progs/gperf/gen-perf.o: gen-perf.cc icc/options icc/read-line icc/bool-array
$(objdir)/riscos-progs/gperf/hash.o: hash.cc
$(objdir)/riscos-progs/gperf/hash-table.o: hash-table.cc icc/options
$(objdir)/riscos-progs/gperf/iterator.o: iterator.cc
$(objdir)/riscos-progs/gperf/key-list.o: key-list.cc
$(objdir)/riscos-progs/gperf/list-node.o: list-node.cc
$(objdir)/riscos-progs/gperf/main.o: main.cc
$(objdir)/riscos-progs/gperf/new.o: new.cc
$(objdir)/riscos-progs/gperf/options.o: options.cc
$(objdir)/riscos-progs/gperf/read-line.o: read-line.cc icc/read-line
$(objdir)/riscos-progs/gperf/trace.o: trace.cc
$(objdir)/riscos-progs/gperf/vectors.o: vectors.cc
$(objdir)/riscos-progs/gperf/version.o: version.cc
