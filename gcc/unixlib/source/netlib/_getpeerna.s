;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_getpeerna.s,v $
; $Date: 2003/05/13 22:59:47 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_getpeername|
	NAME	_getpeername
|_getpeername|
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWIsimple0	XSocket_Getpeername
	|
	NetSWIsimple0	XSocket_Getpeername_1
	]

	END
