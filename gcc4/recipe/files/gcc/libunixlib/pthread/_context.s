@ Low level context switching code
@ Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2010 UnixLib Developers
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

	@ Offsets into the RMA memory block.
.set	PTHREAD_CALLEVERY_RMA_GOT, 0
.set	PTHREAD_CALLEVERY_RMA_UPCALL_ADDR, 4
.set	PTHREAD_CALLEVERY_RMA_UPCALL_R12, 8
.set	PTHREAD_CALLEVERY_RMA_FILTER_NAME, 12

	.text

@
@ Start a ticker which will set the callback flag every clock tick
@
	.global	__pthread_start_ticker
	NAME	__pthread_start_ticker
__pthread_start_ticker:
 PICNE "STMFD	sp!, {v1-v2, lr}"
 PICEQ "STMFD	sp!, {v1-v2, v4, lr}"

 PICEQ "LDR	v4, =__GOTT_BASE__"
 PICEQ "LDR	v4, [v4, #0]"
 PICEQ "LDR	v4, [v4, #__GOTT_INDEX__]"	@ v4 = GOT ptr

	LDR	v5, .L0			@=__ul_global
 PICEQ "LDR	v5, [v4, v5]"

	@ Don't start until the thread system has been setup
	LDR	a1, [v5, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
 PICEQ "LDMEQFD	sp!, {v1-v2, v4, pc}"
 PICNE "LDMEQFD	sp!, {v1-v2, pc}"

	@ Don't start if there's only one thread running
	LDR	a1, [v5, #GBL_PTH_NUM_RUNNING_THREADS]
	CMP	a1, #1
 PICEQ "LDMLEFD	sp!, {v1-v2, v4, pc}"
 PICNE "LDMLEFD	sp!, {v1-v2, pc}"

	LDR	a3, [v5, #GBL_PTH_CALLEVERY_RMA]

	@ Are we running as WIMP task ?
	@ If we are then we need a filter switching off our ticker when we're
	@ swapped out and switching back on after we're swapped in.
	LDR	a4, [v5, #GBL_TASKHANDLE]
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
 
	STR	a4, [v5, #GBL_TASKHANDLE]	@ __ul_global.taskhandle
 
	@ a4 = current taskhandle
start_ticker_install_filters:
	@ Install the filter routines (a4 = WIMP taskhandle) :
	LDR	v2, .L0+8		@=filter_installed
 PICEQ "LDR	v2, [v4, v2]"
	LDR	a1, [v2]
	STR	v2, [v2]
	TEQ	a1, #0
	BNE	start_ticker_test_running

	@ get filter name RMA pointer
	ADD	a1, a3, #PTHREAD_CALLEVERY_RMA_FILTER_NAME
	ADR	a2, stop_call_every
	@ a3 is still the address of the RMA block allocated and populated above.
	@ a4 is still equal to taskhandle
	SWI	XFilter_RegisterPreFilter
	ADRVC	a2, start_call_every
	@ a3 is still the RMA block
	@ a4 is still equal to taskhandle
	MOVVC	v1, #0
	SWIVC	XFilter_RegisterPostFilter
	ADDVS	a1, a1, #4
	BVS	__pthread_fatal_error

start_ticker_test_running:
	@ start_call_every may be called as a filter routine, in which case
	@ it expects a pointer to the RMA data block in r12. Make sure that's
	@ the case when called directly.
	MOV	r12, a3
	BL	start_call_every
 PICEQ "LDMFD	sp!, {v1-v2, v4, pc}"
 PICNE "LDMFD	sp!, {v1-v2, pc}"
.L0:
	WORD	__ul_global
	WORD	ticker_started
	WORD	filter_installed
	DECLARE_FUNCTION __pthread_start_ticker

	@ Start the RISC OS CallEvery ticker
	@ This may be called as a Wimp post filter, so preserve all registers,
	@ all processor flags and don't return any errors.
	@ R12 is our private word and contains the address of an RMA data block
	@ which contains the GOT pointer at offset 0.
	NAME	start_call_every
start_call_every:
	STMFD	sp!, {a1-v1, lr}
	MRS	v1, CPSR
 PICEQ "LDR	a3, [r12, #PTHREAD_CALLEVERY_RMA_GOT]"
	LDR	a4, .L0+4	@=ticker_started
 PICEQ "LDR	a4, [a3, a4]"
	LDR	a1, [a4]
	TEQ	a1, #0
	BNE	start_call_every_end

	MOV	a1, #1
	ADR	a2, pthread_call_every
	MOV	a3, r12
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
	@ R12 is our private word and contains the address of an RMA data block
	@ which contains the GOT pointer at offset 0.
	NAME	stop_call_every
stop_call_every:
	STMFD	sp!, {a1-a3, lr}
	MRS	a3, CPSR
 PICEQ "LDR	a1, [r12, #PTHREAD_CALLEVERY_RMA_GOT]"
	@ Don't stop it if it isn't enabled yet
	LDR	lr, .L0+4	@=ticker_started
 PICEQ "LDR	lr, [a1, lr]"
	LDR	a2, [lr]
	SUBS	a2, a2, #1
	STREQ	a2, [lr]
	ADREQ	a1, pthread_call_every
	MOVEQ	a2, r12
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

 PICEQ "LDR	v4, =__GOTT_BASE__"
 PICEQ "LDR	v4, [v4, #0]"
 PICEQ "LDR	v4, [v4, #__GOTT_INDEX__]"	@ v4 = GOT ptr

	@ Don't bother if thread system is not running
	LDR	a2, .L1+0		@=__ul_global
 PICEQ "LDR	a2, [v4, a2]"
	LDR	a1, [a2, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
 PICEQ "LDMEQFD	sp!, {v1-v3, v4, pc}"
 PICNE "LDMEQFD	sp!, {v1-v3, pc}"

	LDR	a3, [a2, #GBL_PTH_CALLEVERY_RMA]

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

	ADD	a1, a3, #PTHREAD_CALLEVERY_RMA_FILTER_NAME
	ADR	a2, stop_call_every
	@ a3 is the address of the RMA block.
	@ a4 is still equal to taskhandle
	SWI	XFilter_DeRegisterPreFilter
	ADD	a1, a3, #PTHREAD_CALLEVERY_RMA_FILTER_NAME	@ a1 can be corrupted, so re-init
	ADR	a2, start_call_every
	@ a3 is the address of the RMA block.
	@ a4 is still equal to taskhandle
	MOV	v1, #0
	SWI	XFilter_DeRegisterPostFilter

	STR	v1, [v2]		@ Mark we no longer have filter installed

stop_ticker_core:
	@ stop_call_every may be called as a filter routine, in which case
	@ it expects a pointer to the RMA data block in r12. Make sure that's
	@ the case when called directly.
	MOV	r12, a3
	BL	stop_call_every

 PICEQ "LDMFD	sp!, {v1-v3, v4, pc}"
 PICNE "LDMFD	sp!, {v1-v3, pc}"
.L1:
	WORD	__ul_global
	WORD	filter_installed
	DECLARE_FUNCTION __pthread_stop_ticker

@ The ticker calls this every clock tick, note that it is every *two*
@ centiseconds.
@ Called in SVC mode with IRQs disabled, all registers must be preserved.
@ r12 contains a pointer to a block of RMA memory which stores the address
@ of the SUL upcall handler and the SUL key (and the UnixLib GOT pointer
@ for the shared library). We can't be certain the application is paged in
@ (despite the filters), so we dare not attempt to access its address space,
@ which is why these are in RMA.
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

	@ We need to check the SUL upcall handler and key before we attempt
	@ to access any data within application space.

	@ If it is not a SUL upcall handler, don't set callback
	LDR	a1, [ip, #PTHREAD_CALLEVERY_RMA_UPCALL_ADDR]
	TEQ	a1, a2
	@ If it is not our SUL key, don't set callback
	LDREQ	a1, [ip, #PTHREAD_CALLEVERY_RMA_UPCALL_R12]
	TEQEQ	a1, a3
	LDMNEFD sp!, {a1-a4, pc}

 PICEQ "LDR	ip, [ip, #PTHREAD_CALLEVERY_RMA_GOT]"

	@ Okay, we can access application space now to make further checks.

	LDR	a4, .L1		@=__ul_global
 PICEQ "LDR	a4, [ip, a4]"

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
 PICEQ "LDR	v1, =__GOTT_BASE__"
 PICEQ "LDR	v1, [v1, #0]"
 PICEQ "LDR	v1, [v1, #__GOTT_INDEX__]"	@ ip = GOT ptr

	LDR	a3, .L2			@=__ul_global
 PICEQ "LDR	a3, [v1, a3]"
 
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
	BL	__setup_signalhandler_stack

	@ Save regs to thread's save area
	LDR	a1, .L2+4	@=__pthread_running_thread
 PICEQ "LDR	a1, [v1, a1]"
	LDR	a1, [a1]
	LDR	a1, [a1, #__PTHREAD_CONTEXT_OFFSET]	@ __pthread_running_thread->saved_context

	@ Copy integer regs
	LDR	a2, .L2+8	@=__cbreg
 PICEQ "MOV	ip, v1"		@ Save PIC register
 PICEQ "LDR	a2, [v1, a2]"
	LDMIA	a2!, {a3, a4, v1, v2, v3, v4, v5, v6}
	STMIA	a1!, {a3, a4, v1, v2, v3, v4, v5, v6}
	LDMIA	a2!, {a3, a4, v1, v2, v3, v4, v5, v6}
	STMIA	a1!, {a3, a4, v1, v2, v3, v4, v5, v6}
	@ Copy SPSR
	LDR	a3, [a2]
 PICEQ "MOV	v1, ip"		@ Restore PIC register
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

	@ Call the scheduler to switch to another thread
	BL	__pthread_context_switch

	@ Now reload the registers from the new thread's save area
	LDR	a1, .L2+4	@=__pthread_running_thread
 PICEQ "LDR	a1, [v1, a1]"
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
 PICEQ "LDR	a2, [v1, a2]"

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
 PICEQ "LDR	r14, [v1, r14]"
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
 PICEQ "LDR	r14, [v1, r14]"
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

	@ Have we installed Filter ?
filter_installed:
	.word	0
	DECLARE_OBJECT filter_installed

	.end
