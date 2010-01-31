/* Internal UnixLib features.h
   UnixLib declaration of its features.
   Copyright (c) 2004, 2005, 2006, 2008 UnixLib Developers.  */

#ifndef __UNIXLIB_FEATURES_H
#include_next <features.h>
#endif

#if !defined(__INTERNAL_FEATURES_H) && defined(__UNIXLIB_FEATURES_H)
#define __INTERNAL_FEATURES_H

#ifdef __BEGIN_DECLS
__BEGIN_DECLS
#endif

/* Amount to align the wimpslot or dynamic area. */
#define __DA_WIMPSLOT_ALIGNMENT (32*1024-1)

#ifndef __GNUC__
#define __builtin_expect(exp, c)  (exp)
#endif

/* Gets the __feature_imagefs_is_file value which can be defined by
   the global variable __feature_imagefs_is_file in the user program.
   Returns a copy of __feature_imagefs_is_file_internal (its default
   value is 1) when __feature_imagefs_is_file is not defined.  */
extern int __get_feature_imagefs_is_file (void);
#if 0
/* Currently the code __set_feature_imagefs_is_file commented out because
   we don't need it in UnixLib internally.  */

/* Sets the __feature_imagefs_is_file value when it's defined.
   Otherwise __feature_imagefs_is_file_internal gets written.  */
extern void __set_feature_imagefs_is_file (int __value);
#endif

#ifdef __END_DECLS
__END_DECLS
#endif

#endif
