/*
 * depend.c
 * Copyright © 1997 Darren Salt
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "error.h"
#include "depend.h"
#include "main.h"

FILE *dependfile;
const char *DependFileName;


void
dependPut (const char *pre, const char *name, const char *post)
{
  if (!dependfile)
    return;
  fprintf (dependfile, "%s%s%s", pre, name, post);
  /* may need further processing */
}


void
dependOpen (const char *objname)
{
  if (dependfile || DependFileName == 0)
    return;
  if ((dependfile = fopen (DependFileName, "w")) == NULL)
    {
      DependFileName = NULL;
      error (ErrorSerious, FALSE, "Failed to open dependencies file");
    }
  else
    {
      if (objname[0] == '@' && objname[1] == '.')
	objname += 2;
      dependPut ("", objname, ":");
      dependPut (" ", SourceFileName, "");
    }
}
