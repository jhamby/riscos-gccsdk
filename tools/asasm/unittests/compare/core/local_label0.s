	; Basic ROUT and local label test.

	AREA	Code1, CODE, READONLY
	[ :LNOT: REFERENCE

xyz	ROUT
01	MOV	r1, #1
02xyz	MOV	r1, #2
	B	%01
	B	%02xyz

|.xy|	ROUT
03	MOV	r1, #3
;; FIXME: 04|.xy|	MOV	r1, #4
	B	%03|.xy|
	;; FIXME: B	%04

	; Test skipping local labels
	IF {FALSE}
05|.xy|	ENDIF		; Should give a warning this label gets ignored.

	WHILE {FALSE}
05|.xy|	WEND		; Should give a warning this label gets ignored.
	
	|

c1lbl1	MOV	r1, #1
c1lbl2	MOV	r1, #2
	B	c1lbl1
	B	c1lbl2

c1lbl3	MOV	r1, #3
;; FIXME: c1bll4	MOV	r1, #4
	B	c1lbl3
	;; FIXME: B	c1blb4

	]
	
	END
