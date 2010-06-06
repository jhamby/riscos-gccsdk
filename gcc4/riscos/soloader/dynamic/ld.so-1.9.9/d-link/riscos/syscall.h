#define SOM_SWI_CHUNK_BASE	0x58580
#define	SWI_X_BIT		0x20000
#define XSOM_ALLOC		(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x0)
#define XSOM_FREE		(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x1)
#define XSOM_ALLOC_LIB		(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x2)
#define XSOM_REGISTER_OBJECT	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x3)
#define XSOM_DEREGISTER_CLIENT	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x4)
#define XSOM_DEREGISTER_OBJECT	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x5)
#define XSOM_QUERY_OBJECT	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x6)
#define XSOM_ITERATE_OBJECTS	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x7)
#define XSOM_GOT_FROM_ADDR	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x8)
#define XSOM_HANDLE_FROM_ADDR	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0x9)
#define XSOM_HANDLE_FROM_NAME	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0xA)
#define XSOM_RESOLVE_SYMLINKS	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0xB)
#define XSOM_GENERATE_RUNTIME_ARRAY	(SOM_SWI_CHUNK_BASE + SWI_X_BIT + 0xC)

#define SOM_REGISTER_LOADER		0
#define SOM_REGISTER_CLIENT		1
#define SOM_REGISTER_LIBRARY		2

#define SOM_QUERY_OBJECT_GLOBAL		0
#define SOM_QUERY_OBJECT_CLIENT		1

#define SOM_ITERATE_REASON_FIRST	0
#define SOM_ITERATE_REASON_NEXT		1
#define SOM_ITERATE_REASON_PREV		2
#define SOM_ITERATE_REASON_LAST		3

struct som_rt_elem
{
  int	private_got_ptr;
  char *public_rw_ptr;
  char *private_rw_ptr;
  int	rw_size;
};

typedef struct os_error
{
  unsigned int errnum;
  char errmess[252];
} os_error;

extern inline void _dl_exit(int status);
extern inline void _dl_close(int fd);
extern inline int _dl_mmap(void * addr, unsigned int size,
				    unsigned int prot,
				    unsigned int flags, int fd,
				    unsigned int f_offset);
#ifndef _dl_MAX_ERRNO
#define _dl_MAX_ERRNO 4096
#endif
#define _dl_mmap_check_error(__res)	\
	(((int)__res) < 0 && ((int)__res) >= -_dl_MAX_ERRNO)
extern /*inline */int _dl_open(char * addr, unsigned int flags);
extern inline int _dl_write(int fd, const char * buf, int len);
extern inline int _dl_read(int fd, const char * buf, int len);
extern inline int _dl_mprotect(const char * addr, int size, int prot);
#ifdef __riscos
#include <sys/stat.h>
#else
#define new_stat stat
#include <asm/stat.h>
#undef new_stat
#endif /* __riscos */
extern /*inline */int _dl_stat(char * filename, struct stat *st);
extern inline int _dl_munmap(char * addr, int size);

/* Here are the definitions for a bunch of syscalls that are required
   by the dynamic linker.  The idea is that we want to be able
   to call these before the errno symbol is dynamicly linked, so
   we use our own version here.  Note that we cannot assume any
   dynamic linking at all, so we cannot return any error codes.
   We just punt if there is an error. */

/* This macro is for printing debug text before the GOT pointer is
   available */
#define PRINT_TEXT(x)	\
  asm volatile ("\t"	\
		"swi 0x1;\n\t" /* SWI "OS_WriteS" */ \
		".asciz \""x"\";\n\t"	\
		".align\n\t")

static inline void print_text(char *s)
{
  asm volatile ("	mov r0, %[s];\n"
		"	swi %[OS_Write0];\n"
		: /* no outputs */
		: [s] "r" (s),
		  [OS_Write0] "i" (0x2)
		: "a1", "cc");
}

static inline void print_hex(unsigned int v)
{
  asm volatile ("	mov r0, %[v];\n"
		"	sub r1, sp, #20;\n"
		"	mov r2, #20;\n"
		"	swi %[OS_ConvertHex4];\n"
		"	mov r2, #0;\n"
		"	strb r2, [r1, #0];\n"
		"	swi %[OS_Write0];\n"
		: /* no outputs */
		: [v] "r" (v),
		  [OS_ConvertHex4] "i" (0xD4),
		  [OS_Write0] "i" (0x2)
		: "a1", "a2", "a3", "cc");
}

