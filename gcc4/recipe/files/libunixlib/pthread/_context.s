@ Low level context switching code
@ Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008 UnixLib Developers
@ Written by Martin Piper and Alex Waugh

@ The context switcher works as follows:
@
@ OS_CallEvery is used to cause an interrupt every centisecond. This interrupt
@ could occur at any time, so we might be in the middle of a SWI call.
@ Therefore a callback is set, so when the SWI returns, or when the callevery
@ routine returns if a SWI call was not taking place, the callback handler is
@ invoked. (A transient callback cannot be used as this might be called when
@ RISC OS is idleing, eg. in an OS_ReadC call.)
@
@ The callback handler saves all integer and floating point register values
@ in the thread's context save area, and then calls the scheduler
@ __pthread_context_switch which decides which thread to run next. The
@ register values (including the saved program counter) from this new thread
@ are then loaded from the thread's context save area into the appropriate
@ registers, thus restoring that thread to where is was before it was switched
@ out, and returning from the callback at the same time.
@
@ If a routine does not want to be switched out, the it can call
@ __pthread_disable_ints or __pthread_protect_unsafe which will alter
@ __pthread_work_semaphore. The callevery interrupt will still occur, but it
@ will take note of the state of the semaphore, and not set a callback
@
@ Multitasking programs use the Filter module (Filter_RegisterPreFilter and
@ Filter_RegisterPostFilter) to enable/disable OS_CallEvery.

#include "internal/asm_dec.s"

	.text

@
@ Start a ticker which will set the callback flag every clock tick
@
	.global	__pthread_start_ticker
	NAME	__pthread_start_ticker
__pthread_start_ticker:
 PICNE "STMFD	sp!, {v1-v2, lr}"
 PICEQ "STMFD	sp!, {v1-v2, v4, lr}"

 PICEQ "LDR	v4, .L0+16"
