;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/_signal.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

; This file handles all the hairy exceptions that can occur when a
; program runs. This includes hardware exceptions like data abort and
; software exceptions like errors. It also handles callbacks, which are
; used to raise some execptions and by timers.
;
; The code is written for compactness rather than speed since it is
; executed under exceptional circumstances. This means we branch more
; to common code rather than having long sequences of conditionally
; executed instructions.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__unixlib_raise_signal|
	IMPORT	|__pthread_system_running|
	IMPORT	|__pthread_disable_ints|
	IMPORT	|__pthread_enable_ints|
	IMPORT	|__pthread_callback_pending|
	IMPORT	|__pthread_running_thread|


;-----------------------------------------------------------------------
; static void __raise (int signo)
;
; On entry:
;	a1 = signal number
; On exit:
;	All registers preserved
; Use:
;	Raise a signal while preserving all registers, providing the signal
;	number is non-zero. An APCS stack frame is established before
;	calling raise() so that __backtrace can print the registers when
;	it backtracks to this function.

|__raise|
	CMP	a1, #0
	MOVEQS	pc, lr
	STMFD	sp, {a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,lr,pc}
	SUB	sp, sp, #64
	SUB	ip, pc, #4
	MOV	a4, lr
	ADD	a3, sp, #64
	ORR	a2, fp, #&80000000	; for __backtrace()
	STMFD	sp!, {a2, a3, a4, ip}	; create signal frame
	ADD	fp, sp, #12
	MOV	v1, sp
	MOV	a2, a1
	MOV	a1, #0
	BL	|__unixlib_raise_signal|
	ADD	sp, v1, #16		; skip signal frame
	LDMFD	sp, {a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,pc}^

;-----------------------------------------------------------------------
; void __seterr (const _kernel_oserror *err)
; On entry:
;	a1 = err
; On exit:
;	APCS compliant. a1-a4, ip corrupted.
;
; Set UnixLib's errno to EOPSYS and copy the error from err to UnixLib's
; error buffer.
;
	EXPORT	|__seterr|
	NAME	__seterr
|__seterr|
	TEQ	a1, #0			; quick exit when no error
	MOVEQS	pc, lr

	STMFD	sp!, {v1-v5,lr}		; Stack working registers
	MOV	a3, #EOPSYS
	__set_errno	a3, a2 ; Set errno = EOPSYS

	[ __FEATURE_PTHREADS = 1
	LDR	a2, =|__pthread_running_thread|
	LDR	a2, [a2]
	ADD	a2, a2, #__PTHREAD_ERRBUF_OFFSET
	|
	LDR     a2, =|__ul_errbuf_errblock|
	]

	; Copy the error to UnixLib's buffer.
	MOV	a3, #|__ul_errbuf__size|
