# Makefile for GNU sed
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I. -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H

$(objdir)/riscos-progs/sed/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

OBJS = $(objdir)/riscos-progs/sed/compile.o \
	$(objdir)/riscos-progs/sed/execute.o \
	$(objdir)/riscos-progs/sed/fmt.o \
	$(objdir)/riscos-progs/sed/regex.o \
	$(objdir)/riscos-progs/sed/regexp.o \
	$(objdir)/riscos-progs/sed/sed.o \
	$(objdir)/riscos-progs/sed/strverscmp.o \
	$(objdir)/riscos-progs/sed/utils.o

all:	$(bin_dir)/sed$(exesfx) $(info_dir)/sed

$(bin_dir)/sed$(exesfx):	$(OBJS)
	$(CROSS_CC) -o $@ $(OBJS) -L$(objdir)/libiberty/$(STAGE) -liberty

$(info_dir)/sed:	doc/config.texi doc/sed.texi doc/version.texi
	$(MAKEINFO) --no-split --number-sections doc/sed.texi -o $@ -I doc

$(objdir)/riscos-progs/sed/compile.o: compile.c
$(objdir)/riscos-progs/sed/execute.o: execute.c
$(objdir)/riscos-progs/sed/fmt.o: fmt.c
$(objdir)/riscos-progs/sed/regex.o: regex.c regex.h
$(objdir)/riscos-progs/sed/regexp.o: regexp.c
$(objdir)/riscos-progs/sed/sed.o: sed.c sed.h
$(objdir)/riscos-progs/sed/strverscmp.o: strverscmp.c strverscmp.h
$(objdir)/riscos-progs/sed/utils.o: utils.c utils.h
