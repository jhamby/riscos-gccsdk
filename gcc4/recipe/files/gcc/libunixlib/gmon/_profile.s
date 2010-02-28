@ Low level profiling support code
@ Copyright (c) 2010 UnixLib Developers.
@ Written by Lee Noar.

#include "internal/asm_dec.s"

				.struct	0
HAL_ROUTINES_IRQ_SOURCE:	.skip	4
HAL_ROUTINES_IRQ_ENABLE:	.skip	4
HAL_ROUTINES_IRQ_DISABLE:	.skip	4
HAL_ROUTINES_IRQ_CLEAR:		.skip	4
HAL_ROUTINES_STATIC_BASE:	.skip	4
HAL_ROUTINES_STRUCT_SIZE:

/* Flag indicating whether any sample overflowed when the
   interrupt routine incremented it.  */
.set INT_HANDLER_FLAG_OVERFLOWED,		1

/* Flag indicating whether the interrupt handler should record
   any samples. Set and cleared by the WIMP filters.  */
.set INT_HANDLER_FLAG_DISABLED_BY_FILTER,	2

/* Flag indicating whether the user disabled profiling using
   moncontrol().  */
.set INT_HANDLER_FLAG_DISABLED_BY_USER,		4

				.struct -4
@ The address of the old handler is loaded via PC-relative addressing
@ in the interrupt handler.
INT_HANDLER_OLD_HANDLER:	.skip	4
@ This is where the C structure starts
INT_HANDLER_SAMPLES:		.skip	4
INT_HANDLER_NSAMPLES:		.skip	4
INT_HANDLER_PC_OFFSET:		.skip	4
INT_HANDLER_PC_SCALE:		.skip	4
INT_HANDLER_TIMER_NUMBER:	.skip	4
INT_HANDLER_TIMER_DEVICE:	.skip	4
INT_HANDLER_FLAGS:		.skip	4
INT_HANDLER_HAL_ROUTINES:	.skip	HAL_ROUTINES_STRUCT_SIZE
INT_HANDLER_STRUCT_SIZE:

/* Flag indicating whether the WIMP filters were installed.  */
.set PROFILER_FLAG_FILTER_INSTALLED,	1

/* Flag indicating whether the interrupt handler is installed
   (using SWI OS_ClaimProcessorVector).  */
.set PROFILER_FLAG_HANDLER_INSTALLED,	2

/* Flag indicating whether this is the first time that profiling was enabled.  */
.set PROFILER_FLAG_FIRST_TIME,		4

				.struct	0
PROFILER_DA_NUMBER:		.skip	4
PROFILER_DA_BASE:		.skip	4
PROFILER_TIMER_NUMBER:		.skip	4
PROFILER_TIMER_DEVICE:		.skip	4
PROFILER_TIMER_PERIOD:		.skip	4
PROFILER_TIMER_FREQUENCY:	.skip	4
PROFILER_RMA_BLOCK:		.skip	4
PROFILER_HANDLER_CODE:		.skip	4
PROFILER_HANDLER_DATA:		.skip	4
PROFILER_FILTER_NAME:		.skip	4
PROFILER_FLAGS:			.skip	4
PROFILER_STRUCT_SIZE:

@ Reason codes of some of the HAL routines used.
.set	HAL_IRQEnable, 1
.set	HAL_IRQDisable, 2
.set	HAL_Timers, 12
.set	HAL_TimerDevice, 13
.set	HAL_TimerGranularity,  14
.set	HAL_TimerSetPeriod, 16
.set	HAL_TimerPeriod, 17

	.text

	@ int __hal_timer_device(int timer)
	.global __hal_timer_device
	NAME	__hal_timer_device
	.hidden __hal_timer_device
