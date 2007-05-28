@ Stub routines for linking with the SharedCLibrary
@ Copyright (c) 1997-2005 Nick Burrett
@ Copyright (c) 2005-2007 UnixLib Developers
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

	.macro	MakePtr ptrname
#ifdef __TARGET_MODULE__
	.word	\ptrname(GOTOFF)
#else
	.word	\ptrname
#endif
	.endm

	.section .riscos.libscl.chunkstub.id02,"ax",%progbits
	@ Chunk 2 is a requirement and always needed.
	.word	2			@ lib chunk id : CLib
	MakePtr	clib_vectors_begin
	MakePtr	clib_vectors_end
	MakePtr	clib_statics_begin
	MakePtr	clib_statics_end

	.text
clib_vectors_begin:

	.global	TrapHandler
TrapHandler:			MOV	PC, #0
	.global	UncaughtTrapHandler
UncaughtTrapHandler:		MOV	PC, #0
	.global	EventHandler
EventHandler:			MOV	PC, #0
	.global	UnhandledEventHandler
UnhandledEventHandler:		MOV	PC, #0
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
	.global	clock
clock:				MOV	PC, #0
	.global	difftime
difftime:			MOV	PC, #0
	.global	mktime
mktime:				MOV	PC, #0
	.global	time
time:				MOV	PC, #0
	.global	asctime
asctime:			MOV	PC, #0
	.global	ctime
ctime:				MOV	PC, #0
	.global	gmtime
gmtime:				MOV	PC, #0
	.global	localtime
localtime:			MOV	PC, #0
	.global	strftime
strftime:			MOV	PC, #0
	.global	memcpy
memcpy:				MOV	PC, #0
	.global	memmove
memmove:			MOV	PC, #0
	.global	strcpy
strcpy:				MOV	PC, #0
	.global	strncpy
strncpy:			MOV	PC, #0
	.global	strcat
strcat:				MOV	PC, #0
	.global	strncat
strncat:			MOV	PC, #0
	.global	memcmp
memcmp:				MOV	PC, #0
	.global	strcmp
strcmp:				MOV	PC, #0
	.global	strncmp
strncmp:			MOV	PC, #0
	.global	memchr
memchr:				MOV	PC, #0
	.global	strchr
strchr:				MOV	PC, #0
	.global	strcspn
strcspn:			MOV	PC, #0
	.global	strpbrk
strpbrk:			MOV	PC, #0
	.global	strrchr
strrchr:			MOV	PC, #0
	.global	strspn
strspn:				MOV	PC, #0
	.global	strstr
strstr:				MOV	PC, #0
	.global	strtok
strtok:				MOV	PC, #0
	.global	memset
memset:				MOV	PC, #0
	.global	strerror
strerror:			MOV	PC, #0
	.global	strlen
strlen:				MOV	PC, #0
	.global	atof
atof:				MOV	PC, #0
	.global	atoi
atoi:				MOV	PC, #0
	.global	atol
atol:				MOV	PC, #0
	.global	strtod
strtod:				MOV	PC, #0
	.global	strtol
strtol:				MOV	PC, #0
	.global	strtoul
strtoul:			MOV	PC, #0
	.global	rand
rand:				MOV	PC, #0
	.global	srand
srand:				MOV	PC, #0
	.global	calloc
calloc:				MOV	PC, #0
	.global	free
free:				MOV	PC, #0
	.global	malloc
malloc:				MOV	PC, #0
	.global	realloc
realloc:			MOV	PC, #0
	.global	abort
abort:				MOV	PC, #0
	.global	atexit
atexit:				MOV	PC, #0
	.global	exit
exit:				MOV	PC, #0
	.global	getenv
getenv:				MOV	PC, #0
	.global	system
system:				MOV	PC, #0
	.global	bsearch
bsearch:			MOV	PC, #0
	.global	qsort
qsort:				MOV	PC, #0
	.global	abs
abs:				MOV	PC, #0
	.global	div
div:				MOV	PC, #0
	.global	labs
labs:				MOV	PC, #0
	.global	ldiv
ldiv:				MOV	PC, #0
	.global	remove
remove:				MOV	PC, #0
	.global	rename
rename:				MOV	PC, #0
	.global	tmpfile
tmpfile:			MOV	PC, #0
	.global	__old_tmpnam
__old_tmpnam:			MOV	PC, #0
	.global	fclose
fclose:				MOV	PC, #0
	.global	fflush
fflush:				MOV	PC, #0
	.global	fopen
fopen:				MOV	PC, #0
	.global	freopen
freopen:			MOV	PC, #0
	.global	setbuf
setbuf:				MOV	PC, #0
	.global	setvbuf
setvbuf:			MOV	PC, #0
	.global	printf
printf:				MOV	PC, #0
	.global	fprintf
fprintf:			MOV	PC, #0
	.global	sprintf
sprintf:			MOV	PC, #0
	.global	scanf
scanf:				MOV	PC, #0
	.global	fscanf
fscanf:				MOV	PC, #0
	.global	sscanf
sscanf:				MOV	PC, #0
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
	.global	fgetc
fgetc:				MOV	PC, #0
	.global	fgets
fgets:				MOV	PC, #0
	.global	fputc
fputc:				MOV	PC, #0
	.global	fputs
