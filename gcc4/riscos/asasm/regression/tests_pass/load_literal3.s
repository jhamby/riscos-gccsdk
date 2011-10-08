	AREA Code, CODE

	; 'Bla' is defined after its use and can actually
	; trigger a LDR -> MOV optimisation.  The test is that
	; we don't output an unnecessary literal slot for it.
	[ :LNOT: REFERENCE
	
	LDR	r0, =5
	LDR	r1, =Bla

Bla	*	10
	
	LDR	r0, =&105
	LDR	r1, =Bla2

Bla2	*	&10A

	LTORG
	NOP

	|

	MOV	r0, #5
	MOV	r1, #10
	LDR	r0, lc1
	LDR	r1, lc2
lc1	DCD	&105
lc2	DCD	&10A

	LTORG
	NOP
	
	]

	END