|__seterr.00|
	LDMIA	a1!, {a4, v1-v5, ip, lr}
	STMIA	a2!, {a4, v1-v5, ip, lr}
	SUBS	a3, a3, #8*4
	BNE	|__seterr.00|

	MOV	a1, #0			; ensure zero-terminated.
	STRB	a1, [a2, #-1]

	LDMFD	sp!, {v1-v5, pc}^

;-----------------------------------------------------------------------
; _kernel_oserror *_kernel_last_oserror (void)
; On exit:
;	APCS compliant. a1, a2 corrupted.
;
; Provide access to the last operating system error.  This is a
; SharedCLibrary compatibility function.  It appears in here because
; it is associated with the __seterr function above.  It is also not
; very big.
;
	EXPORT	|_kernel_last_oserror|
	NAME	_kernel_last_oserror
|_kernel_last_oserror|
	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_running_thread|
	LDR	a1, [a1]
	ADD	a1, a1, #__PTHREAD_ERRBUF_OFFSET
	|
	LDR	a1, =|__ul_errbuf_errblock|
	]
	LDR	a2, [a1, #0]
	CMP	a2, #0
	MOVNES	pc, lr
	MOV	a1, #0
	MOV	pc, lr

	; The following code is in a writable area because we need access to
	; the callback register save area without corrupting any registers
	; or needing to stack any registers. Thus, the callback register
	; save area must be within the reach of PC offset addressing.
	AREA	|C$$wrcode|, CODE

;-----------------------------------------------------------------------
; The hardware exception handlers (PRM 1-111).
;
; We are interested in some hardware exceptions and convert them into
; software signals which are propagated back to the program via raise().
; The hardware exceptions of interest are:
;
;	Undefined instruction		raise (SIGILL)
;	Prefetch abort			raise (SIGSEGV)
;	Data abort			raise (SIGSEGV)
;	Address exception		raise (SIGBUS)
;
; These handlers branch to the common callback handler code after saving
; the exception PC (from R14) in the callback register save area,
; putting the signal number in R14, changing to USR mode and then
; raising the appropriate signal. The old code used to save all the
; registers in the callback register save area, but we do not do that
; since it is unnecessary.
; The handlers will not be switched out by the pthread scheduler whilst
; they remain in SVC/IRQ mode (as callbacks only occur when in USR mode)

;-----------------------------------------------------------------------
; Undefined instruction handler
; Entered in SVC mode
; On entry:
;	lr = address of instruction 1 word after the undefined instruction.
;
	EXPORT	|__h_sigill|
	NAME	__h_sigill
|__h_sigill|
	STR	lr, |__cbreg|+60
	ADR	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers
	MOV	a1, #SIGILL
	SUB	lr, lr, #13*4		; adjust base register back
	STR	a1, |__cba1|
	B	|__h_cback|

;-----------------------------------------------------------------------
; Address exception handler
; Entered in SVC mode
; On entry:
;	lr = address of instruction 1 word after the address exception.
;
	EXPORT	|__h_sigbus|
	NAME	__h_sigbus
|__h_sigbus|
	SUB	lr, lr, #4
	STR	lr, |__cbreg|+60
	ADR	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers
	MOV	a1, #SIGBUS
	SUB	lr, lr, #13*4		; adjust base register back
	STR	a1, |__cba1|
	B	|__h_cback|

;-----------------------------------------------------------------------
; Prefetch abort handler
; Entered in SVC mode
; On entry:
;	lr = address of instruction 1 word after the aborted instruction.
;
	EXPORT	|__h_sigsegv0|
	NAME	__h_sigsegv0
|__h_sigsegv0|
	SUB	lr, lr, #4
	STR	lr, |__cbreg|+60
	ADR	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers
	MOV	a1, #SIGSEGV
	SUB	lr, lr,#13*4		; adjust base register back
	STR	a1, |__cba1|
	B	|__h_cback|

;-----------------------------------------------------------------------
; Data abort handler
; Entered in SVC mode
; On entry:
;	lr = address of instruction 2 words after the aborted instruction.
;
; We could try and be extremely clever and work out the cause of the
; data abort and fixup the possibly modified base register. However, it
; is really of dubious value and certainly a lot of effort for something
; unlikely to be noticed by the user. Note, to fixup the base register
; we need to:
;
;   . know whether the ARM is configured to early or late aborts. This
;     requires determing the processor type (trickier with ARM 2) and
;     then using something like a co-processor instruction.
;
;   . decode the instruction and determine what, if any, fixup to the
;     base register is required.
;
;   . protect against a nested data abort.
;
;   . possibly handle a SVC mode data abort.
;
;   . update this code whenever a new ARM standard is released.
;
;   . do the bits I've forgotten about.
;
; It's all rather pointless since RO is not a virtual memory OS and the
; OS should be fixing up the base register before calling us. Further,
; other than printing out the correct value for the base register, we
; cannot continue executing the program, so the signal handler is not
; expected to return.
; Finally, the SCL does not bother, so why should we ?
	EXPORT	|__h_sigsegv1|
	NAME	__h_sigsegv1
|__h_sigsegv1|
	SUB	lr, lr, #8
	STR	lr, |__cbreg|+60
	ADR	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers
	MOV	a1, #SIGSEGV
	STR	a1, |__cba1|
	B	|__h_cback|


;-----------------------------------------------------------------------
; The software handlers (PRM 1-288).
;
; We are interested in some software exceptions and convert them into
; software signals which are propagated back to the program via raise().
; Some software exceptions require that the raise() call is via a
; callback, because the exception occurs in non USR mode.
; The software exceptions of interest are:
;
;	Error		raise (SIGERR | SIGEMT | SIGFPE)
;	Escape		raise (SIGINT)
;	Event		on Internet event, raise (SIGIO | SIGURG | SIGPIPE)
;	Exit		see sys/syslib.s
;	Unused SWI	raise (SIGSYS)
;	UpCall		restore handlers on UpCall 256
;	CallBack	raise a deferred signal
;


;-----------------------------------------------------------------------
; Error handler (1-289).
; Entered in USR mode.
; On entry:
;	a1 = pointer to [pc, error number, zero term. error string]
; On exit:
;	Undefined.
;
; Set UnixLib's errno to EOPSYS and raise one of SIGERR, SIGFPE or SIGEMT.
;
; Decode the error number and raise an appropriate signal.
; PRM 1-43 states that bit 31, if set, implies that the error was
; a serious error, usually a hardware exception or a floating point
; exception, from which it was not possible to sensibly return
; with V set. In such cases different error ranges are used:
;
;	&80000000 - &800000FF		Machine exceptions
;	&80000100 - &800001FF		CoProcessor exceptions
;	&80000200 - &800002FF		Floating Point exceptions
;	&80000300 - &800003FF		Econet exceptions
;
; For these classes of exceptions, we map Floating Point exceptions
; to SIGFPE and all others to SIGEMT.
; For non-serious errors, bit 31 = 0, raise SIGERR.
;
	EXPORT	|__h_error|
	NAME	"*** UnixLib error handler ***"
|__h_error|
	; Entered in USR mode. Setup an APCS stack frame
	; so we can get a proper stack backtrace in case anything
	; goes horribly wrong.
	MOV	ip, sp
	STMFD	sp!, {a1, a2, a3, a4, fp, ip, lr, pc}
	SUB	fp, ip, #4

	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BLNE	|__pthread_disable_ints|
	]

	; Set errno to EOPSYS
	MOV	a2, #EOPSYS
	__set_errno	a2, a1

	[ __FEATURE_PTHREADS = 1
	IMPORT	|memcpy|
	; Copy error buffer into thread specific storage
	LDR	a1, =|__pthread_running_thread|
	LDR	a1, [a1]
	ADD	a1, a1, #__PTHREAD_ERRBUF_OFFSET
	LDR	a2, =|__ul_errbuf_errblock|
	LDR	a3, =|__ul_errbuf__size|
	LDR	a3, [a3]
	BL	|memcpy|
	]

	; Check the error number. Its value will determine the
	; appropriate signal to call.
	LDR	a2, =|__ul_errbuf_errblock|
	LDR	a3, [a2, #0]

	; Check bit 31 of the error number.  If it is set, then it
	; indicates a serious error, usually a hardware exception e.g.
	; a page-fault or a floating point exception.
	TST	a3, #&80000000
	BNE	unrecoverable_error
	; Bit 31 was not set so raise a RISC OS error.
	MOV	a2, #SIGERR
	MOV	a1, #0
	BL	|__unixlib_raise_signal|

	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BLNE	|__pthread_enable_ints|
	]

	LDMEA	fp, {a1, a2, a3, a4, fp, sp, pc}^

