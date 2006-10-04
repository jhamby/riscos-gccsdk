; __backtrace_getfp
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|__backtrace_getfp|
	NAME	__backtrace_getfp
|__backtrace_getfp|
	MOV	a1, fp
	MOV	pc, lr

	END