static inline void print_nl(void)
{
  asm volatile ("	swi 0x10a;\n"
		"	swi 0x10d;\n");
}

static inline void print_dec(unsigned int v)
{
  asm volatile ("	mov r0, %[v];\n"
  		"	sub r1, sp, #20;\n"
		"	mov r2, #20;\n"
		"	swi %[OS_ConvertInteger4];\n"
		"	mov r2, #0;\n"
		"	strb r2, [r1, #0];\n"
		"	swi %[OS_Write0];\n"
		: /* no outputs */
		: [v] "r" (v),
		  [OS_ConvertInteger4] "i" (0xDC),
		  [OS_Write0] "i" (0x2)
		: "a1", "a2", "a3", "cc");
}

#if 0
/*
  This is used only for debugging the dynamic loader
*/
extern inline volatile void backtrace(void)
{
  asm volatile ("mov r3,fp;\n\t"
  		"cmp r3,#0x8000;\n\t"
  		"ble 5f;\n"
  		"1:\n\t"
  		"mov r0,#'&';\n\t"
  		"swi 0x0;\n\t"		/* swi "OS_WriteC" */
  		"ldr r0,[r3,#0];\n\t"
  		"teq r0,#0;\n\t"
  		"beq 2f;\n\t"
  		"teq pc,pc;\n\t"
  		"bicne r0,r0,#0xfc000003;\n\t"
  		"adr r1,6f;\n\t"
  		"mov r2,#16;\n\t"
  		"swi 0xd4;\n\t"		/* swi "OS_ConvertHex8" */
  		"swi 0x2;\n\t"		/* swi "OS_Write0" */
  		"swi 0x1;\n\t"		/* swi "OS_WriteS" */
  		".byte 0x20,0x20,0x20,0x20,0x0\n\t"
  		".align\n\t"
  		"ldr r0,[r3,#0];\n\t"
  		"teq pc,pc;\n\t"
  		"bicne r0,r0,#0xfc000003;\n\t"
  		"sub r0,r0,#16;\n\t"
  		"ldr r1,[r0,#0];\n\t"
  		"tst r1,#0xff000000;\n\t"
  		"bne 3f;\n\t"
  		"swi 0x1;\n\t"
  		".ascii \"<Name not stored>\\0;\"\n\t"
   		".align\n\t"
  		"b 4f;\n"
  		"6:\n\t"
  		".word 0,0,0,0\n"
  		"3:\n\t"
  		"bic r1,r1,#0xff000000;\n\t"
  		"sub r0,r0,r1;\n\t"
  		"swi 0x2;\n"		/* swi "OS_Write0" */
  		"4:\n\t"
  		"swi 0x3;\n\t"		/* swi "OS_NewLine" */
  		"ldr r3,[r3,#-12];\n\t"
  		"cmp r3,#0;\n\t"
  		"bne 1b;\n\t"
  		"b 2f;\n\t"
  		"5:\n\t"
  		"swi 0x1;\n\t"		/* swi "OS_WriteS" */
  		".ascii \"Invalid frame pointer\\r\\n\\0\"\n\t"
  		".align\n"
  		"2:\n"
  		: /* no outputs */
  		: /* no inputs */
  		: "a1", "a2", "a3", "a4", "cc");
}
#endif

static inline unsigned int _dl_check_system_files(char *name)
{
unsigned int res;

  asm volatile ("	mov	r0, %[name];\n"
		"	swi	%[XSOM_HandleFromName];\n"
		"	mov	%[result], r0;\n"
		: [result] "=r" (res)
		: [name] "r" (name),
		  [XSOM_HandleFromName] "i" (XSOM_HANDLE_FROM_NAME)
		: "a1", "cc");
  return res;
}

static inline unsigned int _dl_generate_runtime_array(void)
{
unsigned int err_flag;

  asm volatile ("	swi	%[XSOM_GenerateRuntimeArray];\n"
		"	movvc	%[err_flag], #0;\n"
		"	movvs	%[err_flag], #1;\n"
		: [err_flag] "=r" (err_flag)
		: [XSOM_GenerateRuntimeArray] "i" (XSOM_GENERATE_RUNTIME_ARRAY)
		: "a1", "cc");
  return err_flag;
}

