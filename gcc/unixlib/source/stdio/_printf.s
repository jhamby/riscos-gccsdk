;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/stdio/_printf.s,v $
; $Date: 2000/07/15 14:52:32 $
; $Revision: 1.1.1.1 $
; $State: Exp $
; $Author: nick $
;
;----------------------------------------------------------------------------

	AREA	|C$$code|,CODE,READONLY

	IMPORT	printf
	EXPORT	|_printf|
|_printf|
	B	printf

	IMPORT	fprintf
	EXPORT	|_fprintf|
|_fprintf|
	B	fprintf

	IMPORT	sprintf
	EXPORT	|_sprintf|
|_sprintf|
	B	sprintf

	IMPORT	vfprintf
	EXPORT	|_vfprintf|
|_vfprintf|
	B	vfprintf

	END
