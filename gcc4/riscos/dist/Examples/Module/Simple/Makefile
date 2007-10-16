# Probably the smallest example of how to create a module using GCCSDK.

CC          = gcc
CCFLAGS     = -mthrowback -mmodule -O3
ASM         = $(CC)
ASMFLAGS    = $(CCFLAGS)
LINK        = gcc
LINKFLAGS   = -mmodule
CMHG        = cmunge
CMHGFLAGS   = -tgcc -32bit

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
