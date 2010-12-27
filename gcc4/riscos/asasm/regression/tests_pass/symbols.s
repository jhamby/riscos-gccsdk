; Tests symbols

	AREA	Code, CODE
	
	[ :LNOT: REFERENCE
	
Tst1		DCD	1
|Tst2|		DCD	2
||Tst3||	DCD	3

		DCD	Tst1
		DCD	|Tst1|
		DCD	||Tst1||
		DCD	Tst2
		DCD	|Tst2|
		DCD	||Tst2||
		DCD	Tst3
		DCD	|Tst3|
		DCD	||Tst3||

	|

		DCD	1
		DCD	2
		DCD	3
		
		DCD	Code + 0
		DCD	Code + 0
		DCD	Code + 0
		DCD	Code + 4
		DCD	Code + 4
		DCD	Code + 4
		DCD	Code + 8
		DCD	Code + 8
		DCD	Code + 8
	
	]
	
	END