/*
  Return next object for given object
*/
static inline struct elf_resolve *_dl_next_object(void **handle)
{
struct elf_resolve *res;

  asm volatile ("	mov	r0, %[reason];\n"
		"	mov	r1, %[handle_i];\n"
		"	swi	%[XSOM_IterateObjects];\n"
		"	mov	%[res], r0;\n"
		"	mov	%[handle_o], r1;\n"
		: [res] "=r" (res), [handle_o] "=r" (*handle)
		: [handle_i] "1" (*handle),
		  [XSOM_IterateObjects] "i" (XSOM_ITERATE_OBJECTS),
		  [reason] "I" (SOM_ITERATE_REASON_NEXT)
		: "a1", "a2", "cc");
  return res;
}

/*
  Return first object for current client's object list
*/
static inline struct elf_resolve *_dl_first_object(void **handle)
{
struct elf_resolve *res;

  asm volatile ("	mov	r0, %[reason];\n"
		"	swi	%[XSOM_IterateObjects];\n"
		"	mov	%[res], r0;\n"
		"	mov	%[handle], r1;\n"
		: [res] "=r" (res), [handle] "=r" (*handle)
		: [XSOM_IterateObjects] "i" (XSOM_ITERATE_OBJECTS),
		  [reason] "I" (SOM_ITERATE_REASON_FIRST)
		: "a1", "a2", "cc");
  return res;
}

static inline unsigned int _dl_handle_from_addr(void *addr)
{
unsigned int res;

  asm volatile ("	mov	r0, %[arg];\n"
		"	swi	%[XSOM_HandleFromAddr];\n"
		"	mov	%[res], r0;\n"
		: [res] "=r" (res)
		: [arg] "r" (addr),
		  [XSOM_HandleFromAddr] "i" (XSOM_HANDLE_FROM_ADDR)
		: "a1", "cc");
  return res;
}

static inline unsigned int *_dl_got_from_addr(void *addr)
{
unsigned int *got;

  asm volatile ("	mov	r0,%[arg];\n"
		"	swi	%[XSOM_GOTFromAddr];\n"
		"	mov	%[got], r0;\n"
		: [got] "=r" (got)
		: [arg] "r" (addr),
		  [XSOM_GOTFromAddr] "i" (XSOM_GOT_FROM_ADDR)
		: "a1", "cc");
  return got;
}

static inline char *_dl_resolve_symlinks(const char *filename)
{
char *res;

  asm volatile ("	mov	r0, %[filename];\n"
		"	swi	%[XSOM_ResolveLinks];\n"
		"	movvc	%[res], r0;\n"
		"	movvs	%[res], #0;\n"
		: [res] "=r" (res)
		: [filename] "r" (filename),
		  [XSOM_ResolveLinks] "i" (XSOM_RESOLVE_SYMLINKS)
		: "a1", "cc");
  return res;
}

static inline void _dl_som_free(void *addr)
{
  asm volatile ("	mov	r0, %[addr];\n"
		"	swi	%[XSOM_Free];\n"
		: /* no outputs */
		: [addr] "r" (addr),
		  [XSOM_Free] "i" (XSOM_FREE)
		: "a1", "cc");
}

static inline void _dl_exit(int status)
{
  asm volatile ("	swi	%[XSOM_DeregisterClient];\n"
		"	ldr	r0, 1f;\n"
		"	ldr	r1, 0f;\n"
		"	mov	r2, %[status];\n"
		"	swi	%[OS_Exit];\n"
		"0:\n"
		"	.word	0x58454241\n"
		"1:\n"
		"	.word	0\n"
		"	.asciz	\"Dynamic Loader error\"\n"
		"	.align\n"
		: /* no outputs */
		: [status] "r" (status),
		  [XSOM_DeregisterClient] "i" (XSOM_DEREGISTER_CLIENT),
		  [OS_Exit] "i" (0x11));
}

static inline void _dl_generate_error(const os_error *err)
{
  asm volatile ("	swi	%[XSOM_DeregisterClient];\n"
		"	mov	r0, %[err];\n"
		"	swi	%[OS_GenerateError];\n"
		: /* no outputs */
		: [err] "r" (err),
		  [XSOM_DeregisterClient] "i" (XSOM_DEREGISTER_CLIENT),
		  [OS_GenerateError] "i" (0x2b)
		: "r0", "cc");
}

static inline void _dl_close(int fd)
{
  asm volatile ("	mov r1, %[fd];\n"
		"	mov r0, #0;\n"
		"	swi %[XOS_Find];\n"
		: /* no outputs */
		: [fd] "r" (fd),
		  [XOS_Find] "i" (0x2000d)
		: "a1","a2");
}

