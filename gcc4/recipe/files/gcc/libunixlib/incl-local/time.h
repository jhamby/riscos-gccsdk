/* Internal UnixLib time.h
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#ifndef __TIME_H
#include_next <time.h>
#endif

#if !defined(__INTERNAL_TIME_H) && defined(__TIME_H)
#define __INTERNAL_TIME_H

__BEGIN_DECLS

#ifdef __TIME_H
/* Convert broken time to 5-byte RISC OS time.  */
extern void __cvt_broken_time (const struct tm *__brokentime,
			       char *__riscos_time);

/* Common function for localtime() and gmtime().  */
extern struct tm *__calendar_convert (int __swinum, const time_t *__tp,
				      struct tm *resultp);

/* Common function for ctime() and asctime().  */
extern char *__standard_time (const char *__riscos_time, char *__buffer);
#endif

__END_DECLS

#endif
