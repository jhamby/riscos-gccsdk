;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_getpeerna.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.7 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

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
