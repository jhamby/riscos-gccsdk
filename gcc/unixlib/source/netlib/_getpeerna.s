; _getpeername
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_getpeername|
	NAME	_getpeername
|_getpeername|
	NetSWIsimple0	XSocket_Getpeername_1

	END
