;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/_coredump.s,v $
; $Date: 2004/11/28 21:31:34 $
; $Revision: 1.8 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|__backtrace_getfp|
	NAME	__backtrace_getfp
|__backtrace_getfp|
	MOV	a1, fp
	MOV	pc, lr

	END
