; _writev
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_writev|
	NAME	_writev
|_writev|
	NetSWIsimple	XSocket_Writev

	END
