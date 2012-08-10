; Tests PLD, PLDW, PLI
; FIXME: do PLDW and PLI
; RUNOPT: -CPU=7-A

		AREA Code, CODE, READONLY

	[ :LNOT: REFERENCE
		; Case 1 : PLD{W} [<Rn>, #+/-<imm12>]
		PLD	[R1]
		PLD	[R1,#4]
		PLD	[R1,#-4]

		; Case 2 : PLD{W} <label>
		; FIXME: todo

		; Case 3 : PLD{W} [<Rn>,+/-<Rm>{, <shift>}]
		PLD	[R1,R3]
		PLD	[R1,-R3]

		PLD	[R1,R2,LSL #3]
		PLD	[R1,R2,LSR #4]
		PLD	[R1,-R2,ASR #5]
		PLD	[R1,-R2,ROR #6]
		PLD	[R1,-R2,RRX]
	|
		; Case 1
		DCI	&f5d1f000	; PLD      [r1]
		DCI	&f5d1f004	; PLD      [r1,#4]
		DCI	&f551f004	; PLD      [r1,#-4]

		; Case 2

		; Case 3
		DCI	&f7d1f003	; PLD      [r1,r3]
		DCI	&f751f003	; PLD      [r1,-r3]

		DCI	&f7d1f182	; PLD      [r1,r2,LSL #3]
		DCI	&f7d1f222	; PLD      [r1,r2,LSR #4]
		DCI	&f751f2c2	; PLD      [r1,-r2,ASR #5]
		DCI	&f751f362	; PLD      [r1,-r2,ROR #6]
		DCI	&f751f062	; PLD      [r1,-r2,RRX]
	]

		END
