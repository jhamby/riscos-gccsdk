;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/stdio/s/_printf,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.4 $
; $State: Exp $
; $Author: admin $
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
