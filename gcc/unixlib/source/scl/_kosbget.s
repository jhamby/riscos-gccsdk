;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosbget.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; int _kernel_osbget (int handle)
	EXPORT	|_kernel_osbget|
	NAME	_kernel_osbget
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
