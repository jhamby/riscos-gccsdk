/* This file is used when building 'decaof' natively for RISC OS.  */

#ifdef CROSS_COMPILE
#include "sdk-config.h"
#else

/* Global defines that are common to both target libraries.  */

/* Define if the linker should append ,ff8 to all executables in the
   cross-compiler */
#define ENABLE_FILETYPE_FF8 1

/* Program name */
#define PACKAGE "decaof"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME ""

/* Define to the full name and version of this package. */
#define PACKAGE_STRING ""

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME ""

/* Define to the version of this package. */
#define PACKAGE_VERSION ""

/* Program version */
#define VERSION "3.4.0"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1


#ifdef __TARGET_SCL__
/* libscl specific defines.  */

#define HAVE_BSEARCH 1
#define HAVE_CPP_STRINGIFY 1
#define HAVE_DECL_SYS_SIGLIST 1
#define HAVE_ENDIAN_H 1
#define HAVE_ISASCII 1
#define HAVE_STDINT_H 1
#define HAVE_LIMITS_H 1
#define HAVE_STDDEF_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STDINT_H 1
#define HAVE_STRCHR 1
#define HAVE_STRDUP 1
#define HAVE_STRERROR 1
#define HAVE_STRING_H 1
#define HAVE_STRRCHR 1
#define HAVE_STRSIGNAL 1
#define HAVE_STRTOUL 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_TIME_H 1
#define HAVE_UNISTD_H 1
#define TIME_WITH_SYS_TIME 1

#else /* ! __TARGET_SCL__ */

#define HAVE_ATOLL 1
#define HAVE_BCMP 1
#define HAVE_BCOPY 1
#define HAVE_BSEARCH 1
#define HAVE_BZERO 1
#define HAVE_CPP_STRINGIFY 1
#define HAVE_DECL_SYS_SIGLIST 1
#define HAVE_ENDIAN_H 1
#define HAVE_FCNTL_H 1
#define HAVE_FORK 1
#define HAVE_FPUTC_UNLOCKED 1
#define HAVE_FPUTS_UNLOCKED 1
#define HAVE_GETRLIMIT 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_INDEX 1
#define HAVE_INTTYPES_H 1
#define HAVE_ISASCII 1
#define HAVE_KILL 1
#define HAVE_LIMITS_H 1
#define HAVE_MEMORY_H 1
#define HAVE_POPEN 1
#define HAVE_PUTC_UNLOCKED 1
#define HAVE_PUTENV 1
#define HAVE_SETRLIMIT 1
#define HAVE_STAB_H 1
#define HAVE_STDDEF_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STPCPY 1
#define HAVE_STRCHR 1
#define HAVE_STRDUP 1
#define HAVE_STRERROR 1
#define HAVE_STRICMP 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_STRNCASECMP 1
#define HAVE_STRNDUP 1
#define HAVE_STRRCHR 1
#define HAVE_STRSIGNAL 1
#define HAVE_STRTOUL 1
#define HAVE_SYSCONF 1
#define HAVE_SYS_FILE_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIMES_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_WAIT_H 1
#define HAVE_TIME_H 1
#define HAVE_UNISTD_H 1
#define HAVE_VFORK 1
#define HAVE_WORKING_FORK 1
#define HAVE_WORKING_VFORK 1
#define TIME_WITH_SYS_TIME 1

#endif /* ! __TARGET_SCL__ */

#endif /* !CROSS_COMPILE */
