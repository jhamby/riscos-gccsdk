;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_modsi3.s,v $
; $Date: 2002/09/24 21:02:38 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

; GCC compatability file

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|_kernel_sdiv|

	EXPORT	|__modsi3|
	NAME	__modsi3
|__modsi3|
	STMFD	sp!, {lr}
	MOV	a3, a1
	MOV	a1, a2
	MOV	a2, a3
	BL	|_kernel_sdiv|
	MOV	a1, a2
	LDMFD	sp!, {pc}

	END
