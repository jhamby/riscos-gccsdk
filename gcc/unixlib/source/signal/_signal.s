;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/_signal.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	raise
	IMPORT	|errno|
	IMPORT	sys_errlist

|__raise|
	CMP	a1,#0
	MOVEQS	pc,lr
	STMFD	sp,{a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,lr,pc}
	SUB	sp,sp,#64
	SUB	ip,pc,#4
	MOV	a4,lr
	ADD	a3,sp,#64
	ORR	a2,fp,#&80000000	; for __backtrace()
	STMFD	sp!,{a2,a3,a4,ip}	; create signal frame
	ADD	fp,sp,#12
	MOV	v1,sp
	BL	|raise|
	ADD	sp,v1,#16		; skip signal frame
	LDMFD	sp,{a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,pc}^



	AREA	|C$$wrcode|,CODE

	EXPORT	|__h_sigill|
|__h_sigill|
	STR	lr,|__cbreg|+60
	ADR	lr,|__cbreg|
	STMIA	lr!,{a1-ip}		; store non-banked registers
	STMIA	lr,{sp,lr}^		; store banked registers
	MOV	a1,#SIGILL
	SUB	lr,lr,#13*4		; adjust base register back
	STR	a1,|__cba1|
	B	|__h_cback|

	EXPORT	|__h_sigbus|
|__h_sigbus|
	SUB	lr,lr,#4
	STR	lr,|__cbreg|+60
	ADR	lr,|__cbreg|
	STMIA	lr!,{a1-ip}		; store non-banked registers
	STMIA	lr,{sp,lr}^		; store banked registers
	MOV	a1,#SIGBUS
	SUB	lr,lr,#13*4		; adjust base register back
	STR	a1,|__cba1|
	B	|__h_cback|

	; __h_sigsegv0 is for aborts during instruction prefetches
	EXPORT	|__h_sigsegv0|
|__h_sigsegv0|
	SUB	lr,lr,#4
	STR	lr,|__cbreg|+60
	ADR	lr,|__cbreg|
	STMIA	lr!,{a1-ip}		; store non-banked registers
	STMIA	lr,{sp,lr}^		; store banked registers
	MOV	a1,#SIGSEGV
	SUB	lr,lr,#13*4		; adjust base register back
	STR	a1,|__cba1|
	B	|__h_cback|


	; __h_sigsegv1 is for aborts during data access
	EXPORT	|__h_sigsegv1|
