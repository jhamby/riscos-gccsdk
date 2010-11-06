	AREA	Code, CODE, READONLY

	[ :LNOT: REFERENCE

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

	|

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

	]

	END

