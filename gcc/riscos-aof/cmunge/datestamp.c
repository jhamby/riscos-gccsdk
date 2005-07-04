/*
 * Copyright (C) 1999-2000 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "kernel.h"

#include "datestamp.h"
#include "error.h"
#include "mem.h"
#include "options.h"

void DateStamp(void) {

  char *date, *date0;
  char *s;
  char datebuf[24];
  char osword[5];
  int i;
  _kernel_swi_regs regs;

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
    sprintf(datebuf, " (%s)", opt.datestring);
  } else {
    /* OS_Word 14, 3 */
    osword[0] = 3;
    regs.r[0] = 14;
    regs.r[1] = (int)osword;
    _kernel_swi(0x07, &regs, &regs);
    /* OS_ConvertDateAndTime */
    regs.r[0] = (int)osword;
    regs.r[1] = (int)datebuf;
    regs.r[2] = 24;
    regs.r[3] = (int)" (%DY %M3 %CE%YR)";
    _kernel_swi(0xC1, &regs, &regs);
  }

  sprintf(date,  datebuf);
  date += strlen(datebuf);
  if (*s)
    *date++ = 32;
  while (*s)
    *date++ = *s++;
  *date = 0;
  opt.help = date0;

  opt.date = Malloc(strlen(datebuf)+1);
  sprintf(opt.date, datebuf+2);
  opt.date[strlen(opt.date)-1] = 0;
}