|__h_sigsegv1|
	SUB	lr,lr,#8
	STR	lr,|__cbreg|+60
	ADR	lr,|__cbreg|
	STMIA	lr!,{a1-ip}		; store non-banked registers
	STMIA	lr,{sp,lr}^		; store banked registers

	LDR	r0, |__cbreg|+60	; Swap this with previous for StrongARM?
	; SUB	lr,lr,#13*4		; adjust base register back
	BIC	r0, r0, #&fc000003
	LDR	r1, [r0, #0]		; get the instruction
	; Get the contents of the base register. We know it must be a
	; load/store data instruction because only that type of instruction
	; can cause this abort.
	ADR	lr, |__cbreg|
	AND	r2, r1, #&f0000
	LDR	r3, [lr, r2, LSR #14]
	; If bit 26 is set, we have a LDR/STR, otherwise a LDM/STM
	TST	r1, #(1:SHL:26)
	BNE	|__h_sigsegv1.ldr.str|
	; If the instruction didn't have writeback, there is no need
	; to correct the base register. We will just raise the signal.
	TST	r1, #(1:SHL:21)
	MOVEQ	a1, #SIGSEGV
	STREQ	a1, |__cba1|
	BEQ	|__h_cback|

	; Bit 23 is the direction. From the register list bitmask,
	; calculate the number of registers saved so we may correct
	; the base register.
	TST	r1, #(1:SHL:23)
	MOVEQ	r6, #-4
	MOVNE	r6, #4
	MOV	r4, #0
	MOV	r5, r1, LSL #16
|__h_sigsegv1.count.regs|
	MOVS	r5, r5, LSL #1
	ADDCS	r4, r4, r6
	BNE	|__h_sigsegv1.count.regs|
	; Subtract the bytes transferred off the base register.
	SUB	r3, r3, r4
	STR	r3, [lr, r2, LSR #14]
	MOV	a1,#SIGSEGV
	STR	a1,|__cba1|
	B	|__h_cback|

|__h_sigsegv1.ldr.str|
	; If the instruction has pre-indexed addressing, the base register
	; does not need to be corrected because it is the true address
	; that caused the data abort.
	TST	r1, #(1:SHL:24)
	MOVEQ	a1, #SIGSEGV
	STREQ	a1, |__cba1|
	BEQ	|__h_cback|

	; Post indexed addressing.

	; Separate the offset field.
	MOV	r4, r1, LSL #20
	MOV	r4, r4, LSR #20
	; If we have an immediate offset, things are very simple
	TST	r1, #(1:SHL:25)
	BEQ	|__h_sigsegv1.immediate.offset|
	; Get the value of the offset register (Rm)
	AND	r5, r4, #15
	LDR	r5, [lr, r5, LSL #2]

	; Perform close inspection of the register shift amount. LDR/STR does
	; not have register specific shift amount. Get the shift type into r6.
	AND	r6, r4, #96
	MOVS	r6, r6, LSR #5
	CMPNE	r6, #3
	BNE	|__h_sigsegv1.others|

	MOV	r7, r4, LSR #7
	; logical shift left
	CMP	r6, #0
	MOVEQ	r4, r5, LSL r7
	BEQ	|__h_sigsegv1.immediate.offset|

	; rotate right with extend
	LDR	r4, |__cbreg|+60
	TEQ	pc, r4, LSL #3 ; Get the aborted C flag
	MOV	r4, r5, RRX
	B	|__h_sigsegv1.immediate.offset|

|__h_sigsegv1.others|
	MOVS	r7, r4, LSR #7
	ORREQ	r7, r7, #32

	; logical shift right
	CMP	r6, #1
	MOVEQ	r4, r5, LSR r7
	; arithmetic shift right
	CMP	r6, #2
	MOVEQ	r4, r5, ASR r7
	; rotate right
	CMP	r6, #3
	MOVEQ	r4, r5, ROR r7

|__h_sigsegv1.immediate.offset|
	; Do we have a positive or negative offset
	TST	r1, #(1:SHL:23)
	SUBEQ	r3, r3, r4
	ADDNE	r3, r3, r4
	MOV	a1, #SIGSEGV
	STR	a1, |__cba1|
	B	|__h_cback|


	EXPORT	|__h_errbuf|
|__h_errbuf|
	DCD	0

	IMPORT	|__unixlib_exec_sig_interrupt|
	EXPORT	|__h_error|
lb1	DCB	"*** UnixLib error handler ***", 0
	ALIGN
lb2	DCD	&FF000000 + lb2 - lb1
|__h_error|
	; Entered in USR mode. Setup an APCS stack frame
	; so we can get a proper stack backtrace incase anything
	; goes horribly wrong.
	MOV	ip, sp
	STMFD	sp!, {a1, a2, a3, a4, fp, ip, lr, pc}
	SUB	fp, ip, #4

	; errno = EOPSYS
	LDR	a1, =|errno|
	MOV	a2, #EOPSYS
	STR	a2, [a1, #0]
	; sys_errlist = error string
	LDR	a3, |__h_errbuf|
	LDR	a1, =sys_errlist
	ADD	a2, a3, #8
	STR	a2, [a1, #(EOPSYS:SHL:2)]

	; Check the error number. Its value will determine the
	; appropriate signal to call.
	LDR	a2, [a3, #4]
	; Check for a serious error.
	TST	a2, #&80000000
	MOVEQ	a1, #SIGERR
	BLEQ	raise
	LDMEQEA	fp, {a1, a2, a3, a4, fp, sp, pc}^

	; Print the error
	ADR	a1, |__h_error_msg|
	SWI	XOS_Write0
	ADD	a1, a3, #8
	SWI	XOS_Write0
	SWI	XOS_NewLine

	; Raise a signal
	BIC	a2, a2, #&80000000
	MOV	a2, a2, LSR #8
	AND	a2, a2, #&FF
	CMP	a2,#&02
	MOVEQ	a1,#SIGFPE
	MOVNE	a1,#SIGEMT
	BL	raise
	LDMEA	fp, {a1, a2, a3, a4, fp, sp, pc}^

|__h_error_msg|
	DCB	13, 10, "Unrecoverable error received:", 13, 10, "  ", 0

	; This function is called when an Escape condition is detected

	EXPORT	|__h_sigint|
|__h_sigint|
	; Entered in IRQ mode. Be quick by just clearing the escape
	; flag and setting a callback.

	; Check for the escape condition
	TST	r11, #64
	MOVEQS	pc, lr
	MOV	ip, #SIGINT
	STR	ip, |__cba1|
	; Set the escape condition flag
	LDR	ip, |__cbflg|
	ORR	ip, ip, #1		; set CallBack
	STR	ip,|__cbflg|		; set __cbflg bit 0
	MOVS	pc, lr

callback_signal
	; Entered in SVC mode.  r12 contains the signal number to be
	; raised.
	STMFD	sp!, {r0-r12, lr}
	; Check we're not already in a callback.  Prevents re-entrancy
	; issues.
	ADR	a1, |__cbflg|
	LDR	a2, [a1, #0]
	TST	a2, #4		; __cbflg bit 2
	LDMEQFD	sp!, {r0-r12, pc}^
	ORR	a2, a2, #4	;  set that we are now in a callback
	STR	a2, [a1, #0]
	; Change to USR mode
	TEQP	pc, #0
	MOV	a1, a1

	; Preserve the old stack
	MOV	v1, sp

	; Signal number
	MOV	a1, r12

	; Setup signal stack, with a fresh APCS-compliant stack frame
	; Don't assume anything about the stack state in USR mode.  Use
	; our own instead.
	LDR	sp, =|__sigstk|
	LDR	sl, =|__sigstksize|
	SUB	sl, sp, sl
	MOV	fp, #0

	; Raise the signal
	BL	raise

	; Restore the old stack
	MOV	sp, v1
	
	; Go back into SVC mode and return.
	SWI	OS_EnterOS

	; We are no longer processing a callback
	ADR	a1, __cbflg
	LDR	a2, [a1, #0]
	BIC	a2, a2, #4
	STR	a2, [a1, #0]
	LDMFD	sp!, {r0-r12, pc}^

	EXPORT	|__h_event|
|__h_event|
	; Check for the event 'Internet event'.
	CMP	a1, #19
	MOVNES	pc, lr
	STMFD	sp!, {lr}
	; Convert the internet event into a suitable signal for raising
	CMP	a2, #1 ; Out-of-band data has arrived
	ADREQ	a1, callback_signal
	MOVEQ	a2, #SIGURG
	SWIEQ	XOS_AddCallBack

	LDMFD	sp!, {pc}^

	EXPORT	|__h_sigsys|
|__h_sigsys|
	ADR	ip,|__cbreg|
	STMIA	ip,{lr}
	MOV	ip,#SIGSYS
	STR	ip,|__cba1|
	ORR	ip,pc,#3		; SVC mode
	TEQP	ip,#0
	MOV	a1,a1
	STMFD	sp!,{lr}
	SWI	XOS_SetCallBack
	LDMFD	sp!,{lr}
	ADR	ip,|__cbreg|
	LDMIA	ip,{pc}^

	EXPORT	|__h_upcall|
	IMPORT	|__restore_calling_environment_handlers|
|__h_upcall|
	; Check for the application starting UpCall
	CMP	a1, #256
	MOVNES	pc, lr
	STMFD	sp!, {a1, a2, a3, a4, lr}
	BL	|__restore_calling_environment_handlers|
	LDMFD	sp!, {a1, a2, a3, a4, pc}^

	EXPORT	|__h_cback|
|__h_cback|
	ORR	lr, pc, #&0c000000	; USR mode IntOff
	MOVS	pc, lr

	; Load USR regs.  These would previously have been saved by
	; the likes of __h_sigill, __h_sigbus etc.
	ADR	a1, |__cbreg|
	LDMIA	a1, {a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,lr}
	; Cannot use writeback at this point on sp
	STMFD	sp,{a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,lr,pc}
	; sp still points above list of 16 registers.
	LDR	a4,|__cbreg|+60
	STR	a4,[sp,#-4]		; saved USR pc overwrites pc on stack
	SUB	sp, sp, #64

	; If bit 1 of __cbflg is set then don't re-execute the instruction
	; that caused the problem.   XXX FIXME:	 This is currently unused,
	; but I suspect it should be used by something.
	LDR	a1,|__cbflg|
	TST	a1, #2			; check __cbflg bit 1
	SUBNE	a4,a4,#4

	; Create an APCS-compilant signal stack frame
	MOV	a3,lr			; saved USR lr
	ADD	a2,sp,#64		; saved USR sp
	ORR	a1,fp,#&80000000	; saved USR fp | 0x80000000
	STMFD	sp!,{a1,a2,a3,a4}	; create signal frame
	ADD	fp,sp,#12
	SWI	XOS_IntOn

	; Check for an escape condition
	LDR	a1,|__cbflg|		; check __cbflg bit 0
	TST	a1,#1
	BEQ	|__h_cback_l1|

	; There was an escape condition.  Clear it.
	MOV	a1,#&7c
	SWI	XOS_Byte

|__h_cback_l1|
	LDR	a1,|__cbflg|
	BIC	a1,a1,#1
	STR	a1,|__cbflg|
	LDR	a1,|__cba1|
	MOV	v1,sp
	BL	|raise|
	ADD	sp,v1,#16		; skip signal frame
	LDMFD	sp,{a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,lr,pc}^

	EXPORT	|__cbreg|
	; User registers are preserved in here for the callback execution.
|__cbreg|	%	64

	; bit 0 Escape condition flag
	; bit 1 no re-execute inst. flag
	; bit 2 set if we are executing a callback.  This is to prevent
	;       re-entrancy.
|__cbflg|
	DCD	0

|__cba1|
	DCD	0


	IMPORT	|_exit|

	EXPORT	|__h_exit|
|__h_exit|
	ORR	lr,pc,#&0c000000	; USR mode IntOff
	MOVS	pc,lr
	MOV	a1,a1
	MOV	a1,#EXIT_SUCCESS
	B	|_exit|

	; Interval timer handler for ITIMER_REAL
	EXPORT	|__h_sigalrm|
|__h_sigalrm|	ROUT
	STMFD	sp!, {a1, a2, a3, lr}
	; Enter user mode
	TEQP	pc, #0
	MOV	a1, a1
	STMFD	sp!, {lr}
	; Raise the SIGALRM signal
	MOV	a1, #SIGALRM
	BL	|__raise|
	; Have we previously setup a CallEvery handler
	LDR	a1, |__h_sigalrm_sema|
	CMP	a1, #1
	BEQ	%F00
	; r12->it_interval = 0 secs and 0 usecs then exit
	LDMIA	ip, {a1, a2}
	CMP	a1, #0
	CMPEQ	a2, #0
	BEQ	%F00
	; Calculate delay in csecs between successive calls
	MOV	a3, #100
	MLA	a1, a3, a1, a2
	ADD	a1, a1, #1
	; Function to call
	ADR	a2, |__h_sigalrm_init|
	MOV	a3, ip
	SWI	XOS_CallEvery
	; Set semaphore to say we have a CallEvery already set up
	MOV	a1, #1
	STR	a1, |__h_sigalrm_sema|
00
	LDMFD	sp!, {lr}
	SWI	XOS_EnterOS
	LDMFD	sp!, {a1, a2, a3, pc}


	EXPORT	|__h_sigalrm_init|
|__h_sigalrm_init|
	STMFD	sp!, {a1, a2, lr}
	ADR	a1, |__h_sigalrm|
	MOV	a2, r12
	SWI	XOS_AddCallBack
	LDMFD	sp!, {a1, a2, pc}

	EXPORT	|__h_sigalrm_sema|
	; Set to one to prevent multiple CallEvery's being set up.
|__h_sigalrm_sema|
	DCD	0


	; Interval timer handler for ITIMER_VIRTUAL
	EXPORT	|__h_sigvtalrm|
|__h_sigvtalrm|
	STMFD	sp!, {a1, a2, a3, lr}
	TEQP	pc, #0
	MOV	a1, a1
	STMFD	sp!, {lr}
	MOV	a1, #SIGVTALRM
	BL	|__raise|
	LDR	a1, |__h_sigvtalrm_sema|
	CMP	a1, #1
	BEQ	%B00
	LDMIA	ip, {a1, a2}
	CMP	a1, #0
	CMPEQ	a2, #0
	BEQ	%B00
	MOV	a3, #100
	MLA	a1, a3, a1, a2
	ADD	a1, a1, #1
	ADR	a2, |__h_sigvtalrm_init|
	MOV	a3, ip
	SWI	XOS_CallEvery
	MOV	a1, #1
	STR	a1, |__h_sigvtalrm_sema|
	B	%B00

	EXPORT	|__h_sigvtalrm_init|
|__h_sigvtalrm_init|
	STMFD	sp!, {a1, a2, lr}
	ADR	a1, |__h_sigvtalrm|
	MOV	a2, r12
	SWI	XOS_AddCallBack
	LDMFD	sp!, {a1, a2, pc}

	EXPORT	|__h_sigvtalrm_sema|
	; Set to one to prevent multiple CallEvery's being set up.
|__h_sigvtalrm_sema|
	DCD	0

	; Interval timer handler for ITIMER_PROF
	EXPORT	|__h_sigprof|
|__h_sigprof|
	STMFD	sp!, {a1, a2, a3, lr}
	TEQP	pc, #0
	MOV	a1, a1
	STMFD	sp!, {lr}
	MOV	a1, #SIGPROF
	BL	|__raise|
	LDR	a1, |__h_sigprof_sema|
	CMP	a1, #1
	BEQ	%B00
	LDMIA	ip, {a1, a2}
	CMP	a1, #0
	CMPEQ	a2, #0
	BEQ	%B00
	MOV	a3, #100
	MLA	a1, a3, a1, a2
	ADD	a1, a1, #1
	ADR	a2, |__h_sigprof_init|
	MOV	a3, ip
	SWI	XOS_CallEvery
	MOV	a1, #1
	STR	a1, |__h_sigprof_sema|
	B	%B00

	EXPORT	|__h_sigprof_init|
|__h_sigprof_init|
	STMFD	sp!, {a1, a2, lr}
	ADR	a1, |__h_sigprof|
	MOV	a2, r12
	SWI	XOS_AddCallBack
	LDMFD	sp!, {a1, a2, pc}

	EXPORT	|__h_sigprof_sema|
	; Set to one to prevent multiple CallEverys being set up.
|__h_sigprof_sema|
	DCD	0


	END
