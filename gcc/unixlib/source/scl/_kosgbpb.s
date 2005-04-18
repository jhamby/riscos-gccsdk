;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosgbpb.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__ul_seterr|

	; _kernel_osgbpb (int op, unsigned handle, _kernel_osgbpb_block *inout)
	EXPORT	|_kernel_osgbpb|
	NAME	_kernel_osgbpb
|_kernel_osgbpb|
	STMFD	sp!, {v1, v2, v3, v4, lr}
	MOV	v4, a3
	LDMIA	v4, {a3, a4, v1, v2, v3}
	SWI	XOS_GBPB
	STMIA	v4, {a3, a4, v1, v2, v3}
	MVNCS	a1, #0
	LDMCSFD	sp!, {v1, v2, v3, v4, pc}

	MOV	a2, #0
	BL	|__ul_seterr|
	MVN	a1, #1
	LDMFD	sp!, {v1, v2, v3, v4, pc}

	END
