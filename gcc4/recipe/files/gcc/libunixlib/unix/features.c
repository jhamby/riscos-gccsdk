/* Set UnixLib run-time features based on environment variables.
   Copyright (c) 2002-2012 UnixLib Developers.  */

#include <string.h>

#include <unixlib/local.h>
#include <internal/unix.h>
#include <internal/local.h>
#include <internal/os.h>

/* #define DEBUG 1 */
#ifdef DEBUG
#  include <sys/debug.h>
#endif

static const char __sfix_default[] = "c:cc:f:h:i:ii:l:o:p:s:y";

/* 'cli' can only be a RISC OS executable path.  */
static const char *
get_program_name (const char *cli, char *fname_buf, size_t fname_buf_len)
{
  if (fname_buf_len == 0)
    return NULL;

  /* Did the user specify the program name ?  */
  if (&__program_name)
    return __program_name;

  /* Skip any initial whitespace.  */
  while (*cli == ' ')
    cli++;

  /* Find the end of the program name.  Set 'start' to mark the
     beginning of the program name. Use '.' and ':' as separation
     characters to calculate this.  */
  const char *start = cli;
  while (*cli != ' ' && *cli != '\0')
    {
      if (*cli == '.' || *cli == ':')
	/* Point to the character after the separation char.  */
	start = cli + 1;
      cli++;
    }
  const char * const end = cli;

  /* Copy the program name into 'out'.  Its bounds should now be
     marked out by 'start' and 'end'.  */
  char *out = fname_buf;
  while (--fname_buf_len && start != end)
    *out++ = *start++;

  /* Zero terminate it.  */
  *out = '\0';

  return fname_buf;
}

/* Build up an environment variable name, variable, with the UnixEnv$
   prefixed to it.  If program_name is non-zero then look for the
   variable as UnixEnv$program_name$variable.  If nothing is found by
   this method then look for it as UnixEnv$variable.

   The intention is to allow the programmer the opportunity to turn on/off
   certain UnixLib runtime functionality, for example the filename
   translations.  This can either be done globally i.e. once set, all
   applications will be affected, or per-process.  */
static char *
env (const char *program_name, const char *variable, char *buffer,
     size_t bufsiz)
{
  char envvar[128];
  char *base_ptr, *result;

  base_ptr = stpcpy (envvar, __UNIX_ENV_PREFIX);
  if (program_name != NULL)
    {
      char *ptr = stpcpy (base_ptr, program_name);
      *ptr++ = '$';
      stpcpy (ptr, variable);

#ifdef DEBUG
      debug_printf ("-- features.c (env): Looking for '%s'.\n", envvar);
#endif

      result = __getenv_from_os (envvar, buffer, bufsiz, NULL);
    }
  else
    result = NULL;

  if (!result)
    {
      stpcpy (base_ptr, variable);

#ifdef DEBUG
      debug_printf ("-- features.c (env): Looking for '%s'.\n", envvar);
#endif

      result = __getenv_from_os (envvar, buffer, bufsiz, NULL);
    }

#ifdef DEBUG
  debug_printf ((result) ? "   -> Found\n" : "   -> Not found\n");
#endif
  return result;
}

/* We have to be careful with the string processing because not enough
   of UnixLib will have been initialised to use more powerful functions.  */
static void
features (const char *progname)
{
  struct __sul_process *sulproc = __ul_global.sulproc;
  char varbuf[256];
  char *ptr;

  /* Feature "nonametrans".  */
  ptr = env (progname, "nonametrans", varbuf, sizeof (varbuf) - 1);
  if (ptr != NULL)
    __set_riscosify_control(__RISCOSIFY_NO_PROCESS);

  /* Feature "sfix".  */
  ptr = env (progname, "sfix", varbuf, sizeof (varbuf) - 1);
  __sfixinit (ptr ? ptr : __sfix_default);

  /* Feature "uid".  */
  ptr = env (progname, "uid", varbuf, sizeof (varbuf) - 1);
  if (ptr != NULL)
    sulproc->uid = sulproc->euid = (uid_t) __decstrtoui(ptr, NULL);

  /* Feature "coredump".  */
  ptr = env (progname, "coredump", varbuf, sizeof (varbuf) - 1);
  if (ptr != NULL && ptr[0] != '\0')
    __init_coredump (ptr);
}

void
__runtime_features (const char *cli)
{
  char program_name[128];

  /* Initialise runtime features to their default values before querying
     the environment variables.  */
  __sdirinit (); /* Initialise riscosify.  */

  features (NULL);
  features (get_program_name (cli, program_name, sizeof (program_name)));
}
