; Tests MCRR/MRRC.
; -RUNOPT: -CPU=7-A

		AREA |C$$code|, CODE, READONLY

		; Enhanced DSP instructions

	[ :LNOT: REFERENCE
		MCRR	p0,0,r0,r1,c0
		MRRC	p0,0,r0,r1,c0
	|
		DCI	&EC410000
		DCI	&EC510000
	]

		END
