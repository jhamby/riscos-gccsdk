; Tests BKPT.
; RUNOPT: -CPU=5T

		AREA |C$$code|, CODE, READONLY

	[ :LNOT: REFERENCE
		ARM
		BKPT	#1234
		BKPT.W	&Feba

		THUMB
		BKPT	#0xC2
		BKPT.N	&C2
	|
		ARM
		DCI &e1204d72	; bkpt	0x04d2
		DCI &e12feb7a	; bkpt	0xfeba

		THUMB
		DCI.N &bec2	; bkpt	0x00c2
		DCI.N &bec2	; bkpt	0x00c2

	]

		END
