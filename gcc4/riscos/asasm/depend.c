/*
 * Copyright (c) 1997 Darren Salt
 * Copyright (c) 2004-2008 GCCSDK Developers
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
 * depend.c
 */

#include <stdio.h>

#include "error.h"
#include "depend.h"
#include "main.h"

static FILE *dependFile;
static const char *sourceFileName;

const char *DependFileName = NULL;

void
dependInit (const char *srcFileName)
{
  sourceFileName = srcFileName;
}


void
dependFinish (void)
{
  if (dependFile != NULL)
    {
      fclose(dependFile);
      dependFile = NULL;
    }
}


void
dependPut (const char *pre, const char *name, const char *post)
{
  if (dependFile == NULL)
    return;
  fprintf (dependFile, "%s%s%s", pre, name, post);
  /* may need further processing */
}


void
dependOpen (const char *objname)
{
  if (dependFile != NULL || DependFileName == NULL)
    return;

  if ((dependFile = fopen (DependFileName, "w")) == NULL)
    {
      error (ErrorSerious, FALSE, "Failed to open dependencies file \"%s\"",
             DependFileName);
      DependFileName = NULL;
    }
  else
    {
      if (objname[0] == '@' && objname[1] == '.')
	objname += 2;
      dependPut ("", objname, ":");
      dependPut (" ", sourceFileName, "");
    }
}
