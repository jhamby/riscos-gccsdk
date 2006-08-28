# Makefile for Fortify wrapper - memory checker
# Written by Peter Naulls <peter@chocky.org>
# Copyright (c) 2005, 2006 GCCSDK Developers

CROSS_CFLAGS += -DFORTIFY

$(objdir)/riscos-progs/fortify/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

SRCS = fortify.c memwrap.c

OBJS = $(addprefix $(objdir)/riscos-progs/fortify/, $(SRCS:.c=.o))

all:    $(ux_gcc_bin_dir)/$(TRG)/libfortify.o \
        $(ux_gcc_bin_dir)/$(TRG)/memory-edit-fortify

$(ux_gcc_bin_dir)/$(TRG)/libfortify.o:	$(OBJS)
	$(CROSS_AR) -c $@ $(OBJS)

$(ux_gcc_bin_dir)/$(TRG)/memory-edit-fortify: gen-memory-edit $(OBJS) $(ux_gcc_bin_dir)/$(TRG)/unixlib.o
	./gen-memory-edit $(ux_gcc_bin_dir)/$(TRG)/unixlib.o $(objdir)/riscos-progs/fortify/fortify.o > $@

$(objdir)/riscos-progs/fortify/fortify.o: fortify.c ufortify.h
$(objdir)/riscos-progs/fortify/memwrap.o: memwrap.c
