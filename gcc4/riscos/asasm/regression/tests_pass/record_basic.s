		GET config.h

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

;		^	(2*lbl1 + lbl2 + 9) / 5
;lbl3		#	1

		MOV	R2, #lbl1
		MOV	R3, #lbl2
;		MOV	R4, #lbl3
	|
		MOV	R0, #0
		MOV	R1, #0
		MOV	R2, #7
		MOV	R3, #2
;		MOV	R4, #5
	]

		END
