/* auto-host-riscos.h.
   Handwritten by Nick Burrett <nick@dsvr.net>

   This file is based on the functionality provided by UnixLib
   elsewhere within the GCCSDK source tree.   It applies to the
   creation of the stage 2 compiler i.e. the one that actually
   runs on RISC OS.  */

#define BYTEORDER 1234

/* Define if you can safely include both <string.h> and <strings.h>.  */
#define STRING_WITH_STRINGS 1

/* Define if printf supports "%p".  */
#define HAVE_PRINTF_PTR 1

#define PACKAGE "gcc"
#define VERSION "3.4.0"

/* Define if you want expensive run-time checks. */
/* #undef ENABLE_CHECKING */

/* Define if your cpp understands the stringify operator.  */
#define HAVE_CPP_STRINGIFY 1

/* Define if your compiler understands volatile.  */
#define HAVE_VOLATILE 1

#define HAVE_STPCPY 1

/* Define if your assembler supports specifying the maximum number
   of bytes to skip when using the GAS .p2align command. */
/* #undef HAVE_GAS_MAX_SKIP_P2ALIGN */

/* Define if your assembler supports .balign and .p2align.  */
/* #undef HAVE_GAS_BALIGN_AND_P2ALIGN */

/* Define if your assembler supports .subsection and .subsection -1 starts
   emitting at the beginning of your section */
/* #undef HAVE_GAS_SUBSECTION_ORDERING */

#define HAVE_GAS_SHF_MERGE 0

/* Define if you want expensive run-time checks. */
/* #undef ENABLE_CHECKING */

/* Define to enable the use of a default assembler. */
/* #undef DEFAULT_ASSEMBLER */

/* Define to enable the use of a default linker. */
/* #undef DEFAULT_LINKER */


/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have <vfork.h>.  */
/* #undef HAVE_VFORK_H */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if `sys_siglist' is declared by <signal.h>.  */
#define SYS_SIGLIST_DECLARED 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define vfork as fork if vfork does not work.  */
/* #undef vfork */

/* Define if you have the atoll function.  */
#define HAVE_ATOLL 1

/* Define if you have the atoq function.  */
/* #undef HAVE_ATOQ */

/* Define if you have the bcmp function.  */
#define HAVE_BCMP 1

/* Define if you have the bcopy function.  */
#define HAVE_BCOPY 1

/* Define if you have the bsearch function.  */
#define HAVE_BSEARCH 1

/* Define if you have the bzero function.  */
#define HAVE_BZERO 1

/* Define if you have the fputc_unlocked function.  */
/* #undef HAVE_FPUTC_UNLOCKED */

/* Define if you have the fputs_unlocked function.  */
/* #undef HAVE_FPUTS_UNLOCKED */

/* Define if you have the getrlimit function.  */
#define HAVE_GETRLIMIT 1

/* Define if you have the getrusage function.  */
#define HAVE_GETRUSAGE 1

/* Define if you have the gettimeofday function.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have the index function.  */
#define HAVE_INDEX 1

/* Define if you have the isascii function.  */
#define HAVE_ISASCII 1

/* Define if you have the kill function.  */
#define HAVE_KILL 1

/* Define if you have the popen function.  */
#define HAVE_POPEN 1

/* Define if you have the putc_unlocked function.  */
/* #undef HAVE_PUTC_UNLOCKED */

/* Define if you have the putenv function.  */
#define HAVE_PUTENV 1

/* Define if you have the rindex function.  */
#define HAVE_RINDEX 1

/* Define if you have the setrlimit function.  */
#define HAVE_SETRLIMIT 1

/* Define if you have the stpcpy function.  */
#define HAVE_STPCPY 1

/* Define if you have the strchr function.  */
#define HAVE_STRCHR 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the strrchr function.  */
#define HAVE_STRRCHR 1

/* Define if you have the strsignal function.  */
#define HAVE_STRSIGNAL 1

/* Define if you have the strstr function.  */
#define HAVE_STRSTR 1

/* Define if you have the strtoul function.  */
#define HAVE_STRTOUL 1

/* Define if you have the sysconf function.  */
#define HAVE_SYSCONF 1

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define if you have the <endian.h> header file.  */
#define HAVE_ENDIAN_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <inttypes.h> header file.  */
#define HAVE_INTTYPES_H 1

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <machine/endian.h> header file.  */
#define HAVE_MACHINE_ENDIAN_H

/* Define if you have the <stab.h> header file.  */
/* #define HAVE_STAB_H 1 */

/* Define if you have the <stddef.h> header file.  */
#define HAVE_STDDEF_H 1

/* Define if you have the <stdint.h> header file.  */
#define HAVE_STDINT_H 1

/* Define if you have the <stdlib.h> header file.  */
#define HAVE_STDLIB_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <strings.h> header file.  */
#define HAVE_STRINGS_H 1

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 1

/* Define if you have the <sys/param.h> header file.  */
#define HAVE_SYS_PARAM_H 1

/* Define if you have the <sys/resource.h> header file.  */
#define HAVE_SYS_RESOURCE_H 1

/* Define if you have the <sys/stat.h> header file.  */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/times.h> header file.  */
#define HAVE_SYS_TIMES_H 1

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the <time.h> header file.  */
#define HAVE_TIME_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_GETENV 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_ATOL 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_SBRK 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_ABORT 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_ATOF 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_GETCWD 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_GETWD 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_STRSIGNAL 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_PUTC_UNLOCKED 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_FPUTS_UNLOCKED 0

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_STRSTR 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_ENVIRON 0

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_MALLOC 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_REALLOC 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_CALLOC 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_FREE 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_BASENAME 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_GETOPT 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_CLOCK 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_GETRLIMIT 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_SETRLIMIT 1

/* Define to 1 if we found this declaration otherwise define to 0. */
#define HAVE_DECL_GETRUSAGE 1

/* Define if <sys/times.h> defines struct tms. */
#define HAVE_STRUCT_TMS 1

/* Define if <time.h> defines clock_t. */
#define HAVE_CLOCK_T 1

/* Define if host mkdir takes a single argument. */
/* #undef MKDIR_TAKES_ONE_ARG */

/* Define if your compiler understands volatile. */
#define HAVE_VOLATILE 1

/* Define if your compiler supports the `long long' type. */
#define HAVE_LONG_LONG 1

/* Define if your compiler supports the `__int64' type. */
/* #undef HAVE___INT64 */

/* The number of bytes in type short */
#define SIZEOF_SHORT 2

/* The number of bytes in type int */
#define SIZEOF_INT 4

/* The number of bytes in type long */
#define SIZEOF_LONG 4

/* The number of bytes in type long long */
#define SIZEOF_LONG_LONG 8

/* The number of bytes in type void * */
#define SIZEOF_VOID_P 4

/* Define to 1 if you want to enable namespaces (-fhonor-std) by default. */
#define ENABLE_STD_NAMESPACE 1

/* Define 0/1 to force the choice for exception handling model. */
#define CONFIG_SJLJ_EXCEPTIONS 1

/* Define if the host execution character set is EBCDIC. */
/* #undef HOST_EBCDIC */

/* Define to the name of a file containing a list of extra machine modes
   for this architecture. */
#define EXTRA_MODES_FILE "config/arm/arm-modes.def"

/* Define if the target architecture needs extra machine modes to represent
   the results of comparisons. */
#define EXTRA_CC_MODES 1

/* Define if you have the iconv() function. */
/* #undef HAVE_ICONV /*

/* Define as const if the declaration of iconv() needs const. */
/* #undef ICONV_CONST */
