# Makefile for GNU sed
# Copyright (c) 2000, 2001, 2002 Nick Burrett
# Copyright (c) 2003, 2004, 2005 GCCSDK Developers

INCLUDES = -I. -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/sed/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

SRCS = compile.c execute.c fmt.c regex.c regexp.c sed.c strverscmp.c utils.c
HDRS = basicdefs.h config.h regex.h regex_internal.h sed.h \
	strverscmp.h utils.h

all:	$(bin_dir)/sed$(exesfx)

riscos-dist: $(info_dir)/sed,faf

$(objdir)/riscos-progs/sed.o:	$(SRCS) $(HDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/sed$(exesfx):	$(objdir)/riscos-progs/sed.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/sed.o -L$(objdir)/libiberty/$(STAGE) -liberty

$(info_dir)/sed,faf:	doc/config.texi doc/sed.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections --html doc/sed.texi -o $@ -I doc
