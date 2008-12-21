# Copyright (C) 2005-2008 GCCSDK Developers
# This requires at least OSLib 7 (which supports ELF).

# Option: enable unit-at-a-time (yes/no) ?
UNITATATIME=no

# When using OSLib, DeskLib, etc, you have to specify their
# header include directories.  E.g. -IOSLib:
INCLUDE    = -IOSLib:

CC          = gcc
CCFLAGS     = -mthrowback -mmodule -O3 -std=c99 $(INCLUDE)
ASM         = $(CC)
ASMFLAGS    = $(CCFLAGS)
LINK        = gcc
LINKFLAGS   = -mmodule
CMHG        = cmunge
CMHGFLAGS   = -throwback -p -zbase -tgcc -apcs 3/32
MKDIR       = cdir

# Here you need to specify extra libraries if you use them, like:
# "-LOSLib: -lOSLib32" which specifies to link with the library "libOSLib32/a"
# to be found in the directory "OSLib:".
LIBS = -LOSLib: -lOSLib32

# Add here all your C files :
MODCSRCS   = gccmodule.c
# Add here all your ASM files :
MODSSRCS   = 
# Add here your CMHG file :
MODCMHGSRC = module.cmhg

all: gccmodule

.PHONY: all

.SUFFIXES:
.SUFFIXES: .o .c .s .cmhg .h

.c.o:;	$(CC) $(CCFLAGS) -c -o $@ $<
.s.o:;	$(ASM) $(ASMFLAGS) -c -o $@ $<
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

$(MODCSRCS): $(MODCMHGSRC:.cmhg=.h)

ifeq ($(UNITATATIME),yes)
# One big compilation:
gccmodule: $(MODCSRCS) $(MODSSRCS) $(MODCMHGSRC:.cmhg=.h) $(MODCMHGSRC:.cmhg=.o)
	$(CC) $(CCFLAGS) -o $@ $(MODCSRCS) $(MODSSRCS) $(MODCMHGSRC:.cmhg=.o) $(LIBS)
else
# Object-per-object compilation:
gccmodule: $(MODCSRCS:.c=.o) $(MODSSRCS:.s=.o) $(MODCMHGSRC:.cmhg=.h) $(MODCMHGSRC:.cmhg=.o)
	$(LINK) $(LINKFLAGS) -o $@ $(MODCSRCS:.c=.o) $(MODSSRCS:.s=.o) $(MODCMHGSRC:.cmhg=.o) $(LIBS)
endif
