@ _dl_start.s
@ Short entry point function to make accessing the data on the stack easier.
@ Written by Lee Noar

	.global	_dl_start
_dl_start:
	MOV	r0, sp
	B	_dl_boot

	.type	_dl_start, %function
	.size	_dl_start, . - _dl_start

	.end
