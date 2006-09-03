		GET	config.h

		AREA |C$$code|,CODE,READONLY

		; Addressing Mode 1 - Data processing operands

	[ :LNOT: REFERENCE
		ADC	R0,R1,#0		; 1
		ADC	R0,R1,#4
		ADC	R0,R1,#&FF
		ADC	R0,R1,#&3F0
		ADC	R0,R1,R2		; 2
		ADC	R0,R1,R2,LSL #2		; 3
		ADC	R0,R1,R2,LSL #31
		ADC	R0,R1,R2,LSL R3		; 4
		ADC	R0,R1,R2,LSR #2		; 5
		ADC	R0,R1,R2,LSR #32
		ADC	R0,R1,R2,LSR R3		; 6
		ADC	R0,R1,R2,ASR #2		; 7
		ADC	R0,R1,R2,ASR #32
		ADC	R0,R1,R2,ASR R3		; 8
		ADC	R0,R1,R2,ROR #2		; 9
		ADC	R0,R1,R2,ROR #31
		ADC	R0,R1,R2,ROR R3		; 10
		ADC	R0,R1,R2,RRX		; 11

		ADCS	R0,R1,#0		; 1
		ADCS	R0,R1,#4
		ADCS	R0,R1,#&FF
		ADCS	R0,R1,#&3F0
		ADCS	R0,R1,R2		; 2
		ADCS	R0,R1,R2,LSL #2		; 3
		ADCS	R0,R1,R2,LSL #31
		ADCS	R0,R1,R2,LSL R3		; 4
		ADCS	R0,R1,R2,LSR #2		; 5
		ADCS	R0,R1,R2,LSR #32
		ADCS	R0,R1,R2,LSR R3		; 6
		ADCS	R0,R1,R2,ASR #2		; 7
		ADCS	R0,R1,R2,ASR #32
		ADCS	R0,R1,R2,ASR R3		; 8
		ADCS	R0,R1,R2,ROR #2		; 9
		ADCS	R0,R1,R2,ROR #31
		ADCS	R0,R1,R2,ROR R3		; 10
		ADCS	R0,R1,R2,RRX		; 11
	|
		DCD	&E2A10000
		DCD	&E2A10004
		DCD	&E2A100FF
		DCD	&E2A10E3F
		DCD	&E0A10002
		DCD	&E0A10102
		DCD	&E0A10F82
		DCD	&E0A10312
		DCD	&E0A10122
		DCD	&E0A10022
		DCD	&E0A10332
		DCD	&E0A10142
		DCD	&E0A10042
		DCD	&E0A10352
		DCD	&E0A10162
		DCD	&E0A10FE2
		DCD	&E0A10372
		DCD	&E0A10062

		DCD	&E2B10000
		DCD	&E2B10004
		DCD	&E2B100FF
		DCD	&E2B10E3F
		DCD	&E0B10002
		DCD	&E0B10102
		DCD	&E0B10F82
		DCD	&E0B10312
		DCD	&E0B10122
		DCD	&E0B10022
		DCD	&E0B10332
		DCD	&E0B10142
		DCD	&E0B10042
		DCD	&E0B10352
		DCD	&E0B10162
		DCD	&E0B10FE2
		DCD	&E0B10372
		DCD	&E0B10062
	]

	[ :LNOT: REFERENCE
		ADD	R0,R1,#0		; 1
		ADD	R0,R1,#4
		ADD	R0,R1,#&FF
		ADD	R0,R1,#&3F0
		ADD	R0,R1,R2		; 2
		ADD	R0,R1,R2,LSL #2		; 3
		ADD	R0,R1,R2,LSL #31
		ADD	R0,R1,R2,LSL R3		; 4
		ADD	R0,R1,R2,LSR #2		; 5
		ADD	R0,R1,R2,LSR #32
		ADD	R0,R1,R2,LSR R3		; 6
		ADD	R0,R1,R2,ASR #2		; 7
		ADD	R0,R1,R2,ASR #32
		ADD	R0,R1,R2,ASR R3		; 8
		ADD	R0,R1,R2,ROR #2		; 9
		ADD	R0,R1,R2,ROR #31
		ADD	R0,R1,R2,ROR R3		; 10
		ADD	R0,R1,R2,RRX		; 11

		ADDS	R0,R1,#0		; 1
		ADDS	R0,R1,#4
		ADDS	R0,R1,#&FF
		ADDS	R0,R1,#&3F0
		ADDS	R0,R1,R2		; 2
		ADDS	R0,R1,R2,LSL #2		; 3
		ADDS	R0,R1,R2,LSL #31
		ADDS	R0,R1,R2,LSL R3		; 4
		ADDS	R0,R1,R2,LSR #2		; 5
		ADDS	R0,R1,R2,LSR #32
		ADDS	R0,R1,R2,LSR R3		; 6
		ADDS	R0,R1,R2,ASR #2		; 7
		ADDS	R0,R1,R2,ASR #32
		ADDS	R0,R1,R2,ASR R3		; 8
		ADDS	R0,R1,R2,ROR #2		; 9
		ADDS	R0,R1,R2,ROR #31
		ADDS	R0,R1,R2,ROR R3		; 10
		ADDS	R0,R1,R2,RRX		; 11
	|
		DCD	&E2810000
		DCD	&E2810004
		DCD	&E28100FF
		DCD	&E2810E3F
		DCD	&E0810002
		DCD	&E0810102
		DCD	&E0810F82
		DCD	&E0810312
		DCD	&E0810122
		DCD	&E0810022
		DCD	&E0810332
		DCD	&E0810142
		DCD	&E0810042
		DCD	&E0810352
		DCD	&E0810162
		DCD	&E0810FE2
		DCD	&E0810372
		DCD	&E0810062

		DCD	&E2910000
		DCD	&E2910004
		DCD	&E29100FF
		DCD	&E2910E3F
		DCD	&E0910002
		DCD	&E0910102
		DCD	&E0910F82
		DCD	&E0910312
		DCD	&E0910122
		DCD	&E0910022
		DCD	&E0910332
		DCD	&E0910142
		DCD	&E0910042
		DCD	&E0910352
		DCD	&E0910162
		DCD	&E0910FE2
		DCD	&E0910372
		DCD	&E0910062
	]

	[ :LNOT: REFERENCE
		AND	R0,R1,#0		; 1
		AND	R0,R1,#4
		AND	R0,R1,#&FF
		AND	R0,R1,#&3F0
		AND	R0,R1,R2		; 2
		AND	R0,R1,R2,LSL #2		; 3
		AND	R0,R1,R2,LSL #31
		AND	R0,R1,R2,LSL R3		; 4
		AND	R0,R1,R2,LSR #2		; 5
		AND	R0,R1,R2,LSR #32
		AND	R0,R1,R2,LSR R3		; 6
		AND	R0,R1,R2,ASR #2		; 7
		AND	R0,R1,R2,ASR #32
		AND	R0,R1,R2,ASR R3		; 8
		AND	R0,R1,R2,ROR #2		; 9
		AND	R0,R1,R2,ROR #31
		AND	R0,R1,R2,ROR R3		; 10
		AND	R0,R1,R2,RRX		; 11

		ANDS	R0,R1,#0		; 1
		ANDS	R0,R1,#4
		ANDS	R0,R1,#&FF
		ANDS	R0,R1,#&3F0
		ANDS	R0,R1,R2		; 2
		ANDS	R0,R1,R2,LSL #2		; 3
		ANDS	R0,R1,R2,LSL #31
		ANDS	R0,R1,R2,LSL R3		; 4
		ANDS	R0,R1,R2,LSR #2		; 5
		ANDS	R0,R1,R2,LSR #32
		ANDS	R0,R1,R2,LSR R3		; 6
		ANDS	R0,R1,R2,ASR #2		; 7
		ANDS	R0,R1,R2,ASR #32
		ANDS	R0,R1,R2,ASR R3		; 8
		ANDS	R0,R1,R2,ROR #2		; 9
		ANDS	R0,R1,R2,ROR #31
		ANDS	R0,R1,R2,ROR R3		; 10
		ANDS	R0,R1,R2,RRX		; 11
	|
		DCD	&E2010000
		DCD	&E2010004
		DCD	&E20100FF
		DCD	&E2010E3F
		DCD	&E0010002
		DCD	&E0010102
		DCD	&E0010F82
		DCD	&E0010312
		DCD	&E0010122
		DCD	&E0010022
		DCD	&E0010332
		DCD	&E0010142
		DCD	&E0010042
		DCD	&E0010352
		DCD	&E0010162
		DCD	&E0010FE2
		DCD	&E0010372
		DCD	&E0010062

		DCD	&E2110000
		DCD	&E2110004
		DCD	&E21100FF
		DCD	&E2110E3F
		DCD	&E0110002
		DCD	&E0110102
		DCD	&E0110F82
		DCD	&E0110312
		DCD	&E0110122
		DCD	&E0110022
		DCD	&E0110332
		DCD	&E0110142
		DCD	&E0110042
		DCD	&E0110352
		DCD	&E0110162
		DCD	&E0110FE2
		DCD	&E0110372
		DCD	&E0110062
	]

	[ :LNOT: REFERENCE
		BIC	R0,R1,#0		; 1
		BIC	R0,R1,#4
		BIC	R0,R1,#&FF
		BIC	R0,R1,#&3F0
		BIC	R0,R1,R2		; 2
		BIC	R0,R1,R2,LSL #2		; 3
		BIC	R0,R1,R2,LSL #31
		BIC	R0,R1,R2,LSL R3		; 4
		BIC	R0,R1,R2,LSR #2		; 5
		BIC	R0,R1,R2,LSR #32
		BIC	R0,R1,R2,LSR R3		; 6
		BIC	R0,R1,R2,ASR #2		; 7
		BIC	R0,R1,R2,ASR #32
		BIC	R0,R1,R2,ASR R3		; 8
		BIC	R0,R1,R2,ROR #2		; 9
		BIC	R0,R1,R2,ROR #31
		BIC	R0,R1,R2,ROR R3		; 10
		BIC	R0,R1,R2,RRX		; 11

		BICS	R0,R1,#0		; 1
		BICS	R0,R1,#4
		BICS	R0,R1,#&FF
		BICS	R0,R1,#&3F0
		BICS	R0,R1,R2		; 2
		BICS	R0,R1,R2,LSL #2		; 3
		BICS	R0,R1,R2,LSL #31
		BICS	R0,R1,R2,LSL R3		; 4
		BICS	R0,R1,R2,LSR #2		; 5
		BICS	R0,R1,R2,LSR #32
		BICS	R0,R1,R2,LSR R3		; 6
		BICS	R0,R1,R2,ASR #2		; 7
		BICS	R0,R1,R2,ASR #32
		BICS	R0,R1,R2,ASR R3		; 8
		BICS	R0,R1,R2,ROR #2		; 9
		BICS	R0,R1,R2,ROR #31
		BICS	R0,R1,R2,ROR R3		; 10
		BICS	R0,R1,R2,RRX		; 11
	|
		DCD	&E3C10000
		DCD	&E3C10004
		DCD	&E3C100FF
		DCD	&E3C10E3F
		DCD	&E1C10002
		DCD	&E1C10102
		DCD	&E1C10F82
		DCD	&E1C10312
		DCD	&E1C10122
		DCD	&E1C10022
		DCD	&E1C10332
		DCD	&E1C10142
		DCD	&E1C10042
		DCD	&E1C10352
		DCD	&E1C10162
		DCD	&E1C10FE2
		DCD	&E1C10372
		DCD	&E1C10062

		DCD	&E3D10000
		DCD	&E3D10004
		DCD	&E3D100FF
		DCD	&E3D10E3F
		DCD	&E1D10002
		DCD	&E1D10102
		DCD	&E1D10F82
		DCD	&E1D10312
		DCD	&E1D10122
		DCD	&E1D10022
		DCD	&E1D10332
		DCD	&E1D10142
		DCD	&E1D10042
		DCD	&E1D10352
		DCD	&E1D10162
		DCD	&E1D10FE2
		DCD	&E1D10372
		DCD	&E1D10062
	]

	[ :LNOT: REFERENCE
		CMN	R0,#0			; 1
		CMN	R0,#4
		CMN	R0,#&FF
		CMN	R0,#&3F0
		CMN	R0,R1			; 2
		CMN	R0,R1,LSL #2		; 3
		CMN	R0,R1,LSL #31
		CMN	R0,R1,LSL R2		; 4
		CMN	R0,R1,LSR #2		; 5
		CMN	R0,R1,LSR #32
		CMN	R0,R1,LSR R2		; 6
		CMN	R0,R1,ASR #2		; 7
		CMN	R0,R1,ASR #32
		CMN	R0,R1,ASR R2		; 8
		CMN	R0,R1,ROR #2		; 9
		CMN	R0,R1,ROR #31
		CMN	R0,R1,ROR R2		; 10
		CMN	R0,R1,RRX		; 11
	|
		DCD	&E3700000
		DCD	&E3700004
		DCD	&E37000FF
		DCD	&E3700E3F
		DCD	&E1700001
		DCD	&E1700101
		DCD	&E1700F81
		DCD	&E1700211
		DCD	&E1700121
		DCD	&E1700021
		DCD	&E1700231
		DCD	&E1700141
		DCD	&E1700041
		DCD	&E1700251
		DCD	&E1700161
		DCD	&E1700FE1
		DCD	&E1700271
		DCD	&E1700061
	]

	[ :LNOT: REFERENCE
		CMP	R0,#0			; 1
		CMP	R0,#4
		CMP	R0,#&FF
		CMP	R0,#&3F0
		CMP	R0,R1			; 2
		CMP	R0,R1,LSL #2		; 3
		CMP	R0,R1,LSL #31
		CMP	R0,R1,LSL R2		; 4
		CMP	R0,R1,LSR #2		; 5
		CMP	R0,R1,LSR #32
		CMP	R0,R1,LSR R2		; 6
		CMP	R0,R1,ASR #2		; 7
		CMP	R0,R1,ASR #32
		CMP	R0,R1,ASR R2		; 8
		CMP	R0,R1,ROR #2		; 9
		CMP	R0,R1,ROR #31
		CMP	R0,R1,ROR R2		; 10
		CMP	R0,R1,RRX		; 11
	|
		DCD	&E3500000
		DCD	&E3500004
		DCD	&E35000FF
		DCD	&E3500E3F
		DCD	&E1500001
		DCD	&E1500101
		DCD	&E1500F81
		DCD	&E1500211
		DCD	&E1500121
		DCD	&E1500021
		DCD	&E1500231
		DCD	&E1500141
		DCD	&E1500041
		DCD	&E1500251
		DCD	&E1500161
		DCD	&E1500FE1
		DCD	&E1500271
		DCD	&E1500061
	]

	[ :LNOT: REFERENCE
		EOR	R0,R1,#0		; 1
		EOR	R0,R1,#4
		EOR	R0,R1,#&FF
		EOR	R0,R1,#&3F0
		EOR	R0,R1,R2		; 2
		EOR	R0,R1,R2,LSL #2		; 3
		EOR	R0,R1,R2,LSL #31
		EOR	R0,R1,R2,LSL R3		; 4
		EOR	R0,R1,R2,LSR #2		; 5
		EOR	R0,R1,R2,LSR #32
		EOR	R0,R1,R2,LSR R3		; 6
		EOR	R0,R1,R2,ASR #2		; 7
		EOR	R0,R1,R2,ASR #32
		EOR	R0,R1,R2,ASR R3		; 8
		EOR	R0,R1,R2,ROR #2		; 9
		EOR	R0,R1,R2,ROR #31
		EOR	R0,R1,R2,ROR R3		; 10
		EOR	R0,R1,R2,RRX		; 11

		EORS	R0,R1,#0		; 1
		EORS	R0,R1,#4
		EORS	R0,R1,#&FF
		EORS	R0,R1,#&3F0
		EORS	R0,R1,R2		; 2
		EORS	R0,R1,R2,LSL #2		; 3
		EORS	R0,R1,R2,LSL #31
		EORS	R0,R1,R2,LSL R3		; 4
		EORS	R0,R1,R2,LSR #2		; 5
		EORS	R0,R1,R2,LSR #32
		EORS	R0,R1,R2,LSR R3		; 6
		EORS	R0,R1,R2,ASR #2		; 7
		EORS	R0,R1,R2,ASR #32
		EORS	R0,R1,R2,ASR R3		; 8
		EORS	R0,R1,R2,ROR #2		; 9
		EORS	R0,R1,R2,ROR #31
		EORS	R0,R1,R2,ROR R3		; 10
		EORS	R0,R1,R2,RRX		; 11
	|
		DCD	&E2210000
		DCD	&E2210004
		DCD	&E22100FF
		DCD	&E2210E3F
		DCD	&E0210002
		DCD	&E0210102
		DCD	&E0210F82
		DCD	&E0210312
		DCD	&E0210122
		DCD	&E0210022
		DCD	&E0210332
		DCD	&E0210142
		DCD	&E0210042
		DCD	&E0210352
		DCD	&E0210162
		DCD	&E0210FE2
		DCD	&E0210372
		DCD	&E0210062

		DCD	&E2310000
		DCD	&E2310004
		DCD	&E23100FF
		DCD	&E2310E3F
		DCD	&E0310002
		DCD	&E0310102
		DCD	&E0310F82
		DCD	&E0310312
		DCD	&E0310122
		DCD	&E0310022
		DCD	&E0310332
		DCD	&E0310142
		DCD	&E0310042
		DCD	&E0310352
		DCD	&E0310162
		DCD	&E0310FE2
		DCD	&E0310372
		DCD	&E0310062
	]

	[ :LNOT: REFERENCE
		MOV	R0,#0			; 1
		MOV	R0,#4
		MOV	R0,#&FF
		MOV	R0,#&3F0
		MOV	R0,R1			; 2
		MOV	R0,R1,LSL #2		; 3
		MOV	R0,R1,LSL #31
		MOV	R0,R1,LSL R2		; 4
		MOV	R0,R1,LSR #2		; 5
		MOV	R0,R1,LSR #32
		MOV	R0,R1,LSR R2		; 6
		MOV	R0,R1,ASR #2		; 7
		MOV	R0,R1,ASR #32
		MOV	R0,R1,ASR R2		; 8
		MOV	R0,R1,ROR #2		; 9
		MOV	R0,R1,ROR #31
		MOV	R0,R1,ROR R2		; 10
		MOV	R0,R1,RRX		; 11

		MOVS	R0,#0			; 1
		MOVS	R0,#4
		MOVS	R0,#&FF
		MOVS	R0,#&3F0
		MOVS	R0,R1			; 2
		MOVS	R0,R1,LSL #2		; 3
		MOVS	R0,R1,LSL #31
		MOVS	R0,R1,LSL R2		; 4
		MOVS	R0,R1,LSR #2		; 5
		MOVS	R0,R1,LSR #32
		MOVS	R0,R1,LSR R2		; 6
		MOVS	R0,R1,ASR #2		; 7
		MOVS	R0,R1,ASR #32
		MOVS	R0,R1,ASR R2		; 8
		MOVS	R0,R1,ROR #2		; 9
		MOVS	R0,R1,ROR #31
		MOVS	R0,R1,ROR R2		; 10
		MOVS	R0,R1,RRX		; 11
	|
		DCD	&E3A00000
		DCD	&E3A00004
		DCD	&E3A000FF
		DCD	&E3A00E3F
		DCD	&E1A00001
		DCD	&E1A00101
		DCD	&E1A00F81
		DCD	&E1A00211
		DCD	&E1A00121
		DCD	&E1A00021
		DCD	&E1A00231
		DCD	&E1A00141
		DCD	&E1A00041
		DCD	&E1A00251
		DCD	&E1A00161
		DCD	&E1A00FE1
		DCD	&E1A00271
		DCD	&E1A00061

		DCD	&E3B00000
		DCD	&E3B00004
		DCD	&E3B000FF
		DCD	&E3B00E3F
		DCD	&E1B00001
		DCD	&E1B00101
		DCD	&E1B00F81
		DCD	&E1B00211
		DCD	&E1B00121
		DCD	&E1B00021
		DCD	&E1B00231
		DCD	&E1B00141
		DCD	&E1B00041
		DCD	&E1B00251
		DCD	&E1B00161
		DCD	&E1B00FE1
		DCD	&E1B00271
		DCD	&E1B00061
	]

	[ :LNOT: REFERENCE
		MVN	R0,#0			; 1
		MVN	R0,#4
		MVN	R0,#&FF
		MVN	R0,#&3F0
		MVN	R0,R1			; 2
		MVN	R0,R1,LSL #2		; 3
		MVN	R0,R1,LSL #31
		MVN	R0,R1,LSL R2		; 4
		MVN	R0,R1,LSR #2		; 5
		MVN	R0,R1,LSR #32
		MVN	R0,R1,LSR R2		; 6
		MVN	R0,R1,ASR #2		; 7
		MVN	R0,R1,ASR #32
		MVN	R0,R1,ASR R2		; 8
		MVN	R0,R1,ROR #2		; 9
		MVN	R0,R1,ROR #31
		MVN	R0,R1,ROR R2		; 10
		MVN	R0,R1,RRX		; 11

		MVNS	R0,#0			; 1
		MVNS	R0,#4
		MVNS	R0,#&FF
		MVNS	R0,#&3F0
		MVNS	R0,R1			; 2
		MVNS	R0,R1,LSL #2		; 3
		MVNS	R0,R1,LSL #31
		MVNS	R0,R1,LSL R2		; 4
		MVNS	R0,R1,LSR #2		; 5
		MVNS	R0,R1,LSR #32
		MVNS	R0,R1,LSR R2		; 6
		MVNS	R0,R1,ASR #2		; 7
		MVNS	R0,R1,ASR #32
		MVNS	R0,R1,ASR R2		; 8
		MVNS	R0,R1,ROR #2		; 9
		MVNS	R0,R1,ROR #31
		MVNS	R0,R1,ROR R2		; 10
		MVNS	R0,R1,RRX		; 11
	|
		DCD	&E3E00000
		DCD	&E3E00004
		DCD	&E3E000FF
		DCD	&E3E00E3F
		DCD	&E1E00001
		DCD	&E1E00101
		DCD	&E1E00F81
		DCD	&E1E00211
		DCD	&E1E00121
		DCD	&E1E00021
		DCD	&E1E00231
		DCD	&E1E00141
		DCD	&E1E00041
		DCD	&E1E00251
		DCD	&E1E00161
		DCD	&E1E00FE1
		DCD	&E1E00271
		DCD	&E1E00061

		DCD	&E3F00000
		DCD	&E3F00004
		DCD	&E3F000FF
		DCD	&E3F00E3F
		DCD	&E1F00001
		DCD	&E1F00101
		DCD	&E1F00F81
		DCD	&E1F00211
		DCD	&E1F00121
		DCD	&E1F00021
		DCD	&E1F00231
		DCD	&E1F00141
		DCD	&E1F00041
		DCD	&E1F00251
		DCD	&E1F00161
		DCD	&E1F00FE1
		DCD	&E1F00271
		DCD	&E1F00061
	]

	[ :LNOT: REFERENCE
		ORR	R0,R1,#0		; 1
		ORR	R0,R1,#4
		ORR	R0,R1,#&FF
		ORR	R0,R1,#&3F0
		ORR	R0,R1,R2		; 2
		ORR	R0,R1,R2,LSL #2		; 3
		ORR	R0,R1,R2,LSL #31
		ORR	R0,R1,R2,LSL R3		; 4
		ORR	R0,R1,R2,LSR #2		; 5
		ORR	R0,R1,R2,LSR #32
		ORR	R0,R1,R2,LSR R3		; 6
		ORR	R0,R1,R2,ASR #2		; 7
		ORR	R0,R1,R2,ASR #32
		ORR	R0,R1,R2,ASR R3		; 8
		ORR	R0,R1,R2,ROR #2		; 9
		ORR	R0,R1,R2,ROR #31
		ORR	R0,R1,R2,ROR R3		; 10
		ORR	R0,R1,R2,RRX		; 11

		ORRS	R0,R1,#0		; 1
		ORRS	R0,R1,#4
		ORRS	R0,R1,#&FF
		ORRS	R0,R1,#&3F0
		ORRS	R0,R1,R2		; 2
		ORRS	R0,R1,R2,LSL #2		; 3
		ORRS	R0,R1,R2,LSL #31
		ORRS	R0,R1,R2,LSL R3		; 4
		ORRS	R0,R1,R2,LSR #2		; 5
		ORRS	R0,R1,R2,LSR #32
		ORRS	R0,R1,R2,LSR R3		; 6
		ORRS	R0,R1,R2,ASR #2		; 7
		ORRS	R0,R1,R2,ASR #32
		ORRS	R0,R1,R2,ASR R3		; 8
		ORRS	R0,R1,R2,ROR #2		; 9
		ORRS	R0,R1,R2,ROR #31
		ORRS	R0,R1,R2,ROR R3		; 10
		ORRS	R0,R1,R2,RRX		; 11
	|
		DCD	&E3810000
		DCD	&E3810004
		DCD	&E38100FF
		DCD	&E3810E3F
		DCD	&E1810002
		DCD	&E1810102
		DCD	&E1810F82
		DCD	&E1810312
		DCD	&E1810122
		DCD	&E1810022
		DCD	&E1810332
		DCD	&E1810142
		DCD	&E1810042
		DCD	&E1810352
		DCD	&E1810162
		DCD	&E1810FE2
		DCD	&E1810372
		DCD	&E1810062

		DCD	&E3910000
		DCD	&E3910004
		DCD	&E39100FF
		DCD	&E3910E3F
		DCD	&E1910002
		DCD	&E1910102
		DCD	&E1910F82
		DCD	&E1910312
		DCD	&E1910122
		DCD	&E1910022
		DCD	&E1910332
		DCD	&E1910142
		DCD	&E1910042
		DCD	&E1910352
		DCD	&E1910162
		DCD	&E1910FE2
		DCD	&E1910372
		DCD	&E1910062
	]

	[ :LNOT: REFERENCE
		RSB	R0,R1,#0		; 1
		RSB	R0,R1,#4
		RSB	R0,R1,#&FF
		RSB	R0,R1,#&3F0
		RSB	R0,R1,R2		; 2
		RSB	R0,R1,R2,LSL #2		; 3
		RSB	R0,R1,R2,LSL #31
		RSB	R0,R1,R2,LSL R3		; 4
		RSB	R0,R1,R2,LSR #2		; 5
		RSB	R0,R1,R2,LSR #32
		RSB	R0,R1,R2,LSR R3		; 6
		RSB	R0,R1,R2,ASR #2		; 7
		RSB	R0,R1,R2,ASR #32
		RSB	R0,R1,R2,ASR R3		; 8
		RSB	R0,R1,R2,ROR #2		; 9
		RSB	R0,R1,R2,ROR #31
		RSB	R0,R1,R2,ROR R3		; 10
		RSB	R0,R1,R2,RRX		; 11

		RSBS	R0,R1,#0		; 1
		RSBS	R0,R1,#4
		RSBS	R0,R1,#&FF
		RSBS	R0,R1,#&3F0
		RSBS	R0,R1,R2		; 2
		RSBS	R0,R1,R2,LSL #2		; 3
		RSBS	R0,R1,R2,LSL #31
		RSBS	R0,R1,R2,LSL R3		; 4
		RSBS	R0,R1,R2,LSR #2		; 5
		RSBS	R0,R1,R2,LSR #32
		RSBS	R0,R1,R2,LSR R3		; 6
		RSBS	R0,R1,R2,ASR #2		; 7
		RSBS	R0,R1,R2,ASR #32
		RSBS	R0,R1,R2,ASR R3		; 8
		RSBS	R0,R1,R2,ROR #2		; 9
		RSBS	R0,R1,R2,ROR #31
		RSBS	R0,R1,R2,ROR R3		; 10
		RSBS	R0,R1,R2,RRX		; 11
	|
		DCD	&E2610000
		DCD	&E2610004
		DCD	&E26100FF
		DCD	&E2610E3F
		DCD	&E0610002
		DCD	&E0610102
		DCD	&E0610F82
		DCD	&E0610312
		DCD	&E0610122
		DCD	&E0610022
		DCD	&E0610332
		DCD	&E0610142
		DCD	&E0610042
		DCD	&E0610352
		DCD	&E0610162
		DCD	&E0610FE2
		DCD	&E0610372
		DCD	&E0610062

		DCD	&E2710000
		DCD	&E2710004
		DCD	&E27100FF
		DCD	&E2710E3F
		DCD	&E0710002
		DCD	&E0710102
		DCD	&E0710F82
		DCD	&E0710312
		DCD	&E0710122
		DCD	&E0710022
		DCD	&E0710332
		DCD	&E0710142
		DCD	&E0710042
		DCD	&E0710352
		DCD	&E0710162
		DCD	&E0710FE2
		DCD	&E0710372
		DCD	&E0710062
	]

	[ :LNOT: REFERENCE
		RSC	R0,R1,#0		; 1
		RSC	R0,R1,#4
		RSC	R0,R1,#&FF
		RSC	R0,R1,#&3F0
		RSC	R0,R1,R2		; 2
		RSC	R0,R1,R2,LSL #2		; 3
		RSC	R0,R1,R2,LSL #31
		RSC	R0,R1,R2,LSL R3		; 4
		RSC	R0,R1,R2,LSR #2		; 5
		RSC	R0,R1,R2,LSR #32
		RSC	R0,R1,R2,LSR R3		; 6
		RSC	R0,R1,R2,ASR #2		; 7
		RSC	R0,R1,R2,ASR #32
		RSC	R0,R1,R2,ASR R3		; 8
		RSC	R0,R1,R2,ROR #2		; 9
		RSC	R0,R1,R2,ROR #31
		RSC	R0,R1,R2,ROR R3		; 10
		RSC	R0,R1,R2,RRX		; 11

		RSCS	R0,R1,#0		; 1
		RSCS	R0,R1,#4
		RSCS	R0,R1,#&FF
		RSCS	R0,R1,#&3F0
		RSCS	R0,R1,R2		; 2
		RSCS	R0,R1,R2,LSL #2		; 3
		RSCS	R0,R1,R2,LSL #31
		RSCS	R0,R1,R2,LSL R3		; 4
		RSCS	R0,R1,R2,LSR #2		; 5
		RSCS	R0,R1,R2,LSR #32
		RSCS	R0,R1,R2,LSR R3		; 6
		RSCS	R0,R1,R2,ASR #2		; 7
		RSCS	R0,R1,R2,ASR #32
		RSCS	R0,R1,R2,ASR R3		; 8
		RSCS	R0,R1,R2,ROR #2		; 9
		RSCS	R0,R1,R2,ROR #31
		RSCS	R0,R1,R2,ROR R3		; 10
		RSCS	R0,R1,R2,RRX		; 11
	|
		DCD	&E2E10000
		DCD	&E2E10004
		DCD	&E2E100FF
		DCD	&E2E10E3F
		DCD	&E0E10002
		DCD	&E0E10102
		DCD	&E0E10F82
		DCD	&E0E10312
		DCD	&E0E10122
		DCD	&E0E10022
		DCD	&E0E10332
		DCD	&E0E10142
		DCD	&E0E10042
		DCD	&E0E10352
		DCD	&E0E10162
		DCD	&E0E10FE2
		DCD	&E0E10372
		DCD	&E0E10062

		DCD	&E2F10000
		DCD	&E2F10004
		DCD	&E2F100FF
		DCD	&E2F10E3F
		DCD	&E0F10002
		DCD	&E0F10102
		DCD	&E0F10F82
		DCD	&E0F10312
		DCD	&E0F10122
		DCD	&E0F10022
		DCD	&E0F10332
		DCD	&E0F10142
		DCD	&E0F10042
		DCD	&E0F10352
		DCD	&E0F10162
		DCD	&E0F10FE2
		DCD	&E0F10372
		DCD	&E0F10062
	]

	[ :LNOT: REFERENCE
		SBC	R0,R1,#0		; 1
		SBC	R0,R1,#4
		SBC	R0,R1,#&FF
		SBC	R0,R1,#&3F0
		SBC	R0,R1,R2		; 2
		SBC	R0,R1,R2,LSL #2		; 3
		SBC	R0,R1,R2,LSL #31
		SBC	R0,R1,R2,LSL R3		; 4
		SBC	R0,R1,R2,LSR #2		; 5
		SBC	R0,R1,R2,LSR #32
		SBC	R0,R1,R2,LSR R3		; 6
		SBC	R0,R1,R2,ASR #2		; 7
		SBC	R0,R1,R2,ASR #32
		SBC	R0,R1,R2,ASR R3		; 8
		SBC	R0,R1,R2,ROR #2		; 9
		SBC	R0,R1,R2,ROR #31
		SBC	R0,R1,R2,ROR R3		; 10
		SBC	R0,R1,R2,RRX		; 11

		SBCS	R0,R1,#0		; 1
		SBCS	R0,R1,#4
		SBCS	R0,R1,#&FF
		SBCS	R0,R1,#&3F0
		SBCS	R0,R1,R2		; 2
		SBCS	R0,R1,R2,LSL #2		; 3
		SBCS	R0,R1,R2,LSL #31
		SBCS	R0,R1,R2,LSL R3		; 4
		SBCS	R0,R1,R2,LSR #2		; 5
		SBCS	R0,R1,R2,LSR #32
		SBCS	R0,R1,R2,LSR R3		; 6
		SBCS	R0,R1,R2,ASR #2		; 7
		SBCS	R0,R1,R2,ASR #32
		SBCS	R0,R1,R2,ASR R3		; 8
		SBCS	R0,R1,R2,ROR #2		; 9
		SBCS	R0,R1,R2,ROR #31
		SBCS	R0,R1,R2,ROR R3		; 10
		SBCS	R0,R1,R2,RRX		; 11
	|
		DCD	&E2C10000
		DCD	&E2C10004
		DCD	&E2C100FF
		DCD	&E2C10E3F
		DCD	&E0C10002
		DCD	&E0C10102
		DCD	&E0C10F82
		DCD	&E0C10312
		DCD	&E0C10122
		DCD	&E0C10022
		DCD	&E0C10332
		DCD	&E0C10142
		DCD	&E0C10042
		DCD	&E0C10352
		DCD	&E0C10162
		DCD	&E0C10FE2
		DCD	&E0C10372
		DCD	&E0C10062

		DCD	&E2D10000
		DCD	&E2D10004
		DCD	&E2D100FF
		DCD	&E2D10E3F
		DCD	&E0D10002
		DCD	&E0D10102
		DCD	&E0D10F82
		DCD	&E0D10312
		DCD	&E0D10122
		DCD	&E0D10022
		DCD	&E0D10332
		DCD	&E0D10142
		DCD	&E0D10042
		DCD	&E0D10352
		DCD	&E0D10162
		DCD	&E0D10FE2
		DCD	&E0D10372
		DCD	&E0D10062
	]

	[ :LNOT: REFERENCE
		SUB	R0,R1,#0		; 1
		SUB	R0,R1,#4
		SUB	R0,R1,#&FF
		SUB	R0,R1,#&3F0
		SUB	R0,R1,R2		; 2
		SUB	R0,R1,R2,LSL #2		; 3
		SUB	R0,R1,R2,LSL #31
		SUB	R0,R1,R2,LSL R3		; 4
		SUB	R0,R1,R2,LSR #2		; 5
		SUB	R0,R1,R2,LSR #32
		SUB	R0,R1,R2,LSR R3		; 6
		SUB	R0,R1,R2,ASR #2		; 7
		SUB	R0,R1,R2,ASR #32
		SUB	R0,R1,R2,ASR R3		; 8
		SUB	R0,R1,R2,ROR #2		; 9
		SUB	R0,R1,R2,ROR #31
		SUB	R0,R1,R2,ROR R3		; 10
		SUB	R0,R1,R2,RRX		; 11

		SUBS	R0,R1,#0		; 1
		SUBS	R0,R1,#4
		SUBS	R0,R1,#&FF
		SUBS	R0,R1,#&3F0
		SUBS	R0,R1,R2		; 2
		SUBS	R0,R1,R2,LSL #2		; 3
		SUBS	R0,R1,R2,LSL #31
		SUBS	R0,R1,R2,LSL R3		; 4
		SUBS	R0,R1,R2,LSR #2		; 5
		SUBS	R0,R1,R2,LSR #32
		SUBS	R0,R1,R2,LSR R3		; 6
		SUBS	R0,R1,R2,ASR #2		; 7
		SUBS	R0,R1,R2,ASR #32
		SUBS	R0,R1,R2,ASR R3		; 8
		SUBS	R0,R1,R2,ROR #2		; 9
		SUBS	R0,R1,R2,ROR #31
		SUBS	R0,R1,R2,ROR R3		; 10
		SUBS	R0,R1,R2,RRX		; 11
	|
		DCD	&E2410000
		DCD	&E2410004
		DCD	&E24100FF
		DCD	&E2410E3F
		DCD	&E0410002
		DCD	&E0410102
		DCD	&E0410F82
		DCD	&E0410312
		DCD	&E0410122
		DCD	&E0410022
		DCD	&E0410332
		DCD	&E0410142
		DCD	&E0410042
		DCD	&E0410352
		DCD	&E0410162
		DCD	&E0410FE2
		DCD	&E0410372
		DCD	&E0410062

		DCD	&E2510000
		DCD	&E2510004
		DCD	&E25100FF
		DCD	&E2510E3F
		DCD	&E0510002
		DCD	&E0510102
		DCD	&E0510F82
		DCD	&E0510312
		DCD	&E0510122
		DCD	&E0510022
		DCD	&E0510332
		DCD	&E0510142
		DCD	&E0510042
		DCD	&E0510352
		DCD	&E0510162
		DCD	&E0510FE2
		DCD	&E0510372
		DCD	&E0510062
	]

	[ :LNOT: REFERENCE
		TEQ	R0,#0			; 1
		TEQ	R0,#4
		TEQ	R0,#&FF
		TEQ	R0,#&3F0
		TEQ	R0,R1			; 2
		TEQ	R0,R1,LSL #2		; 3
		TEQ	R0,R1,LSL #31
		TEQ	R0,R1,LSL R2		; 4
		TEQ	R0,R1,LSR #2		; 5
		TEQ	R0,R1,LSR #32
		TEQ	R0,R1,LSR R2		; 6
		TEQ	R0,R1,ASR #2		; 7
		TEQ	R0,R1,ASR #32
		TEQ	R0,R1,ASR R2		; 8
		TEQ	R0,R1,ROR #2		; 9
		TEQ	R0,R1,ROR #31
		TEQ	R0,R1,ROR R2		; 10
		TEQ	R0,R1,RRX		; 11
	|
		DCD	&E3300000
		DCD	&E3300004
		DCD	&E33000FF
		DCD	&E3300E3F
		DCD	&E1300001
		DCD	&E1300101
		DCD	&E1300F81
		DCD	&E1300211
		DCD	&E1300121
		DCD	&E1300021
		DCD	&E1300231
		DCD	&E1300141
		DCD	&E1300041
		DCD	&E1300251
		DCD	&E1300161
		DCD	&E1300FE1
		DCD	&E1300271
		DCD	&E1300061
	]

	[ :LNOT: REFERENCE
		TST	R0,#0			; 1
		TST	R0,#4
		TST	R0,#&FF
		TST	R0,#&3F0
		TST	R0,R1			; 2
		TST	R0,R1,LSL #2		; 3
		TST	R0,R1,LSL #31
		TST	R0,R1,LSL R2		; 4
		TST	R0,R1,LSR #2		; 5
		TST	R0,R1,LSR #32
		TST	R0,R1,LSR R2		; 6
		TST	R0,R1,ASR #2		; 7
		TST	R0,R1,ASR #32
		TST	R0,R1,ASR R2		; 8
		TST	R0,R1,ROR #2		; 9
		TST	R0,R1,ROR #31
		TST	R0,R1,ROR R2		; 10
		TST	R0,R1,RRX		; 11
	|
		DCD	&E3100000
		DCD	&E3100004
		DCD	&E31000FF
		DCD	&E3100E3F
		DCD	&E1100001
		DCD	&E1100101
		DCD	&E1100F81
		DCD	&E1100211
		DCD	&E1100121
		DCD	&E1100021
		DCD	&E1100231
		DCD	&E1100141
		DCD	&E1100041
		DCD	&E1100251
		DCD	&E1100161
		DCD	&E1100FE1
		DCD	&E1100271
		DCD	&E1100061
	]

		END