static inline int _dl_alloc_lib(unsigned int size)
{
int addr;

  asm volatile ("	mov r0, %[size];\n"
		"	swi %[XSOM_AllocLib];\n"
		"	movvs %[addr], #0;\n"
		"	movvc %[addr], r0;\n"
		: [addr] "=r" (addr)
		: [size] "r" (size),
		  [XSOM_AllocLib] "i" (XSOM_ALLOC_LIB)
		: "a1", "cc");
  return addr;
}

struct object_info
{
  char *base_addr;	/* Base address of the library */
  char *public_rw_ptr;	/* The library's copy of the r/w segment */
  char *private_rw_ptr;	/* The client's copy of the r/w segment */
  int rw_size;		/* Size of the r/w segment */
  int got_offset;	/* Offset in bytes of the GOT in to the r/w segment */
  int bss_offset;	/* Offset in bytes of the bss area in to the r/w segment */
  int bss_size;		/* Size in bytes of the bss area */
  int dyn_offset;	/* Offset in bytes of dynamic segment in to R/W segment */
  int dyn_size;		/* Size in bytes of dynamic segment */
  char *name;
  unsigned int flags;
};

/*
  Register a library for the current app
*/
static inline void _dl_register_lib(unsigned int handle,struct object_info *buffer)
{
  asm volatile ("	mov	r0, %[reason_code_REGISTER_LIBRARY];\n"
		"	mov	r1, %[handle];\n"
		"	mov	r2, %[buffer];\n"
		"	swi	%[XSOM_RegisterObject];\n"
		: /* no outputs */
		: [handle] "r" (handle),
		  [buffer] "r" (buffer),
		  [XSOM_RegisterObject] "i" (XSOM_REGISTER_OBJECT),
		  [reason_code_REGISTER_LIBRARY] "I" (SOM_REGISTER_LIBRARY)
		: "a1", "a2", "a3", "cc");
}

static inline void _dl_deregister_lib(unsigned int handle)
{
  asm volatile ("	mov	r0, %[handle];\n"
		"	swi	%[XSOM_DeregisterSharedObject];\n"
		: /* no outputs */
		: [handle] "r" (handle),
		  [XSOM_DeregisterSharedObject] "i" (XSOM_DEREGISTER_OBJECT)
		: "a1", "cc");
}

/*
  Fill buffer with information about the object whose handle is given.
  Returns -1 if error occured (including not finding the object) or 0 for success.
*/
static inline int _dl_query_object_global(unsigned int handle,struct object_info *buffer)
{
int result;

  asm volatile ("	mov	r0, %[handle];\n"
		"	mov	r1, %[buffer];\n"
		"	mov	r2, %[reason_code_QUERY_GLOBAL];\n"
		"	swi	%[XSOM_QueryObject];\n"
		"	movvs	%[result], #-1;\n"
		"	movvc	%[result], #0;\n"
		: [result] "=r" (result)
		: [handle] "r" (handle),
		  [buffer] "r" (buffer),
		  [reason_code_QUERY_GLOBAL] "I" (SOM_QUERY_OBJECT_GLOBAL),
		  [XSOM_QueryObject] "i" (XSOM_QUERY_OBJECT)
		: "a1", "a2", "a3", "cc", "memory");
  return result;
}

/*
  Fill buffer with information about the object whose handle is given.
  Returns -1 if error occured (including not finding the object) or 0 for success.
*/
static inline int _dl_query_object_client(unsigned int handle,struct object_info *buffer)
{
int result;

  asm volatile ("	mov	r0, %[handle];\n"
		"	mov	r1, %[buffer];\n"
		"	mov	r2, %[reason_code_QUERY_CLIENT];\n"
		"	swi	%[XSOM_QueryObject];\n"
		"	movvs	%[result], #-1;\n"
		"	movvc	%[result], #0;\n"
		: [result] "=r" (result)
		: [handle] "r" (handle),
		  [buffer] "r" (buffer),
		  [reason_code_QUERY_CLIENT] "I" (SOM_QUERY_OBJECT_CLIENT),
		  [XSOM_QueryObject] "i" (XSOM_QUERY_OBJECT)
		: "a1", "a2", "a3", "cc", "memory");
  return result;
}

