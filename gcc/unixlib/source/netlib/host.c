/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/netlib/host.c,v $
 * $Date: 2002/02/07 10:19:31 $
 * $Revision: 1.2.2.1 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: host.c,v 1.2.2.1 2002/02/07 10:19:31 admin Exp $";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <unixlib/local.h>

/* File handle for the hosts file.  */
static FILE *hostfile = NULL;

/* Set to 1 if the database file should be kept open between
   calls to these routines.  */
static int keepopen = 0;

static int __sethostent (int allowrewind);
static struct hostent *__gethostent (void);

/* Open and rewind the hosts file.  */
int
sethostent (int stayopen)
{
  /* Record whether the file should be kept open */
  keepopen = stayopen;

  return __sethostent (1);
}

/* Do the real work of opening/rewinding the hosts file.  */
static int
__sethostent (int allowrewind)
{
  /* Open or rewind the file as necessary */
  if (hostfile)
    {
      if (allowrewind)
	rewind (hostfile);
    }
  else
    {
      hostfile = fopen ("InetDBase:Hosts", "r");
    }

  return (hostfile == NULL) ? -1 : 0;
}

/* Fetch the next entry from the hosts file.  */
struct hostent *
gethostent ()
{
  struct hostent *host;

  /* Open the file if necessary */
  if (hostfile == NULL)
    if (__sethostent (0) == -1)
      return NULL;

  /* Do the actual read */
  host = __gethostent ();

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endhostent ();

  return host;
}

/* Do the real work of getting an entry from the file.  */
struct hostent *
__gethostent ()
{
  static struct hostent host =
  {
    NULL, NULL, AF_INET, sizeof (struct in_addr), NULL
  };

  char **item;
  char line[256];
  char *element;
  int aliases;

  /* Free up any memory in use */
  if (host.h_name)
    {
      for (item = host.h_aliases; *item; item++)
	free (*item);
      for (item = host.h_addr_list; *item; item++)
	free (*item);
      free (host.h_name);
      free (host.h_aliases);
      free (host.h_addr_list);

      host.h_name = NULL;
    }

  /* Read a line from the file */
  if (__net_readline (hostfile, line, sizeof (line) - 1) == NULL)
    return NULL;

  /* Extract the address from the line */
  host.h_addr_list = malloc (2 * sizeof (struct in_addr *));
  host.h_addr_list[0] = malloc (sizeof (struct in_addr));
  host.h_addr_list[1] = NULL;
  element = strtok (line, " \t");
  ((struct in_addr *)(void *)(host.h_addr_list[0]))->s_addr =
    inet_addr (element);

  /* Extract the offical hostname from the line */
  element = strtok (NULL, " \t");
  host.h_name = strdup (element);

  /* Initialialise the alias list */
  host.h_aliases = malloc (sizeof (char *));
  host.h_aliases[0] = NULL;
  aliases = 1;

  /* Extract the aliases */
  while ((element = strtok (NULL, " \t")) != NULL)
    {
      aliases += 1;
      host.h_aliases = realloc (host.h_aliases, aliases * sizeof (char *));
      host.h_aliases[aliases - 2] = strdup (element);
      host.h_aliases[aliases - 1] = NULL;
    }

  return &host;
}

/* Close the hosts file.  */
int
endhostent ()
{
  int status = 0;

  /* If its open, close it */
  if (hostfile)
    {
      status = fclose (hostfile);
      hostfile = 0;
    }

  return status;
}

/* Search the hosts file for a given host name.  */
struct hostent *
gethostbyname (const char *name)
{
  struct hostent *host;
  char **alias;

  /* Try the resolver first */
  if ((host = _gethostbyname (name)) != NULL)
    {
      return host;
    }

  /* Open/rewind the file */
  if (__sethostent (1) == -1)
    return NULL;

  /* Look through the file for a match */
  while ((host = __gethostent ()) != NULL)
    {
      /* Does the offical name match? */
      if (strcmp (host->h_name, name) == 0)
	break;

      /* Do any of the aliases match? */
      for (alias = host->h_aliases; *alias; alias++)
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
    endhostent ();

  return host;
}

/* Search the hosts file for a given address.  */
struct hostent *
gethostbyaddr (const char *addr, int len, int type)
{
  struct hostent *host;
  char **address;

  /* Try the resolver first */
  if ((host = _gethostbyaddr (addr, len, type)) != NULL)
    {
      return host;
    }

  /* Open/rewind the file */
  if (__sethostent (1) == -1)
    return NULL;

  /* Look through the file for a match */
  while ((host = __gethostent ()) != NULL)
    {
      /* If the type and length don't match, try the next one */
      if ((host->h_length != len) || (host->h_addrtype != type))
	continue;

      /* Do any of the addresses match? */
      for (address = host->h_addr_list; *address; address++)
	{
	  if (memcmp (*address, addr, len) == 0)
	    break;
	}

      /* Did any of the aliases match? */
      if (*address)
	break;
    }

  /* Close the file unless the user has prohibited it */
  if (!keepopen)
    endhostent ();

  return host;
}
