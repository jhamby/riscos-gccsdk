;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_syslib.s,v $
; $Date: 2003/07/29 23:04:27 $
; $Revision: 1.21 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	; Keep in sync with error_table.
NO_MEMORY   * 0
NO_CALLASWI * 1
NO_SUL      * 2
NO_FPE      * 3

; Constants for field offsets within a stack chunk
; The stack is allocated in chunks, each chunk is a multiple of 4KB, and
; contains a 24 byte header.
; The next and prev fields form a doubly linked list of allocated chunks.
; This is fairly similar to how the scl's stack works
; (see PRMs, 4-239 and Appendix C)
;
;  ______
; |      | +0
; |Header|
; |      |
; |______|
; |      | +24
; |      |
; |      | +536 <- sl
; |      |
; |      |
; ________/
;/
; |      |
; |______|
;         +4096 * n  <- sp (initially)
;
CHUNK_MAGIC	*	0	; Magic number to help detect if someone overwrites the stack
CHUNK_NEXT	*	4	; Ptr to next chunk
CHUNK_PREV	*	8	; Ptr to previous chunk
CHUNK_SIZE	*	12	; Size of chunk, including header
CHUNK_DEALLOC	*	16	; Function to call to free the chunk
CHUNK_RETURN	*	20	; Return address after freeing this chunk

CHUNK_OVERHEAD	*	24	; Size of chunk header

STACK_CHECK_MAGIC	*	1	; Should we check that the magic number is valid each time the stack is extended/shrunk
EXTREMELY_PARANOID	*	0	; Should we check that the entire stack chunk chain is valid each time the stack is extended/shrunk

	AREA	|C$$code|, CODE, READONLY

	IMPORT  |__unixinit|            ;C function (unix/unix.c)
	IMPORT  |exit|                  ;C function (unix/unix.c)
	IMPORT	|__stackalloc_init|     ;C function (sys/stackalloc.c)
	IMPORT	|__stackalloc|          ;C function (sys/stackalloc.c)
	IMPORT	|__stackfree|           ;C function (sys/stackalloc.c)
	IMPORT  |__cbreg|               ;Data (signal/_signal.s)
	IMPORT  |__h_sigill|            ;ASM function (signal/_signal.s)
	IMPORT  |__h_sigsegv0|          ;ASM function (signal/_signal.s)
	IMPORT  |__h_sigsegv1|          ;ASM function (signal/_signal.s)
	IMPORT  |__h_sigbus|            ;ASM function (signal/_signal.s)
	IMPORT  |__h_error|             ;ASM function (signal/_signal.s)
	IMPORT  |__h_cback|             ;ASM function (signal/_signal.s)
	IMPORT  |__h_sigint|            ;ASM function (signal/_signal.s)
	IMPORT  |__h_event|             ;ASM function (signal/_signal.s)
	IMPORT  |__h_exit|              ;ASM function (signal/_signal.s)
	IMPORT  |__pthread_system_running| ;variable (pthread/_context.s)
	IMPORT  |__pthread_disable_ints|   ;ASM function (pthread/_ints.s)
	IMPORT  |__pthread_enable_ints|    ;ASM function (pthread/_ints.s)
	IMPORT  |__executing_signalhandler|;variable (signal/_signal.s)
	IMPORT  |__signalhandler_sp|       ;variable (signal/_signal.s)
	IMPORT  |__signalhandler_sl|       ;variable (signal/_signal.s)

	IMPORT	|_main|
	IMPORT	|__dynamic_no_da|, WEAK
	IMPORT	|__dynamic_da_name|, WEAK

|rmensure|
	DCB "RMEnsure SharedUnixLibrary 1.00 RMLoad System:Modules.SharedULib", 0
	ALIGN

	EXPORT	|__main|
	NAME	__main

	ENTRY
