# Makefile for GNU Libio for the ARM/RISC OS GCC port
# Copyright (c) 1997-2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>
#
# This file is part of the GNU Compiler Collection
#

OPTS = -mthrowback -mpoke-function-name
#OPTS = -mthrowback -mno-apcs-frame -mno-apcs-stack-check -muse-arg-pointer

# Defs for GCC
CC = $(standard_exec_prefix)/gcc $(CFLAGS)
CFLAGS = -O2 $(OPTS) -DIN_GCC -DHAVE_CONFIG_H $(TARGET_DEFS)
INCLUDES = -I. -I../include

ioobjdir=$(objdir)/gcc-$(VERSION)/libio/$(TRG)

$(ioobjdir)/%.o: %.c
	$(CC) -o $@ -c $< $(INCLUDES) $(CFL)

$(ioobjdir)/%.o: %.cc
	$(CC) -o $@ -c $< $(INCLUDES) $(CFL)

IO_OBJECTS = $(ioobjdir)/filedoalloc.o \
	$(ioobjdir)/floatconv.o \
	$(ioobjdir)/genops.o \
	$(ioobjdir)/fileops.o \
	$(ioobjdir)/iovfprintf.o \
	$(ioobjdir)/iovfscanf.o \
	$(ioobjdir)/ioignore.o \
	$(ioobjdir)/iopadn.o \
	$(ioobjdir)/iofgetpos.o \
	$(ioobjdir)/iofread.o \
	$(ioobjdir)/iofscanf.o \
	$(ioobjdir)/iofsetpos.o \
	$(ioobjdir)/iogetdelim.o \
	$(ioobjdir)/iogetline.o \
	$(ioobjdir)/ioprintf.o \
	$(ioobjdir)/ioseekoff.o \
	$(ioobjdir)/ioseekpos.o \
	$(ioobjdir)/outfloat.o \
	$(ioobjdir)/strops.o \
	$(ioobjdir)/iofclose.o \
	$(ioobjdir)/iopopen.o \
	$(ioobjdir)/ioungetc.o \
	$(ioobjdir)/peekc.o \
	$(ioobjdir)/iogetc.o \
	$(ioobjdir)/ioputc.o \
	$(ioobjdir)/iofeof.o \
	$(ioobjdir)/ioferror.o

STDIO_OBJECTS = $(ioobjdir)/stdfiles.o

# These emulate stdio functionality, but with a different name (_IO_ungetc
# instead of ungetc), and using _IO_FILE instead of FILE.
# They are not needed for C++ iostream, nor stdio, though some stdio
# files are build using the same source files (see stdio/configure.in).
# They are needed for iostdio.h.  They are needed under Linux to avoid
# version incompatibility problems with the C library.
# iofclose.o is not here, because it is needed for stdio (by pclose).

STDIO_WRAP_OBJECTS =  $(ioobjdir)/iofdopen.o \
	$(ioobjdir)/iofflush.o \
	$(ioobjdir)/iofgets.o \
	$(ioobjdir)/iofopen.o \
	$(ioobjdir)/iofprintf.o \
	$(ioobjdir)/iofputs.o \
	$(ioobjdir)/iofwrite.o \
	$(ioobjdir)/iogets.o \
	$(ioobjdir)/ioperror.o \
	$(ioobjdir)/ioputs.o \
	$(ioobjdir)/ioscanf.o \
	$(ioobjdir)/iosetbuffer.o \
	$(ioobjdir)/iosetvbuf.o \
	$(ioobjdir)/iosprintf.o \
	$(ioobjdir)/iosscanf.o \
	$(ioobjdir)/ioftell.o \
	$(ioobjdir)/iovsprintf.o \
	$(ioobjdir)/iovsscanf.o

IOSTREAM_OBJECTS = $(ioobjdir)/builtinbuf.o \
	$(ioobjdir)/filebuf.o \
	$(ioobjdir)/fstream.o \
	$(ioobjdir)/indstream.o \
	$(ioobjdir)/ioassign.o \
	$(ioobjdir)/ioextend.o \
	$(ioobjdir)/iomanip.o \
	$(ioobjdir)/iostream.o \
	$(ioobjdir)/isgetline.o \
	$(ioobjdir)/isgetsb.o \
	$(ioobjdir)/isscan.o \
	$(ioobjdir)/osform.o \
	$(ioobjdir)/procbuf.o \
	$(ioobjdir)/sbform.o \
	$(ioobjdir)/sbgetline.o \
	$(ioobjdir)/sbscan.o \
	$(ioobjdir)/stdiostream.o \
	$(ioobjdir)/stdstrbufs.o \
	$(ioobjdir)/stdstreams.o \
	$(ioobjdir)/stream.o \
	$(ioobjdir)/streambuf.o \
	$(ioobjdir)/strstream.o \
	$(ioobjdir)/PlotFile.o \
	$(ioobjdir)/SFile.o \
	$(ioobjdir)/parsestream.o \
	$(ioobjdir)/pfstream.o \
	$(ioobjdir)/editbuf.o

# These files define _IO_read etc, which are just wrappers for read(2) etc.
# They need to be changed to use name-space-clean (e.g. __read) versions
# for each specific libc.
OSPRIM_OBJECTS = $(ioobjdir)/ioprims.o \
	$(ioobjdir)/iostrerror.o \
	$(ioobjdir)/cleanup.o

