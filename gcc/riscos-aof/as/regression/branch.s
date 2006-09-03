		GET config.h

		AREA |C$$code|, CODE, READONLY

		; Branch instructions
	[ :LNOT: REFERENCE
label
		B	label
		BL	label

;		BLX	label
;		BLX	label + 2

;		BLX	R0

;		BX	R0
	|
		DCD	&EAFFFFFE		; B label
		DCD	&EBFFFFFD		; BL label

;		DCD	&FAFFFFFC		; BLX label
;		DCD	&FBFFFFFB		; BLX label + 2

;		DCD	&E12FFF30		; BLX R0

;		DCD	&E12FFF10		; BX R0
	]

		END
