;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/sys/s/_sigjmp,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__sigsetjmp_helper|
	IMPORT	|__siglongjmp_helper|
	IMPORT	setjmp
	IMPORT	longjmp

	EXPORT	sigsetjmp
sigsetjmp
	; It's best to preserve these registers.
	STMFD	sp!, {a1, a2, ip, lr}
	BL	|__sigsetjmp_helper|
	LDMFD	sp!, {a1, a2, ip, lr}
	B	setjmp

	EXPORT	siglongjmp
	; siglongjmp (sigjmp_buf state, int value)
siglongjmp
	; Preserve state and value
	; __siglongjmp_helper will update the process's blocked signal
	; mask, if it was saved by sigsetjmp. If there are any pending
	; signals unblocked, then these will be delivered.
	STMFD	sp!, {a1, a2}
	BL	|__siglongjmp_helper|
	LDMFD	sp!, {a1, a2}
	B	longjmp

	END
