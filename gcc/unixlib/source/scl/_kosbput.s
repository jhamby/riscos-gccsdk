;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosbput.s,v $
; $Date: 2002/09/24 21:02:37 $
; $Revision: 1.3 $
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
	LDMVCFD	sp!, {pc}
	BL	|__seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
