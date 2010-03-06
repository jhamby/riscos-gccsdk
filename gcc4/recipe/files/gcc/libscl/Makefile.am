# Automake style makefile for libscl (SharedCLibrary stub)
# Copyright (c) 2005, 2006, 2007, 2008 UnixLib developers
# Written by John Tytgat <John.Tytgat@aaug.net> based on the work done for
# UnixLib.

# Directory where headers will be installed.
tooldir = $(exec_prefix)/$(target_alias)

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
# Header files that provide additional definitions only required for the
# building of libscl should be held in the source directories.
AM_CPPFLAGS = -isystem $(top_srcdir)/include -I $(top_srcdir)/incl-local

AM_CFLAGS = -std=c99 -mlibscl

# Assembler files rely heavily on the C preprocessor to keep structures
# referred to by both languages in sync.
AM_CCASFLAGS = -xassembler-with-cpp -mlibscl -isystem $(top_srcdir)/include -I $(top_srcdir)/incl-local

includedir = $(prefix)

gcccompat_src = \
	gcccompat/_b_frame.s gcccompat/_b_return.s gcccompat/galloca.c \
	gcccompat/_galloca.s
# FIXME: math_src: not done yet
riscos_src = \
	riscos/chunkid01.s riscos/chunkid02.s riscos/chunkid04.s \
	riscos/chunkid05.s riscos/chunkstub.s \
	riscos/redirect_setjmp.s
stdio_src = \
	stdio/fdopen.c stdio/fileno.c
# FIXME: stdlib_src: not done yet
string_src = \
	string/strdup.c \
	string/stricmp.c \
	string/strnicmp.c
# FIXME: time_src: not done yet

# All files in the 'include' directory.
libc_headers = 	\
	alloca.h \
	assert.h \
	endian.h \
	fcntl.h \
	kernel.h \
	locale.h \
	memory.h \
	setjmp.h \
	stdarg.h \
	stdint.h \
	stdlib.h \
	strings.h \
 \
	sys/elf.h \
	sys/os.h \
	sys/stat.h \
	sys/times.h \
	sys/types.h \
 \
	unistd.h \
	ctype.h \
	errno.h \
	float.h \
	limits.h \
	math.h \
	pragmas.h \
	signal.h \
	stddef.h \
	stdio.h \
	string.h \
	swis.h \
	time.h \
	varargs.h

toolexeclib_LIBRARIES = libscl.a
libscl_a_SOURCES = $(gcccompat_src) $(riscos_src) $(stdio_src) $(string_src)

toolexeclib_DATA = crt0-scl.o

crt0-scl.o: crt/crt0-scl.s
	$(CCASCOMPILE) -o crt0-scl.o -c `test -f 'crt/crt0-scl.s' || echo '$(srcdir)/'`crt/crt0-scl.s

install-data-local: install-headers

install-headers:
	$(mkinstalldirs) $(DESTDIR)${tooldir}/include/libscl
	for dir in sys; do \
	  $(mkinstalldirs) $(DESTDIR)${tooldir}/include/libscl/$${dir} ; done
	for file in ${libc_headers}; do \
	  $(INSTALL_DATA) $(srcdir)/include/$${file} $(DESTDIR)${tooldir}/include/libscl/$${file} ; done
