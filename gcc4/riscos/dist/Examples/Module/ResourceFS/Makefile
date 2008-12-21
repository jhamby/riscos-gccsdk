# Copyright (C) 2006-2008 GCCSDK Developers
# This requires at least OSLib 7 (which supports ELF).

# Enable unit-at-a-time (yes/no) ?
UNITATATIME=no

# When using OSLib, DeskLib, etc, you have to specify their
# header include directories.  E.g. -IOSLib:
INCLUDE    = -IOSLib:

CC           = gcc
CCFLAGS      = -mthrowback -mmodule -O3 -std=c99 $(INCLUDE)
ASM          = $(CC)
ASMFLAGS     = $(CCFLAGS)
LINK         = gcc
LINKFLAGS    = -mmodule
CMHG         = cmunge
CMHGFLAGS    = -throwback -p -zbase -tgcc -apcs 3/32
MKRESFS      = mkresfs
MKRESFSFLAGS =
MKDIR        = cdir

# Here you need to specify extra libraries if you use them, like:
# OSLib:o.OSLib32
LIBS = 

# Add here all your C files :
MODCSRCS   = gccmodule.c
# Add here all your ASM files :
MODSSRCS   = 
# Add here your CMHG file :
MODCMHGSRC = module.cmhg
# Add here the top directory containing files and directory structure you
# want to register with ResourceFS:
MKRESFSDATA = data
MKRESFSSRC  = resfs.c

all: gccmodule

.PHONY: all

.SUFFIXES:
.SUFFIXES: .o .c .s .cmhg .h

.c.o:;	$(CC) $(CCFLAGS) -c -o $@ $<
.s.o:;	$(ASM) $(ASMFLAGS) -c -o $@ $<
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<

# Static dependencies:
$(MKRESFSSRC) $(MKRESFSSRC:.c=.h):
	$(MKRESFS) -o $(MKRESFSSRC) -p $(MKRESFSSRC:.c=.h) -v $(MKRESFSDATA)

$(MODCSRCS): $(MODCMHGSRC:.cmhg=.h) $(MKRESFSSRC:.c=.h)

ifeq ($(UNITATATIME),yes)
# One big compilation:
gccmodule: $(MODCSRCS) $(MODSSRCS) $(MODCMHGSRC:.cmhg=.o) $(MKRESFSSRC)
	$(CC) $(CCFLAGS) -o $@ $(MODCSRCS) $(MODSSRCS) $(MODCMHGSRC:.cmhg=.o) $(MKRESFSSRC) $(LIBS)
else
# Object-per-object compilation:
gccmodule: $(MODCSRCS:.c=.o) $(MODSSRCS:.s=.o) $(MODCMHGSRC:.cmhg=.o) $(MKRESFSSRC:.c=.o)
	$(LINK) $(LINKFLAGS) -o $@ $(MODCSRCS:.c=.o) $(MODSSRCS:.s=.o) $(MODCMHGSRC:.cmhg=.o) $(MKRESFSSRC:.c=.o) $(LIBS)
endif
