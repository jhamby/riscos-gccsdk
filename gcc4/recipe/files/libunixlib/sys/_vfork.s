@ vfork, fork
@ Copyright (c) 2000-2007 UnixLib Developers

#include "unixlib/asm_dec.s"

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
	.global	fork
	NAME	vfork
vfork:
	MOV	a1, #0
	B	fork_common
fork:
	MOV	a1, #1
fork_common:
	STMFD	sp!, {a1, lr}
	SUB	sp, sp, #2*4
	ADD	a2, sp, #4	@ Space for __proc->sul_fork
	MOV	a3, sp		@ Space for __proc->pid
	BL	__fork_pre
	TEQ	a1, #0
	ADDNE	sp, sp, #3*4
	LDMNEFD	sp!, {pc}

	LDMFD	sp!, {a1-a3, lr}
	@ a1 = __proc->pid
	@ a2 = __proc->sul_fork
	@ a3 = isfork
	@ lr = return address fork()/vfork()
 PICEQ "STMFD	sp!, {v4-v5}"

 PICEQ "LDR	v4, .L0+12"
.LPIC0:
 PICEQ "ADD	v4, pc, v4"		@ v4 = _GLOBAL_OFFSET_TABLE_+4
 PICEQ "LDMIA	v4, {v4, v5}"		@ v4 = Object index, v5 = GOT ptr array location
 PICEQ "LDR	v5, [v5, #0]"		@ v5 = GOT ptr array
 PICEQ "LDR	v4, [v5, v4, LSL#4]"	@ v5 = GOT ptr

	MOV	ip, a2
	@ Save lr as we can't use the stack as it may be corrupted
	@ by the time we return as the parent
	LDR	a2, .L0			@=__saved_lr
 PICEQ "LDR	a2, [v4, a2]"
 PICEQ "LDMFD	sp!, {a4, v5}"
 PICEQ "STMIA	a2!, {a4, v5}"
	STMIA	a2, {a3, lr}

	LDR	a2, .L0+4		@=__proc
 PICEQ "LDR	a2, [v4, a2]"
	TEQ	a3, #0
	LDRNE	a3, .L0+8		@=__ul_memory
 PICEQ "LDRNE	a3, [v4, a3]"
	LDRNE	a4, [a3, #MEM_UNIXLIB_STACK]
	LDRNE	a3, [a3, #MEM_UNIXLIB_STACK_LIMIT]
	MOVEQ	a4, #0

	@ Now call sul_fork()
	MOV	lr, pc
	MOV	pc, ip

	@ The PIC register should have been restored on return from the
	@ child process.

	@ Tail-call __fork_post to do the remaining work
	LDR	lr, .L0			@=__saved_lr
 PICEQ "LDR	lr, [v4, lr]"
 PICEQ "LDMIA	lr!, {v4, v5}"
	LDMIA	lr, {a2, lr}

	B	__fork_post
.L0:
	WORD	__saved_lr
	WORD	__proc
	WORD	__ul_memory
 PICEQ ".word	_GLOBAL_OFFSET_TABLE_-(.LPIC0+4)"
	DECLARE_FUNCTION vfork
	DECLARE_FUNCTION fork

	.section ".bss"
__saved_lr:
 PICNE ".space	8"
 PICEQ ".space	16"
	DECLARE_OBJECT __saved_lr

	.end
