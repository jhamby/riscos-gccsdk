;----------------------------------------------------------------------------
;
; $Source$
; $Date$
; $Revision$
; $State$
; $Author$
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	sys_siglist

sys_siglist_ptr
	DCD	sys_siglist

	EXPORT	|__write_termination|
	NAME	__write_termination
|__write_termination|
	LDR	a2, sys_siglist_ptr
	MOV	ip, lr
	LDR	a1, [a2, a1, LSL #2]
	SWI	XOS_WriteS
	DCB	10, 13, "Termination signal received: ", 0
	ALIGN
	SWIVC	XOS_Write0
	SWI	XOS_NewLine
	return	AL, pc, ip

	EXPORT	|__write_corefile|
	NAME	__write_corefile
|__write_corefile|
	LDR	a2, sys_siglist_ptr
	MOV	ip, lr
	LDR	a1, [a2, a1, LSL #2]
	SWI	XOS_WriteS
	DCB	10, 13, "Fatal signal received: ", 0
	ALIGN
	SWIVC	XOS_Write0
	SWI	XOS_WriteS
	DCB	10, 13, "A stack backtrace will now follow ...", 10, 13, 0
	ALIGN
;	B	__core
	MOV	lr, ip

; Do not assume sl or usr_sp valid at this point
; uses a1-a4, ip, lr, pc
; preserves v1-v6, sl, fp, sp
; does not touch v1-v3, sl, fp, sp
; runs in USR or SVC mode
; hence APCS compliant

	EXPORT	|__core|
	NAME	__core
|__core|
	MOV	a1, fp
;
; B __backtrace
;

	IMPORT |__calling_environment|

NUM_ENV_HANDLERS * 17

|__backtrace|
	LDR	ip, |__backtrace_reg_ptr|

	STMIA	ip, {v4, v5, v6, lr}	; Store in private "stack"
	LDR	ip, |__calling_environment_end|

	MOV	v6, a1			; arg1 = frame pointer passed in

	; Remove environment handlers.
	MOV	a1, #NUM_ENV_HANDLERS - 1
backtrace_remove_loop
	LDMDB	ip, {a2, a3, a4}
	CMP	a1, #11			; Do not remove exit handler.
	SWINE	XOS_ChangeEnvironment
	; Ugh. Terribly hacky, but then so is RISC OS. :-)
	; Temporarily store UnixLib handlers in place of the
	; calling environment handlers.  The original values will
	; be restored when we reach backtrace_reinstall_loop later.
	; Hopefully this should work because backtrace was written
	; to safeguard against any dodgy memory access.
	STMDB	ip!, {a2, a3, a4}
	SUBS	a1, a1, #1
	BGE	backtrace_remove_loop

	SWI	XOS_WriteS
	DCB	13, "stack backtrace:", 10, 13, 10, 0
	ALIGN

	; a1 = -1 after loop
	MOV	a1, #0
	SWI	XOS_PlatformFeatures
	MOVVC	v5, a1			; Store features word
	MOVVS	v5, #0
; If SWI returns an error then assume unknown and thus not StrongARM.

	MOV	v4, #0			; old frame pointer

; v6 = frame pointer
; v5 = features word
; v4 = old frame pointer / corrected PC

backtrace_while_loop
	MOVS	a2, v6			; while frame pointer != 0
	BEQ	backtrace_endwhile

	CMP	v6, v4
	BEQ	backtrack_unchanged_fp

	SUB	a1, a2, #12		; Check 4 words are readable
	SWI	XOS_ValidateAddress
	BVS	backtrace_out_of_bounds
	BCS	backtrace_out_of_bounds

;    unsigned int *fp;
;    unsigned int *sp;
;    unsigned int *lr;
;    unsigned int *pc;		v6 points here.

	LDR	a1, [v6]
	; Bit 3 of features set => stored PC is PC+8 rather than PC+12.
	TST	v5, #&08	; order for StrongARM
	AND	a1, a1, #&03fffffc
	ADDNE	a1, a1, #4
	MOV	v4, a1		; put PC somewhere safe


	LDR	a2, [v6, #-12]
	TST	a2, #&80000000
	BEQ	backtrace_bit31_clear

; bit 31 set - do register dump for this value of PC

; common code to print values of pc and sp from frame pointer in v6
	BL	backtrace_print_pc_and_sp

; check that pc value in backtrace pointer points to real memory (well, RAM)
	MOV	a1, v4
	MOV	a2, a1
	SWI	XOS_ValidateAddress
	BVS	backtrace_bit31_pc_invalid
	BCS	backtrace_bit31_pc_invalid

	ADR	ip, backtrace_register_dump
	B	backtrace_print_function_name


backtrace_bit31_pc_invalid
	SWI	XOS_WriteS
	DCB	10, 13, "Invalid pc address ", 0
	ALIGN
	MOV	a1, a2	; Retrieve PC value (in case corrupted by error in R0)
	BL	backtrace_prhex
	SWI	XOS_NewLine

backtrace_register_dump

	SWI	XOS_WriteS
	DCB	10, 13, 10, "Register dump:", 10, 13, 0
	ALIGN
	MOV	v4, v6	; store old fp
;	Acchieve this by moving loop counter increment to before LDR
;	ADD	v6, v6, #4
	MOV	a4, #0

backtrace_register_dump_loop
	TST	a4, #3
	ADREQ	a1, backtrace_register_dump_newline
	SWIEQ	XOS_Write0
	ADR	a1, rname		; array of register names
	ADD	a1, a1, a4, LSL #1	; index to correct register
	MOV	a2, #2
	SWI	XOS_WriteN		; print 2 chars of register name
	SWI	XOS_WriteS
	DCB	": ", 0
	ALIGN

	ADD	a4, a4, #1		; Increment the loop counter here.
	ADD	a1, v6, a4, LSL #2
	MOV	a2, a1
	SWI	XOS_ValidateAddress	; check whether we will be able to load
	BVS	backtrace_register_dump_loop_invalid
	BCS	backtrace_register_dump_loop_invalid
	LDR	a1, [a2]		; safe so load value
	ADR	lr, backtrace_register_dump_loop_end
	B	backtrace_prhex		; BL then B to loop_end

backtrace_register_dump_loop_invalid
	SWI	XOS_WriteS
	DCB	"bad addr", 0
	ALIGN

backtrace_register_dump_loop_end
	SWI	XOS_WriteI + 32 ; " "
	CMP	a4, #16
	BLT	backtrace_register_dump_loop

; use old_fp (v4) here
; fp = (unsigned int *) ((unsigned int) (f->fp) & ~0x80000000U);
	LDR	v6, [v4, #-12]

	SWI	XOS_WriteS		; Order for StrongARM
	DCB	10, 13, 10, 10, 0
	ALIGN
	BIC	v6, v6, #&80000000
	B	backtrace_while_loop

backtrace_register_dump_newline
	DCB	10, 13, " ", 0

backtrace_bit31_clear
; a1 = pc
; a2 = fp from backtrace at this point (if this helps)
	MOV	a2, a1
	SWI	XOS_ValidateAddress
	BVS	backtrace_bit31_clear_pc_invalid
	BCS	backtrace_bit31_clear_pc_invalid

	; a1 preserved (no error)
	BL	backtrace_print_pc_and_sp
	ADR	ip, backtrace_bit31_clear_1
	B	backtrace_print_function_name	; returns to address in ip

backtrace_bit31_clear_pc_invalid
	SWI	XOS_WriteS
	DCB	"Invalid pc address ", 0
	ALIGN
	MOV	a1, a2	; Retrieve PC value (in case corrupted by error in R0)
	BL	backtrace_prhex

backtrace_bit31_clear_1
	SWI	XOS_NewLine
	MOV	v4, v6	; store old fp
	LDR	v6, [v4, #-12]
	B	backtrace_while_loop

backtrace_out_of_bounds
	SWI	XOS_WriteS
	DCB	"Stack frame has gone out of bounds with address 0x", 0
	ALIGN
	SUB	v6, v6, #12
	B	backtrack_fail_fp

backtrack_unchanged_fp

	SWI	XOS_WriteS
	DCB	"fp unchanged at 0x", 0
	ALIGN

backtrack_fail_fp
	MOV	a1, v6
	LDR	a2, backtrace_prhex_buffer_ptr
	MOV	a3, #12
	SWI	XOS_ConvertHex8
	SWIVC	XOS_Write0

	SWI	XOS_NewLine
	; break; from while loop

backtrace_endwhile
	SWI	XOS_NewLine

	; Reinstall environment handlers.
	LDR	ip, |__calling_environment_end|
	MOV	a1, #NUM_ENV_HANDLERS - 1
backtrace_reinstall_loop
	LDMDB	ip, {a2, a3, a4}
	CMP	a1, #11	; Exit handler was not removed, so do not restore.
	SWINE	XOS_ChangeEnvironment
	; Here we are restoring the original calling environment handlers
	; that were overwritten in __backtrace above.
	STMDB	ip!, {a2, a3, a4}
	SUBS	a1, a1, #1
	BGE	backtrace_reinstall_loop

	LDR	ip, |__backtrace_reg_ptr|
	MOV	a1, #1	; for the benefit of _write_corefile, which returns 1
	[	{CONFIG} = 26
	LDMIA	ip, {v4, v5, v6, pc}^
	|
	LDMIA	ip, {v4, v5, v6, pc}
	]

|__calling_environment_end|
	DCD |__calling_environment| + NUM_ENV_HANDLERS * 12
|__backtrace_reg_ptr|
	DCD |__backtrace_reg|
backtrace_prhex_buffer_ptr
	DCD	backtrace_prhex_buffer

rname
	DCB	"a1"
	DCB	"a2"
	DCB	"a3"
	DCB	"a4"

	DCB	"v1"
	DCB	"v2"
	DCB	"v3"
	DCB	"v4"
	DCB	"v5"
	DCB	"v6"

	DCB	"sl"
	DCB	"fp"
	DCB	"ip"
	DCB	"sp"
	DCB	"lr"
	DCB	"pc"

; 3 support functions:
;
; backtrace_print_pc_and_sp
; fprintf (" pc: %8x sp: %8x", a1, a2->sp);
;
; backtrace_prhex
; fprintf ("%8x", a1);
; preserves a4
;
; backtrace_print_function_name
; finds and prints function name
;
; Entry ip = return address
;	v4 = pc from backtrace
backtrace_print_function_name

; firstly check whether words before pc exist
	SUB	a1, v4, #7 * 4
	SUB	a2, v4, #3 * 4
	SWI	XOS_ValidateAddress
	BVS	backtrace_print_function_name_invalid
	BCS	backtrace_print_function_name_invalid

;  for (i = -3; i > -7; i--)
;    if ((pc[i] & 0xffffff00U) == 0xff000000U)
;      {
;	 n = (char *) (pc + i) - (pc[i] & 0xffU);
;	 break;
;      }

	SUB	a1, a2, #4 * 4	; The limit of the loop
	; a1 here so that it is spare when SWIs are called below

backtrace_print_function_name_loop
	LDR	a4, [a2], #-4
	BIC	a3, a4, #&FF
	TEQ	a3, #&ff000000
	BEQ	backtrace_print_function_name_found
	CMP	a2, a1
	BGE	backtrace_print_function_name_loop
; name not found.
	SWI	XOS_WriteS
	DCB	" ?()", 0
	ALIGN
	return	AL, pc, ip
backtrace_print_function_name_found
	ADD	a2, a2, #4	; correct for post index addressing
	SWI	XOS_WriteI + 32 ; " "

;	n = (char *) (pc + i) - (pc[i] & 0xffU);
	AND	a4, a4, #&FF
	SUB	a1, a2, a4
	SWI	XOS_Write0
	SWI	XOS_WriteS
	DCB	"()", 0
	ALIGN
	return	AL, pc, ip
backtrace_print_function_name_invalid
	SWI	XOS_WriteS
	DCB	"[invalid address]", 0
	ALIGN
	return	AL, pc, ip

;      os_print (" pc: ");
;      os_prhex ((int) pc);
;      os_print (" sp: ");
;      os_prhex ((int) f->sp);

; enter with pc value in a1, fp in v6
backtrace_print_pc_and_sp
	SWI	XOS_WriteS
	DCB	" pc: ", 0
	ALIGN
	MOV	a4, lr		; preserve lr during call
	BL	backtrace_prhex
	SWI	XOS_WriteS
	DCB	" sp: ", 0
	ALIGN
	LDR	a1, [v6, #-8]	; load old stack pointer
	MOV	lr, a4
; tail optimise - run through into prhex

; enter a1 = value
; a2, a3, ip scratch
; a4, v1-v6 preserved
backtrace_prhex
	LDR	a2, backtrace_prhex_spaces
	LDR	a3, backtrace_prhex_buffer_ptr
	MOV	ip, a2		; order for StrongARM
	STMIA	a3!, {a2, ip}	; fill buffer with 8 spaces

; a3 points at byte beyond end of buffer
backtrace_prhex_l1
	AND	a2, a1, #&f
	CMP	a2, #10
	ADDLT	a2, a2, #48
	ADDGE	a2, a2, #87
	STRB	a2, [a3, #-1]!
	MOVS	a1, a1, LSR #4	; Will be 0 when 8 nibbles are written
	BNE	backtrace_prhex_l1
; may be 0 before -> eliminates leading zeros

	LDR	a1, backtrace_prhex_buffer_ptr
	MOV	a2, #8
	MOV	ip, lr
	SWI	XOS_WriteN
	return	AL, pc, ip

backtrace_prhex_spaces
	DCB	"    "	; four space characters to initialise the buffer

	AREA	|C$$zidata|, DATA, NOINIT

backtrace_prhex_buffer
	%	12
|__backtrace_reg|
	%	16	; Four registers - v4, v5, v6, lr

	END
