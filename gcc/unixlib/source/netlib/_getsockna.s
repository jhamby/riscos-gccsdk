; _getsockname
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_getsockname|
	NAME	_getsockname
|_getsockname|
	NetSWIsimple0	XSocket_Getsockname_1

	END
