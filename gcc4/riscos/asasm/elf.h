#ifndef NO_ELF_SUPPORT
#ifndef _elf_h
#define _elf_h

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

typedef __signed__ long long __s64;
typedef unsigned long long __u64;

/* 32-bit ELF base types. */
typedef __u32	Elf32_Addr;
typedef __u16	Elf32_Half;
typedef __u32	Elf32_Off;
typedef __s32	Elf32_Sword;
typedef __u32	Elf32_Word;

/* 64-bit ELF base types. */
typedef __u64	Elf64_Addr;
typedef __u16	Elf64_Half;
typedef __s16	Elf64_SHalf;
typedef __u64	Elf64_Off;
typedef __s32	Elf64_Sword;
typedef __u32	Elf64_Word;
typedef __u64	Elf64_Xword;
typedef __s64	Elf64_Sxword;

/* These constants are for the segment types stored in the image headers */
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff

/* These constants define the different elf file types */
#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

/* These constants define the various ELF target machines */
#define EM_NONE  0
#define EM_M32   1
#define EM_SPARC 2
#define EM_386   3
#define EM_68K   4
#define EM_88K   5
#define EM_486   6   /* Perhaps disused */
#define EM_860   7

#define EM_MIPS		8	/* MIPS R3000 (officially, big-endian only) */

#define EM_MIPS_RS4_BE 10	/* MIPS R4000 big-endian */

#define EM_PARISC      15	/* HPPA */

#define EM_SPARC32PLUS 18	/* Sun's "v8plus" */

#define EM_PPC	       20	/* PowerPC */
#define EM_PPC64       21       /* PowerPC64 */

#define EM_ARM         40       /* ARM */

#define EM_SH	       42	/* SuperH */

#define EM_SPARCV9     43	/* SPARC v9 64-bit */

#define EM_IA_64	50	/* HP/Intel IA-64 */

#define EM_X86_64	62	/* AMD x86-64 */

#define EM_S390		22	/* IBM S/390 */

#define EM_CRIS         76      /* Axis Communications 32-bit embedded processor */

/*
 * This is an interim value that we will use until the committee comes
 * up with a final number.
 */
#define EM_ALPHA	0x9026

/*
 * This is the old interim value for S/390 architecture
 */
#define EM_S390_OLD     0xA390

/* This is the info that is needed to parse the dynamic section of the file */
#define DT_NULL		0
#define DT_NEEDED	1
#define DT_PLTRELSZ	2
#define DT_PLTGOT	3
#define DT_HASH		4
#define DT_STRTAB	5
#define DT_SYMTAB	6
#define DT_RELA		7
#define DT_RELASZ	8
#define DT_RELAENT	9
#define DT_STRSZ	10
#define DT_SYMENT	11
#define DT_INIT		12
#define DT_FINI		13
#define DT_SONAME	14
#define DT_RPATH 	15
#define DT_SYMBOLIC	16
#define DT_REL	        17
#define DT_RELSZ	18
#define DT_RELENT	19
#define DT_PLTREL	20
#define DT_DEBUG	21
#define DT_TEXTREL	22
#define DT_JMPREL	23
#define DT_LOPROC	0x70000000
#define DT_HIPROC	0x7fffffff

/* This info is needed when parsing the symbol table */
#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4

#define ELF32_ST_BIND(x) ((x) >> 4)
#define ELF32_ST_TYPE(x) (((unsigned int) x) & 0xf)
#define ELF32_ST_INFO(b, t) (((b)<<4)+((t)&0xf))

/* Symbolic values for the entries in the auxiliary table
   put on the initial stack */
#define AT_NULL   0	/* end of vector */
#define AT_IGNORE 1	/* entry should be ignored */
#define AT_EXECFD 2	/* file descriptor of program */
#define AT_PHDR   3	/* program headers for program */
#define AT_PHENT  4	/* size of program header entry */
#define AT_PHNUM  5	/* number of program headers */
#define AT_PAGESZ 6	/* system page size */
#define AT_BASE   7	/* base address of interpreter */
#define AT_FLAGS  8	/* flags */
#define AT_ENTRY  9	/* entry point of program */
#define AT_NOTELF 10	/* program is not ELF */
#define AT_UID    11	/* real uid */
#define AT_EUID   12	/* effective uid */
#define AT_GID    13	/* real gid */
#define AT_EGID   14	/* effective gid */
#define AT_PLATFORM 15  /* string identifying CPU for optimizations */
#define AT_HWCAP  16    /* arch dependent hints at CPU capabilities */
#define AT_CLKTCK 17	/* frequency at which times() increments */

