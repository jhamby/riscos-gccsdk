@ Chunk 2 stub routines for linking with the SharedCLibrary
@ Copyright (c) 1997-2005 Nick Burrett
@ Copyright (c) 2005-2010 UnixLib Developers
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

	DefSCLFnc	TrapHandler
	DefSCLFnc	UncaughtTrapHandler
	DefSCLFnc	EventHandler
	DefSCLFnc	UnhandledEventHandler
	.global	__rt_stkovf_split_small
	.global	x$stack_overflow
__rt_stkovf_split_small:
x$stack_overflow:		MOV	PC, #0
	.global	__rt_stkovf_split_big
	.global	x$stack_overflow_1
__rt_stkovf_split_big:
x$stack_overflow_1:		MOV	PC, #0
	.global	__rt_udiv
	.global	x$udivide
__rt_udiv:
x$udivide:			MOV	PC, #0
	.global	x$uremainder
x$uremainder:			MOV	PC, #0
	.global	__rt_sdiv
	.global	x$divide
__rt_sdiv:
x$divide:			MOV	PC, #0
	.global	__rt_divtest
	.global	x$divtest
__rt_divtest:
x$divtest:			MOV	PC, #0
	.global	x$remainder
x$remainder:			MOV	PC, #0
	.global	x$multiply
x$multiply:			MOV	PC, #0
	.global	__rt_rd1chk
	.global	_rd1chk
__rt_rd1chk:
_rd1chk:			MOV	PC, #0
	.global	__rt_rd2chk
	.global	_rd2chk
__rt_rd2chk:
_rd2chk:			MOV	PC, #0
	.global	__rt_rd4chk
	.global	_rd4chk
__rt_rd4chk:
_rd4chk:			MOV	PC, #0
	.global	__rt_wR1chk
	.global	_wR1chk
__rt_wR1chk:
_wR1chk:			MOV	PC, #0
	.global	__rt_wR2chk
	.global	_wR2chk
__rt_wR2chk:
_wR2chk:			MOV	PC, #0
	.global	__rt_wR4chk
	.global	_wR4chk
__rt_wR4chk:
_wR4chk:			MOV	PC, #0
	.global	_main
_main:				MOV	PC, #0
	.global	_exit
_exit:				MOV	PC, #0
	.global	_clib_initialise
_clib_initialise:		MOV	PC, #0
	.global	_backtrace
_backtrace:			MOV	PC, #0
	.global	_count
_count:				MOV	PC, #0
	.global	_count1
_count1:			MOV	PC, #0
	.global	_stfp
_stfp:				MOV	PC, #0
	.global	_ldfp
_ldfp:				MOV	PC, #0
	.global	_printf
_printf:			MOV	PC, #0
	.global	_fprintf
_fprintf:			MOV	PC, #0
	.global	_sprintf
_sprintf:			MOV	PC, #0
	DefSCLFnc	clock
	DefSCLFnc	difftime
	DefSCLFnc	mktime
	DefSCLFnc	time
	DefSCLFnc	asctime
	DefSCLFnc	ctime
	DefSCLFnc	gmtime
	DefSCLFnc	localtime
	DefSCLFnc	strftime
	DefSCLFnc	memcpy
	DefSCLFnc	memmove
	DefSCLFnc	strcpy
	DefSCLFnc	strncpy
	DefSCLFnc	strcat
	DefSCLFnc	strncat
	DefSCLFnc	memcmp	@ FIXME: define bcmp alias ?
	DefSCLFnc	strcmp
	DefSCLFnc	strncmp
	DefSCLFnc	memchr
	DefSCLFnc	strchr
	DefSCLFnc	strcspn
	DefSCLFnc	strpbrk
	DefSCLFnc	strrchr
	DefSCLFnc	strspn
	DefSCLFnc	strstr
	DefSCLFnc	strtok
	DefSCLFnc	memset
	DefSCLFnc	strerror
	DefSCLFnc	strlen
	DefSCLFnc	atof
	DefSCLFnc	atoi
	DefSCLFnc	atol
	DefSCLFnc	strtod
	DefSCLFnc	strtol
	DefSCLFnc	strtoul
	DefSCLFnc	rand
	DefSCLFnc	srand
	DefSCLFnc	calloc
	DefSCLFnc	free
	DefSCLFnc	malloc
	DefSCLFnc	realloc
	DefSCLFnc	abort
	DefSCLFnc	atexit
	DefSCLFnc	exit
	DefSCLFnc	getenv
	DefSCLFnc	system
	DefSCLFnc	bsearch
	DefSCLFnc	qsort
	DefSCLFnc	abs
	DefSCLFnc	div
	DefSCLFnc	labs
	DefSCLFnc	ldiv
	DefSCLFnc	remove
	DefSCLFnc	rename
	DefSCLFnc	tmpfile
	.global	__old_tmpnam