fputs:				MOV	PC, #0
	.global	__filbuf
__filbuf:			MOV	PC, #0
	.global	getc
getc:				MOV	PC, #0
	.global	getchar
getchar:			MOV	PC, #0
	.global	gets
gets:				MOV	PC, #0
	.global	__flsbuf
__flsbuf:			MOV	PC, #0
	.global	putc
putc:				MOV	PC, #0
	.global	putchar
putchar:			MOV	PC, #0
	.global	puts
puts:				MOV	PC, #0
	.global	ungetc
ungetc:				MOV	PC, #0
	.global	fread
fread:				MOV	PC, #0
	.global	fwrite
fwrite:				MOV	PC, #0
	.global	fgetpos
fgetpos:			MOV	PC, #0
	.global	fseek
fseek:				MOV	PC, #0
	.global	fsetpos
fsetpos:			MOV	PC, #0
	.global	ftell
ftell:				MOV	PC, #0
	.global	rewind
rewind:				MOV	PC, #0
	.global	clearerr
clearerr:			MOV	PC, #0
	.global	feof
feof:				MOV	PC, #0
	.global	ferror
ferror:				MOV	PC, #0
	.global	perror
perror:				MOV	PC, #0
	.global	__ignore_signal_handler
__ignore_signal_handler:	MOV	PC, #0
	.global	__error_signal_marker
__error_signal_marker:		MOV	PC, #0
	.global	__default_signal_handler
__default_signal_handler:	MOV	PC, #0
	.global	signal
signal:				MOV	PC, #0
	.global	raise
raise:				MOV	PC, #0
	.global	setjmp
setjmp:				MOV	PC, #0
	.global	longjmp
longjmp:			MOV	PC, #0
	.global	acos
acos:				MOV	PC, #0
	.global	asin
asin:				MOV	PC, #0
	.global	atan
atan:				MOV	PC, #0
	.global	atan2
atan2:				MOV	PC, #0
	.global	cos
cos:				MOV	PC, #0
	.global	sin
sin:				MOV	PC, #0
	.global	tan
tan:				MOV	PC, #0
	.global	cosh
cosh:				MOV	PC, #0
	.global	sinh
sinh:				MOV	PC, #0
	.global	tanh
tanh:				MOV	PC, #0
	.global	exp
exp:				MOV	PC, #0
	.global	frexp
frexp:				MOV	PC, #0
	.global	ldexp
ldexp:				MOV	PC, #0
	.global	log
log:				MOV	PC, #0
	.global	log10
log10:				MOV	PC, #0
	.global	modf
modf:				MOV	PC, #0
	.global	pow
pow:				MOV	PC, #0
	.global	sqrt
sqrt:				MOV	PC, #0
	.global	ceil
ceil:				MOV	PC, #0
	.global	fabs
fabs:				MOV	PC, #0
	.global	floor
floor:				MOV	PC, #0
	.global	fmod
fmod:				MOV	PC, #0
	.global	setlocale
setlocale:			MOV	PC, #0
	.global	isalnum
isalnum:			MOV	PC, #0
	.global	isalpha
isalpha:			MOV	PC, #0
	.global	iscntrl
iscntrl:			MOV	PC, #0
	.global	isdigit
isdigit:			MOV	PC, #0
	.global	isgraph
isgraph:			MOV	PC, #0
	.global	islower
islower:			MOV	PC, #0
	.global	isprint
isprint:			MOV	PC, #0
	.global	ispunct
ispunct:			MOV	PC, #0
	.global	isspace
isspace:			MOV	PC, #0
	.global	isupper
isupper:			MOV	PC, #0
	.global	isxdigit
isxdigit:			MOV	PC, #0
	.global	tolower
tolower:			MOV	PC, #0
	.global	toupper
toupper:			MOV	PC, #0
	.global	__assert
__assert:			MOV	PC, #0
	.global	_memcpy
_memcpy:			MOV	PC, #0
	.global	_memset
_memset:			MOV	PC, #0
	.global	localeconv
localeconv:			MOV	PC, #0
	.global	mblen
mblen :				MOV	PC, #0
	.global	mbtowc
mbtowc:				MOV	PC, #0
	.global	wctomb
wctomb:				MOV	PC, #0
	.global	mbstowcs
mbstowcs:			MOV	PC, #0
	.global	wcstombs
wcstombs:			MOV	PC, #0
	.global	strxfrm
strxfrm:			MOV	PC, #0
	.global	strcoll
strcoll:			MOV	PC, #0
	.global	_clib_finalisemodule
_clib_finalisemodule:		MOV	PC, #0
	.global	_clib_version
_clib_version:			MOV	PC, #0
	.global	_Clib_Finalise
_Clib_Finalise:			MOV	PC, #0
	.global	tmpnam
tmpnam:				MOV	PC, #0
	.global	_swi
_swi:				MOV	PC, #0
	.global	_swix
_swix:				MOV	PC, #0
clib_vectors_mid:
	.space			clib_vectors_mid - clib_vectors_begin
clib_vectors_end:

	.global	__SIG_IGN
.set	__SIG_IGN, 0xfffffffd
	.global	__SIG_ERR
.set	__SIG_ERR, 0xfffffffe
	.global	__SIG_DFL
.set	__SIG_DFL, 0xffffffff

	.bss
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