typedef struct dynamic{
  Elf32_Sword d_tag;
  union{
    Elf32_Sword	d_val;
    Elf32_Addr	d_ptr;
  } d_un;
} Elf32_Dyn;

typedef struct {
  Elf64_Sxword d_tag;		/* entry tag value */
  union {
    Elf64_Xword d_val;
    Elf64_Addr d_ptr;
  } d_un;
} Elf64_Dyn;

/* The following are used with relocations */
#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

/* ARM Relocation types */

/* Operation notes: 
     S: Address of the symbol.
     A: Addend for relocation.
     P: Address of the place being relocated.
     Pa: Adjusted address of the place being relocated (P & 0xfffffffc)
     T: is 1 if S has type STT_FUNC and the symbol addresses a Thumb
        instruction.Thumb-bit; it is 0 otherwise.
     B(S): Addressing origin of the output segment defining S.
     GOT_ORG: Addressing origin of the Global Offset Table.
     GOT(S): Address of the GOT entry for S.
 */

enum
{
				/* Type		Class	Operation */
				/* ------------------------------ */
  R_ARM_NONE = 0,		/* Static	Misc */
  R_ARM_PC24 = 1,		/* Deprecated	ARM	((S + A) | T) - P */
  R_ARM_ABS32 = 2,		/* Static	Data	(S + A) | T */
  R_ARM_REL32 = 3,		/* Static	Data	((S + A) | T) - P */
  R_ARM_LDR_PC_G0 = 4,		/* Static	ARM	S + A - P */
  R_ARM_ABS16 = 5,		/* Static	Data	S + A */
  R_ARM_ABS12 = 6,		/* Static	ARM	S + A */
  R_ARM_THM_ABS5 = 7,		/* Static	Thumb16	S + A */
  R_ARM_ABS8 = 8,		/* Static	Data	S + A */
  R_ARM_SBREL32 = 9,		/* Static	Data	((S + A) | T) - B(S) */
  R_ARM_THM_CALL = 10,		/* Static	Thumb32	((S + A) | T) - P */
  R_ARM_THM_PC8 = 11,		/* Static	Thumb16 */
  R_ARM_BREL_ADJ = 12,		/* Dynamic	Data	DeltaB(S) + A */
  R_ARM_TLS_DESC = 13,		/* Dynamic	Data */
  R_ARM_THM_SWI8 = 14,		/* Obsolete */
  R_ARM_XPC25 = 15,		/* Obsolete */
  R_ARM_THM_XPC22 = 16,		/* Obsolete */
  R_ARM_TLS_DTPMOD32 = 17,	/* Dynamic	Data	Module(S) */
  R_ARM_TLS_DTPOFF32 = 18,	/* Dynamic	Data	S + A - TLS */
  R_ARM_TLS_TPOFF32 = 19,	/* Dynamic	Data	S + A - tp */
  R_ARM_COPY = 20,		/* Dynamic	Misc */
  R_ARM_GLOB_DAT = 21,		/* Dynamic	Data	(S + A) | T */
  R_ARM_JUMP_SLOT = 22,		/* Dynamic	Data	(S + A) | T */
  R_ARM_RELATIVE = 23,		/* Dynamic	Data	B(S) + A */
  R_ARM_GOTOFF32 = 24,		/* Static	Data	(((S + A) | T) - GOT_ORG */
  R_ARM_BASE_PREL = 25,		/* Static	Data	B(S) + A - P */
  R_ARM_GOT_BREL = 26,		/* Static	Data	GOT(S) + A - GOT_ORG */
  R_ARM_PLT32 = 27,		/* Deprecated	ARM	((S + A) | T) - P */
  R_ARM_CALL = 28,		/* Static	ARM	((S + A) | T) - P */
  R_ARM_JUMP24 = 29,		/* Static	ARM	((S + A) | T) - P */
  R_ARM_THM_JUMP24 = 30,	/* Static	Thumb32	((S + A) | T) - P */
  R_ARM_BASE_ABS = 31,		/* Static	Data	B(S) + A */
  R_ARM_ALU_PCREL_7_0 = 32,	/* Obsolete */
  R_ARM_ALU_PCREL_15_8 = 33,	/* Obsolete */
  R_ARM_ALU_PCREL_23_15 = 34,	/* Obsolete */
  R_ARM_LDR_SBREL_11_0_NC = 35,	/* Deprecated	ARM	S + A - B(S) */
  R_ARM_ALU_SBREL_19_12_NC = 36,/* Deprecated	ARM	S + A - B(S) */
  R_ARM_ALU_SBREL_27_20_CK = 37,/* Deprecated	ARM	S + A - B(S) */
  R_ARM_TARGET1 = 38,		/* Data		Misc	(S + A) | T or */
				/*			((S + A) | T) - P */
  R_ARM_SBREL31 = 39,		/* Deprecated	Data	((S + A) | T) - B(S) */
  R_ARM_V4BX = 40,		/* Static	Misc  */
  R_ARM_TARGET2 = 41,		/* Static	Misc */
  R_ARM_PREL31 = 42,		/* Static	Data	((S + A) | T) - P */
  R_ARM_MOVW_ABS_NC = 43,	/* Static	ARM	(S + A) | T */
  R_ARM_MOVT_ABS = 44,		/* Static	ARM	S + A */
  R_ARM_MOVW_PREL_NC = 45,	/* Static	ARM	((S + A) | T) - P */
  R_ARM_MOVT_PREL = 46,		/* Static	ARM	S + A - P */
  R_ARM_THM_MOVW_ABS_NC = 47,	/* Static	Thumb32	(S + A) | T */
  R_ARM_THM_MOVT_ABS = 48,	/* Static	Thumb32	S + A - P */
  R_ARM_THM_MOVW_PREL_NC =  49,	/* Static	Thumb32	((S + A) | T) - P */
  R_ARM_THM_MOVT_PREL = 50,	/* Static	Thumb32	S + A - P */
  R_ARM_THM_JUMP19 = 51,	/* Static	Thumb32	((S + A) | T) - P */
  R_ARM_THM_JUMP6 = 52,		/* Static	Thumb16	S + A - P */
  R_ARM_THM_ALU_PREL_11_0 = 53,	/* Static	Thumb32	((S + A) | T) - Pa */
  R_ARM_THM_PC12 = 54,		/* Static	Thumb32	S + A - Pa */
  R_ARM_ABS32_NOI = 55,		/* Static	Data	S + A */
  R_ARM_REL32_NOI = 56,		/* Static	Data	S + A - P */
  R_ARM_ALU_PC_G0_NC = 57,	/* Static	ARM	((S + A) | T) - P */
  R_ARM_ALU_PC_G0 = 58,		/* Static	ARM	((S + A) | T) - P */
  R_ARM_ALU_PC_G1_NC = 59,	/* Static	ARM	((S + A) | T) - P */
  R_ARM_ALU_PC_G1 = 60,		/* Static	ARM	((S + A) | T) - P */
  R_ARM_ALU_PC_G2 = 61,		/* Static	ARM	((S + A) | T) - P */
  R_ARM_LDR_PC_G1 = 62,		/* Static	ARM	S + A - P */
  R_ARM_LDR_PC_G2 = 63,		/* Static	ARM	S + A - P */
  R_ARM_LDRS_PC_G0 = 64,	/* Static	ARM	S + A - P */
  R_ARM_LDRS_PC_G1 = 65,	/* Static	ARM	S + A - P */
  R_ARM_LDRS_PC_G2 = 66,	/* Static	ARM	S + A - P */
  R_ARM_LDC_PC_G0 = 67,		/* Static	ARM	S + A - P */
  R_ARM_LDC_PC_G1 = 68,		/* Static	ARM	S + A - P */
  R_ARM_LDC_PC_G2 = 69,		/* Static	ARM	S + A - P */
  R_ARM_ALU_SB_G0_NC = 70,	/* Static	ARM	((S + A) | T) - B(S) */
  R_ARM_ALU_SB_G0 = 71,		/* Static	ARM	((S + A) | T) - B(S) */
  R_ARM_ALU_SB_G1_NC = 72,	/* Static	ARM	((S + A) | T) - B(S) */
  R_ARM_ALU_SB_G1 = 73,		/* Static	ARM	((S + A) | T) - B(S) */
  R_ARM_ALU_SB_G2 = 74,		/* Static	ARM	((S + A) | T) - B(S) */
  R_ARM_LDR_SB_G0 = 75,		/* Static	ARM	S + A - B(S) */
  R_ARM_LDR_SB_G1 = 76,		/* Static	ARM	S + A - B(S) */
  R_ARM_LDR_SB_G2 = 77,		/* Static	ARM	S + A - B(S) */
  R_ARM_LDRS_SB_G0 = 78,	/* Static	ARM	S + A - B(S) */
  R_ARM_LDRS_SB_G1 = 79,	/* Static	ARM	S + A - B(S) */
  R_ARM_LDRS_SB_G2 = 80,	/* Static	ARM	S + A - B(S) */
  R_ARM_LDC_SB_G0 = 81,		/* Static	ARM	S + A - B(S) */
  R_ARM_LDC_SB_G1 = 82,		/* Static	ARM	S + A - B(S) */
  R_ARM_LDC_SB_G2 = 83,		/* Static	ARM	S + A - B(S) */
  R_ARM_MOVW_BREL_NC = 84,	/* Static	ARM	((S + A) | T) - B(S) */
  R_ARM_MOVT_BREL = 85,		/* Static	ARM	S + A - B(S) */
  R_ARM_MOVW_BREL = 86,		/* Static	ARM	((S + A) | T) - B(S) */
  R_ARM_THM_MOVW_BREL_NC = 87,	/* Static	Thumb32	((S + A) | T) - B(S) */
  R_ARM_THM_MOVT_BREL = 88,	/* Static	Thumb32	S + A - B(S) */
  R_ARM_THM_MOVW_BREL = 89,	/* Static	Thumb32	((S + A) | T) - B(S) */
  R_ARM_TLS_GOTDESC = 90,	/* Static	Data */
  R_ARM_TLS_CALL = 91,		/* Static	ARM */
  R_ARM_TLS_DESCSEQ = 92,	/* Static	ARM	TLS relaxation */
  R_ARM_THM_TLS_CALL = 93,	/* Static	Thumb32 */
  R_ARM_PLT32_ABS =  94,	/* Static	Data	PLT(S) + A */
  R_ARM_GOT_ABS =  95,		/* Static	Data	GOT(S) + A */
  R_ARM_GOT_PREL = 96,		/* Static	Data	GOT(S) + A - P */
  R_ARM_GOT_BREL12 =  97,	/* Static	ARM	GOT(S) + A - GOT_ORG */
  R_ARM_GOTOFF12 =  98,		/* Static	ARM	S + A - GOT_ROG */
  R_ARM_GOTRELAX =  99,		/* Static	Misc */
  R_ARM_GNU_VTENTRY = 100,	/* Deprecated	Data */
  R_ARM_GNU_VTINHERIT = 101,	/* Deprecated	Data */
  R_ARM_THM_JUMP11 = 102,	/* Static	Thumb16	S + A - P */
  R_ARM_THM_JUMP8 = 103,	/* Static	Thumb16	S + A - P */
  R_ARM_TLS_GD32 = 104,		/* Static	Data	GOT(S) + A - P */
  R_ARM_TLS_LDM32 = 105,	/* Static	Data	GOT(S) + A - P */
  R_ARM_TLS_LDO32 = 106,	/* Static	Data	S + A - TLS */
  R_ARM_TLS_IE32 = 107,		/* Static	Data	GOT(S) + A - P */
  R_ARM_TLS_LE32 = 108,		/* Static	Data	S + A - tp */
  R_ARM_TLS_LDO12 = 109,	/* Static	ARM	S + A - TLS */
  R_ARM_TLS_LE12 = 110,		/* Static	ARM	S + A - tp */
  R_ARM_TLS_IE12GP = 111,	/* Static	ARM	GOT(S) + A - GOT_ORG */
  R_ARM_PRIVATE_0 = 112,	/* Private (n = 0, 1, ... 15) */
  R_ARM_PRIVATE_1 = 113,
  R_ARM_PRIVATE_2 = 114,
  R_ARM_PRIVATE_3 = 115,
  R_ARM_PRIVATE_4 = 116,
  R_ARM_PRIVATE_5 = 117,
  R_ARM_PRIVATE_6 = 118,
  R_ARM_PRIVATE_7 = 119,
  R_ARM_PRIVATE_8 = 120,
  R_ARM_PRIVATE_9 = 121,
  R_ARM_PRIVATE_10 = 122,
  R_ARM_PRIVATE_11 = 123,
  R_ARM_PRIVATE_12 = 124,
  R_ARM_PRIVATE_13 = 125,
  R_ARM_PRIVATE_14 = 126,
  R_ARM_PRIVATE_15 = 127,
  R_ARM_ME_TOO = 128,		/* Obsolete */
  R_ARM_THM_TLS_DESCSEQ16 = 129,/* Static	Thumb16 */
  R_ARM_THM_TLS_DESCSEQ32 = 130,/* Static	Thumb32 */
  /* 131 - 139			Unallocated */
  /* 140 - 159			Dynamic		Reserved for future allocation */
  /* 160 - 255			Unallocated */
};