unrecoverable_error
	; Bit 31-was set, therefore it was a hardware error.

	; Print the error
	ADR	a1, unrecoverable_error_msg
	SWI	XOS_Write0
	ADD	a1, a2, #4		; Write out __ul_errbuf_errblock+4
	SWI	XOS_Write0
	SWI	XOS_NewLine

	; Test the type of hardware error.  We currently aren't doing
	; much other than saying it was a Floating Point Exception
	; or something else.
	; For compatability with ISO C99 we should decode the FP exception
	; more accurately.
	BIC	a3, a3, #&80000000
	MOV	a3, a3, LSR #8
	AND	a3, a3, #&FF
	CMP	a3, #&02
	MOVEQ	a2, #SIGFPE	;  A floating point exception
	MOVNE	a2, #SIGEMT	;  A RISC OS exception.
	MOV	a1, #0
	BL	|__unixlib_raise_signal|

	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BLNE	|__pthread_enable_ints|
	]

	LDMEA	fp, {a1, a2, a3, a4, fp, sp, pc}^

unrecoverable_error_msg
	DCB	13, 10, "Unrecoverable error received:", 13, 10, "  ", 0

;-----------------------------------------------------------------------
; Escape handler (1-290).
; Entered in IRQ mode.
; On entry:
;	r11/fp = bit 6 set, implies escape condition.
;	r12/ip = pointer to workspace, if set up - should never be 1.
;	r13/sp = a full, descending stack pointer
; On exit:
;	ip = 1 means set callback flag
;	All other registers undefined
;
; This handler is called when an Escape condition is detected.
; Be quick by just clearing the escape flag and setting a callback,
; by settting r12 = 1 on exit, to raise SIGINT.
; FIXME. What action must we take to guard against recursive escapes and
; race conditions ?
;
	EXPORT	|__h_sigint|
	NAME	__h_sigint
