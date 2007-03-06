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

.set	OS_GenerateError, 0x2B
.set	OS_Exit, 0x11
.set	OS_GetEnv, 0x10
.set	OS_Module, 0x1E

.set	X_Bit, 0x20000

.set	SharedCLibrary_LibInitAPCS_32, 0x80683
.set	SharedCLibrary_LibInitModuleAPCS_32, 0x80684

	.text
	.global	__main
	.type	__main, %function
__main:
	SWI	OS_GetEnv
	MOV	R2, R1			@ workspace limit
	ADR	R0, stubs		@ list of stubs
	LDR	R1, rwlimit		@ workspace start
	MOV	R3, #-1			@ "-1"
	MOV	R4, #0			@ "0"
	MOV	R5, #-1			@ "-1"

	@ check for __root_stack_size. If it doesn't exist, then
	@ make our own up.
	LDR	R6, stksiz
	TEQ	R6, #0
	LDRNE	R6, [R6]		@ use __root_stack_size
	MOVEQ	R6, #4<<16		@ 4KB (single chunk) stack, then.
	MOVNE	R6, R6, ASR#10		@ convert bytes to KB
	MOVNE	R6, R6, LSL#16		@ put in upper 16bits

	TEQ	R0, R0			@ Set Z flag
	TEQ	PC, PC			@ EQ if running in 32-bit mode
	ORREQ	R6, R6, #1

	SWI	SharedCLibrary_LibInitAPCS_32
	MOV	R6, R6, LSL#16		@ safety catch for ancient Shared C Lib modules
	CMP	R6, #6<<16		@ check library version number (32bit)

	MOVGE	R4, R0			@ end of workspace
	ADRGE	R0, kernel_init_block
	MOVGE	R3, #0			@ we're an application
	BGE	_kernel_init		@ off we go!

	ADR	R0, out_of_date_error
	SWI	OS_GenerateError
out_of_date_error:
	.word	0x800e91
	.asciz	"Shared C Library not loaded or out of date"
	.align

stubs:
	.word	1			@ lib chunk id : kernel
	.word	kernel_vectors_begin(GOTOFF)
	.word	kernel_vectors_end(GOTOFF)
	.word	kernel_statics_begin(GOTOFF)
	.word	kernel_statics_end(GOTOFF)
	.word	2			@ lib chunk id : CLib
	.word	clib_vectors_begin(GOTOFF)
	.word	clib_vectors_end(GOTOFF)
	.word	clib_statics_begin(GOTOFF)
	.word	clib_statics_end(GOTOFF)
	.word	5                       @ lib chunk id : Clib extra functions
	.word	extra_vectors_begin(GOTOFF)
	.word	extra_vectors_end(GOTOFF)
	.word	0
	.word	0
	.word	-1			@ end of list

kernel_init_block:
	.word	Image$$RO$$Base(GOTOFF)		@ image base
	.word	RTSK$$Data$$Base(GOTOFF)	@ start of lang blocks
	.word	RTSK$$Data$$Limit(GOTOFF)	@ end of lang blocks

rwlimit:
	.word	Image$$RW$$Limit(GOTOFF)
stksiz:
	.weak	__root_stack_size
	.word	__root_stack_size
rwbase:
	.word	Image$$RW$$Base(GOTOFF)
zibase:
	.word	Image$$ZI$$Base(GOTOFF)

	.global	_Mod$Reloc$Off
.set	_Mod$Reloc$Off, 0x218
	.global	_Lib$Reloc$Off
.set	_Lib$Reloc$Off, 0x21c
	@ With CMunge, it converts to the constant 540 when adjusting the SL
	@ register in the module initialisation and command interface code.
	.global	_Lib$Reloc$Off$DP
.set	_Lib$Reloc$Off$DP, 0xf87

	@ RMA module entry point, called from the module initialisation
	@ code.
	@ On entry:
	@	R0 = 1, then allocate image space incl. 12 bytes for SCL
	@	   = 0, just allocate 12 bytes for SCL
	@ On exit:
	@	R0 = RMA module workspace ptr
	@	SL = base SVC + 560
	@ May return with VS set and R0 pointing to RISC OS error block.
	.global	_clib_initialisemodule
	.type	_clib_initialisemodule, %function
