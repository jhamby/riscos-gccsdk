;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosbput.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; int _kernel_osbput (int ch, int handle)
	EXPORT	|_kernel_osbput|
	NAME	_kernel_osbput
|_kernel_osbput|
	SWI	XOS_BPut
	MOVVS	a1, #-2
	MOVVC	a1, #0
	MOV	pc, lr

	END
