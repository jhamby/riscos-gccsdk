;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/_context.s,v $
; $Date: 2003/04/28 12:07:02 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: alex $
;
;----------------------------------------------------------------------------

; Low level context switching code
; Written by Martin Piper and Alex Waugh

; For a single tasking program (not running in a taskwindow), the context
; switcher works as follows:
; OS_CallEvery is used to cause an interrupt every centisecond. This interrupt
; could occur at any time, so we might be in the middle of a SWI call.
; Therefore a callback is set, so when the SWI returns, or when the callevery
; routine returns if a SWI call was not taking place, the callback handler is
; invoked. (A transient callback cannot be used as this might be called when
; RISC OS is idleing, eg. in an OS_ReadC call.)
; The callback handler saves all integer and floating point register values
; in the thread's context save area, and then calls the scheduler
; (__pthread_context_switch) which decides which thread to run next. The
; register values (including the saved program counter) from this new thread
; are then loaded from the thread's context save area into the appropriate
; registers, thus restoring that thread to where is was before it was switched
; out, and returning from the callback at the same time.
; If a routine does not want to be switched out, the it can call
; __pthread_disable_ints or __pthread_protect_unsafe which will alter
; __pthread_work_semaphore. The callevery interrupt will still occur, but it
; will take note of the state of the semaphore, and not set a callback


	GET	clib/unixlib/asm_dec.s


	AREA |C$$code|, CODE, READONLY

	IMPORT	|__pthread_context_switch|
	IMPORT	|__pthread_worksemaphore|
	IMPORT	|__setup_signalhandler_stack|
	IMPORT	|__executing_signalhandler|
	IMPORT	|__taskwindow|
	IMPORT	|__pthread_fatal_error|
	IMPORT	|__pthread_running_thread|
	IMPORT	|__pthread_num_running_threads|
	IMPORT	|__cbreg|

	EXPORT	|__pthread_start_ticker|
	EXPORT	|__pthread_stop_ticker|
	EXPORT	|__pthread_init_save_area|
	EXPORT	|__pthread_system_running|
	EXPORT	|__pthread_callback_semaphore|
	EXPORT	|__pthread_callback_missed|

;
; Start a ticker which will set the callback flag every clock tick
;
|__pthread_start_ticker|
	STMFD	sp!, {lr}
	; Don't start until the thread system has been setup
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #1
	stackreturn	NE, "pc"
	; Don't start if there's only one thread running
	LDR	a1, =|__pthread_num_running_threads|
	LDR	a1, [a1]
	CMP	a1, #1
	stackreturn	LE, "pc"
	; Don't start if the ticker is already running
	LDR	a1, =|ticker_started|
	LDR	a1, [a1]
	CMP	a1, #0
	stackreturn	NE, "pc"
	; Are we running in a task window?
	; If we are it's bad news for the ticker interupt so don't start
	LDR	a1, =|__taskwindow|
	LDR	a1, [a1]
	CMP	a1, #0
	stackreturn	NE, "pc"

	; Set the ticker
	MOV	a1, #1
	ADR	a2, call_every
	MOV	a3, #0
	SWI	XOS_CallEvery
	ADDVS	a1, a1, #4
	BVS	|__pthread_fatal_error|
	MOV	a1, #1
	LDR	a2, =|ticker_started|
	STR	a1, [a2]
	stackreturn	AL, "pc"

;
; Stop the ticker running
;
|__pthread_stop_ticker|
	STMFD	sp!, {lr}
	; Don't bother if thread system is not running
	LDR	a1, =|__pthread_system_running|
	LDR	a1, [a1]
	CMP	a1, #0
	stackreturn	EQ, "pc"
	; Don't stop it if it isn't enabled yet
	LDR	a3, =|ticker_started|
	LDR	a2, [a3]
	CMP	a2, #1
	stackreturn	NE, "pc"
	MOV	a2, #0
	STR	a2, [a3]

	ADR	a1, |call_every|
	MOV	a2, #0
	SWI	XOS_RemoveTickerEvent
	stackreturn	AL, "pc"


; The ticker calls this every clock tick
; Called in SVC mode with IRQs disabled
|call_every|
	STMFD	sp!, {a1, lr}
	LDR	a1, =|__pthread_callback_semaphore|
	LDR	a1, [a1]
	CMP	a1, #0	; Check we are not already in the middle of a context switch callback
	LDMNEFD	sp!, {a1, pc}

	LDR	a1, =|__pthread_worksemaphore|
	LDR	a1, [a1]
	CMP	a1, #0
	LDMNEFD	sp!, {a1, pc}	; In a critical region, so don't switch threads

	SWI	XOS_SetCallBack	; Set the OS callback flag. Not much we can do if this fails
	LDMFD	sp!, {a1, pc}


