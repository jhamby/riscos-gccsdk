;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_sendmsg.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.7 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	EXPORT	|_sendmsg|
	NAME	_sendmsg
|_sendmsg|
	[ __UNIXLIB_COMPAT_INET4 > 0
	NetSWIsimple	XSocket_Sendmsg
	|
	NetSWIsimple	XSocket_Sendmsg_1
	]

	END
