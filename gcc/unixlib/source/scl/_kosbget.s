;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_kosbget,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; int _kernel_osbget (int handle)
	IMPORT	|__seterr|
	EXPORT	|_kernel_osbget|
|_kernel_osbget|
	STMFD	sp!, {lr}
	MOV	a2, a1
	SWI	XOS_BGet
	MOVCS	a1, #-1		; We have EOF. Return -1
	stackreturn	CS, "pc"

	BL	|__seterr|
	MOV	a1, #-2		; Error occurred. Return -2
	stackreturn	AL, "pc"

	END
