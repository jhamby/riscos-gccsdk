; Tests =, ==, !=, <>, /= for integer, string, addr, boolean, registers.
; Tests >, >=, <, <= for integer, string and addr.

	[ :LNOT: :DEF:EXTENSION
	GBLL	EXTENSION
EXTENSION SETL	{FALSE}
	]

	AREA Data, DATA, READONLY

	[ :LNOT:REFERENCE

	^ 4, r2
x2	#	4
x3	#	4

	^ 0, r2
y1	#	4
y2	#	4
y3	#	4

	^ 0, r3
z1	#	4
z2	#	4
z3	#	4

FooR3	RN	r3

	MACRO
	InvokeRel $rel, $ext
	= "Test $rel : "
	= :STR:(3 $rel 5)
	= :STR:(5 $rel 3)
	= :STR:(-1 $rel 1)
	= :STR:(-1 $rel &FFFFFFFF)
	= "/"
	= :STR:(:CHR:235 $rel 5)
	= :STR:(:CHR:235 $rel 235)
	= "/"
	= :STR:("ABC" $rel "DEF")
	= :STR:("ABC" $rel "ABC")
	= :STR:("ABC "$rel "ABCD")
	= :STR:("ABCD "$rel "ABC")
	= "/"
	= :STR:(98 $rel "a")
	= :STR:(98 $rel "b")
	= :STR:("a" $rel 98)
	= :STR:("b" $rel 98)
	= "/"
	= :STR:(x2 $rel x3)
	= :STR:(x2 $rel y1)
	= :STR:(x2 $rel y2)
	= :STR:(x2 $rel y3)
	= :STR:(x2 $rel z1)	; ObjAsm: Always {FALSE} as base register differ.
	= :STR:(x2 $rel z2)	; ObjAsm: Always {FALSE} as base register differ.
	= :STR:(x2 $rel z3)	; ObjAsm: Always {FALSE} as base register differ.
	[ $ext
	; ObjAsm doesn't seem to support /= on {TRUE}/{FALSE}
	[ EXTENSION :LOR: ("$rel" /= "/=")
	= "/"
	= :STR:({FALSE} $rel {FALSE})
	= :STR:({FALSE} $rel {TRUE})
	= :STR:({TRUE} $rel {FALSE})
	= :STR:({TRUE} $rel {TRUE})
	]
	= "/"
10
	= :STR:(. $rel %b10)
	= :STR:(. $rel %b10)
	= :STR:(FooR3 $rel r3)
	= :STR:(FooR3 $rel c0)
	= :STR:(FooR3 $rel f3)	; Register type gets ignored.  Only register value counts.
	]
	= "\n"
	MEND

	InvokeRel =, {TRUE}
	InvokeRel /=, {TRUE}

	[ EXTENSION
	InvokeRel ==, {TRUE}
	InvokeRel <>, {TRUE}
	InvokeRel !=, {TRUE}
	]

	InvokeRel >, {FALSE}
	InvokeRel >=, {FALSE}
	InvokeRel <, {FALSE}
	InvokeRel <=, {FALSE}

	|

	= "Test = : FFFT/FT/FTFF/FTFT/FFTFFFF/TFFT/TFTFT\n"
	= "Test /= : TTTF/TF/TFTT/TFTF/TTFTFFF/FTTF/FTFTF\n"

	[ EXTENSION
	= "Test == : FFFT/FT/FTFF/FTFT/FFTFFFF/TFFT/TFTFT\n"
	= "Test <> : TTTF/TF/TFTT/TFTF/TTFTFFF/FTTF/FTFTF\n"
	= "Test != : TTTF/TF/TFTT/TFTF/TTFTFFF/FTTF/FTFTF\n"
	]

	= "Test > : FTTF/TF/FFFT/TFFF/FTFFFFF\n"
	= "Test >= : FTTT/TT/FTFT/TTFT/FTTFFFF\n"
	= "Test < : TFFF/FF/TFTF/FFTF/TFFTFFF\n"
	= "Test <= : TFFT/FT/TTTF/FTTT/TFTTFFF\n"

	]

	AREA	Data2, DATA, READONLY
	[ :LNOT: REFERENCE
Lbl2	; Needs to be at start of area.
	= :STR:(.=Lbl2)
	|
	= "T"
	]

	AREA	Data3, DATA, READONLY
	[ :LNOT: REFERENCE
Lbl3	; Needs to be at start of area.
	= :STR:(.<>Lbl3)
	|
	= "F"
	]

	END
