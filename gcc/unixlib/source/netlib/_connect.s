; _connect
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_connect|
	NAME	_connect
|_connect|
	NetSWIsimple0	XSocket_Connect

	END
