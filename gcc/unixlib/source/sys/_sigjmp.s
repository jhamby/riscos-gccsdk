;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_sigjmp.s,v $
; $Date: 2001/01/29 15:10:21 $
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

	; sigsetjmp (sigjmp_buf state, int value)
	EXPORT	sigsetjmp
	NAME	sigsetjmp
sigsetjmp
	; It's best to preserve these registers.
	STMFD	sp!, {a1, lr}
	BL	|__sigsetjmp_helper|
	LDMFD	sp!, {a1, lr}
	B	setjmp

	; siglongjmp (sigjmp_buf state, int value)
	EXPORT	siglongjmp
	NAME	siglongjmp
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
