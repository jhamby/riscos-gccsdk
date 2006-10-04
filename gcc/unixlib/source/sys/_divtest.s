; __rt_divtest, x$divtest
; Copyright (c) 2000-2006 UnixLib Developers

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
