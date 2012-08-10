	GBLS	Test
Test	SETS	"GET	Hdr:<APCS>.Common"
	; Environement variables after variable expansion need to be expanded
	; as well.
	$Test

	[ HAVE_SEEN_APCS32
	; Ok, test succeeded
	|
	; Test failed
	TEST FAILED
	]

	; Environment variables in strings may not expand.
	AREA	Data, DATA
	[ :LNOT:REFERENCE
	=	"xxx<APCS>zzz"
	=	"$Test"
	|
	=	"xxx" :CC: :CHR:60 :CC: "APCS" :CC: :CHR:62 :CC: "zzz"
	=	"GET	Hdr:" :CC: :CHR:60 :CC: "APCS" :CC: :CHR:62 :CC: ".Common"
	]
	ALIGN

	END
