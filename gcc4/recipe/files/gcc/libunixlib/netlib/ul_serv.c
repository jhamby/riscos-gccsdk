/* setservent (), getservent (), endservent (), getservbyname (),
 * getservbyport ()
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

/* File handle for the services file.  */
static FILE *servfile = NULL;

/* Set to 1 if the database file should be kept open between
   calls to these routines.  */
static int keepopen = 0;

static int __setservent (int allowrewind);
static struct servent *__getservent (void);

/* Open and rewind the services file.  */
void
setservent (int stayopen)
{
  PTHREAD_UNSAFE

  /* Record whether the file should be kept open */
  keepopen = stayopen;

  (void) __setservent (1);
}

/* Do the real work of opening/rewinding the services file.  */
static int
__setservent (int allowrewind)
{
  /* Open or rewind the file as necessary */
  if (servfile)
    {
      if (allowrewind)
	rewind (servfile);
    }
  else
    {
      servfile = fopen ("InetDBase:Services", "r");
    }

  return (servfile == NULL) ? -1 : 0;
}

/* Fetch the next entry from the services file.  */
struct servent *
getservent ()
{
  struct servent *serv;

  PTHREAD_UNSAFE

  /* Open the file if necessary */
  if (servfile == NULL)
    if (__setservent (0) == -1)
      return NULL;

  /* Do the actual read */
  serv = __getservent ();

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endservent ();

  return serv;
}

/* Do the real work of getting an entry from the file.  */
struct servent *
__getservent ()
{
  static struct servent serv =
  {
    NULL, NULL, 0, NULL
  };

  char **item;
  char line[256];
  char *element;
  int aliases;

  /* Free up any memory in use */
  if (serv.s_name)
    {
      for (item = serv.s_aliases; *item; item++)
	free (*item);
      free (serv.s_name);
      free (serv.s_aliases);
      free (serv.s_proto);

      serv.s_name = NULL;
    }

  /* Read a line from the file */
  if (__net_readline (servfile, line, sizeof (line) - 1) == NULL)
    return NULL;

  /* Extract the offical service name from the line */
  element = strtok (line, " \t");
  serv.s_name = strdup (element);

  /* Extract the port number from the line */
  element = strtok (NULL, "/");
  serv.s_port = htons (atoi (element));

  /* Extract the protocol name from the line */
  element = strtok (NULL, " \t");
  serv.s_proto = strdup (element);

  /* Initialise the alias list */
  serv.s_aliases = malloc (sizeof (char *));
  serv.s_aliases[0] = NULL;
  aliases = 1;

  /* Extract the aliases */
  while ((element = strtok (NULL, " \t")) != NULL)
    {
      aliases += 1;
      serv.s_aliases = realloc (serv.s_aliases, aliases * sizeof (char *));
      serv.s_aliases[aliases - 2] = strdup (element);
      serv.s_aliases[aliases - 1] = NULL;
    }

  return &serv;
}

/* Close the services file.  */
void
endservent (void)
{
  PTHREAD_UNSAFE

  /* If it's open, close it */
  if (servfile)
    {
      (void) fclose (servfile);
      servfile = 0;
    }
}

/* Search the services file for a given service name.  */
struct servent *
getservbyname (const char *name, const char *proto)
{
  struct servent *serv;
  char **alias;

  PTHREAD_UNSAFE

  /* Open/rewind the file */
  if (__setservent (1) == -1)
    return NULL;

  /* Look through the file for a match */
  while ((serv = __getservent ()) != NULL)
    {
      /* Give up now if the protocol doesn't match */
      if (proto && (strcmp (serv->s_proto, proto) != 0))
	continue;

      /* Does the offical name match? */
      if (strcmp (serv->s_name, name) == 0)
	break;

      /* Do any of the aliases match? */
      for (alias = serv->s_aliases; *alias; alias++)
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
    endservent ();

  return serv;
}

/* Search the services file for a given port.  */
struct servent *
getservbyport (int port, const char *proto)
{
  struct servent *serv;

  PTHREAD_UNSAFE

  /* Open/rewind the file */
  if (__setservent (1) == -1)
    return NULL;

  /* Look through the file for a match */
  while ((serv = __getservent ()) != NULL)
    {
      /* Give up now if the protocol doesn't match */
      if (proto && (strcmp (serv->s_proto, proto) != 0))
	continue;

      /* If the port matches, we've found it */
      if (serv->s_port == port)
	break;

    }

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endservent ();

  return serv;
}