__old_tmpnam:			MOV	PC, #0
	DefSCLFnc	fclose
	DefSCLFnc	fflush
	DefSCLFnc	fopen
	DefSCLFnc	freopen
	DefSCLFnc	setbuf
	DefSCLFnc	setvbuf
	DefSCLFnc	printf
	DefSCLFnc	fprintf
	DefSCLFnc	sprintf
	DefSCLFnc	scanf
	DefSCLFnc	fscanf
	DefSCLFnc	sscanf
	.global	__gcc_vprintf
	.global	vprintf
__gcc_vprintf:
vprintf:			MOV	PC, #0
	.global	__gcc_vfprintf
	.global	vfprintf
__gcc_vfprintf:
vfprintf:			MOV	PC, #0
	.global	__gcc_vsprintf
	.global	vsprintf
__gcc_vsprintf:
vsprintf:			MOV	PC, #0
	.global	_vfprintf
_vfprintf:			MOV	PC, #0
	DefSCLFnc	fgetc
	DefSCLFnc	fgets
	DefSCLFnc	fputc
	DefSCLFnc	fputs
	.global	__filbuf
__filbuf:			MOV	PC, #0
	DefSCLFnc	getc
	DefSCLFnc	getchar
	DefSCLFnc	gets
	.global	__flsbuf
__flsbuf:			MOV	PC, #0
	DefSCLFnc	putc
	DefSCLFnc	putchar
	DefSCLFnc	puts
	DefSCLFnc	ungetc
	DefSCLFnc	fread
	DefSCLFnc	fwrite
	DefSCLFnc	fgetpos

	DefSCLGbl	fseeko
	DefSCLFnc	fseek

	DefSCLFnc	fsetpos

	DefSCLGbl	ftello
	DefSCLFnc	ftell

	DefSCLFnc	rewind
	DefSCLFnc	clearerr
	DefSCLFnc	feof
	DefSCLFnc	ferror
	DefSCLFnc	perror
	DefSCLFnc	__ignore_signal_handler
	DefSCLFnc	__error_signal_marker
	DefSCLFnc	__default_signal_handler
	DefSCLFnc	signal
	DefSCLFnc	raise

	@ Entry
	@ int setjmp (jmp_buf __state);
	.global	__libscl_redirected_setjmp
__libscl_redirected_setjmp:
	MOV	PC, #0

	@ Entry
	@ void longjmp (jmp_buf __state, int __value);
	.global	__libscl_redirected_longjmp