/* This is only used for loading the cache */
static inline int _dl_mmap(void * rqd_addr, unsigned int size,
				    unsigned int prot,
				    unsigned int flags, int fd,
				    unsigned int f_offset)
{
int addr = 0;

  asm volatile ("	mov	r0, %[size];\n"
  		"	swi	%[XSOM_Alloc];\n"
  		"	movvs	%[addr], #-1;\n"
  		"	bvs	0f;\n"
  		"	mov	%[addr], r0;\n"

  		"	mov	r4, %[offset];\n"
		"	mov	r3, %[size];\n"
		"	mov	r2, r0;\n"
		"	mov	r1, %[fd];\n"
		"	mov	r0, #3;\n"
		"	swi	%[XOS_GBPB];\n"
		"0:\n"
		: [addr] "=r" (addr)
		: [size] "r" (size),
		  [fd] "r" (fd),
		  [offset] "rI" (f_offset),
		  [XSOM_Alloc] "i" (XSOM_ALLOC),
		  [XOS_GBPB] "i" (0x2000C)
		: "a1", "a2", "a3", "a4", "v1", "cc");

  return addr;
}

static inline int _dl_write(int fd, const char * buf, int len)
{
  int status;

  /* Only used for output to console? Trap fd == 2 and fd == 1 and write directly to console */
  asm volatile ("	teq	%[fd], #1;\n"
		"	teqne	%[fd], #2;\n"
		"	bne	0f;\n"
		"	mov	r0, %[buffer];\n"
  		"	mov	r1, %[len];\n"
  		"	swi	%[XOS_WriteN];\n"
  		"	sub	r1, r1, #1;\n"
  		"	ldrb	r0, [r0, r1];\n"
  		"	cmp	r0, #10;\n"
  		"	swieq	0x10d;\n" /* if last character is newline, then output carriage return as well */
		"	b	1f\n"
		"0:\n"
		"	cmp	%1, #0;\n"
		"	bmi	1f;\n"
		"	mov	r3, %[len];\n"
		"	mov	r2, %[buffer];\n"
		"	mov	r1, %[fd];\n"
		"	mov	r0, #2;\n"
		"	swi	%[XOS_GBPB];\n"
		"1:\n"
		: [status] "=r" (status)
		: [fd] "r" (fd),
		  [buffer] "r" (buf),
		  [len] "r" (len),
		  [XOS_GBPB] "i" (0x2000C),
		  [XOS_WriteN] "i" (0x20046)
		: "a1", "a2", "a3", "a4", "v1", "cc");

  return status;
}


static inline int _dl_read(int fd, const char * buf, int len)
{
  int status;

  asm volatile ("	cmp	%[fd], #-1;\n"
		"	movmi	%[status], #-1;\n"
		"	bmi	0f;\t\n"
		"	mov	r3, %[len];\n"
		"	mov	r2, %[buffer];\n"
		"	mov	r1, %[fd];\n"
		"	mov	r0,#4;\n"
		"	swi	%[XOS_GBPB];\n"
		"	movvs	%[status], #-1;\n"
		"	movvc	%[status], #0;\n"
		"0:\n\t"
		: [status] "=r" (status)
		: [fd] "r" (fd),
		  [buffer] "r" (buf),
		  [len] "r" (len),
		  [XOS_GBPB] "i" (0x2000c)
		: "a1", "a2", "a3", "a4", "v1", "cc");

  return status;
}

static inline int _dl_set_file_pos(int fd, int pos)
{
int status;

  asm volatile ("	mov	r0, #1;\n"
		"	mov	r1, %[fd];\n"
		"	mov	r2, %[pos];\n"
		"	swi	%[XOS_Args];\n"
		"	movvs	%[status], #-1;\n"
		"	movvc	%[status], #0;\n"
		: [status] "=r" (status)
		: [fd] "r" (fd),
		  [pos] "r" (pos),
		  [XOS_Args] "i" (0x20009)
		: "a1", "a2", "a3", "cc");
  return status;
}

static inline int _dl_munmap(char * addr, int size)
{
int ret;

  asm volatile ("	mov	r0, %[addr];\n"
		"	swi	%[XSOM_Free];\n"
		"	movvc	%[result], #0;\n"
		"	movvs	%[result], #-1;\n"
		: [result] "=r" (ret)
		: [addr] "r" (addr),
		  [XSOM_Free] "i" (XSOM_FREE)
		: "a1", "cc");

  return ret;
}

/*
 * Not an actual syscall, but we need something in assembly to say whether
 * this is OK or not.
 */

static inline int _dl_suid_ok(void)
{
  return 1;
}
