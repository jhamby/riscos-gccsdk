;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_vfork.s,v $
; $Date: 2005/03/04 20:59:06 $
; $Revision: 1.12 $
; $State: Exp $
; $Author: alex $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__fork_pre|
	IMPORT	|__fork_post|
	IMPORT	|__ul_memory|
	IMPORT	|__proc|

; vfork is similar to fork.
;
; fork makes a complete copy of the calling process's address space
; and allows both the parent and child to execute independently.
; vfork does not make this copy.
;
; A child process created with vfork shares its parent's address
; space until it calls exit or one of the exec functions. In the
; meantime, the parent process suspends execution.

	EXPORT	vfork
	EXPORT  fork
	NAME	vfork
vfork
	MOV	a1, #0
	B	fork_common
fork
	MOV	a1, #1
fork_common
	STMFD	sp!, {a1,lr}
	SUB	sp, sp, #2*4
	ADD	a2, sp, #4	; Space for __proc->sul_fork
	MOV	a3, sp		; Space for __proc->pid
	BL	|__fork_pre|
	TEQ	a1, #0
	ADDNE	sp, sp, #3*4
	LDMNEFD	sp!, {pc}

	LDMFD	sp!, {a1-a3,lr}
	MOV	ip, a2
	; Save lr as we can't use the stack as it may be corrupted
	; by the time we return as the parent
	LDR	a2, =|__saved_lr|
	STMIA	a2, {a3, lr}
	LDR	a2, =|__proc|
	TEQ	a3, #0
	LDRNE	a3, =|__ul_memory|
	LDRNE	a4, [a3, #MEM_UNIXLIB_STACK]
	LDRNE	a3, [a3, #MEM_UNIXLIB_STACK_LIMIT]
	MOVEQ	a4, #0
	; Now call sul_fork()
	MOV	lr, pc
	MOV	pc, ip

	; Tail-call __fork_post to do the remaining work
	LDR	lr, =|__saved_lr|
	LDMIA	lr, {a2, lr}
	B	|__fork_post|

	AREA	|C$$zidata|, DATA, NOINIT
|__saved_lr|
	%	8

	END
