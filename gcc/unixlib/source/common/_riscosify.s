;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/common/_riscosify.s,v $
; $Date: 2003/04/07 20:11:28 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: alex $
;
;----------------------------------------------------------------------------

        GET     clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	IMPORT	|__riscosify_control|, WEAK

	;int __get_riscosify_control (void)
	EXPORT	|__get_riscosify_control|
	NAME	"__get_riscosify_control"
|__get_riscosify_control|
	LDR	a1, =|__riscosify_control|
	TEQ	a1, #0
	LDREQ	a1, =|__riscosify_control_internal|
	LDR	a1, [a1, #0]
	return	AL, pc, lr

	;void __set_riscosify_control(int __riscosify_flags)
	EXPORT	|__set_riscosify_control|
	NAME	"__set_riscosify_control"
|__set_riscosify_control|
	LDR	a2, =|__riscosify_control|
	TEQ	a2, #0
	LDREQ	a2, =|__riscosify_control_internal|
	STR	a1, [a1, #0]
	return	AL, pc, lr

	AREA	|C$$data|, DATA

|__riscosify_control_internal|
	DCD	0

	END
