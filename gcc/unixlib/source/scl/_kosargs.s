;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosargs.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_osargs (int op, unsigned handle, int arg)
	EXPORT	|_kernel_osargs|
	NAME	_kernel_osargs
|_kernel_osargs|
	STMFD	sp!, {lr}
	ORR	ip, a1, a2	; ip = 0 iff op = handle = 0
	SWI	XOS_Args
	BVS	error
	CMP	ip, #0
	MOVNE	a1, a3
	stackreturn	AL, "pc"
error
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	VS, "pc"

	END
