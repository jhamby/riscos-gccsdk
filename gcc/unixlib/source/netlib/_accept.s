;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_accept.s,v $
; $Date: 2003/05/13 22:59:47 $
; $Revision: 1.6 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_accept|
	NAME	_accept
|_accept|
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWIsimple	XSocket_Accept
	|
	NetSWIsimple	XSocket_Accept_1
	]

	END
