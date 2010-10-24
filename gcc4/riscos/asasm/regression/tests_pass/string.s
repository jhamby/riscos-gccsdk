	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE

	=	">Dbl Quote char "" <"
	=	">Dollar char $$ <"
	=	">Dollar char $ <"	; Results in a failed lookup
	=	">Dollar char $x <"	; Results in a failed lookup
	=	">Dollar char $x. <"	; Results in a failed lookup

	|

	=	">Dbl Quote char " :CC: :CHR:&22 :CC: " <"
	=	">Dollar char " :CC: :CHR:&24 :CC: " <"
	=	">Dollar char " :CC: :CHR:&24 :CC: " <"
	=	">Dollar char " :CC: :CHR:&24 :CC: "x <"
	=	">Dollar char " :CC: :CHR:&24 :CC: "x. <"

	]

	END
