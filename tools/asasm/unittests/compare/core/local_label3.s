	AREA	Code, CODE

	[ :LNOT: REFERENCE

	; ----------------------------------
	; Test 1:
	ROUT
	MOV	r0, #1
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	;B	%b01
	;B	%ba01
	;B	%bt01
01
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	B	%b01
	B	%ba01
	B	%bt01
01

	; ----------------------------------
	; Test 2:
	MACRO
	Test2Level1
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	;B	%b01
	;B	%ba01
	;B	%bt01
01
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	B	%b01
	B	%ba01
	B	%bt01
01
	MEND

	ROUT
	MOV	r0, #2
	;B	%01
	B	%a01
	;B	%t01
	;B	%f01
	B	%fa01
	;B	%ft01
	;B	%b01
	;B	%ba01
	;B	%bt01
	MOV	r1, #2
	Test2Level1
	MOV	r2, #3
	;B	%01
	B	%a01
	;B	%t01
	;B	%f01
	;B	%fa01
	;B	%ft01
	;B	%b01
	B	%ba01
	;B	%bt01

	; ----------------------------------
	; Test 3:
	MACRO
	Test3Level1
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	B	%b01
	B	%ba01
	;B	%bt01
01
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	B	%b01
	B	%ba01
	B	%bt01
01
	MEND

	ROUT
	MOV	r0, #3
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	;B	%b01
	;B	%ba01
	;B	%bt01
01
	MOV	r1, #3
	Test3Level1
	MOV	r2, #3
	B	%01
	B	%a01	; xxx
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	B	%b01
	B	%ba01	; xxx
	B	%bt01
01

	; ----------------------------------
	; Test 4:
	MACRO
	Test4Level1
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	;B	%b01
	;B	%ba01
	;B	%bt01
01
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	B	%b01
	B	%ba01
	B	%bt01
01
	MEND

	ROUT
	MOV	r0, #4
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	;B	%b01
	;B	%ba01
	;B	%bt01
	MOV	r1, #4
	Test4Level1
	MOV	r2, #4
01
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	B	%b01
	B	%ba01
	B	%bt01
01

	; ----------------------------------
	; Test 5:
	MACRO
	Test5Level1
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	;B	%b01
	;B	%ba01
	;B	%bt01
01
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	B	%b01
	B	%ba01
	B	%bt01
01
	MEND

	ROUT
	MOV	r0, #5
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	;B	%b01
	;B	%ba01
	;B	%bt01
	MOV	r1, #5
	Test5Level1
	MOV	r2, #5
	B	%01
	B	%a01
	B	%t01
	B	%f01
	B	%fa01
	B	%ft01
	;B	%b01
	B	%ba01
	;B	%bt01
01

	|

	MOV      r0,#1
	B	lbl1c
	B	lbl1c
	B	lbl1c
	B	lbl1c
	B	lbl1c
	B	lbl1c
lbl1c
	B	lbl1c
	B	lbl1c
	B	lbl1c
	B	lbl40
	B	lbl40
	B	lbl40
	B	lbl1c
	B	lbl1c
	B	lbl1c
lbl40
	MOV      r0,#2
	B	lbl68
	B	lbl68
	MOV      r1,#2
	B	lbl68
	B	lbl68
	B	lbl68
	B	lbl68
	B	lbl68
	B	lbl68
lbl68
	B	lbl68
	B	lbl68
	B	lbl68
	B	lbl8c
	B	lbl8c
	B	lbl8c
	B	lbl68
	B	lbl68
	B	lbl68
lbl8c
	MOV      r2,#3
	B	lbl8c
	B	lbl8c
	MOV      r0,#3
	B	lblb4
	B	lblb4
	B	lblb4
	B	lblb4
	B	lblb4
	B	lblb4
lblb4
	MOV      r1,#3
	B	lblb4
	B	lblb4
	B	lbld8
	B	lbld8
	B	lbld8
	B	lbld8
	B	lblb4
	B	lblb4
lbld8
	B	lbld8
	B	lbld8
	B	lbld8
	B	lblfc
	B	lblfc
	B	lblfc
	B	lbld8
	B	lbld8
	B	lbld8
lblfc
	MOV      r2,#3
	B	lblb4
	B	lblb4	; ObjAsm: B	lblfc
	B	lblb4
	B	lbl124
	B	lbl124
	B	lbl124
	B	lblb4
	B	lblb4	; ObjAsm: B	lblfc
	B	lblb4
lbl124
	MOV      r0,#4
	B	lbl184
	B	lbl15c
	B	lbl184
	B	lbl184
	B	lbl15c
	B	lbl184
	MOV      r1,#4
	B	lbl15c
	B	lbl15c
	B	lbl15c
	B	lbl15c
	B	lbl15c
	B	lbl15c
lbl15c
	B	lbl15c
	B	lbl15c
	B	lbl15c
	B	lbl180
	B	lbl180
	B	lbl180
	B	lbl15c
	B	lbl15c
	B	lbl15c
lbl180
	MOV      r2,#4
lbl184
	B	lbl184
	B	lbl184
	B	lbl184
	B	lbl1a8
	B	lbl1a8
	B	lbl1a8
	B	lbl184
	B	lbl184
	B	lbl184
lbl1a8
	MOV      r0,#5
	B	lbl224
	B	lbl1e0
	B	lbl224
	B	lbl224
	B	lbl1e0
	B	lbl224
	MOV      r1,#5
	B	lbl1e0
	B	lbl1e0
	B	lbl1e0
	B	lbl1e0
	B	lbl1e0
	B	lbl1e0
lbl1e0
	B	lbl1e0
	B	lbl1e0
	B	lbl1e0
	B	lbl204
	B	lbl204
	B	lbl204
	B	lbl1e0
	B	lbl1e0
	B	lbl1e0
lbl204
	MOV      r2,#5
	B	lbl224
	B	lbl204
	B	lbl224
	B	lbl224
	B	lbl224
	B	lbl224
	B	lbl204
lbl224

	]

	END
