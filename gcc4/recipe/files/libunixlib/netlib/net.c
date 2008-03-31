/* setnetent (), getnetent (), endnetent (), getnetbyname (), getentbyaddr ()
 * Copyright (c) 2000-2008 UnixLib Developers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <internal/local.h>
#include <internal/unix.h>
#include <pthread.h>

/* File handle for the nets file.  */
static FILE *netfile = NULL;

/* Set to 1 if the database file should be kept open between
   calls to these routines.  */
static int keepopen = 0;

static int __setnetent (int allowrewind);
static struct netent *__getnetent (void);

/* Open and rewind the nets file.  */
void
setnetent (int stayopen)
{
  PTHREAD_UNSAFE

  /* Record whether the file should be kept open */
  keepopen = stayopen;

  (void) __setnetent (1);
}

/* Do the real work of opening/rewinding the nets file.  */
static int
__setnetent (int allowrewind)
{
  /* Open or rewind the file as necessary */
  if (netfile)
    {
      if (allowrewind)
	rewind (netfile);
    }
  else
    {
      netfile = fopen ("InetDBase:Networks", "r");
    }

  return (netfile == NULL) ? -1 : 0;
}

/* Fetch the next entry from the nets file.  */
struct netent *
getnetent ()
{
  struct netent *net;

  PTHREAD_UNSAFE

  /* Open the file if necessary */
  if (netfile == NULL)
    if (__setnetent (0) == -1)
      return NULL;

  /* Do the actual read */
  net = __getnetent ();

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endnetent ();

  return net;
}

/* Do the real work of getting an entry from the file.  */
struct netent *
__getnetent ()
{
  static struct netent net =
  {
    NULL, NULL, AF_INET, 0
  };

  char **item;
  char line[256];
  char *element;
  int aliases;

  /* Free up any memory in use */
  if (net.n_name)
    {
      for (item = net.n_aliases; *item; item++)
	free (*item);
      free (net.n_name);
      free (net.n_aliases);

      net.n_name = NULL;
    }

  /* Read a line from the file */
  if (__net_readline (netfile, line, sizeof (line) - 1) == NULL)
    return NULL;

  /* Extract the offical network name from the line */
  element = strtok (line, " \t");
  net.n_name = strdup (element);

  /* Extract the address from the line */
  element = strtok (NULL, " \t");
  net.n_net = inet_network (element);

  /* Initialialise the alias list */
  net.n_aliases = malloc (sizeof (char *));
  net.n_aliases[0] = NULL;
  aliases = 1;

  /* Extract the aliases */
  while ((element = strtok (NULL, " \t")) != NULL)
    {
      aliases += 1;
      net.n_aliases = realloc (net.n_aliases, aliases * sizeof (char *));
      net.n_aliases[aliases - 2] = strdup (element);
      net.n_aliases[aliases - 1] = NULL;
    }

  return &net;
}

/* Close the nets file.  */
void
endnetent ()
{
  PTHREAD_UNSAFE

  /* If its open, close it */
  if (netfile)
    {
      (void) fclose (netfile);
      netfile = 0;
    }
}

/* Search the nets file for a given net name.  */
struct netent *
getnetbyname (const char *name)
{
  struct netent *net;
  char **alias;

  PTHREAD_UNSAFE

  /* Open/rewind the file */
  if (__setnetent (1) == -1)
    return NULL;

  /* Look through the file for a match */
  while ((net = __getnetent ()) != NULL)
    {
      /* Does the offical name match? */
      if (strcmp (net->n_name, name) == 0)
	break;

      /* Do any of the aliases match? */
      for (alias = net->n_aliases; *alias; alias++)
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
    endnetent ();

  return net;
}

/* Search the nets file for a given address.  */
struct netent *
getnetbyaddr (uint32_t netaddr, int type)
{
  struct netent *net;

  PTHREAD_UNSAFE

  /* Open/rewind the file */
  if (__setnetent (1) == -1)
    return NULL;

  /* Look through the file for a match */
  while ((net = __getnetent ()) != NULL)
    {
      /* If the type and length match, we've found it */
      if ((net->n_addrtype == type) && (net->n_net == netaddr))
	break;
    }

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endnetent ();

  return net;
}
