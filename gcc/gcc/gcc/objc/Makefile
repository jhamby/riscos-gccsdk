# Makefile for the GNU Objective-C compiler for ARM/RISC OS
# Copyright (c) 1997, 1998, 1999, 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Defs for GCC
LINK = $(CC) -o $@
INCLUDES = -I./ -I../ -I../../../include -I../config
LIBS = -L$(objdir)/libiberty/$(STAGE) -liberty

$(gccobjdir)/objc/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES)

all: objc

.PHONY: objc

objc: $(gcc_bin_dir)/cc1obj$(gexesfx)

LIBDEPS = $(gccobjdir)/prefix.o

ifeq (${STAGE},stage1)
RISC_OBJS=
RISCOS_OBJS=
else
RISC_OBJS = $(gccobjdir)/config/arm/riscos_fix.o
RISCOS_OBJS = $(RISC_OBJS) $(gccobjdir)/config/arm/riscos.o
endif

OBJC_OBJS = $(gccobjdir)/objc/objc-parse.o \
	$(gccobjdir)/objc/objc-act.o \
	$(gccobjdir)/objc/objc-lang.o \
	$(C_AND_OBJC_OBJS)


# The real targets
$(gcc_bin_dir)/cc1obj$(gexesfx): $(BACKEND) $(OBJC_OBJS) $(RISCOS_OBJS) $(LIBDEPS)
	$(LINK) $(OBJC_OBJS) $(BACKEND) $(RISCOS_OBJS) $(LIBS)

##############################################################################

objc-parse.c: objc-parse.y
	bison objc-parse.y -o objc-parse.c

##############################################################################


# Dependencies
$(gccobjdir)/objc/objc-act.c: objc-act.c
$(gccobjdir)/objc/objc-parse.c: objc-parse.c objc-parse.y
