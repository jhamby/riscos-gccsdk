@ Provides an Object Format file for linking with the SharedCLibrary
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
@ NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES@ LOSS OF USE,
@ DATA, OR PROFITS@ OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
@ THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
@ (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
@ THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

.set	OS_GenerateError, 0x2B
.set	OS_Exit, 0x11
.set	OS_GetEnv, 0x10
.set	OS_Module, 0x1E

.set	X_Bit, 0x20000

.set	SharedCLibrary_LibInitAPCS_R, 0x80681
.set	SharedCLibrary_LibInitModule, 0x80682
.set	SharedCLibrary_LibInitAPCS_32, 0x80683
.set	SharedCLibrary_LibInitModuleAPCS_32, 0x80684

.weak	__root_stack_size
.weak	main

.global	_kernel_init
.global	_kernel_exit
.global	_kernel_setreturncode
.global	_kernel_exittraphandler
.global	_kernel_unwind
.global	_kernel_procname
.global	_kernel_language
.global	_kernel_command_string
.global	_kernel_hostos
.global	_kernel_swi
.global	_kernel_osbyte
.global	_kernel_osrdch
.global	_kernel_oswrch
.global	_kernel_osbget
.global	_kernel_osbput
.global	_kernel_osgbpb
.global	_kernel_osword
.global	_kernel_osfind
.global	_kernel_osfile
.global	_kernel_osargs
.global	_kernel_oscli
.global	_kernel_last_oserror
.global	_kernel_system
.global	_kernel_getenv
.global	_kernel_setenv
.global	_kernel_register_allocs
.global	_kernel_alloc
.global	_kernel_stkovf_split_0frame
.global	_kernel_stkovf_split
.global	_kernel_stkovf_copyargs
.global	_kernel_stkovf_copy0args
.global	_kernel_udiv
.global	__rt_udiv
.global	_kernel_urem
.global	_kernel_udiv10
.global	__rt_udiv10
.global	_kernel_sdiv
.global	__rt_sdiv
.global	_kernel_srem
.global	_kernel_sdiv10
.global	__rt_sdiv10
.global	_kernel_fpavailable
.global	_kernel_moduleinit
.global	_kernel_irqs_on
.global	_kernel_irqs_off
.global	_kernel_irqs_disabled
.global	_kernel_entermodule
.global	_kernel_escape_seen
.global	_kernel_current_stack_chunk
.global	_kernel_swi_c
.global	_kernel_register_slotextend
.global	_kernel_raise_error

.global	TrapHandler
.global	UncaughtTrapHandler
.global	EventHandler
.global	UnhandledEventHandler
.global	x$stack_overflow
.global	__rt_stkovf_split_small
.global	x$stack_overflow_1
.global	__rt_stkovf_split_big
.global	x$udivide
.global	x$uremainder
.global	x$divide
.global	x$divtest
.global	__rt_divtest
.global	x$remainder
.global	x$multiply
.global	_rd1chk
.global	_rd2chk
.global	_rd4chk
.global	_wr1chk
.global	_wr2chk
.global	_wr4chk
.global	_main
.global	_exit
.global	_clib_initialise
.global	_backtrace
.global	_count
.global	_count1
.global	_stfp
.global	_ldfp
.global	_printf
.global	_fprintf
.global	_sprintf
.global	clock
.global	difftime
.global	mktime
.global	time
.global	asctime
.global	ctime
.global	gmtime
.global	localtime
.global	strftime
.global	memcpy
.global	memmove
.global	strcpy
.global	strncpy
.global	strcat
.global	strncat
.global	memcmp
.global	strcmp
.global	strncmp
.global	memchr
.global	strchr
.global	strcspn
.global	strpbrk
.global	strrchr
.global	strspn
.global	strstr
.global	strtok
.global	memset
.global	strerror
.global	strlen
.global	atof
.global	atoi
.global	atol
.global	strtod
.global	strtol
.global	strtoul
.global	rand
.global	srand
.global	calloc
.global	free
.global	malloc
.global	realloc
.global	abort
.global	atexit
.global	exit
.global	getenv
.global	system
.global	bsearch
.global	qsort
.global	abs
.global	div
.global	labs
.global	ldiv
.global	remove
.global	rename
.global	tmpfile
.global	__old_tmpnam
.global	fclose
.global	fflush
.global	fopen
.global	freopen
.global	setbuf
.global	setvbuf
.global	printf
.global	fprintf
.global	sprintf
.global	scanf
.global	fscanf
.global	sscanf
.global	vprintf
.global	__gcc_vprintf
.global	vfprintf
.global	__gcc_vfprintf
.global	vsprintf
.global	__gcc_vsprintf
.global	_vfprintf
.global	fgetc
.global	fgets
.global	fputc
.global	fputs
.global	__filbuf
.global	getc
.global	getchar
.global	gets
.global	__flsbuf
.global	putc
.global	putchar
.global	puts
.global	ungetc
.global	fread
.global	fwrite
.global	fgetpos
.global	fseek
.global	fsetpos
.global	ftell
.global	rewind
.global	clearerr
.global	feof
.global	ferror
.global	perror
.global	__SIG_IGN
.global	__ignore_signal_handler
.global	__SIG_ERR
.global	__default_signal_handler
.global	__SIG_DFL
.global	__error_signal_marker
.global	signal
.global	raise
.global	setjmp
.global	longjmp
.global	acos
.global	asin
.global	atan
.global	atan2
.global	cos
.global	sin
.global	tan
.global	cosh
.global	sinh
.global	tanh
.global	exp
.global	frexp
.global	ldexp
.global	log
.global	log10
.global	modf
.global	pow
.global	sqrt
.global	ceil
.global	fabs
.global	floor
.global	fmod
.global	setlocale
.global	isalnum
.global	isalpha
.global	iscntrl
.global	isdigit
.global	isgraph
.global	islower
.global	isprint
.global	ispunct
.global	isspace
.global	isupper
.global	isxdigit
.global	tolower
.global	toupper
.global	__assert
.global	_memcpy
.global	_memset
.global	localeconv
.global	mblen
.global	mbtowc
.global	wctomb
.global	mbstowcs
.global	wcstombs
.global	strxfrm
.global	strcoll
.global	_clib_finalisemodule
.global	_clib_version
.global	_clib_finalise @ RISC OS PRMs say 'Finalise' but that'll clash
.global	_Clib_Finalise @ StubsG calls it this instead
.global	tmpnam
.global	_swi
.global	_swix
.global  disable_stack_extension
.global	__errno
.global	errno
.global	__iob
.global	_interrupts_off
.global	__ctype
.global	__huge_val

        @@EXPORT __fpclassifyf
.global __fpclassifyd
.global __signbitf
.global __signbitd
.global copysign
        @@EXPORT copysignf
.global nan
.global nanf
.global nextafter
        @@EXPORT nextafterf
.global fdim
        @@EXPORT fdimf
.global fmax
        @@EXPORT fmaxf
.global fmin
        @@EXPORT fminf
        @@EXPORT fabsf
.global hypot
.global hypotf
.global feclearexcept
.global fegetexceptflag
.global feraiseexcept
.global fesetexceptflag
.global fetestexcept
.global fegetround
.global fesetround
.global fegetenv
.global feholdexcept
.global fesetenv
.global feupdateenv
.global _snprintf
.global snprintf
.global __gcc_vsnprintf
.global vsnprintf
.global vfscanf
.global vscanf
.global vsscanf
        @@EXPORT ceilf
        @@EXPORT floorf
.global nearbyint
        @@EXPORT nearbyintf
.global rint
        @@EXPORT rintf
.global lrint
        @@EXPORT lrintf
.global round
        @@EXPORT roundf
.global lround
        @@EXPORT lroundf
.global trunc
        @@EXPORT truncf
.global remainder
        @@EXPORT remainderf
.global llabs
.global lldiv
.global atoll
.global strtoll
.global strtoull
.global imaxabs
.global imaxdiv
.global strtoimax
.global strtoumax
.global __assert2
.global _Exit
        @@EXPORT acosf
        @@EXPORT asinf
        @@EXPORT atanf
        @@EXPORT atan2f
        @@EXPORT cosf
        @@EXPORT sinf
        @@EXPORT tanf
.global acosh
        @@EXPORT acoshf
.global asinh
        @@EXPORT asinhf
.global atanh
        @@EXPORT atanhf
        @@EXPORT expf
.global exp2
        @@EXPORT exp2f
.global expm1
        @@EXPORT expm1f
        @@EXPORT frexpf
.global ilogb
        @@EXPORT ilogbf
        @@EXPORT ldexpf
        @@EXPORT logf
        @@EXPORT log10f
.global log1p
        @@EXPORT log1pf
.global log2
        @@EXPORT log2f
.global logb
        @@EXPORT logbf
        @@EXPORT modff
        @@EXPORT fmodf
.global scalbn
        @@EXPORT scalbnf
.global scalbln
        @@EXPORT scalblnf
.global cbrt
        @@EXPORT cbrtf
        @@EXPORT powf
        @@EXPORT sqrtf
.global erf
        @@EXPORT erff
.global erfc
        @@EXPORT erfcf
.global lgamma
        @@EXPORT lgammaf
.global tgamma
        @@EXPORT tgammaf
.global nexttoward
        @@EXPORT nexttowardf

	.text
	.global	__main
__main:
	SWI	OS_GetEnv
	MOV	r2, r1			@ workspace limit
	ADR	r0, stubs		@ list of stubs
	LDR	r1, rwlimit		@ workspace start
	MOV	r3, #-1			@ "-1"
	MOV	r4, #0			@ "0"
	MOV	r5, #-1			@ "-1"

	@ check for __root_stack_size. If it doesn't exist, then
	@ make our own up.
	LDR	r6, stksiz
	TEQ	r6, #0
	LDRNE	r6, [r6]		@ use __root_stack_size
	MOVEQ	r6, #4<<16		@ 4KB (single chunk) stack, then.
	MOVNE	r6, r6, asr #10 	@ convert bytes to KB
	MOVNE	r6, r6, lsl #16 	@ put in upper 16bits

	TEQ	r0, r0			@ Set Z flag
	TEQ	pc, pc			@ EQ if in 32-bit mode
	ORREQ	r6, r6, #1

#ifdef SUPPORT_SCL26BIT
	SWI	SharedCLibrary_LibInitAPCS_R
	MOV	r6, r6, lsl #16 	@ safety catch for ancient Shared C Lib modules
	CMP	r6, #(5<<16)		@ check library version number
#else
	SWI	SharedCLibrary_LibInitAPCS_32
	MOV	r6, r6, lsl #16 	@ safety catch for ancient Shared C Lib modules
	CMP	r6, #(6<<16)		@ check library version number (32bit)
#endif

	MOVGE	r4, r0			@ end of workspace
	ADRGE	r0, kernel_init_block
	MOVGE	r3, #0			@ we're an application
	BGE	_kernel_init		@ off we go!

	ADR	r0, error
	SWI	OS_GenerateError
error:
	.word	0
	.asciz	"Shared C Library not loaded or out of date"
	.align

stubs:
	.word	1			@ lib chunk id : kernel
	.word	kernel_vectors_begin
	.word	kernel_vectors_end
	.word	kernel_statics_begin
	.word	kernel_statics_end
	.word	2			@ lib chunk id : CLib
	.word	clib_vectors_begin
	.word	clib_vectors_end
	.word	clib_statics_begin
	.word	clib_statics_end
	.word	5                       @ lib chunk id : Clib extra functions
	.word	extra_vectors_begin
	.word	extra_vectors_end
	.word	0
	.word	0
	.word	-1			@ end of list

kernel_init_block:
	.word	Image$$RO$$Base	@ image base
	.word	RTSK$$Data$$Base	@ start of lang blocks
	.word	RTSK$$Data$$Limit	@ end of lang blocks
rwlimit:	.word	Image$$RW$$Limit
stksiz:	.word	__root_stack_size
rwbase:	.word	Image$$RW$$Base
zibase:	.word	Image$$ZI$$Base

.global	_Lib$Reloc$Off$DP
	@ XXX This feels bogus to me, but with CMunge, it converts to
	@ the constant 540 when adjusting the SL register in the module
	@ initialisation and command interface code.
.set	_Lib$Reloc$Off$DP, 0xf87

@@.global _Mod$Reloc$Off
@@_Mod$Reloc$Off        EQU &218

	.weak	__RelocCode
reloccode:
	.word	__RelocCode

	@ RMA module entry point, called from the module initialisation
	@ code.
	@ On entry:	 r0 = 1, then allocate image space
	@		 r0 = 0, just allocate 12 bytes for SCL
.global	_clib_initialisemodule
_clib_initialisemodule:
	str	r14, [sp, #-4]!
	mov	r9, r0

	@ Relocate module.  Check weak link exists (and therefore function
	@ is defined) before calling it.
	ldr	r0, reloccode
	teq	r0, #0
	blne	__RelocCode

	@ Claim RMA space for module data
	mov	r0, #6
	mov	r4, #0
	mov	r5, #0
	cmp	r9, #0
	moveq	r3, #12	@ reserve 12 bytes for scl statics
	ldrne	r4, rwbase		@ Image$$RW$$Base
	ldrne	r5, rwlimit		@ Image$$RW$$Limit
	subne	r3, r5, r4
	addne	r3, r3, #12	@ 12 bytes for scl statics
	swi	OS_Module + X_Bit
	@ On exit:	 r2 = pointer to claimed block
	@		 r3 = preserved (i.e. requested block size)
	ldrvs	pc, [sp], #4

	mov	r9, r12		@ hold workspace pointer
	@ Set module private word to point to this new allocation
	str	r2, [r12, #0]

	mov	r12, r2
	@ First word of private word is size of allocated block
	str	r3, [r12, #0]

	adr	r0, stubs	@ list of stub descriptions
	add	r1, r12, #12	@ pointer to workspace start
	add	r2, r12, r3	@ pointer to workspace limit
	ldr	r3, zibase	@ Image$$ZI$$base
	ldr	r6, stksiz
	teq	r6, #0
	ldrne	r6, [r6]	@ use __root_stack_size
	moveq	r6, #4<<16	@ otherwise default to 4K for root stack chunk
	movne	r6, r6, asr #10	@ convert bytes to KB
	movne	r6, r6, lsl #16 @ put in upper 16 bits
#ifdef SUPPORT_SCL26BIT
	swi	SharedCLibrary_LibInitModule + X_Bit
#else
	swi	SharedCLibrary_LibInitModuleAPCS_32 + X_Bit
#endif
	bvc	modscl_init

	@ An error occurred
	mov	r1, r0		@ preserve error pointer
	mov	r0, #7		@ free claimed workspace
	ldr	r2, [r9, #0]
	swi	OS_Module + X_Bit
	mov	r0, #0		@ set private word pointer
	str	r0, [r9, #0]
	mov	r0, r1		@ restore error pointer
	ldr	pc, [sp], #4	@ exit


modscl_init:
	@ Store the static data offsets for SCL and client use in the
	@ first 12 bytes of module workspace.  The cmgh/cmunge module
	@ header interface assumes the position of these words.

	@ The client static data offset becomes the value that we
	@ reference through sl[-536] for accessing global variables.
	ldr	r7, [r1, #20]	@ scl static data offset
	ldr	r8, [r1, #24]	@ client static data offset

	str	r7, [r12, #4]	@ store in private workspace
	str	r8, [r12, #8]

	mov	r4, r0			@ end of workspace
	adr	r0, kernel_init_block
	adr	lr, clibinit
	str	lr, [sp, #-4]!
	@ _kernel_moduleinit pops the return address off the stack
	b	_kernel_moduleinit
clibinit:
	str	r9, [sp, #-4]!	@ save workspace pointer
	bl	_clib_initialise
	ldr	r0, [sp], #4
	ldr	pc, [sp], #4

.global	_clib_entermodule
_clib_entermodule:
	adr	r0, kernel_init_block
	mov	r8, r12
	mvn	r12, #0

	@ Set stack size according to __root_stack_size, or 4K if it
	@ does not exist.
	ldr	r6, stksiz
	cmp	r6, #0
	moveq	r6, #4 * 1024
	ldrne	r6, [r6, #0]
	b	_kernel_entermodule

	@@ Area: RTSK$$Data
	.data
RTSK$$Data$$Base:
	.word	40			@ RTSK$$Data$$Limit - RTSK$$Data$$Base
	.word	Image$$RO$$Base		@ C$$code$$Base	@ gcc is not nicely compatible with SCL
	.word	Image$$RO$$Limit	@ C$$code$$Limit	@ because it makes C$$CodeX areas @-(
	.word	language_name		@ must be "C"
	.word	__sclmain		@ our PROC-returning InitProc
	.word	0			@ finaliser
	@ these are the "optionals"...
	.word	TrapHandler		@ SharedCLib's own trap handler...
	.word	UncaughtTrapHandler	@ ...and uncaught trap proc
	.word	EventHandler		@ ...and event proc
	.word	UnhandledEventHandler	@ ...and unhandled event proc!
	@ no fast event proc
	@ no unwind proc
	@ no name proc
RTSK$$Data$$Limit:

	.text
language_name:
	.asciz	"C"
	.align

__sclmain:
	MOV	r0, #1
	LDR	r1, stub_data
	STRB	r0, [r1, #_stub_kallocExtendsWS - kernel_statics_begin]

	MOV	r0, sp			@ (historical??)
	STMFD	sp!, {lr}
	LDR	r1, rtsk_data
	LDMIB	r1, {r1, r2}		@ fetch code start/end (historical??)
	BL	_clib_initialise
	LDR	r0, c_run		@ get our main()
	CMP	r0, #0			@ was there one?
	ADRNE	r0, c_next		@ yup, so point to hook to it

	@ For 26-bit SharedCLibrary (in which case, we're using a 26-bit OS)
	@ we still need to restore flags on exit.  For 32-bit SCL on 26-bit
	@ we restore them anyway, as this makes no difference.

	TEQ     r0, r0                  @ Set Z flag
	TEQ     pc, pc                  @ EQ if in 32-bit mode

	LDMEQFD sp!, {pc}
	LDMFD   sp!, {pc}^

	@ This is called when all RTL blocks have been initialised via their InitProc
	.asciz	"___init"
	.align
	.word	0xff000008
c_next:
	MOV	ip, sp
	STMFD	sp!, {fp, ip, lr, pc}
	SUB	fp, ip, #4
	BL	_kernel_command_string	@ get r0=command string
	LDR	r1, c_run		@ get ptr to main() function
	BL	_main			@ call clib to enter it.  SHOULD never return from here.
	LDMFD   fp, {fp, sp, pc}

stub_data:
	.word	Stub$$Data$$Base

rtsk_data:
	.word	RTSK$$Data$$Base

c_run:
	.word	main

	.text
kernel_vectors_begin:

_kernel_init:			MOV	pc, #0
_kernel_exit:			MOV	pc, #0
_kernel_setreturncode:		MOV	pc, #0
_kernel_exittraphandler:	MOV	pc, #0
_kernel_unwind:			MOV	pc, #0
_kernel_procname:		MOV	pc, #0
_kernel_language:		MOV	pc, #0
_kernel_command_string:		MOV	pc, #0
_kernel_hostos:			MOV	pc, #0
_kernel_swi:			MOV	pc, #0
_kernel_osbyte:			MOV	pc, #0
_kernel_osrdch:			MOV	pc, #0
_kernel_oswrch:			MOV	pc, #0
_kernel_osbget:			MOV	pc, #0
_kernel_osbput:			MOV	pc, #0
_kernel_osgbpb:			MOV	pc, #0
_kernel_osword:			MOV	pc, #0
_kernel_osfind:			MOV	pc, #0
_kernel_osfile:			MOV	pc, #0
_kernel_osargs:			MOV	pc, #0
_kernel_oscli:			MOV	pc, #0
_kernel_last_oserror:		MOV	pc, #0
_kernel_system:			MOV	pc, #0
_kernel_getenv:			MOV	pc, #0
_kernel_setenv:			MOV	pc, #0
_kernel_register_allocs:	MOV	pc, #0
_kernel_alloc:			MOV	pc, #0
_kernel_stkovf_split_0frame:	MOV	pc, #0
_kernel_stkovf_split:		MOV	pc, #0
_kernel_stkovf_copyargs:	MOV	pc, #0
_kernel_stkovf_copy0args:	MOV	pc, #0
__rt_udiv:
_kernel_udiv:			MOV	pc, #0
_kernel_urem:			MOV	pc, #0
__rt_udiv10:
_kernel_udiv10:			MOV	pc, #0
__rt_sdiv:
_kernel_sdiv:			MOV	pc, #0
_kernel_srem:			MOV	pc, #0
__rt_sdiv10:
_kernel_sdiv10:			MOV	pc, #0
_kernel_fpavailable:		MOV	pc, #0
_kernel_moduleinit:		MOV	pc, #0
_kernel_irqs_on:		MOV	pc, #0
_kernel_irqs_off:		MOV	pc, #0
_kernel_irqs_disabled:		MOV	pc, #0
_kernel_entermodule:		MOV	pc, #0
_kernel_escape_seen:		MOV	pc, #0
_kernel_current_stack_chunk:	MOV	pc, #0
_kernel_swi_c:			MOV	pc, #0
_kernel_register_slotextend:	MOV	pc, #0
_kernel_raise_error:		MOV	pc, #0

kernel_vectors_end:

clib_vectors_begin:

TrapHandler:			MOV	pc, #0
UncaughtTrapHandler:		MOV	pc, #0
EventHandler:			MOV	pc, #0
UnhandledEventHandler:		MOV	pc, #0
__rt_stkovf_split_small:
x$stack_overflow:		MOV	pc, #0
__rt_stkovf_split_big:
x$stack_overflow_1:		MOV	pc, #0
x$udivide:			MOV	pc, #0
x$uremainder:			MOV	pc, #0
x$divide:			MOV	pc, #0
__rt_divtest:
x$divtest:			MOV	pc, #0
x$remainder:			MOV	pc, #0
x$multiply:			MOV	pc, #0
_rd1chk:			MOV	pc, #0
_rd2chk:			MOV	pc, #0
_rd4chk:			MOV	pc, #0
_wr1chk:			MOV	pc, #0
_wr2chk:			MOV	pc, #0
_wr4chk:			MOV	pc, #0
_main:				MOV	pc, #0
_exit:				MOV	pc, #0
_clib_initialise:		MOV	pc, #0
_backtrace:			MOV	pc, #0
_count:				MOV	pc, #0
_count1:			MOV	pc, #0
_stfp:				MOV	pc, #0
_ldfp:				MOV	pc, #0
_printf:			MOV	pc, #0
_fprintf:			MOV	pc, #0
_sprintf:			MOV	pc, #0
clock:				MOV	pc, #0
difftime:			MOV	pc, #0
mktime:				MOV	pc, #0
time:				MOV	pc, #0
asctime:			MOV	pc, #0
ctime:				MOV	pc, #0
gmtime:				MOV	pc, #0
localtime:			MOV	pc, #0
strftime:			MOV	pc, #0
memcpy:				MOV	pc, #0
memmove:			MOV	pc, #0
strcpy:				MOV	pc, #0
strncpy:			MOV	pc, #0
strcat:				MOV	pc, #0
strncat:			MOV	pc, #0
memcmp:				MOV	pc, #0
strcmp:				MOV	pc, #0
strncmp:			MOV	pc, #0
memchr:				MOV	pc, #0
strchr:				MOV	pc, #0
strcspn:			MOV	pc, #0
strpbrk:			MOV	pc, #0
strrchr:			MOV	pc, #0
strspn:				MOV	pc, #0
strstr:				MOV	pc, #0
strtok:				MOV	pc, #0
memset:				MOV	pc, #0
strerror:			MOV	pc, #0
strlen:				MOV	pc, #0
atof:				MOV	pc, #0
atoi:				MOV	pc, #0
atol:				MOV	pc, #0
strtod:				MOV	pc, #0
strtol:				MOV	pc, #0
strtoul:			MOV	pc, #0
rand:				MOV	pc, #0
srand:				MOV	pc, #0
calloc:				MOV	pc, #0
free:				MOV	pc, #0
malloc:				MOV	pc, #0
realloc:			MOV	pc, #0
abort:				MOV	pc, #0
atexit:				MOV	pc, #0
exit:				MOV	pc, #0
getenv:				MOV	pc, #0
system:				MOV	pc, #0
bsearch:			MOV	pc, #0
qsort:				MOV	pc, #0
abs:				MOV	pc, #0
div:				MOV	pc, #0
labs:				MOV	pc, #0
ldiv:				MOV	pc, #0
remove:				MOV	pc, #0
rename:				MOV	pc, #0
tmpfile:			MOV	pc, #0
__old_tmpnam:			MOV	pc, #0
fclose:				MOV	pc, #0
fflush:				MOV	pc, #0
fopen :				MOV	pc, #0
freopen:			MOV	pc, #0
setbuf:				MOV	pc, #0
setvbuf:			MOV	pc, #0
printf:				MOV	pc, #0
fprintf:			MOV	pc, #0
sprintf:			MOV	pc, #0
scanf:				MOV	pc, #0
fscanf:				MOV	pc, #0
sscanf:				MOV	pc, #0
__gcc_vprintf:
vprintf:	 		MOV	pc, #0
__gcc_vfprintf:
vfprintf:			MOV	pc, #0
__gcc_vsprintf:
vsprintf:			MOV	pc, #0
_vfprintf:			MOV	pc, #0
fgetc:				MOV	pc, #0
fgets:				MOV	pc, #0
fputc:				MOV	pc, #0
fputs:				MOV	pc, #0
__filbuf:			MOV	pc, #0
getc:				MOV	pc, #0
getchar:			MOV	pc, #0
gets:				MOV	pc, #0
__flsbuf:			MOV	pc, #0
putc:				MOV	pc, #0
putchar:			MOV	pc, #0
puts:				MOV	pc, #0
ungetc:				MOV	pc, #0
fread:				MOV	pc, #0
fwrite:				MOV	pc, #0
fgetpos:			MOV	pc, #0
fseek:				MOV	pc, #0
fsetpos:			MOV	pc, #0
ftell:				MOV	pc, #0
rewind:				MOV	pc, #0
clearerr:			MOV	pc, #0
feof:				MOV	pc, #0
ferror:				MOV	pc, #0
perror:				MOV	pc, #0
__SIG_IGN:
__ignore_signal_handler:	MOV	pc, #0
__SIG_ERR:
__error_signal_marker:		MOV	pc, #0
__SIG_DFL:
__default_signal_handler:	MOV	pc, #0
signal:				MOV	pc, #0
raise:				MOV	pc, #0
setjmp:				MOV	pc, #0
longjmp:			MOV	pc, #0
acos:				MOV	pc, #0
asin:				MOV	pc, #0
atan:				MOV	pc, #0
atan2:				MOV	pc, #0
cos:				MOV	pc, #0
sin:				MOV	pc, #0
tan:				MOV	pc, #0
cosh:				MOV	pc, #0
sinh:				MOV	pc, #0
tanh:				MOV	pc, #0
exp:				MOV	pc, #0
frexp:				MOV	pc, #0
ldexp:				MOV	pc, #0
log:				MOV	pc, #0
log10:				MOV	pc, #0
modf:				MOV	pc, #0
pow:				MOV	pc, #0
sqrt:				MOV	pc, #0
ceil:				MOV	pc, #0
fabs:				MOV	pc, #0
floor:				MOV	pc, #0
fmod:				MOV	pc, #0
setlocale:			MOV	pc, #0
isalnum:			MOV	pc, #0
isalpha:			MOV	pc, #0
iscntrl:			MOV	pc, #0
isdigit:			MOV	pc, #0
isgraph:			MOV	pc, #0
islower:			MOV	pc, #0
isprint:			MOV	pc, #0
ispunct:			MOV	pc, #0
isspace:			MOV	pc, #0
isupper:			MOV	pc, #0
isxdigit:			MOV	pc, #0
tolower:			MOV	pc, #0
toupper:			MOV	pc, #0
__assert:			MOV	pc, #0
_memcpy:			MOV	pc, #0
_memset:			MOV	pc, #0
localeconv:			MOV	pc, #0
mblen :				MOV	pc, #0
mbtowc:				MOV	pc, #0
wctomb:				MOV	pc, #0
mbstowcs:			MOV	pc, #0
wcstombs:			MOV	pc, #0
strxfrm:			MOV	pc, #0
strcoll:			MOV	pc, #0
_clib_finalisemodule:		MOV	pc, #0
_clib_version:			MOV	pc, #0
_Clib_Finalise:
_clib_finalise:			MOV	pc, #0
tmpnam:				MOV	pc, #0
_swi:				MOV	pc, #0
_swix:				MOV	pc, #0

clib_vectors_end:

extra_vectors_begin:

__fpclassifyf_FIXME_float_arg:  MOV	pc, #0
__fpclassifyd:			MOV	pc, #0
__signbitf:			MOV	pc, #0
__signbitd:			MOV	pc, #0
copysign:			MOV	pc, #0
copysignf_not_used:		MOV	pc, #0
nan:				MOV	pc, #0
nanf:				MOV	pc, #0
nextafter:			MOV	pc, #0
nextafterf_FIXME_float_arg:	MOV	pc, #0
fdim:				MOV	pc, #0
fdimf_FIXME_float_arg:		MOV	pc, #0
fmax:				MOV	pc, #0
fmaxf_FIXME_float_arg:		MOV	pc, #0
fmin:				MOV	pc, #0
fminf_FIXME_float_arg:		MOV	pc, #0
fabsf_not_used:			MOV	pc, #0
hypot:				MOV	pc, #0
hypotf:				MOV	pc, #0
feclearexcept:			MOV	pc, #0
fegetexceptflag:		MOV	pc, #0
feraiseexcept:			MOV	pc, #0
fesetexceptflag:		MOV	pc, #0
fetestexcept:			MOV	pc, #0
fegetround:			MOV	pc, #0
fesetround:			MOV	pc, #0
fegetenv:			MOV	pc, #0
feholdexcept:			MOV	pc, #0
fesetenv:			MOV	pc, #0
feupdateenv:			MOV	pc, #0
_snprintf:			MOV	pc, #0
snprintf:			MOV	pc, #0
__gcc_vsnprintf:
vsnprintf:			MOV	pc, #0
vfscanf:			MOV	pc, #0
vscanf:				MOV	pc, #0
vsscanf:			MOV	pc, #0
ceilf_not_used:			MOV	pc, #0
floorf_not_used:		MOV	pc, #0
nearbyint:			MOV	pc, #0
nearbyintf_FIXME_float_arg:	MOV	pc, #0
rint:				MOV	pc, #0
rintf_FIXME_float_arg:		MOV	pc, #0
lrint:				MOV	pc, #0
lrintf_FIXME_float_arg:		MOV	pc, #0
round:				MOV	pc, #0
roundf_FIXME_float_arg:		MOV	pc, #0
lround:				MOV	pc, #0
lroundf_FIXME_float_arg:	MOV	pc, #0
trunc:				MOV	pc, #0
truncf_FIXME_float_arg:		MOV	pc, #0
remainder:			MOV	pc, #0
remainderf_FIXME_float_arg:	MOV	pc, #0
llabs:				MOV	pc, #0
lldiv:				MOV	pc, #0
atoll:				MOV	pc, #0
strtoll:			MOV	pc, #0
strtoull:			MOV	pc, #0
imaxabs:			MOV	pc, #0
imaxdiv:			MOV	pc, #0
strtoimax:			MOV	pc, #0
strtoumax:			MOV	pc, #0
__assert2:			MOV	pc, #0
_Exit:				MOV	pc, #0
acosf_not_used:			MOV	pc, #0
asinf_not_used:			MOV	pc, #0
atanf_not_used:			MOV	pc, #0
atan2f_not_used:		MOV	pc, #0
cosf_not_used:			MOV	pc, #0
sinf_not_used:			MOV	pc, #0
tanf_not_used:			MOV	pc, #0
acosh:				MOV	pc, #0
acoshf_FIXME_float_arg:		MOV	pc, #0
asinh:				MOV	pc, #0
asinhf_FIXME_float_arg:		MOV	pc, #0
atanh:				MOV	pc, #0
atanhf_FIXME_float_arg:		MOV	pc, #0
expf_not_used:			MOV	pc, #0
exp2:				MOV	pc, #0
exp2f_FIXME_float_arg:		MOV	pc, #0
expm1:				MOV	pc, #0
expm1f_FIXME_float_arg:		MOV	pc, #0
frexpf_FIXME_float_arg:		MOV	pc, #0
ilogb:				MOV	pc, #0
ilogbf_FIXME_float_arg:		MOV	pc, #0
ldexpf_FIXME_float_arg:		MOV	pc, #0
logf_not_used:			MOV	pc, #0
log10f_FIXME_float_arg:		MOV	pc, #0
log1p:				MOV	pc, #0
log1pf_FIXME_float_arg:		MOV	pc, #0
log2:				MOV	pc, #0
log2f_FIXME_float_arg:		MOV	pc, #0
logb:				MOV	pc, #0
logbf_FIXME_float_arg:		MOV	pc, #0
modff_not_used:			MOV	pc, #0
fmodf_FIXME_float_arg:		MOV	pc, #0
scalbn:				MOV	pc, #0
scalbnf_FIXME_float_arg:	MOV	pc, #0
scalbln:			MOV	pc, #0
scalblnf_FIXME_float_arg:	MOV	pc, #0
cbrt:				MOV	pc, #0
cbrtf_FIXME_float_arg:		MOV	pc, #0
powf_not_used:			MOV	pc, #0
sqrtf_FIXME_float_arg:		MOV	pc, #0
erf:				MOV	pc, #0
erff_FIXME_float_arg:		MOV	pc, #0
erfc:				MOV	pc, #0
erfcf_FIXME_float_arg:		MOV	pc, #0
lgamma:				MOV	pc, #0
lgammaf_FIXME_float_arg:	MOV	pc, #0
tgamma:				MOV	pc, #0
tgammaf_FIXME_float_arg:	MOV	pc, #0
nexttoward:			MOV	pc, #0
nexttowardf_FIXME_float_arg:	MOV	pc, #0

extra_vectors_end:


	.section ".bss"
Stub$$Data$$Base:
kernel_statics_begin:
	.space	0x31c
kernel_statics_end:

.set	_stub_imageBase, kernel_statics_begin + 0x000
.set	_stub_rtsDataBase, kernel_statics_begin + 0x004
.set	_stub_rtsDataLimit, kernel_statics_begin + 0x008
.set	_stub_errorR12, kernel_statics_begin + 0x00c
.set	_stub_errorBuffer, kernel_statics_begin + 0x010
.set	_stub_errorNumber, kernel_statics_begin + 0x014
.set	_stub_errorString, kernel_statics_begin + 0x018
.set	_stub_registerDump, kernel_statics_begin + 0x07c
.set	_stub_oldAbortHandlers, kernel_statics_begin + 0x0bc
.set	_stub_oldExitHandler, kernel_statics_begin + 0x0cc
.set	_stub_oldMemoryLimit, kernel_statics_begin + 0x0d4
.set	_stub_oldErrorHandler, kernel_statics_begin + 0x0d8
.set	_stub_oldErrorR0, kernel_statics_begin + 0x0dc
.set	_stub_oldErrorBuffer, kernel_statics_begin + 0x0e0
.set	_stub_oldCallBackHandler, kernel_statics_begin + 0x0e4
.set	_stub_oldEscapeHandler, kernel_statics_begin + 0x0f0
.set	_stub_oldEventHandler, kernel_statics_begin + 0x0f8
.set	_stub_oldUpCallHandler, kernel_statics_begin + 0x100
.set	_stub_languageEnvSave, kernel_statics_begin + 0x108
.set	_stub_hadEscape, kernel_statics_begin + 0x114
.set	_stub_kallocExtendsWS, kernel_statics_begin + 0x115
.set	_stub_inTrapHandler, kernel_statics_begin + 0x116
.set	_stub_beingDebugged, kernel_statics_begin + 0x117
.set	_stub_fpPresent, kernel_statics_begin + 0x118
.set	_stub_initialised, kernel_statics_begin + 0x119
.set	_stub_callbackInactive, kernel_statics_begin + 0x11a
.set	_stub_unused_byte_2, kernel_statics_begin + 0x11b
.set	_stub_IIHandlerInData, kernel_statics_begin + 0x11c
.set	_stub_PAHandlerInData, kernel_statics_begin + 0x128
.set	_stub_DAHandlerInData, kernel_statics_begin + 0x134
.set	_stub_AEHandlerInData, kernel_statics_begin + 0x140
.set	_stub_eventCode, kernel_statics_begin + 0x14c
.set	_stub_eventUserR13, kernel_statics_begin + 0x150
.set	_stub_eventRegisters, kernel_statics_begin + 0x154
.set	_stub_fastEventStack, kernel_statics_begin + 0x184
.set	_stub_fastEventStackEnd, kernel_statics_begin + 0x284
.set	_stub_heapTop, kernel_statics_begin + 0x284
.set	_stub_heapLimit, kernel_statics_begin + 0x288
.set	_stub_allocProc, kernel_statics_begin + 0x28c
.set	_stub_freeProc, kernel_statics_begin + 0x290
.set	_stub_returnCode, kernel_statics_begin + 0x294
.set	_stub_moduleDataWord, kernel_statics_begin + 0x298
.set	_stub_APCS_A_Client, kernel_statics_begin + 0x29c
.set	_stub_escapeSeen, kernel_statics_begin + 0x29d
.set	_stub_unwinding, kernel_statics_begin + 0x29e
.set	_stub_underDesktop, kernel_statics_begin + 0x29f
.set	_stub_heapBase, kernel_statics_begin + 0x2a0
.set	_stub_ArgString, kernel_statics_begin + 0x2a4
.set	_stub_heapExtender, kernel_statics_begin + 0x2a8
.set	_stub_knownSlotSize, kernel_statics_begin + 0x2ac
.set	_stub_initSlotSize, kernel_statics_begin + 0x2b0
.set	_stub_lk_RestoreOSHandlers, kernel_statics_begin + 0x2b4
.set	_stub_extendChunk, kernel_statics_begin + 0x2b8
.set	_stub_rootStackChunk, kernel_statics_begin + 0x2bc
.set	_stub_pc_hex_buff, kernel_statics_begin + 0x2c0
.set	_stub_returnCodeLimit, kernel_statics_begin + 0x2c0
.set	_stub_reg_hex_buff, kernel_statics_begin + 0x2cc
.set	disable_stack_extension, kernel_statics_begin + 0x2d8
.set	_stub_unused, kernel_statics_begin + 0x2dc

clib_statics_begin:
	.space 0xb48
clib_statics_end:

.set	errno, clib_statics_begin + 0
.set	__errno, clib_statics_begin + 0
.set	__iob, clib_statics_begin + 4
.set	_interrupts_off, clib_statics_begin + 0x284
.set	_stub__saved_interrupt, clib_statics_begin + 0x288
.set	_stub_ctype_eof, clib_statics_begin + 0x28C
.set	__ctype, clib_statics_begin + 0x290
.set	__huge_val, clib_statics_begin + 0x390

.set	_stub_app_space_end, clib_statics_begin + 0x6b4
.set	_stub_stack_o_flag, clib_statics_begin + 0x6b8
.set	_stub_inSignalHandler, clib_statics_begin + 0x6b9
.set	_stub_dummybyte, clib_statics_begin + 0x6ba
.set	_stub_, clib_statics_begin + 0x6bb
.set	_stub_ClibSpace, clib_statics_begin + 0x6bc
Stub$$Data$$Limit:
