;----------------------------------------------------------------------------
;
; $Source: /usr/local/cvsroot/gccsdk/unixlib/source/sys/_mem.s,v $
; $Date: 2004/09/07 14:05:11 $
; $Revision: 1.5 $
; $State: Exp $
; $Author: joty $
;
;----------------------------------------------------------------------------

	GET	clib/unixlib/asm_dec.s

	AREA	|C$$code|, CODE, READONLY

	EXPORT	|_memset|
	EXPORT	|__memset|
	EXPORT	|memset|
	NAME	memset
|memset|
|__memset|
	MOV	ip, a1
	AND	a2, a2, #&ff
	CMP	a3, #4
	BCC	|__memset_l4|

	ANDS	a4, ip, #3
	BEQ	|_memset|

	RSB	a4, a4, #4
	SUB	a3, a3, a4
	CMP	a4, #2
	STRB	a2, [ip], #1
	STRGEB	a2, [ip], #1
	STRGTB	a2, [ip], #1
|_memset|
	ORR	a2, a2, a2, LSL #8
	ORR	a2, a2, a2, LSL #16
	ANDS	a4, a3, #12
	BEQ	|__memset_l2|

	CMP	a4, #8
	STR	a2, [ip], #4
	STRGE	a2, [ip], #4
	STRGT	a2, [ip], #4
|__memset_l2|
	BICS	a3, a3, #12
	return	EQ, pc, lr
	BICS	a4, a3, #15
	BEQ	|__memset_l5|

	STMFD	sp!, {v1, v2, lr}
	MOV	v1, a2
	MOV	v2, a2
	MOV	lr, a2
	SUB	a3, a3, a4
|__memset_l3|
	STMIA	ip!, {a2, v1, v2, lr}
	SUBS	a4, a4, #32
	STMGEIA ip!, {a2, v1, v2, lr}
	BGT	|__memset_l3|
	LDMFD	sp!, {v1, v2, lr}
|__memset_l4|
	TEQ	a3, #0
	return	EQ, pc, lr
|__memset_l5|
	CMP	a3, #2
	STRB	a2, [ip], #1
	STRGEB	a2, [ip], #1
	STRGTB	a2, [ip], #1
	return	AL, pc, lr


	EXPORT	|_memcpy|
	EXPORT	|_memmove|
	NAME	_memcpy
|_memcpy|
|_memmove|
	TEQ	a1, a2
	TEQNE	a3, #0
	return	EQ, pc, lr
	MOV	ip, a1
	CMP	ip, a2
	BLS	|__memcpy_wd|
	ADDHI	ip, ip, a3
	ADDHI	a2, a2, a3
	B	|__memcpy_wu|

	EXPORT	|__memcpy|
	EXPORT	|memcpy|
	EXPORT	|__memmove|
	EXPORT	|memmove|
	NAME	memcpy
|memcpy|
|__memcpy|
|__memmove|
|memmove|
	TEQ	a1, a2
	TEQNE	a3, #0
	return	EQ, pc, lr
	MOV	ip, a1
	CMP	ip, a2
	ADDGT	ip, ip, a3
	ADDGT	a2, a2, a3
	ANDS	a4, ip, #3
	ANDEQS	a4, a2, #3
	BEQ	|__memcpy_w|
	CMP	a3, #4
	BCC	|__memcpy_b|

|__memcpy_c|				; complex copy
	CMP	ip, a2
	BLS	|__memcpy_cd|

