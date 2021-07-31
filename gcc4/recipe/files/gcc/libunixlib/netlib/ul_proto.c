/* setprotoent (), getprotoent (), endprotoent (), getprotobyname (),
 * getprotobynumber ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <internal/local.h>
#include <internal/unix.h>
#include <pthread.h>

/* File handle for the protocols file.  */
static FILE *protofile = NULL;

/* Set to 1 if the database file should be kept open between
   calls to these routines.  */
static int keepopen = 0;

static int __setprotoent (int allowrewind);
static struct protoent *__getprotoent (void);

/* Open and rewind the protocols file.  */
void
setprotoent (int stayopen)
{
  PTHREAD_UNSAFE

  /* Record whether the file should be kept open */
  keepopen = stayopen;

  (void) __setprotoent (1);
}

/* Do the real work of opening/rewinding the protocols file.  */
static int
__setprotoent (int allowrewind)
{
  /* Open or rewind the file as necessary */
  if (protofile)
    {
      if (allowrewind)
	rewind (protofile);
    }
  else
    {
      protofile = fopen ("InetDBase:Protocols", "r");
    }

  return (protofile == NULL) ? -1 : 0;
}

/* Fetch the next entry from the protocols file.  */
struct protoent *
getprotoent ()
{
  struct protoent *proto;

  PTHREAD_UNSAFE

  /* Open the file if necessary */
  if (protofile == NULL)
    if (__setprotoent (0) == -1)
      return NULL;

  /* Do the actual read */
  proto = __getprotoent ();

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endprotoent ();

  return proto;
}

/* Do the real work of getting an entry from the file.  */
struct protoent *
__getprotoent ()
{
  static struct protoent proto =
  {
    NULL, NULL, 0
  };

  char **item;
  char line[256];
  char *element;
  int aliases;

  /* Free up any memory in use */
  if (proto.p_name)
    {
      for (item = proto.p_aliases; *item; item++)
	free (*item);
      free (proto.p_name);
      free (proto.p_aliases);

      proto.p_name = NULL;
    }

  /* Read a line from the file */
  if (__net_readline (protofile, line, sizeof (line) - 1) == NULL)
    return NULL;

  /* Extract the offical protocol name from the line */
  element = strtok (line, " \t");
  proto.p_name = strdup (element);

  /* Extract the port number from the line */
  element = strtok (NULL, " \t");
  proto.p_proto = atoi (element);

  /* Initialise the alias list */
  proto.p_aliases = malloc (sizeof (char *));
  proto.p_aliases[0] = NULL;
  aliases = 1;

  /* Extract the aliases */
  while ((element = strtok (NULL, " \t")) != NULL)
    {
      aliases += 1;
      proto.p_aliases = realloc (proto.p_aliases, aliases * sizeof (char *));
      proto.p_aliases[aliases - 2] = strdup (element);
      proto.p_aliases[aliases - 1] = NULL;
    }

  return &proto;
}

/* Close the protocols file.  */
void
endprotoent ()
{
  PTHREAD_UNSAFE

  /* If its open, close it */
  if (protofile)
    {
      (void) fclose (protofile);
      protofile = 0;
    }
}

/* Search the protocols file for a given protocol name.  */
struct protoent *
getprotobyname (const char *name)
{
  struct protoent *proto;
  char **alias;

  PTHREAD_UNSAFE

  /* Open/rewind the file */
  if (__setprotoent (1) == -1)
    return NULL;

  /* Look through the file for a match */
  while ((proto = __getprotoent ()) != NULL)
    {
      /* Does the offical name match? */
      if (strcmp (proto->p_name, name) == 0)
	break;

      /* Do any of the aliases match? */
      for (alias = proto->p_aliases; *alias; alias++)
	{
	  if (strcmp (*alias, name) == 0)
	    break;
	}

      /* Did any of the aliases match? */
      if (*alias)
	break;
    }

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endprotoent ();

  return proto;
}

/* Search the protocols file for a given number.  */
struct protoent *
getprotobynumber (int protonum)
{
  struct protoent *proto;

  PTHREAD_UNSAFE

  /* Open/rewind the file */
  if (__setprotoent (1) == -1)
    return NULL;

  /* Look through the file for a match */
  while ((proto = __getprotoent ()) != NULL)
    {
      /* If the protocol number matches, we've found it */
      if (proto->p_proto == protonum)
	break;
    }

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endprotoent ();

  return proto;
}
