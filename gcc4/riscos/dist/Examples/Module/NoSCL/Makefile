# Shows how to create a module using GCCSDK which does *not* make use of
# the SharedCLibrary, which means you can't use the standard C calls.
#
# Compared to the usual set of options to create modules, add the '-znoscl'
# option to CMunge and give as extra linker options '-nostdlib -lgcc'.

CC          = gcc
CCFLAGS     = -mthrowback -mmodule -O2 -std=c99
ASM         = $(CC)
ASMFLAGS    = $(CCFLAGS)
LINK        = gcc
LINKFLAGS   = -nostdlib -lgcc -mmodule
CMHG        = cmunge
CMHGFLAGS   = -tgcc -32bit -zbase -znoscl

OBJS = gccmodule.o module.o

gccmodule: $(OBJS)
	$(LINK) $(LINKFLAGS) $(OBJS) -o $@

.SUFFIXES:
.SUFFIXES: .c .o .h .cmhg

.c.o:;	$(CC) $(CCFLAGS) -c -o $@ $<

# CMunge invocation rule:
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

# Dependencies:
gccmodule.c: module.h
