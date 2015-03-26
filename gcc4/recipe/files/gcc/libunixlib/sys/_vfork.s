@ vfork, fork
@ Copyright (c) 2000-2012 UnixLib Developers

#include "internal/asm_dec.s"

	.text

@ vfork is similar to fork.
@
@ fork makes a complete copy of the calling process's address space
@ and allows both the parent and child to execute independently.
@ vfork does not make this copy.
@
@ A child process created with vfork shares its parent's address
@ space until it calls exit or one of the exec functions. In the
@ meantime, the parent process suspends execution.

	.global	vfork
	NAME	vfork
vfork:
	MOV	a1, #0
	B	fork_common
	DECLARE_FUNCTION vfork

	.global	fork
	NAME	fork
fork:
	MOV	a1, #1
fork_common:
	STMFD	sp!, {a1, lr}
	SUB	sp, sp, #2*4
	ADD	a2, sp, #4	@ Space for __ul_global.sulproc->sul_fork
	MOV	a3, sp		@ Space for __ul_global.sulproc->pid
	BL	__fork_pre
	TEQ	a1, #0
	ADDNE	sp, sp, #3*4
	LDMNEFD	sp!, {pc}

	LDMFD	sp!, {a1-a3, lr}
	@ a1 = child pid
	@ a2 = ptr to child's __ul_global.sulproc->sul_fork future value.
	@ a3 = isfork
	@ lr = return address fork()/vfork()

	@ FIXME: Don't need to save v5 anymore
 PICEQ "STMFD	sp!, {v4-v5}"

 PICEQ "LDR	v4, =__GOTT_BASE__"
 PICEQ "LDR	v4, [v4, #0]"
 PICEQ "LDR	v4, [v4, #__GOTT_INDEX__]"	@ v4 = GOT ptr

	MOV	ip, a2
	@ Save lr as we can't use the stack as it may be corrupted
	@ by the time we return as the parent
	LDR	a2, .L0			@=__saved_lr
 PICEQ "LDR	a2, [v4, a2]"
 PICEQ "LDMFD	sp!, {a4, v5}"
 PICEQ "STMIA	a2!, {a4, v5}"
	STMIA	a2!, {a3, lr}
 
#if !defined(__SOFTFP__) && defined(__VFP_FP__)
	@ Prepare active VFP context for the fork by nonlazily deactivating it
	STMFD	sp!, {a1, a2}
	MOV	a1, #0
	MOV	a2, #0
	SWI	VFPSupport_ChangeContext
	LDR	a2, [sp, #4]
	STR	a1, [a2]	@ Remember which context was active
	LDR	a1, [sp], #8
#endif

	LDR	a2, .L0+4		@=__ul_global
 PICEQ "LDR	a2, [v4, a2]"
	ADD	a2, a2, #GBL_SULPROC	@ a2 = &__ul_global.sulproc
	TEQ	a3, #0
	LDRNE	a3, .L0+8		@=__ul_memory
 PICEQ "LDRNE	a3, [v4, a3]"
	LDRNE	a4, [a3, #MEM_STACK]
	LDRNE	a3, [a3, #MEM_STACK_LIMIT]
	MOVEQ	a4, #0

	@ Now call sul_fork()
	@ a1 = child pid
	@ a2 = &__ul_global.sulproc
	@ a3 = __ul_memory.stack_limit (fork) / 0 (vfork)
	@ a4 = __ul_memory.stack (fork) / 0 (vfork)
	MOV	lr, pc
	MOV	pc, ip

	@ The PIC register should have been restored on return from the
	@ child process.

	@ Tail-call __fork_post to do the remaining work
	LDR	a3, .L0			@=__saved_lr
 PICEQ "LDR	a3, [v4, a3]"
 PICEQ "LDMIA	a3!, {v4, v5}"
	LDMIA	a3!, {a2, lr}
 
#if !defined(__SOFTFP__) && defined(__VFP_FP__)
	@ Reactivate the VFP context
	STMFD	sp!, {a1-a2}
	LDR	a1, [a3]
	MOV	a2, #1
	SWI	VFPSupport_ChangeContext
	LDMFD	sp!, {a1-a2}
#endif

	B	__fork_post
.L0:
	WORD	__saved_lr
	WORD	__ul_global
	WORD	__ul_memory
	DECLARE_FUNCTION fork

	.section ".bss"

	.align

__saved_lr:
 PICNE ".space	8"
 PICEQ ".space	16"
#if !defined(__SOFTFP__) && defined(__VFP_FP__)
	.space	4	@ Extra space for remembering the active VFP context
#endif
	DECLARE_OBJECT __saved_lr

	.end
