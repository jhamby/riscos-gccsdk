/*******************************************************************
 * File:     filename
 * Purpose:  Filename translations for supporting unix-style naming
 *           schemes
 * Author:   Justin Fletcher
 * Date:     20 Jul 2005
 * Note:     The indention of this file is to support the most general
 *           forms of names that might be passed to commands that expect
 *           (or are expected to expect) unix-style filenames. This
 *           means that simple extensions will be translated from the
 *           unix-style format to the RISC OS format, eg file.c becomes
 *           c.file.
 *           The extensions which will be processed by this operation
 *           are described through a parameter to the functions.
 ******************************************************************/

#include <stdio.h>
#include <string.h>
#include "filename.h"

/* #define DEBUG */

#ifdef DEBUG
#define dprintf if (1) printf
#else
#define dprintf if (0) printf
#endif

/* We use a static buffer for the filenames returned; so don't leave a pointer
   hanging around in variables.
   The use of the two separate buffers is a legacy thing; at some point
   it could be removed as we are unlikely to ever expose the non-extension
   functions (the caller can just pass a null extension list) */
static char filenamebuffer[1024];
static char unixbuffer[1024];

static char *unix_to_riscos(const char *src)
{
  char previous='.'; /* Previous is a native filename char */
  char *dest=filenamebuffer;
  char c;

  dprintf("unix_to_riscos('%s')\n",src);
  while ((c=*src++)!='\0')
  {
    if (c=='.')
    {
      if (previous=='.' && *src=='/')
      {
        src++;
        continue; /* /./ */
      }
      if (previous=='.' && *src=='.' && src[1]=='/')
      {
        src++;
        c='^';
      }
      else
        c='/';
    }
    else if (c=='/')
    {
      if (previous=='.')
        continue;
      c='.';
    }
    previous=c;
    *dest++=c;
  }
  *dest='\0';
  dprintf("  = '%s'\n",filenamebuffer);
  return filenamebuffer;
}

/*************************************************** Gerph *********
 Function:     filename_unixtoriscos
 Description:  Convert a filename from Unix-style to RISC OS
 Parameters:   src-> the source string to convert (unix style)
               extlist-> a colon separated list of extensions to
                         reverse
 Returns:      pointer to a static string
 ******************************************************************/
char *filename_unixtoriscos(const char *src, const char *extlist)
{
  char *buffer=unix_to_riscos(src);
  char *trail;
  char *lastcomp = strrchr(buffer, '.');
  dprintf("RISC OS extension name translation: %s ",buffer);

  if (extlist != NULL)
  {
    /* lastcomp will point to the last component of the filename (the leaf) */
    if (lastcomp==NULL) lastcomp = buffer;
    else lastcomp++;

    trail = strrchr(lastcomp, '/');
    if (trail && trail != lastcomp)
    {
      trail++;
      while (*extlist)
      {
        const char *extend = strchr(extlist, ':');
        int extlen;
        if (extend == NULL)
          extend = extlist+strlen(extlist);
        extlen = extend - extlist;

        if (strncmp(trail, extlist, extlen)==0 &&
            trail[extlen] == '\0')
        {
          /* The extension matched, so we perform the reversal */
          memmove(lastcomp + extlen+1, lastcomp,
                  strlen(lastcomp) - extlen - 1);
          memcpy(lastcomp, extlist, extlen);
          lastcomp[extlen] = '.';
          break;
        }

        extlist = extend;
        if (*extlist == ':')
          extlist++;
      }
    }
  }
  dprintf("= %s\n",buffer);
  return buffer;
}

/* Perform the reverse translation - RISCOS -> Unix.
*/
static char *riscos_to_unix(const char *src)
{
  char *dest=unixbuffer;
  char c;
  dprintf("riscos_to_unix('%s')",src);

  while ((c=*src++)!='\0')
  {
    if (c=='.')
      c='/';
    else if (c=='/')
      c='.';
    *dest++=c;
  }
  *dest='\0';
  dprintf(" = %s\n",unixbuffer);
  return unixbuffer;
}


/*************************************************** Gerph *********
 Function:     filename_riscostounix
 Description:  Convert a filename from RISC OS-style to Unix-style
 Parameters:   src-> the source string to convert (RISC OS style)
               extlist-> a colon separated list of extensions to
                         reverse
 Returns:      pointer to a static string
 ******************************************************************/
char *filename_riscostounix(const char *src, const char *extlist)
{
  char *buffer=riscos_to_unix(src);
  char *lastcomp = strrchr(buffer, '/');
  char *prevcomp;
  dprintf("Unix extension name translation: %s ",buffer);

  if (extlist != NULL)
  {
    /* Name will most likely be in the form :
          directory/ext/leaf
       and we need to make it in to :
          directory/leaf.ext
     */
    if (lastcomp == NULL || lastcomp == buffer)
      return buffer; /* We're done; there's no component in it to reverse */

    prevcomp = lastcomp;
    while (prevcomp != buffer)
    {
      prevcomp--;
      if (*prevcomp=='/')
        break;
    }

    if (*prevcomp == '/')
      prevcomp++;
    lastcomp++;

    while (*extlist)
    {
      const char *extend = strchr(extlist, ':');
      int extlen;
      if (extend == NULL)
        extend = extlist+strlen(extlist);
      extlen = extend - extlist;

      if (memcmp(prevcomp, extlist, extlen)==0 && prevcomp[extlen]=='/')
      {
        int lastcomplen = strlen(lastcomp);
        /* The extension matched, so we perform the reversal */
        memmove(prevcomp, lastcomp, lastcomplen);
        prevcomp[lastcomplen] = '.';
        memcpy(prevcomp + lastcomplen + 1, extlist, extlen);
        break;
      }

      extlist = extend;
      if (*extlist == ':')
        extlist++;
    }
  }

  dprintf("= %s\n",buffer);
  return buffer;
}
