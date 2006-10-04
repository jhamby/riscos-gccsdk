; _accept
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_accept|
	NAME	_accept
|_accept|
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWIsimple	XSocket_Accept
	|
	NetSWIsimple	XSocket_Accept_1
	]

	END
