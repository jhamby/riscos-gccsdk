/*
 * depend.c
 * Copyright © 1997 Darren Salt
 * Copyright © 2005 GCCSDK Developers
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