|__memcpy_cu|				; complex copyup - word align ip
	ANDS	a4, ip, #3
	BEQ	|__memcpy_cu_l0|
	SUB	a3, a3, a4
	CMP	a4, #2
	LDRB	a4, [a2, #-1]!
	STRB	a4, [ip, #-1]!
	LDRCSB	a4, [a2, #-1]!
	STRCSB	a4, [ip, #-1]!
	LDRHIB	a4, [a2, #-1]!
	STRHIB	a4, [ip, #-1]!
|__memcpy_cu_l0|
	ANDS	a4, a2, #3
	BEQ	|__memcpy_wu|
	STMFD	sp!, {a1, v1, v2, lr}
	BIC	a2, a2, #3		; word align a2 [1]
	MOV	a4, a4, LSL #3		; a4 = shift1
	RSB	v1, a4, #32		; v1 = shift2
	LDR	a1, [a2]
	ANDS	lr, a3, #12
	BEQ	|__memcpy_cu_l1|
	LDR	v2, [a2, #-4]!
	MOV	a1, a1, LSL v1
	ORR	a1, a1, v2, LSR a4
	STR	a1, [ip, #-4]!
	MOV	a1, v2
	SUBS	lr, lr, #4
	BEQ	|__memcpy_cu_l1|
	LDR	v2, [a2, #-4]!
	MOV	a1, a1, LSL v1
	ORR	a1, a1, v2, LSR a4
	STR	a1, [ip, #-4]!
	MOV	a1, v2
	SUBS	lr, lr, #4
	BEQ	|__memcpy_cu_l1|
	LDR	v2, [a2, #-4]!
	MOV	a1, a1, LSL v1
	ORR	a1, a1, v2, LSR a4
	STR	a1, [ip, #-4]!
	MOV	a1, v2
|__memcpy_cu_l1|
	BICS	a3, a3, #12
	stackreturn	EQ, "a1, v1, v2, pc"
	BICS	lr, a3, #15
	BEQ	|__memcpy_cu_l4|
	STMFD	sp!, {v3, v4, v5}
	MOV	v5, a1
	SUB	a3, a3, lr
|__memcpy_cu_l2|
	LDMDB	a2!, {a1, v2, v3, v4}
	MOV	v5, v5, LSL v1
	ORR	v5, v5, v4, LSR a4
	MOV	v4, v4, LSL v1
	ORR	v4, v4, v3, LSR a4
	MOV	v3, v3, LSL v1
	ORR	v3, v3, v2, LSR a4
	MOV	v2, v2, LSL v1
	ORR	v2, v2, a1, LSR a4
	STMDB	ip!, {v2, v3, v4, v5}
	MOV	v5, a1
	SUBS	lr, lr, #16
	BGT	|__memcpy_cu_l2|
|__memcpy_cu_l3|
	LDMFD	sp!, {v3, v4, v5}
|__memcpy_cu_l4|
	ADD	a2, a2, a4, LSR #3		; reset a2 from [1]
	LDMFD	sp!, {a1, v1, v2, lr}
	CMP	a3, #0
	return	EQ, pc, lr
	B	|__memcpy_bu|

|__memcpy_cd|				; complex copydown - word align ip
	ANDS	a4, ip, #3
	BEQ	|__memcpy_cd_l0|
	RSB	a4, a4, #4
	SUB	a3, a3, a4
	CMP	a4, #2
	LDRB	a4, [a2], #1
	STRB	a4, [ip], #1
	LDRGEB	a4, [a2], #1
	STRGEB	a4, [ip], #1
	LDRGTB	a4, [a2], #1
	STRGTB	a4, [ip], #1
|__memcpy_cd_l0|
	ANDS	a4, a2, #3
	BEQ	|__memcpy_wd|
	STMFD	sp!, {a1, v1, v2, lr}
	BIC	a2, a2, #3		; word align a2 [1]
	MOV	a4, a4, LSL #3		; a4 = shift1
	RSB	v1, a4, #32		; v1 = shift2
	LDR	a1, [a2], #4
	ANDS	lr, a3, #12
	BEQ	|__memcpy_cd_l1|
	LDR	v2, [a2], #4
	MOV	a1, a1, LSR a4
	ORR	a1, a1, v2, LSL v1
	STR	a1, [ip], #4
	MOV	a1, v2
	SUBS	lr, lr, #4
	BEQ	|__memcpy_cd_l1|
	LDR	v2, [a2], #4
	MOV	a1, a1, LSR a4
	ORR	a1, a1, v2, LSL v1
	STR	a1, [ip], #4
	MOV	a1, v2
	SUBS	lr, lr, #4
	BEQ	|__memcpy_cd_l1|
	LDR	v2, [a2], #4
	MOV	a1, a1, LSR a4
	ORR	a1, a1, v2, LSL v1
	STR	a1, [ip], #4
	MOV	a1, v2
|__memcpy_cd_l1|
	BICS	a3, a3, #12
	stackreturn	EQ, "a1, v1, v2, pc"
	BICS	lr, a3, #15
	BEQ	|__memcpy_cd_l4|
	STMFD	sp!, {v3, v4, v5}
	SUB	a3, a3, lr
|__memcpy_cd_l2|
	LDMIA	a2!, {v2, v3, v4, v5}
	MOV	a1, a1, LSR a4
	ORR	a1, a1, v2, LSL v1
	MOV	v2, v2, LSR a4
	ORR	v2, v2, v3, LSL v1
	MOV	v3, v3, LSR a4
	ORR	v3, v3, v4, LSL v1
	MOV	v4, v4, LSR a4
	ORR	v4, v4, v5, LSL v1
	STMIA	ip!, {a1, v2, v3, v4}
	MOV	a1, v5
	SUBS	lr, lr, #16
	BGT	|__memcpy_cd_l2|
|__memcpy_cd_l3|
	LDMFD	sp!, {v3, v4, v5}
|__memcpy_cd_l4|
	SUB	a2, a2, v1, LSR #3		; reset a2 from [1]
	LDMFD	sp!, {a1, v1, v2, lr}
	CMP	a3, #0
	return	EQ, pc, lr
	B	|__memcpy_bd|

|__memcpy_b|				; byte copy 1 <= length <= 3
	CMP	ip, a2
	BLS	|__memcpy_bd|

|__memcpy_bu|				; byte copyup
	CMP	a3, #2
	LDRB	a4, [a2, #-1]!
	STRB	a4, [ip, #-1]!
	LDRCSB	a4, [a2, #-1]!
	STRCSB	a4, [ip, #-1]!
	LDRHIB	a4, [a2, #-1]!
	STRHIB	a4, [ip, #-1]!
	return	AL, pc, lr

|__memcpy_bd|				; byte copydown
	CMP	a3, #2
	LDRB	a4, [a2], #1
	STRB	a4, [ip], #1
	LDRGEB	a4, [a2], #1
	STRGEB	a4, [ip], #1
	LDRGTB	a4, [a2], #1
	STRGTB	a4, [ip], #1
	return	AL, pc, lr

|__memcpy_w|				; word aligned copy
	CMP	ip, a2
	BLE	|__memcpy_wd|

|__memcpy_wu|				; word aligned copyup
	ANDS	a4, a3, #12
	BEQ	|__memcpy_wu_l0|
	CMP	a4, #8
	LDR	a4, [a2, #-4]!
	STR	a4, [ip, #-4]!
	LDRCS	a4, [a2, #-4]!
	STRCS	a4, [ip, #-4]!
	LDRHI	a4, [a2, #-4]!
	STRHI	a4, [ip, #-4]!
|__memcpy_wu_l0|
	BICS	a3, a3, #12
	return	EQ, pc, lr
	BICS	a4, a3, #15
	BEQ	|__memcpy_bu|
	STMFD	sp!, {a1, v1, v2, lr}
	SUB	a3, a3, a4
|__memcpy_wu_l1|
	LDMDB	a2!, {a1, v1, v2, lr}
	STMDB	ip!, {a1, v1, v2, lr}
	SUBS	a4, a4, #32
	LDMGEDB a2!, {a1, v1, v2, lr}
	STMGEDB ip!, {a1, v1, v2, lr}
	BGT	|__memcpy_wu_l1|
	CMP	a3, #0
	LDMFD	sp!, {a1, v1, v2, lr}
	return	EQ, pc, lr
	B	|__memcpy_bu|

|__memcpy_wd|				; word aligned copydown
	ANDS	a4, a3, #12
	BEQ	|__memcpy_wd_l0|
	CMP	a4, #8
	LDR	a4, [a2], #4
	STR	a4, [ip], #4
	LDRGE	a4, [a2], #4
	STRGE	a4, [ip], #4
	LDRGT	a4, [a2], #4
	STRGT	a4, [ip], #4
|__memcpy_wd_l0|
	BICS	a3, a3, #12
	return	EQ, pc, lr
	BICS	a4, a3, #15
	BEQ	|__memcpy_bd|
	STMFD	sp!, {a1, v1, v2, lr}
	SUB	a3, a3, a4
|__memcpy_wd_l1|
	LDMIA	a2!, {a1, v1, v2, lr}
	STMIA	ip!, {a1, v1, v2, lr}
	SUBS	a4, a4, #32
	LDMGEIA a2!, {a1, v1, v2, lr}
	STMGEIA ip!, {a1, v1, v2, lr}
	BGT	|__memcpy_wd_l1|
	CMP	a3, #0
	LDMFD	sp!, {a1, v1, v2, lr}
	return	EQ, pc, lr
	B	|__memcpy_bd|

	END
