;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_kosfile.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; _kernel_osfile (int op, char *name, _kernel_osfile_block *inout)
	EXPORT	|_kernel_osfile|
	NAME	_kernel_osfile
|_kernel_osfile|
	; don't need v3, but it can be set with OS_File 20-23
	STMFD	sp!, {v1-v3, lr}
	MOVS	ip, a3
	LDMNEIA ip, {a3, a4, v1, v2}
	SWI	XOS_File
	STMNEIA ip, {a3, a4, v1, v2}
	stackreturn	VC, "v1-v3, pc"
	BL	|__seterr|
	MOV	a1, #-2
	stackreturn	AL, "v1-v3, pc"

	END
