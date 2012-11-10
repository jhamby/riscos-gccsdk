@ Chunk 2 stub routines for linking with the SharedCLibrary
@ Copyright (c) 1997-2005 Nick Burrett
@ Copyright (c) 2005-2011 UnixLib Developers
@ All rights reserved.

@ Redistribution and use in source and binary forms, with or without
@ modification, are permitted provided that the following conditions
@ are met:
@ 1. Redistributions of source code must retain the above copyright
@    notice, this list of conditions and the following disclaimer.
@ 2. Redistributions in binary form must reproduce the above copyright
@    notice, this list of conditions and the following disclaimer in the
@    documentation and/or other materials provided with the distribution.
@ 3. The name of the author may not be used to endorse or promote products
@    derived from this software without specific prior written permission.
@
@ THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
@ IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
@ OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
@ IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
@ INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
@ NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
@ DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
@ THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
@ THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

@ Only suited for APCS-32 linking (SharedCLibrary 5) with code compiled
@ with GCCSDK 4.x.

#include "internal/scl-macros.s"

	.section .riscos.libscl.chunkstub.id02,"ax",%progbits
	@ Chunk 2 is a requirement and always needed.
	.word	2			@ CLib
	MakePtr	clib_vectors_begin
	MakePtr	clib_vectors_end
	MakePtr	clib_statics_begin
	MakePtr	clib_statics_end

	.text
clib_vectors:
clib_vectors_begin:

	@ Entry 0
	DefSCLFncS	TrapHandler

	@ Entry 1
	DefSCLFncS	UncaughtTrapHandler

	@ Entry 2
	DefSCLFncS	EventHandler

	@ Entry 3
	DefSCLFncS	UnhandledEventHandler

	@ Entry 4
	DefSCLFncS	__rt_stkovf_split_small
	.global		x$stack_overflow
x$stack_overflow = __rt_stkovf_split_small

	@ Entry 5
	DefSCLFncS	__rt_stkovf_split_big
	.global		x$stack_overflow_1
x$stack_overflow_1 = __rt_stkovf_split_big

	@ Entry 6
	DefSCLFncS	__rt_udiv
	.global		x$udivide
x$udivide = __rt_udiv

	@ Entry 7
	DefSCLFncS	x$uremainder

	@ Entry 8
	DefSCLFncS	__rt_sdiv
	.global		x$divide
x$divide = __rt_sdiv

	@ Entry 9
	DefSCLFncS	__rt_divtest
	.global		x$divtest
x$divtest = __rt_divtest

	@ Entry 10
	DefSCLFncS	x$remainder

	@ Entry 11
	DefSCLFncS	x$multiply

	@ Entry 12
	DefSCLFncS	__rt_rd1chk
	.global		_rd1chk
_rd1chk = __rt_rd1chk

	@ Entry 13
	DefSCLFncS	__rt_rd2chk
	.global		_rd2chk
_rd2chk = __rt_rd2chk

	@ Entry 14
	DefSCLFncS	__rt_rd4chk
	.global		_rd4chk
_rd4chk = __rt_rd4chk

	@ Entry 15
	DefSCLFncS	__rt_wR1chk
	.global		_wR1chk
_wR1chk = __rt_wR1chk

	@ Entry 16
	DefSCLFncS	__rt_wR2chk
	.global		_wR2chk
_wR2chk = __rt_wR2chk

	@ Entry 17
	DefSCLFncS	__rt_wR4chk
	.global		_wR4chk
