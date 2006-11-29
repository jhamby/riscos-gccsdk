; _accept
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_accept|
	NAME	_accept
|_accept|
	NetSWIsimple	XSocket_Accept_1

	END
