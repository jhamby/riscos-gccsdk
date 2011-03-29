	GET	Hdr:<APCS>.Common

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
	|
	=	"xxx" :CC: :CHR:60 :CC: "APCS" :CC: :CHR:62 :CC: "zzz"
	]
	ALIGN

	END
