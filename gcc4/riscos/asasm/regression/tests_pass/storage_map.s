	AREA	Data, DATA, READONLY, BASED r4

	%	40
dt0	%	4

	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE

		[ EXTENSION
; FIXME:	MOV	r1, dt0		; ObjAsm extension: MOV r1, r4, #8 -> ADD r1, r4, #40
		]
		LDR	r2, dt0

	^	8, r3
dt1	#	10

		[ EXTENSION
; FIXME:	MOV	r1, dt1		; ObjAsm extension: MOV r1, r3, #8 -> ADD r1, r3, #8
		]
		LDR	r2, dt1

	|

		[ EXTENSION
; FIXME:	ADD	r1, r4, #40
		]
		LDR	r2, [r4, #40]

		[ EXTENSION
; FIXME:	ADD	r1, r3, #8
		]
		LDR	r2, [r3, #8]

	]

	END
