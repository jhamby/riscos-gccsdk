;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/unixlib/source/sys/s/_mman,v $
; $Date: 2000/12/21 15:09:13 $
; $Revision: 1.3 $
; $State: Exp $
; $Author: admin $
;
;----------------------------------------------------------------------------
; sys/_mman.s: Written by Peter Burwood, June 1997

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|,CODE,READONLY

	; Copy a3 bytes from a2 to a1. a3 is guaranteed a multiple
	; of the page size so we perform a fast multiple register
	; copy is a slightly unrolled loop, copying 128 bytes per loop.
	; It is also guaranteed that the source and destination areas
	; do not overlap, so we don't have to worry whether to copy up
	; or copy down.
	EXPORT	|__mmap_page_copy|
|__mmap_page_copy|
	STMFD	sp!,{v1-v6,lr}
|__mmap_page_copy_l1|
	LDMIA	a2!,{a4,v1,v2,v3,v4,v5,v6,ip}
	STMIA	a1!,{a4,v1,v2,v3,v4,v5,v6,ip}
	LDMIA	a2!,{a4,v1,v2,v3,v4,v5,v6,ip}
	STMIA	a1!,{a4,v1,v2,v3,v4,v5,v6,ip}
	LDMIA	a2!,{a4,v1,v2,v3,v4,v5,v6,ip}
	STMIA	a1!,{a4,v1,v2,v3,v4,v5,v6,ip}
	LDMIA	a2!,{a4,v1,v2,v3,v4,v5,v6,ip}
	STMIA	a1!,{a4,v1,v2,v3,v4,v5,v6,ip}
	SUBS	a3,a3,#8*4*4
	BNE	|__mmap_page_copy_l1|
	stackreturn	AL, "v1-v6, pc"

	END
