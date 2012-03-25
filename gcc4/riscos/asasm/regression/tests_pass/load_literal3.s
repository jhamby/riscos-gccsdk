; -RUNOPT: -CPU=6T2

	AREA Code, CODE

	; 'Bla*' variables are defined after their use and can actually
	; trigger a LDR -> MOV/MVN/MOVW/MOVT optimisation.  The test is that
	; we don't output an unnecessary literal slot for it.
	[ :LNOT: REFERENCE
	
	LDR	r0, =5
	LDR	r1, =~5
	LDR	r2, =&BA98
	LDR	r3, =&FEDC0000

	LDR	r0, =Bla10
	LDR	r1, =BlaInv10
	LDR	r2, =Bla4321
	LDR	r3, =Bla87650000

Bla10		* 10
BlaInv10	* ~10
Bla4321		* &4321
Bla87650000	* &87650000
	
	LDR	r0, =&10105
	LDR	r1, =Bla1010A

Bla1010A	* &1010A

	LTORG
	NOP

	|

	MOV	r0, #5
	MVN	r1, #5
	DCI &e30b2a98	; FIXME: MOVW	r2, #&BA98
	DCI &e34f3edc	; FIXME: MOVT	r3, #&FEDC

	MOV	r0, #10
	MVN	r1, #10
	DCI &e3042321	; FIXME: MOVW	r2, #&4321
	DCI &e3483765	; FIXME: MOVT	r3, #&8765

	LDR	r0, lc1
	LDR	r1, lc2
lc1	DCD	&10105
lc2	DCD	&1010A

	LTORG
	NOP
	
	]

	END
