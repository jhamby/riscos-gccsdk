; Tests RFE and SRS.

	AREA	Code, CODE

	[ :LNOT: REFERENCE

	; Test RFE:
	RFE	r1
	RFEIA	r1!
	RFEIB		r2
	RFEDA	r2	!
	RFEDB	r2

	; Use alternative stack notations:
	RFEFD	r3
	RFEED	r4
	RFEFA	r5
	RFEEA	r6

	; Test SRS:
REGSP	RN	13

	; UAL syntax:
	SRS	sp, # 0x8 + 0x8
	SRSIA	sp !, #0x10
	SRSIB	sp!   ,#	0x10
	SRSDA		REGSP,  #16
	SRSDB	REGSP!, #32/2

	; Pre-UAL syntax:
	SRS	# 0x8 + 0x8
	SRSIA	#0x10!
	SRSIB	#	0x10  !
	SRSDA		#16
	SRSDB	#32/2 !

	; Use alternative stack notations:
	SRSEA	#0x10
	SRSFA	#0x11
	SRSED	#0x12
	SRSFD	#0x13

	|

	DCD &f8910a00 	; rfeia	r1
	DCD &f8b10a00 	; rfeia	r1!
	DCD &f9920a00 	; rfeib	r2
	DCD &f8320a00 	; rfeda	r2!
	DCD &f9120a00 	; rfedb	r2

	DCD &f8930a00 	; rfeia	r3
	DCD &f9940a00 	; rfeib	r4
	DCD &f8150a00 	; rfeda	r5
	DCD &f9160a00 	; rfedb	r6

	DCD &f8cd0510 	; srsia	sp, #16
	DCD &f8ed0510 	; srsia	sp!, #16
	DCD &f9ed0510 	; srsib	sp!, #16
	DCD &f84d0510 	; srsda	sp, #16
	DCD &f96d0510 	; srsdb	sp!, #16

	DCD &f8cd0510 	; srsia	sp, #16
	DCD &f8ed0510 	; srsia	sp!, #16
	DCD &f9ed0510 	; srsib	sp!, #16
	DCD &f84d0510 	; srsda	sp, #16
	DCD &f96d0510 	; srsdb	sp!, #16

	DCD &f8cd0510 	; srsia	sp, #16
	DCD &f9cd0511 	; srsib	sp, #17
	DCD &f84d0512 	; srsda	sp, #18
	DCD &f94d0513 	; srsdb	sp, #19

	]

	END