;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/sys/s/_divtest,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
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
