;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/scl/_koscli.s,v $
; $Date: 2004/09/23 22:16:39 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__seterr|

	; int _kernel_oscli (const char *cli)
	EXPORT	|_kernel_oscli|
	NAME	_kernel_oscli
|_kernel_oscli|
	STMFD	sp!, {lr}
	SWI	XOS_CLI
	MOVVC	a1, #1
	LDMVCFD	sp!, {pc}
	BL	|__seterr|
	MOV	a1, #-2
	LDMFD	sp!, {pc}

	END
