/****************************************************************************
 *
 * $Source: /usr/local/cvsroot/gccsdk/unixlib/source/unix/features.c,v $
 * $Date: 2001/09/11 14:16:00 $
 * $Revision: 1.2.2.2 $
 * $State: Exp $
 * $Author: admin $
 *
 ***************************************************************************/

#ifdef EMBED_RCSID
static const char rcs_id[] = "$Id: features.c,v 1.2.2.2 2001/09/11 14:16:00 admin Exp $";
#endif

/* #define DEBUG 1 */

#include <string.h>
#include <unixlib/unix.h>
#include <unixlib/local.h>

/* Get the leaf name from the command line used to run the program.  */
static char *get_program_name (const char *cli, char *fname_buf)
{
  char *out = fname_buf;
  const char *start, *end;

  /* Skip any initial whitespace.  */
  while (*cli == ' ')
    cli++;

  /* Find the end of the program name.  Set 'start' to mark the
     beginning of the program name. Use '.' and '/' as the directory
     separation characters to calculate this.  */
  start = cli;
  while (*cli != ' ' && *cli != '\0')
    {
      if (*cli == '/' || *cli == '.')
	/* Point to the character after the separation char.  */
	start = cli + 1;
      cli++;
    }
  end = cli;

  /* Copy the program name into 'out'.  It's bounds should now be
     marked out by 'start' and 'end'.  */
  while (start != end)
    *out++ = *start++;

  /* Zero terminate it.  */
  *out = '\0';

  return fname_buf;
}

/* Build up an environment variable name, variable, with the UnixEnv$
   prefixed to it.  If program_name is non-zero then look for the
   variable as UnixEnv$program_name$variable, otherwise look for it
   as UnixEnv$variable.

   The intention is to allow the programmer the opportunity to turn on/off
   certain UnixLib runtime functionality, for example the filename
   translations.  This can either be done globally i.e. once set, all
   applications will be affected, or per-process.  */
static char *env (const char *program_name, const char *variable,
		  char *buffer, size_t bufsiz)
{
  char envvar[128];
  char *ptr, *result;

  ptr = stpcpy (envvar, __UNIX_ENV_PREFIX);
  if (program_name != NULL)
    {
      ptr = stpcpy (ptr, program_name);
      *ptr++ = '$';
    }
  ptr = stpcpy (ptr, variable);

#ifdef DEBUG
  __os_print ("features.c (env): Looking for '"); __os_print (envvar);
  __os_print ("'.");
#endif

  result = __getenv_from_os (envvar, buffer, bufsiz);

#ifdef DEBUG
  __os_print ((result) ? "Found\r\n" : "Not found\r\n");
#endif
  return result;
}

/* Allow string to be writable by not making const.  */
static char __sfix_default[] = "a:c:cc:f:h:i:ii:l:o:p:s:y";

/* We have to be careful with the string processing because not enough
   of UnixLib will have been initialised to use more powerful functions.  */
static void features (const char *progname)
{
  char varbuf[256];
  char *ptr;

  ptr = env (progname, "nonametrans", varbuf, sizeof (varbuf) - 1);
  if (ptr != NULL)
    __riscosify_control = __RISCOSIFY_NO_PROCESS;

  ptr = env (progname, "sfix", varbuf, sizeof (varbuf) - 1);
  __sfixinit (ptr ? ptr : __sfix_default);
}

void __runtime_features (const char *cli)
{
  char program_name[128];

  /* Initialise runtime features to their default values before querying
     the environment variables.  */
  __riscosify_control = 0;
  __sdirinit (); /* Initialise riscosify.  */

  features (NULL);
  features (get_program_name (cli, program_name));
}
