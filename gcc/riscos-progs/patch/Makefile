# Makefile for GNU patch
# Written by Nick Burrett <nick@dsvr.net>

INCLUDES = -I. -I ../../include
CROSS_CFLAGS += -DHAVE_CONFIG_H -Ded_PROGRAM=\"ed\"

$(objdir)/riscos-progs/patch/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

pobj = $(objdir)/riscos-progs/patch

OBJS = $(pobj)/addext.o \
	$(pobj)/argmatch.o \
	$(pobj)/backupfile.o \
	$(pobj)/basename.o \
	$(pobj)/error.o \
	$(pobj)/inp.o \
	$(pobj)/maketime.o \
	$(pobj)/partime.o \
	$(pobj)/patch.o \
	$(pobj)/pch.o \
	$(pobj)/quotearg.o \
	$(pobj)/quotesys.o \
	$(pobj)/util.o \
	$(pobj)/version.o \
	$(pobj)/xmalloc.o

all:	$(bin_dir)/patch$(exesfx)

$(bin_dir)/patch$(exesfx):	$(OBJS)
	$(CROSS_CC) -o $@ $(OBJS) -L$(objdir)/libiberty/$(STAGE) -liberty

$(pobj)/addext.o: addext.c
$(pobj)/argmatch.o: argmatch.c
$(pobj)/backupfile.o: backupfile.c
$(pobj)/basename.o: basename.c
$(pobj)/error.o: error.c
$(pobj)/inp.o: inp.c
$(pobj)/maketime.o: maketime.c
$(pobj)/partime.o: partime.c
$(pobj)/patch.o: patch.c
$(pobj)/pch.o: pch.c
$(pobj)/quotearg.o: quotearg.c
$(pobj)/quotesys.o: quotesys.c
$(pobj)/util.o: util.c
$(pobj)/version.o: version.c
$(pobj)/xmalloc.o: xmalloc.c
