;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosbyte.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; int _kernel_osbyte (int op, int x, int y)
	EXPORT	|_kernel_osbyte|
	NAME	_kernel_osbyte
|_kernel_osbyte|
	STMFD	sp!, {lr}
	SWI	XOS_Byte
	BVS	|err|
	AND	a1, a2, #&FF
	AND	a3, a3, #&FF
	ORR	a1, a1, a3, LSL #8
	; If carry set, then third byte = 1
	ORRCS	a1, a1, #1:SHL:16
	stackreturn	AL, "pc"
|err|
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "pc"

	END
