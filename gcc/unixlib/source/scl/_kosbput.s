;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosbput.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; int _kernel_osbput (int ch, int handle)
	EXPORT	|_kernel_osbput|
	NAME	_kernel_osbput
|_kernel_osbput|
	STMFD	sp!, {lr}
	SWI	XOS_BPut
	stackreturn	VC, "pc"
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "pc"

	END
