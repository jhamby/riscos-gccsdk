;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/_coredump.s,v $
; $Date: 2004/02/23 16:07:29 $
; $Revision: 1.7 $
; $State: Exp $
; $Author: peter $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|__backtrace_getfp|
|__backtrace_getfp|
	MOV	a1, fp
	MOV	pc, lr

	END
