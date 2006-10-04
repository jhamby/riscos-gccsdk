; _sockstat
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_sockstat|
	NAME	_sockstat
|_sockstat|
	NetSWIsimple	XSocket_Stat

	END
