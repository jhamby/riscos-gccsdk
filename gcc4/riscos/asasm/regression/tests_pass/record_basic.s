	AREA Code1, CODE, READONLY

	; Basic record test

	[ :LNOT: REFERENCE
		MOV	R0, #0
		MOV	R1, #0

		^	5
		#	2
lbl1		#	1

		MAP	2
lbl2		FIELD	0

		^	(2*lbl1 + lbl2 + 9) / 5
lbl3		#	1

		MOV	R2, #lbl1	; #7
		MOV	R3, #lbl2	; #2
		MOV	R4, #lbl3	; #5

		MOV	R5, #|lbl1| + |lbl2|			; #7 + 2
		LDR	R6, [R1, #|lbl1| + |lbl2|]		; #7 + 2
		LDR	R7, [R1, #|lbl1| + |lbl2| + |lbl3|]	; #7 + 2 + 5

; FIXME		MOV	R2, #lbl4	; #7
; FIXME		MOV	R3, #lbl5	; #2
; FIXME		MOV	R4, #lbl6

; FIXME		MOV	R5, #|lbl4| + |lbl5|			; #7 + 2
; FIXME		LDR	R6, [R1, #|lbl4| + |lbl5|]		; #7 + 2
; FIXME		LDR	R7, [R1, #|lbl4| + |lbl5| + |lbl6|]	; #7 + 2 + 5

		^	5
		MAP	2
lbl4		FIELD	1

		^	2
lbl5		#	0

		^	(2*lbl4 + lbl5 + 9) / 5
lbl6		#	1

	|
		MOV	R0, #0
		MOV	R1, #0

		MOV	R2, #7
		MOV	R3, #2
		MOV	R4, #5

		MOV	R5, #9
		LDR	R6, [R1, #9]
		LDR	R7, [R1, #14]

; FIXME		MOV	R2, #7
; FIXME		MOV	R3, #2
; FIXME		MOV	R4, #5

; FIXME		MOV	R5, #9
; FIXME		LDR	R6, [R1, #9]
; FIXME		LDR	R7, [R1, #14]
	]

	AREA	Code2, CODE, READONLY

	[ :LNOT: REFERENCE
		%	20

tst2_b		%	32
		^	tst2_b
field0		#	4
field1		#	8
field2		#	16

		LDR	r1, field1
		DCD	field1

		LDR	r2, field4
		DCD	field4

tst2_a		%	32
		^	tst2_a
field3		#	4
field4		#	8
field5		#	16

	|
		%	20 + 32

		LDR	r1, {PC} - 32 + 4
		DCD	{PC} - 4 - 32 + 4
		
		LDR	r2, {PC} + 8 + 4
		DCD	{PC} + 4 + 4
		
		%	32
	]


	AREA	Code3, CODE
	
	[ :LNOT: REFERENCE
		^	4
tst3_a		#	4
tst3_b		#	-4
tst3_c		#	-4
tst3_d		#	0

		MOV	r1, #tst3_a
		MOV	r2, #tst3_b
		MOV	r3, #tst3_c
		MOV	r4, #tst3_d
	|
		MOV	r1, #4
		MOV	r2, #8
		MOV	r3, #4
		MOV	r4, #0
	]

	END
