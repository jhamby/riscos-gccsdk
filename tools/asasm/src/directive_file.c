/*
 * AS an assembler for ARM
 * Copyright (c) 1992 Niklas Röjemo
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2000-2012 GCCSDK Developers
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * directive_file.c
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive_file.h"
#include "error.h"
#include "filestack.h"
#include "include.h"
#include "input.h"
#include "lit.h"
#include "main.h"
#include "put.h"

/**
 * Called for GET / INCLUDE
 */
bool
c_get (void)
{
  char *fileName;
  if ((fileName = strdup (Input_Rest ())) == NULL)
    errorOutOfMem ();
  char *cptr;
  for (cptr = fileName; *cptr && !isspace ((unsigned char)*cptr); cptr++)
    /* */;
  if (*cptr)
    {
      *cptr++ = '\0'; /* must be a space */
      while (*cptr && isspace ((unsigned char)*cptr))
	cptr++;
      if (*cptr && *cptr != ';')
	error (ErrorError, "Skipping extra characters '%s' after filename", cptr);
    }

  if (!FS_PushFilePObject (fileName) && option_verbose)
    fprintf (stderr, "Including file \"%s\" as \"%s\"\n", fileName, FS_GetCurFileName ());
  free (fileName);
  return false;
}

/**
 * Implements LNK.
 */
bool
c_lnk (void)
{
  char *fileName;
  if ((fileName = strdup (Input_Rest ())) == NULL)
    errorOutOfMem ();
  char *cptr;
  for (cptr = fileName; *cptr && !isspace ((unsigned char)*cptr); cptr++)
    /* */;
  if (*cptr)
    {
      *cptr++ = '\0'; /* must be a space */
      while (*cptr && isspace ((unsigned char)*cptr))
	cptr++;
      if (*cptr && *cptr != ';')
	error (ErrorError, "Skipping extra characters '%s' after filename", cptr);
    }

  /* Terminate all outstanding macro calls and finish the current file.  */
  while (gCurPObjP->type == POType_eMacro)
    FS_PopPObject (true);
  FS_PopPObject (true);

  /* Dump literal pool.  */
  Lit_DumpPool ();

  if (!FS_PushFilePObject (fileName) && option_verbose)
    fprintf (stderr, "Linking to file \"%s\" as \"%s\"\n", fileName, FS_GetCurFileName ());
  free (fileName);
  return false;
}

/**
 * Implements BIN / INCBIN.
 */
bool
c_incbin (void)
{
  char *fileName;
  if ((fileName = strdup (Input_Rest ())) == NULL)
    errorOutOfMem ();
  char *cptr;
  for (cptr = fileName; *cptr && !isspace ((unsigned char)*cptr); cptr++)
    /* */;
  *cptr = '\0';

  ASFile asFile;
  FILE *binfp = Include_Get (fileName, &asFile, true);
  if (!binfp)
    error (ErrorError, "Cannot open file \"%s\"", fileName);
  else
    {
      if (option_verbose)
	fprintf (stderr, "Including binary file \"%s\" as \"%s\"\n", fileName, asFile.canonName);

      /* Include binary file.  */
      int c;
      while ((c = getc (binfp)) != EOF)
	Put_Data (1, c);
      fclose (binfp);
    }

  free (fileName);
  ASFile_Free (&asFile);
  return false;
}

/**
 * Implements END.
 */
bool
c_end (void)
{
  if (gCurPObjP->type == POType_eMacro)
    error (ErrorError, "Cannot use END within a macro");
  else
    FS_PopPObject (false);
  return false;
}
