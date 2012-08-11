	; FIXME: GLOBAL
	; FIXME: test EXPORT attributes

	; Export symbols from a normal code area.
	AREA	Code1, CODE

	MOV	r0, #0
	EXPORT	Exp11
Exp11	MOV	r0, #1

Exp12	MOV	r0, #2
	EXPORT	Exp12

	; Export symbols from absolute area.
	AREA	Code2, CODE, ABS

	MOV	r0, #0
	EXPORT	Exp21
Exp21	MOV	r0, #1

Exp22	MOV	r0, #2
	EXPORT	Exp22

	; Export some symbols which we don't have defined.
	AREA	Code3, CODE

	EXPORT	Exp31Foo
	EXPORT	Exp32Foo

	MOV	r0, #1

	; Export symbols from register based area.
	AREA	Data4, DATA, BASED r3

	DCD	0
	EXPORT	Exp41
Exp41	DCD	1

Exp42	DCD	2
	EXPORT	Exp42

	END

