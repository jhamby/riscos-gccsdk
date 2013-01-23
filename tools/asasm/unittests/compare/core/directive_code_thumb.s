; Test CODE16/CODE32/ARM/THUMB/THUMBX
; RUNOPT: -cpu=6T2

	AREA	CodeARM, CODE
	[ :LNOT:REFERENCE
	DCB	0xFF
	ARM
	DCB	0xEE
	CODE32
	DCB	0xDD
	MOV	r0, #0
	|
	DCB	0xFF
	DCB	0, 0, 0	; Alignment by ARM
	DCB	0xEE
	DCB	0, 0, 0	; Alignment by CODE32
	DCB	0xDD
	DCB	0, 0, 0	; Alignment by MOV r0, #0
	MOV	r0, #0
	]

	AREA	CodeThumb, CODE
	[ :LNOT:REFERENCE
	DCB	0xFF
	CODE16
	DCB	0xEE
	THUMB
	DCB	0xDD
	THUMBX
	DCB	0xCC
	movw	r9, #1
	|
	DCB	0xFF
	DCB	0	; Alignment by CODE16
	DCB	0xEE
	DCB	0	; Alignmnet by THUMB
	DCB	0xDD
	DCB	0	; Alignment by THUMBX
	DCB	0xCC
	DCB	0	; Alignment by movw r9, #1
	THUMBX
	DCI.W &f2400901
	]

	END