/* e_flags entries for ARM */

enum
{
  EF_ARM_RELEXEC = 0x01,
  EF_ARM_HASENTRY = 0x02,
  EF_ARM_INTERWORK = 0x04,
  EF_ARM_SYMSARESORTED = 0x04,	/* Conflicts with EF_ARM_INTERWORK.  */
  EF_ARM_APCS_26 = 0x08,
  EF_ARM_DYNSYMSUSESEGIDX = 0x08,	/* Conflicts with EF_ARM_APCS_26.  */
  EF_ARM_APCS_FLOAT = 0x10,
  EF_ARM_MAPSYMSFIRST = 0x10,	/* Conflicts with EF_ARM_APCS_FLOAT.  */
  EF_ARM_PIC = 0x20,
  EF_ARM_ALIGN8 = 0x40,		/* 8-bit structure alignment is in use.  */
  EF_ARM_NEW_ABI = 0x80,
  EF_ARM_OLD_ABI = 0x100,
  EF_ARM_SOFT_FLOAT = 0x200,
  EF_ARM_VFP_FLOAT = 0x400,
  EF_ARM_MAVERICK_FLOAT = 0x800,

  EF_ARM_LE8 = 0x00400000,
  EF_ARM_BE8 = 0x00800000,

  /* Mask to extract EABI version, not really a flag value.  */
  EF_ARM_EABIMASK = 0xFF000000,

