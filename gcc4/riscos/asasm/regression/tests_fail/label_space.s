; label space '?' operator is not implemented in asasm.

	GET	config.h

	^	&1000
Space1	#	16
Space2	#	8
Space3	#	4

	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE
	DCD	Space1
	DCD	Space2
	DCD	Space3

	DCD	?Space1
	DCD	?Space2
	DCD	?Space3

	LDR	r0, =?Space1
	MOV	r0, #?Space2
	MOV	r0, r1, LSL#?Space3

	|

	DCD	&1000
	DCD	&1010
	DCD	&1018

	DCD	16
	DCD	8
	DCD	4

	MOV	r0, #16
	MOV	r0, #8
	MOV	r0, r1, LSL#4

	]

	END
