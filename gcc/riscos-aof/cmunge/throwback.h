/*
  throwback.h

  (c) 1998 Justin Fletcher
*/

#ifndef THROWBACK_H

#define THROWBACK_H

/*
 Throwback provides a means of notifying an external process that a processed
 file contains warnings or errors.
*/

#include <stdarg.h>

typedef enum {
  s_warning=0, s_error, s_seriouserror, s_information
} seriousness_t;

void vThrowbackf(seriousness_t seriousness,char *file,int line,char *format,va_list args);

void Throwback(seriousness_t seriousness,char *file,int line,char *message);

#endif
