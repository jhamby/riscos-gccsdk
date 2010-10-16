	^	0
foo	#	4
bar	#	8
baz	#	16
max	*	:INDEX:@

	[ max <> 28
		! 1, "INDEX failure"
	]

	[ :INDEX:@ <> 28
		! 1, "INDEX failure"
	]

	END

