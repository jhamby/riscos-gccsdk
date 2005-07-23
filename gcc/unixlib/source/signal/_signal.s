; Signal exception handling
; Copyright (c) 2002, 2003, 2004, 2005 UnixLib Developers

; This file handles all the hairy exceptions that can occur when a
; program runs. This includes hardware exceptions like data abort and
; software exceptions like errors. It also handles callbacks, which are
; used to raise some exceptions and by timers.
;
; The code is written for compactness rather than speed since it is
; executed under exceptional circumstances. This means we branch more
; to common code rather than having long sequences of conditionally
; executed instructions.

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__unixlib_raise_signal|
	IMPORT	|__pthread_disable_ints|
	IMPORT	|__pthread_enable_ints|
	IMPORT	|__pthread_running_thread|
	IMPORT	|__pthread_worksemaphore|
	IMPORT	|__ul_global|
	IMPORT	|__ul_memory|
	IMPORT	|__unixlib_fatal|
	IMPORT	|exit|


;-----------------------------------------------------------------------
; static void __raise (int signo)
;
; On entry:
;	a1 = signal number
; On exit:
;	All registers preserved
; Use:
;	Change to USR mode and raise a signal while preserving all registers,
;	providing the signal number is non-zero. We can't make any assumptions
;	about the state of the USR mode stack, so set up our own.
;	This can only be called in SVC mode.

	NAME	__raise
