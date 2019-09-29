@ Enable and disable the callevery interrupt from causing context switches
@ disable_ints can be called multiple times, provided enable_ints is
@ subsequently called an equal number of times
@ Written by Martin Piper and Alex Waugh
@ Copyright (c) 2002-2016 UnixLib Developers

#include "internal/asm_dec.s"

	.text

	.global	__pthread_disable_ints
	.global	__pthread_enable_ints
	.global	__pthread_protect_unsafe

@ Disable context switches by incrementing the semaphore
@ May be called from USR or SVC mode
	NAME	__pthread_disable_ints
__pthread_disable_ints:
	PIC_LOAD a2

	LDR	a1, .L0				@ =__ul_global
 PICEQ "LDR	a1, [a2, a1]"
	LDR	ip, [a1, #GBL_PTH_CALLEVERY_RMA]
#ifdef __ARM_EABI__
	ADD	a1, ip, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE
#else
	LDR	a4, [a1, #GBL_CPU_FLAGS]
	ADD	a1, ip, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE
	TST	a4, #__CPUCAP_HAVE_SWP
	@ From this point onwards we will not be interrupted by the callback
	BEQ	0f

	MOV	a3, #1
	SWP	a2, a3, [a1]
	ADD	a2, a2, #1
	STR	a2, [a1]
	MOV	pc, lr
#endif
0:
	LDREX	a2, [a1]
	ADD	a2, a2, #1
	STREX	a3, a2, [a1]
	TEQ	a3, #1
	BEQ	0b
	MOV	pc, lr
.L0:
	WORD	__ul_global
	DECLARE_FUNCTION __pthread_disable_ints

@ Decrement the semaphore, thus enabling context switches if it reaches 0
@ May be called from USR or SVC mode
	NAME	__pthread_enable_ints
__pthread_enable_ints:
	PIC_LOAD a1

	LDR	a2, .L1			@=__ul_global
 PICEQ "LDR	a2, [a1, a2]"
	LDR	a2, [a2, #GBL_PTH_CALLEVERY_RMA]
	LDR	a1, [a2, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE]
#if __UNIXLIB_PARANOID
	CMP	a1, #0
	ADRLE	a1, semazero
	BLE	__pthread_fatal_error
#endif
	SUB	a1, a1, #1
	STR	a1, [a2, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE]
	MOV	pc, lr
.L1:
	WORD	__ul_global
#if __UNIXLIB_PARANOID
semazero:
	.asciz	"__pthread_enable_ints called with semaphore already 0 or lower"
	.align
#endif
	DECLARE_FUNCTION __pthread_enable_ints

@ Similar to __pthread_disable_ints, but alters the caller's
@ stack frame so __pthread_unprotect_unsafe is called
@ when the caller returns
	NAME	__pthread_protect_unsafe
__pthread_protect_unsafe:
#if __UNIXLIB_PARANOID
	@ We can't do much without a stack frame
	CMP	fp, #0
	ADREQ	a1, noframe
	BEQ	__pthread_fatal_error
#endif

	PIC_LOAD a1

	LDR	a4, .L2				@ =__ul_global
 PICEQ "LDR	a4, [a1, a4]"
	LDR	ip, [a4, #GBL_PTH_CALLEVERY_RMA]
#ifdef __ARM_EABI__
	ADD	a1, ip, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE
#else
	LDR	a2, [a4, #GBL_CPU_FLAGS]
	ADD	a1, ip, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE
	TST	a2, #__CPUCAP_HAVE_SWP
	@ From this point onwards we cannot be interrupted by the callback
	BEQ	0f

	MOV	a2, #1
	SWP	a3, a2, [a1]
	CMP	a3, #0
	BEQ	1f
	STR	a3, [a1, #0]	@ Restore original value
	@ Return, as if ints are disabled on entry to the
	@ calling function then they should not be reenabled
	@ until the calling function has returned
	MOV	pc, lr
#endif
0:
	LDREX	a3, [a1]
	CMP	a3, #0
	MOVEQ	ip, #1
	MOVNE	ip, a3		@ Keep original value
	STREX	a2, ip, [a1]
	CMP	a2, #0
	BNE	0b

	CMP	a3, #0
	MOVNE	pc, lr
1:

#if __UNIXLIB_PARANOID
	LDR	a3, [a4, #GBL_PTH_RETURN_ADDRESS]
	CMP	a3, #0
	ADRNE	a1, return_notempty
	BNE	__pthread_fatal_error
#endif
#ifdef __ARM_EABI__
	LDR	a1, [fp, #0]
#elif defined(__clang__)
	@ Clang and GCC set fp to slightly different positions within the stack frame.
	LDR	a1, [fp, #4]	@ Load calling function's return address
#else
	LDR	a1, [fp, #-4]	@ Load calling function's return address
#endif
	STR	a1, [a4, #GBL_PTH_RETURN_ADDRESS]
	ADR	a2, __pthread_unprotect_unsafe
	@ Alter calling function's return address to point
	@ to __pthread_unprotect_unsafe
#ifdef __ARM_EABI__
	STR	a2, [fp, #0]
#elif defined(__clang__)
	STR	a2, [fp, #4]
#else
	STR	a2, [fp, #-4]
#endif

	MOV	pc, lr
.L2:
	WORD	__ul_global

#if __UNIXLIB_PARANOID
noframe:
	.asciz	"__pthread_protect_unsafe called without an APCS-32 stack frame"
	.align
return_notempty:
	.asciz	"Return address is not empty in __pthread_protect_unsafe"
	.align
#endif
	DECLARE_FUNCTION __pthread_protect_unsafe

@ Similar to __pthread_enable_ints, but return to the saved
@ __ul_global.pthread_return_address.
@ Can corrupt a3-a4,ip,lr but NOT a1 nor a2
	NAME	__pthread_unprotect_unsafe
__pthread_unprotect_unsafe:
	PIC_LOAD a3

	LDR	ip, .L3			@=__ul_global
 PICEQ "LDR	ip, [a3, ip]"
	LDR	lr, [ip, #GBL_PTH_RETURN_ADDRESS]
#if __UNIXLIB_PARANOID
	CMP	lr, #0
	ADREQ	a1, return_unset
	BEQ	__pthread_fatal_error
	MOV	a3, #0
	STR	a3, [ip, #GBL_PTH_RETURN_ADDRESS]
#endif

	@ Last use of ip as __ul_global before loading it as pthread_callevery_rma pointer
	LDR	a4, [ip, #GBL_PTH_CALLBACK_MISSED]
	LDR	ip, [ip, #GBL_PTH_CALLEVERY_RMA]

#if __UNIXLIB_PARANOID
	LDR	a3, [ip, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE]
	CMP	a3, #1
	ADRNE	a1, bad_semaphore
	BNE	__pthread_fatal_error
#endif

	MOV	a3, #0
	STR	a3, [ip, #PTHREAD_CALLEVERY_RMA_WORKSEMAPHORE]

	@ Check if callback missed
	CMP	a4, #0
	MOVEQ	pc, lr	@ No callback occured while ints were disabled

	LDR	a4, [ip, #PTHREAD_CALLEVERY_RMA_CALLBACK_SEMAPHORE]
	CMP	a4, #0
	MOVNE	pc, lr	@ Don't yield if we're in the middle of a context switch

	@ A callback occured whilst ints were disabled, so yield to
	@ avoid hogging the processor.  We need to protect a1 and a2 here
	@ as these can contain the return data from a function.
	STMFD	sp!, {a1, a2, lr}
	BL	pthread_yield
	LDMFD	sp!, {a1, a2, pc}
.L3:
	WORD	__ul_global

#if __UNIXLIB_PARANOID
bad_semaphore:
	.asciz	"Semaphore not set correctly in __pthread_unprotect_unsafe"
	.align
return_unset:
	.asciz	"Return address unset in __pthread_unprotect_unsafe"
	.align
#endif
	DECLARE_FUNCTION __pthread_unprotect_unsafe

	.end