  EF_ARM_EABI_UNKNOWN = 0x00000000,
  EF_ARM_EABI_VER1 = 0x01000000,
  EF_ARM_EABI_VER2 = 0x02000000,
  EF_ARM_EABI_VER3 = 0x03000000,
  EF_ARM_EABI_VER4 = 0x04000000,
  EF_ARM_EABI_VER5 = 0x05000000,
};

typedef struct elf32_rel {
  Elf32_Addr	r_offset;
  Elf32_Word	r_info;
} Elf32_Rel;

typedef struct elf64_rel {
  Elf64_Addr r_offset;	/* Location at which to apply the action */
  Elf64_Xword r_info;	/* index and type of relocation */
} Elf64_Rel;

typedef struct elf32_rela{
  Elf32_Addr	r_offset;
  Elf32_Word	r_info;
  Elf32_Sword	r_addend;
} Elf32_Rela;

typedef struct elf64_rela {
  Elf64_Addr r_offset;	/* Location at which to apply the action */
  Elf64_Xword r_info;	/* index and type of relocation */
  Elf64_Sxword r_addend;	/* Constant addend used to compute value */
} Elf64_Rela;

typedef struct elf32_sym{
  Elf32_Word	st_name;
  Elf32_Addr	st_value;
  Elf32_Word	st_size;
  unsigned char	st_info;
  unsigned char	st_other;
  Elf32_Half	st_shndx;
} Elf32_Sym;

