;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/string/_ffs.s,v $
; $Date: 2004/10/17 16:24:44 $
; $Revision: 1.1 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

; Fast strlen Contriubted by Adrian Lees
; Determine the length of a NUL-terminated string
;
; entry	a1 -> NUL-terminated string
; exit	a1 = length excluding terminator

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|strlen|
|strlen|
	LDR	ip,[a1]		;read 1st word, possibly rotated
	MOV	a4,#1
	MOV	a2,a1
	ORR	a4,a4,a4,LSL #8
	ANDS	a3,a1,#3
	ORR	a4,a4,a4,LSL #16
	BNE	strlen_align

|strlen_lp|
	SUBS	a3,ip,a4	;3 instruction test for zero byte in word
	EORCS	a3,a3,ip
	BICCSS	a3,a4,a3
	LDREQ	a3,[a2,#4]!	;safe to read next word now
	BNE	strlen_end

	SUBS	ip,a3,a4	;3 instruction test for zero byte in word
	EORCS	ip,ip,a3
	BICCSS	ip,a4,ip
	LDREQ	ip,[a2,#4]!	;safe to read next word now
	BNE	strlen_end_a3

	SUBS	a3,ip,a4	;3 instruction test for zero byte in word
	EORCS	a3,a3,ip
	BICCSS	a3,a4,a3
	LDREQ	a3,[a2,#4]!	;safe to read next word now
	BNE	strlen_end

	SUBS	ip,a3,a4	;3 instruction test for zero byte in word
	EORCS	ip,ip,a3
	BICCSS	ip,a4,ip
	LDREQ	ip,[a2,#4]!	;safe to read next word now
	BEQ	strlen_lp

|strlen_end_a3|
	MOV	ip,a3

|strlen_end|
	SUB	a1,a2,a1	;excludes last word read
	TST	ip,#&FF
	ADDNE	a1,a1,#1
	TSTNE	ip,#&FF00
	ADDNE	a1,a1,#1
	TSTNE	ip,#&FF0000
	ADDNE	a1,a1,#1
	MOV	pc,lr

|strlen_align|
	CMP	a3,#2
	BHI	strlen3
	BEQ	strlen2
	TST	ip,#&FF
	MOVEQ	a1,#0
	MOVEQ	pc,lr
	MOV	ip,ip,LSR #8
	ADD	a2,a2,#1
|strlen2|
	TST	ip,#&FF
	MOVNE	ip,ip,LSR #8
	ADDNE	a2,a2,#1
|strlen3|
	TSTNE	ip,#&FF
	LDRNE	ip,[a2,#1]!
	SUBEQ	a1,a2,a1
	MOVEQ	pc,lr
	B	strlen_lp

	END
