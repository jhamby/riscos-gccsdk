;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_exec.s,v $
; $Date: 2004/09/07 14:05:11 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__exret|
	IMPORT	|__env_wimpslot|

	IMPORT	|__image_ro_base|
	IMPORT	|__image_rw_lomem|
	IMPORT	|__unixlib_rwlimit|
	IMPORT	|__dynamic_num|
	IMPORT	|__unixlib_real_break|

	IMPORT	|__exerr|
	IMPORT	|abort|
	IMPORT	|_exit|

	; a1 = cli
|__exec_s0|

|__exec_s1|
	MOV	v1, a1
	; Save stack registers.  We will recover these in the exit
	; handler.
	ADR	a1, |__exreg|
	STMIA	a1, {sl, fp, sp}

	; Set up the exit handler.  Execution will return to __exec_s4
	; when the program that we are about to execute calls OS_Exit.
	MOV	a1, #11
	ADR	a2, |__exec_s4|
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment


	LDR	a1, [pc, #|__base_ptr| - . -8]	; StrongARM order
	STR	a2, [pc, #|__o_exit| - . -8]

	LDR	a1, [a1, #0]
	LDR	a2, [pc, #|__rwlimit_ptr| - . -8]
	STR	a1, [pc, #|__base_| - . -8]
	LDR	a2, [a2, #0]
	; save __real_break and __dynamic_num for later since they could get
	; corrupted by the code shift below
	LDR	a4, [pc, #|__real_break_ptr| - . -8]
	STR	a2, [pc, #|__rwlimit_| - . - 8]	; StrongARM order
	LDR	a4, [a4, #0]
	STR	a4, [pc, #|__real_break_| - . -8]
	LDR	a4, [pc, #|__dynamic_num_ptr| - . -8]
	LDR	a4, [a4, #0]
	LDR	a3, [pc, #|__codeshift| - . -8]
	STR	a4, [pc, #|__dynamic_num_| - . -8]


	; Relocate old application prior to executing the new app.
	[ __UNIXLIB_PARANOID > 0
	CMP	a3, #0
	MOVEQ	a3, a1			; no code shift required, so set
	BEQ	|__exec_s3|		; a3 to base (what else?)
	]

	ADD	a3, a2, a3

	SUB	a1, a2, a1
	ANDS	a4, a1, #31
	BEQ	|__exec_s2_1|
|__exec_s2_0|
	LDR	ip, [a2, #-4]!
	SUB	a1, a1, #4
	STR	ip, [a3, #-4]!
	ANDS	a4, a1, #31
	BNE	|__exec_s2_0|
|__exec_s2_1|
	CMP	a1, #0
	BEQ	|__exec_s3|
|__exec_s2_2|
	LDMDB	a2!, {a4, v2, v3, v4, v5, v6, sl, ip}
	STMDB	a3!, {a4, v2, v3, v4, v5, v6, sl, ip}
	SUBS	a1, a1, #64
	LDMGEDB a2!, {a4, v2, v3, v4, v5, v6, sl, ip}
	STMGEDB a3!, {a4, v2, v3, v4, v5, v6, sl, ip}
	BGT	|__exec_s2_2|

|__exec_s3|
	; save dynamic area information four times at top of application
	; memory so it can be validated in new image
	; start of area for spawned programs = __real_break
	LDR	a4, [pc, #|__real_break_|-.-8]
	EOR	v3, a4, a4, ROR #7
	EOR	v5, a4, a4, ROR #13
	EOR	sl, a4, a4, ROR #23
	LDR	v2, [pc, #|__dynamic_num_|-.-8]
	EOR	v4, v2, v2, ROR #7
	EOR	v6, v2, v2, ROR #13
	EOR	ip, v2, v2, ROR #23
	STMDB	a3!, {a4, v2, v3, v4, v5, v6, sl, ip}

	; Set memory limit for new application.  Prevent it from writing
	; over us.
	MOV	a1, #0
	MOV	a2, a3
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment
	; Save old memory limit.
	STR	a2, [pc, #|__o_himem|-.-8]

	; Execute the command/application.
	MOV	a1, v1
	SWI	XOS_CLI
	MOVVC	a1, #0
	STR	a1, [pc, #|__exerr_|-.-8]

	; If we created an DDE extended command line and OSCLI has failed
	; (probably due to insufficient memory to load program) then it is
	; likely that nothing has read (and hence cleared) the extended
	; command line. We must do it before it confuses any other program.
	MOVVS	a1, #0
	SWIVS	XDDEUtils_SetCLSize
	; You wouldn't credit it, but there is actually a race condition in
	; DDEUtils  !!
	; It only holds *one* extended command line, so it's bad luck if
	; your task is building the command only for another to issue OSCLI
	; before you!


	; Exit handler.  We enter here from any application executed
	; by XOS_CLI that called OS_Exit to terminate.  Entered in USR
	; mode.
|__exec_s4|
	; Restore old memory limit.
	MOV	a1, #0
	LDR	a2, [pc, #|__o_himem|-.-8]
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment

	LDR	a1, [pc, #|__rwlimit_|-.-8]
	LDR	a2, [pc, #|__codeshift|-.-8]
	LDR	a3, [pc, #|__base_|-.-8]
	[ __UNIXLIB_PARANOID > 0
	CMP	a2, #0
	BEQ	|__exec_s6|
	]

	; Relocate our application back down to its original position.
	ADD	a1, a1, a2
	ADD	a2, a3, a2

	SUB	a1, a1, a2
	ANDS	a4, a1, #31
	BEQ	|__exec_s5_1|
|__exec_s5_0|
	LDR	ip, [a2], #4
	SUB	a1, a1, #4
	STR	ip, [a3], #4
	ANDS	a4, a1, #31
	BNE	|__exec_s5_0|
|__exec_s5_1|
	CMP	a1, #0
	BEQ	|__exec_s6|
|__exec_s5_2|
	LDMIA	a2!, {a4, v2, v3, v4, v5, v6, sl, ip}
	STMIA	a3!, {a4, v2, v3, v4, v5, v6, sl, ip}
	SUBS	a1, a1, #64
	LDMGEIA a2!, {a4, v2, v3, v4, v5, v6, sl, ip}
	STMGEIA a3!, {a4, v2, v3, v4, v5, v6, sl, ip}
	BGT	|__exec_s5_2|

|__exec_s6|
	; Flush instruction cache for StrongARM. Use RWLimit rather
	; than ROLimit in case some code is in the data section
	MOV	a1, #1
	LDR	a2, [pc, #|__base_|-.-8]
	LDR	a3, [pc, #|__rwlimit_|-.-8]
	SWI	XOS_SynchroniseCodeAreas

	; Restore the original exit handler
	MOV	a1, #11
	LDR	a2, [pc, #|__o_exit|-.-8]
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment

	LDR	a1, [pc, #|__exerr_ptr|-.-8]
	LDR	a2, [pc, #|__exerr_|-.-8]
	STR	a2, [a1, #0]

	; Restore the stack registers.
	ADR	a1, |__exreg|
	LDMIA	a1, {sl, fp, sp}

	LDR	a1, [pc, #|__exret_ptr|-.-8]
	LDR	a2, [pc, #|__base_|-.-8]
	CMP	a1, a2
	BCC	|__exec_rtn_corrupt|
	BIC	a2, a1, #3
	CMP	a1, a2
	BNE	|__exec_rtn_corrupt|
	; Call __exret
	MOV	pc, a1
|__exec_rtn_corrupt|
	ADR	a1, |__exec_rtn_msg|
	SWI	XOS_Write0
	; This call will never return.
	MOV	a1, #127
	B	|_exit|
|__exec_rtn_msg|
	DCB	"return from exec() not possible, return point corrupted"
	DCB	13, 10, 0
	ALIGN

|__o_exit|
	DCD	0	; old Exit Handler
|__o_himem|
	DCD	0	; old HIMEM
|__exret_ptr|
	DCD	|__exret|
|__base_ptr|
	DCD	|__image_ro_base|
|__base_|
	DCD	0	; __base value
|__rwlimit_ptr|
	DCD	|__unixlib_rwlimit|
|__rwlimit_|
	DCD	0	; __rwlimit value
|__exreg|
	%	12	; sl, fp, sp
|__exerr_ptr|
	DCD	|__exerr|
|__exerr_|
	DCD	0
|__dynamic_num_ptr|
	DCD	|__dynamic_num|
|__dynamic_num_|
	DCD	0	; __dynamic_num value
|__real_break_ptr|
	DCD	|__unixlib_real_break|
|__real_break_|
	DCD	0	; __real_break value

	EXPORT	|__exshift|
|__exshift|
	DCD	0
	EXPORT	|__codeshift|
|__codeshift|
	DCD	0

|__exec_s7|

	EXPORT	|__exptr|
|__exptr|
	DCD	|__exec_s0|
	EXPORT	|__exlen|
|__exlen|
	DCD	|__exec_s7|-|__exec_s0|

        EXPORT  |__exec_cli|
|__exec_cli|
        LDR     a2, =|__image_rw_lomem|
        LDR     a2, [a2]
|__exec_cli_0|
        LDRB    a3, [a1], #1
        STRB    a3, [a2], #1
        CMP     a3, #0
        BNE     |__exec_cli_0|
        BL      |__env_wimpslot|

        LDR     a1, =|__image_rw_lomem|
        LDR     a1, [a1]
        SWI     XOS_CLI

        MOV     R0, #0
        SWI     XDDEUtils_SetCLSize

        MOV     R0, #0
        MOV     R1, #0
        MOV     R2, #0
        SWI     XOS_Exit

	END
