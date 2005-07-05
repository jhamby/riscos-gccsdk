# Makefile for "A free alternative to CMHG" - CMunge
# Copyright (c) 2005 GCCSDK Developers

ifeq (${STAGE},stage1)
prefix = $(unix_prefix)
CFLAGS += -DCROSS_COMPILE
exesfx =
else
prefix = $(riscos_prefix)
CC = $(CROSS_CC)
CFLAGS = $(CROSS_CFLAGS)
endif

INCLUDES = -I. -I../../
CFLAGS += -Wall

cmungeobjdir = $(objdir)/riscos-aof/cmunge/$(STAGE)

all:	$(bin_dir)/cmunge$(exesfx)

SRCS =	assemble.c \
	blank.c \
	comments.c \
	datestamp.c \
	error.c \
	format.c \
	gfile.c \
	main.c \
	mem.c \
	options.c \
	readfile.c \
	str.c \
	throwback.c \
	writeexport.c \
	writefile.c \
	writeheader.c

HDRS =	../../sdk-config.h \
	assemble.h \
	blank.h \
	comments.h \
	copyright.h \
	datestamp.h \
	error.h \
	format.h \
	gfile.h \
	mem.h \
	options.h \
	readfile.h \
	str.h \
	throwback.h \
	writeexport.h \
	writefile.h \
	writeheader.h

ifeq (${STAGE},stage1)
# This funky line tells make to change all '.c' suffixes to '.o' and add
# '$(cmungeobjdir)/' to each object.
OBJS = $(addprefix $(cmungeobjdir)/, $(SRCS:.c=.o))

$(cmungeobjdir)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDES) \
	-DGCC_BIN_DIR=\"$(prefix)$(gn_gcc_bin_dir)\"

$(bin_dir)/cmunge$(exesfx): $(OBJS) $(DEPS)
	$(CC) -o $@ $(OBJS)
endif

ifeq (${STAGE},stage2)
# Since we know that we are compiling with GCC 3.4, we can compile the
# application using inter-module analysis, effectivly building as if all
# functions are contained within one source file.
	DEPS += $(gcc_bin_dir)/$(TRG)/o/unixlib

$(cmungeobjdir)/cmunge.o:	$(SRCS) $(DEPS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS) $(INCLUDES) \
	-DGCC_BIN_DIR=\"$(prefix)$(gn_gcc_bin_dir)\"

$(bin_dir)/cmunge$(exesfx): $(cmungeobjdir)/cmunge.o $(DEPS)
	$(CC) -o $@ $(cmungeobjdir)/cmunge.o
	# In order to comply with the CMunge license when distributing modified copies:
	echo -e The following changes were made by the GCCSDK developers against the official CMunge version v`grep Module_MajorVersion_CMHG VersionNum | tac -s ' ' | head -n 1` :\\n\\n > Docs/GCCSDKModifications
	-cvs -z9 -q diff -u -r CMunge-`grep Module_MajorVersion_CMHG VersionNum | tac -s ' ' | head -n 1 | sed "s/\./_/"` >> Docs/GCCSDKModifications
endif