_clib_initialisemodule:
	STR	R14, [SP, #-4]!
	MOV	R9, R0		@ R9 => our module private word

	BL	__RelocCode	@ Relocate module code.

	@ Module RMA workspace:
	@   + 0 : size workspace (not really used)
	@   + 4 : SCL static relocation offset
	@   + 8 : client static relocation offset
	@   +12 : copy of Image$$RW$$Base -> Image$$ZI$$Base area followed by
	@         Image$$RW$$Limit - Image$$ZI$$Base number of zero bytes.

	@ Claim RMA space for module data
	MOV	R0, #6
	MOV	R4, #0
	MOV	R5, #0
	TEQ	R9, #0
	MOVEQ	R3, #12		@ Reserve 12 bytes for SCL statics
	LDRNE	R4, rwbase	@ Image$$RW$$Base
	LDRNE	R5, rwlimit	@ Image$$RW$$Limit
	SUBNE	R3, R5, R4
	ADDNE	R3, R3, #12	@ 12 bytes for SCL statics
	SWI	OS_Module + X_Bit
	@ On exit:	 R2 = pointer to claimed block
	@		 R3 = preserved (i.e. requested block size)
	LDRVS	PC, [SP], #4

	MOV	R9, R12		@ hold workspace pointer
	@ Set module private word to point to this new allocation
	STR	R2, [R12, #0]

	MOV	R12, R2
	@ First word of private word is size of allocated block
	STR	R3, [R12, #0]

	ADR	R0, stubs	@ R0 => list of stub descriptions
	ADD	R1, R12, #12	@ R1 => RMA workspace start
	ADD	R2, R12, R3
	LDR	R3, zibase	@ R3 = Image$$ZI$$base
				@ R4 = Image$$RW$$Base
				@ R5 = Image$$RW$$Limit
	LDR	R6, stksiz
	TEQ	R6, #0
	LDRNE	R6, [R6]	@ use __root_stack_size
	MOVEQ	R6, #4<<16	@ otherwise default to 4K for root stack chunk
	MOVNE	R6, R6, ASR#10	@ convert bytes to KB
	MOVNE	R6, R6, LSL#16	@ put in upper 16 bits

	TEQ	R0, R0		@ Set Z flag
	TEQ	PC, PC		@ EQ if running in 32-bit mode
	ORREQ	R6, R6, #1

	SWI	SharedCLibrary_LibInitModuleAPCS_32 + X_Bit
	BVS	_clib_initialisemodule_error

	@ R1 = stack base (preserve until the end !)
	@ R6 = library version number

	MOV	R6, R6, LSL#16
	CMP	R6, #6<<16
	ADRLT	R0, out_of_date_error
	BLT	_clib_initialisemodule_error

	@ Store the static relocation data offsets for SCL and client use in
	@ the first 12 bytes of module workspace.  The CMGH/CMunge module
	@ header interface assumes the position of these words.
	@ The client static relocation data offset becomes the value that we
	@ reference through SL[-536] for accessing global variables.
	LDR	R7, [R1, #20]	@ SCL static relocation data offset
	LDR	R8, [R1, #24]	@ Client static relocation data offset
	STR	R7, [R12, #4]	@ Store in private workspace
	STR	R8, [R12, #8]

	MOV	R4, R0		@ End of workspace (FIXME: really needed ?)

	@ Do additional relocation of RW data words pointing to other RW data
	@ words.  After this data relocation, those should point into our module
	@ RMA workspace instead of module code (or beyond that for .bss data).
	MOV	R0, R8		@ R0 = client static relocation data offset
	LDR	R2, rwbase	@ R2 = Image$$RW$$Base
	BL	__RelocData

	ADR	R0, kernel_init_block
	ADR	LR, _clib_initialisemodule_initclib
	STR	LR, [SP, #-4]!
	@ _kernel_moduleinit pops the return address off the stack
	B	_kernel_moduleinit
_clib_initialisemodule_initclib:
	STR	R9, [SP, #-4]!	@ Save workspace pointer
	BL	_clib_initialise
	ADDS	R0, R0, #0	@ Clear V flag
	LDMIA	SP!, {R0, PC}

_clib_initialisemodule_error:
	@ An error occurred:
	@  R0 => RISC OS error block
	@  R9 => module private word holding ptr to RMA memory block
	MOV	R1, R0		@ Preserve error pointer

	MOV	R0, #7		@ Free claimed workspace
	LDR	R2, [R9, #0]
	SWI	OS_Module + X_Bit
	MOV	R0, #0		@ Set private word pointer
	STR	R0, [R9, #0]

	MOV	R0, R1		@ Restore error pointer

	CMP	R0, #1<<31	@ Make sure V flag is set
	CMNVC	R0, #1<<31
	LDR	PC, [SP], #4	@ Exit

	.text
	@ Called by CMunge module start code.
	.global	_clib_entermodule
	.type	_clib_entermodule, %function
_clib_entermodule:
	ADR	R0, kernel_init_block
	@ Set stack size according to __root_stack_size, or 4K if it
	@ does not exist.
	LDR	R6, stksiz
	TEQ	R6, #0
	MOVEQ	R6, #4 * 1024
	LDRNE	R6, [R6, #0]
	MOV	R8, R12
	MOV	R12, #-1
	B	_kernel_entermodule

	@@ Area: RTSK$$Data
	.text
RTSK$$Data$$Base:
	.word	RTSK$$Data$$Limit - RTSK$$Data$$Base
	.word	Image$$RO$$Base(GOTOFF)		@ C$$code$$Base
	.word	Image$$RO$$Limit(GOTOFF)	@ C$$code$$Limit
	.word	language_name(GOTOFF)		@ Must be "C"
	.word	__sclmain(GOTOFF)		@ Our PROC-returning InitProc
	.word	0				@ Finaliser
	@ These are the "optionals"...
	.word	TrapHandler(GOTOFF)		@ SCL's own trap handler...
	.word	UncaughtTrapHandler(GOTOFF)	@ ...and uncaught trap proc
	.word	EventHandler(GOTOFF)		@ ...and event proc
	.word	UnhandledEventHandler(GOTOFF)	@ ...and unhandled event proc!
	@ No fast event proc
	@ No unwind proc
	@ No name proc
RTSK$$Data$$Limit:

language_name:
	.asciz	"C"
	.align

	.text
	.type	__sclmain, %function
__sclmain:
	@ FIXME: vvv what's this magic ?
	LDR	R0, kallocextendsws_data
	LDR	R1, [SL, #- _Mod$Reloc$Off]
	MOV	R2, #1
	STRB	R2, [R0, R1]

	MOV	R0, SP
	STR	LR, [SP, #-4]!
	LDR	R1, rtsk_data
	LDMIB	R1, {R1, R2}		@ fetch code start/end
	BL	_clib_initialise

	LDR	R0, c_run		@ get our main()
	TEQ	R0, #0			@ was there one?
	ADRNE	R0, c_next		@ yup, so point to hook to it
	LDR	PC, [R13], #4

kallocextendsws_data:			@ Ptr into .data, so needs static relocation data offset
	.word	_stub_kallocExtendsWS(GOTOFF)
rtsk_data:				@ Ptr into .text
	.word	RTSK$$Data$$Base(GOTOFF)
c_run:					@ Ptr into .text
	.weak	main
	.word	main(GOTOFF)

	@ This is called when all RTL blocks have been initialised via their
	@ InitProc
	.asciz	"___init"
	.align
	.word	0xff000008
	.type	c_next, %function
c_next:
	MOV	IP, SP
	STMFD	SP!, {FP, IP, LR, PC}
	SUB	FP, IP, #4
	BL	_kernel_command_string	@ Get R0 = command string
	LDR	R1, c_run		@ Get ptr to main() function
	BL	_main			@ Call clib to enter it.  SHOULD never return from here.
	LDMDB	FP, {FP, SP, PC}


	.text
kernel_vectors_begin:

	.global	_kernel_init
_kernel_init:			MOV	PC, #0
	.global	_kernel_exit
_kernel_exit:			MOV	PC, #0
	.global	_kernel_setreturncode
_kernel_setreturncode:		MOV	PC, #0
	.global	_kernel_exittraphandler
_kernel_exittraphandler:	MOV	PC, #0
	.global	_kernel_unwind
_kernel_unwind:			MOV	PC, #0
	.global	_kernel_procname
_kernel_procname:		MOV	PC, #0
	.global	_kernel_language
_kernel_language:		MOV	PC, #0
	.global	_kernel_command_string
_kernel_command_string:		MOV	PC, #0
	.global	_kernel_hostos
_kernel_hostos:			MOV	PC, #0
	.global	_kernel_swi
_kernel_swi:			MOV	PC, #0
	.global	_kernel_osbyte
_kernel_osbyte:			MOV	PC, #0
	.global	_kernel_osrdch
_kernel_osrdch:			MOV	PC, #0
	.global	_kernel_oswrch
_kernel_oswrch:			MOV	PC, #0
	.global	_kernel_osbget
_kernel_osbget:			MOV	PC, #0
	.global	_kernel_osbput
_kernel_osbput:			MOV	PC, #0
	.global	_kernel_osgbpb
_kernel_osgbpb:			MOV	PC, #0
	.global	_kernel_osword
_kernel_osword:			MOV	PC, #0
	.global	_kernel_osfind
_kernel_osfind:			MOV	PC, #0
	.global	_kernel_osfile
_kernel_osfile:			MOV	PC, #0
	.global	_kernel_osargs
_kernel_osargs:			MOV	PC, #0
	.global	_kernel_oscli
_kernel_oscli:			MOV	PC, #0
	.global	_kernel_last_oserror
_kernel_last_oserror:		MOV	PC, #0
	.global	_kernel_system
_kernel_system:			MOV	PC, #0
	.global	_kernel_getenv
_kernel_getenv:			MOV	PC, #0
	.global	_kernel_setenv
_kernel_setenv:			MOV	PC, #0
	.global	_kernel_register_allocs
_kernel_register_allocs:	MOV	PC, #0
	.global	_kernel_alloc
_kernel_alloc:			MOV	PC, #0
	.global	_kernel_stkovf_split_0frame
_kernel_stkovf_split_0frame:	MOV	PC, #0
	.global	_kernel_stkovf_split
_kernel_stkovf_split:		MOV	PC, #0
	.global	_kernel_stkovf_copyargs
_kernel_stkovf_copyargs:	MOV	PC, #0
	.global	_kernel_stkovf_copy0args
_kernel_stkovf_copy0args:	MOV	PC, #0
	.global	_kernel_udiv
_kernel_udiv:			MOV	PC, #0
	.global	_kernel_urem
_kernel_urem:			MOV	PC, #0
	.global	__rt_udiv10
__rt_udiv10:
	.global	_kernel_udiv10
_kernel_udiv10:			MOV	PC, #0
	.global	_kernel_sdiv
_kernel_sdiv:			MOV	PC, #0
	.global	_kernel_srem
_kernel_srem:			MOV	PC, #0
	.global	__rt_sdiv10
__rt_sdiv10:
	.global	_kernel_sdiv10
_kernel_sdiv10:			MOV	PC, #0
	.global	_kernel_fpavailable
_kernel_fpavailable:		MOV	PC, #0
	.global	_kernel_moduleinit
_kernel_moduleinit:		MOV	PC, #0
	.global	_kernel_irqs_on
_kernel_irqs_on:		MOV	PC, #0
	.global	_kernel_irqs_off
_kernel_irqs_off:		MOV	PC, #0
	.global	_kernel_irqs_disabled
_kernel_irqs_disabled:		MOV	PC, #0
	.global	_kernel_entermodule
_kernel_entermodule:		MOV	PC, #0
	.global	_kernel_escape_seen
_kernel_escape_seen:		MOV	PC, #0
	.global	_kernel_current_stack_chunk
_kernel_current_stack_chunk:	MOV	PC, #0
	.global	_kernel_swi_c
_kernel_swi_c:			MOV	PC, #0
	.global	_kernel_register_slotextend
_kernel_register_slotextend:	MOV	PC, #0
	.global	_kernel_raise_error
_kernel_raise_error:		MOV	PC, #0
kernel_vectors_mid:
	.space			kernel_vectors_mid - kernel_vectors_begin
kernel_vectors_end:

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

extra_vectors_begin:

	@.global	__fpclassifyf
__fpclassifyf_FIXME_float_arg:  MOV	PC, #0
	.global	__fpclassifyd
__fpclassifyd:			MOV	PC, #0
	.global	__signbitf
__signbitf:			MOV	PC, #0
	.global	__signbitd
__signbitd:			MOV	PC, #0
	.global	copysign
copysign:			MOV	PC, #0
	@.global	copysignf
copysignf_not_used:		MOV	PC, #0
	.global	nan
nan:				MOV	PC, #0
	.global	nanf
nanf:				MOV	PC, #0
	.global	nextafter
nextafter:			MOV	PC, #0
	@.global	nextafterf
nextafterf_FIXME_float_arg:	MOV	PC, #0
	.global	fdim
fdim:				MOV	PC, #0
	@.global	fdimf
fdimf_FIXME_float_arg:		MOV	PC, #0
	.global	fmax
fmax:				MOV	PC, #0
	@.global	fmaxf
fmaxf_FIXME_float_arg:		MOV	PC, #0
	.global	fmin
fmin:				MOV	PC, #0
	@.global	fminf
fminf_FIXME_float_arg:		MOV	PC, #0
	@.global	fabsf
fabsf_not_used:			MOV	PC, #0
	.global	hypot
hypot:				MOV	PC, #0
	.global	hypotf
hypotf:				MOV	PC, #0
	.global	feclearexcept
feclearexcept:			MOV	PC, #0
	.global	fegetexceptflag
fegetexceptflag:		MOV	PC, #0
	.global	feraiseexcept
feraiseexcept:			MOV	PC, #0
	.global	fesetexceptflag
fesetexceptflag:		MOV	PC, #0
	.global	fetestexcept
fetestexcept:			MOV	PC, #0
	.global	fegetround
fegetround:			MOV	PC, #0
	.global	fesetround
fesetround:			MOV	PC, #0
	.global	fegetenv
fegetenv:			MOV	PC, #0
	.global	feholdexcept
feholdexcept:			MOV	PC, #0
	.global	fesetenv
fesetenv:			MOV	PC, #0
	.global	feupdateenv
feupdateenv:			MOV	PC, #0
	.global	_snprintf
_snprintf:			MOV	PC, #0
	.global	snprintf
snprintf:			MOV	PC, #0
	.global	__gcc_vsnprintf
	.global	vsnprintf
__gcc_vsnprintf:
vsnprintf:			MOV	PC, #0
	.global	vfscanf
vfscanf:			MOV	PC, #0
	.global	vscanf
vscanf:				MOV	PC, #0
	.global	vsscanf
vsscanf:			MOV	PC, #0
	@.global	ceilf
ceilf_not_used:			MOV	PC, #0
	@.global	floorf
floorf_not_used:		MOV	PC, #0
	.global	nearbyint
nearbyint:			MOV	PC, #0
	@.global	nearbyintf
nearbyintf_FIXME_float_arg:	MOV	PC, #0
	.global	rint
rint:				MOV	PC, #0
	@.global	rintf
rintf_FIXME_float_arg:		MOV	PC, #0
	.global	lrint
lrint:				MOV	PC, #0
	@.global	lrintf
lrintf_FIXME_float_arg:		MOV	PC, #0
	.global	round
round:				MOV	PC, #0
	@.global	roundf
roundf_FIXME_float_arg:		MOV	PC, #0
	.global	lround
lround:				MOV	PC, #0
	@.global	lroundf
lroundf_FIXME_float_arg:	MOV	PC, #0
	.global	trunc
trunc:				MOV	PC, #0
	@.global	truncf
truncf_FIXME_float_arg:		MOV	PC, #0
	.global	remainder
remainder:			MOV	PC, #0
	@.global	remainderf
remainderf_FIXME_float_arg:	MOV	PC, #0
	.global	llabs
llabs:				MOV	PC, #0
	.global	lldiv
lldiv:				MOV	PC, #0
	.global	atoll
atoll:				MOV	PC, #0
	.global	strtoll
strtoll:			MOV	PC, #0
	.global	strtoull
strtoull:			MOV	PC, #0
	.global	imaxabs
imaxabs:			MOV	PC, #0
	.global	imaxdiv
imaxdiv:			MOV	PC, #0
	.global	strtoimax
strtoimax:			MOV	PC, #0
	.global	strtoumax
strtoumax:			MOV	PC, #0
	.global	__assert2
__assert2:			MOV	PC, #0
	.global	_Exit
_Exit:				MOV	PC, #0
	@.global	acosf
acosf_not_used:			MOV	PC, #0
	@.global	asinf
asinf_not_used:			MOV	PC, #0
	@.global	atanf
atanf_not_used:			MOV	PC, #0
	@.global	atan2f
atan2f_not_used:		MOV	PC, #0
	@.global	cosf
cosf_not_used:			MOV	PC, #0
	@.global	sinf
sinf_not_used:			MOV	PC, #0
	@.global	tanf
tanf_not_used:			MOV	PC, #0
	.global	acosh
acosh:				MOV	PC, #0
	@.global	acoshf
acoshf_FIXME_float_arg:		MOV	PC, #0
	.global	asinh
asinh:				MOV	PC, #0
	@.global	asinhf
asinhf_FIXME_float_arg:		MOV	PC, #0
	.global	atanh
atanh:				MOV	PC, #0
	@.global	atanhf
atanhf_FIXME_float_arg:		MOV	PC, #0
	@.global	expf
expf_not_used:			MOV	PC, #0
	.global	exp2
exp2:				MOV	PC, #0
	@.global	exp2f
exp2f_FIXME_float_arg:		MOV	PC, #0
	.global	expm1
expm1:				MOV	PC, #0
	@.global	expm1f
expm1f_FIXME_float_arg:		MOV	PC, #0
	@.global	frexpf
frexpf_FIXME_float_arg:		MOV	PC, #0
	.global	ilogb
ilogb:				MOV	PC, #0
	@.global	ilogbf
ilogbf_FIXME_float_arg:		MOV	PC, #0
	@.global	ldexpf
ldexpf_FIXME_float_arg:		MOV	PC, #0
	@.global	logf
logf_not_used:			MOV	PC, #0
	@.global	log10f
log10f_FIXME_float_arg:		MOV	PC, #0
	.global	log1p
log1p:				MOV	PC, #0
	@.global	log1pf
log1pf_FIXME_float_arg:		MOV	PC, #0
	.global	log2
log2:				MOV	PC, #0
	@.global	log2f
log2f_FIXME_float_arg:		MOV	PC, #0
	.global	logb
logb:				MOV	PC, #0
	@.global	logbf
logbf_FIXME_float_arg:		MOV	PC, #0
	@.global	modff
modff_not_used:			MOV	PC, #0
	@.global	fmodf
fmodf_FIXME_float_arg:		MOV	PC, #0
	.global	scalbn
scalbn:				MOV	PC, #0
	@.global	scalbnf
scalbnf_FIXME_float_arg:	MOV	PC, #0
	.global	scalbln
scalbln:			MOV	PC, #0
	@.global	scalblnf
scalblnf_FIXME_float_arg:	MOV	PC, #0
	.global	cbrt
cbrt:				MOV	PC, #0
	@.global	cbrtf
cbrtf_FIXME_float_arg:		MOV	PC, #0
	@.global	powf
powf_not_used:			MOV	PC, #0
	@.global	sqrtf
sqrtf_FIXME_float_arg:		MOV	PC, #0
	.global	erf
erf:				MOV	PC, #0
	@.global	erff
erff_FIXME_float_arg:		MOV	PC, #0
	.global	erfc
erfc:				MOV	PC, #0
	@.global	erfcf
erfcf_FIXME_float_arg:		MOV	PC, #0
	.global	lgamma
lgamma:				MOV	PC, #0
	@.global	lgammaf
lgammaf_FIXME_float_arg:	MOV	PC, #0
	.global	tgamma
tgamma:				MOV	PC, #0
	@.global	tgammaf
tgammaf_FIXME_float_arg:	MOV	PC, #0
	.global	nexttoward
nexttoward:			MOV	PC, #0
	@.global	nexttowardf
nexttowardf_FIXME_float_arg:	MOV	PC, #0
	.global	fmaf
fmaf:				MOV	PC, #0
extra_vectors_mid:
	.space			extra_vectors_mid - extra_vectors_begin
extra_vectors_end:


	.section ".bss"
Stub$$Data$$Base:

kernel_statics_begin:
	.space	0x31c
kernel_statics_end:
.set	_stub_imageBase, kernel_statics_begin + 0x0000
.set	_stub_rtsDataBase, kernel_statics_begin + 0x0004
.set	_stub_rtsDataLimit, kernel_statics_begin + 0x0008
.set	_stub_IIHandlerInData, kernel_statics_begin + 0x000c
.set	_stub_PAHandlerInData, kernel_statics_begin + 0x001c
.set	_stub_DAHandlerInData, kernel_statics_begin + 0x002c
.set	_stub_AEHandlerInData, kernel_statics_begin + 0x003c
.set	_stub_undStack, kernel_statics_begin + 0x004c
.set	_stub_svcStack, kernel_statics_begin + 0x0050
.set	_stub_unused4, kernel_statics_begin + 0x0054
.set	_stub_registerDump, kernel_statics_begin + 0x0078
.set	_stub_oldAbortHandlers, kernel_statics_begin + 0x00bc
.set	_stub_oldExitHandler, kernel_statics_begin + 0x00cc
.set	_stub_oldMemoryLimit, kernel_statics_begin + 0x00d4
.set	_stub_oldErrorHandler, kernel_statics_begin + 0x00d8
.set	_stub_oldErrorR0, kernel_statics_begin + 0x00dc
.set	_stub_oldErrorBuffer, kernel_statics_begin + 0x00e0
.set	_stub_oldCallBackHandler, kernel_statics_begin + 0x00e4
.set	_stub_oldEscapeHandler, kernel_statics_begin + 0x00f0
.set	_stub_oldEventHandler, kernel_statics_begin + 0x00f8
.set	_stub_oldUpCallHandler, kernel_statics_begin + 0x0100
.set	_stub_languageEnvSave, kernel_statics_begin + 0x0108
.set	_stub_hadEscape, kernel_statics_begin + 0x0114
.set	_stub_kallocExtendsWS, kernel_statics_begin + 0x0115
.set	_stub_inTrapHandler, kernel_statics_begin + 0x0116
.set	_stub_beingDebugged, kernel_statics_begin + 0x0117
.set	_stub_fpPresent, kernel_statics_begin + 0x0118
.set	_stub_initialised, kernel_statics_begin + 0x0119
.set	_stub_callbackInactive, kernel_statics_begin + 0x011a
.set	_stub_unused_byte_2, kernel_statics_begin + 0x011b
.set	_stub_errorBuffer, kernel_statics_begin + 0x011c
.set	_stub_errorNumber, kernel_statics_begin + 0x0120
.set	_stub_errorString, kernel_statics_begin + 0x0124
.set	_stub_FatalErrorBuffer, kernel_statics_begin + 0x0220
.set	_stub_eventCode, kernel_statics_begin + 0x0220
.set	_stub_eventUserR13, kernel_statics_begin + 0x0224
.set	_stub_eventRegisters, kernel_statics_begin + 0x0228
.set	_stub_unused2, kernel_statics_begin + 0x0258
.set	_stub_heapTop, kernel_statics_begin + 0x0284
.set	_stub_heapLimit, kernel_statics_begin + 0x0288
.set	_stub_allocProc, kernel_statics_begin + 0x028c
.set	_stub_freeProc, kernel_statics_begin + 0x0290
.set	_stub_returnCode, kernel_statics_begin + 0x0294
.set	_stub_moduleDataWord, kernel_statics_begin + 0x0298
.set	_stub_ClientFlags, kernel_statics_begin + 0x029c
.set	_stub_escapeSeen, kernel_statics_begin + 0x029d
.set	_stub_unwinding, kernel_statics_begin + 0x029e
.set	_stub_underDesktop, kernel_statics_begin + 0x029f
.set	_stub_heapBase, kernel_statics_begin + 0x02a0
.set	_stub_ArgString, kernel_statics_begin + 0x02a4
.set	_stub_heapExtender, kernel_statics_begin + 0x02a8
.set	_stub_knownSlotSize, kernel_statics_begin + 0x02ac
.set	_stub_initSlotSize, kernel_statics_begin + 0x02b0
.set	_stub_lk_RestoreOSHandlers, kernel_statics_begin + 0x02b4
.set	_stub_extendChunk, kernel_statics_begin + 0x02b8
.set	_stub_rootStackChunk, kernel_statics_begin + 0x02bc
.set	_stub_pc_hex_buff, kernel_statics_begin + 0x02c0
.set	_stub_returnCodeLimit, kernel_statics_begin + 0x02c0
.set	_stub_reg_hex_buff, kernel_statics_begin + 0x02cc
	.global	disable_stack_extension
.set	disable_stack_extension, kernel_statics_begin + 0x02d8
.set	_stub_unused, kernel_statics_begin + 0x02dc

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

Stub$$Data$$Limit:
