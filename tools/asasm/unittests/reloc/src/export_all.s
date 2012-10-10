; Test "EXPORT" (without any arguments)

	AREA Data, DATA

	DCD	1
ExtDat1	DCD	2
	DCD	3
ExtDat2

	AREA Code, CODE

	MOV	R0, #1
ExtCod1	MOV	R0, #2
	MOV	R0, #3
ExtCod2

	EXPORT

	END
