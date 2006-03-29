# Makefile for GNU patch
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005, 2006 GCCSDK Developers

INCLUDES = -I. -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H -Ded_PROGRAM=\"ed\"

SRCS = addext.c argmatch.c backupfile.c basename.c error.c inp.c \
	maketime.c partime.c patch.c pch.c quotearg.c quotesys.c \
	util.c version.c xmalloc.c

HDRS = argmatch.h backupfile.h basename.h common.h config.h \
	error.h inp.h maketime.h partime.h patchlevel.h \
	pch.h quotearg.h quotesys.h util.h version.h xalloc.h

all:	$(bin_dir)/patch$(exesfx)

$(objdir)/riscos-progs/patch.o:	$(SRCS) $(HDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/patch$(exesfx):	$(objdir)/riscos-progs/patch.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/patch.o
