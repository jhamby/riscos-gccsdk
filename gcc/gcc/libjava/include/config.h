/* include/config.h.  Generated automatically by configure.  */
/* include/config.h.in.  Generated automatically from configure.in by autoheader 2.13.  */

/* Define if using alloca.c.  */
/* #undef C_ALLOCA */

/* Define to one of _getb67, GETB67, getb67 for Cray-2 and Cray-YMP systems.
   This function is required for alloca.c support on those systems.  */
/* #undef CRAY_STACKSEG_END */

/* Define if you have alloca, as a function or macro.  */
#define HAVE_ALLOCA 1

/* Define if you have <alloca.h> and it should be used (not on Ultrix).  */
#define HAVE_ALLOCA_H 1

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if your struct tm has tm_zone.  */
#define HAVE_TM_ZONE 1

/* Define if you don't have tm_zone but do have the external array
   tzname.  */
/* #undef HAVE_TZNAME */

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at run-time.
 STACK_DIRECTION > 0 => grows toward higher addresses
 STACK_DIRECTION < 0 => grows toward lower addresses
 STACK_DIRECTION = 0 => direction of growth unknown
 */
/* #undef STACK_DIRECTION */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if your <sys/time.h> declares struct tm.  */
/* #undef TM_IN_SYS_TIME */

/* Define if the X Window System is missing or not being used.  */
/* #undef X_DISPLAY_MISSING */

/* Define this if you want runtime debugging enabled.  */
/* #undef DEBUG */

/* Define if using POSIX threads that have the mutexattr functions.  */
#define HAVE_PTHREAD_MUTEXATTR_INIT 1

/* Define if you have memcpy.  */
#define HAVE_MEMCPY 1

/* Define if you have memmove.  */
#define HAVE_MEMMOVE 1

/* Define if you have strerror.  */
#define HAVE_STRERROR 1

/* Define if you have int32_t and uint32_t. */
#define HAVE_INT32_DEFINED 1

/* Define if you have u_int32_t */
#define HAVE_BSD_INT32_DEFINED 1

/* Define if you're running eCos. */
/* #undef ECOS */

/* */
#define HAVE_LOCALTIME 1

/* */
#define HAVE_MKTIME 1

/* Define if using POSIX threads on Linux.  */
#define LINUX_THREADS 1

/* Define if you have the `gmtime_r' function.  */
#define HAVE_GMTIME_R 1

/* Define if you have the `localtime_r' function.  */
#define HAVE_LOCALTIME_R 1

/* Define to `int' if `ssize_t' is not defined.  */
/* #undef ssize_t */

/* Define to 1 if `in_addr_t' is defined in sys/types.h or
   netinet/in.h.  */
#define HAVE_IN_ADDR_T 1

/* Define if inet6 structures are defined in netinet/in.h.  */
#define HAVE_INET6 1

/* Define if struct ip_mreq is defined in netinet/in.h.  */
#define HAVE_STRUCT_IP_MREQ 1

/* Define if struct ipv6_mreq is defined in netinet/in.h.  */
#define HAVE_STRUCT_IPV6_MREQ 1

/* Define it socklen_t typedef is in sys/socket.h.  */
#define HAVE_SOCKLEN_T 1

/* Define if Boehm GC in use.  */
#define HAVE_BOEHM_GC 1

/* Define if gethostname is declared in <unistd.h>.  */
#define HAVE_GETHOSTNAME_DECL 1

/* Define if gethostbyname_r returns `int'.  */
#define GETHOSTBYNAME_R_RETURNS_INT 1

/* Define if gethostbyaddr_r returns `int'.  */
#define GETHOSTBYADDR_R_RETURNS_INT 1

/* Define if struct tm has tm_gmtoff field.  */
#define STRUCT_TM_HAS_GMTOFF 1

/* Define if global `timezone' exists.  */
/* #undef HAVE_TIMEZONE */

/* Define if if the synchronization code should try to avoid pthread_self 
   calls by caching thread IDs in a hashtable.  */
#define SLOW_PTHREAD_SELF 1

/* Define if you have the appropriate function.  */
#define HAVE_ACCESS 1
#define HAVE_STAT 1
#define HAVE_MKDIR 1
#define HAVE_RENAME 1
#define HAVE_RMDIR 1
#define HAVE_UNLINK 1
#define HAVE_REALPATH 1
#define HAVE_READDIR_R 1
#define HAVE_GETHOSTBYNAME_R 1
#define HAVE_GETHOSTBYADDR_R 1
#define HAVE_FTRUNCATE 1

