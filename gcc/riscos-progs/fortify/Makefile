# Makefile for fortify wrapper - memory checker
# Written by Peter Naulls <peter@chocky.org>

CROSS_CFLAGS += -DFORTIFY

$(objdir)/riscos-progs/fortify/%.o: %.c
	$(CROSS_CC) $(CROSS_CFLAGS) -o $@ -c $< $(INCLUDES)

SRCS = fortify.c memwrap.c

OBJS = $(addprefix $(objdir)/riscos-progs/fortify/, $(SRCS:.c=.o))

all:    $(ux_gcc_bin_dir)/$(TRG)/libfortify.o \
        $(ux_gcc_bin_dir)/$(TRG)/memory-edit

$(ux_gcc_bin_dir)/$(TRG)/libfortify.o:	$(OBJS)
	$(CROSS_AR) -c $@ $(OBJS)

$(ux_gcc_bin_dir)/$(TRG)/memory-edit: memory-edit
	sed s#OBJDIR#$(objdir)# < $< > $@

$(objdir)/riscos-progs/fortify/fortify.o: fortify.c ufortify.h
$(objdir)/riscos-progs/fortify/memwrap.o: memwrap.c

