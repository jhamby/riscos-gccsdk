;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosbput.s,v $
; $Date: 2000/07/15 14:52:29 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
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
