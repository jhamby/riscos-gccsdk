;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/sys/s/_umodsi3,v $
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
	EXPORT	|__umodsi3|

|__umodsi3|
	STMFD	sp!, {lr}
	MOV	a3, a1
	MOV	a1, a2
	MOV	a2, a3
	BL	|_kernel_udiv|
	MOV	a1, a2
	stackreturn	AL, "pc"

	END
