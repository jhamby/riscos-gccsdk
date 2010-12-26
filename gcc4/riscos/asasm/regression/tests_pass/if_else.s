; Tests [ | ] IF ELSE ELIF ENDIF

	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE

	; Using [ | ]
	; One level:
	[ {TRUE}	; Comment
	DCD	&100
	]		; Comment
	[ {FALSE}
	ASSERT {FALSE}, "May not be assembled"
	]
	[ {TRUE}
	DCD	&101
	|		; Comment
	ASSERT {FALSE}, "May not be assembled"
	]
	[ {FALSE}
	ASSERT {FALSE}, "May not be assembled"
	|
	DCD	&102
	]

	; Two levels:
	[ {TRUE}
		[ {TRUE}
		DCD	&200
		]
		[ {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {TRUE}	; Comment
		DCD	&201
		|		; Comment
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		|		; Comment
		DCD	&202
		]		; Comment
	|
		ASSERT {FALSE}, "May not be assembled"
	]
	[ {TRUE}
		DCD	&210
	|
		[ {TRUE}
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {FALSE}	; Comment
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {TRUE}
		ASSERT {FALSE}, "May not be assembled"
		|		; Comment
		ASSERT {FALSE}, "May not be assembled"
		]		; Comment
		[ {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		|
		ASSERT {FALSE}, "May not be assembled"
		]
	]
	[ {FALSE}
		ASSERT {FALSE}, "May not be assembled"
	|
		[ {TRUE}
		DCD	&220
		]
		[ {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {TRUE}
		DCD	&221
		|
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		|
		DCD	&222
		]
	]
	[ {FALSE}
		[ {TRUE}
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {TRUE}
		ASSERT {FALSE}, "May not be assembled"
		|
		ASSERT {FALSE}, "May not be assembled"
		]
		[ {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		|
		ASSERT {FALSE}, "May not be assembled"
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
	ASSERT {FALSE}, "May not be assembled"
	ENDIF
	IF {TRUE}
	DCD	&101
	ELSE		; Comment
	ASSERT {FALSE}, "May not be assembled"
	ENDIF
	IF {FALSE}
	ASSERT {FALSE}, "May not be assembled"
	ELSE
	DCD	&102
	ENDIF

	; Two levels:
	IF {TRUE}
		IF {TRUE}
		DCD	&200
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {TRUE}	; Comment
		DCD	&201
		ELSE		; Comment
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		ELSE		; Comment
		DCD	&202
		ENDIF		; Comment
	ELSE
		ASSERT {FALSE}, "May not be assembled"
	ENDIF
	IF {TRUE}
		DCD	&210
	ELSE
		IF {TRUE}
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {FALSE}	; Comment
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {TRUE}
		ASSERT {FALSE}, "May not be assembled"
		ELSE		; Comment
		ASSERT {FALSE}, "May not be assembled"
		ENDIF		; Comment
		IF {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		ELSE
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
	ENDIF
	IF {FALSE}
		ASSERT {FALSE}, "May not be assembled"
	ELSE
		IF {TRUE}
		DCD	&220
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {TRUE}
		DCD	&221
		ELSE
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		ELSE
		DCD	&222
		ENDIF
	ENDIF
	IF {FALSE}
		IF {TRUE}
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {TRUE}
		ASSERT {FALSE}, "May not be assembled"
		ELSE
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
		IF {FALSE}
		ASSERT {FALSE}, "May not be assembled"
		ELSE
		ASSERT {FALSE}, "May not be assembled"
		ENDIF
	ELSE
		DCD	&230
	ENDIF

	; Level 1:
	IF	{TRUE}
		DCD	&400
	ELIF	{TRUE}
		ASSERT {FALSE}, "May not be assembled"
	ELIF	{TRUE}
		ASSERT {FALSE}, "May not be assembled"
	ELSE
		ASSERT {FALSE}, "May not be assembled"
	ENDIF
	IF	{FALSE}
		ASSERT {FALSE}, "May not be assembled"
	ELIF	{TRUE}
		DCD	&401
	ELIF	{TRUE}
		ASSERT {FALSE}, "May not be assembled"
	ELSE
		ASSERT {FALSE}, "May not be assembled"
	ENDIF
	IF	{FALSE}
		ASSERT {FALSE}, "May not be assembled"
	ELIF	{FALSE}
		ASSERT {FALSE}, "May not be assembled"
	ELIF	{TRUE}
		DCD	&402
	ELSE
		ASSERT {FALSE}, "May not be assembled"
	ENDIF
	IF	{FALSE}
		ASSERT {FALSE}, "May not be assembled"
	ELIF	{FALSE}
		ASSERT {FALSE}, "May not be assembled"
	ELIF	{FALSE}
		ASSERT {FALSE}, "May not be assembled"
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

	END
