;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/pthread/_exit.s,v $
; $Date: 2002/12/15 13:16:55 $
; $Revision: 1.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

; Written by Alex Waugh

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__alloca_thread_free_all|,WEAK
	IMPORT	|___arm_alloca_thread_free_all|,WEAK

	EXPORT	|__pthread_exit|

;
; Call alloca thread free functions as necessary
	NAME	__pthread_exit
|__pthread_exit|
	STMFD	sp!, {lr}
	LDR	a1, =|__alloca_thread_free_all|
	ADR	lr, |__pthread_exit_l1|
	TEQ	a1, #0
	MOVNE	pc, a1
|__pthread_exit_l1|
	LDR	a1, =|___arm_alloca_thread_free_all|
	LDMFD	sp!, {lr}
	TEQ	a1, #0
	MOVNE	pc, a1
	return	AL, pc, lr

	END