typedef struct elf64_sym {
  Elf64_Word st_name;		/* Symbol name, index in string tbl */
  unsigned char	st_info;	/* Type and binding attributes */
  unsigned char	st_other;	/* No defined meaning, 0 */
  Elf64_Half st_shndx;		/* Associated section index */
  Elf64_Addr st_value;		/* Value of the symbol */
  Elf64_Xword st_size;		/* Associated symbol size */
} Elf64_Sym;


#define EI_NIDENT	16

typedef struct elf32_hdr{
  unsigned char	e_ident[EI_NIDENT];
  Elf32_Half	e_type;
  Elf32_Half	e_machine;
  Elf32_Word	e_version;
  Elf32_Addr	e_entry;  /* Entry point */
  Elf32_Off	e_phoff;
  Elf32_Off	e_shoff;
  Elf32_Word	e_flags;
  Elf32_Half	e_ehsize;
  Elf32_Half	e_phentsize;
  Elf32_Half	e_phnum;
  Elf32_Half	e_shentsize;
  Elf32_Half	e_shnum;
  Elf32_Half	e_shstrndx;
} Elf32_Ehdr;

typedef struct elf64_hdr {
  unsigned char	e_ident[16];		/* ELF "magic number" */
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;		/* Entry point virtual address */
  Elf64_Off e_phoff;		/* Program header table file offset */
  Elf64_Off e_shoff;		/* Section header table file offset */
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} Elf64_Ehdr;

