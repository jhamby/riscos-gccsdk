;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_udivsi3.s,v $
; $Date: 2001/01/29 15:10:21 $
; $Revision: 1.2 $
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
	NAME	__udivmodsi4
|__udivsi3|
|__udivmodsi4|
	MOV	a3, a1
	MOV	a1, a2
	MOV	a2, a3
	B	|_kernel_udiv|

	END
