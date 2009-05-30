/*
 * Copyright (C) 1999-2000 Robin Watts/Justin Fletcher
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "options.h"
#include "throwback.h"

void ErrorFatal(const char *err, ...) {

  va_list args;

  va_start(args, err);
  fprintf(stderr, "Serious Error: ");
  vfprintf(stderr, err, args);
  va_end(args);
  if (opt.infile!=NULL && opt.infile[0]!='\0' && opt.atline!=0)
  {
    fprintf(stderr, "\n               at %s, line %i\n",opt.infile,opt.atline);
    if (opt.throwback)
    {
      va_start(args, err);
      vThrowbackf(s_seriouserror,opt.infile,opt.atline,err,args);
      va_end(args);
    }
    else
      fputc('\n',stderr);
  }
  else
    fputc('\n',stderr);
  exit(EXIT_FAILURE);
}

void Warning(const char *err, ...) {

  va_list args;

  va_start(args, err);
  fprintf(stderr, "Warning: ");
  vfprintf(stderr, err, args);
  va_end(args);
  fprintf(stderr, "\n");
  if (opt.infile!=NULL && opt.infile[0]!='\0' && opt.atline!=0)
    if (opt.throwback)
    {
      va_start(args, err);
      vThrowbackf(s_warning,opt.infile,opt.atline,err,args);
      va_end(args);
    }
}

/* This will eventually do either warning or fatal error depending
   on a switch */
void CMHGWarning(const char *err, ...) {

  va_list args;

  va_start(args, err);
  fprintf(stderr, "Warning: ");
  vfprintf(stderr, err, args);
  va_end(args);
  fprintf(stderr, " (CMunge only)\n");
  if (opt.infile!=NULL && opt.infile[0]!='\0' && opt.atline!=0)
    if (opt.throwback)
    {
      va_start(args, err);
      vThrowbackf(s_warning,opt.infile,opt.atline,err,args);
      va_end(args);
    }
}
