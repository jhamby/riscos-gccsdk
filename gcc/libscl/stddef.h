/* stddef.h

   For use with the GNU compilers and the SharedCLibrary.
   (c) Copyright 1997, Nick Burrett.  */

#ifndef __STDDEF_H
#define __STDDEF_H

/* Signed type of difference of two pointers.  */
#ifndef __PTRDIFF_TYPE__
#define __PTRDIFF_TYPE__ int
#endif
typedef __PTRDIFF_TYPE__ ptrdiff_t;


/* Unsigned type of sizeof something.  */
#ifndef __size_t
#define __size_t 1
#endif
#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ unsigned int
#endif
#ifndef size_t
typedef __SIZE_TYPE__ size_t;
#endif

/* Wide character type.  */
#ifndef __WCHAR_TYPE__
#define __WCHAR_TYPE__ int
#endif
#ifndef __cplusplus
typedef __WCHAR_TYPE__ wchar_t;
#endif

typedef void *ptr_t;

#ifndef NULL
#define NULL (0)
#endif

/* Offset of member 'member' in a struct of type 'type'.  */
#define offsetof(type, member) ((size_t) &((type *)0)->member)

#endif