_wR4chk = __rt_wR4chk

	@ Entry 18
	DefSCLFncS	_main

	@ Entry 19
	DefSCLFncS	_exit

	@ Entry 20
	DefSCLFncS	_clib_initialise

	@ Entry 21
	DefSCLFncS	_backtrace

	@ Entry 22
	DefSCLFncS	_count

	@ Entry 23
	DefSCLFncS	_count1

	@ Entry 24
	DefSCLFncS	_stfp

	@ Entry 25
	DefSCLFncS	_ldfp

	@ Entry 26
	DefSCLFncS	_printf

	@ Entry 27
	DefSCLFncS	_fprintf

	@ Entry 28
	DefSCLFncS	_sprintf

	@ Entry 29
	DefSCLFnc	clock

	@ Entry 30
	DefSCLFnc	difftime

	@ Entry 31
	DefSCLFnc	mktime

	@ Entry 32
	DefSCLFnc	time

	@ Entry 33
	DefSCLFnc	asctime

	@ Entry 34
	DefSCLFnc	ctime

	@ Entry 35
	DefSCLFnc	gmtime

	@ Entry 36
	DefSCLFnc	localtime

	@ Entry 37
	DefSCLFnc	strftime

	@ Entry 38
	DefSCLFnc	memcpy

	@ Entry 39
	DefSCLFnc	memmove

	@ Entry 40
	DefSCLFnc	strcpy

	@ Entry 41
	DefSCLFnc	strncpy

	@ Entry 42
	DefSCLFnc	strcat

	@ Entry 43
	DefSCLFnc	strncat

	@ Entry 44
	@ int memcmp (const void *s1, const void *s2, size_t n);
	DefSCLFnc	memcmp
	@ int bcmp (const void *s1, const void *s2, size_t n);
	DefSCLFncAlias	bcmp memcmp

	@ Entry 45
	DefSCLFnc	strcmp

	@ Entry 46
	DefSCLFnc	strncmp

	@ Entry 47
	DefSCLFnc	memchr

	@ Entry 48
	DefSCLFnc	strchr

	@ Entry 49
	DefSCLFnc	strcspn

	@ Entry 50
	DefSCLFnc	strpbrk

	@ Entry 51
	DefSCLFnc	strrchr

	@ Entry 52
	DefSCLFnc	strspn

	@ Entry 53
	DefSCLFnc	strstr

	@ Entry 54
	DefSCLFnc	strtok

	@ Entry 55
	DefSCLFnc	memset

	@ Entry 56
	@ char *strerror(int errnum);
	@ We have our own strerror() implementation which accepts more errno
	@ values than the SCL does.  Export this stub entry as different
	@ function.
	DefSCLFncS	__scl_org_strerror

	@ Entry 57
	DefSCLFnc	strlen

	@ Entry 58
	DefSCLFnc	atof

	@ Entry 59
	DefSCLFnc	atoi

	@ Entry 60
	DefSCLFnc	atol

	@ Entry 61
	@ double strtod (const char * restrict string,
	@                char ** restrict endptr);
	DefSCLFnc	strtod

	@ Entry 62
	@ long int strtol (const char * restrict string,
	@                  char ** restrict endptr,
	@                  int base);
	DefSCLFnc	strtol

	@ Entry 63
	@ unsigned long int strtoul (const char * restrict string,
	@                            char ** restrict endptr,
	@                            int base);
	DefSCLFnc	strtoul

	@ Entry 64
	DefSCLFnc	rand

	@ Entry 65
	DefSCLFnc	srand

	@ Entry 66
	DefSCLFnc	calloc

	@ Entry 67
	DefSCLFnc	free

	@ Entry 68
	DefSCLFnc	malloc

	@ Entry 69
	DefSCLFnc	realloc

	@ Entry 70
	DefSCLFnc	abort

	@ Entry 71
	DefSCLFnc	atexit

	@ Entry 72
	DefSCLFnc	exit

	@ Entry 73
	DefSCLFnc	getenv

	@ Entry 74
	DefSCLFnc	system

	@ Entry 75
	DefSCLFnc	bsearch

	@ Entry 76
	DefSCLFnc	qsort

	@ Entry 77
	DefSCLFnc	abs

	@ Entry 78
	DefSCLFnc	div

	@ Entry 79
	DefSCLFnc	labs

	@ Entry 80
	DefSCLFnc	ldiv

	@ Entry 81
	DefSCLFnc	remove
	DefSCLFncAlias	unlink remove

	@ Entry 82
	DefSCLFnc	rename

	@ Entry 83
	DefSCLFnc	tmpfile

	@ Entry 84
	DefSCLFncS	__old_tmpnam

	@ Entry 85
	DefSCLFnc	fclose

	@ Entry 86
	DefSCLFnc	fflush

	@ Entry 87
	DefSCLFnc	fopen

	@ Entry 88
	DefSCLFnc	freopen

	@ Entry 89
	DefSCLFnc	setbuf

	@ Entry 90
	DefSCLFnc	setvbuf

	@ Entry 91
	DefSCLFnc	printf

	@ Entry 92
	DefSCLFnc	fprintf

	@ Entry 93
	DefSCLFnc	sprintf

	@ Entry 94
	DefSCLFnc	scanf

	@ Entry 95
	DefSCLFnc	fscanf

	@ Entry 96
	DefSCLFnc	sscanf

	@ Entry 97
	DefSCLFnc	vprintf
	.global		__gcc_vprintf
