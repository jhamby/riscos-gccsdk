@ Stub routines for linking with the SharedCLibrary
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

.set	OS_GenerateError, 0x2B
.set	OS_GetEnv, 0x10
.set	OS_Module, 0x1E

.set	X_Bit, 0x20000

.set	SharedCLibrary_LibInitAPCS_32, 0x80683
.set	SharedCLibrary_LibInitModuleAPCS_32, 0x80684

	.macro	MakePtr ptrname
#ifdef __TARGET_MODULE__
	.word	\ptrname(GOTOFF)
#else
	.word	\ptrname
#endif
	.endm

#ifndef __TARGET_MODULE__
	.global	_start
	.type	_start, %function
_start:
	B	__main
#endif

	.text

#ifndef __TARGET_MODULE__
	@ For application code only.

	.global	__main
	.type	__main, %function
__main:
	SWI	OS_GetEnv
	MOV	R2, R1			@ workspace limit
	LDR	R0, stubs		@ list of stubs
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
#endif
	@ Used by application and module code.
out_of_date_error:
	.word	0x800e91
	.asciz	"Shared C Library not loaded or out of date"
	.align

kernel_init_block:
	MakePtr	Image$$RO$$Base		@ image base
	MakePtr	RTSK$$Data$$Base	@ start of lang blocks
	MakePtr	RTSK$$Data$$Limit	@ end of lang blocks

stubs:
	MakePtr	__chunkstub
rwlimit:
	MakePtr	Image$$RW$$Limit
stksiz:
	.weak	__root_stack_size
	.word	__root_stack_size

#ifdef __TARGET_MODULE__
	@ For module code.

	.global	_Mod$Reloc$Off
.set	_Mod$Reloc$Off, 0x218
	.global	_Lib$Reloc$Off
.set	_Lib$Reloc$Off, 0x21c
	@ With CMunge, it converts to the constant 540 when adjusting the SL
	@ register in the module initialisation and command interface code.
	.global	_Lib$Reloc$Off$DP
.set	_Lib$Reloc$Off$DP, 0xf87

rwbase:
	MakePtr	Image$$RW$$Base
