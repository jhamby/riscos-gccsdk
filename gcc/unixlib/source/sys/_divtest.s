;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_divtest.s,v $
; $Date: 2002/09/24 21:02:38 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	raise

	EXPORT	|__rt_divtest|
	EXPORT	|x$divtest|
	NAME	__rt_divtest
|__rt_divtest|
|x$divtest|
	TEQ	a1, #0
	MOVNE	pc, lr
	ADR	a1, divide_by_zero
	SWI	XOS_Write0
	MOV	a1, #SIGABRT
	B	raise

divide_by_zero
	DCB	"Divide by zero", 13, 10, 0

	END