__hal_timer_device:
	STR	r9, [sp, #-4]!

	MOV	r8, #0
	MOV	r9, #HAL_TimerDevice
	SWI	XOS_Hardware

	LDR	r9, [sp], #4
	MOV	pc, lr

	@ unsigned int __hal_timer_granularity(int timer)
	.global __hal_timer_granularity
	NAME	__hal_timer_granularity
	.hidden __hal_timer_granularity
__hal_timer_granularity:
	STR	r9, [sp, #-4]!

	MOV	r8, #0
	MOV	r9, #HAL_TimerGranularity
	SWI	XOS_Hardware

	LDR	r9, [sp], #4
	MOV	pc, lr

	@ int __hal_timers(void)
	.global __hal_timers
	NAME	__hal_timers
	.hidden	__hal_timers
__hal_timers:
	STR	r9, [sp, #-4]!

	MOV	r8, #0
	MOV	r9, #HAL_Timers
	SWI	XOS_Hardware

	LDR	r9, [sp], #4
	MOV	pc, lr
	DECLARE_FUNCTION __hal_timers

	@ void *__hal_get_routine_address (int routine_num, void **sb)
	.global __hal_get_routine_address
	NAME	__hal_get_routine_address
	.hidden	__hal_get_routine_address
__hal_get_routine_address:
	STR	r9, [sp, #-4]!

	MOV	r12, r1

	MOV	r8, #1
	MOV	r9, r0
	SWI	XOS_Hardware
	TEQ	r12, #0
	STRNE	r1, [r12, #0]

	LDR	r9, [sp], #4
	MOV	pc, lr

samples		.req	r0
nsamples	.req	r1
pc_offset	.req	r2
pc_scale	.req	r3

@ Normally, I would use r12 as the data pointer,
@ but the HAL routines are allowed to corrupt it.
data_ptr	.req	r8

@ Entry:
@  Processor is in IRQ mode
@  Called directly, with no pre-veneer, as a replacement for the OS IRQ
@  handler. Any IRQs that we are not interested in must be passed to the
@  OS handler in order for the machine to continue working normally.
@
@  r13 = irq stack
@
@  This routine is copied to RMA and executed from there so that the program
@  doesn't need to be paged in. We could disable/enable IRQs for the timer
@  in the WIMP filter, but this is likely to be more expensive than just
@  setting/clearing a flag as we do now and actually leads to instability.
	NAME	__profiler_interrupt_handler
__profiler_interrupt_handler:
	STMFD	sp!, {r0-r4, r8, r9, r12, lr}
	MRS	r4, SPSR

	SUB	r12, lr, #4
	ADR	data_ptr, handler_data

	LDR	r9, [data_ptr, #INT_HANDLER_HAL_ROUTINES + HAL_ROUTINES_STATIC_BASE]

	@ Find out which device caused the interrupt
	MOV	lr, pc
	LDR	pc, [data_ptr, #INT_HANDLER_HAL_ROUTINES + HAL_ROUTINES_IRQ_SOURCE]

	@ Was it our timer?
	LDR	r1, [data_ptr, #INT_HANDLER_TIMER_DEVICE]
	TEQ	r0, r1
	BEQ	0f

	@ If it's not our timer, then pass it on to the OS interrupt handler
	MSR	SPSR_cxsf, r4
	LDMFD	sp!, {r0-r4, r8, r9, r12, lr}
	LDR	pc, old_handler
0:
	@ If the interrupt has been disabled either by the WIMP filter or the user, then
	@ don't record any samples, just acknowledge it and return.
	LDR	r0, [data_ptr, #INT_HANDLER_FLAGS]
	TST	r0, #(INT_HANDLER_FLAG_DISABLED_BY_FILTER + INT_HANDLER_FLAG_DISABLED_BY_USER)
	BNE	exit

	LDMIA	data_ptr, {samples, nsamples, pc_offset, pc_scale}

	SUB	lr, r12, pc_offset
	MOV	lr, lr, LSR #1

	@ pc_scale is a 16.16 fixed point number - we use 64bit multiply
	@ to avoid overflow.
	@ Rdlo:Rdhi = Rm x Rs
	UMULL	r4, r2, pc_scale, lr

	@ Shift 64bit number 16bits to the right to remove the fractional
	@ part and place the whole into a 32bit register.
	MOV	r4, r4, LSR#16
	ORR	r4, r4, r2, LSL#16

	@ This combined with the shift in the LDR/STR below
	@ converts the sample index to a word aligned offset.
	BIC	lr, r4, #1

	@ Treating the values as unsigned (by using LS) will catch
	@ negative sample indices as well.
	CMP	nsamples, r4
	BLS	exit

	@ Load the 32bit word containing the 16bit sample.
	LDR	r1, [samples, lr, LSL #1]

	@ Determine which half of the 32bit word is the sample.
	TST	r4, #1

	@ An odd numbered sample is in the lower half of the 32bit
	@ word. Move it to the top of the word so that we can detect
	@ its overflow.
	MOVNE	r1, r1, ROR #16
	ADDS	r1, r1, #(1 << 16)
	BCS	overflow

	@ Put the samples back in the right order if necessary, and
	@ store back to the buffer.
	TST	r4, #1
	MOVNE	r1, r1, ROR #16

	STR	r1, [samples, lr, LSL #1]

exit:
	@ r9 is still the static base
	LDR	r0, [data_ptr, #INT_HANDLER_TIMER_DEVICE]
	MOV	lr, pc
	LDR	pc, [data_ptr, #INT_HANDLER_HAL_ROUTINES + HAL_ROUTINES_IRQ_CLEAR]

	LDMFD	sp!, {r0-r4, r8, r9, r12, lr}
	SUBS	pc, lr, #4
overflow:
	@ r9 is still the static base
	LDR	r0, [data_ptr, #INT_HANDLER_TIMER_DEVICE]
	MOV	lr, pc
	LDR	pc, [data_ptr, #INT_HANDLER_HAL_ROUTINES + HAL_ROUTINES_IRQ_DISABLE]

	LDR	r0, [data_ptr, #INT_HANDLER_FLAGS]
	ORR	r0, r0, #INT_HANDLER_FLAG_OVERFLOWED
	STR	r0, [data_ptr, #INT_HANDLER_FLAGS]

	LDMFD	sp!, {r0-r4, r8, r9, r12, lr}
	SUBS	pc, lr, #4

	@ We need to be able to branch to the old handler without using a register,
	@ so we use pc relative addressing. Although the handler is copied to RMA,
	@ we arrange for the address of the old handler to be written to the same
	@ relative position.
old_handler:
	.word	0
handler_data:
	DECLARE_FUNCTION __profiler_interrupt_handler

	@ _kernel_oserror *__profile_enable (struct ul_global *, struct profiler_data_t *);
	.global	__profile_enable
	NAME	__profile_enable
	.protected __profile_enable
__profile_enable:
	STMFD	sp!, {r4, r9, lr}

	MOV	r12, r1
	MOV	r14, r0

	@ Are we running as WIMP task ?
	LDR	r3, [r14, #GBL_TASKHANDLE]
	TEQ	r3, #0
	BNE	install_filters

	@ Application may have called Wimp_Initialise since we last checked
	@ Code similar to __get_taskhandle.
	MOV	r0, #3				@ In desktop?
	SWI	XWimp_ReadSysInfo
	MOVVS	r0, #0
	TEQ	r0, #0
	MOVNE	r0, #5				@ Read taskhandle, if in desktop
	SWINE	XWimp_ReadSysInfo
	MOVVS	r0, #0

	MOVS	r3, r0
	BEQ	finish_filter_install

	STR	r3, [r14, #GBL_TASKHANDLE]	@ __ul_global.taskhandle

	@ If we're running as a task, enable the WIMP filters, otherwise,
	@ other tasks will contribute to the samples giving useless results.
install_filters:
	LDR	r0, [r12, #PROFILER_FILTER_NAME]
	ADR	r1, wimp_filter_disable_timer
	LDR	r2, [r12, #PROFILER_HANDLER_DATA]
	@ r3 already task handle
	SWI	XFilter_RegisterPreFilter
	ADRVC	r1, wimp_filter_enable_timer
	MOVVC	r4, #0
	SWIVC	XFilter_RegisterPostFilter
	BVS	0f

	LDR	r0, [r12, #PROFILER_FLAGS]
	ORR	r0, r0, #PROFILER_FLAG_FILTER_INSTALLED
	STR	r0, [r12, #PROFILER_FLAGS]

finish_filter_install:
	LDR	r4, [r12, #PROFILER_TIMER_NUMBER]

	@ The period is the number of machine ticks between each interrupt.
	MOV	r0, r4
	LDR	r1, [r12, #PROFILER_TIMER_PERIOD]
	MOV	r8, #0
	MOV	r9, #HAL_TimerSetPeriod
	SWI	XOS_Hardware

	@ The period we asked for may not have been posible, so
	@ get the period we actually ended up with.
	MOVVC	r0, r4
	MOVVC	r9, #HAL_TimerPeriod
	SWIVC	XOS_Hardware
	BVS	0f

	STR	r0, [r12, #PROFILER_TIMER_PERIOD]

	@ Install interrupt handler on hardware vector. This replaces the
	@ previous handler, so we store the address of the old handler so that
	@ we can pass on any interrupts that we're not interested in.
	MOV	r0, #6
	ORR	r0, r0, #0x100
	LDR	r1, [r12, #PROFILER_HANDLER_CODE]
	SWI	XOS_ClaimProcessorVector
	BVS	0f
	LDR	r0, [r12, #PROFILER_HANDLER_DATA]
	STR	r1, [r0, #INT_HANDLER_OLD_HANDLER]

	@ Record the fact that our handler is installed.
	LDR	r0, [r12, #PROFILER_FLAGS]
	ORR	r0, r0, #PROFILER_FLAG_HANDLER_INSTALLED
	STR	r0, [r12, #PROFILER_FLAGS]

	@ Tell the timer to start interrupting.
	LDR	r0, [r12, #PROFILER_TIMER_DEVICE]
	MOV	r9, #HAL_IRQEnable
	SWI	XOS_Hardware
	MOVVC	r0, #0
0:
	LDMFD	sp!, {r4, r9, pc}
	DECLARE_FUNCTION __profile_enable

	@ void __profile_disable (struct ul_global *, struct profiler_data_t *);
	.global	__profile_disable
	NAME	__profile_disable
	.protected __profile_disable
__profile_disable:
	STMFD	sp!, {r4-r6, r9, lr}

	MOV	r5, r0
	MOV	r6, r1

	@ Stop the timer from generating interrupts.
	MOV	r8, #0
	MOV	r9, #HAL_IRQDisable
	LDR	r0, [r6, #PROFILER_TIMER_DEVICE]
	SWI	XOS_Hardware

	LDR	r8, [r6, #PROFILER_HANDLER_DATA]

	@ Remove the interrupt handler from the hardware vectors,
	@ and restore the previous handler.
	MOV	r0, #6
	LDR	r1, [r8, #INT_HANDLER_OLD_HANDLER]
	LDR	r2, [r6, #PROFILER_HANDLER_CODE]
	SWI	XOS_ClaimProcessorVector

	@ Record the fact that our interrupt handler is no longer installed.
	MOV	r9, #0
	STR	r9, [r8, #INT_HANDLER_OLD_HANDLER]
	LDR	r9, [r6, #PROFILER_FLAGS]
	BIC	r9, r9, #PROFILER_FLAG_HANDLER_INSTALLED

	@ If the WIMP filters were installed, then remove them now.
	TST	r9, #PROFILER_FLAG_FILTER_INSTALLED
	BEQ	finish_filter_removal

	LDR	r0, [r6, #PROFILER_FILTER_NAME]
	ADR	r1, wimp_filter_disable_timer
	LDR	r2, [r6, #PROFILER_HANDLER_DATA]
	LDR	r3, [r5, #GBL_TASKHANDLE]
	SWI	XFilter_DeRegisterPreFilter

	ADR	r1, wimp_filter_enable_timer
	MOV	r4, #0
	SWI	XFilter_DeRegisterPostFilter

	@ Record the fact that our WIMP filters are no longer installed.
	BIC	r9, r9, #PROFILER_FLAG_FILTER_INSTALLED

finish_filter_removal:
	STR	r9, [r6, #PROFILER_FLAGS]

	LDMFD	sp!, {r4-r6, r9, pc}
	DECLARE_FUNCTION __profile_disable

@ Entry:
@  Processor is in SVC mode
@  r12 = ptr to interrupt handler data block
@  Preserve all other registers
	NAME	wimp_filter_disable_timer
wimp_filter_disable_timer:
	STR	r0, [sp, #-4]!

	LDR	r0, [r12, #INT_HANDLER_FLAGS]
	ORR	r0, r0, #INT_HANDLER_FLAG_DISABLED_BY_FILTER
	STR	r0, [r12, #INT_HANDLER_FLAGS]

	LDR	r0, [sp], #4
	MOV	pc, lr
	DECLARE_FUNCTION wimp_filter_disable_timer

@ Entry:
@  Processor is in SVC mode
@  r12 = ptr to interrupt handler data block
@  Preserve all other registers
	NAME	wimp_filter_enable_timer
wimp_filter_enable_timer:
	STR	r0, [sp, #-4]!

	LDR	r0, [r12, #INT_HANDLER_FLAGS]
	BIC	r0, r0, #INT_HANDLER_FLAG_DISABLED_BY_FILTER
	STR	r0, [r12, #INT_HANDLER_FLAGS]

	LDR	r0, [sp], #4
	MOV	pc, lr
	DECLARE_FUNCTION wimp_filter_enable_timer

	.data

	.global	__profiler_interrupt_handler_start
	.hidden	__profiler_interrupt_handler_start
__profiler_interrupt_handler_start:
	.word	__profiler_interrupt_handler
	DECLARE_OBJECT __profiler_interrupt_handler_start

	.global	__profiler_interrupt_handler_end
	.hidden	__profiler_interrupt_handler_end
__profiler_interrupt_handler_end:
	.word	handler_data
	DECLARE_OBJECT __profiler_interrupt_handler_end

	.end