/* Define if you want a bytecode interpreter.  */
#define INTERPRETER 1

/* Define if pthread_mutex_t has m_count member.  */
/* #undef PTHREAD_MUTEX_HAVE_M_COUNT */

/* Define if pthread_mutex_t has __m_count member.  */
#define PTHREAD_MUTEX_HAVE___M_COUNT 1

/* Define if java.net native functions should be stubbed out.  */
/* #undef DISABLE_JAVA_NET */

/* Define if we're to use libffi.  */
#define USE_LIBFFI 1

/* Define if system properties shouldn't be read from
   getenv("GCJ_PROPERTIES").  */
/* #undef DISABLE_GETENV_PROPERTIES */

/* Define if we should ignore arguments to main().  */
/* #undef DISABLE_MAIN_ARGS */

/* Define if you have /proc/self/exe */
#define HAVE_PROC_SELF_EXE 1

 
/* Define if you have dladdr() */
#define HAVE_DLADDR 1

/* Define if getuid() and friends are missing.  */
/* #undef NO_GETUID */

/* Define if libltdl is in use.  */
#define USE_LTDL 1

/* Define if g++ has a bug preventing us from inlining math routines.  */
/* #undef __NO_MATH_INLINES */

/* Define if you are using JVMPI.  */
#define ENABLE_JVMPI 1

/* Define if your platform has a working backtrace() function.  */
#define HAVE_BACKTRACE 1

/* Define if your platform has the global _timezone variable.  */
/* #undef HAVE_UNDERSCORE_TIMEZONE */

/* Define if you have the access function.  */
#define HAVE_ACCESS 1

/* Define if you have the chmod function.  */
#define HAVE_CHMOD 1

/* Define if you have the execvp function.  */
#define HAVE_EXECVP 1

/* Define if you have the fork function.  */
#define HAVE_FORK 1

/* Define if you have the fstat function.  */
#define HAVE_FSTAT 1

/* Define if you have the fsync function.  */
#define HAVE_FSYNC 1

/* Define if you have the ftime function.  */
#define HAVE_FTIME 1

/* Define if you have the ftruncate function.  */
#define HAVE_FTRUNCATE 1

/* Define if you have the getcwd function.  */
#define HAVE_GETCWD 1

/* Define if you have the gethostbyaddr_r function.  */
#define HAVE_GETHOSTBYADDR_R 1

/* Define if you have the gethostbyname_r function.  */
#define HAVE_GETHOSTBYNAME_R 1

/* Define if you have the gethostname function.  */
#define HAVE_GETHOSTNAME 1

/* Define if you have the getpwuid_r function.  */
#define HAVE_GETPWUID_R 1

/* Define if you have the gettimeofday function.  */
#define HAVE_GETTIMEOFDAY 1

/* Define if you have the gmtime_r function.  */
#define HAVE_GMTIME_R 1

/* Define if you have the inet_addr function.  */
/* #undef HAVE_INET_ADDR */

/* Define if you have the inet_aton function.  */
#define HAVE_INET_ATON 1

/* Define if you have the inet_ntoa function.  */
#define HAVE_INET_NTOA 1

/* Define if you have the inet_pton function.  */
#define HAVE_INET_PTON 1

/* Define if you have the ioctl function.  */
#define HAVE_IOCTL 1

/* Define if you have the localtime_r function.  */
#define HAVE_LOCALTIME_R 1

/* Define if you have the memcpy function.  */
#define HAVE_MEMCPY 1

/* Define if you have the memmove function.  */
#define HAVE_MEMMOVE 1

/* Define if you have the mkdir function.  */
#define HAVE_MKDIR 1

/* Define if you have the nl_langinfo function.  */
#define HAVE_NL_LANGINFO 1

/* Define if you have the open function.  */
#define HAVE_OPEN 1

/* Define if you have the opendir function.  */
#define HAVE_OPENDIR 1

/* Define if you have the pipe function.  */
#define HAVE_PIPE 1

/* Define if you have the pthread_mutexattr_setkind_np function.  */
#define HAVE_PTHREAD_MUTEXATTR_SETKIND_NP 1

/* Define if you have the pthread_mutexattr_settype function.  */
#define HAVE_PTHREAD_MUTEXATTR_SETTYPE 1

