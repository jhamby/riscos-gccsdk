;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/sys/s/_udivsi3,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

; GCC compatability file

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|_kernel_udiv|
	EXPORT	|__udivsi3|
	EXPORT	|__udivmodsi4|

|__udivsi3|
|__udivmodsi4|
	MOV	a3, a1
	MOV	a1, a2
	MOV	a2, a3
	B	|_kernel_udiv|

	END
