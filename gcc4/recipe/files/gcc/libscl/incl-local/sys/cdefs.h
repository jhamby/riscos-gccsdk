/* Internal SCL sys/cdefs.h
 * Copyright (c) 2008 UnixLib Developers
 */

#ifndef __INTERNAL_SYS_CDEFS_H
#define __INTERNAL_SYS_CDEFS_H

#define strong_alias(name, aliasname) _strong_alias(name, aliasname)
#define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name)));

/* This comes between the return type and function name in
   a function definition to make that definition weak.  */
#define weak_function __attribute__ ((weak))
#define weak_const_function __attribute__ ((weak, __const__))

#define weak_alias(name, aliasname) _weak_alias(name, aliasname)
#define _weak_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((weak, alias (#name)));

#endif
