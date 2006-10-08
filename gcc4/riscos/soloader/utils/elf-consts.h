@;* by Lee Noar
@;* 28 June, 2004


@; File header structure for ELF format programs
		.struct	0
E_IDENT:	.skip	16
E_TYPE:		.skip	2
E_MACHINE:	.skip	2
E_VERSION:	.skip	4
E_ENTRY:	.skip	4
E_PHOFF:	.skip	4
E_SHOFF:	.skip	4
E_FLAGS:	.skip	4
E_EHSIZE:	.skip	2
E_PHENTSIZE:	.skip	2
E_PHNUM:	.skip	2
E_SHENTSIZE:	.skip	2
E_SHNUM:	.skip	2
E_SHSTRNDX:	.skip	2
E_HDRSIZE:

@;* Program header structure
		.struct	0
P_TYPE:		.skip	4
P_OFFSET:	.skip	4
P_VADDR:	.skip	4
P_PADDR:	.skip	4
P_FILESZ:	.skip	4
P_MEMSZ:	.skip	4
P_FLAGS:	.skip	4
P_ALIGN:

@;* segment flags
.equ	PF_R,		4
.equ	PF_W,		2
.equ	PF_X,		1

@; These constants are for the segment types stored in the image headers
.equ	PT_NULL,    		0
.equ	PT_LOAD,    		1
.equ	PT_DYNAMIC, 		2
.equ	PT_INTERP,  		3
.equ	PT_NOTE,    		4
.equ	PT_SHLIB,   		5
.equ	PT_PHDR,    		6
.equ	PT_LOPROC,  		0x70000000
.equ	PT_HIPROC, 		0x7fffffff
.equ	PT_MIPS_REGINFO,	0x70000000

@;* Section header structure
		.struct	0
SH_NAME:	.skip	4
SH_TYPE:	.skip	4
SH_FLAGS:	.skip	4
SH_ADDR:	.skip	4
SH_OFFSET:	.skip	4
SH_SIZE:	.skip	4
SH_LINK:	.skip	4
SH_INFO:	.skip	4
SH_ADDRALIGN:	.skip	4
SH_ENTSIZE:

.equ	EI_MAG0,		0
.equ	EI_MAG1,		1
.equ	EI_MAG2,		2
.equ	EI_MAG3,		3
.equ	EI_CLASS,		4
.equ	EI_DATA,		5
.equ	EI_VERSION,		6
.equ	EI_OSABI,		7
.equ	EI_ABIVERSION,		8
.equ	EI_PAD,			9
.equ	EI_NIDENT,        	16

.equ	EM_ARM,			40

.equ	ELFCLASSNONE,		0
.equ	ELFCLASS32,		1
.equ	ELFCLASS64,		2

.equ	ELFDATANONE,		0
.equ	ELFDATA2LSB,		1
.equ	ELFDATA2MSB,		2

.equ	EV_NONE,		0
.equ	EV_CURRENT,		1

.equ	ET_NONE,		0
.equ	ET_REL,			1
.equ	ET_EXEC,		2
.equ	ET_DYN,			3

.equ	SHT_NULL,		0
.equ	SHT_PROGBITS,		1
.equ	SHT_SYMTAB,		2
.equ	SHT_STRTAB,		3
.equ	SHT_RELA,		4
.equ	SHT_HASH,		5
.equ	SHT_DYNAMIC,		6
.equ	SHT_NOTE,		7
.equ	SHT_NOBITS,		8
.equ	SHT_REL,		9
.equ	SHT_SHLIB,		10
.equ	SHT_DYNSYM,		11

@; This is the info that is needed to parse the dynamic section of the file
.equ	DT_NULL,		0
.equ	DT_NEEDED,		1
.equ	DT_PLTRELSZ,		2
.equ	DT_PLTGOT,		3
.equ	DT_HASH,		4
.equ	DT_STRTAB,		5
.equ	DT_SYMTAB,		6
.equ	DT_RELA,		7
.equ	DT_RELASZ,		8
.equ	DT_RELAENT,		9
.equ	DT_STRSZ,		10
.equ	DT_SYMENT,		11
.equ	DT_INIT,		12
.equ	DT_FINI,		13
.equ	DT_SONAME,		14
.equ	DT_RPATH, 		15
.equ	DT_SYMBOLIC,		16
.equ	DT_REL, 	        17
.equ	DT_RELSZ,		18
.equ	DT_RELENT,		19
.equ	DT_PLTREL,		20
.equ	DT_DEBUG,		21
.equ	DT_TEXTREL,		22
.equ	DT_JMPREL,		23
