; Test immediate12 constant support.

	AREA	Code, CODE

	[ :LNOT: REFERENCE
	MOV	r1, #&81,0
	MOV	r2, #&81,2
	MOV	r3, #&81,8
	MOV	r4, #&81,16
	MOV	r5, #&81,24
	MOV	r6, #&81,30
	|
	MOV	r1, #&81
	MOV	r2, #&40000020
	MOV	r3, #&81000000
	MOV	r4, #&810000
	MOV	r5, #&8100
	MOV	r6, #&204
	]

	END

