@;* so-module.h

@;===================================================================
@; Constants
@;===================================================================

.equ	SOM_ERROR_BASE,			0x81dc00

.equ	SOM_REGISTER_LOADER,		0
.equ	SOM_REGISTER_CLIENT,		1
.equ	SOM_REGISTER_LIBRARY,		2

.equ	SOM_QUERY_OBJECT_CLIENT,	1

.equ	SOM_ITERATE_REASON_FIRST,	0
.equ	SOM_ITERATE_REASON_NEXT,	1
.equ	SOM_ITERATE_REASON_PREV,	2
.equ	SOM_ITERATE_REASON_LAST,	3

.equ	OBJECT_FLAG_TYPE_LOADER,	1
.equ	OBJECT_FLAG_TYPE_CLIENT,	2
.equ	OBJECT_FLAG_TYPE_SHARED,	4
.equ	OBJECT_FLAG_TYPE_MASK,		0b111
