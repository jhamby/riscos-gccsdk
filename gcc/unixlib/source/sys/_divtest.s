;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_divtest.s,v $
; $Date: 2000/07/15 14:52:34 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$code|, CODE, READONLY

	EXPORT	|x$divtest|
	EXPORT	|__rt_divtest|
	IMPORT	raise

|__rt_divtest|
|x$divtest|
	CMP	a1, #0
	return	NE, pc, lr
	ADR	a1, divide_by_zero
	SWI	XOS_Write0
	MOV	a1, #SIGABRT
	B	raise

divide_by_zero
	DCB	"Divide by zero", 13, 10, 0

	END
