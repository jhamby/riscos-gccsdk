; _sclose
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_sclose|
	NAME	_sclose
|_sclose|
	NetSWIsimple0	XSocket_Close


	END
