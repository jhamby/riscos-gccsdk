;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/_sendtosm.s,v $
; $Date: 2001/01/29 15:10:20 $
; $Revision: 1.2 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	IMPORT	|__net_error_simple_entry|

	EXPORT	|_sendtosm|
	NAME	_sendtosm
|_sendtosm|
	NetSWIsimple	XSocket_Sendtosm

	END