__gcc_vprintf = vprintf

	@ Entry 98
	DefSCLFnc	vfprintf
	.global		__gcc_vfprintf
__gcc_vfprintf = vfprintf

	@ Entry 99
	DefSCLFnc	vsprintf
	.global		__gcc_vsprintf
__gcc_vsprintf = vsprintf

	@ Entry 100
	DefSCLFncS	_vfprintf

	@ Entry 101
	DefSCLFnc	fgetc

	@ Entry 102
	DefSCLFnc	fgets

	@ Entry 103
	DefSCLFnc	fputc

	@ Entry 104
	DefSCLFnc	fputs

	@ Entry 105
	DefSCLFncS	__filbuf

	@ Entry 106
	DefSCLFnc	getc

	@ Entry 107
	DefSCLFnc	getchar

	@ Entry 108
	DefSCLFnc	gets

	@ Entry 109
	DefSCLFncS	__flsbuf

	@ Entry 110
	DefSCLFnc	putc

	@ Entry 111
	DefSCLFnc	putchar

	@ Entry 112
	DefSCLFnc	puts

	@ Entry 113
	DefSCLFnc	ungetc

	@ Entry 114
	DefSCLFnc	fread

	@ Entry 115
	DefSCLFnc	fwrite

	@ Entry 116
	DefSCLFnc	fgetpos

	@ Entry 117
	@ int fseek (FILE *stream, long offset, int whence);
	DefSCLFnc	fseek
	@ int fseeko (FILE *stream, off_t offset, int whence);
	@ without #define _FILE_OFFSET_BITS 64
	DefSCLFncAlias	fseeko fseek

	@ Entry 118
	DefSCLFnc	fsetpos

	@ Entry 119
	@ long ftell (FILE *stream);
	DefSCLFnc	ftell
	@ off_t ftello (FILE *stream);
	@ without #define _FILE_OFFSET_BITS 64
	DefSCLFncAlias	ftello ftell

	@ Entry 120
	DefSCLFnc	rewind

	@ Entry 121
	DefSCLFnc	clearerr

	@ Entry 122
	DefSCLFnc	feof

	@ Entry 123
	DefSCLFnc	ferror

	@ Entry 124
	DefSCLFnc	perror

	@ Entry 125
	DefSCLFncS	__ignore_signal_handler

	@ Entry 126
	DefSCLFncS	__error_signal_marker

	@ Entry 127
	DefSCLFncS	__default_signal_handler

	@ Entry 128
	DefSCLFnc	signal

	@ Entry 129
	DefSCLFnc	raise

	@ Entry 130
	@ int setjmp (jmp_buf __state);
	DefSCLFncS	__scl_redirected_setjmp

	@ Entry 131
	@ void longjmp (jmp_buf __state, int __value);
	DefSCLFncS	__scl_redirected_longjmp

	@ Entry 132
	@ double acos (double x);
	DefSCLFnc	acos
	@ long double acosl(long double x);
	DefSCLFncAlias	acosl acos

	@ Entry 133
	@ double asin (double x);
	DefSCLFnc	asin
	@ long double asinl(long double x);
	DefSCLFncAlias	asinl asin

	@ Entry 134
	@ double atan (double x);
	DefSCLFnc	atan
	@ long double atanl(long double x);
	DefSCLFncAlias	atanl atan

	@ Entry 135
	@ double atan2 (double y, double x);
	DefSCLFnc	atan2
	@ long double atan2l(long double y, long double x);
	DefSCLFncAlias	atan2l atan2

	@ Entry 136
	@ double cos (double x);
	DefSCLFnc	cos
	@ long double cosl (long double x);
	DefSCLFncAlias	cosl cos

	@ Entry 137
	@ double sin (double x);
	DefSCLFnc	sin
	@ long double sinl (long double x);
	DefSCLFncAlias	sinl sin

	@ Entry 138
	@ double tan (double x);
	DefSCLFnc	tan
	@ long double tanl (long double x);
	DefSCLFncAlias	tanl tan

	@ Entry 139
	@ double cosh (double x);
	DefSCLFnc	cosh
	@ long double coshl (long double x);
	DefSCLFncAlias	coshl cosh

	@ Entry 140
	@ double sinh (double x);
	DefSCLFnc	sinh
	@ long double sinhl (long double x);
	DefSCLFncAlias	sinhl sinh

	@ Entry 141
	@ double tanh (double x);
	DefSCLFnc	tanh
	@ long double tanhl (long double x);
	DefSCLFncAlias	tanhl tanh

	@ Entry 142
	@ double exp (double x);
	DefSCLFnc	exp
	@ long double expl (long double x);
	DefSCLFncAlias	expl exp

	@ Entry 143
	@ double frexp (double x, int *exp);
	DefSCLFnc	frexp
	@ long double frexpl (long double x, int *exp);
	DefSCLFncAlias	frexpl frexp

	@ Entry 144
	@ double ldexp (double x, int exp);
	DefSCLFnc	ldexp
	@ long double ldexpl (long double x, int exp);
	DefSCLFncAlias	ldexpl ldexp

	@ Entry 145
	@ double log (double x);
	DefSCLFnc	log
	@ long double logl (long double x);
	DefSCLFncAlias	logl log

	@ Entry 146
	@ double log10 (double x);
	DefSCLFnc	log10
	@ long double log10l (long double x);
	DefSCLFncAlias	log10l log10

	@ Entry 147
	@ double modf (double x, double *iptr);
	DefSCLFnc	modf
	@ long double modfl (long double x, long double *iptr);
	DefSCLFncAlias	modfl modf

	@ Entry 148
	@ double pow (double x, double y);
	DefSCLFnc	pow
	@ long double powl (long double x, long double y);
	DefSCLFncAlias	powl pow

	@ Entry 149
	@ double sqrt (double x);
	DefSCLFnc	sqrt
	@ long double sqrtl (long double x);
	DefSCLFncAlias	sqrtl sqrt

	@ Entry 150
	@ double ceil (double x);
	DefSCLFnc	ceil
	@ long double ceill (long double x);
	DefSCLFncAlias	ceill ceil

	@ Entry 151
	@ double fabs (double x);
	DefSCLFnc	fabs
	@ long double fabsl (long double x);
	DefSCLFncAlias	fabsl fabs

	@ Entry 152
	@ double floor (double x);
	DefSCLFnc	floor
	@ long double floorl (long double x);
	DefSCLFncAlias	floorl floor

	@ Entry 153
	@ double fmod (double x, double y);
	DefSCLFnc	fmod
	@ long double fmodl (long double x, long double y);
	DefSCLFncAlias	fmodl fmod

	@ Entry 154
	DefSCLFnc	setlocale

	@ Entry 155
	DefSCLFnc	isalnum

	@ Entry 156
	DefSCLFnc	isalpha

	@ Entry 157
	DefSCLFnc	iscntrl

	@ Entry 158
	DefSCLFnc	isdigit

	@ Entry 159
	DefSCLFnc	isgraph

	@ Entry 160
	DefSCLFnc	islower

	@ Entry 161
	DefSCLFnc	isprint

	@ Entry 162
	DefSCLFnc	ispunct

	@ Entry 163
	DefSCLFnc	isspace

	@ Entry 164
	DefSCLFnc	isupper

	@ Entry 165
	DefSCLFnc	isxdigit

	@ Entry 166
	DefSCLFnc	tolower

	@ Entry 167
	DefSCLFnc	toupper

	@ Entry 168
	DefSCLFncS	__assert

	@ Entry 169
	DefSCLFncS	_memcpy

	@ Entry 170
	DefSCLFncS	_memset

	@ Entry 171
	DefSCLFnc	localeconv

	@ Entry 172
	DefSCLFnc	mblen

	@ Entry 173
	DefSCLFnc	mbtowc

	@ Entry 174
	DefSCLFnc	wctomb

	@ Entry 175
	DefSCLFnc	mbstowcs

	@ Entry 176
	DefSCLFnc	wcstombs

	@ Entry 177
	DefSCLFnc	strxfrm

	@ Entry 178
	DefSCLFnc	strcoll

	@ Entry 179
	DefSCLFncS	_clib_finalisemodule

	@ Entry 180
	@ const char *_clib_version (void);
	DefSCLFncS	_clib_version

	@ Entry 181
	DefSCLFncS	_Clib_Finalise

	@ Entry 182
	DefSCLFnc	tmpnam

	@ Entry 183
	@ int _swi (int swi_number, unsigned int flags, ...);
	DefSCLFncS	_swi

	@ Entry 184
	@ _kernel_oserror *_swix (int swi_number, unsigned int mask, ...);
	DefSCLFncS	_swix

