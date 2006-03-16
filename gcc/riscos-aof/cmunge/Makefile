# Makefile for "A free alternative to CMHG" - CMunge
# Copyright (c) 2005-2006 GCCSDK Developers

ifeq (${STAGE},stage1)
prefix = $(unix_prefix)
CFLAGS += -DCROSS_COMPILE
exesfx =
else
# In stage2, we compile & link with libscl.  Do not change this unless you're
# sure there aren't any licence issues between CMunge and UnixLib.
prefix = $(riscos_prefix)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS) -mlibscl
endif

INCLUDES = -I. -I../../
CFLAGS += -Wall

cmungeobjdir = $(objdir)/riscos-aof/cmunge/$(STAGE)

all:	$(bin_dir)/cmunge$(exesfx)

riscos-dist:
	mkdir -p $(info_dir)/CMunge
	cp -p Docs/Authors $(info_dir)/CMunge
	cp -p Docs/Copyright $(info_dir)/CMunge
	cp -p Docs/History $(info_dir)/CMunge
	cp -p Docs/Manual $(info_dir)/CMunge
	cp -p Docs/Warranty $(info_dir)/CMunge

SRCS =	apcscli.c \
	assemble.c \
	blank.c \
	comments.c \
	datestamp.c \
	error.c \
	format.c \
	filename.c \
	gfile.c \
	main.c \
	mem.c \
	options.c \
	readfile.c \
	str.c \
	system.c \
	throwback.c \
	writeexport.c \
	writefile.c \
	writeheader.c

HDRS =	../../sdk-config.h \
	apcscli.h\
	assemble.h \
	blank.h \
	comments.h \
	copyright.h \
	datestamp.h \
	error.h \
	filename.h \
	format.h \
	gfile.h \
	mem.h \
	options.h \
	readfile.h \
	str.h \
	system.h \
	throwback.h \
	VersionNum \
	writeexport.h \
	writefile.h \
	writeheader.h

ifeq (${STAGE},stage1)
# This funky line tells make to change all '.c' suffixes to '.o' and add
# '$(cmungeobjdir)/' to each object.
OBJS = $(addprefix $(cmungeobjdir)/, $(SRCS:.c=.o))

$(cmungeobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) \
	-DGCC_BIN_DIR=\"$(prefix)$(gn_standard_exec_prefix)\"

$(bin_dir)/cmunge$(exesfx): $(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS)
endif

ifeq (${STAGE},stage2)
# Since we know that we are compiling with GCC 3.4, we can compile the
# application using inter-module analysis, effectivly building as if all
# functions are contained within one source file.

$(cmungeobjdir)/cmunge.o:	$(SRCS) $(DEPS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS) $(INCLUDES) \
	-DGCC_BIN_DIR=\"\"

$(bin_dir)/cmunge$(exesfx): $(cmungeobjdir)/cmunge.o $(DEPS)
	$(CC) -o $@ -mlibscl $(cmungeobjdir)/cmunge.o
endif
