# Makefile for GNU sed
# Written by Nick Burrett <nick@sqrt.co.uk>

INCLUDES = -I. -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/sed/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

SRCS = compile.c execute.c fmt.c regex.c regexp.c sed.c strverscmp.c utils.c
HDRS = basicdefs.h config.h regex.h regex_internal.h sed.h \
	strverscmp.h utils.h

all:	$(bin_dir)/sed$(exesfx) $(info_dir)/sed

$(objdir)/riscos-progs/sed.o:	$(SRCS) $(HDRS)
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $(SRCS) $(INCLUDES)

$(bin_dir)/sed$(exesfx):	$(objdir)/riscos-progs/sed.o
	$(CROSS_CC) -o $@ $(objdir)/riscos-progs/sed.o -L$(objdir)/libiberty/$(STAGE) -liberty

$(info_dir)/sed:	doc/config.texi doc/sed.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections doc/sed.texi -o $@ -I doc