__libscl_redirected_longjmp:
	MOV	PC, #0

	DefSCLFnc	acos
	DefSCLFnc	asin
	DefSCLFnc	atan
	DefSCLFnc	atan2
	DefSCLFnc	cos
	DefSCLFnc	sin
	DefSCLFnc	tan
	DefSCLFnc	cosh
	DefSCLFnc	sinh
	DefSCLFnc	tanh
	DefSCLFnc	exp
	DefSCLFnc	frexp
	DefSCLFnc	ldexp
	DefSCLFnc	log
	DefSCLFnc	log10
	DefSCLFnc	modf
	DefSCLFnc	pow
	DefSCLFnc	sqrt
	DefSCLFnc	ceil
	DefSCLFnc	fabs
	DefSCLFnc	floor
	DefSCLFnc	fmod
	DefSCLFnc	setlocale
	DefSCLFnc	isalnum
	DefSCLFnc	isalpha
	DefSCLFnc	iscntrl
	DefSCLFnc	isdigit
	DefSCLFnc	isgraph
	DefSCLFnc	islower
	DefSCLFnc	isprint
	DefSCLFnc	ispunct
	DefSCLFnc	isspace
	DefSCLFnc	isupper
	DefSCLFnc	isxdigit
	DefSCLFnc	tolower
	DefSCLFnc	toupper
	.global	__assert
__assert:			MOV	PC, #0
	.global	_memcpy
_memcpy:			MOV	PC, #0
	.global	_memset
_memset:			MOV	PC, #0
	DefSCLFnc	localeconv
	DefSCLFnc	mblen
	DefSCLFnc	mbtowc
	DefSCLFnc	wctomb
	DefSCLFnc	mbstowcs
	DefSCLFnc	wcstombs
	DefSCLFnc	strxfrm
	DefSCLFnc	strcoll
	.global	_clib_finalisemodule
_clib_finalisemodule:		MOV	PC, #0

	@ Entry 180
	@ const char *_clib_version(void);
	.global	_clib_version
_clib_version:
	MOV	PC, #0

	.global	_Clib_Finalise
_Clib_Finalise:
	MOV	PC, #0

	@ Entry 182
	DefSCLFnc	tmpnam

	@ Entry 183
	@ int _swi(int swi_number, unsigned int flags, ...);
	.global	_swi
_swi:
	MOV	PC, #0

	@ Entry 184
	@ _kernel_oserror *_swix(int swi_number, unsigned int mask, ...);
	.global	_swix
_swix:
	MOV	PC, #0

clib_vectors_end:
	.space	clib_vectors_end - clib_vectors_begin
	.size	clib_vectors, . - clib_vectors

	.global	__SIG_IGN
.set	__SIG_IGN, 0xfffffffd
	.global	__SIG_ERR
.set	__SIG_ERR, 0xfffffffe
	.global	__SIG_DFL
.set	__SIG_DFL, 0xffffffff

	.section .bss.riscos.libscl.chunkstub.id02,"w",%nobits
clib_statics_begin:
	.space 0xb48
clib_statics_end:
	.global	errno
.set	errno, clib_statics_begin + 0x0000
	.global	__errno
.set	__errno, clib_statics_begin + 0x0000
	@ stdin  at clib_statics_begin + 0x0004
	@ stdout at clib_statics_begin + 0x002c
	@ stderr at clib_statics_begin + 0x0054
	.global	__iob
.set	__iob, clib_statics_begin + 0x0004
	.global	_interrupts_off
.set	_interrupts_off, clib_statics_begin + 0x0284
.set	_stub__saved_interrupt, clib_statics_begin + 0x0288
.set	_stub_ctype_eof, clib_statics_begin + 0x028c
	.global	__ctype
.set	__ctype, clib_statics_begin + 0x0290
	.global	__huge_val
.set	__huge_val, clib_statics_begin + 0x0390
.set	_stub_app_space_end, clib_statics_begin + 0x0398
.set	_stub_stack_o_flag, clib_statics_begin + 0x069c
.set	_stub_inSignalHandler, clib_statics_begin + 0x039d
.set	_stub_dummybyte, clib_statics_begin + 0x039e
.set	_stub_, clib_statics_begin + 0x039f
.set	_stub_ClibSpace, clib_statics_begin + 0x03a0

	.end