clib_vectors_end:
	.space	clib_vectors_end - clib_vectors_begin
	.size	clib_vectors, . - clib_vectors

	.global		__SIG_IGN
.set	__SIG_IGN, 0xfffffffd
	.global		__SIG_ERR
.set	__SIG_ERR, 0xfffffffe
	.global		__SIG_DFL
.set	__SIG_DFL, 0xffffffff

	.section .bss.riscos.libscl.chunkstub.id02,"w",%nobits
	.align 2
clib_statics_begin:
	.space 0xb48
clib_statics_end:
	.global		errno
.set	errno, clib_statics_begin + 0x0000
	.global		__errno
.set	__errno, clib_statics_begin + 0x0000
	@ stdin  at clib_statics_begin + 0x0004
	@ stdout at clib_statics_begin + 0x002c
	@ stderr at clib_statics_begin + 0x0054
	.global		__iob
.set	__iob, clib_statics_begin + 0x0004
	.global		_interrupts_off
.set	_interrupts_off, clib_statics_begin + 0x0284
.set	_stub__saved_interrupt, clib_statics_begin + 0x0288
.set	_stub_ctype_eof, clib_statics_begin + 0x028c
	.global		__ctype
.set	__ctype, clib_statics_begin + 0x0290
	.global		__huge_val
.set	__huge_val, clib_statics_begin + 0x0390
.set	_stub_app_space_end, clib_statics_begin + 0x0398
.set	_stub_stack_o_flag, clib_statics_begin + 0x069c
.set	_stub_inSignalHandler, clib_statics_begin + 0x039d
.set	_stub_dummybyte, clib_statics_begin + 0x039e
.set	_stub_, clib_statics_begin + 0x039f
.set	_stub_ClibSpace, clib_statics_begin + 0x03a0

	.end