LIBIOSTREAM_OBJECTS = $(IO_OBJECTS) $(IOSTREAM_OBJECTS) $(OSPRIM_OBJECTS)
LIBIO_OBJECTS = $(IO_OBJECTS) $(STDIO_WRAP_OBJECTS) $(OSPRIM_OBJECTS) $(STDIO_OBJECTS)

all:	$(TBIN)/$(TRG)/libio.o $(TBIN)/$(TRG)/libiostream.o

$(TBIN)/$(TRG)/libio.o: $(LIBIO_OBJECTS)
	$(BIN)/libfile -c $@ $(LIBIO_OBJECTS)

$(TBIN)/$(TRG)/libiostream.o: $(LIBIOSTREAM_OBJECTS)
	$(BIN)/libfile -c $@ $(LIBIOSTREAM_OBJECTS)


$(ioobjdir)/cleanup.o: cleanup.c
$(ioobjdir)/filedoalloc.o: filedoalloc.c
$(ioobjdir)/fileops.o: fileops.c
$(ioobjdir)/floatconv.o: floatconv.c
$(ioobjdir)/genops.o: genops.c
$(ioobjdir)/iofclose.o: iofclose.c
$(ioobjdir)/iofdopen.o: iofdopen.c
$(ioobjdir)/iofeof.o: iofeof.c
$(ioobjdir)/ioferror.o: ioferror.c
$(ioobjdir)/iofflush.o: iofflush.c
$(ioobjdir)/iofflush_u.o: iofflush_u.c
$(ioobjdir)/iofgetpos.o: iofgetpos.c
$(ioobjdir)/iofgets.o: iofgets.c
$(ioobjdir)/iofopen.o: iofopen.c
$(ioobjdir)/iofprintf.o: iofprintf.c
$(ioobjdir)/iofputs.o: iofputs.c
$(ioobjdir)/iofread.o: iofread.c
$(ioobjdir)/iofscanf.o: iofscanf.c
$(ioobjdir)/iofsetpos.o: iofsetpos.c
$(ioobjdir)/ioftell.o: ioftell.c
$(ioobjdir)/iofwrite.o: iofwrite.c
$(ioobjdir)/iogetc.o: iogetc.c
$(ioobjdir)/iogetdelim.o: iogetdelim.c
$(ioobjdir)/iogetline.o: iogetline.c
$(ioobjdir)/iogets.o: iogets.c
$(ioobjdir)/ioignore.o: ioignore.c
$(ioobjdir)/iopadn.o: iopadn.c
$(ioobjdir)/ioperror.o: ioperror.c
$(ioobjdir)/iopopen.o: iopopen.c
$(ioobjdir)/ioprims.o: ioprims.c
$(ioobjdir)/ioprintf.o: ioprintf.c
$(ioobjdir)/ioputc.o: ioputc.c
$(ioobjdir)/ioputs.o: ioputs.c
$(ioobjdir)/ioscanf.o: ioscanf.c
$(ioobjdir)/ioseekoff.o: ioseekoff.c
$(ioobjdir)/ioseekpos.o: ioseekpos.c
$(ioobjdir)/iosetbuffer.o: iosetbuffer.c
$(ioobjdir)/iosetvbuf.o: iosetvbuf.c
$(ioobjdir)/iosprintf.o: iosprintf.c
$(ioobjdir)/iosscanf.o: iosscanf.c
$(ioobjdir)/iostrerror.o: iostrerror.c
$(ioobjdir)/ioungetc.o: ioungetc.c
$(ioobjdir)/iovfprintf.o: iovfprintf.c
$(ioobjdir)/iovfscanf.o: iovfscanf.c
$(ioobjdir)/iovsprintf.o: iovsprintf.c
$(ioobjdir)/iovsscanf.o: iovsscanf.c
$(ioobjdir)/outfloat.o: outfloat.c
$(ioobjdir)/peekc.o: peekc.c
$(ioobjdir)/stdfiles.o: stdfiles.c
$(ioobjdir)/strops.o: strops.c

$(ioobjdir)/builtinbuf.o: builtinbuf.cc
$(ioobjdir)/editbuf.o: editbuf.cc
$(ioobjdir)/filebuf.o: filebuf.cc
$(ioobjdir)/fstream.o: fstream.cc
$(ioobjdir)/indstream.o: indstream.cc
$(ioobjdir)/ioassign.o: ioassign.cc
$(ioobjdir)/ioextend.o: ioextend.cc
$(ioobjdir)/iomanip.o: iomanip.cc
$(ioobjdir)/iostream.o: iostream.cc
$(ioobjdir)/isgetline.o: isgetline.cc
$(ioobjdir)/isgetsb.o: isgetsb.cc
$(ioobjdir)/isscan.o: isscan.cc
$(ioobjdir)/osform.o: osform.cc
$(ioobjdir)/parsestream.o: parsestream.cc
$(ioobjdir)/pfstream.o: pfstream.cc
$(ioobjdir)/PlotFile.o: PlotFile.cc
$(ioobjdir)/procbuf.o: procbuf.cc
$(ioobjdir)/sbform.o: sbform.cc
$(ioobjdir)/sbgetline.o: sbgetline.cc
$(ioobjdir)/sbscan.o: sbscan.cc
$(ioobjdir)/SFile.o: SFile.cc
$(ioobjdir)/stdiostream.o: stdiostream.cc
$(ioobjdir)/stdstrbufs.o: stdstrbufs.cc
$(ioobjdir)/stdstreams.o: stdstreams.cc
$(ioobjdir)/stream.o: stream.cc
$(ioobjdir)/streambuf.o: streambuf.cc
$(ioobjdir)/strstream.o: strstream.cc

