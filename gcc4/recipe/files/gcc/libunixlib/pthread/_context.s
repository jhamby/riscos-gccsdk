@ Low level context switching code
@ Written by Martin Piper and Alex Waugh
@ Copyright (c) 2002-2012 UnixLib Developers

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
@ If a routine does not want to be switched out, then it can call
@ __pthread_disable_ints or __pthread_protect_unsafe which will alter
@ __pthread_work_semaphore. The callevery interrupt will still occur, but it
@ will take note of the state of the semaphore, and not set a callback
@
@ Multitasking programs use the Filter module (Filter_RegisterPreFilter and
@ Filter_RegisterPostFilter) to enable/disable OS_CallEvery.

#include "internal/asm_dec.s"


	.syntax unified
	.text

@
@ Start a ticker which will set the callback flag every clock tick
@
	.global	__pthread_start_ticker
	NAME	__pthread_start_ticker
__pthread_start_ticker:
 PICNE "STMFD	sp!, {v1-v2, v5, lr}"
 PICEQ "STMFD	sp!, {v1-v2, v4, v5, lr}"

	PIC_LOAD v4

	LDR	v5, .L0			@=__ul_global
 PICEQ "LDR	v5, [v4, v5]"

	@ Don't start until the thread system has been setup
	LDR	a1, [v5, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
 PICEQ "LDMFDEQ	sp!, {v1-v2, v4, v5, pc}"
 PICNE "LDMFDEQ	sp!, {v1-v2, v5, pc}"

	@ Don't start if there's only one thread running
	LDR	a1, [v5, #GBL_PTH_NUM_RUNNING_THREADS]
	CMP	a1, #1
 PICEQ "LDMFDLE	sp!, {v1-v2, v4, v5, pc}"
 PICNE "LDMFDLE	sp!, {v1-v2, v5, pc}"

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
	LDR	v2, .L0+4		@=filter_installed
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
 PICEQ "LDMFD	sp!, {v1-v2, v4, v5, pc}"
 PICNE "LDMFD	sp!, {v1-v2, v5, pc}"
.L0:
	WORD	__ul_global
	WORD	filter_installed
	DECLARE_FUNCTION __pthread_start_ticker

	@ Start the RISC OS CallEvery ticker
	@ This may be called as a Wimp post filter, so preserve all registers,
	@ all processor flags and don't return any errors.
	@ R12 is our private word and contains the address of an RMA data block.
	NAME	start_call_every
start_call_every:
	STMFD	sp!, {a1-v1, lr}
	MRS	v1, CPSR
	LDR	a1, [r12, #PTHREAD_CALLEVERY_RMA_TICKER_STARTED]
	TEQ	a1, #0
	BNE	start_call_every_end

	MOV	a1, #1
	ADR	a2, pthread_call_every
	MOV	a3, r12
	SWI	XOS_CallEvery

	MOVVC	a1, #1
	STRVC	a1, [r12, #PTHREAD_CALLEVERY_RMA_TICKER_STARTED]
start_call_every_end:
	MSR	CPSR_f, v1
	LDMFD	sp!, {a1-v1, pc}
	DECLARE_FUNCTION start_call_every

	@ Stop the RISC OS CallEvery ticker
	@ This may be called as a Wimp pre filter, so preserve all registers,
	@ all processor flags and don't return any errors.
	@ R12 is our private word and contains the address of an RMA data block.
	NAME	stop_call_every
stop_call_every:
	STMFD	sp!, {a1-a3, lr}
	MRS	a3, CPSR
	LDR	a2, [r12, #PTHREAD_CALLEVERY_RMA_TICKER_STARTED]
	SUBS	a2, a2, #1
	STREQ	a2, [r12, #PTHREAD_CALLEVERY_RMA_TICKER_STARTED]
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

	PIC_LOAD v4

	@ Don't bother if thread system is not running
	LDR	a2, .L1+0		@=__ul_global
 PICEQ "LDR	a2, [v4, a2]"
	LDR	a1, [a2, #GBL_PTH_SYSTEM_RUNNING]
	TEQ	a1, #0
 PICEQ "LDMFDEQ	sp!, {v1-v3, v4, pc}"
 PICNE "LDMFDEQ	sp!, {v1-v3, pc}"

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
	LDMFDVS	sp!, {a1-a4, pc}

	@ We need to check the SUL upcall handler and key before we attempt
	@ to access any data within application space.

	@ If it is not a SUL upcall handler, don't set callback
	LDR	a1, [ip, #PTHREAD_CALLEVERY_RMA_UPCALL_ADDR]
	TEQ	a1, a2
	@ If it is not our SUL key, don't set callback
	LDREQ	a1, [ip, #PTHREAD_CALLEVERY_RMA_UPCALL_R12]
	TEQEQ	a1, a3
	LDMFDNE sp!, {a1-a4, pc}

	@ Okay, we can access application space now to make further checks.

	@ If we are in the middle of a context-switch callback,
	@ don't set the callback.
	LDREQ	a1, [ip, #PTHREAD_CALLEVERY_RMA_CALLBACK_SEMAPHORE]
	TEQEQ	a1, #0
	@ If we are in a critical section, don't set the callback
	LDREQ	a1, [ip, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE]
	TEQEQ	a1, #0
	SWIEQ	XOS_SetCallBack
0:
	LDMFD	sp!, {a1-a4, pc}
	DECLARE_FUNCTION pthread_call_every

@ This is called from _signal.s::__h_cback and pthread_yield.
@ Entered in SVC or IRQ mode with IRQs disabled.

@ The operating system will eventually be returning to USR mode.
	.global	__pthread_callback

	NAME	__pthread_callback
__pthread_callback:
	PIC_LOAD v1
 
	LDR	a3, .L2			@=__ul_global
 PICEQ "LDR	a3, [v1, a3]"
 
	@ If we are in a critical region, do not switch threads and
	@ exit quicky.
	LDR	ip, [a3, #GBL_PTH_CALLEVERY_RMA]
	LDR	a1, [ip, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE]

	TEQ	a1, #0
	@ If we are already in the middle of a context switch callback,
	@ then quickly exit.
	LDREQ	a1, [ip, #PTHREAD_CALLEVERY_RMA_CALLBACK_SEMAPHORE]
	TEQEQ	a1, #0
	BNE	skip_contextswitch

	@ Everything checks out, so from now on we're going to change
	@ contexts.

	@ Set __ul_global.pthread_callback_semaphore to ensure that another
	@ context interrupt does not interfere with us during this critical
	@ time.
	MOV	a1, #1
	STR	a1, [ip, #PTHREAD_CALLEVERY_RMA_CALLBACK_SEMAPHORE]

	@ Setup a stack for the context switcher
	BL	__setup_signalhandler_stack

	@ Save regs to thread's save area
	LDR	a1, .L2+4	@=__pthread_running_thread
 PICEQ "LDR	a1, [v1, a1]"
	LDR	a1, [a1]
	LDR	a1, [a1, #__PTHREAD_CONTEXT_OFFSET]	@ __pthread_running_thread->saved_context

	@ Copy integer regs
	MOV	a2, ip		@ callback regs are stored at the start of the fast access RMA block
 PICEQ "MOV	ip, v1"		@ Save PIC register
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
#  ifdef __VFP_FP__
	@ Save the thread's VFP registers, and create a new context on the stack for the
	@ thread scheduler to use
	LDR	a3, .L2			@=__ul_global
 PICEQ "LDR	a3, [v1, a3]"
#    ifdef __ARM_EABI__
	MOV	a1, #0x40000001	@ User mode, dynamic area, lazy activate
#    else
	MOV	a1, #0x40000003	@ User mode, application space, lazy activate
#    endif
	LDR	a2, [a3, #GBL_VFP_REGCOUNT]
	SWI	VFPSupport_CheckContext
	MOV	v2, sp		@ Remember old SP
	SUB	a3, sp, a1
	BIC	a3, a3, #7	@ AAPCS wants 8 byte alignment
	MOV	sp, a3
#    ifdef __ARM_EABI__
	MOV	a1, #0x40000001	@ User mode, dynamic area, lazy activate
#    else
	MOV	a1, #0x40000003
#    endif
	MOV	a4, #0
	SWI	VFPSupport_CreateContext
#  else
	@ Save floating point regs
	SFM	f0, 4, [a1], #48
	SFM	f4, 4, [a1], #48
	RFS	a2	@ Read floating status
	STR	a2, [a1]
#  endif
#endif

	@ Call the scheduler to switch to another thread
	BL	__pthread_context_switch

	@ Now reload the registers from the new thread's save area
	LDR	a1, .L2+4	@=__pthread_running_thread
 PICEQ "LDR	a1, [v1, a1]"
	LDR	a1, [a1]
	LDR	a2, [a1, #__PTHREAD_CONTEXT_OFFSET]	@ __pthread_running_thread->saved_context

#ifndef __SOFTFP__
#  ifdef __VFP_FP__
	@ Destroy our temp context, and in the process switch to the target context
	LDR	a2, [a2, #17*4]
	MOV	a1, sp
	BIC	a2, a2, #1
	SWI	VFPSupport_DestroyContext
	MOV	sp, v2
#  else
	ADD	a2, a2, #17*4
	LFM	f0, 4, [a2], #48
	LFM	f4, 4, [a2], #48
	LDR	a1, [a2]
	WFS	a1	@ Write floating status
#  endif
#endif

	SWI	XOS_EnterOS	@ Back to supervisor mode

	CHGMODE	a2, SVC_Mode+IFlag	@ Force SVC mode, IRQs off

	LDR	a2, .L2		@=__ul_global
 PICEQ "LDR	a2, [v1, a2]"

	@ Indicate that this context switch was successful
	MOV	a1, #0
	STR	a1, [a2, #GBL_PTH_CALLBACK_MISSED]

	@ Signify that we are no longer in the middle of a context switch.
	LDR	ip, [a2, #GBL_PTH_CALLEVERY_RMA]
	STR	a1, [ip, #PTHREAD_CALLEVERY_RMA_CALLBACK_SEMAPHORE]

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

	@ On entry, a3 = __ul_global
	@	    ip = ptr to fast access RMA block
skip_contextswitch:
	@ Indicate that this context switch did not occur
	MOV	a1, #1
	STR	a1, [a3, #GBL_PTH_CALLBACK_MISSED]

	@ Exiting from the CallBack handler requires us to reload all registers from the
	@ register save area which is at the beginning of the fast access RMA block
	LDR	a1, [r14, #16*4]	@ Get user PSR
	MSR	SPSR_cxsf, a1		@ Put it into SPSR_SVC/IRQ (NOP on ARM2/3, shouldn't have any effect in 26bit mode)
	LDMIA	r14, {r0-r14}^		@ Load USR mode regs
	MOV	a1, a1

	LDR	r14, [r14, #15*4]	@ Load the old PC value
	MOVS	pc, lr			@ Return (Valid for 26 and 32bit modes)
.L2:
	WORD	__ul_global
	WORD	__pthread_running_thread
	DECLARE_FUNCTION __pthread_callback

@ entry:
@   R0 = save area
	.global	__pthread_init_save_area
	NAME	__pthread_init_save_area
__pthread_init_save_area:
#ifndef __SOFTFP__
#  ifdef __VFP_FP__
	@ Allocate a VFP context from the heap
	@ Make sure we specify the 'application space' flag if we're not using a DA
	STMFD	sp!, {a1, v1, v2, lr}

	PIC_LOAD ip

	LDR	v2, .L3			@=__ul_global
 PICEQ "LDR	v2, [ip, v2]"

	LDR	a1, [v2, #GBL_DYNAMIC_NUM]
	CMP	a1, #-1
	MOVEQ	a1, #0x3	@ User mode, application space
	MOVNE	a1, #0x1	@ User mode, dynamic area
	LDR	a2, [v2, #GBL_VFP_REGCOUNT]
	MOV	v1, a1
	SWI	VFPSupport_CheckContext

	MOV	a2, a1
	LDR	a1, [v2, #GBL_MALLOC_STATE]
	BL	malloc_unlocked		@ TODO check for null

	MOV	a3, a1
	MOV	a1, v1
	LDR	a2, [v2, #GBL_VFP_REGCOUNT]
	MOV	a4, #0
	SWI	VFPSupport_CreateContext
	LDMFD	sp!, {a2, v1, v2, lr}
	STR	a1, [a2, #17*4]
#  else
	ADD	a2, a1, #17*4
	SFM	f0, 4, [a2], #48
	SFM	f4, 4, [a2], #48
	RFS	a1	@ Read floating status
	STR	a1, [a2], #12
#  endif
#endif
	MOV	pc, lr
.L3:
	WORD	__ul_global
	DECLARE_FUNCTION __pthread_init_save_area


	.data

	@ Have we installed Filter ?
filter_installed:
	.word	0
	DECLARE_OBJECT filter_installed

	.end
