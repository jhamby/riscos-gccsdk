;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_syslib.s,v $
; $Date: 2002/02/11 13:11:31 $
; $Revision: 1.3.2.9 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	; Keep in sync with error_table.
NO_MEMORY   * 0
NO_CALLASWI * 1
NO_SUL      * 2

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
	EXPORT	|__real_break|  ; top limit of dynamic area allocated
	EXPORT	|__fpflag|
	EXPORT	|__taskwindow|  ; non-zero if executing in a TaskWindow
	EXPORT	|__wimpprogram| ; non-zero if executing as a Wimp program
	EXPORT	|__sigstk|	; stack for callback signals
	EXPORT	|__sigstksize|	; size of callback signal stack
	EXPORT	|__dynamic_num|
	EXPORT	|__u|		; pointer to proc structure
		
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
|__wimpprogram|	DCD	0				; offset = 60
|__sigstk|	DCD	0				; offset = 64
|__sigstksize|	DCD	4096				; offset = 68

|__dynamic_num|	DCD	-1				; offset = 72
|__u|		DCD	0				; offset = 76

	AREA	|C$$zidata|, DATA, NOINIT

	; This space is reserved for UnixLib to store the environment handlers
	; of the calling application.
	EXPORT |__calling_environment|
|__calling_environment|
	% 204

	AREA	|C$$code|, CODE, READONLY

	IMPORT  |__unixinit|            ;C function (unix/unix.c)
	IMPORT  |exit|                  ;C function (unix/unix.c)
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
	IMPORT  |__h_upcall|            ;ASM function (signal/_signal.s)

	IMPORT	|_main|
	IMPORT	|__dynamic_no_da|, WEAK
	IMPORT	|__dynamic_da_name|, WEAK
	IMPORT	|__alloca_list|, WEAK

	EXPORT	|__main|

|rmensure|
	DCB "RMEnsure SharedUnixLibrary 1.00 RMLoad System:Modules.SharedULib"
	DCB 0
	ALIGN

	ENTRY
