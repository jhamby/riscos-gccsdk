# Makefile for GNU make - a build management tool
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005 GCCSDK Developers

INCLUDES = -I.
CROSS_CFLAGS += -DHAVE_CONFIG_H -DLOCALEDIR=\".\" -DINCLUDEDIR=\".\" \
                -DLIBDIR=\".\" -DGCC_IS_NATIVE

SRCS = ar.c arscan.c commands.c default.c dir.c expand.c \
	file.c function.c getloadavg.c hash.c implicit.c \
	job.c main.c misc.c read.c remake.c remote-stub.c \
	rule.c signame.c variable.c version.c vpath.c

HDRS = amiga.h commands.h config.h debug.h dep.h filedef.h \
	getopt.h gettext.h hash.h job.h make.h rule.h variable.h \
	vmsdir.h

all:	$(bin_dir)/make$(exesfx)

riscos-dist: $(info_dir)/make

$(objdir)/riscos-progs/make.o:	$(SRCS) $(HDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/make$(exesfx):	$(objdir)/riscos-progs/make.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/make.o -L$(objdir)/libiberty/$(STAGE) -liberty

$(info_dir)/make:	doc/fdl.texi doc/make-stds.texi doc/make.texi
	$(MAKEINFO) --no-split --number-sections doc/make -o $@ -I doc
