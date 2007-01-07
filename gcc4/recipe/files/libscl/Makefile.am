# Automake style makefile for libscl (SharedCLibrary stub)
# Copyright (c) 2005, 2006, 2007 UnixLib developers
# Written by John Tytgat <John.Tytgat@aaug.net> based on the work done for
# UnixLib.

# Multilib support
MAKEOVERRIDES=

# These flags exclusively apply to a C compiler
WARNINGSC = -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs

WARNINGS = -pedantic -Wall -Wundef -Wpointer-arith \
	-Wcast-align -Wwrite-strings -Winline -Wno-unused \
	-Winline -Wno-unused -W -Wcast-qual -Wshadow $(WARNINGSC)

# The -isystem command tells GCC to prefer the header files in libscl,
# rather than trying to compile the library with the system headers.
# Header files that provide additional defintions only required for the
# building of libscl should be held in the source directories.
AM_CPPFLAGS = -isystem $(top_srcdir)/include

AM_CFLAGS = -std=c99

# Assembler files rely heavily on the C preprocessor to keep structures
# referred to by both languages in sync.
AM_CCASFLAGS = -xassembler-with-cpp -isystem $(top_srcdir)/include

includedir = $(prefix)
riscos_src = 	riscos/stubs.s

libc_headers = 	\
	include/assert.h \
	include/endian.h \
	include/fcntl.h \
	include/kernel.h \
	include/locale.h \
	include/memory.h \
	include/setjmp.h \
	include/stdarg.h \
	include/stdint.h \
	include/stdlib.h \
	include/strings.h \
 \
	include/sys/os.h \
	include/sys/stat.h \
	include/sys/times.h \
	include/sys/types.h \
 \
	include/unistd.h \
	include/ctype.h \
	include/errno.h \
	include/float.h \
	include/limits.h \
	include/math.h \
	include/pragmas.h \
	include/signal.h \
	include/stddef.h \
	include/stdio.h \
	include/string.h \
	include/swis.h \
	include/time.h \
	include/varargs.h

toolexeclib_LTLIBRARIES = libscl.la
libscl_la_SOURCES = $(riscos_src)

libscl_la_LDFLAGS = -Wc,-nostdlib -version-info 2:0:0

install-data-local: install-headers

install-headers:
	$(mkinstalldirs) $(DESTDIR)${tooldir}/include
	for dir in sys; do \
	  $(mkinstalldirs) $(DESTDIR)${tooldir}/include/$${dir} ; done
	for file in ${libc_headers}; do \
	  $(INSTALL_DATA) $(srcdir)/$${file} $(DESTDIR)${tooldir}/$${file} ; done
