/* Allow this file to be included multiple times
   with different settings of NDEBUG.  */
#undef assert
#undef __assert

#ifdef NDEBUG
#define assert(ignore) ((void) 0)
#else

#ifndef __GNUC__

#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (expression, __FILE__, __LINE__)))

#define __assert(expression, file, lineno)  \
  (printf ("%s:%u: failed assertion\n", file, lineno),	\
   abort (), 0)

#else

#if defined(__STDC__) || defined (__cplusplus)

#ifdef __cplusplus
extern "C" void __assert (const char *, const char *, const char *, int)
extern "C" void __assert2(const char *, const char *, const char *, int);
#else
extern void __assert(const char *, const char *, int);
extern void __assert2(const char *, const char *, const char *, int);
#endif


#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert2 (#expression, __func__, __FILE__, __LINE__)))

#else /* no __STDC__ and not C++; i.e. -traditional.  */

extern void __assert(const char *, const char *, int);

#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (#expression, __FILE__, __LINE__)))

#endif /* no __STDC__ and not C++; i.e. -traditional. */
#endif /* no __GNU__; i.e., /bin/cc.  */
#endif
