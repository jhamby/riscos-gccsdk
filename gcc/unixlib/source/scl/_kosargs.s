;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/scl/s/_kosargs,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_osargs (int op, unsigned handle, int arg)
	IMPORT	|__seterr|
	EXPORT	|_kernel_osargs|
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
