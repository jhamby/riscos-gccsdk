/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/local.h,v $
 * $Date: 2000/07/15 14:52:17 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 * This file should eventually contain most / all of the unixlib specific
 * functions.
 ***************************************************************************/


#ifndef __UNIXLIB_LOCAL_H
#define __UNIXLIB_LOCAL_H 1

#ifndef __STDDEF_H
#include <stddef.h>
#endif

#ifdef __UNIXLIB_INTERNALS

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

/* Prevent inclusion of <stdio.h>.  */
#ifndef __FILE_declared
#define __FILE_declared 1
typedef struct __iobuf FILE;
#endif

#endif /* __UNIXLIB_INTERNALS */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __UNIXLIB_INTERNALS

/* Returns TRUE if specified range is ok, Otherwise returns FALSE.
   Uses OS_ValidateAddress (PRMs 1-379).  */
extern int __address_valid (const void *__start, const void *__end);

/* Generate a file serial number. This should distinguish the file from
   all other files on the same device.  */
extern __ino_t __get_file_ino (const char *__directory,
			       const char *__filename);

/* Return the converted to canonicalised RISCOS filename, or NULL on
   failure.  */
extern char * __fd_to_name (int __riscos_fd, char *__buf, size_t __buflen);

/* Convert a file mode into a attribute mask suitable for
   passing to RISC OS.  */
extern int __set_protection (__mode_t __mode);

/* Convert a RISC OS attribute mask into a 'stat' compatible
   file access mode.  */
extern __mode_t __get_protection (int __attrib);

/* Return nonzero if DIR is an existent directory.  */
extern int __isdir (const char *__dir);

/* Return nonzero if OBJECT exists as file, dir or image.  */
extern int __object_exists (const char *__object);

/* Return nonzero if DIR is an existent directory. Don't call __uname */
extern int __isdir_raw (const char *__dir);

/* Return nonzero if OBJECT exists as file, dir or image.
   Don't call __uname.  */
extern int __object_exists_raw (const char *__object);

/* Convert RISC OS format 5 byte time into Unix format time.
   The lowest significant 4 bytes is passed 'low' and the most
   significant byte in 'high'. */
extern __time_t __cvt_riscos_time (unsigned int __high, unsigned int __low);

/* Convert Unix time into RISC OS five byte time.
   The lowest significant 4 bytes are returned in 'low' and the
   most significant byte in 'high'.  */
extern void __cvt_unix_time (__time_t __unix_time, unsigned int *__high,
			     unsigned int *__low);

/* Return 0 if 'socket' is both a valid file descriptor
   and refers to a socket device.  Return -1 if not.  */
extern int __socket_valid (int __socket);

/* Open a file descriptor.  Returns the file descriptor on success,
   -1 on failure.  */
extern int __open (int __fd, const char *__file, int __oflag, int __mode);

/* Close a file descriptor.  Returns zero on success, -1 on failure.  */
extern int __close (int __fd);

/* Re-open a file descriptor.  Returns the file descriptor on success,
   -1 on failure.  */
extern int __reopen (int __fd, const char *__file, int __oflag, ...);

/* Read a line from a file and remove any comments.  */
extern char *__net_readline (FILE *__file, char *__buffer, int __length);

/* Initialise unix file name translation.  */
extern void __riscosify_init (void);

#if 0
extern int __sfixadd_l (const char *__sfix, ...);
extern int __sfixadd_v (const char *const *__sfixes);
extern int __sfixdel_l (const char *__sfix, ...);
extern int __sfixdel_v (const char *const *__sfixes);
#endif

/* Search special suffix list for __sfix (zero char terminated).  Returns
   NULL if suffix isn't to be used for suffix swapping, ptr to suffix
   otherwise.  */
extern const char *__sfixfind (const char *__sfix);

extern const char __filename_char_map[256];

#endif  /* __UNIXLIB_INTERNALS */

/* This bit map controls how riscosify processes filenames.
   Flags value for internal conversions (in this thread?) of UnixLib.
   The default value is 0.
     Conversion on
     Don't truncate (hence don't drop vowels)
     Do perform suffix processing
     Don't create suffix dirs (open/rename functions DO create dirs).
     Don't look at/react on ",xyz" filetype extensions.  */

extern int __riscosify_control;

/* Don't actually process filenames, copy verbatim into the output buffer.  */
#define __RISCOSIFY_NO_PROCESS		0x0040

/* Don't truncate path elements (default), providing buffer is large enough.  */
#define __RISCOSIFY_DONT_TRUNCATE	0x0000
/* Truncate at various shorter lengths, suitable for various filing systems.  */
#define __RISCOSIFY_LONG_TRUNCATE	0x0010	/* 55 - longfiles */
#define __RISCOSIFY_MEDIUM_TRUNCATE	0x0020	/* 19 - sparkfs is buggy */
#define __RISCOSIFY_SHORT_TRUNCATE	0x0030	/* 10 - filecore */
/* Extract above truncate option and map to 0-3.  */
#define __RISCOSIFY_TRUNCATE_VALUE(flags) ((flags & 0x0030) >> 4)