|__raise|
	TEQ	a1, #0
	MOVEQ	pc, lr

	; Saves LR_SVC.
	STMFD	sp!, {lr}
	; Saves all USR registers on the SP_SVC stack.
	SUB	sp, sp, #15*4
	STMIA	sp, {a1, a2, a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, sp, lr}^
	MOV	a1, a1

	MOV	v1, a1
	BL	|__setup_signalhandler_stack|

	[ __UNIXLIB_FEATURE_PTHREADS > 0
	BL	|__pthread_disable_ints|
	]

	; Drop into USR mode and raise the signal
	CHGMODE	a2, USR_Mode
	MOV	a2, v1
	MOV	a1, #0
	BL	|__unixlib_raise_signal|
	; Return to SVC mode.
	SWI	XOS_EnterOS

	[ __UNIXLIB_FEATURE_PTHREADS > 0
	BL	|__pthread_enable_ints|
	]

	LDR	a2, =|__ul_global|
	LDR	a1, [a2, #GBL_EXECUTING_SIGNALHANDLER]
	SUB	a1, a1, #1
	STR	a1, [a2, #GBL_EXECUTING_SIGNALHANDLER]

	; Restores all USR registers from the SP_SVC stack.
	LDMIA	sp, {a1, a2, a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, sp, lr}^
	MOV	a1, a1
	ADD	sp, sp, #15*4
	LDMIA	sp!, {pc}


;-----------------------------------------------------------------------
; void __ul_seterr (const _kernel_oserror *err, int seterrno)
; On entry:
;	a1 = RISC OS error block
;	a2 = non-zero if errno should be set,
;	     zero if errno should not be set
; On exit:
;	APCS-32 compliant.
;       a1 = -1 if there was an error, else 0
;       a2-a4, ip corrupted.
;
; Set UnixLib's errno to EOPSYS and copy the error from err to UnixLib's
; error buffer.
;
	EXPORT	|__ul_seterr|
	NAME	__ul_seterr
|__ul_seterr|
	TEQ	a1, #0			; quick exit when no error
	MOVEQ	pc, lr

	STMFD	sp!, {v1-v5, lr}	; Stack working registers
	CMP	a2, #0
	BEQ	|seterr01|		; user requested not to set errno
	MOV	a3, #EOPSYS
	__set_errno	a3, a2		; Set errno = EOPSYS

|seterr01|
	[ __UNIXLIB_FEATURE_PTHREADS > 0
	LDR	a2, =|__pthread_running_thread|
	LDR	a2, [a2]

        ; If __pthread_running_thread == NULL then something has gone
        ; wrong during initialisation, or we have an inconsistent build,
        ; or something has written over UnixLib's memory.  Whichever
        ; way, we're screwed.  Better report the problem and quit fast.
        CMP     a2, #0
        ADREQ   a1, |seterr_fatal|
        BEQ     |__unixlib_fatal|
	ADD	a2, a2, #__PTHREAD_ERRBUF_OFFSET
	|
	LDR     a2, =|__ul_errbuf_errblock|
	]

	; Copy the error to UnixLib's buffer.

	; If errnum is zero, we cheat and make it 1 so that at least
	; _kernel_last_oserror returns that error message and not NULL.
	; FIXME: better have a boolean somewhere saying we filled in
	; a real error message but that requires changes to pthread_t
	; structure which has implications.
	LDMIA	a1!, {a4, v1-v5, ip, lr}
	TEQ	a4, #0
	MOVEQ	a4, #1
	STMIA	a2!, {a4, v1-v5, ip, lr}

	MOV	a3, #|__ul_errbuf__size| - 8*4
|seterr00|
	LDMIA	a1!, {a4, v1-v5, ip, lr}
	STMIA	a2!, {a4, v1-v5, ip, lr}
	SUBS	a3, a3, #8*4
	BNE	|seterr00|

	MOV	a1, #0			; ensure zero-terminated.
	STRB	a1, [a2, #-1]

        MVN     a1, #0
	LDMFD	sp!, {v1-v5, pc}

|seterr_fatal|
	DCB	"UnixLib inconsistency found: __pthread_running_thread == NULL.  Exiting", 0
	ALIGN

;-----------------------------------------------------------------------
; _kernel_oserror *_kernel_last_oserror (void)
; On exit:
;	APCS-32 compliant. a1, a2 corrupted.
;
; Provide access to the last operating system error.  This is a
; SharedCLibrary compatibility function.  It appears in here because
; it is associated with the __seterr function above.  It is also not
; very big.
;
	EXPORT	|_kernel_last_oserror|
	NAME	_kernel_last_oserror
|_kernel_last_oserror|
	[ __UNIXLIB_FEATURE_PTHREADS > 0
	LDR	a1, =|__pthread_running_thread|
	LDR	a1, [a1, #0]
	LDR	a2, [a1, #__PTHREAD_ERRBUF_OFFSET]!
	|
	LDR	a1, =|__ul_errbuf_errblock|
	LDR	a2, [a1, #0]
	]
	TEQ	a2, #0
	MOVEQ	a1, #0
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
; Entered in SVC26 mode (26 bit system) or UND32 mode (32 bit system).
; On entry:
;	lr = address of instruction 1 word after the undefined instruction.
;
	EXPORT	|__h_sigill|
	NAME	__h_sigill
|__h_sigill|
	SUB	lr, lr, #4
	STR	lr, |__cbreg| + 15*4
	ADRL	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers

	TEQ	pc, pc			; store CPSR at time of exception
	LDRNE	a1, [lr, #4*2]
	MRSEQ	a1, SPSR
	STR	a1, [lr, #4*3]

	MOV	a1, #SIGILL
	STR	a1, |__cba1|
	B	|__h_cback_common|

;-----------------------------------------------------------------------
; Address exception handler
; Entered in SVC26 mode
; On entry:
;	lr = address of instruction 1 word after the address exception.
;
	EXPORT	|__h_sigbus|
	NAME	__h_sigbus
|__h_sigbus|
	SUB	lr, lr, #4
	STR	lr, |__cbreg| + 15*4
	ADRL	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers
	; No need to save CPSR because an address exception can only
	; happen in 26 bit mode and then CPSR is nothing more than
	; flags from pc.
	MOV	a1, #SIGBUS
	STR	a1, |__cba1|
	B	|__h_cback_common|

;-----------------------------------------------------------------------
; Prefetch abort handler
; Entered in SVC26 mode (26 bit system) or ABT32 (32 bit system).
; On entry:
;	lr = address of instruction 1 word after the aborted instruction.
;
	EXPORT	|__h_sigsegv0|
	NAME	__h_sigsegv0
|__h_sigsegv0|
	SUB	lr, lr, #4
	STR	lr, |__cbreg| + 15*4
	ADR	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers

	TEQ	pc, pc			; store CPSR at time of exception
	LDRNE	a1, [lr, #4*2]
	MRSEQ	a1, SPSR
	STR	a1, [lr, #4*3]

	MOV	a1, #SIGSEGV
	STR	a1, |__cba1|
	B	|__h_cback_common|

;-----------------------------------------------------------------------
; Data abort handler
; Entered in SVC26 mode (26 bit system) or ABT32 (32 bit system).
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
; It's all rather pointless since RISC OS is not a virtual memory OS and
; the OS should be fixing up the base register before calling us. Further,
; other than printing out the correct value for the base register, we
; cannot continue executing the program, so the signal handler is not
; expected to return.
; Finally, the SCL does not bother, so why should we ?
	EXPORT	|__h_sigsegv1|
	NAME	__h_sigsegv1
|__h_sigsegv1|
	SUB	lr, lr, #8
	STR	lr, |__cbreg| + 15*4
	ADR	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers

	TEQ	pc, pc			; store CPSR at time of exception
	LDRNE	a1, [lr, #4*2]
	MRSEQ	a1, SPSR
	STR	a1, [lr, #4*3]

	MOV	a1, #SIGSEGV
	STR	a1, |__cba1|
	B	|__h_cback_common|


;-----------------------------------------------------------------------
; The software handlers (PRM 1-288).
;
; We are interested in some software exceptions and convert them into
; software signals which are propagated back to the program via raise().
; Some software exceptions require that the raise() call is via a
; callback, because the exception occurs in non USR mode.
; The software exceptions of interest are:
;
;	Error		raise (SIGOSERROR | SIGEMT | SIGFPE)
;	Escape		raise (SIGINT)
;	Event		on Internet event, raise (SIGIO | SIGURG | SIGPIPE)
;	Exit		see sys/syslib.s
;	Unused SWI	raise (SIGSYS)
;	UpCall		restore handlers on UpCall 256
;	CallBack	raise a deferred signal
;


;-----------------------------------------------------------------------
; Error handler (1-289).
; Entered in USR26 mode (26 bit system) or USR32 (32 bit system).
; On entry:
;	a1 = pointer to [pc, error number, zero terminated error string]
; On exit:
;	Undefined.
;
; Set UnixLib's errno to EOPSYS and raise one of SIGOSERROR, SIGFPE
; or SIGEMT.
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
; For non-serious errors, bit 31 = 0, raise SIGOSERROR.
;
	EXPORT	|__h_error|
	NAME	UnixLibErrorHandler
|__h_error|
	MOV	v1, fp	; Save some USR regs. There is no guarantee that these
	MOV	v2, sp	; contain anything of use, but they seem to be the USR
	MOV	v3, lr	; regs at the time of the error.
	SWI	XOS_EnterOS	; Change to SVC mode so we don't get any
				; callbacks while we are setting up the stack

	[ __UNIXLIB_FEATURE_PTHREADS > 0
	LDR	a1, =|__ul_global|
	LDR	a1, [a1, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
	BLNE	|__pthread_disable_ints|
	]

	BL	|__setup_signalhandler_stack|
	CHGMODE	a1, USR_Mode	; Back to USR mode now we have a stack

	ADR	v4, |__h_error| + 4*3	; Point at handler name for backtrace
	STMFD	sp!, {v1, v2, v3, v4}	; Setup an APCS-32 stack frame so we
	ADD	fp, sp, #4*3		; can get a proper stack backtrace in
					; case anything goes horribly wrong.

	LDR	v1, =|__ul_callbackfp|	; We don't have a r0-r15, CPSR
	MOV	a1, #0			; snapshot on our sp stack.
	STR	a1, [v1]

	; Set errno to EOPSYS
	MOV	a2, #EOPSYS
	__set_errno	a2, a1

	[ __UNIXLIB_FEATURE_PTHREADS > 0
	IMPORT	|memcpy|
	; Copy error buffer into thread specific storage
	LDR	a1, =|__pthread_running_thread|
	LDR	a1, [a1]
	ADD	a1, a1, #__PTHREAD_ERRBUF_OFFSET
	LDR	a2, =|__ul_errbuf_errblock|
	MOV	a3, #|__ul_errbuf__size|
	BL	|memcpy|
	]

	; Mark the error buffer as valid.
	LDR	a1, =|__ul_errbuf_valid|
	MOV	a2, #1
	STR	a2, [a1]

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
	MOV	a2, #SIGOSERROR
	MOV	a1, #0
	BL	|__unixlib_raise_signal|

	MOV	a1, #EXIT_FAILURE
	B	exit	; There is nowhere to go if the signal handler returns

unrecoverable_error
	; Bit 31-was set, therefore it was a hardware error.

	; Test the type of hardware error.  We currently aren't doing
	; much other than saying it was a Floating Point Exception
	; or something else.
	; FIXME: for compatability with ISO C99 we should decode the FP
	; exception more accurately.
	BIC	a3, a3, #&80000000
	MOV	a3, a3, LSR #8
	AND	a3, a3, #&FF
	CMP	a3, #&02
	MOVNE	a2, #SIGEMT	;  A RISC OS exception.
	BNE	|non_fp_exception|

	; Store FP registers.
	LDR	a1, =|__ul_fp_registers|
	RFS	a2		; Read FP status register
	STR	a2, [a1], #4
	BIC	a3, a3, #&FF<<16; Disable all exceptions to prevent the
	WFS	a3		; signal handler triggering another exception

	; We can't use SFM because we really want to write double
	; values.
	STFD	f0, [a1], #8
	STFD	f1, [a1], #8
	STFD	f2, [a1], #8
	STFD	f3, [a1], #8
	STFD	f4, [a1], #8
	STFD	f5, [a1], #8
	STFD	f6, [a1], #8
	STFD	f7, [a1], #8

	WFS	a2		; Restore FPE status

	MOV	a2, #SIGFPE	;  A floating point exception

non_fp_exception
	MOV	a1, #0

	BL	|__unixlib_raise_signal|

	MOV	a1, #EXIT_FAILURE
	B	exit	; There is nowhere to go if the signal handler returns

;-----------------------------------------------------------------------
; Escape handler (1-290).
; Entered in IRQ mode (FIXME: is this *really* correct ?) (26 bit system)
; or SVC32 (32 bit system).
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
	STR	ip, |__cbflg|		; set/clear __cbflg bit 0

	MOVEQ	ip, #0
	MOVNE	ip, #1
	MOV	pc, lr

;-----------------------------------------------------------------------
; Event handler (1-290).
; Entered in either SVC26 *or* IRQ26 mode (26 bit system), or SVC32 *or*
; IRQ32 (32 bit system), called from EventV.
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
	MOVNE	pc, lr

	; Convert the internet event into a suitable signal for raising
	TEQ	a2, #1		; Out-of-band data has arrived
	MOVNE	pc, lr

	; Set the internet event flag
	LDR	ip, |__cbflg|
	ORR	ip, ip, #2
	STR	ip, |__cbflg|

	MOV	ip, #SIGURG
	STR	ip, |__cba1|
	MOV	ip, #1		; CallBack set if R12 = 1
	MOV	pc, lr

;-----------------------------------------------------------------------
; Unused SWI handler (1-291).
; Entered in SVC mode, IRQ state is same state as caller (so undefined).
; Called from UKSWIV (1-95).
; On entry:
;	r11/fp = SWI number (Bit 17 (the X bit) clear)
;	r13/sp = SVC stack pointer
;	r14/lr = return address in the kernel with NZCVIF flags the
;                same as the callers (except V clear) (26 bit system) or
;                return address in the kernel (32 bit system).
;       PSR flags undefined (except I+F as caller)
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
	STR	lr, |__cbreg|+15*4
	ADR	lr, |__cbreg|
	STMIA	lr!, {a1-ip}		; store non-banked registers
	STMIA	lr, {sp, lr}^		; store banked registers
	MOV	a1, #0			; No meaningful CPSR so null it.
	STR	a1, [lr, #4*3]
	MOV	a1, #SIGSYS
	STR	a1, |__cba1|
	B	|__h_cback_common|

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
; SUL calls __env_riscos_and_wimpslot directly

;-----------------------------------------------------------------------
; CallBack handler.
; Entered in SVC26 or IRQ26 mode (26 bit system) or SVC32 (32 bit system),
; IRQs disabled.
; __h_cback_commom is called directly from other handlers.
; On entry:
;
; On exit:
;	All registers should be loaded from the register save area

	IMPORT	|__pthread_callback|
	EXPORT	|__h_cback|
	NAME	__h_cback
|__h_cback|
	[ __UNIXLIB_FEATURE_PTHREADS > 0
	; Check that the return PC value is within our wimpslot.
	; If it isn't, then we don't want to do a context switch
	; so return straight away.
	LDR	a1, |__cbreg| + 15*4
	TEQ	pc, pc
	BICNE	a1, a1, #&fc000003

	LDR	a3, =|__ul_memory|
	LDR	a2, [a3, #MEM_ROBASE]
	CMP	a1, a2
	BLO	return_quickly

	LDR	a2, [a3, #MEM_APPSPACE_LIMIT]
	CMP	a1, a2
	BHI	return_quickly

	LDR	a1, |__cbflg|
	TST	a1, #3			; Check escape and internet flags
	BNE	|__h_cback_common|

	LDR	a3, =|__ul_global|
	LDR	a1, [a3, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
	BNE	|__pthread_callback|
	B	|__h_cback_common|

return_quickly
	ADR	lr, |__cbreg|
	LDR	a1, [lr, #16*4]
	MSR	SPSR_cxsf, a1
	LDMIA	lr, {a1-lr}^
	MOV	a1, a1
	LDR	lr, [lr, #15*4]
	MOVS	pc, lr
	]

	; This is the common entry point for many of the RISC OS exception
	; handlers.  On entry, assume that all registers are corrupted.
|__h_cback_common|
	[ __UNIXLIB_FEATURE_PTHREADS > 0
	LDR	a3, =|__ul_global|
	LDR	a1, [a3, #GBL_PTH_WORKSEMAPHORE]
	ADD	a1, a1, #1
	STR	a1, [a3, #GBL_PTH_WORKSEMAPHORE]
	]

	BL	|__setup_signalhandler_stack|

	; Back to USR mode now we have a stack, but with IRQs disabled.
	CHGMODE	a1, USR_Mode+IFlag

	; The USR mode registers r0-r15 and CPSR are extracted from the
	; callback register block while IRQs are disabled. The registers
	; are then saved on the USR mode signal handler stack.
	ADR	a1, |__cbreg| + 8*4	; Copy R8-R15, R0-R7
	LDMIA	a1, {a2, a3, a4, v1, v2, v3, v4, v5}
	STMFD	sp!, {a2, a3, a4, v1, v2, v3, v4, v5}
	LDMDB	a1, {a2, a3, a4, v1, v2, v3, v4, v5}
	STMFD	sp!, {a2, a3, a4, v1, v2, v3, v4, v5}
	LDR	a2, [a1, #8*4]		; Copy CPSR
	STMFD	sp!, {a2}

	; Check for an escape condition
	LDR	a1, |__cbflg|		; check __cbflg bit 0
	TST	a1, #1
	BIC	a1, a1, #2		; clear the internet event flag bit
	STR	a1, |__cbflg|
	; There was an escape condition.  Clear it.
	MOVNE	a1, #&7e
	SWINE	XOS_Byte		; This calls our escape handler

	; Create an APCS-32 compilant signal stack frame
	ADR	a4, |__h_cback| + 4*3	; point at handler name for backtrace
	LDR	a3, [sp, #14*4 + 4]	; saved USR lr
	LDR	a2, [sp, #13*4 + 4]	; saved USR sp
	LDR	a1, [sp, #11*4 + 4]	; saved USR fp
	STMFD	sp!, {a1, a2, a3, a4}	; create signal frame
	ADD	fp, sp, #4*3

	LDR	v1, =|__ul_callbackfp|	; Save callback FP backtrace
	STR	a1, [v1]

	MOV	a1, #0
	LDR	a2, |__cba1|
	; Raise the signal in USR mode with IRQs enabled
	SWI	XOS_IntOn
	BL	|__unixlib_raise_signal|
	; Disable IRQs again while updating semaphores
	SWI	XOS_IntOff

	LDR	a3, =|__ul_global|
	[ __UNIXLIB_FEATURE_PTHREADS > 0
	LDR	a1, [a3, #GBL_PTH_WORKSEMAPHORE]
	SUB	a1, a1, #1
	STR	a1, [a3, #GBL_PTH_WORKSEMAPHORE]
	]

	LDR	a1, [a3, #GBL_EXECUTING_SIGNALHANDLER]
	SUB	a1, a1, #1
	STR	a1, [a3, #GBL_EXECUTING_SIGNALHANDLER]

	ADD	a1, sp, #16	; Skip signal frame
	ADD	sp, sp, #16+17*4
	SWI	XOS_EnterOS	; We need to be in SVC mode so reenbling IRQs
				; is atomic with returning to USR mode,
				; otherwise USR sp could be overwitten by
				; another callback
	MOV	lr, a1
	LDR	a1, [lr], #4	; Get user PSR
	MSR	SPSR_cxsf, a1	; Put it into SPSR_SVC/IRQ
	LDMIA	lr, {a1, a2, a3, a4, v1, v2, v3, v4, v5, v6, sl, fp, ip, sp, lr}^
	MOV	a1, a1
	LDR	lr, [lr, #15*4]	; Load the old PC value
	MOVS	pc, lr		; Return (Valid for 26 and 32bit modes)


	; Setup a USR mode stack for the signal handler and pthread
	; context switcher. Should be called in USR, SVC or IRQ mode.
	; We cope with recursive signals by only setting up the stack
	; if we're not already in a signal handler
	EXPORT	|__setup_signalhandler_stack|
|__setup_signalhandler_stack|
	LDR	a3, =|__ul_global|
	LDR	a1, [a3, #GBL_EXECUTING_SIGNALHANDLER]
	TEQ	a1, #0
	ADD	a1, a1, #1
	STR	a1, [a3, #GBL_EXECUTING_SIGNALHANDLER]

	;FIXME: we need a sanity check here (based on __UNIXLIB_EXTREMELY_PARANOID ?):
	;if the signalhandler is already executing, verify that
	;sl < sp, if not, panic and jump to e.g. __exit.
	LDR	sl, [a3, #GBL_SIGNALHANDLER_SL]
	MOV	fp, #0
	MOVNE	pc, lr

	ADD	a1, a3, #GBL_SIGNALHANDLER_SP
	TEQ	pc, pc
	MOVNE	a2, pc
	MRSEQ	a2, CPSR
	TST	a2, #Mode_Bits

	LDMNEIA	a1, {sp}^	; Not USR mode, set USR sp
	LDREQ	sp, [a1, #0]	; USR mode
	MOV	pc, lr

	; User registers are preserved in here for the callback execution.
	; User registers = R0-R15 and CPSR in 32-bit mode
	;		 = R0-R15 only in 26-bit mode (the 17th word equals
	;		   the pc value)
	EXPORT	|__cbreg|
|__cbreg|
	%	68	; 4 * 17

	; bit 0 Escape condition flag
	; bit 1 Internet event flag
|__cbflg|
	DCD	0

	; Signal number to use for __unixlib_raise_signal().
|__cba1|
	DCD	0

; Exit handler
; Called in USR mode
	EXPORT	|__h_exit|
	NAME	__h_exit
|__h_exit|
	MOV	a1, #EXIT_FAILURE
	B	|exit|

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; The various timer handlers. We know that these can only be invoked
; from an application that is not a taskwindow application, since the
; code that calls OS_CallAfter checks that we are not in a taskwindow.
; We also have a check for when we are a WIMP application.

; Interval timer handler for ITIMER_REAL
; On entry R12 = pointer to interval timerval
; Called in SVC mode.
; Preserve all registers

	; Interval timer handler for ITIMER_REAL
	EXPORT	|__h_sigalrm|
	NAME	__h_sigalrm
|__h_sigalrm|
	STMFD	sp!, {a1, a2, a3, lr}
	; Raise the SIGALRM signal
	MOV	a1, #SIGALRM
	BL	|__raise|
	; Have we previously setup a CallEvery handler
	LDR	a1, |__h_sigalrm_sema|
	TEQ	a1, #1
	LDMEQFD	sp!, {a1, a2, a3, pc}
	; r12->it_interval = 0 secs and 0 usecs then exit
	LDMIA	ip, {a1, a2}
	TEQ	a1, #0
	TEQEQ	a2, #0
	LDMEQFD	sp!, {a1, a2, a3, pc}
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
	LDMFD	sp!, {a1, a2, a3, pc}

; Called in SVC mode with IRQs disabled.
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
; Called in SVC mode.
; Preserve all registers
	EXPORT	|__h_sigvtalrm|
	NAME	__h_sigvtalrm
|__h_sigvtalrm|
	STMFD	sp!, {a1, a2, a3, lr}
	MOV	a1, #SIGVTALRM	;  No access to banked registers
	BL	|__raise|
	LDR	a1, |__h_sigvtalrm_sema|
	TEQ	a1, #1
	LDMEQFD	sp!, {a1, a2, a3, pc}
	LDMIA	ip, {a1, a2}
	TEQ	a1, #0
	TEQEQ	a2, #0
	LDMEQFD	sp!, {a1, a2, a3, pc}
	MOV	a3, #100
	MLA	a1, a3, a1, a2
	ADD	a1, a1, #1
	ADR	a2, |__h_sigvtalrm_init|
	MOV	a3, ip
	SWI	XOS_CallEvery
	MOV	a1, #1
	STR	a1, |__h_sigvtalrm_sema|
	LDMFD	sp!, {a1, a2, a3, pc}

; Called in SVC mode with IRQs disabled.
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
; Called in SVC mode.
; Preserve all registers
	EXPORT	|__h_sigprof|
	NAME	__h_sigprof
|__h_sigprof|
	STMFD	sp!, {a1, a2, a3, lr}
	MOV	a1, #SIGPROF	; No access to banked registers
	BL	|__raise|
	LDR	a1, |__h_sigprof_sema|
	TEQ	a1, #1
	LDMEQFD	sp!, {a1, a2, a3, pc}
	LDMIA	ip, {a1, a2}
	TEQ	a1, #0
	TEQEQ	a2, #0
	LDMEQFD	sp!, {a1, a2, a3, pc}
	MOV	a3, #100
	MLA	a1, a3, a1, a2
	ADD	a1, a1, #1
	ADR	a2, |__h_sigprof_init|
	MOV	a3, ip
	SWI	XOS_CallEvery
	MOV	a1, #1
	STR	a1, |__h_sigprof_sema|
	LDMFD	sp!, {a1, a2, a3, pc}

; Called in SVC mode with IRQs disabled.
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

	AREA	|C$$data|, DATA

; The fp at the time when __h_cback_common is executed.  This value
; will be used during write_backtrace() to know when we can dump the CPSR
; and USR registers at fp + 4.
	EXPORT  |__ul_callbackfp|
|__ul_callbackfp|
	DCD	0

;-----------------------------------------------------------------------
; UnixLib's error buffer.
; If threads are in use then it is only temporary, as the error handler
; copies it into the thread's error buffer.
;
; According to PRM 1-289 the error handler must provide an error buffer
; of size 256 bytes, the address of which should be set along with the
; handler address.  This is wrong, it actually requires a 260 byte
; buffer, for the error PC and the normal 256 byte RISC OS error
; block.
	EXPORT  |__ul_errbuf|
|__ul_errbuf|
	%	4	; PC when error occurred
|__ul_errbuf_errblock|
	%	4	; Error number provided with the error
	%	252	; Error string, zero terminated
|__ul_errbuf__size|	EQU	{PC} - |__ul_errbuf_errblock|
|__ul_errbuf_valid|
	DCD	0	; Valid flag for errbuf

	EXPORT |__ul_fp_registers|
|__ul_fp_registers|
	%	68	; (4 + 8*8)  FPSR and 8 double-precision registers

	END