.LPIC0:
 PICEQ "ADD	v4, pc, v4"		@ v4 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	v4, {v4, v5}"		@ v4 = Object index, v5 = GOT array location
 PICEQ "LDR	v5, [v5, #0]"		@ v5 = GOT array
 PICEQ "LDR	v4, [v5, v4, LSL#4]"	@ v4 = GOT (private)

	LDR	a3, .L0			@=__ul_global
 PICEQ "LDR	a3, [v4, a3]"

	@ Don't start until the thread system has been setup
	LDR	a1, [a3, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
 PICEQ "LDMEQFD	sp!, {v1-v2, v4, pc}"
 PICNE "LDMEQFD	sp!, {v1-v2, pc}"

	@ Don't start if there's only one thread running
	LDR	a1, [a3, #GBL_PTH_NUM_RUNNING_THREADS]
	CMP	a1, #1
 PICEQ "LDMLEFD	sp!, {v1-v2, v4, pc}"
 PICNE "LDMLEFD	sp!, {v1-v2, pc}"

	@ Are we running as WIMP task ?
	@ If we are then we need a filter switching off our ticker when we're
	@ swapped out and switching back on after we're swapped in.
	LDR	a4, [a3, #GBL_TASKHANDLE]
	TEQ	a4, #0
	BNE	start_ticker_install_filters

	@ Application may have called Wimp_Initialise since we last checked
	@ Code similar to __get_taskhandle.
	MOV	a1, #3				@ In desktop?
	SWI	XWimp_ReadSysInfo
	MOVVS	a1, #0
	TEQ	a1, #0
	MOVNE	a1, #5				@ Read taskhandle, iff in desktop
	SWINE	XWimp_ReadSysInfo
	MOVVS	a1, #0

	MOVS	a4, a1
	BEQ	start_ticker_test_running
 
	STR	a4, [a3, #GBL_TASKHANDLE]	@ __ul_global.taskhandle
 
	@ a4 = current taskhandle
start_ticker_install_filters:
	@ Install the filter routines (a4 = WIMP taskhandle) :
	LDR	v2, .L0+8		@=filter_installed
 PICEQ "LDR	v2, [v4, v2]"
	LDR	a1, [v2]
	STR	v2, [v2]
	TEQ	a1, #0
	BNE	start_ticker_test_running

	MOV	a2, a4			@ save taskhandle in a2

	MOV	a1, #6			@ allocate some RMA for filter name
	MOV	a4, #filter_name_end - filter_name
	SWI	XOS_Module
	ADDVS	a1, a1, #4
	BVS	__pthread_fatal_error

	LDR	v2, .L0+12		@=filter_name_rma_ptr
 PICEQ "LDR	v2, [v4, v2]"
	STR	a3, [v2]

	MOV	a4, a2			@ restore taskhandle

 	ADR	a1, filter_name
start_ticker_copy_filter_name:		@ copy filter name to RMA
	LDRB	a2, [a1], #1
	STRB	a2, [a3], #1
	TEQ	a2, #0
	BNE	start_ticker_copy_filter_name

	LDR	a1, [v2]		@ get filter name RMA pointer
	ADR	a2, stop_call_every
	@ In the shared library, pass the GOT pointer to the filter
	@ code.
 PICEQ "MOV	a3, v4"
 PICNE "MOV	a3, #0"
	@ a4 is still equal to taskhandle
	SWI	XFilter_RegisterPreFilter
	ADRVC	a2, start_call_every
	@ a3 is still v4 (PIC) or 0 (no PIC)
	@ a4 is still equal to taskhandle
	MOVVC	v1, #0
	SWIVC	XFilter_RegisterPostFilter
	ADDVS	a1, a1, #4
	BVS	__pthread_fatal_error

start_ticker_test_running:
	@ start_call_every may be called as a filter routine, in which case
	@ it expects the GOT pointer in r12. Make sure that's the case when
	@ called directly.
 PICEQ "MOV	r12, v4"
	BL	start_call_every
 PICEQ "LDMFD	sp!, {v1-v2, v4, pc}"
 PICNE "LDMFD	sp!, {v1-v2, pc}"
.L0:
	WORD	__ul_global
	WORD	ticker_started
	WORD	filter_installed
	WORD	filter_name_rma_ptr
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"

filter_name:
	.asciz	"UnixLib pthread"
filter_name_end:
	.align
	DECLARE_FUNCTION __pthread_start_ticker

	@ Start the RISC OS CallEvery ticker
	@ This may be called as a Wimp post filter, so preserve all registers,
	@ all processor flags and don't return any errors.
	@ R12 is our private word and contains the GOT pointer.
	NAME	start_call_every
start_call_every:
	STMFD	sp!, {a1-v1, lr}
	MRS	v1, CPSR
	LDR	a4, .L0+4	@=ticker_started
 PICEQ "LDR	a4, [r12, a4]"
	LDR	a1, [a4]
	TEQ	a1, #0
	BNE	start_call_every_end

	MOV	a1, #1
	ADR	a2, pthread_call_every
 PICEQ "MOV	a3, r12"
 PICNE "MOV	a3, #0"
	SWI	XOS_CallEvery

	MOVVC	a1, #1
	STRVC	a1, [a4]
start_call_every_end:
	MSR	CPSR_f, v1
	LDMFD	sp!, {a1-v1, pc}
	DECLARE_FUNCTION start_call_every

	@ Stop the RISC OS CallEvery ticker
	@ This may be called as a Wimp pre filter, so preserve all registers,
	@ all processor flags and don't return any errors.
	@ R12 is our private word and contains the GOT pointer.
	NAME	stop_call_every
stop_call_every:
	STMFD	sp!, {a1-a3, lr}
	MRS	a3, CPSR
	@ Don't stop it if it isn't enabled yet
	LDR	lr, .L0+4	@=ticker_started
 PICEQ "LDR	lr, [r12, lr]"
	LDR	a2, [lr]
	SUBS	a2, a2, #1
	STREQ	a2, [lr]

	ADREQ	a1, pthread_call_every
 PICEQ "MOVEQ	a2, r12"
	@ Non PIC: a2 is still 0
@@ PICNE "MOVEQ	a2, #0"
	SWIEQ	XOS_RemoveTickerEvent
	MSR	CPSR_f, a3
	LDMFD	sp!, {a1-a3, pc}
	DECLARE_FUNCTION stop_call_every

@
@ Stop the ticker running
@
	.global	__pthread_stop_ticker
	NAME	__pthread_stop_ticker
__pthread_stop_ticker:
 PICNE "STMFD	sp!, {v1-v3, lr}"
 PICEQ "STMFD	sp!, {v1-v3, v4, lr}"

 PICEQ "LDR	v4, .L1+12"
.LPIC1:
 PICEQ "ADD	v4, pc, v4"		@ v4 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	v4, {v4, v5}"		@ v4 = Object index, v5 = GOT array location
 PICEQ "LDR	v5, [v5, #0]"		@ v5 = GOT array
 PICEQ "LDR	v4, [v5, v4, LSL#4]"	@ v4 = GOT (private)

	@ Don't bother if thread system is not running
	LDR	a2, .L1+0		@=__ul_global
 PICEQ "LDR	a2, [v4, a2]"
	LDR	a1, [a2, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
 PICEQ "LDMEQFD	sp!, {v1-v3, v4, pc}"
 PICNE "LDMEQFD	sp!, {v1-v3, pc}"

	@ Need to remove the filters ?
	LDR	a4, [a2, #GBL_TASKHANDLE]
	TEQ	a4, #0
	BEQ	stop_ticker_core

	@ Remove the filter routines (a4 = WIMP taskhandle) :
	@ Ignore any errors.
	LDR	v2, .L1+4		@=filter_installed
 PICEQ "LDR	v2, [v4, v2]"
	LDR	a1, [v2]
	TEQ	a1, #0
	BEQ	stop_ticker_core

	LDR	v3, .L1+8		@=filter_name_rma_ptr
 PICEQ "LDR	v3, [v4, v3]"

	LDR	a1, [v3]
	ADR	a2, stop_call_every
 PICEQ "MOV	a3, v4"
 PICNE "MOV	a3, #0"
	@ a4 is still equal to taskhandle
	SWI	XFilter_DeRegisterPreFilter
	LDR	a1, [v3]		@ a1 can be corrupted, so re-init
	ADR	a2, start_call_every
	@ a3 is still v4 (PIC) or 0 (non PIC)
	@ a4 is still equal to taskhandle
	MOV	v1, #0
	SWI	XFilter_DeRegisterPostFilter

	STR	a3, [v2]		@ Mark we no longer have filter installed

	@ Release RMA containing filter name
	MOV	a1, #7
	LDR	a3, [v3]
	SWI	XOS_Module
	MOV	a3, #0
	STR	a3, [v3]

stop_ticker_core:
	@ stop_call_every may be called as a filter routine, in which case
	@ it expects the GOT pointer in r12. Make sure that's the case when
	@ called directly.
 PICEQ "MOV	r12, v4"
	BL	stop_call_every
 PICEQ "LDMFD	sp!, {v1-v3, v4, pc}"
 PICNE "LDMFD	sp!, {v1-v3, pc}"
.L1:
	WORD	__ul_global
	WORD	filter_installed
	WORD	filter_name_rma_ptr
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC1+4)"
	DECLARE_FUNCTION __pthread_stop_ticker

@ The ticker calls this every clock tick, note that it is every *two*
@ centiseconds.
@ Called in SVC mode with IRQs disabled, all registers must be preserved.
@ In the shared library, r12 contains the GOT pointer of UnixLib.
	NAME	pthread_call_every
pthread_call_every:
	STMFD	sp!, {a1-a4, lr}

	@ First check that our upcall handler is paged in. If it is not
	@ then it is likely that the taskwindow module is in the process
	@ of paging us out/in so setting a callback would be a bad idea.
	@ As the upcall handler is within SUL then we know nothing else
	@ can have a handler at the same address except other UnixLib
	@ programs. We can distinguish between these with the SUL key.
	MOV	a1, #16
	MOV	a2, #0
	MOV	a3, #0
	MOV	a4, #0
	SWI	XOS_ChangeEnvironment
	LDMVSFD	sp!, {a1-a4, pc}

	LDR	a4, .L1	@=__ul_global
 PICEQ "LDR	a4, [r12, a4]"

	@ If it is not a SUL upcall handler, don't set callback
	LDR	a1, [a4, #GBL_UPCALL_HANDLER_ADDR]
	TEQ	a1, a2
	@ If it is not our SUL key, don't set callback
	LDREQ	a1, [a4, #GBL_UPCALL_HANDLER_R12]
	TEQEQ	a1, a3
	@ If we are in the middle of a context-switch callback,
	@ don't set the callback.
	LDREQ	a1, [a4, #GBL_PTH_CALLBACK_SEMAPHORE]
	TEQEQ	a1, #0
	@ If we are in a critical section, don't set the callback
	LDREQ	a1, [a4, #GBL_PTH_WORKSEMAPHORE]
	TEQEQ	a1, #0
	SWIEQ	XOS_SetCallBack
	LDMFD	sp!, {a1-a4, pc}
	DECLARE_FUNCTION pthread_call_every

@ This is called from _signal.s::__h_cback and pthread_yield.
@ Entered in SVC or IRQ mode with IRQs disabled.

@ The operating system will eventually be returning to USR mode.
	.global	__pthread_callback

	NAME	__pthread_callback
__pthread_callback:
	@ Use ip for temporary PIC register as all other registers
	@ are used further down.
 PICEQ "LDR	v5, .L2+12"
.LPIC2:
 PICEQ "ADD	v5, pc, v5"		@ v5 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	v5, {v5, ip}"		@ v5 = Object index, ip = GOT ptr array location
 PICEQ "LDR	ip, [ip, #0]"		@ ip = GOT ptr array
 PICEQ "LDR	ip, [ip, v5, LSL#4]"	@ ip = GOT ptr
 
	LDR	a3, .L2			@=__ul_global
 PICEQ "LDR	a3, [ip, a3]"
 
	@ If we are in a critical region, do not switch threads and
	@ exit quicky.
	LDR	a1, [a3, #GBL_PTH_WORKSEMAPHORE]
	TEQ	a1, #0
	@ If we are already in the middle of a context switch callback,
	@ then quickly exit.
	LDREQ	a1, [a3, #GBL_PTH_CALLBACK_SEMAPHORE]
	TEQEQ	a1, #0
	BNE	skip_contextswitch

	@ Everything checks out, so from now on we're going to change
	@ contexts.

	@ Set __ul_global.pthread_callback_semaphore to ensure that another
	@ context interrupt does not interfere with us during this critical
	@ time.
	MOV	a1, #1
	STR	a1, [a3, #GBL_PTH_CALLBACK_SEMAPHORE]

	@ Setup a stack for the context switcher
 PICEQ "MOV	v4, ip"
	BL	__setup_signalhandler_stack

	@ Save regs to thread's save area
	LDR	a1, .L2+4	@=__pthread_running_thread
 PICEQ "LDR	a1, [ip, a1]"
	LDR	a1, [a1]
	LDR	a1, [a1, #__PTHREAD_CONTEXT_OFFSET]	@ __pthread_running_thread->saved_context

	@ Copy integer regs
	LDR	a2, .L2+8	@=__cbreg
 PICEQ "LDR	a2, [ip, a2]"
	LDMIA	a2!, {a3, a4, v1, v2, v3, v4, v5, v6}
	STMIA	a1!, {a3, a4, v1, v2, v3, v4, v5, v6}
	LDMIA	a2!, {a3, a4, v1, v2, v3, v4, v5, v6}
	STMIA	a1!, {a3, a4, v1, v2, v3, v4, v5, v6}
	@ Copy SPSR
	LDR	a3, [a2]
	STR	a3, [a1], #4

	@ We have to copy the integer regs before switching IRQs
	@ back on, so they don't get overwritten by another callback,
	@ but the floating point instructions should only be called
	@ from user mode
	CHGMODE	a3, USR_Mode

#ifndef __SOFTFP__
	@ Save floating point regs
	SFM	f0, 4, [a1], #48
	SFM	f4, 4, [a1], #48
	RFS	a2	@ Read floating status
	STR	a2, [a1]
#endif

	@ Switch to normal PIC register so that call to C function
	@ doesn't corrupt it.
 PICEQ "MOV	v4, ip"
	@ Call the scheduler to switch to another thread
	BL	__pthread_context_switch

	@ Now reload the registers from the new thread's save area
	LDR	a1, .L2+4	@=__pthread_running_thread
 PICEQ "LDR	a1, [v4, a1]"
	LDR	a1, [a1]
	LDR	a2, [a1, #__PTHREAD_CONTEXT_OFFSET]	@ __pthread_running_thread->saved_context

#ifndef __SOFTFP__
	ADD	a2, a2, #17*4
	LFM	f0, 4, [a2], #48
	LFM	f4, 4, [a2], #48
	LDR	a1, [a2]
	WFS	a1	@ Write floating status
#endif

	SWI	XOS_EnterOS	@ Back to supervisor mode

	CHGMODE	a2, SVC_Mode+IFlag	@ Force SVC mode, IRQs off

	LDR	a2, .L2		@=__ul_global
 PICEQ "LDR	a2, [v4, a2]"

	@ Indicate that this context switch was successful
	MOV	a1, #0
	STR	a1, [a2, #GBL_PTH_CALLBACK_MISSED]

	@ Signify that we are no longer in the middle of a context switch.
	STR	a1, [a2, #GBL_PTH_CALLBACK_SEMAPHORE]

	@ Indicate that we are no longer in a signal handler, since we
	@ will be returning direct to USR mode and the application itself.
	LDR	a1, [a2, #GBL_EXECUTING_SIGNALHANDLER]
	SUB	a1, a1, #1
	STR	a1, [a2, #GBL_EXECUTING_SIGNALHANDLER]

	@ Point to the register save area for the new thread.
	LDR	r14, .L2+4	@=__pthread_running_thread
 PICEQ "LDR	r14, [v4, r14]"
	LDR	r14, [r14]
	LDR	r14, [r14, #__PTHREAD_CONTEXT_OFFSET]	@ __pthread_running_thread->saved_context

	@ Restore thread's registers
	LDR	a1, [r14, #16*4]	@ Get user PSR
	MSR	SPSR_cxsf, a1		@ Put it into SPSR_SVC/IRQ (NOP on ARM2/3, shouldn't have any effect in 26bit mode)
	LDMIA	r14, {r0-r14}^		@ Load USR mode regs
	MOV	a1, a1

	LDR	r14, [r14, #15*4]	@ Load the old PC value
	MOVS	pc, lr			@ Return (Valid for 26 and 32bit modes)


	@ Called because we are fast exiting from the context switcher
	@ because some other context switching operation is already going on,
	@ or because the program indicates that we are in a critical
	@ section.

	@ On entry, a3 == __ul_global
skip_contextswitch:
	@ Indicate that this context switch did not occur
	MOV	a1, #1
	STR	a1, [a3, #GBL_PTH_CALLBACK_MISSED]

	@ Exiting from the CallBack handler requires us to reload all
	@ registers from the register save area.
	LDR	r14, .L2+8		@=__cbreg
 PICEQ "LDR	r14, [v4, r14]"
	LDR	a1, [r14, #16*4]	@ Get user PSR
	MSR	SPSR_cxsf, a1		@ Put it into SPSR_SVC/IRQ (NOP on ARM2/3, shouldn't have any effect in 26bit mode)
	LDMIA	r14, {r0-r14}^		@ Load USR mode regs
	MOV	a1, a1

	LDR	r14, [r14, #15*4]	@ Load the old PC value
	MOVS	pc, lr			@ Return (Valid for 26 and 32bit modes)
.L2:
	WORD	__ul_global
	WORD	__pthread_running_thread
	WORD	__cbreg
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC2+4)"
	DECLARE_FUNCTION __pthread_callback

@ entry:
@   R0 = save area
	.global	__pthread_init_save_area
	NAME	__pthread_init_save_area
__pthread_init_save_area:
#ifndef __SOFTFP__
	ADD	a2, a1, #17*4
	SFM	f0, 4, [a2], #48
	SFM	f4, 4, [a2], #48
	RFS	a1	@ Read floating status
	STR	a1, [a2], #12
#endif
	MOV	pc, lr
	DECLARE_FUNCTION __pthread_init_save_area


	.data

	@ Have we registered the CallEvery ticker ?
	@ Value 0 : no, CallEvery ticket is not enabled
	@       1 : yes CallEvery ticker is enabled
ticker_started:
	.word	0
	DECLARE_OBJECT ticker_started

	@ Pointer to filter name in RMA
filter_name_rma_ptr:
	.word	0
	DECLARE_OBJECT filter_name_rma_ptr

	@ Have we installed Filter ?
filter_installed:
	.word	0
	DECLARE_OBJECT filter_installed

	.end
