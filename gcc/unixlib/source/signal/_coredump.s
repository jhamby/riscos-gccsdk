;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/signal/_coredump.s,v $
; $Date: 2003/06/23 20:33:03 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT |__backtrace_getfp|
|__backtrace_getfp|
	MOV	a1, fp
	MOV	pc, lr

	END
