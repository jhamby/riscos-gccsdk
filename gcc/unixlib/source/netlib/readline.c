/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/readline.c,v $
 * $Date: 2000/07/15 14:52:25 $
 * $Revision: 1.1.1.1 $
 * $State: Exp $
 * $Author: nick $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: readline.c,v 1.1.1.1 2000/07/15 14:52:25 nick Exp $";
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
