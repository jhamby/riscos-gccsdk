/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/clib/unixlib/local.h,v $
 * $Date: 2003/08/18 22:35:36 $
 * $Revision: 1.9 $
 * $State: Exp $
 * $Author: joty $
 *
 * This file should eventually contain most / all of the unixlib specific
 * functions.
 ***************************************************************************/


#ifndef __UNIXLIB_LOCAL_H
#define __UNIXLIB_LOCAL_H 1

#ifdef __UNIXLIB_INTERNALS

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_FILE
#include <stdio.h>

#endif /* __UNIXLIB_INTERNALS */

#ifndef __UNIXLIB_FEATURES_H
#include <unixlib/features.h>
#endif

#define __need_size_t
#include <stddef.h>


__BEGIN_DECLS

#ifdef __UNIXLIB_INTERNALS

/* Generate a file serial number. This should distinguish the file from
   all other files on the same device.  */
extern __ino_t __get_file_ino (const char *__directory,
			       const char *__filename);

/* Return the converted to canonicalised RISC OS filename, or NULL on
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

/* Return nonzero if DIR is an existent directory.
   Don't call __riscosify[_std] ().  */
extern int __isdir_raw (const char *__dir);

/* Return nonzero if OBJECT exists as file, dir or image.
   Don't call __riscosify[_std] ().  */
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

extern void __sfixinit (const char *list);
extern void __sdirinit (void);

/* Search special suffix list for __sfix (zero char terminated).  Returns
   NULL if suffix isn't to be used for suffix swapping, ptr to suffix
   otherwise.  */
extern const char *__sfixfind (const char *__sfix);

extern const char __filename_char_map[256];

#endif  /* __UNIXLIB_INTERNALS */

/* This bit map controls how riscosify[_std]() processes filenames.
   Flags value for internal conversions (in this thread?) of UnixLib.
   The default value is 0 unless user program overrules this by
   defining an explicit instance of __riscosify_control of its own.
     Conversion on
     Do perform suffix processing
     Don't create suffix dirs (open/rename functions DO create dirs).
     Don't look at/react on ",xyz" filetype extensions.
     Set filetype according to MimeMap.  */

extern int __riscosify_control; /* Note: this is a weak symbol.  */

/* Bits 1 - 5 (incl), 7 and 15 - 31 (incl) of __riscosify_control are not
   allocated.  */

/* When set, __riscosify[_std]() won't try to recognise RISC OS filenames
   but assume that all filenames are Unix based.  */
#define __RISCOSIFY_STRICT_UNIX_SPECS   0x0001
/* Don't actually process filenames, copy verbatim into the output buffer.
   When set, this has priority over all other bits.  */
#define __RISCOSIFY_NO_PROCESS		0x0040

/* Disable suffix swapping (suffix swapping is implicitly disabled for
   directories).  */
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

/* If the filename has a unix-style extension, look up the extension
   using RISC OS MimeMap SWIs and set a filetype.  If the filetype
   could not be found, use 0xFFF.  */
#define __RISCOSIFY_FILETYPE_NOT_SET    0x4000

/* Mask of acceptable values. Keep other bits zero. Checks may be made.  */
#define __RISCOSIFY_MASK                0x7F41

/* Value indicating that __riscosify[_std] didn't see a filetype extension
   in its argument __name or that it wasn't instructed to look for one.  */
#define __RISCOSIFY_FILETYPE_NOTFOUND   -1

/* Value indicating for __unixify that there is no filetype (e.g. a
   directory) even when __RISCOSIFY_FILETYPE_EXT is specified as one of
   the flag bits.  */
#define __RISCOSIFY_FILETYPE_NOTSPECIFIED -1

/* Convert Unix filenames/pathnames to RISC OS format creating the final
   directory if necessary and CREATE_DIR is non-zero.
   Pass RISC OS pathnames through unchanged (only when
   __RISCOSIFY_STRICT_UNIX_SPECS is clear).
   Returns pointer to terminating '\0' in buffer,
   or NULL if there was a problem.  */
extern char *__riscosify (const char *__name, int __create_dir,
			  int __riscosify_flags,
			  char *__buffer, size_t __buf_len,
			  int *__filetype);

/* Calls __riscosify() with __riscosify_flags equal to
   __riscosify_control.  */
extern char *__riscosify_std (const char *__name, int __create_dir,
			      char *__buffer, size_t __buf_len,
			      int *__filetype);

#ifdef __UNIXLIB_INTERNALS

/* Gets the __riscosify_control value which can be defined by
   the global variable __riscosify_control in the user program.
   Returns a copy of __riscosify_control_internal (whom its default
   value is 0) when __riscosify_control is not defined.  */
extern int __get_riscosify_control (void);
/* Sets the __riscosify_control value when it's defined.
   Otherwise __riscosify_control_internal gets written.  */
extern void __set_riscosify_control (int __riscosify_flags);

#endif  /* __UNIXLIB_INTERNALS */

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

/* Get an object's filetype, object type, etc and do some common checks.
   Returns nonzero and sets errno on error. Returns riscosified filename
   in __buffer. __objtype, __ftype and __attr may be NULL if not needed. */
int
__object_get_attrs (const char *__ux_file, char *__buffer, size_t __buf_len,
                    int *__objtype, int *__ftype, int *__loadaddr,
                    int *__execaddr, int *__length, int *__attr);

#define __ATTR_NOTSPECIFIED (-1)

/* Sets an objects filetype and attributes (either can be set to
   __ATTR_NOTSPECIFIED to leave unchanged).
   Returns nonzero and sets errno on error.  */
int
__object_set_attrs (const char *__ux_file, char *__buffer, size_t __buf_len,
                    int __ftype, int __attr);

__END_DECLS

#endif
