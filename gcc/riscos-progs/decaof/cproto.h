/*
 * protect non-ANSI C Compilers from function prototype arguments
 *
 * Andy Duplain, BT Customer Systems, Brighton, UK.  duplain@btcs.bt.co.uk
 */

#ifndef __CPROTO_H
#define __CPROTO_H

#if defined (__STDC__) || defined(__cplusplus)
#define P__(s)	s
#define ANSI_C
#else /* PCC compiler */
#define P__(s)	()
#undef ANSI_C
#endif /* ANSI compiler */

#endif /* __CPROTO_H */
