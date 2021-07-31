	AREA Code1, CODE

	DCI &10
Code1Br
	DCI &11
	DCI &12
	B Code1Br	; Branch inside this area.
	B &a004		; Branch inside this area (when ORG &a000), otherwise possibly outside this area.
	B &1000		; Branch outside this area (before this area).
	B &b000		; Branch outside this area (after this area).
	B Code2Br	; Branch to another area.

	AREA Code2, CODE
	DCI &20
	DCI &21
	DCI &22
	DCI &23
Code2Br

	END