/* These constants define the permissions on sections in the program
   header, p_flags. */
#define PF_R		0x4
#define PF_W		0x2
#define PF_X		0x1

/* ARM-specific program header flags.  */
#define PF_ARM_SB          0x10000000   /* Segment contains the location addressed by the static base.  */
#define PF_ARM_PI          0x20000000   /* Segment is position-independent.  */
#define PF_ARM_ABS         0x40000000   /* Segment must be loaded at its base address.  */

typedef struct elf32_phdr{
  Elf32_Word	p_type;
  Elf32_Off	p_offset;
  Elf32_Addr	p_vaddr;
  Elf32_Addr	p_paddr;
  Elf32_Word	p_filesz;
  Elf32_Word	p_memsz;
  Elf32_Word	p_flags;
  Elf32_Word	p_align;
} Elf32_Phdr;

typedef struct elf64_phdr {
  Elf64_Word p_type;
  Elf64_Word p_flags;
  Elf64_Off p_offset;		/* Segment file offset */
  Elf64_Addr p_vaddr;		/* Segment virtual address */
  Elf64_Addr p_paddr;		/* Segment physical address */
  Elf64_Xword p_filesz;		/* Segment size in file */
  Elf64_Xword p_memsz;		/* Segment size in memory */
  Elf64_Xword p_align;		/* Segment alignment, file & memory */
} Elf64_Phdr;

/* sh_type */
#define SHT_NULL	0
#define SHT_PROGBITS	1
#define SHT_SYMTAB	2
#define SHT_STRTAB	3
#define SHT_RELA	4
#define SHT_HASH	5
#define SHT_DYNAMIC	6
#define SHT_NOTE	7
#define SHT_NOBITS	8
#define SHT_REL		9
#define SHT_SHLIB	10
#define SHT_DYNSYM	11
#define SHT_NUM		12
#define SHT_LOPROC	0x70000000
#define SHT_HIPROC	0x7fffffff
#define SHT_LOUSER	0x80000000
#define SHT_HIUSER	0xffffffff

/* sh_flags */
#define SHF_WRITE	0x1
#define SHF_ALLOC	0x2
#define SHF_EXECINSTR	0x4
#define SHF_MASKPROC	0xf0000000

/* Additional section types.  */
#define SHT_ARM_EXIDX	       0x70000001	/* Section holds ARM unwind info.  */
#define SHT_ARM_PREEMPTMAP     0x70000002	/* Section pre-emption details.  */
#define SHT_ARM_ATTRIBUTES     0x70000003	/* Section holds attributes.  */
#define SHT_ARM_DEBUGOVERLAY   0x70000004	/* Section holds overlay debug info.  */
#define SHT_ARM_OVERLAYSECTION 0x70000005	/* Section holds GDB and overlay integration info.  */

