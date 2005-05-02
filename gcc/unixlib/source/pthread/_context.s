;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/_context.s,v $
; $Date: 2005/04/23 10:44:38 $
; $Revision: 1.15 $
; $State: Exp $
; $Author: nick $
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


	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__pthread_context_switch|
	IMPORT	|__setup_signalhandler_stack|
	IMPORT	|__executing_signalhandler|
	IMPORT	|__pthread_fatal_error|
	IMPORT	|__cbreg|
	IMPORT	|__ul_global|
	IMPORT	|__pthread_running_thread|

	EXPORT	|__pthread_start_ticker|
	EXPORT	|__pthread_stop_ticker|
	EXPORT	|__pthread_init_save_area|

;
; Start a ticker which will set the callback flag every clock tick
;
	NAME	__pthread_start_ticker
|__pthread_start_ticker|
	STMFD	sp!, {v1-v2, lr}
	LDR	a2, =|__ul_global|

	; Don't start until the thread system has been setup
	LDR	a1, [a2, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
	LDMEQFD	sp!, {v1-v2, pc}

	; Don't start if there's only one thread running
	LDR	a1, [a2, #GBL_PTH_NUM_RUNNING_THREADS]
	CMP	a1, #1
	LDMLEFD	sp!, {v1-v2, pc}

	; Don't start if the ticker is already running
	LDR	a1, =|ticker_started|
	LDR	a1, [a1]
	TEQ	a1, #0
	LDMNEFD	sp!, {v1-v2, pc}

	; Are we running as WIMP task ?
	; If we are then we need a filter switching off our ticker when we're
	; swapped out.
	LDR	a1, [a2, #GBL_TASKWINDOW]
	TEQ	a1, #0
	BEQ	|start_ticker_core|

	; Temporarily disable the filter code as it is not stable.
	LDMFD	sp!, {v1-v2, pc}

	; Install the filter routines (a4 = WIMP taskhandle) :
	LDR	v2, =|filter_installed|
	LDR	a1, [v2]
	STR	v2, [v2]
	TEQ	a1, #0
	BNE	|start_ticker_core|

	ADR	a1, |filter_name|
	ADR	a2, |stop_call_every|
	MOV	a3, #0
	SWI	XFilter_RegisterPreFilter
	ADRVC	a2, |start_call_every|
	MVNVC	v1, #0
	SWIVC	XFilter_RegisterPostFilter
	ADDVS	a1, a1, #4
	BVS	|__pthread_fatal_error|

start_ticker_core
	BL	|start_call_every|
	LDMFD	sp!, {v1-v2, pc}

filter_name
	DCB	"pthread filter", 0
	ALIGN


	; Start the RISC OS CallEvery ticker
	NAME	start_call_every
|start_call_every|
	STMFD	sp!, {lr}
	LDR	a4, =|ticker_started|
	LDR	a1, [a4]
	TEQ	a1, #0
	LDMNEFD	sp!, {pc}

	MOV	a1, #1
	ADR	a2, |pthread_call_every|
	MOV	a3, #0
	SWI	XOS_CallEvery

	MOVVC	a1, #1
	STRVC	a1, [a4]
	LDMVCFD	sp!, {pc}
	ADD	a1, a1, #4
	B	|__pthread_fatal_error|


	; Stop the RISC OS CallEvery ticker
	NAME	stop_call_every
|stop_call_every|
	STMFD	sp!, {lr}
	; Don't stop it if it isn't enabled yet
	LDR	a3, =|ticker_started|
	LDR	a2, [a3]
	TEQ	a2, #1
	LDMNEFD	sp!, {pc}
	MOV	a2, #0
	STR	a2, [a3]

	ADR	a1, |pthread_call_every|
	MOV	a2, #0
	SWI	XOS_RemoveTickerEvent
	LDMFD	sp!, {pc}


;
; Stop the ticker running
;
	NAME	__pthread_stop_ticker
|__pthread_stop_ticker|
	STMFD	sp!, {v1, v2, lr}
	; Don't bother if thread system is not running
	LDR	a2, =|__ul_global|
	LDR	a1, [a2, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
	LDMEQFD	sp!, {v1, v2, pc}

	; Need to remove the filters ?
	LDR	a1, [a2, #GBL_TASKHANDLE]
	TEQ	a1, #0
	BEQ	|stop_ticker_core|

	; Remove the filter routines (a4 = WIMP taskhandle) :
	; Ignore any errors.
	LDR	v2, =|filter_installed|
	LDR	a1, [v2]
	TEQ	a1, #0
	BEQ	|stop_ticker_core|

	ADR	a1, |filter_name|
	ADR	a2, |stop_call_every|
	MOV	a3, #0
	SWI	XFilter_DeRegisterPreFilter
	ADR	a1, |filter_name|	; a1 can be corrupted, so re-init
	ADR	a2, |start_call_every|
	MVN	v1, #0
	SWI	XFilter_DeRegisterPostFilter
	STR	a3, [v2]

stop_ticker_core
	BL	|stop_call_every|
	LDMFD	sp!, {v1, v2, pc}


; The ticker calls this every clock tick, note that it is every *two*
; centiseconds.
; Called in SVC mode with IRQs disabled, all registers must be preserved.
	NAME	pthread_call_every
|pthread_call_every|
	STMFD	sp!, {a1-a4, lr}

	; First check that our upcall handler is paged in. If it is not
	; then it is likely that the taskwindow module is in the process
	; of paging us out/in so setting a callback would be a bad idea.
	; As the upcall handler is within SUL then we know nothing else
	; can have a handler at the same address except other UnixLib
	; programs. We can distinguish between these with the SUL key.
	MOV	a1, #16
	MOV	a2, #0
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment

	LDR	a4, =|__ul_global|
	LDMVSFD	sp!, {a1-a4, pc}

	; If it is not a SUL upcall handler, don't set callback
	LDR	a1, [a4, #GBL_UPCALL_HANDLER_ADDR]
	TEQ	a1, a2
	; If it is not our SUL key, don't set callback
	LDREQ	a1, [a4, #GBL_UPCALL_HANDLER_R12]
	TEQEQ	a1, a3
	; If we are in the middle of a context-switch callback,
	; don't set the callback.
	LDREQ	a1, [a4, #GBL_PTH_CALLBACK_SEMAPHORE]
	TEQEQ	a1, #0
	; If we are in a critical section, don't set the callback
	LDREQ	a1, [a4, #GBL_PTH_WORKSEMAPHORE]
	TEQEQ	a1, #0
	SWIEQ	XOS_SetCallBack
	LDMFD	sp!, {a1-a4, pc}

; This is called from _signal.s::__h_cback and pthread_yield.
; Entered in SVC or IRQ mode with IRQs disabled.

; The operating system will eventually be returning to USR mode.
	EXPORT	|__pthread_callback|

	NAME	__pthread_callback
|__pthread_callback|
	LDR	a3, =|__ul_global|
	; If we are in a critical region, do not switch threads and
	; exit quicky.
	LDR	a1, [a3, #GBL_PTH_WORKSEMAPHORE]
	TEQ	a1, #0
	; If we are already in the middle of a context switch callback,
	; then quickly exit.
	LDREQ	a1, [a3, #GBL_PTH_CALLBACK_SEMAPHORE]
	TEQEQ	a1, #0
	BNE	|skip_contextswitch|

	; Everything checks out, so from now on we're going to change
	; contexts.

	; Set __pthread_callback_semaphore to ensure that another
	; context interrupt does not interfere with us during this critical
	; time.
	MOV	a1, #1
	STR	a1, [a3, #GBL_PTH_CALLBACK_SEMAPHORE]

	; Setup a stack for the context switcher
	BL	|__setup_signalhandler_stack|

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
	; Copy SPSR
	LDR	a3, [a2]
	STR	a3, [a1], #4

	; We have to copy the integer regs before switching IRQs
	; back on, so they don't get overwritten by another callback,
	; but the floating point instructions should only be called
	; from user mode
	CHGMODE	a3, USR_Mode

        [ {SOFTFLOAT}={FALSE}
	; Save floating point regs
	SFM	f0, 4, [a1], #48
	SFM	f4, 4, [a1], #48
	RFS	a2	; Read floating status
	STR	a2, [a1]
	]

	; Call the scheduler to switch to another thread
	BL	|__pthread_context_switch|

	; Now reload the registers from the new thread's save area
	LDR	a1, =|__pthread_running_thread|
	LDR	a1, [a1]
	LDR	a2, [a1, #__PTHREAD_CONTEXT_OFFSET]	; __pthread_running_thread->saved_context

        [ {SOFTFLOAT}={FALSE}
	ADD	a2, a2, #17*4
	LFM	f0, 4, [a2], #48
	LFM	f4, 4, [a2], #48
	LDR	a1, [a2]
	WFS	a1	; Write floating status
	]

	SWI	XOS_EnterOS	; Back to supervisor mode

	CHGMODE	a2, SVC_Mode+IFlag	; Force SVC mode, IRQs off

	LDR	a2, =|__ul_global|

	; Indicate that this context switch was successful
	MOV	a1, #0
	STR	a1, [a2, #GBL_PTH_CALLBACK_MISSED]

	; Signify that we are no longer in the middle of a context switch.
	STR	a1, [a2, #GBL_PTH_CALLBACK_SEMAPHORE]
	
	; Indicate that we are no longer in a signal handler, since we
	; will be returning direct to USR mode and the application itself.
	LDR	a1, [a2, #GBL_EXECUTING_SIGNALHANDLER]
	SUB	a1, a1, #1
	STR	a1, [a2, #GBL_EXECUTING_SIGNALHANDLER]

	; Point to the register save area for the new thread.
	LDR	r14, =|__pthread_running_thread|
	LDR	r14, [r14]
	LDR	r14, [r14, #__PTHREAD_CONTEXT_OFFSET]	; __pthread_running_thread->saved_context

	; Restore thread's registers
	LDR	a1, [r14, #16*4]	; Get user PSR
	MSR	SPSR_cxsf, a1		; Put it into SPSR_SVC/IRQ (NOP on ARM2/3, shouldn't have any effect in 26bit mode)
	LDMIA	r14, {r0-r14}^		; Load USR mode regs
	MOV	a1, a1

	LDR	r14, [r14, #15*4]	; Load the old PC value
	MOVS	pc, lr			; Return (Valid for 26 and 32bit modes)


	; Called because we are fast exiting from the context switcher
	; because some other context switching operation is already going on,
	; or because the program indicates that we are in a critical
	; section.

	; On entry, a2 == __ul_global
|skip_contextswitch|
	; Indicate that this context switch did not occur
	MOV	a1, #1
	STR	a1, [a3, #GBL_PTH_CALLBACK_MISSED]

	; Exiting from the CallBack handler requires us to reload all
	; registers from the register save area.
	LDR	r14, =|__cbreg|
	LDR	a1, [r14, #16*4]	; Get user PSR
	MSR	SPSR_cxsf, a1		; Put it into SPSR_SVC/IRQ (NOP on ARM2/3, shouldn't have any effect in 26bit mode)
	LDMIA	r14, {r0-r14}^		; Load USR mode regs
	MOV	a1, a1

	LDR	r14, [r14, #15*4]	; Load the old PC value
	MOVS	pc, lr			; Return (Valid for 26 and 32bit modes)

; entry:
;   R0 = save area
	NAME	__pthread_init_save_area
|__pthread_init_save_area|
        [ {SOFTFLOAT}={FALSE}
	ADD	a2, a1, #17*4
	SFM	f0, 4, [a2], #48
	SFM	f4, 4, [a2], #48
	RFS	a1	; Read floating status
	STR	a1, [a2], #12
	]
	MOV	pc, lr


	AREA	|C$data|, DATA

	; Have we registered the CallEvery ticker ?
|ticker_started|
	DCD	0

	; Have we installed Filter ?
|filter_installed|
	DCD	0

	END
