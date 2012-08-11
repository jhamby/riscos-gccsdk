	; Import symbols

	; CODE area, relative.
	AREA	Code1, CODE

	MOV	r0, #0
	IMPORT	Imp11
	BL	Imp11

	BL	Imp12
	IMPORT	Imp12

	BL	Imp13	; Not declared as IMPORT, asasm will implicitly do this and warn about it.

	; CODE area, absolute.
	AREA	Code2, ABS, CODE

	MOV	r0, #0
	IMPORT	Imp21
	BL	Imp21

	BL	Imp22
	IMPORT	Imp22

	BL	Imp23	; Not declared as IMPORT, asasm will implicitly do this and warn about it.

	; DATA area with BASED register.
	AREA	Data3, BASED r4

	DCD	0
	IMPORT	Imp31
	DCD	Imp31

	DCD	Imp32
	IMPORT	Imp32

	DCD	Imp33	; Not declared as IMPORT, asasm will implicitly do this and warn about it.

	END
