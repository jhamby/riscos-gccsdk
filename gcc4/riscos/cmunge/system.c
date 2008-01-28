/*
 * Copyright (C) 2003 Justin Fletcher
 */

#if !defined(__riscos) || !defined(__TARGET_SCL__)
/* To make WEXITSTATUS() available which converts the return code of
   system() into a real exit status value.  */
#  define _XOPEN_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef __riscos
#include "swis.h"
#endif
#include "error.h"

/* #define DEBUG */

#ifdef DEBUG
#define dprintf if (1) printf
#else
#define dprintf if (0) printf
#endif

/* This function is here for two reasons ...

   1. To provide escaping for the invoked comand when used under linux.
      Under RISC OS we can happily invoke the command directly without any
      problems (except as mentioned in 2.)

      Basically this gets around the issue of using 'system variables' under
      linux. Trying to issue a command with (for example) -I<Lib$Dir> in its
      path will fail on linux because the < is taken as a redirection. Such
      arguments will be quoted and $'s escaped where necessary.

   2. To pass the command to DDEUtils if it's even a little long under
      RISC OS. The SCL will only invoke DDEUtils if the command line
      exceeds 255 characters, which is fine if a canonical command name is
      used, but if a Run$Path-based variable is invoked (eg CC, so quite
      often) it may overflow very easily as the path is expanded.

      Because we don't invoke anything that's not a library command written
      in C, this won't matter.
 */

int our_system(const char *cmdtxt)
{
  int rc;
#if defined(__riscos) && defined(__TARGET_SCL__)

#ifndef DDEUtils_SetCLSize
#define DDEUtils_SetCLSize (0x42581)
#endif
#ifndef DDEUtils_SetCL
#define DDEUtils_SetCL     (0x42582)
#endif

  _kernel_oserror *err;
  char *cmd = malloc(strlen(cmdtxt)*4+256); /* Lots more space, just in case */
  char *acc;
  if (cmd==NULL)
    ErrorFatal("*** NOT ENOUGH MEMORY TO INVOKE COMMAND ***\n");

  /* Copy the command name to our temporary buffer */
  acc = cmd;
  while (*cmdtxt > ' ')
    *acc++ = *cmdtxt++;
  *acc = '\0';

  /* Now pass the length to DDEUtils */
  if ((err=_swix(DDEUtils_SetCLSize,_IN(0),strlen(cmdtxt)+1))!=NULL)
  {
    ErrorFatal("Cannot set Extended CLI size (%s); aborting\n",err->errmess);
  }

  /* And the command line itself */
  if ((err=_swix(DDEUtils_SetCL,_IN(0),cmdtxt))!=NULL)
  {
    ErrorFatal("Cannot set Extended CLI (%s); aborting\n",err->errmess);
  }

  dprintf("Starting command '%s' with DDEUtils CL '%s'\n", cmd, cmdtxt);
  /* The Pace SharedCLibrary is broken with regard to command line invocation.
     Apparently, it explicitly clears the DDEUtils command line before
     invocation of the command with system(). This means that we cannot set
     the command line here using DDEUtils if we use system(). Our fix is to
     call _kernel_system instead, which does not perform the DDEUtils command
     line clearing and which functions correctly on all known SCL versions.
     (Fault reported by Alex Waugh)
   */
  rc = _kernel_system(cmd, 0);
  dprintf("  got return code %i\n", rc);
  /* Clear DDEUtils' command line buffer as this seems not to happen when the
     command can not be found.  */
  if (rc != EXIT_SUCCESS
      && (err = _swix(DDEUtils_SetCLSize, _IN(0), 0)) != NULL)
  {
    ErrorFatal("Cannot reset Extended CLI size (%s); aborting\n",err->errmess);
  }
  free(cmd);
#else
  int need_quotes =0;
  char *acc;
  const char *lastspace;
  char *cmd = malloc(strlen(cmdtxt)*4+256); /* Lots more space, just in case */
  if (cmd==NULL)
    ErrorFatal("*** NOT ENOUGH MEMORY TO INVOKE COMMAND ***\n");
  lastspace = cmdtxt;
  acc = cmd;
  /* printf("Expanding: '%s'\n", cmdtxt); */
  while (1)
  {
    if (*cmdtxt == ' ' || *cmdtxt == '\0')
    {
      /* printf("... word '%.*s'\n", cmdtxt-lastspace, lastspace); */
      if (need_quotes)
        *acc++='"';
      while (*lastspace != ' ' && *lastspace != '\0')
      {
        if (*lastspace == '$')
          *acc++='\\';
        *acc++=*lastspace++;
      }
      if (need_quotes)
        *acc++='"';
      *acc++=*cmdtxt;
      if (*cmdtxt++=='\0')
        break;
      need_quotes = 0;
      lastspace = cmdtxt;
    }
    else
    {
      if (*cmdtxt == '<' && cmdtxt[1] != '\0' && cmdtxt[1] != ' ')
        need_quotes = 1;
      if (*cmdtxt == '>' && cmdtxt != lastspace)
        need_quotes = 1;
      cmdtxt++;
    }
  }
  /* printf("About to execute... %s\n",cmd); */
  rc = system(cmd);
  free(cmd);

  /* Fake the RISC OS form of 'could not start process' */
  if (rc==-1)
    rc = -2;
  else
    rc = WEXITSTATUS(rc);
#endif
  return rc;
}
