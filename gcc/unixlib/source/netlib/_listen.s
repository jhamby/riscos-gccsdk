; _listen
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_listen|
	NAME	_listen
|_listen|
	NetSWIsimple0	XSocket_Listen

	END
