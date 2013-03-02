/* UnixLib internal function declarations.
   Copyright (c) 2002-2013 UnixLib Developers.  */

#ifndef __INTERNAL_LOCAL_H
#define __INTERNAL_LOCAL_H 1

#ifndef __UNIXLIB_TYPES_H
#include <unixlib/types.h>
#endif

#define __need_FILE
#include <stdio.h>

#ifndef __UNIXLIB_FEATURES_H
#include <features.h>
#endif

#define __need_size_t
#include <stddef.h>


__BEGIN_DECLS

/* Put value into the OS environment. Will not overwrite an existing
   variable of the same name if replace is 0 */
extern int __addenv_to_os (const char *name, const char *value, int replace);

/* Remove NAME from the OS environment (always succeeds and can be used for
   tail calls from unsetenv().  */
extern int __remenv_from_os (const char *name);

/* Generate a file serial number. This should distinguish the file from
   all other files on the same device.  */
extern __ino_t __get_file_ino (const char *__directory,
			       const char *__filename);

/* Convert a file mode into a attribute mask suitable for
   passing to RISC OS.  */
extern int __set_protection (__mode_t __mode);

/* Convert a RISC OS attribute mask into a 'stat' compatible
   file access mode.  */
extern __mode_t __get_protection (int __attrib);

/* Return 0 if UX_OBJ does not exist; 1 if it is an existing file or an
   ImageFS image when __feature_imagefs_is_file is 1; 2 if it  an
   existing directory or an ImageFS image when __feature_imagefs_is_file
   is 0.
   UX_OBJ is a Unix path.  */
extern int __object_exists_ux (const char *__ux_obj);

/* Return 0 if RO_OBJ does not exist; 1 if it is an existing file or an
   ImageFS image when __feature_imagefs_is_file is 1; 2 if it  an
   existing directory or an ImageFS image when __feature_imagefs_is_file
   is 0.
   RO_OBJ is a RISC OS path.  */
extern int __object_exists_ro (const char *__ro_obj);

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

/* Read a line from a file and remove any comments.  */
extern char *__net_readline (FILE *__file, char *__buffer, int __length);

/* Initialise unix file name translation.  */
extern void __riscosify_init (void);

extern void __sdirinit (void);

/* Search special suffix list for __sfix of length __len.  Returns
   NULL if suffix isn't to be used for suffix swapping, ptr to suffix
   otherwise.  */
extern const char *__sfixfind (const char *__sfix, size_t __len);

extern const char __filename_char_map[256];

/* Gets the __riscosify_control value which can be defined by
   the global variable __riscosify_control in the user program.
   Returns a copy of __riscosify_control_internal (with a default
   value of 0) when __riscosify_control is not defined.  */
extern int __get_riscosify_control (void);
/* Sets the __riscosify_control value when it's defined.
   Otherwise __riscosify_control_internal gets written.  */
extern void __set_riscosify_control (int __riscosify_flags);

/* Get an object's filetype, object type, etc and do some common checks.
   Returns nonzero and sets errno on error. Returns riscosified filename
   in __buffer. __objtype, __ftype and __attr may be NULL if not needed. */
extern int
__object_get_attrs (const char *__ux_file, char *__buffer, size_t __buf_len,
		    int *__objtype, int *__ftype, int *__loadaddr,
		    int *__execaddr, int *__length, int *__attr);

/* Get an object's filetype, object type, etc and do some common checks.
   Returns nonzero and sets errno on error. Returns riscosified filename
   in __buffer. __objtype, __ftype and __attr may be NULL if not needed.
   If the object is a symlink, return info on the link rather than the
   object that it leads to.  */
extern int
__object_get_lattrs (const char *__ux_file, char *__buffer, size_t __buf_len,
		    int *__objtype, int *__ftype, int *__loadaddr,
		    int *__execaddr, int *__length, int *__attr);

#define __ATTR_NOTSPECIFIED (-1)

/* Sets an objects filetype and attributes (either can be set to
   __ATTR_NOTSPECIFIED to leave unchanged).
   Returns nonzero and sets errno on error.  */
extern int
__object_set_attrs (const char *__ux_file, char *__buffer, size_t __buf_len,
		    int __ftype, int __attr);

/* Returns malloced block of memory holding DDEUtils_Prefix non zero-length
   string value or NULL when there isn't one set.  Caller needs to free
   memory after use.  */
extern const char *__get_dde_prefix (void);

/* Canonicalise given pathname and return result in a malloced buffer.
   Caller gets ownership.
   Returns NULL when something went wrong, errno will be set
   appropriately.  */
extern char *__canonicalise_path (const char *pathname, const char *pathvar);

/* Canonicalise given file handle and return result in a malloced buffer.
   Caller gets ownership.
   Returns NULL when something went wrong, errno will be set
   appropriately.  */
extern char *__canonicalise_handle (int fh);

__END_DECLS

#endif