/* Define if you have the readdir_r function.  */
#define HAVE_READDIR_R 1

/* Define if you have the realpath function.  */
#define HAVE_REALPATH 1

/* Define if you have the rename function.  */
#define HAVE_RENAME 1

/* Define if you have the rmdir function.  */
#define HAVE_RMDIR 1

/* Define if you have the sched_yield function.  */
#define HAVE_SCHED_YIELD 1

/* Define if you have the select function.  */
#define HAVE_SELECT 1

/* Define if you have the setlocale function.  */
#define HAVE_SETLOCALE 1

/* Define if you have the sigaction function.  */
#define HAVE_SIGACTION 1

/* Define if you have the sleep function.  */
#define HAVE_SLEEP 1

/* Define if you have the stat function.  */
#define HAVE_STAT 1

/* Define if you have the strerror function.  */
#define HAVE_STRERROR 1

/* Define if you have the time function.  */
#define HAVE_TIME 1

/* Define if you have the uname function.  */
#define HAVE_UNAME 1

/* Define if you have the unlink function.  */
#define HAVE_UNLINK 1

/* Define if you have the utime function.  */
#define HAVE_UTIME 1

/* Define if you have the <arpa/inet.h> header file.  */
#define HAVE_ARPA_INET_H 1

/* Define if you have the <bstring.h> header file.  */
/* #undef HAVE_BSTRING_H */

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <dlfcn.h> header file.  */
#define HAVE_DLFCN_H 1

/* Define if you have the <execinfo.h> header file.  */
#define HAVE_EXECINFO_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <inttypes.h> header file.  */
#define HAVE_INTTYPES_H 1

/* Define if you have the <langinfo.h> header file.  */
#define HAVE_LANGINFO_H 1

/* Define if you have the <locale.h> header file.  */
#define HAVE_LOCALE_H 1

/* Define if you have the <net/if.h> header file.  */
#define HAVE_NET_IF_H 1

/* Define if you have the <netdb.h> header file.  */
#define HAVE_NETDB_H 1

/* Define if you have the <netinet/in.h> header file.  */
#define HAVE_NETINET_IN_H 1

/* Define if you have the <pwd.h> header file.  */
#define HAVE_PWD_H 1

/* Define if you have the <stdint.h> header file.  */
#define HAVE_STDINT_H 1

/* Define if you have the <sys/config.h> header file.  */
/* #undef HAVE_SYS_CONFIG_H */

/* Define if you have the <sys/filio.h> header file.  */
/* #undef HAVE_SYS_FILIO_H */

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/select.h> header file.  */
#define HAVE_SYS_SELECT_H 1

/* Define if you have the <sys/socket.h> header file.  */
#define HAVE_SYS_SOCKET_H 1

/* Define if you have the <sys/stat.h> header file.  */
#define HAVE_SYS_STAT_H 1

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/types.h> header file.  */
#define HAVE_SYS_TYPES_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Name of package */
#define PACKAGE "libjava"

/* Version number of package */
#define VERSION "0.0.7"

/* Define if the compiler is configured for setjmp/longjmp exceptions. */
/* #undef SJLJ_EXCEPTIONS */

/* Indicate that linker is not able to 8-byte align static data */
/* #undef JV_LINKER_CANNOT_8BYTE_ALIGN_STATICS */

/* Required define if using POSIX threads */
/* #undef _REENTRANT */

/* Required define if using POSIX threads */
#define _POSIX_PTHREAD_SEMANTICS 1

/* Define if hash synchronization is in use */
#define JV_HASH_SYNCHRONIZATION 1

/* Define if you have the iconv() function. */
#define HAVE_ICONV 1

/* Define as const if the declaration of iconv() needs const. */
#define ICONV_CONST 

/* Define if your <locale.h> file defines LC_MESSAGES. */
#define HAVE_LC_MESSAGES 1

/* Required define if using POSIX threads */
/* #undef _REENTRANT */

/* Define if struct hostent_data is defined in netdb.h */
/* #undef HAVE_STRUCT_HOSTENT_DATA */

/* Define if dlopen is available */
#define HAVE_DLOPEN 1

/* The number of bytes in type void * */
#define SIZEOF_VOID_P 4

/* Short GCJ version ID */
#define GCJVERSION "3.3"

/* Define if <inttypes.h> is available */
#define HAVE_INTTYPES_H 1

/* Define if <inttypes.h> is available */
#define JV_HAVE_INTTYPES_H 1

