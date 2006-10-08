@;* utils.h
@;* by Lee Noar
@;* 28 June, 2004

@;*	r0 = addr to fill from
@;*	r1 = number of bytes
@;*	r2 = byte to fill with
@	IMPORT	fillmem

@;*	r0 = dst addr
@;*	r1 = src addr
@;*	r2 = number of bytes
@;*	code from unixlib version used
@	IMPORT	strncpy

@;* entry:
@;*	r0 = s1
@;*	r1 = s2
@;* exit:
@;*	r0 = 0 if s1 == s2, < 0 if s1 < s2, > 0 s1 > s2
@;*	code from unixlib version used
@	IMPORT	strcmp

@;*	r0 = s1
@;*	r1 = s2
@;*	code from unixlib version used
@	IMPORT	strcpy

@;* entry:
@;*	r0 = s
@;* exit:
@;*	r0 = length of s not including terminator
@;*	code from unixlib version used
@	IMPORT	strlen
