# Probably the smallest example of how to create a module using GCCSDK.

CC          = gcc
CCFLAGS     = -mthrowback -mmodule -O3
ASM         = $(CC)
ASMFLAGS    = $(CCFLAGS)
LINK        = gcc
LINKFLAGS   = -mmodule
CMHG        = cmunge
CMHGFLAGS   =  -tgcc -32bit

gccmodule: gccmodule.o module.o
	$(LINK) $(LINKFLAGS) $< -o $@

.c.o:;	$(CC) $(CCFLAGS) -c -o $@ $<
# CMunge invocation rule:
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

# Dependencies:
gccmodule.c: module.h