|__main|
	; Read environment parameters
	; On exit:
	;   a1 = pointer to environment string
	;   a2 = permitted RAM limit (highest address available + 1)
	;   a3 = pointer to real time the program was started (5 bytes)
	SWI	XOS_GetEnv

	; struct_base is the start of our memory environment variables
	; See the end of this file.  For the initialisation here, we
	; will always use ip as the base register.
	LDR	ip, =struct_base

	STR	a1, [ip, #0]	; __cli = pointer to command line
	STR	a2, [ip, #4]	; __himem = permitted RAM limit

	LDMIA	a3, {a1, a2}	; Get time
	STR	a1, [ip, #8]	; __time (low word)
	STR	a2, [ip, #12]	; __time (high word)

	MVNS	a1, #0          ; ensure a flag is set
	TEQ	pc, pc          ; EQ if in 32-bit mode, NE if 26-bit
	STREQ   a1, [ip, #76]   ; *** HACK HACK for objasm: was "[__32bit - struct_base]"   ; __32bit

	MOV	a1, #14	; Read appspace value
	MOV	a2, #0
	MOV	a3, #0
	SWI	XOS_ChangeEnvironment
	STR	a2, [ip, #72]	; Default value of __real_himem is the size of application space

	; For a description of the memory layout of a UnixLib application
	; see sys/brk.c.
	LDR	a1, [ip, #20]	; __robase
	LDR	a2, [ip, #24]	; __rwlimit

	STR	a1, [ip, #28]	; __base = __robase
	STR	a2, [ip, #32]	; __lomem = __rwlimit
	STR	a2, [ip, #36]	; __break = __rwlimit
	STR	a2, [ip, #40]	; __stack_limit = __rwlimit
	STR	a2, [ip, #48]	; __real_break = __rwlimit

	; The stack is allocated in chunks in the wimpslot, with the first 4KB
	; chunk immediately below __himem.  We cannot place it in a dynamic
	; area because GCC might generate trampolines.
	; Trampolines (for the un-initiated) are little code fragments that
	; execute in stack space.

	LDR	sp, [ip, #4]	; __himem
	; 8 bytes are needed above the initial chunk
	; for the stackalloc heap
	; Reserve the top 4K for the signal handler stack
	LDR	a1, =|__signalhandler_sp|
	SUB	sp, sp, #8
	STR	sp, [a1]
	SUB	sp, sp, #4096
	LDR	a1, =|__signalhandler_sl|
	ADD	sl, sp, #512	; The signal handler stack cannot be extended,
	STR	sl, [a1]	; so we don't need to setup the chunk structure
	SUB	a1, sp, #4096
	ADD	sl, a1, #512 + CHUNK_OVERHEAD

	SUB	a3, a1, #8
	; __stackalloc_init needs a minimum of 8 bytes below the initial
	; chunk for its heap - check this doesn't overlap the code section
	STR	a3, [ip, #16]	; __stack = bottom of stack
	CMP	a3, a2
	BCC	exit_with_error_no_memory	; No room for stack, exit.

	; Initialise the first stack chunk
	MOV	a2, #0
	STR	a2, [a1, #CHUNK_PREV]
	STR	a2, [a1, #CHUNK_NEXT]
	STR	a2, [a1, #CHUNK_DEALLOC]
	STR	a2, [a1, #CHUNK_RETURN]
	LDR	a2, |__stackchunk_magic_number|
	STR	a2, [a1, #CHUNK_MAGIC]
	MOV	a2, #4096
	STR	a2, [a1, #CHUNK_SIZE]

	MOV	v1, ip		; Temporary variable

	; Check for the existence of CallASWI.  Terminate the process
	; with an appropriate error message, if it doesn't exist.
	ADR	a1, error_no_callaswi
	MOV	a2, a1
	MOV	ip, #OS_GenerateError
	ORR	ip, ip, #X_Bit
	SWI	XOS_CallASWIR12
	; If CallASWI is unknown then R0 will point to a different address,
	; the error block for Unknown SWI
	CMP	a2, a1
	MOVNE	a1, #NO_CALLASWI
	BNE	exit_with_error

	MOV	ip, v1		; Restore ip.

	LDR	a1, =|rmensure|
	SWI	XOS_CLI
	MOVVS	a1, #NO_SUL
	BVS	exit_with_error

	; use of da's explicitly overridden if __dynamic_no_da is declared
	LDR	a1, =|__dynamic_no_da|
	TEQ	a1, #0
	BNE	no_dynamic_area

	; check OS version for RISC OS 3.5+
	MOV	a1, #129
	MOV	a2, #0
	MOV	a3, #255
	SWI	XOS_Byte
	BVS	no_dynamic_area
	CMP	a2, #165
	BCC	no_dynamic_area

	; use old dynamic area if possible
	; if app mem size = mem size, then can't be a spawned program
	LDR	a1, [ip, #4]	; __himem
	LDR	a2, [ip, #72]	; __real_himem
	CMP	a2, a1
	BEQ	no_old_area
	; validate numbers at top of application memory (see sys.s._exec)
	LDMIA	a1, {a1, a2, a3, a4, v1, v2, v3, v4}
	EOR	a3, a3, a1, ROR #7
	CMP	a3, a1
	EOREQ	v1, v1, a1, ROR #13
	CMPEQ	v1, a1
	EOREQ	v3, v3, a1, ROR #23
	CMPEQ	v3, a1
	EOREQ	a4, a4, a2, ROR #7
	CMPEQ	a4, a2
	EOREQ	v2, v2, a2, ROR #13
	CMPEQ	v2, a2
	EOREQ	v4, v4, a2, ROR #23
	CMPEQ	v4, a2
	BNE	no_old_area
	MOV	v6, a1		; save new __lomem
	MOV	a1, #2
	SWI	XOS_DynamicArea
	BVS	no_old_area	; bail out if SWI failed
	; check new __lomem = end of dynamic area
	ADD	a4, a4, a3
	TEQ	a4, v6
	BNE	no_old_area	; bail out if sizes mismatch
	MOV	v6, #0
	B	da_found


no_old_area
	; Allow the user the option of setting their own name for the
	; dyanmic area used as a heap.   If the variable __dynamic_da_name
	; exists, then it must be a char pointer (not an array) to an
	; alternate name.  DAs are always used in this case, and there's
	; no need to set a $heap variable.

	; The main use of this is when the binary is called !RunImage.
	; e.g.:  const char *__dynamic_da_name = "Nettle Heap";

	LDR	v5, =|__dynamic_da_name|
	TEQ	v5, #0
	LDRNE	v5, [v5, #0]	; get the actual string referred to
	BNE	t07

	; area name is program name + "$Heap"
	LDR	a1, [ip, #0]	; __cli
	MOV	a2, a1
	; search for space or end of cli string
t01
	LDRB	a3, [a1], #1
	CMP	a3, #" "
	BGT	t01
	SUB	a1, a1, #1	; back up to point at terminator char

	; use a maximum of 10 characters from the program name
	LDR	v5, =dynamic_area_name_end

	SUB	a4, a1, #10
	CMP	a4, a2
	MOVHI	a2, a4
	; a2 = max (a2, a4) - ie
	; 10 chars from end of name if length name > 10
	; else start of name

	; So, descending copy from a1 to v5
	; limit is a2, terminate early if we find "." or ":"
t02
	LDRB	a4, [a1, #-1]!
	CMP	a4, #"."
	CMPNE	a4, #':'
	STRNEB	a4, [v5, #-1]!
	BEQ	t03		; Not sure if some very slick conditionals
	CMP	a1, a2		; can eliminate that branch
	BHI	t02		; limit not yet reached
t03

	; check environment variable for creating a DA
	MOV	a1, v5
	MOV	a3, #-1
	MOV	a4, #0
	MOV	v1, #0
	SWI	XOS_ReadVarVal
	CMP	a3, #0
	BGE	no_dynamic_area

t07
	; create dynamic area
	MOV	a1, #0
	MOV	a2, #-1
	; set initial size of dynamic area to 0K
	MOV	a3, #0
	MOV	a4, #-1
	MOV	v1, #&80
	MOV	v2, #-1			; maximum size is total RAM in m/c
	MOV	v3, #0
	MOV	v4, #0
	; v5 is already set from above
	SWI	XOS_DynamicArea
	BVS	exit_with_error_no_memory	; no DA, report and exit
	MOV	v6, a3				; setup for deletion at exit
da_found
	STR	a2, [ip, #64]	; __dynamic_num

	; v6 is size left in area, a4 is start offset
	ADD	a1, v6, a4
	STR	a4, [ip, #32]	; __lomem = start of dynamic area
	STR	a1, [ip, #36]	; __break = end of used part of DA
	STR	a1, [ip, #48]	; __real_break = end of used part of DA


	; if current size == passed in size, then delete DA at exit
	; this means a spawning program must ensure that some of area is used
	; which is always true, since 8K of heap is initially allocated
	; FIXME - if we record the initial DA limit, then we can shrink the
	; dynamic area when the spawned program exits.
	LDR	v3, =dynamic_deletion	; Could move this above but code would
					; be less maintainable.
	CMP	a3, v6
	MOVEQ	v5, #1
	MOVNE	v5, #0
	STR	v5, [v3, #0]

no_dynamic_area

	MOV	fp, #0

	; This line is needed for the GNU Objective-C runtime system.
	; We need a safe 128 bytes of stack space to prevent a program
	; crashing when the library does a direct copy off the stack.
	SUB	sp, sp, #128

	; Find out whether we are executing within a TaskWindow or not.
	MOV	a1, #0
	SWI	XTaskWindow_TaskInfo
	MOVVS	a1, #0
	STR	a1, [ip, #56]	; __taskwindow

	; Find out whether we are executing as a WIMP program or not.
	MOV     a1, #3		; Text output mode, or desktop mode ?
	SWI     XWimp_ReadSysInfo
	MOVVS	a1, #0
	TEQ	a1, #0
	MOVNE	a1, #5		; When desktop mode, get the taskhandle
	SWINE	XWimp_ReadSysInfo
	STR	a1, [ip, #60]	; __taskhandle

	; Recognise the Floating Point facility by determining whether
	; the SWI FPEmulator_Version actually exists (and works).
	; If it does, then there is a floating point ability.
	SWI	XFPEmulator_Version
	MOVVS	a1, #0
	STR	a1, [ip, #52]	; __fpflag
	CMP	a1, #400	; We want 4.00 or above so we can use SFM/LFM
	MOVLT	a1, #NO_FPE
	BLT	exit_with_error

	; Now we'll initialise the C library, then call the user program.

	; Check the environment variable UnixLib$env.  If set, then
	; it will be an integer pointer to the parent process structure.
	; If we have successfully read the variable, then immediately
	; delete it so that we don't confuse any other forking processes.
	ADR	a1, env		; Environment variable name
	ADD	a2, ip, #68	; pointer to __u
	MOV	a3, #4		; buffer length is 4 bytes
	MOV	a4, #0		; context pointer (0 = first call)
	MOV	v1, #1		; variable type is number
	SWI	XOS_ReadVarVal
	LDR	a1, [ip, #68]	; If __u is non-zero, then variable existed
	CMP	a1, #0		; so delete it.
	ADRNE	a1, env		; Environment variable name
	MVNNE	a3, #0		; Set negative to delete variable
	MOVNE	a4, #0		; context pointer (0 = first call)
	MOVNE	v1, #1		; variable type is number
	SWINE	XOS_SetVarVal

	; Read the current RISC OS environment handler state
	BL	|__env_read|
	; Install the Unixlib environment handlers
	BL	|__env_unixlib|
	; Initialise the stack heap in application space
	BL	|__stackalloc_init|
	; Initialise the UnixLib library
	BL	|__unixinit|
	; Run the users program.
	BL	|_main|
	; C programs always terminate by calling exit.
	B	exit

env
	DCB	"UnixLib$env", 0
	ALIGN

exit_with_error_no_memory
	MOV	a1, #NO_MEMORY
	; a1 contains an index to the error to print.
exit_with_error
	ADR	a2, error_table
	LDR	a1, [a2, a1, LSL #2]
	SWI	XOS_Write0
	MOV	a1, #0
	LDR	a2, exit_word
	MOV	a3, #1
	SWI	OS_Exit

error_table
	DCD	error_no_memory
	DCD	error_no_callaswi
	DCD	error_no_sharedunixlib
	DCD	error_no_fpe

error_no_callaswi
	DCB	"Module CallASWI is not present.", 13, 10, 0
error_no_memory
	DCB	"Insufficient memory for application", 13, 10, 0
error_no_sharedunixlib
	DCB	"This application requires version 1.00 of the SharedUnixLibrary module", 13, 10, 0
error_no_fpe
	DCB	"This application requires version 4.00 or later of the FPEmulator module", 13, 10, 0
	ALIGN

check_for_callaswi

	IMPORT	|__munmap_all|
	EXPORT	|__dynamic_area_exit|
	NAME	__dynamic_area_exit
|__dynamic_area_exit|
	STMFD	sp!, {lr}
	BL	|__munmap_all|		; this must be done here for exec.c
	LDR	a2, =dynamic_deletion
	LDMFD	sp!, {lr}
	LDR	a2, [a2]
	TEQ	a2, #1
	return	NE, pc, lr
	LDR	a2, =|__dynamic_num|
	MOV	a4, lr
	LDR	a2, [a2]
	MOV	a1, #1
	CMP	a2, #-1
	SWINE	XOS_DynamicArea
	return	AL, pc, a4

	EXPORT	|__exit|
	NAME	__exit
|__exit|
	MOV	v1, a1
	BL	|__dynamic_area_exit|
	BL	|__env_riscos_and_wimpslot|
	LDR	a3, =|__sharedunixlibrary_key|
	LDR	a3, [a3]
	SWI	XSharedUnixLibrary_DeRegisterUpCall
	; re-enable Escape (in case SIGINT handler fired in ttyicanon)
	MOV	a1, #229
	MOV	a2, #0
	MOV	a3, #0
	SWI	XOS_Byte
	MOV	a3, v1
	LDR	a2, exit_word
	MOV	a1, #0
	SWI	OS_Exit		; - never returns

exit_word
	DCD	&58454241

	EXPORT	|__exit_no_code|
	NAME	__exit_no_code
|__exit_no_code|
	BL	|__dynamic_area_exit|
	BL	|__env_riscos_and_wimpslot|
	MOV	a1, #0
	MOV	a2, #0
	MOV	a3, #0
	SWI	OS_Exit		; - never returns

	IMPORT	|__vret|, WEAK
	EXPORT	|___vret|
|___vret|
	DCD	|__vret|

	; Restore original wimpslot, appspace and himem limits, and original
	; RISC OS environment handlers
|__env_riscos_and_wimpslot|
	; As we are reducing the wimpslot, the stack pointer may end up
	; pointing to invalid memory, so we need to reset it.
	; This will overwrite some of the original stack, but this is
	; only called just before exiting so won't be a problem
	LDR	a1, =|__calling_environment|
	LDR	sp, [a1]	; Original himem value

	STMFD	sp!, {v1, lr}
	ADD	v1, a1, #14*3*4

	LDR	a1, [v1]	; Original appspace value
	SUB	a1, a1, #&8000
	MOV	a2, #-1
	SWI	XWimp_SlotSize

	MOV	a1, #14
	LDMIA	v1, {a2, a3, a4}
	SWI	XOS_ChangeEnvironment
	MOV	a1, #0
	SUB	v1, v1, #14*3*4
	LDMIA	v1, {a2, a3, a4}
	SWI	XOS_ChangeEnvironment

	LDMFD	sp!, {v1, lr}
	; Fall through to __env_riscos

	EXPORT	|__env_riscos|
	; Restore original RISC OS environment handlers
|__env_riscos|
	STMFD	sp!, {v1, v2, lr}

	MOV	v1, #0
	LDR	v2, =|__calling_environment|
t04
	MOV	a1, v1
	LDMIA	v2!, {a2, a3, a4}
	CMP	a1, #0  ; Don't restore the himem limit
	CMPNE	a1, #14 ; Don't restore the appspace limit
	SWINE	XOS_ChangeEnvironment
	ADD	v1, v1, #1
	CMP	v1, #17		;  __ENVIRONMENT_HANDLERS
	BLT	t04
	stackreturn	AL, "v1, v2, pc"

	; Get current environment handler setup
	EXPORT	|__env_read|
	NAME	__env_read
|__env_read|
	STMFD	sp!, {a1, a2, a3, a4, v1, v2, lr}
	MOV	v1, #0
	LDR	v2, =|__calling_environment|
t05
	MOV	a1, v1
	MOV	a2, #0
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment
	STMIA	v2!, {a2, a3, a4}
	ADD	v1, v1, #1
	CMP	v1, #17
	BLT	t05
	LDMFD	sp!, {a1, a2, a3, a4, v1, v2, pc}

	IMPORT	|__ul_errbuf|
	; Install the Unixlib environment handlers
	EXPORT	|__env_unixlib|
	NAME	__env_unixlib
|__env_unixlib|
	STMFD	sp!, {a1, a2, a3, a4, v1, v2, lr}
	SWI	XOS_IntOff
	MOV	v1, #0
	ADR	v2, handlers
t06
	MOV	a1, v1
	LDR	a2, [v2], #4
	MOV	a3, #0
	MOV	a4, #0
	CMP	a1, #6
	LDREQ	a4, =|__ul_errbuf|
	CMP	a1, #7
	LDREQ	a4, =|__cbreg|
	SWI	XOS_ChangeEnvironment
	ADD	v1, v1, #1
	CMP	v1, #17
	BLT	t06

	LDR	r1, =|__env_riscos_and_wimpslot|
	SWI	XSharedUnixLibrary_RegisterUpCall
	LDR	r0, =|__sharedunixlibrary_key|
	STR	r2, [r0]
	MOV	r0, #16
	SWI	XOS_ChangeEnvironment

	SWI	XOS_IntOn
	LDMFD	sp!, {a1, a2, a3, a4, v1, v2, pc}

handlers
	DCD	0		; Memory limit
	DCD	|__h_sigill|	; Undefined instruction
	DCD	|__h_sigsegv0|	; Prefetch abort
	DCD	|__h_sigsegv1|	; Data abort
	DCD	|__h_sigbus|	; Address exception
	DCD	0		; Other exception
	DCD	|__h_error|	; Error
	DCD	|__h_cback|	; Callback
	DCD	0		; Breakpoint
	DCD	|__h_sigint|	; Escape
	DCD	|__h_event|	; Event
	DCD	|__h_exit|	; Exit
	DCD	0		; Unknown SWI
	DCD	0		; Exception registers
	DCD	0		; Application space
	DCD	0		; Currently active object
	DCD	0		; UpCall

	EXPORT	|__sharedunixlibrary_key|
|__sharedunixlibrary_key|
	DCD	0

	EXPORT	|__stackchunk_magic_number|
|__stackchunk_magic_number|
	DCD	&F60690FF ;Same as the scl's magic number, for compatibility in libgcc

	IMPORT	|__unixlib_raise_signal|
	; Allocate 512 bytes more stack
	; Round up to 4K as stack is only allocated in multiples of 4K
	EXPORT	|x$stack_overflow|
	EXPORT	|__rt_stkovf_split_small|
	NAME	__rt_stkovf_split_small
|x$stack_overflow|
|__rt_stkovf_split_small|
	STMFD	sp!, {a1, a2, a3, a4, v1, v2, v3, lr}	; This must store the same regs as for __rt_stkovf_split_big

	MOV	v1, #4096
	B	stack_overflow_common


	; Allocate stack to below <ip>
	EXPORT	|x$stack_overflow_1|
	EXPORT	|__rt_stkovf_split_big|
	NAME	__rt_stkovf_split_big
|x$stack_overflow_1|
|__rt_stkovf_split_big|
	CMP	ip, sl	; sanity check
	return	CS, pc, lr
	STMFD	sp!, {a1, a2, a3, a4, v1, v2, v3, lr}
	; This must store the same regs as for __rt_stkovf_split_small
	; and if changed, also update the 8*4 in the frame size calculation below

	SUB	v1, sp, ip	; Get required frame size
	ADD	v1, v1, #8*4	; Take account of the earlier STMFD

	ADD	v1, v1, #512+CHUNK_OVERHEAD	;size of chunk header and space for stack extension procedures
	ADD	v1, v1, #&FF0	;round up to nearest 4KB
	ADD	v1, v1, #&00F
	BIC	v1, v1, #&FF0
	BIC	v1, v1, #&00F

	; The rest is common to both split_small and split_big
	; Thread-safe, other than the __stackalloc/free calls
stack_overflow_common
	[ EXTREMELY_PARANOID = 1
	BL	|__check_stack|
	]

	SUB	v2, sl, #512+CHUNK_OVERHEAD	; Find bottom of chunk
	[ STACK_CHECK_MAGIC = 1
	LDR	a1, |__stackchunk_magic_number|
	LDR	a2, [v2, #CHUNK_MAGIC]
	CMP	a1, a2
	BNE	stack_corrupt
	]

	LDR	a1, =|__executing_signalhandler|
	LDR	a1, [a1]
	CMP	a1, #0
	BNE	signalhandler_overflow

	MOV	v3, sp	; Store old sp
	; We know there is enough stack to call stackalloc or stackfree, so sl just
	; needs to be set to a value that won't cause a recursive stack overflow
	SUB	sl, sp, #512

	LDR	a1, [v2, #CHUNK_NEXT]
	CMP	a1, #0
	BEQ	alloc_new_chunk	; There isn't a spare chunk, so we need to alloc a new one

	[ STACK_CHECK_MAGIC = 1
	LDR	a3, |__stackchunk_magic_number|
	LDR	a4, [a1, #CHUNK_MAGIC]
	CMP	a3, a4
	BNE	stack_corrupt
	]
	LDR	a2, [a1, #CHUNK_SIZE]
	CMP	v1, a2
	MOVLE	v1, a2
	BLE	use_existing_chunk	; Spare chunk is big enough, so use it

	; Spare chunk is too small, so free it and alloc another one
	BL	|__free_stack_chain|

alloc_new_chunk
	MOV	a1, v1
	BL	|__stackalloc|
	CMP	a1, #0
	BEQ	raise_sigemt	; No free memory

	LDR	a3, |__stackchunk_magic_number|
	STR	a3, [a1, #CHUNK_MAGIC]

	; Add chunk into linked list
	STR	a1, [v2, #CHUNK_NEXT]
	STR	v2, [a1, #CHUNK_PREV]

use_existing_chunk
	; a1 contains the new/existing chunk to be used, v1 contains its size.
	MOV	a2, #0
	STR	a2, [a1, #CHUNK_NEXT]
	STR	v1, [a1, #CHUNK_SIZE]

	ADD	sp, a1, v1	; New sp is the top of the new chunk
	ADD	sl, a1, #512+CHUNK_OVERHEAD

	LDR	a2, =|__stackfree|
	STR	a2, [a1, #CHUNK_DEALLOC] ; For compatibility with scl, in libgcc

	LDR	a2, [fp, #-4]	; Load the return address
	STR	a2, [a1, #CHUNK_RETURN]	; Remember it

	ADR	a3, free_stack_chunk
	TEQ	a1, a1
	TEQ	pc, pc
	ANDNE	a2, a2, #&fc000003	; Preserve flags if in 26bit mode
	BICNE	a3, a3, #&fc000003
	ORRNE	a3, a2, a3
	STR	a3, [fp, #-4]	; Replace it with our chunk free procedure

	LDMFD	v3, {a1, a2, a3, a4, v1, v2, v3, pc}

raise_sigemt
	MOV	a1, #0
	MOV	a2, #SIGEMT
	BL	|__unixlib_raise_signal|
	LDMFD	v3, {a1, a2, a3, a4, v1, v2, v3, pc}

stack_corrupt_msg
	DCB	"***Fatal error: Stack corruption detected***", 13, 10, 0
	ALIGN
stack_corrupt
	ADR	a1, stack_corrupt_msg
	B	__unixlib_fatal

signalhandler_overflow_msg
	DCB	"***Fatal error: Stack overflow in signal handler***", 13, 10, 0
	ALIGN
signalhandler_overflow
	ADR	a1, signalhandler_overflow_msg
	B	__unixlib_fatal

	[ EXTREMELY_PARANOID = 1
	; Check every stack chunk in the chain to ensure it contains
	; sensible values.
	NAME	"__check_stack"
	EXPORT	|__check_stack|
|__check_stack|
	MOV	ip, sp
	STMFD	sp!, {a1, a2, a3, a4, v1, v2, fp, ip, lr, pc}
	SUB	fp, ip, #4

	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BLNE	|__pthread_disable_ints|
	]

	LDR	a2, =|__stack|
	LDR	a2, [a2]			; a2 = __stack
	LDR	a3, =|__himem|
	LDR	a3, [a3]			; a3 = __himem
	LDR	a4, =|__base|
	LDR	a4, [a4]			; a4 = __base

	SUB	a1, sl, #512+CHUNK_OVERHEAD
__check_stack_l1
	CMP	a1, #0
	BEQ	__check_stack_l2
	BL	__check_chunk
	LDR	a1, [a1, #CHUNK_NEXT]
	B	__check_stack_l1

__check_stack_l2
	SUB	a1, sl, #512+CHUNK_OVERHEAD
__check_stack_l3
	CMP	a1, #0
	BNE	__check_stack_l4
	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BLNE	|__pthread_enable_ints|
	]
	LDMEA	fp, {a1, a2, a3, a4, v1, v2, fp, sp, pc}

__check_stack_l4
	BL	__check_chunk
	LDR	a1, [a1, #CHUNK_PREV]
	B	__check_stack_l3

__check_chunk ; a1 = chunk, a2 = __stack, a3 = __himem, a4 = __base
	CMP	a1, a2
	BCC	stack_corrupt
	CMP	a1, a3
	BCS	stack_corrupt

	LDR	v1, [a1, #CHUNK_MAGIC]
	LDR	v2, |__stackchunk_magic_number|
	CMP	v1, v2
	BNE	stack_corrupt

	LDR	v1, [a1, #CHUNK_SIZE]
	CMP	v1, #&1000
	BLT	stack_corrupt
	SUB	v2, a3, a2
	CMP	v1, v2
	BGT	stack_corrupt

	LDR	v1, [a1, #CHUNK_NEXT]
	CMP	v1, #0
	BEQ	__check_chunk_l1
	CMP	v1, a2
	BCC	stack_corrupt
	CMP	v1, a3
	BCS	stack_corrupt
__check_chunk_l1
	LDR	v1, [a1, #CHUNK_PREV]
	CMP	v1, #0
	BEQ	__check_chunk_l2 ; If the previous chunk is zero then dealloc and return may also be zero
	CMP	v1, a2
	BCC	stack_corrupt
	CMP	v1, a3
	BCS	stack_corrupt

	LDR	v1, [a1, #CHUNK_DEALLOC]
	TEQ	a1, a1
	TEQ	pc, pc	; EQ if in a 32-bit mode, NE if 26-bit
	BICNE	v1, v1, #&fc000003
	CMP	v1, a4
	BCC	stack_corrupt
	CMP	v1, a3
	BCS	stack_corrupt

	LDR	v1, [a1, #CHUNK_RETURN]
	TEQ	a1, a1
	TEQ	pc, pc	; EQ if in a 32-bit mode, NE if 26-bit
	BICNE	v1, v1, #&fc000003
	CMP	v1, a4
	BCC	stack_corrupt
	CMP	v1, a3
	BCS	stack_corrupt
__check_chunk_l2
	return	AL, pc, lr
	] ; EXTREMELY_PARANOID = 1


	EXPORT	|__trim_stack|
|__trim_stack| ; Remove any unused stack chunks
	SUB	a2, sl, #512+CHUNK_OVERHEAD	; Find bottom of chunk
	LDR	a1, [a2, #CHUNK_NEXT]
	CMP	a1, #0
	MOVNE	a3, #0
	STRNE	a3, [a2, #CHUNK_NEXT]
	return	EQ, pc, lr	; Falls through to __free_stack_chain if required

	EXPORT	|__free_stack_chain|
|__free_stack_chain| ; free a chain of stack chunks pointer to by a1
	STMFD	sp!, {v1, lr}
__free_stack_chain_l1
	; Spare chunk is too small, so free it and alloc another one
	[ STACK_CHECK_MAGIC = 1
	MOV	a2, #0	; Invalidate the magic number
	STR	a2, [a1, #CHUNK_MAGIC]
	]
	LDR	v1, [a1, #CHUNK_NEXT]
	BL	|__stackfree|
	MOVS	a1, v1
	BNE	__free_stack_chain_l1
	stackreturn	AL, "v1, pc"


free_stack_chunk
	; Called on exit from a function that caused stack extension
	; a3-a4, ip, sl and lr can be corrupted, all others (including sp)
	; must be preserved. gcc uses a2 to return 64bit results.
	; Stack chunks are freed with one chunk of latency,
	; so we need to free the previously unused chunk, not the chunk that
	; has just been finished with. Therefore the chunk just finished
	; with can be used as a stack for this function
	[ EXTREMELY_PARANOID = 1
	BL	|__check_stack|
	]
	SUB	sl, sl, #512+CHUNK_OVERHEAD	; sl = start of current chunk
	LDR	ip, [sl, #CHUNK_SIZE]
	ADD	ip, sl, ip	; ip = top of chunk
	STMFD	ip!, {a1, a2, fp, sp}	; stack important regs
	MOV	sp, ip	; set up sp
	; sl remains at the bottom of the chunk, but there's 4K of space and
	; __stackfree won't need more than 256 bytes of it so this is ok

	[ STACK_CHECK_MAGIC = 1
	LDR	a1, |__stackchunk_magic_number|
	LDR	a2, [sl, #CHUNK_MAGIC]
	CMP	a1, a2
	BNE	stack_corrupt
	]

	LDR	a1, [sl, #CHUNK_NEXT]
	CMP	a1, #0
	BEQ	no_chunk_to_free

	[ STACK_CHECK_MAGIC = 1
	MOV	a2, #0	; Invalidate the magic number
	STR	a2, [a1, #CHUNK_MAGIC]
	]
	BL	|__stackfree|
	MOV	a1, #0
	STR	a1, [sl, #CHUNK_NEXT]

no_chunk_to_free
	LDMFD	sp, {a1, a2, fp, sp} ; Restore important regs

	LDR	lr, [sl, #CHUNK_RETURN]	; Get the real return address
	LDR	sl, [sl, #CHUNK_PREV]
	ADD	sl, sl, #512+CHUNK_OVERHEAD	; Set sl up correctly for the old stack chunk
	return	AL, pc, lr


	; Globally used panic button.
	; void __unixlib_fatal(const char *message)
	EXPORT	|__unixlib_fatal|
	IMPORT	|strerror|
	IMPORT	|__os_nl|
	IMPORT	|__os_print|
	IMPORT	|_exit|
	NAME	__unixlib_fatal
|__unixlib_fatal|
	; We don't want to assume anything about the stack as the stack
	; corruption detection routines will call this routine in case
	; something is wrong.
	LDR	sp, =|__signalhandler_sp|
	LDR	sp, [sp, #0]
	MOV	fp, #0
	LDR	sl, =|__signalhandler_sl|
	LDR	sl, [sl, #0]

	MOV	ip, sp
	STMDB	sp!, {v1, fp, ip, lr, pc}
	SUB	fp, ip, #4

	MOVS	v1, a1
	BNE	__unixlib_fatal_got_msg
	__get_errno	v1, a1
	BL	strerror
__unixlib_fatal_got_msg
	BL	__os_nl
	MOV	a1, v1
	BL	__os_print
	BL	__os_nl
	MOV	a1, #1
	BL	_exit
	; Should never return
	LDMDB	fp, {v1, fp, sp, pc}

	; Return non-zero if the floating point instruction set is available
	EXPORT	|_kernel_fpavailable|
	NAME	_kernel_fpavailable
|_kernel_fpavailable|
	LDR	a1, =struct_base
	LDR	a1, [a1, #52]	; __fpflag
	return	AL, pc, lr

	AREA	|C$$zidata|, DATA, NOINIT

	; This space is reserved for UnixLib to store the environment handlers
	; of the calling application.
	EXPORT |__calling_environment|
|__calling_environment|
	% 204

	AREA	|C$$data|, DATA

dynamic_deletion
	DCD	0
	DCB	"XXXXXXXXXX"
dynamic_area_name_end
	DCB	"$Heap", 0
	ALIGN


	IMPORT	|Image$$RO$$Base|
	IMPORT	|Image$$RW$$Base|
	IMPORT	|Image$$RW$$Limit|
	EXPORT	|__cli|		; CLI from OS_GetEnv
	EXPORT	|__base|	; BASE (application = 0x8000)
	EXPORT	|__lomem|	; LOMEM
	EXPORT	|__rwlimit|
	EXPORT	|__himem|	; HIMEM from OS_GetEnv
	EXPORT	|__break|	; the 'break'
	EXPORT	|__stack|	; stack limit
	EXPORT	|__stack_limit|	; lower stack limit
	EXPORT	|__time|	; start time - 5 byte format
	EXPORT	|__real_break|	; top limit of dynamic area allocated
	EXPORT	|__fpflag|
	EXPORT	|__taskwindow|	; non-zero if executing in a TaskWindow
	EXPORT	|__taskhandle|	; WIMP task handle, or zero if non WIMP task
	EXPORT	|__dynamic_num|
	EXPORT	|__u|		; pointer to proc structure
	EXPORT	|__real_himem|
	EXPORT	|__32bit|	; non-zero if executing in 32-bit mode

	; Altering this structure will require fixing __main.
struct_base
|__cli|		DCD	0				; offset = 0
|__himem|	DCD	0				; offset = 4
|__time|	DCD	0, 0	; low word, high byte	; offset = 8
|__stack|	DCD	0				; offset = 16

|__robase|	DCD	|Image$$RO$$Base|		; offset = 20
|__rwlimit|	DCD	|Image$$RW$$Limit|		; offset = 24
|__base|	DCD	0				; offset = 28

|__lomem|	DCD	0				; offset = 32
|__break|	DCD	0				; offset = 36
|__stack_limit|	DCD	0				; offset = 40

|__rwbase|	DCD	|Image$$RW$$Base|		; offset = 44
|__real_break|	DCD	0				; offset = 48
|__fpflag|	DCD	0				; offset = 52

|__taskwindow|	DCD	0				; offset = 56
|__taskhandle|	DCD	0				; offset = 60

|__dynamic_num|	DCD	-1				; offset = 64
|__u|		DCD	0				; offset = 68
|__real_himem|	DCD	0				; offset = 72

|__32bit|	DCD	0				; offset = 76

	END
