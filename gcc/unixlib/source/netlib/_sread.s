; _sread
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_sread|
	NAME	_sread
|_sread|
	NetSWIsimple	XSocket_Read


	END