|__h_sigint|
	; Entered in IRQ mode. Be quick by just clearing the escape
	; flag and setting a callback.
	TST	r11, #64		; bit 6
	MOVNE	ip, #SIGINT
	STRNE	ip, |__cba1|
	; Set the escape condition flag
	LDR	ip, |__cbflg|
	ORRNE	ip, ip, #1		; set CallBack
	BICEQ	ip, ip, #1		; clear CallBack
	STR	ip,|__cbflg|		; set/clear __cbflg bit 0

	MOVEQ	ip, #0
	MOVNE	ip, #1
	MOVS	pc, lr

;-----------------------------------------------------------------------
; Event handler (1-290).
; Entered in either SVC *or* IRQ mode, called from EventV.
; On entry:
;	a1 = event reason code
;	a2... parameters according to event code
;	ip = pointer to workspace, if set up - should never be 1
;	sp = full descending stack
; On exit:
;	ip = 1 means set the callback flag
;	All other registers undefined
;
; We catch Internet events and decode the sub-event into a signal which
; is raised via a callback to the user program.
; All other events are ignored.
;

Internet_Event	EQU	19

	EXPORT	|__h_event|
	NAME	__h_event
|__h_event|
	; Check for the event 'Internet event'.
	TEQ	a1, #Internet_Event
	MOVNES	pc, lr

	; Convert the internet event into a suitable signal for raising
	CMP	a2, #1 ; Out-of-band data has arrived
	MOVEQ	ip, #SIGURG
	STREQ	ip, |__cba1|
	MOVEQ	ip, #1 ; Callback set if R12 = 1
	MOVS	pc, lr

