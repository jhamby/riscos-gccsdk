/*******************************************************************
 * File:     apcscli
 * Purpose:  APCS CLI option parsing
 * Author:   Justin Fletcher
 * Date:     29 Dec 2003
 ******************************************************************/

#include <stdio.h>
#include <string.h>
#include "apcscli.h"

/* This is a global variable so that we can use it in the other files */
apcsoptions_t apcsoptions[] = {
  { "32bit",          2, APCS_32BIT,        APCS_32BIT,
    "32 bit APCS variant" },
  { "26bit",          2, APCS_32BIT,        0,
    "26 bit APCS variant" },

  { "reentrant",      5, APCS_REENTRANT,    APCS_REENTRANT,
    "Reentrant APCS variant" },
  { "nonreentrant",   8, APCS_REENTRANT,    0,
    "Non reentrant APCS variant" },

  { "fpe3",           4, APCS_FPE3,         APCS_FPE3,
    "Floating point emulator 3 compatibility" },
  { "fpe2",           4, APCS_FPE3,         0,
    "Floating point emulator 2 compatibility" },

  { "swstackcheck",   4, APCS_SWSTACKCHECK, APCS_SWSTACKCHECK,
    "Software stack checking APCS variant" },
  { "noswstackcheck", 6, APCS_SWSTACKCHECK, 0,
    "No software stack checking APCS variant" },

  { "fpregargs",      3, APCS_FPREGARGS,    APCS_FPREGARGS,
    "FP arguments passed in FP registers" },
  { "nofpregargs",    5, APCS_FPREGARGS,    0,
    "FP arguments are not passed in FP registers" },

  { "fp",             2, APCS_FP,           APCS_FP,
    "Arguments referenced through the frame pointer" },
  { "nofp",           4, APCS_FP,           0,
    "Arguments referenced through the stack pointer" },
  { NULL }
};

/*************************************************** Gerph *********
 Function:     apcscli_parse
 Description:  Parse CLI options from an argument string
 Parameters:   flags = the current APCS flags
               arg-> the string to parse
               errfunc-> the function to call when an error occurs (vararg)
 Returns:      new flags
 ******************************************************************/
unsigned long apcscli_parse(unsigned long flags, const char *arg,
                            void (*errfunc)(char *format, ...))
{
  if (*arg == '3') arg++;
  while (*arg)
  {
    if (*arg=='/')
      arg++;
    else
    {
      const char *argend;
      int arglen;
      argend = strchr(arg,'/');
      if (argend)
        arglen = argend-arg;
      else
        arglen = strlen(arg);

      {
        apcsoptions_t *aarg = &apcsoptions[0];
        while (aarg->name!=NULL)
        {
          if ( strncmp(aarg->name, arg, arglen)==0 &&
               (arglen == aarg->minabbrev ||
                strlen(aarg->name) == arglen)
             )
            break;
          aarg++;
        }
        /* printf("Option '%.*s' matches '%s'\n",arglen,arg,aarg->name); */
        if (aarg->name==NULL)
          errfunc("APCS flag %s not recognised\n", arg);
        flags &=~ aarg->bic;
        flags |=  aarg->orr;
      }

      arg += arglen;
    }
  }
  return flags;
}

/*************************************************** Gerph *********
 Function:     apcscli_help
 Description:  Display a help message, with an optional indication
               of the current state
 Parameters:   flags = the flags to display
               showstate = whether the state in 'flags' should be shown
 Returns:      none
 ******************************************************************/
void apcscli_help(unsigned long flags, int showstate)
{
  int longest=0;
  apcsoptions_t *arg;
  arg = apcsoptions;
  while (arg->name != NULL)
  {
    int len=strlen(arg->name);
    if (len > longest)
      longest = len;
    arg++;
  }
  arg = apcsoptions;

  printf("APCS flags:%s\n", showstate ? " (*=currently selected)" : "");
  while (arg->name != NULL)
  {
    int count;
    int len = strlen(arg->name);
    if (len == arg->minabbrev)
      printf("  /%s   ",arg->name);
    else
      printf("  /%.*s[%s] ",arg->minabbrev, arg->name,
                           &arg->name[arg->minabbrev]);
    for (count=longest-len; count>0; count--)
      putc(' ', stdout);
    if (showstate)
      printf("%s",((flags & arg->bic)==arg->orr) ? "*" : " ");
    printf("%s\n", arg->desc);
    arg++;
  }
}

/*************************************************** Gerph *********
 Function:     apcscli_buildstring
 Description:  Build a command line string from flags
 Parameters:   flags = the flags to build a string from
               ignoreflags = flags which shouldn't be printed
               abbreviate = 1 to only use the minimum string length
               buffer-> the buffer to fill in
               bufferlen = the length of the buffer required
 Returns:      buffer length written, or -ve length if not enough space
 ******************************************************************/
int apcscli_buildstring(unsigned long flags, unsigned long ignoreflags,
                        int abbreviate,
                        char *buffer, int bufferlen)
{
  int totallen = 0;
  apcsoptions_t *arg;
  arg = apcsoptions;

#define ADDTOBUFFER(str,len) if (bufferlen > len) memcpy(buffer, str, len); \
                             buffer += len; \
                             bufferlen -= len; \
                             totallen += len;
  ADDTOBUFFER("3", 1);

  while (arg->name != NULL)
  {
    if ((ignoreflags & arg->bic) == 0)
    {
      int clen;
      if (abbreviate)
        clen = arg->minabbrev;
      else
        clen = strlen(arg->name);

      if ((flags & arg->bic)==arg->orr)
      {
        ADDTOBUFFER("/", 1);
        ADDTOBUFFER(arg->name, clen);
      }
    }

    arg++;
  }
  ADDTOBUFFER("\0", 1);

  if (bufferlen < 0)
    return -totallen;
  return totallen;
}
