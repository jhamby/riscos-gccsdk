; _readv
; Copyright (c) 2000-2006 UnixLib Developers

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_readv|
	NAME	_readv
|_readv|
	NetSWIsimple	XSocket_Readv

	END
