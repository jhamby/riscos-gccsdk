;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/sys/s/_divsi3,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.3 $
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

|__divsi3|
|__divmodsi4|
	MOV	a3, a1
	MOV	a1, a2
	MOV	a2, a3
	B	|_kernel_sdiv|

	END
