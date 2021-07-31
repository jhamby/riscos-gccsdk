; Tests [ | ] IF ELSE ELIF ENDIF

	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE

	; Using [ | ]
	; One level:
	[ {TRUE}	; Comment
	DCD	&100
	]		; Comment
	[ {FALSE}
	ASSERT {FALSE}
	]
	[ {TRUE}
	DCD	&101
	|		; Comment
	ASSERT {FALSE}
	]
	[ {FALSE}
	ASSERT {FALSE}
	|
	DCD	&102
	]

	; Two levels:
	[ {TRUE}
		[ {TRUE}
		DCD	&200
		]
		[ {FALSE}
		ASSERT {FALSE}
		]
		[ {TRUE}	; Comment
		DCD	&201
		|		; Comment
		ASSERT {FALSE}
		]
		[ {FALSE}
		ASSERT {FALSE}
		|		; Comment
		DCD	&202
		]		; Comment
	|
		ASSERT {FALSE}
	]
	[ {TRUE}
		DCD	&210
	|
		[ {TRUE}
		ASSERT {FALSE}
		]
		[ {FALSE}	; Comment
		ASSERT {FALSE}
		]
		[ {TRUE}
		ASSERT {FALSE}
		|		; Comment
		ASSERT {FALSE}
		]		; Comment
		[ {FALSE}
		ASSERT {FALSE}
		|
		ASSERT {FALSE}
		]
	]
	[ {FALSE}
		ASSERT {FALSE}
	|
		[ {TRUE}
		DCD	&220
		]
		[ {FALSE}
		ASSERT {FALSE}
		]
		[ {TRUE}
		DCD	&221
		|
		ASSERT {FALSE}
		]
		[ {FALSE}
		ASSERT {FALSE}
		|
		DCD	&222
		]
	]
	[ {FALSE}
		[ {TRUE}
		ASSERT {FALSE}
		]
		[ {FALSE}
		ASSERT {FALSE}
		]
		[ {TRUE}
		ASSERT {FALSE}
		|
		ASSERT {FALSE}
		]
		[ {FALSE}
		ASSERT {FALSE}
		|
		ASSERT {FALSE}
		]
	|
		DCD	&230
	]

	; Using IF ELSE ENDIF
	; One level:
	IF {TRUE}	; Comment
	DCD	&100
	ENDIF		; Comment
	IF {FALSE}
	ASSERT {FALSE}
	ENDIF
	IF {TRUE}
	DCD	&101
	ELSE		; Comment
	ASSERT {FALSE}
	ENDIF
	IF {FALSE}
	ASSERT {FALSE}
	ELSE
	DCD	&102
	ENDIF

	; Two levels:
	IF {TRUE}
		IF {TRUE}
		DCD	&200
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}
		ENDIF
		IF {TRUE}	; Comment
		DCD	&201
		ELSE		; Comment
		ASSERT {FALSE}
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}
		ELSE		; Comment
		DCD	&202
		ENDIF		; Comment
	ELSE
		ASSERT {FALSE}
	ENDIF
	IF {TRUE}
		DCD	&210
	ELSE
		IF {TRUE}
		ASSERT {FALSE}
		ENDIF
		IF {FALSE}	; Comment
		ASSERT {FALSE}
		ENDIF
		IF {TRUE}
		ASSERT {FALSE}
		ELSE		; Comment
		ASSERT {FALSE}
		ENDIF		; Comment
		IF {FALSE}
		ASSERT {FALSE}
		ELSE
		ASSERT {FALSE}
		ENDIF
	ENDIF
	IF {FALSE}
		ASSERT {FALSE}
	ELSE
		IF {TRUE}
		DCD	&220
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}
		ENDIF
		IF {TRUE}
		DCD	&221
		ELSE
		ASSERT {FALSE}
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}
		ELSE
		DCD	&222
		ENDIF
	ENDIF
	IF {FALSE}
		IF {TRUE}
		ASSERT {FALSE}
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}
		ENDIF
		IF {TRUE}
		ASSERT {FALSE}
		ELSE
		ASSERT {FALSE}
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}
		ELSE
		ASSERT {FALSE}
		ENDIF
	ELSE
		DCD	&230
	ENDIF

	; Level 1:
	IF	{TRUE}
		DCD	&400
	ELIF	{TRUE}
		ASSERT {FALSE}
	ELIF	{TRUE}
		ASSERT {FALSE}
	ELSE
		ASSERT {FALSE}
	ENDIF
	IF	{FALSE}
		ASSERT {FALSE}
	ELIF	{TRUE}
		DCD	&401
	ELIF	{TRUE}
		ASSERT {FALSE}
	ELSE
		ASSERT {FALSE}
	ENDIF
	IF	{FALSE}
		ASSERT {FALSE}
	ELIF	{FALSE}
		ASSERT {FALSE}
	ELIF	{TRUE}
		DCD	&402
	ELSE
		ASSERT {FALSE}
	ENDIF
	IF	{FALSE}
		ASSERT {FALSE}
	ELIF	{FALSE}
		ASSERT {FALSE}
	ELIF	{FALSE}
		ASSERT {FALSE}
	ELSE
		DCD	&403
	ENDIF

	|

	; Using [ | ]
	; One level:
	DCD	&100
	DCD	&101
	DCD	&102

	; Two levels:
	DCD	&200
	DCD	&201
	DCD	&202
	DCD	&210
	DCD	&220
	DCD	&221
	DCD	&222
	DCD	&230

	; Using IF ELSE ENDIF
	; One level:
	DCD	&100
	DCD	&101
	DCD	&102

	; Two levels:
	DCD	&200
	DCD	&201
	DCD	&202
	DCD	&210
	DCD	&220
	DCD	&221
	DCD	&222
	DCD	&230

	; Level 1:
	DCD	&400
	DCD	&401
	DCD	&402
	DCD	&403

	]

	GBLS CloseIF
CloseIF SETS "]"
	[ :LNOT: REFERENCE
	[ {FALSE}
	$CloseIF	; Should close the "[ {FALSE}" construction.
	|
	]

	END
