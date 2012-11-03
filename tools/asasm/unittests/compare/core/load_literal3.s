; -RUNOPT: -CPU=6T2

	AREA Code, CODE

	; 'Bla*' variables are defined after their use and can actually
	; trigger a LDR -> MOV/MVN/MOVW optimisation.  The test is that
	; we don't output an unnecessary literal slot for it.
	[ :LNOT: REFERENCE
	
	LDR	r0, =5
	LDR	r1, =~5
	LDR	r2, =&BA98

	LDR	r0, =Bla10
	LDR	r1, =BlaInv10
	LDR	r2, =Bla4321

Bla10		* 10
BlaInv10	* ~10
Bla4321		* &4321
	
	LDR	r0, =&10105
	LDR	r1, =Bla1010A

Bla1010A	* &1010A

	LTORG
	NOP

	|

	MOV	r0, #5
	MVN	r1, #5
	MOVW	r2, #&BA98

	MOV	r0, #10
	MVN	r1, #10
	MOVW	r2, #&4321

	LDR	r0, lc1
	LDR	r1, lc2
lc1	DCD	&10105
lc2	DCD	&1010A

	LTORG
	NOP
	
	]

	END