|__main|
	; Read environment parameters
	; On exit:
	;   a1 = pointer to environment string
	;   a2 = permitted RAM limit (highest address available + 1)
	;   a3 = pointer to real time the program was started (5 bytes)
	SWI	XOS_GetEnv

	[ |__4K_BOUNDARY| = 1
	; align top of wimpslot to 4k (downwards)
	BIC	a2, a2, #&ff
	BIC	a2, a2, #&f00
	]

	; struct_base is the start of our memory environment variables
	; See the end of this file.  For the initialisation here, we
	; will always use ip as the base register.
	LDR	ip, =struct_base
	
	; The stack is allocated at the top of RAM.  We cannot place it
	; in a dynamic area because GCC might generate trampolines.
	; Trampolines (for the un-initiated) are little code fragments that
	; execute in stack space.
	MOV	sp, a2
	
	; For simplicity, the first X bytes of stack is reserved for the
	; signal callback stack.
	STR	sp, [ip, #64]	; __sigstk
	LDR	a4, [ip, #68]	; __sigstksize
	BIC	a4, a4, #&03	;  Round to a 4 byte boundary.
	SUB	sp, sp, a4	; Setup the application stack
	SUB	sl, sp, #2048	; Default stack limit is 2048 bytes.

	STR	a1, [ip, #0]	; __cli = pointer to command line
	STR	a2, [ip, #4]	; __himem = permitted RAM limit

	LDMIA	a3, {a1, a2}	; Get time
	STR	a1, [ip, #8]	; __time (low word)
	STR	a2, [ip, #12]	; __time (high word)

	STR	sl, [ip, #16]	; __stack = bottom of stack

	; For a description of the memory layout of a UnixLib application
	; see sys/brk.c.
	LDR	a1, [ip, #20]	; __robase
	LDR	a2, [ip, #24]	; __rwlimit

	[ |__4K_BOUNDARY| = 1
	; align rwlimit to 4Kbyte boundary
	ADD	a2, a2, #&1000
	SUB	a2, a2, #1
	BIC	a2, a2, #&ff
	BIC	a2, a2, #&f00
	STR	a2, [ip, #24]	; __rwlimit
	]

	STR	a1, [ip, #28]	; __base = __robase
	STR	a2, [ip, #32]	; __lomem = __rwlimit
	STR	a2, [ip, #36]	; __break = __rwlimit
	STR	a2, [ip, #40]	; __stack_limit = __rwlimit
	STR	a2, [ip, #48]	; __real_break = __rwlimit

	CMP	sl, a1		; Check that stack doesn't overlap program
	BLS	exit_with_error_no_memory ; No stack, exit.

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
	MOV	a1, #14
	MOVS	a2, #0			; Ensure Z flag set
	SWI	XOS_ChangeEnvironment	; preserves Z
	LDRVC	a1, [ip, #64]		; top of our app space at __sigstk 
	CMPVC	a2, a1			; only make check if SWI succeeds
	BEQ	no_old_area		; B if eq or SWI failed
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
	MOV	v6, a1				; save new __lomem
	MOVS	a1, #2				; ensure Z flag clear
	SWI	XOS_DynamicArea			; preserves Z, sets V on error
	; check new __lomem = end of dynamic area
	ADDVC	a4, a4, a3	; If VC here then SWI sucessful,
	TEQVC	a4, v6		; if VS then Z will be clear from earlier MOVS
	BNE	no_old_area	; bail out if SWI failed, or if sizes mismatch
	MOV	v6, #0
	BVC	da_found


no_old_area
	; Allow the user the option of setting theirr own name for the
	; dyanmic area used as a heap.   If the variable __dynamic_da_name
	; exists, then it must be a char pointer (not an array) to an
	; alternate name.  DAs are always used in this case, and there's
	; no need to set a $heap variable.

	; The main use of this is when the binary is called !RumImage.
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
	LDR	v5, =|dynamic_area_name_end|

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
	STR	a2, [ip, #72]	; __dynamic_num

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
	ADD	sl, sl, #256
	LDR	a1, =|__alloca_list|

	MOV	fp, #0
	CMP	a1, #0
	STRNE	fp, [a1, #0]

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
	MOV     a1, #3
	SWI     XWimp_ReadSysInfo
	MOVVS	a1, #0
	STR	a1, [ip, #60]	; __wimpprogram

	; Recognise the Floating Point facility by determining whether
	; the SWI FPEmulator_Version actually exists (and works).
	; If it does, then there is a floating point ability.
	SWI	XFPEmulator_Version
	MOVVS	a1, #0
	MOVVC	a1, #1
	STR	a1, [ip, #52]	; __fpflag

	; Now we'll initialise the C library, then call the user program.
	
	; Check the environment variable UnixLib$env.  If set, then
	; it will be an integer pointer to the parent process structure.
	; If we have successfully read the variable, then immediately
	; delete it so that we don't confuse any other forking processes.
	ADR	a1, env		; Environment variable name
	ADD	a2, ip, #76	; pointer to __u
	MOV	a3, #4		; buffer length is 4 bytes
	MOV	a4, #0		; context pointer (0 = first call)
	MOV	v1, #1		; variable type is number
	SWI	XOS_ReadVarVal
	LDR	a1, [ip, #76]	; If __u is non-zero, then variable existed
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

error_no_callaswi
	DCB	"Module CallASWI is not present.", 13, 10, 0
error_no_memory
	DCB	"Insufficient memory for application", 13, 10, 0
error_no_sharedunixlib
	DCB	"This application requires version 1.00 of the SharedUnixLibrary module", 13, 10, 0
	ALIGN

check_for_callaswi

	IMPORT	|__munmap_all|
	EXPORT	|__dynamic_area_exit|
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
|__exit|
	MOV	v1, a1
	BL	|__dynamic_area_exit|
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
|__exit_no_code|
	BL	|__dynamic_area_exit|
	MOV	a1, #0
	MOV	a2, #0
	MOV	a3, #0
	SWI	OS_Exit		; - never returns

	IMPORT	|__vret|, WEAK
	EXPORT	|___vret|
|___vret|
	DCD	|__vret|

	EXPORT	|__env_riscos|
	; Restore original RISC OS environment handlers
|__env_riscos|
	STMFD	sp!, {a1, a2, a3, a4, v1, v2, lr}
	MOV	v1, #0
	LDR	v2, =|__calling_environment|
t04
	MOV	a1, v1
	LDMIA	v2!, {a2, a3, a4}
	SWI	XOS_ChangeEnvironment
	ADD	v1, v1, #1
	CMP	v1, #17		;  __ENVIRONMENT_HANDLERS
	BLT	t04
	LDMFD	sp!, {a1, a2, a3, a4, v1, v2, pc}^

	; Get current environment handler setup
	EXPORT	|__env_read|
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
	LDMFD	sp!, {a1, a2, a3, a4, v1, v2, pc}^
	
	; Install the Unixlib environment handlers
	EXPORT	|__env_unixlib|
	IMPORT	|__ul_errbuf|
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

	LDR	r1, =|__env_riscos|
	SWI	XSharedUnixLibrary_RegisterUpCall
	LDR	r0, =|__sharedunixlibrary_key|
	STR	r2, [r0]
	MOV	r0, #16
	SWI	XOS_ChangeEnvironment

	SWI	XOS_IntOn
	LDMFD	sp!, {a1, a2, a3, a4, v1, v2, pc}^
	
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


	IMPORT	|__unixlib_raise_signal|
	EXPORT	|x$stack_overflow|
	EXPORT	|__rt_stkovf_split_small|
	; allocate 512 bytes more stack
|x$stack_overflow|
|__rt_stkovf_split_small|
	STMFD	sp!, {a1, a2, lr}
	LDR	lr, =struct_base
	LDR	a2, [lr, #16] ; get __stack
	LDR	a1, [lr, #40] ; get __stack_limit
	SUB	a2, a2, #512
	CMP	a2, a1
	STR	a2, [lr, #16] ; store in __stack
	ADD	sl, a2, #256
	stackreturn	CS, "a1, a2, pc"
	STMFD	sp!, {a3, a4, ip}
	MOV	a1, #0
	MOV	a2, #SIGEMT
	BL	|__unixlib_raise_signal|
	LDMFD	sp!, {a3, a4, ip}
	stackreturn	AL, "a1, a2, pc"

	EXPORT	|x$stack_overflow_1|
	EXPORT	|__rt_stkovf_split_big|
	; Allocate stack to below <ip>
|x$stack_overflow_1|
|__rt_stkovf_split_big|
	CMP	ip, sl ; sanity check
	return	CS, pc, lr
	STMFD	sp!, {a1, a2, lr}
	LDR	lr, =struct_base
	SUB	a2, ip, #512
	LDR	a1, [lr, #40] ; get __stack_limit
	CMP	a2, a1
	STR	a2, [lr, #16] ; store in __stack
	ADD	sl, a2, #256
	stackreturn	CS, "a1, a2, pc"
	STMFD	sp!, {a3, a4, ip}
	MOV	a1, #0
	MOV	a2, #SIGEMT
	BL	|__unixlib_raise_signal|
	LDMFD	sp!, {a3, a4, ip}
	stackreturn	AL, "a1, a2, pc"

	EXPORT	|_kernel_fpavailable|
	; Return non-zero if the floating point instruction set is available
|_kernel_fpavailable|
	LDR	a1, =struct_base
	LDR	a1, [a1, #52]	; __fpflag
	return	AL, pc, lr

	END

