;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_kosbyte,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; int _kernel_osbyte (int op, int x, int y)
	EXPORT	|_kernel_osbyte|
	IMPORT	|__seterr|
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
