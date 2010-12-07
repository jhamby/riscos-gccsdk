		AREA |C$$code|, CODE, READONLY

		; Basic record test

	[ :LNOT: REFERENCE
		MOV	R0, #0
		MOV	R1, #0

		^	5
		#	2
lbl1		#	1

		^	2
lbl2		#	0

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
		#	2
lbl4		#	1

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

		END
