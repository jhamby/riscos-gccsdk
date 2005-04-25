# Makefile for GNU patch
# Written by Nick Burrett <nick@sqrt.co.uk>

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
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/patch.o -L$(objdir)/libiberty/$(STAGE) -liberty
