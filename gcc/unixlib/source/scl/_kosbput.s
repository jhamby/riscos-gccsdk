;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_kosbput,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; int _kernel_osbput (int ch, int handle)
	IMPORT	|__seterr|
	EXPORT	|_kernel_osbput|
|_kernel_osbput|
	STMFD	sp!, {lr}
	SWI	XOS_BPut
	stackreturn	VC, "pc"
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "pc"

	END
