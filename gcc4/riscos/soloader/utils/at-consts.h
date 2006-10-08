@; Most of these are just for compatibility with the dynamic loader code
@; Those marked by an asterix are read by the dynamic loader
.equ	AT_NULL,		0	@; *	marks end of array
.equ	AT_IGNORE,		1
.equ	AT_EXECFD,		2
.equ	AT_PHDR,		3	@; *
.equ	AT_PHENT,		4
.equ	AT_PHNUM,		5	@; *
.equ	AT_PAGESZ,		6
.equ	AT_BASE,		7	@; *
.equ	AT_FLAGS,       	8
.equ	AT_ENTRY,		9	@; *
.equ	AT_NOTELF, 		10	@; 	program is not ELF
.equ	AT_UID,    		11	@; *	real uid
.equ	AT_EUID,  		12	@; *	effective uid
.equ	AT_GID,   		13	@; *	real gid
.equ	AT_EGID,  		14	@; *	effective gid

.equ	AT_DATA,		100

.equ	AUX_ARRAY_SIZE,		6 * 4 * 2
