/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/unixlib/source/netlib/c/readline,v $
 * $Date: 1997/10/09 20:00:20 $
 * $Revision: 1.5 $
 * $State: Exp $
 * $Author: unixlib $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: readline,v 1.5 1997/10/09 20:00:20 unixlib Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unixlib/local.h>

/* Read a line from a file and remove any comments.  */
char *
__net_readline (FILE *file, char *buffer, int length)
{
  char *hash;

  do
    {
      /* Read a line from the file */
      if (fgets (buffer, length, file) == NULL)
	return NULL;

      /* Format the line */
      if ((hash = strchr (buffer, '#')) != NULL)
	{
	  /* Throw away any comments on the line */
	  *hash = '\0';
	}
      else
	{
	  /* Remove the newline */
	  buffer[strlen (buffer) - 1] = '\0';
	}
    }
  while (strlen (buffer) == 0);

  return buffer;
}
