;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosgbpb.s,v $
; $Date: 2000/07/15 14:52:29 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	; _kernel_osgbpb (int op, unsigned handle, _kernel_osgbpb_block *inout)
	IMPORT	|__seterr|
	EXPORT	|_kernel_osgbpb|
|_kernel_osgbpb|
	STMFD	sp!, {v1, v2, v3, v4, lr}
	MOV	v4, a3
	LDMIA	v4, {a3, a4, v1, v2, v3}
	SWI	XOS_GBPB
	STMIA	v4, {a3, a4, v1, v2, v3}
	MVNCS	a1, #0
	stackreturn	CS, "v1, v2, v3, v4, pc"

	BL	|__seterr|
	MVN	a1, #1
	stackreturn	AL, "v1, v2, v3, v4, pc"

	END