; This is called from __h_cback in signal/_signal.s when the OS is returning to USR mode
; Also called directly from pthread_yield
; Called in SVC or IRQ mode with IRQs disabled
	EXPORT	|__pthread_callback|
|__pthread_callback|
	; Setup a stack for the context switcher
	BL	|__setup_signalhandler_stack|

	LDR	a1, =|__pthread_callback_semaphore|
	LDR	a2, [a1]
	CMP	a2, #0
	BNE	|skip_contextswitch|	; Check we are not already in the middle of a context switch callback
	MOV	a2, #1
	STR	a2, [a1]	; Set the semaphore

	LDR	a1, =|__pthread_worksemaphore|
	LDR	a1, [a1]
	CMP	a1, #0
	BNE	|skip_contextswitch|	; In a critical region, so don't switch threads

	; Save regs to thread's save area
	LDR	a1, =|__pthread_running_thread|
	LDR	a1, [a1]
	LDR	a1, [a1, #__PTHREAD_CONTEXT_OFFSET]	; __pthread_running_thread->saved_context

	; Copy integer regs
	LDR	a2, =|__cbreg|
	LDMIA	a2!, {a3, a4, v1, v2, v3, v4, v5, v6}
	STMIA	a1!, {a3, a4, v1, v2, v3, v4, v5, v6}
	LDMIA	a2!, {a3, a4, v1, v2, v3, v4, v5, v6}
	STMIA	a1!, {a3, a4, v1, v2, v3, v4, v5, v6}
	[ {CONFIG} = 32
	; Copy SPSR
	LDR	a3, [a2]
	STR	a3, [a1], #4
	|
	ADD	a1, a1, #4
	]

	; We have to copy the integer regs before switching IRQs
	; back on, so they don't get overwritten by another callback,
	; but the floating point instructions should only be called
	; from user mode
	CHGMODE	a3, USR_Mode

	; Save floating point regs
	SFM	f0, 4, [a1], #48
	SFM	f4, 4, [a1], #48
	RFS	a2	; Read floating status
	STR	a2, [a1]

	BL	|__pthread_context_switch|	; Call the scheduler to switch to another thread

	; Now reload the registers from the new thread's save area
	LDR	a1, =|__pthread_running_thread|
	LDR	a1, [a1]
	LDR	a2, [a1, #__PTHREAD_CONTEXT_OFFSET]	; __pthread_running_thread->saved_context

	ADD	a2, a2, #17*4
	LFM	f0, 4, [a2], #48
	LFM	f4, 4, [a2], #48
	LDR	a1, [a2]
	WFS	a1	; Write floating status

	SWI	XOS_EnterOS	; Back to supervisor mode

	LDR	a3, =|__pthread_callback_missed|	; Indicate that this context switch was successful
	MOV	a1, #0
	STR	a1, [a3]

	LDR	a1, =|__pthread_running_thread|
	LDR	a1, [a1]
	LDR	a1, [a1, #__PTHREAD_CONTEXT_OFFSET]	; __pthread_running_thread->saved_context

|callback_return|	; a1 = address of regs to load
	CHGMODE	a2, SVC_Mode+IFlag	; Force SVC mode, IRQs off

	LDR	a2, =|__executing_signalhandler|
	LDR	a3, [a2]
	SUB	a3, a3, #1
	STR	a3, [a2]

	LDR	a2, =|__pthread_callback_semaphore|
	MOV	a3, #0
	STR	a3, [a2]

	MOV	r14, a1
	[ {CONFIG} = 32
	LDR	a1, [r14, #16*4]	; Get user PSR
	MSR	SPSR_cxsf, a1		; Put it into SPSR_SVC/IRQ (NOP on ARM2/3, shouldn't have any effect in 26bit mode)
	]
	LDMIA	r14, {r0-r14}^		; Load USR mode regs
	MOV	a1, a1

	LDR	r14, [r14, #15*4]	; Load the old PC value
	MOVS	pc, lr			; Return (Valid for 26 and 32bit modes)

|skip_contextswitch|
	LDR	a3, =|__pthread_callback_missed|	; Indicate that this context switch did not occur
	MOV	a1, #1
	STR	a1, [a3]

	LDR	a1, =|__cbreg|
	B	|callback_return|

; entry:
;   R0 = save area
|__pthread_init_save_area|
	ADD	a2, a1, #17*4
	SFM	f0, 4, [a2], #48
	SFM	f4, 4, [a2], #48
	RFS	a1	; Read floating status
	STR	a1, [a2], #12
	return	AL, pc, lr


	AREA |C$data|, DATA

|ticker_started| ; Have we registered the callevery ticker
	DCD	0

|__pthread_callback_semaphore|	; Prevent a callback being set whilst servicing another callback
	DCD	0

|__pthread_system_running|	; Global initialisation flag
	DCD	0

|__pthread_callback_missed|	; Non zero if a callback occured when context switching was temporarily disabled
	DCD	0

 END
