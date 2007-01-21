/*
 * Copyright (C) 1999-2000 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "datestamp.h"
#include "error.h"
#include "mem.h"
#include "options.h"

void DateStamp(void) {

  char *date, *date0;
  char *s;
  char datebuf[24];
  int i;

  if (opt.mode_errors)
    return; /* No work is required if we're only generating error blocks */

  i = 32;
  if (opt.datestring)
    i = 4 + strlen(opt.datestring)+1;
  date0 = date = Malloc(strlen(opt.help)+i);
  s = opt.help;

  /* Skip whitespace */
  while (isspace(*s))
    s++;
  /* Copy name, counting chars */
  i = 0;
  do {
    while (!isspace(*s)) {
      *date++ = *s++;
      i++;
    }
    /* Skip more whitespace, adding tabs as required */
    while (isspace(*s))
      s++;
    if (((*s < '0') || (*s > '9')) && (*s != 0)) {
      *date++ = 32;
      i++;
    }
  } while (((*s < '0') || (*s > '9')) && (*s != 0));
  *date++ = 9;
  i = (i+8)&~7;
  if (i < 16)
    *date++ = 9;
  /* Copy the version number */
  if ((*s < '0') || (*s > '9'))
    ErrorFatal("Malformed help-string found: %s", opt.help);
  i = 0;
  while ((*s >= '0') && (*s <= '9')) {
    i = (i*10)+(*s)-'0';
    *date++ = *s++;
  }
  /* Copy the '.' */
  if (*s == '.')
    *date++ = *s++;
  else
    ErrorFatal("Malformed help-string found: %s", opt.help);
  while ((*s >= '0') && (*s <= '9')) {
    i = (i*10)+(*s)-'0';
    *date++ = *s++;
  }
  while (!isspace(*s) && (*s))
    *date++ = *s++;
  while (isspace(*s))
    s++;

  opt.version = i;

  if (opt.datestring) {
    char *dptr = datebuf;
    char *iptr = opt.datestring;
    char c;
    int wrong = 0;
    *dptr++ = ' ';
    *dptr++ = '(';
    *dptr++ = c = *iptr++; wrong = wrong || !isdigit(c);
    *dptr++ = c = *iptr++; wrong = wrong || !isdigit(c);
    *dptr++ = c = *iptr++; wrong = wrong || !(c==' ');
    *dptr++ = c = *iptr++; wrong = wrong || !isalpha(c);
    *dptr++ = c = *iptr++; wrong = wrong || !isalpha(c);
    *dptr++ = c = *iptr++; wrong = wrong || !isalpha(c);
    *dptr++ = c = *iptr++; wrong = wrong || !(c==' ');
    *dptr++ = c = *iptr++; wrong = wrong || !isdigit(c);
    *dptr++ = c = *iptr++; wrong = wrong || !isdigit(c);
    *dptr++ = c = *iptr++; wrong = wrong || !isdigit(c);
    *dptr++ = c = *iptr++; wrong = wrong || !isdigit(c);
    *dptr++ = ')';
    *dptr++ = '\0';
    if (wrong)
      ErrorFatal("Malformed date-string found: %s", opt.help);
    /* The above checking form of the date string check will confirm
       that the input string is actually sensible. Previously we used:
          sprintf(datebuf, " (%s)", opt.datestring);
     */
  } else {
    /* FIXME: This will only work sensibly if the locale is set to something
              that returns english format months, because everyone expects
              English months. Still, it's better than the old kernel calls
              I had. */
    time_t curtime = time(NULL);
    if (curtime == (time_t)-1)
      ErrorFatal("Failed to retrieve the current time");
    if (strftime(datebuf, sizeof(datebuf), " (%d %b %Y)", gmtime(&curtime)) == 0)
      ErrorFatal("Failed to convert time indication in readable format");
  }

  strcpy(date, datebuf);
  date += strlen(datebuf);
  if (*s)
    *date++ = 32;
  while (*s)
    *date++ = *s++;
  *date = 0;
  opt.help = date0;

  opt.date = Malloc(strlen(datebuf)+1);
  strcpy(opt.date, datebuf+2);
  opt.date[strlen(opt.date)-1] = 0;
}
