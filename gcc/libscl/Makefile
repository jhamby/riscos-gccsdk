# Makefile for the SharedCLibrary stubs
# Copyright (c) 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the RISC OS GCC SDK
#
CC = $(standard_exec_prefix)/gcc $(CFLAGS)
CFLAGS = -O2 $(OPTS) $(TARGET_DEFS) -mlibscl
INCLUDES = -idirafter .

$(objdir)/libscl/$(TRG)/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES) $(CFL)

$(objdir)/libscl/$(TRG)/%.o: %.s
	$(CC) -o $@ -c $<

LIBC = $(objdir)/libscl/$(TRG)/unistd.o \
	$(objdir)/libscl/$(TRG)/stubs.o \
	$(objdir)/libscl/$(TRG)/uname.o

	#$(objdir)/libscl/$(TRG)/fcntl.o

all:	mkdirs $(TBIN)/$(TRG)/libscl.o

.PHONY: mkdirs

mkdirs:
	mkdir -p $(objdir)/libscl/$(TRG) $(TBIN)/$(TRG)

install_headers:
	mkdir -p $(cross_include_dir)/libscl $(cross_include_dir)/libscl/sys
	cp *.h $(cross_include_dir)/libscl
	cp sys/*.h $(cross_include_dir)/libscl/sys

$(TBIN)/$(TRG)/libscl.o:	$(LIBC)
	$(standard_exec_prefix)/libfile -c $@ $(LIBC)

$(objdir)/libscl/$(TRG)/unistd.o: unistd.c unistd.h
$(objdir)/libscl/$(TRG)/fcntl.o: fcntl.c fcntl.h
$(objdir)/libscl/$(TRG)/stubs.o: stubs.s
$(objdir)/libscl/$(TRG)/uname.o: uname.c