;-----------------------------------------------------------------------
; Unused SWI handler (1-291).
; Entered in SVC mode, IRQ state is same state as caller (so undefined).
; Called from UKSWIV (1-95).
; On entry:
;	r11/fp = SWI number (Bit 17 (the X bit) clear)
;	r13/sp = SVC stack pointer
;	r14/lr = user PC with V cleared
; On exit:
;	r10/sl, r11/fp, r12/ip can be corrupted.
;	No other details known.
;
; raise SIGSYS via callback.
; Should return a `No such SWI' error ?
;
	EXPORT	|__h_sigsys|
	NAME	__h_sigsys
|__h_sigsys|
	STR	lr, |__cbreg|+60
	ADR	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers
	MOV	a1, #SIGSYS
	SUB	lr, lr, #13*4		; adjust base register back
	STR	a1, |__cba1|
	B	|__h_cback|

;-----------------------------------------------------------------------
; Upcall handler (1-291).
; Entered in SVC mode, called from UpCallV.
; On entry:
;	r12/ip = pointer to workspace
; On exit:
;	All registers preserved
;
; When UpCall_NewApplication is received, then all handlers should be
; restored to their original values and return to caller, preserving
; registers.
;

UpCall_NewApplication	EQU	256

	IMPORT	|__env_riscos|
	EXPORT	|__h_upcall|
	NAME	__h_upcall
|__h_upcall|
	; Check for the application starting UpCall
	CMP	a1, #UpCall_NewApplication
	MOVNES	pc, lr
	B	|__env_riscos|

	IMPORT	|__pthread_callback|
	EXPORT	|__h_cback|
	NAME	__h_cback
|__h_cback|
	[ __FEATURE_PTHREADS = 1
	; Check if the pthreads code was expecting a callback

	LDR	a1, =|__pthread_callback_pending|
	LDR	a2, [a1]
	CMP	a2, #0
	MOVNE	a2, #0
	STRNE	a2, [a1]
	BNE	|__pthread_callback|
	; We aren't expecting a callback, so prevent any further callbacks then handle this one
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BL	|__pthread_disable_ints|
	]

	TEQP	pc, #IFlag	; USR mode IntOff (irq off, fiq on)
	; The USR mode registers r0-r15 are extracted from the callback
	; register block while irqs are disabled. The registers are then
	; saved on the USR mode stack while ensuring that the USR sp is
	; valid by not pointing above saved data. So, load the registers,
	; allocate room on the stack and then store the original USR
	; registers. This requires special treatment for sp and pc because
	; they are naturally affected by the following code.
	ADR	a1,|__cbreg|		; No access to banked registers
	LDMIA	a1,{a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,lr}
	; Stack all the registers for __backtrace (if it is called).
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
	; There was an escape condition.  Clear it.
	MOVNE	a1,#&7e
	SWINE	XOS_Byte		; This calls our escape handler

	MOV	a1, #0
	LDR	a2,|__cba1|
	MOV	v1,sp
	BL	|__unixlib_raise_signal|

	[ __FEATURE_PTHREADS = 1
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	BLNE	|__pthread_enable_ints|
	]

	ADD	sp,v1,#16		; skip signal frame
	LDMFD	sp,{a1,a2,a3,a4,v1,v2,v3,v4,v5,v6,sl,fp,ip,sp,lr,pc}^

	; User registers are preserved in here for the callback execution.
	EXPORT	|__cbreg|
|__cbreg|	%	64

	; bit 0 Escape condition flag
	; bit 1 no re-execute inst. flag
	; bit 2 set if we are executing a callback.  This is to prevent
	;       re-entrancy.
|__cbflg|
	DCD	0

|__cba1|
	DCD	0

; Exit handler
; Called in USR mode
	IMPORT	|_exit|
	EXPORT	|__h_exit|
	NAME	__h_exit
|__h_exit|
	ORR	lr,pc,#&0c000000	; USR mode IntOff
	MOVS	pc,lr
	MOV	a1,a1
	MOV	a1,#EXIT_SUCCESS
	B	|_exit|

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; The various timer handlers. We know that these can only be invoked
; from an application that is not a taskwindow application, since the
; code that calls OS_CallAfter checks that we are not in a taskwindow.
; We also have a check for when we are a WIMP application.

; Interval timer handler for ITIMER_REAL
; On entry R12 = pointer to interval timerval
; Preserve all registers

	; Interval timer handler for ITIMER_REAL
	EXPORT	|__h_sigalrm|
	NAME	__h_sigalrm
|__h_sigalrm|
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
	BEQ	itimer_exit
	; r12->it_interval = 0 secs and 0 usecs then exit
	LDMIA	ip, {a1, a2}
	CMP	a1, #0
	CMPEQ	a2, #0
	BEQ	itimer_exit
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
itimer_exit
	; Common exit used by the three internal timer handlers.
	LDMFD	sp!, {lr}
	SWI	XOS_EnterOS
	LDMFD	sp!, {a1, a2, a3, pc}


	EXPORT	|__h_sigalrm_init|
	NAME	__h_sigalrm_init
|__h_sigalrm_init|
	STMFD	sp!, {a1, a2, lr}
	ADR	a1, |__h_sigalrm|
	MOV	a2, r12
	SWI	XOS_AddCallBack
	LDMFD	sp!, {a1, a2, pc} ;  return without restoring PSR

	; Set to one to prevent multiple CallEverys being set up.
	EXPORT	|__h_sigalrm_sema|
|__h_sigalrm_sema|
	DCD	0

; Interval timer handler for ITIMER_VIRTUAL
; On entry R12 = pointer to interval timerval
; Preserve all registers
	EXPORT	|__h_sigvtalrm|
	NAME	__h_sigvtalrm
|__h_sigvtalrm|
	STMFD	sp!, {a1, a2, a3, lr}
	TEQP	pc, #0		;  Enter user mode
	MOV	a1, a1
	STMFD	sp!, {lr}
	MOV	a1, #SIGVTALRM	;  No access to banked registers
	BL	|__raise|
	LDR	a1, |__h_sigvtalrm_sema|
	CMP	a1, #1
	BEQ	itimer_exit
	LDMIA	ip, {a1, a2}
	CMP	a1, #0
	CMPEQ	a2, #0
	BEQ	itimer_exit
	MOV	a3, #100
	MLA	a1, a3, a1, a2
	ADD	a1, a1, #1
	ADR	a2, |__h_sigvtalrm_init|
	MOV	a3, ip
	SWI	XOS_CallEvery
	MOV	a1, #1
	STR	a1, |__h_sigvtalrm_sema|
	B	itimer_exit

	EXPORT	|__h_sigvtalrm_init|
	NAME	__h_sigvtalrm_init
|__h_sigvtalrm_init|
	STMFD	sp!, {a1, a2, lr}
	ADR	a1, |__h_sigvtalrm|
	MOV	a2, r12
	SWI	XOS_AddCallBack
	LDMFD	sp!, {a1, a2, pc} ;  return without restoring PSR

	; Set to one to prevent multiple CallEverys being set up.
	EXPORT	|__h_sigvtalrm_sema|
	NAME	__h_sigvtalrm_sema
|__h_sigvtalrm_sema|
	DCD	0

; Interval timer handler for ITIMER_PROF
; On entry R12 = pointer to interval timerval
; Preserve all registers
	EXPORT	|__h_sigprof|
	NAME	__h_sigprof
|__h_sigprof|
	STMFD	sp!, {a1, a2, a3, lr}
	TEQP	pc, #0		; Enter user mode
	MOV	a1, a1
	STMFD	sp!, {lr}
	MOV	a1, #SIGPROF	; No access to banked registers
	BL	|__raise|
	LDR	a1, |__h_sigprof_sema|
	CMP	a1, #1
	BEQ	itimer_exit
	LDMIA	ip, {a1, a2}
	CMP	a1, #0
	CMPEQ	a2, #0
	BEQ	itimer_exit
	MOV	a3, #100
	MLA	a1, a3, a1, a2
	ADD	a1, a1, #1
	ADR	a2, |__h_sigprof_init|
	MOV	a3, ip
	SWI	XOS_CallEvery
	MOV	a1, #1
	STR	a1, |__h_sigprof_sema|
	B	itimer_exit

	EXPORT	|__h_sigprof_init|
	NAME	__h_sigprof_init
|__h_sigprof_init|
	STMFD	sp!, {a1, a2, lr}
	ADR	a1, |__h_sigprof|
	MOV	a2, r12
	SWI	XOS_AddCallBack
	LDMFD	sp!, {a1, a2, pc} ;  return without restoring PSR

	; Set to one to prevent multiple CallEverys being set up.
	EXPORT	|__h_sigprof_sema|
|__h_sigprof_sema|
	DCD	0

	AREA	|C$$zidata|, DATA, NOINIT

;-----------------------------------------------------------------------
; UnixLib's error buffer.
; If threads are in use then it is only temporary, as the error handler
; copies it into the thread's error buffer.
;
; According to 1-289 the error handler must provide an error buffer of
; size 256 bytes, the address of which should be set along with the
; handler address.  This is wrong, it actually requires a 260 byte
; buffer, for the error PC and the normal 256 byte RISC OS error
; block.
;
	EXPORT  |__ul_errbuf|
|__ul_errbuf|
	%	4	; PC when error occurred
|__ul_errbuf_errblock|
	%	4	; Error number provided with the error
	%	252	; Error string, zero terminated
|__ul_errbuf__size|	EQU	{PC} - |__ul_errbuf_errblock|

	END