/* If path element is too long, drop vowels before truncating.  */
#define __RISCOSIFY_DROP_VOWEL		0x0080

/* Disable suffix swapping.  */
#define __RISCOSIFY_NO_SUFFIX		0x0100

/* Disable checking if section between two '/'s exists as directory.  */
#define __RISCOSIFY_DONT_CHECK_DIR	0x0200

/* accept only if dir found above is in the suffix list.  */
#define __RISCOSIFY_CHECK_DIR_IS_SUFFIX	0x0400

/* Disable reverse suffix swapping (RISC OS -> Unix).  */
#define __RISCOSIFY_NO_REVERSE_SUFFIX	0x0800

/* Support file type extensions: ",xyz" at the end of the Unix filename
   means that the file on creation gets the filetype xyz (3 hex nibbles).
   If the filetype extension is not specified or this option isn't set,
   the filetype for freshly created files is `Text' (0xfff).  */
#define __RISCOSIFY_FILETYPE_EXT    	0x1000

/* When set and __RISCOSIFY_FILETYPE_EXT is set, RISC OS text files
   will get a ",fff" extension when translated to their Unix form.  */
#define __RISCOSIFY_FILETYPE_FFF_EXT	0x2000

/* Mask of acceptable values. Keep other bits zero. Checks may be made.  */
#define __RISCOSIFY_MASK		0x3FF0

/* Value indicating that __riscosify[_std] didn't see a filetype extension
   in it's argument __name or that it wasn't instructed to look for one.  */
#define __RISCOSIFY_FILETYPE_NOTFOUND 	-1

/* Value indicating for __unixify that there is no filetype (e.g. a
   directory) even when __RISCOSIFY_FILETYPE_EXT is specified as one of
   the flag bits.  */
#define __RISCOSIFY_FILETYPE_NOTSPECIFIED -1

/* Convert Unix filenames/pathnames to Risc OS format creating the final
   directory if necessary and CREATE_DIR is non-zero.
   Pass Risc OS pathnames through unchanged.
   Returns pointer to terminating '\0' in buffer,
   or NULL if there was a problem.  */
extern char *__riscosify (const char *__name, int __create_dir,
			  int __riscosify_flags,
			  char *__buffer, size_t __buf_len,
			  int *__filetype);

extern char *__riscosify_std (const char *__name, int __create_dir,
			      char *__buffer, size_t __buf_len,
			      int *__filetype);

/* Convert `__name' into a Unix style pathname and store in `buffer'.
   If buffer is non-null then it is at least buf_len long.  If buffer
   is null then a buffer is malloc'ed to store the Unix pathname.
   Return a pointer to the terminating `\0' in the generated Unix pathname
   or null on failure.  */
extern char *__unixify (const char *__name, int __unixify_flags,
			char *__buffer, size_t __buf_len, int __filetype);

extern char *__unixify_std (const char *__name,
       	       	       	    char *__buffer, size_t __buf_len,
       	       	       	    int __filetype);

/* #define __riscosify_std(n,c,b,s) __riscosify (n,c,__riscosify_control,b,s) */

#ifndef __UNIXLIB_OLD_UNAME

#ifdef __UNIXLIB_FAULT_UNAME
/* This should make finding references *very* easy - spot the compiler
   errors!  */

extern struct boom *__uname (void);
extern struct boom *__uname_control;
extern struct boom *__uname_dont_pack_ptr;
#endif

#else /* __UNIXLIB_OLD_UNAME */

#if 0
#define __sfixseg(a) ((__sfixfind (a) ? -1 : 0))
#endif

/* Convert a Unix format filename to RISC OS format.  If __cflag is
   non-zero then missing subdirectories are automatically created.  */
extern char *__uname (const char *__name, int __cflag);
extern int __uname_control;

/* This bit map controls how uname processes filenames.

   The default value is 6 (__UNAME_DROP_VOWEL | __UNAME_LONG_TRUNC)

   Bit zero: if set then no processing is done at all.
   Bit one: if unset then truncate each path element to 10 characters.
   Bit one: if set then the maximum filename length is 55 characters.
   Bit two: if set then if the path element is too long, drop vowels
	    before attempting to truncate.  */

#define __UNAME_NO_PROCESS 0x01
#define __UNAME_LONG_TRUNC 0x02
#define __UNAME_DROP_VOWEL 0x04


/* Just declare __uname_dont_pack to disable vowel dropping.  */
extern int *__uname_dont_pack_ptr;
#endif /* __UNIXLIB_OLD_UNAME */

#ifdef __cplusplus
}
#endif

#endif
