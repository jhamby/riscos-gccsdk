	AREA	Code, CODE

	[ :LNOT:REFERENCE
	MACRO
	Test3Level2 $lbl2
$lbl2	MOV	r2, #0
	MEND

	MACRO
	Test3Level1 $lbl1a, $lbl1b, $lbl2
$lbl1a	MOV	r1, #0
	Test3Level2
$lbl1b	MOV	r1, #1
	MEND

	MACRO
	Test3Level0 $lbl0
	MOV	r0, #-1
$lbl0	MOV	r0, #0

	Test3Level1 ,,
	B	%a01

	Test3Level1 ,,01
	B	%a01

	Test3Level1 ,01,
	B	%a01

	Test3Level1 ,01,01
	B	%a01

	Test3Level1 01,,
	B	%a01

	Test3Level1 01,,01
	B	%a01

	Test3Level1 01,01,
	B	%a01

	Test3Level1 01,01,01
	B	%a01
	MEND

	Test3Level0
	Test3Level0 01

01
	Test3Level0
	Test3Level0 01

	|

	MVN      r0,#0
	MOV      r0,#0
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl30
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl30
	MOV      r1,#0
	MOV      r2,#0
c1lbl30
	MOV      r1,#1
	B        c1lbl30
	MOV      r1,#0
	MOV      r2,#0
c1lbl40
	MOV      r1,#1
	B        c1lbl40
c1lbl48
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl48
c1lbl58
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl58
	MOV      r1,#0
	MOV      r2,#0
c1lbl70
	MOV      r1,#1
	B        c1lbl70
	MOV      r1,#0
	MOV      r2,#0
c1lbl80
	MOV      r1,#1
	B        c1lbl80
	MVN      r0,#0
c1lbl8c
	MOV      r0,#0
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl8c
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl8c
	MOV      r1,#0
	MOV      r2,#0
c1lblb8
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lblb8
	MOV      r1,#0
	MOV      r2,#0
c1lblc8
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lblc8
c1lbld0
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbld0
c1lble0
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lble0
	MOV      r1,#0
	MOV      r2,#0
c1lblf8
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lblf8
	MOV      r1,#0
	MOV      r2,#0
c1lbl108
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl108
c1lbl110
	MVN      r0,#0
	MOV      r0,#0
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl110
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl110
	MOV      r1,#0
	MOV      r2,#0
c1lbl140
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl140
	MOV      r1,#0
	MOV      r2,#0
c1lbl150
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl150
c1lbl158
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl158
c1lbl168
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl168
	MOV      r1,#0
	MOV      r2,#0
c1lbl180
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl180
	MOV      r1,#0
	MOV      r2,#0
c1lbl190
	MOV      r1,#1
	B        c1lbl8c	; ObjAsm: c1lbl190
	MVN      r0,#0
c1lbl19c
	MOV      r0,#0
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl19c
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl19c
	MOV      r1,#0
	MOV      r2,#0
c1lbl1c8
	MOV      r1,#1
	B        c1lbl19c	; ObjAsm: c1lbl1c8
	MOV      r1,#0
	MOV      r2,#0
c1lbl1d8
	MOV      r1,#1
	B        c1lbl19c	; ObjAsm: c1lbl1d8
c1lbl1e0
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl19c	; ObjAsm: c1lbl1e0
c1lbl1f0
	MOV      r1,#0
	MOV      r2,#0
	MOV      r1,#1
	B        c1lbl19c	; ObjAsm: c1lbl1f0
	MOV      r1,#0
	MOV      r2,#0
c1lbl208
	MOV      r1,#1
	B        c1lbl19c	; ObjAsm: c1lbl208
	MOV      r1,#0
	MOV      r2,#0
c1lbl218
	MOV      r1,#1
	B        c1lbl19c	; ObjAsm: c1lbl218

	]
	
	END