zibase:
	MakePtr	Image$$ZI$$Base

	@ RMA module entry point, called from the module initialisation
	@ code.
	@ On entry:
	@	R0 = 1, then allocate image space incl. 12 bytes for SCL
	@	   = 0, just allocate 12 bytes for SCL
	@ On exit:
	@	R0 = RMA module workspace ptr
	@	SL = base SVC + 540
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
	MOVS	R3, R9
	LDRNE	R4, rwbase	@ Image$$RW$$Base
	LDRNE	R5, rwlimit	@ Image$$RW$$Limit
	SUBNE	R3, R5, R4
	ADD	R3, R3, #12	@ 12 bytes for SCL statics
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

	LDR	R0, stubs	@ R0 => list of stub descriptions
	ADD	R1, R12, #12	@ R1 => RMA workspace start
	ADD	R2, R12, R3	@ R2 => RMA workspace end
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
	STMIB	R12, {R7-R8}	@ Store in private workspace

	MOV	R4, R0		@ End of workspace (FIXME: really needed ?)

	@ Do additional relocation of RW data words pointing to other RW data
	@ words.  After this data relocation, those should point into our module
	@ RMA workspace instead of module code (or beyond that for .bss data).
	MOV	R0, R8		@ R0 = client static relocation data offset
	LDR	R2, rwbase	@ R2 = Image$$RW$$Base
	BL	__RelocData

	ADR	R0, kernel_init_block
	BL	_kernel_moduleinit_stub
	STR	R9, [SP, #-4]!	@ Save workspace pointer
	BL	_clib_initialise
	ADDS	R0, R0, #0	@ Clear V flag
	LDMIA	SP!, {R0, PC}

	@ This stub ensures that apart from the return address the processor
	@ mode is saved on the SP stack as well when being in SVC26 mode as
	@ this avoids a problem in some of the prerelease Select 4 SCL
	@ versions.
_kernel_moduleinit_stub:
	@ _kernel_moduleinit pops the return address off the stack
	STR	R14, [SP, #-4]!
	B	_kernel_moduleinit

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
	LDRNE	R6, [R6, #0]    @ R6 requested root stack size
	MOV	R8, R12		@ R8 module pw ptr
	MOV	R12, #-1	@ -1 indicate new stubs, was otherwise module pw ptr
	B	_kernel_entermodule
	.size	_clib_entermodule, . - _clib_entermodule
#endif

	@@ Area: RTSK$$Data
	.text
RTSK$$Data$$Base:
	.word	RTSK$$Data$$Limit - RTSK$$Data$$Base
	MakePtr	Image$$RO$$Base		@ C$$code$$Base
	MakePtr	Image$$RO$$Limit	@ C$$code$$Limit
	MakePtr	language_name		@ Must be "C"
	MakePtr	__sclmain		@ Our PROC-returning InitProc
	.word	0				@ Finaliser
	@ These are the "optionals"...
	MakePtr	TrapHandler		@ SCL's own trap handler...
	MakePtr	UncaughtTrapHandler	@ ...and uncaught trap proc
	MakePtr	EventHandler		@ ...and event proc
	MakePtr	UnhandledEventHandler	@ ...and unhandled event proc!
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
	@ Allow malloc() to grow the WimpSlot in Absolute and Module
	@ applications.
	LDR	R0, kallocextendsws_data
#ifdef __TARGET_MODULE__
	LDR	R1, [SL, #- _Mod$Reloc$Off]
	MOV	R2, #1
	STRB	R2, [R0, R1]
#else
	MOV	R1, #1
	STRB	R1, [R0, #0]
#endif

	MOV	R0, SP
	STR	LR, [SP, #-4]!
	LDR	R1, rtsk_data
	LDMIB	R1, {R1, R2}		@ fetch code start/end
	BL	_clib_initialise

	@ Modules can have a main(), application always do have a main()
#ifdef __TARGET_MODULE__
	LDR	R0, c_run_weak		@ get our main()
	TEQ	R0, #0			@ was there one?
	ADRNE	R0, ___init		@ yup, so point to hook to it
#else
	ADR	R0, ___init
#endif
	LDR	PC, [R13], #4

kallocextendsws_data:			@ Ptr into .data, so needs static relocation data offset
	MakePtr	_stub_kallocExtendsWS
rtsk_data:				@ Ptr into .text
	MakePtr	RTSK$$Data$$Base
#ifdef __TARGET_MODULE__
c_run_weak:				@ Ptr into .text
	.weak	main
	.word	main			@ *NO* GOTOFF as it breaks .weak test
#endif
c_run:					@ Ptr into .text
#ifdef __TARGET_MODULE__
	MakePtr	main
#else
	MakePtr	___main
#endif
	.size	__sclmain, . - __sclmain

	@ This is called when all RTL blocks have been initialised via their
	@ InitProc
	.asciz	"___init"
	.align
	.word	0xff000008
	.type	c_next, %function
___init:
	MOV	IP, SP
	STMFD	SP!, {FP, IP, LR, PC}
	SUB	FP, IP, #4
	BL	_kernel_command_string	@ Get R0 = command string
	LDR	R1, c_run		@ Get ptr to main() function
	BL	_main			@ Call clib to enter it.  SHOULD never return from here.
	LDMDB	FP, {FP, SP, PC}
	.size	___init, . - ___init

#ifndef __TARGET_MODULE__
	.asciz	"___main"
	.align
	.word	0xff000008
	.type	___main, %function
___main:
	MOV	IP, SP
	STMFD	SP!, {R0-R3, FP, IP, LR, PC}
	SUB	FP, IP, #4

	@ R0-R3 are saved on the stack to prevent the arguments to main() from being corrupted.
	LDR	R0, =_fini
	BL	atexit			@ Register static global destructors for calling on exit.
	BL	_init			@ Call static global constructors.

	LDMDB	FP, {R0-R3, FP, SP, LR}
	B	main
	.size	___main, . - ___main
#endif

	.end
