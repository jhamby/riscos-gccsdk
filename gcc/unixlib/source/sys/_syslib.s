;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/sys/s/_syslib,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.25 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	; Keep in sync with error_table.
NO_MEMORY * 0
NO_CALLASWI * 1

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|_main|
	IMPORT	|__dynamic_no_da|, WEAK
	IMPORT	|__alloca_list|, WEAK

	EXPORT	|__main|

	ENTRY
|__main|
	SWI	XOS_GetEnv
	[ |__4K_BOUNDARY| = 1
	; align top of wimpslot to 4k (downwards)
	BIC	a2, a2, #&ff
	BIC	a2, a2, #&f00
	]
	MOV	sp, a2		; Stack = top of RAM
	SUB	sl, sp, #2048	; Limit is 2K below.

	LDR	v6, =|__cli|
	; __cli = a1 (pointer to command line)
	; __himem = a2 (top of memory)
	LDMIA	a3, {a3, a4}	; __time = a3, a4 (start time)
	AND	a4, a4, #&ff
	; __stack = sl (bottom of stack)
	STMIA	v6!, {a1, a2, a3, a4, sl}

	; Read __robase, __rwlimit
	LDMIA	v6!, {a1, a2}
	[ |__4K_BOUNDARY| = 1
	; align rwlimit to 4k
	ADD	a2, a2, #&1000
	SUB	a2, a2, #1
	BIC	a2, a2, #&ff
	BIC	a2, a2, #&f00
	; Store back in __rwlimit
	STR	a2, [v6, #-4]
	]
	; Store __robase in __base
	STR	a1, [v6], #4
	; Store __rwlimit in __lomem, __break and __stack_limit
	MOV	a1, a2
	MOV	a3, a2
	STMIA	v6!, {a1, a2, a3}
	; Store __rwlimit in __real_break.
	LDR	a4, =|__real_break|
	CMP	sl, a1		; order for StrongARM
	STR	a1, [a4, #0]

;	CMP	sl, a1		; reordered above
	; No stack, exit fast.
	BLS	exit_with_error_no_memory

	BL	check_for_callaswi

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
	CMPVC	a2, sp			; only make check if SWI succeeds
	BEQ	no_old_area		; B if eq or SWI failed
	; validate numbers at top of application memory (see sys.s._exec)
	LDMIA	sp, {a1, a2, a3, a4, v1, v2, v3, v4}
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
	; area name is program name + "$Heap"
	LDR	a4, =|__cli|
	LDR	a1, [a4]
	MOV	a2, a1
	; search for space or end of cli string
01
	LDRB	a3, [a1], #1
	CMP	a3, #" "
	BGT	%B01
	SUB	a1, a1, #1	; back up to point at terminator char

	; use a maximum of 10 characters from the program name
	LDR	v5, =dynamic_area_name_end

	SUB	a4, a1, #10
	CMP	a4, a2
	MOVHI	a2, a4
	; a2 = max (a2, a4) - ie
	; 10 chars from end of name if length name > 10
	; else start of name

	; So, decending copy from a1 to v5
	; limit is a2, terminate early if we find "." or ":"
02
	LDRB	a4, [a1, #-1]!
	CMP	a4, #"."
	CMPNE	a4, #':'
	STRNEB	a4, [v5, #-1]!
	BEQ	%F03		; Not sure if some very slick conditionals
	CMP	a1, a2		; can eliminate that branch
	BHI	%B02		; limit not yet reached
03

	; check environment variable for creating a DA
	MOV	a1, v5
	MOV	a3, #-1
	MOV	a4, #0
	MOV	v1, #0
	SWI	XOS_ReadVarVal
	CMP	a3, #0
	BGE	no_dynamic_area

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
	; v6 is size left in area, a4 is start offset
	ADD	a1, v6, a4
	LDR	v3, =|__lomem|
	LDR	v4, =|__break|
	STR	a4, [v3]		; Lomem is start of DA
	STR	a1, [v4]		; break is end of used part of DA
	LDR	v3, =|__real_break|
	LDR	v4, =|__dynamic_num|
	STR	a1, [v3]		; __real_break is end of DA
	STR	a2, [v4]

	; set lower stack limit to __rwlimit
	LDR	v3, =|__rwlimit|
	LDR	v4, =|__stack_limit|
	LDR	a1, [v3]
	STR	a1, [v4]

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
	LDR	a2, =|__taskwindow|
	MOVVS	a1, #0
	STR	a1, [a2]

	; Find out whether we are executing as a WIMP program or not.
	MOV     a1, #3
	SWI     XWimp_ReadSysInfo
	LDR     a2, =|__wimpprogram|
	MOVVS   a1, #0
	STR     a1, [a2]

	; Recognise the Floating Point facility by determining whether
	; the SWI FPEmulator_Version actually exists (and works).
	; If it does, then there is a floating point ability.
	SWI	XFPEmulator_Version
	LDR	a2, =|__fpflag|
	MOVVS	a1, #0
	MOVVC	a1, #1
	STR	a1, [a2, #0]

	; Now we'll initialise the C library, then call the user program.
	B	|_main|


exit_with_error_no_memory
	MOVVS	a1, #NO_MEMORY
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

error_no_callaswi
	DCB	"Module CallASWI is not present.", 13, 10, 0
error_no_memory
	DCB	"Insufficient memory for application", 13, 10, 0
	ALIGN

check_for_callaswi
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
	return	EQ, pc, lr		; Most common case
	MOV	a1, #NO_CALLASWI
	B	exit_with_error


	EXPORT	|__svccli|	; copy CLI in SVC mode
|__svccli|
	LDR	a2, =|__cli|
	MOV	a4, lr
	LDR	a2, [a2, #0]
	SWI	OS_EnterOS
|__svccli_l0|
	LDRBT	a3, [a2], #1
	STRBT	a3, [a1], #1
	CMP	a3, #0
	BNE	|__svccli_l0|
	return	AL, pc, a4		; reiurn to USR mode

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
	IMPORT	|__stdioinit|, WEAK
	IMPORT	|__stdioexit|, WEAK
	EXPORT	|___vret|
	EXPORT	|___stdioinit|
	EXPORT	|___stdioexit|
|___vret|
	DCD	|__vret|
|___stdioinit|
	DCD	|__stdioinit|
|___stdioexit|
	DCD	|__stdioexit|

	EXPORT	|__read_environment_handler|
|__read_environment_handler|
	MOV	ip, a2
	MOV	a2, #0
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment
	STMVCIA ip, {a2, a3, a4}
	MOVVC	a1, #0
	return	AL, pc, lr

	EXPORT	|__write_environment_handler|
|__write_environment_handler|
	LDMIA	a2, {a2, a3, a4}
	SWI	XOS_ChangeEnvironment
	MOVVC	a1, #0
	return	AL, pc, lr

	IMPORT	|__unixlib_raise_signal|
	EXPORT	|x$stack_overflow|
	EXPORT	|__rt_stkovf_split_small|
	; allocate 512 bytes more stack
|x$stack_overflow|
|__rt_stkovf_split_small|
	STMFD	sp!, {a1, a2, lr}
	LDR	lr, =|__cli|
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
	LDR	lr, =|__cli|
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
	LDR	a1, =|__fpflag|
	LDR	a1, [a1, #0]
	return	AL, pc, lr

	AREA	|C$$data|, DATA

	EXPORT	|__dynamic_num|
|__dynamic_num|
	DCD	-1
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

	; Altering this structure will require fixing __main.
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

	END
