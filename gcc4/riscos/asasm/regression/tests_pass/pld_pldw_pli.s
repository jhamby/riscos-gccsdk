; Tests PLD, PLDW, PLI
; FIXME: do PLDW and PLI

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
		DCD	&f5d1f000	; PLD      [r1]
		DCD	&f5d1f004	; PLD      [r1,#4]
		DCD	&f551f004	; PLD      [r1,#-4]

		DCD	&f7d1f003	; PLD      [r1,r3]
		DCD	&f751f003	; PLD      [r1,-r3]

		DCD	&f7d1f182	; PLD      [r1,r2,LSL #3]
		DCD	&f7d1f222	; PLD      [r1,r2,LSR #4]
		DCD	&f751f2c2	; PLD      [r1,-r2,ASR #5]
		DCD	&f751f362	; PLD      [r1,-r2,ROR #6]
		DCD	&f751f062	; PLD      [r1,-r2,RRX]
	]

		END
