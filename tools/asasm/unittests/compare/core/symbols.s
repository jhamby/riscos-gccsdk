; Tests symbols

	AREA	Code, CODE
	
	[ :LNOT: REFERENCE
	
Tst1		DCD	1
|Tst2|		DCD	2
||Tst3||	DCD	3
||Ts|t4||	DCD	4

		DCD	Tst1
		DCD	|Tst1|
		DCD	||Tst1||
		DCD	Tst2
		DCD	|Tst2|
		DCD	||Tst2||
		DCD	Tst3
		DCD	|Tst3|
		DCD	||Tst3||
		DCD	||Ts|t4||

	|

		DCD	1
		DCD	2
		DCD	3
		DCD	4
		
		DCD	Code + 0
		DCD	Code + 0
		DCD	Code + 0
		DCD	Code + 4
		DCD	Code + 4
		DCD	Code + 4
		DCD	Code + 8
		DCD	Code + 8
		DCD	Code + 8
		DCD	Code + 12
	
	]

	; Test redefinition of absolute symbols containing same values are
	; possible.
	AREA	Code2a, CODE
v7	RN	10
v7	RN	10
	AREA	Code2b, CODE
v7	RN	10 
	
	END
