;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_divtest.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	raise

	EXPORT	|__rt_divtest|
	EXPORT	|x$divtest|
	NAME	__rt_divtest
|__rt_divtest|
|x$divtest|
	TEQ	a1, #0
	MOVNE	pc, lr
	MOV	a1, #SIGFPE
	B	raise

	END
