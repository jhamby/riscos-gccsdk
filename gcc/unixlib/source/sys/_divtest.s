;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_divtest.s,v $
; $Date: 2001/09/11 13:05:55 $
; $Revision: 1.2.2.1 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

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
