; Export all local symbols.  Test for code area datum attribute of the
; local symbols.

	KEEP

	AREA	Code, CODE

Code1Sym01	MOV	R0, #0
Code1Sym02	MOV	R0, #1
Code1Sym03
		MOV	R0, #2
Code1Sym04			; FIXME: ideally this should have datum attribute
		DCD	4
Code1Sym05
		DCD	5
Code1Sym06	MOV	R0, #6
Code1Sym07	MOV	R0, #7
Code1Sym08
		DCI	8
Code1Sym09
		DCI	9
Code1Sym10	DCD	10
Code1Sym11
		MOV	R0, #11 ; FIXME: may not have datum attribute

	AREA	Code2, CODE

Code2Sym01
		MOV	R0, #0

	AREA	Data1, DATA

Data1Sym01
		DCD	1
Data1Sym02	DCD	2

	AREA	Data2, DATA

Data2Sym01	DCD	1

	END