/* ARM specific */
#define SHF_ENTRYSECT	0x10000000   /* Section contains an entry point.  */
#define SHF_COMDEF	0x80000000   /* Section may be multiply defined in the input to a link step.  */

/* special section indexes */
#define SHN_UNDEF	0
#define SHN_LORESERVE	0xff00
#define SHN_LOPROC	0xff00
#define SHN_HIPROC	0xff1f
#define SHN_ABS		0xfff1
#define SHN_COMMON	0xfff2
#define SHN_HIRESERVE	0xffff

typedef struct {
  Elf32_Word	sh_name;
  Elf32_Word	sh_type;
  Elf32_Word	sh_flags;
  Elf32_Addr	sh_addr;
  Elf32_Off	sh_offset;
  Elf32_Word	sh_size;
  Elf32_Word	sh_link;
  Elf32_Word	sh_info;
  Elf32_Word	sh_addralign;
  Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct elf64_shdr {
  Elf64_Word sh_name;		/* Section name, index in string tbl */
  Elf64_Word sh_type;		/* Type of section */
  Elf64_Xword sh_flags;		/* Miscellaneous section attributes */
  Elf64_Addr sh_addr;		/* Section virtual addr at execution */
  Elf64_Off sh_offset;		/* Section file offset */
  Elf64_Xword sh_size;		/* Size of section in bytes */
  Elf64_Word sh_link;		/* Index of another section */
  Elf64_Word sh_info;		/* Additional section information */
  Elf64_Xword sh_addralign;	/* Section alignment */
  Elf64_Xword sh_entsize;	/* Entry size if section holds table */
} Elf64_Shdr;

#define	EI_MAG0		0		/* e_ident[] indexes */
#define	EI_MAG1		1
#define	EI_MAG2		2
#define	EI_MAG3		3
#define	EI_CLASS	4
#define	EI_DATA		5
#define	EI_VERSION	6
#define EI_OSABI	7
#define EI_ABIVERSION	8
#define	EI_PAD		9

#define	ELFMAG0		0x7f		/* EI_MAG */
#define	ELFMAG1		'E'
#define	ELFMAG2		'L'
#define	ELFMAG3		'F'
#define	ELFMAG		"\177ELF"
#define	SELFMAG		4

#define	ELFCLASSNONE	0		/* EI_CLASS */
#define	ELFCLASS32	1
#define	ELFCLASS64	2
#define	ELFCLASSNUM	3

#define ELFDATANONE	0		/* e_ident[EI_DATA] */
#define ELFDATA2LSB	1
#define ELFDATA2MSB	2

#define EV_NONE		0		/* e_version, EI_VERSION */
#define EV_CURRENT	1
#define EV_NUM		2

#define ELFOSABI_NONE	0
#define ELFOSABI_ARM	97

/* Notes used in ET_CORE */
#define NT_PRSTATUS	1
#define NT_PRFPREG	2
#define NT_PRPSINFO	3
#define NT_TASKSTRUCT	4
#define NT_PRFPXREG	20

/* Note header in a PT_NOTE section */
typedef struct elf32_note {
  Elf32_Word	n_namesz;	/* Name size */
  Elf32_Word	n_descsz;	/* Content size */
  Elf32_Word	n_type;		/* Content type */
} Elf32_Nhdr;

/* Note header in a PT_NOTE section */
typedef struct elf64_note {
  Elf64_Word n_namesz;	/* Name size */
  Elf64_Word n_descsz;	/* Content size */
  Elf64_Word n_type;	/* Content type */
} Elf64_Nhdr;

#if ELF_CLASS == ELFCLASS32

extern Elf32_Dyn _DYNAMIC [];
#define elfhdr		elf32_hdr
#define elf_phdr	elf32_phdr
#define elf_note	elf32_note

#else

extern Elf64_Dyn _DYNAMIC [];
#define elfhdr		elf64_hdr
#define elf_phdr	elf64_phdr
#define elf_note	elf64_note

#endif


#endif
#endif
