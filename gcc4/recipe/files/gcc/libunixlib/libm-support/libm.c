/* Math library version numbers.
   Copyright (c) 2005 UnixLib Developers.  */

/* This file exists only for the purpose of satisfying the requirement
   that a libm.a must exist.  In UnixLib we combine the math library
   with the C library, but several user-land applications assume that
   the math library exists as a seperate lib.  We build one, just containing
   this source file.  */

#include <features.h>

int __libm_version_major = __ULIBC__;
int __libm_version_minor = __ULIBC_MINOR__;

