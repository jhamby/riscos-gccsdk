;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_divsi3.s,v $
; $Date: 2001/01/29 15:10:21 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

; GCC compatability file

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|_kernel_sdiv|

	EXPORT	|__divsi3|
	EXPORT	|__divmodsi4|
	NAME	__divsi3
|__divsi3|
|__divmodsi4|
	MOV	a3, a1
	MOV	a1, a2
	MOV	a2, a3
	B	|_kernel_sdiv|

	END
