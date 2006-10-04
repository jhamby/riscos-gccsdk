; _getpeername
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_getpeername|
	NAME	_getpeername
|_getpeername|
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWIsimple0	XSocket_Getpeername
	|
	NetSWIsimple0	XSocket_Getpeername_1
	]

	END
