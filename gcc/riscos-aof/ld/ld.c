/* GNU ld implementation as a wrapper to an ARM/RISC OS linker
   for the GNU compiler, with support for the C++ template repository.

   Copyright (C) 1997, 1998, 1999, 2000, 2001 Nick Burrett
   Contributed by Nick Burrett (nick.burrett@btinternet.com)

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* If building for a cross-compilation environment then be sure to
   define CROSS_COMPILE.  */

#include "sdk-config.h"
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#ifndef CROSS_COMPILE
#include <unixlib/local.h>
#endif

#include "obstack.h"
#include "demangle.h"
#include "getopt.h"

#define LD_VERSION "2.22"
#define LD_DATE __DATE__

#ifndef __GNUC__
#undef __attribute__
#define __attribute__(x) /* Ignore */
#endif

/* The maximum number of times we shall re-compile a C++ program to
   instantiate the templates.  With GCC 2.95, I suspect this feature
   is no-longer required.  */
#define MAX_ITERATIONS 17

/* Maximum number of command line arguments that our linker supports.
   FIXME We should remove this feature and support unlimited arguments.  */
#define MAXARGS 2048

/* Obstack allocation and deallocation routines.  */
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

static char *ldout;	  /* pathname for linker errors */
static char *ld_viafile;  /* pathname for linker -via <file> object list  */
static const char *c_file_name; /* pathname of gcc.  */
static struct obstack temporary_obstack;
static struct obstack permanent_obstack;
static char *temporary_firstobj;
static char *temp_filename;

static int tlink_verbose = 0;

typedef struct
{
  char *arg;
} args;

typedef struct list_
{
  char *name;
  struct list_ *next;
} llist;

static llist *library_path = 0;
static llist *libraries = 0;

#define LINKER_DRLINK 0
#define LINKER_LINK_V4 1
#define LINKER_LINK_V5 2

/* Command line.  */
static args *command_line;
static int command_line_offset;
static args *object_list;
static int object_offset;

static void add_library_file (const char *);
static void add_library_search_path (const char *);
static void add_input_file (const char *);
static void add_output_file (const char *);
void ld_error (const char *, ...) __attribute__ ((__noreturn__));
static void llist_add (llist **list, const char *name);
static void llist_free (llist *list);
static void append_arg (args *, int *, const char *);
static void tlink_init (void);
static int tlink_execute (const char *prog, char **argv, char *redir, char *viafile);
static void do_tlink (const char *, char **, args *);
static int choose_temp_base (void);
static void dump_file (char *);
static void ldversion (int);
static void ldhelp (void);
static void parse_args (int, char **);

extern char *riscos_to_unix (const char *, char *);

/* Same as `malloc' but report error if no memory available.  */

static void *
xmalloc (unsigned size)
{
  void *value = malloc (size);
  if (value == 0)
    ld_error ("virtual memory exhausted");
  return value;
}

/* Same as `realloc' but report error if no memory available.  */

static void *
xrealloc (char *ptr, int size)
{
  void *result = realloc (ptr, size);
  if (!result)
    ld_error ("virtual memory exhausted");
  return result;
}

#ifndef HAVE_STPCPY
static char *stpcpy (char *s, const char *s2)
{
  while ((*s++ = *s2++))
    ;

  return s - 1;
}
#endif

static void do_unlink (void)
{
  if (tlink_verbose < 15)
    {
      unlink (ld_viafile);
      unlink (ldout);
    }
}

static void
linker_initialise (void)
{
  FILE *temp;
  int temp_filedesc;

  c_file_name = getenv ("COLLECT_GCC");
  if (c_file_name == 0)
    c_file_name = "gcc";
  obstack_begin (&temporary_obstack, 0);
  obstack_begin (&permanent_obstack, 0);
  temporary_firstobj = (char *) obstack_alloc (&temporary_obstack, 0);

  /* Create a unique filename. Open and close the file, effectively
     creating it, so that the next call to choose_temp_base will
     create a different unique filename.  */
  if (tlink_verbose >= 7)
    printf ("creating 1st temporary file\n");

  temp_filedesc = choose_temp_base ();
  ldout = temp_filename;
  if (temp_filedesc == -1)
    {
      fprintf (stderr, "ld: failed to create temporary filename '%s'\n",
	       ldout);
      exit (1);
    }
  close (temp_filedesc);

  if (tlink_verbose >= 7)
    printf ("creating 2nd temporary file\n");
  /* Create another unique file.  */
  temp_filedesc = choose_temp_base ();
  ld_viafile = temp_filename;
  if (temp_filedesc == -1)
    {
      fprintf (stderr, "ld: failed to create temporary filename '%s'\n",
	       ld_viafile);
      exit (1);
    }
  close (temp_filedesc);

  if (tlink_verbose >= 7)
    printf ("temporary files have been created\n");
}

int
main (int argc, char *argv[])
{
  const char *requested_linker;

  atexit (do_unlink);
  tlink_init ();
  linker_initialise ();

  if (tlink_verbose > 0)
    ldversion (0);

#ifndef STANDARD_EXEC_PREFIX
#define STANDARD_EXEC_PREFIX ""
#endif

#ifdef CROSS_COMPILE
  requested_linker = STANDARD_EXEC_PREFIX "drlink";
#else /* CROSS_COMPILE */
  requested_linker = getenv ("GCC$Linker");

  /* No linker has been specified, try finding one on the Run$Path.  */
  if (requested_linker == NULL || *requested_linker == '\0')
    {
      /* This is to force people to use the correct version of
         drlink i.e. the one distributed with the GCCSDK.

	 In the long term, it makes it easier to support the SDK because
	 little changes in parts of UnixLib, such as struct proc, can
	 make it difficult for `ld' to spawn `drlink'.  Therefore by
	 having this restriction we can make sure it never happens.  */
      requested_linker = STANDARD_EXEC_PREFIX "drlink";
    }
#endif /* CROSS_COMPILE */

  object_list = (args *)xmalloc (sizeof (args) * MAXARGS);
  object_list[0].arg = (char *)0;
  command_line = (args *)xmalloc (sizeof (args) * (MAXARGS >> 2));
  command_line[0].arg = (char *)0;

  object_offset = command_line_offset = 0;
  append_arg (command_line, &command_line_offset, requested_linker);

  if (tlink_verbose >= 5)
    printf ("Parsing command line\n");

  parse_args (argc, argv);

  if (tlink_verbose >= 5)
    printf ("About to do_tlink()\n");

  do_tlink (requested_linker, (char **)command_line, object_list);

  llist_free (library_path);
  llist_free (libraries);
  return 0;
}

void
ld_error (const char *text, ...)
{
  va_list args;
  va_start (args, text);
  fprintf (stderr, "ld fatal error: ");
  vfprintf (stderr, text, args);
  putc ('\n', stderr);
  va_end (args);
  exit (1);
}

/* C++ template repository support details.  */

/* Hash table code.
   The default number of entries to use when creating a hash table.  */
#define DEFAULT_SIZE (1009)

typedef enum {false, true} boolean;

/* Hash table routines.  There is no way to free up a hash table.

   An element in the hash table.  Most uses will actually use a larger
   structure, and an instance of this will be the first field.  */

struct hash_entry
{
  /* Next entry for this hash code.  */
  struct hash_entry *next;
  /* String being hashed.  */
  const char *string;
  /* Hash code.  This is the full hash code, not the index into the
     table.  */
  unsigned long hash;
};

/* A hash table.  */

struct hash_table
{
  /* The hash array.  */
  struct hash_entry **table;
  /* The number of slots in the hash table.  */
  unsigned int size;
  /* A function used to create new elements in the hash table.  The
     first entry is itself a pointer to an element.  When this
     function is first invoked, this pointer will be NULL.  However,
     having the pointer permits a hierarchy of method functions to be
     built each of which calls the function in the superclass.  Thus
     each function should be written to allocate a new block of memory
     only if the argument is NULL.  */
  struct hash_entry *(*newfunc) (struct hash_entry *,
  		    		 struct hash_table *, const char *);
  /* An obstack for this hash table.  */
  struct obstack memory;
};

/* Create a new hash table, given a number of entries.  */

static boolean
hash_table_init_n (struct hash_table *table,
		   struct hash_entry *(*newfunc) (struct hash_entry *,
						  struct hash_table *,
						  const char *),
		   unsigned int size)
{
  unsigned int alloc;

  alloc = size * sizeof (struct hash_entry *);
  if (!obstack_begin (&table->memory, alloc))
    {
      ld_error ("no memory");
      return false;
    }
  table->table = ((struct hash_entry **)
		  obstack_alloc (&table->memory, alloc));
  if (!table->table)
    {
      ld_error ("no memory");
      return false;
    }
  memset ((void *) table->table, 0, alloc);
  table->size = size;
  table->newfunc = newfunc;
  return true;
}

/* Create a new hash table with the default number of entries.  */

static boolean
hash_table_init (struct hash_table *table,
		 struct hash_entry *(*newfunc) (struct hash_entry *,
						struct hash_table *,
						const char *))
{
  return hash_table_init_n (table, newfunc, DEFAULT_SIZE);
}

/* Free a hash table.  */

#if 0
static void
hash_table_free (struct hash_table *table)
{
  obstack_free (&table->memory, (void *) NULL);
}
#endif

/* Look up a string in a hash table.  */

static struct hash_entry *
hash_lookup (struct hash_table *table, const char *string,
	     boolean create, boolean copy)
{
  register const unsigned char *s;
  register unsigned long hash;
  register unsigned int c;
  struct hash_entry *hashp;
  unsigned int len;
  unsigned int index;

  hash = 0;
  len = 0;
  s = (const unsigned char *) string;
  while ((c = *s++) != '\0')
    {
      hash += c + (c << 17);
      hash ^= hash >> 2;
      ++len;
    }
  hash += len + (len << 17);
  hash ^= hash >> 2;

  index = hash % table->size;
  for (hashp = table->table[index];
       hashp != (struct hash_entry *) NULL;
       hashp = hashp->next)
    {
      if (hashp->hash == hash && strcmp (hashp->string, string) == 0)
	return hashp;
    }

  if (! create)
    return (struct hash_entry *) NULL;

  hashp = (*table->newfunc) ((struct hash_entry *) NULL, table, string);
  if (hashp == (struct hash_entry *) NULL)
    return (struct hash_entry *) NULL;
  if (copy)
    {
      char *new;

      new = (char *) obstack_alloc (&table->memory, len + 1);
      if (!new)
	{
	  ld_error ("no memory");
	  return (struct hash_entry *) NULL;
	}
      strcpy (new, string);
      string = new;
    }
  hashp->string = string;
  hashp->hash = hash;
  hashp->next = table->table[index];
  table->table[index] = hashp;

  return hashp;
}

/* Allocate space in a hash table.  */

static void *
hash_allocate (struct hash_table *table, unsigned int size)
{
  void *ret;

  ret = obstack_alloc (&table->memory, size);
  if (ret == NULL && size != 0)
    ld_error ("no memory");
  return ret;
}

/* Base method for creating a new hash table entry.  */

static struct hash_entry *
hash_newfunc (struct hash_entry *entry, struct hash_table *table, const char *string)
{
  if (entry == (struct hash_entry *) NULL)
    entry = ((struct hash_entry *)
	     hash_allocate (table, sizeof (struct hash_entry)));
  return entry;
}

/* Traverse a hash table.  */

#if 0
static void
hash_traverse (struct hash_table *table,
	       boolean (*func) (struct hash_entry *, void *),
	       void *info)
{
  unsigned int i;

  for (i = 0; i < table->size; i++)
    {
      struct hash_entry *p;

      for (p = table->table[i]; p != NULL; p = p->next)
	{
	  if (! (*func) (p, info))
	    return;
	}
    }
}
#endif

typedef struct symbol_hash_entry
{
  struct hash_entry root;
  struct file_hash_entry *file;
  int chosen;
  int tweaking;
  int tweaked;
} symbol;

typedef struct file_hash_entry
{
  struct hash_entry root;
  const char *args;
  const char *dir;
  const char *main;
  int tweaking;
} file;

typedef struct demangled_hash_entry
{
  struct hash_entry root;
  const char *mangled;
} demangled;

static struct hash_table symbol_table;


static struct hash_entry *
symbol_hash_newfunc (struct hash_entry *entry,
		     struct hash_table *table,
		     const char *string)
{
  struct symbol_hash_entry *ret = (struct symbol_hash_entry *) entry;
  if (ret == NULL)
    {
      ret = ((struct symbol_hash_entry *)
	     hash_allocate (table, sizeof (struct symbol_hash_entry)));
      if (ret == NULL)
	return NULL;
    }
  ret = ((struct symbol_hash_entry *)
	 hash_newfunc ((struct hash_entry *) ret, table, string));
  ret->file = NULL;
  ret->chosen = 0;
  ret->tweaking = 0;
  ret->tweaked = 0;
  return (struct hash_entry *) ret;
}

static struct symbol_hash_entry *
symbol_hash_lookup (const char *string,
		    boolean create)
{
  return ((struct symbol_hash_entry *)
	  hash_lookup (&symbol_table, string, create, true));
}

static struct hash_table file_table;

static struct hash_entry *
file_hash_newfunc (struct hash_entry *entry,
		  struct hash_table *table,
		  const char *string)
{
  struct file_hash_entry *ret = (struct file_hash_entry *) entry;
  if (ret == NULL)
    {
      ret = ((struct file_hash_entry *)
	     hash_allocate (table, sizeof (struct file_hash_entry)));
      if (ret == NULL)
	return NULL;
    }
  ret = ((struct file_hash_entry *)
	 hash_newfunc ((struct hash_entry *) ret, table, string));
  ret->args = NULL;
  ret->dir = NULL;
  ret->main = NULL;
  ret->tweaking = 0;
  return (struct hash_entry *) ret;
}

static struct file_hash_entry *
file_hash_lookup (const char *string)
{
  return ((struct file_hash_entry *)
	  hash_lookup (&file_table, string, true, true));
}

static struct hash_table demangled_table;

static struct hash_entry *
demangled_hash_newfunc (struct hash_entry *entry,
			struct hash_table *table,
			const char *string)
{
  struct demangled_hash_entry *ret = (struct demangled_hash_entry *) entry;
  if (ret == NULL)
    {
      ret = ((struct demangled_hash_entry *)
	     hash_allocate (table, sizeof (struct demangled_hash_entry)));
      if (ret == NULL)
	return NULL;
    }
  ret = ((struct demangled_hash_entry *)
	 hash_newfunc ((struct hash_entry *) ret, table, string));
  ret->mangled = NULL;
  return (struct hash_entry *) ret;
}

static struct demangled_hash_entry *
demangled_hash_lookup (const char *string,
		       boolean create)
{
  return ((struct demangled_hash_entry *)
	  hash_lookup (&demangled_table, string, create, true));
}

/* Stack code.  */

struct symbol_stack_entry
{
  symbol *value;
  struct symbol_stack_entry *next;
};
struct obstack symbol_stack_obstack;
struct symbol_stack_entry *symbol_stack;

struct file_stack_entry
{
  file *value;
  struct file_stack_entry *next;
};
struct obstack file_stack_obstack;
struct file_stack_entry *file_stack;

static void
symbol_push (symbol *p)
{
  struct symbol_stack_entry *ep = (struct symbol_stack_entry *) obstack_alloc
    (&symbol_stack_obstack, sizeof (struct symbol_stack_entry));
  ep->value = p;
  ep->next = symbol_stack;
  symbol_stack = ep;
}

static symbol *
symbol_pop (void)
{
  struct symbol_stack_entry *ep = symbol_stack;
  symbol *p;
  if (ep == NULL)
    return NULL;
  p = ep->value;
  symbol_stack = ep->next;
  obstack_free (&symbol_stack_obstack, ep);
  return p;
}

static void
file_push (file *p)
{
  struct file_stack_entry *ep;

  if (p->tweaking)
    return;

  ep = (struct file_stack_entry *) obstack_alloc
    (&file_stack_obstack, sizeof (struct file_stack_entry));
  ep->value = p;
  ep->next = file_stack;
  file_stack = ep;
  p->tweaking = 1;
}

static file *
file_pop (void)
{
  struct file_stack_entry *ep = file_stack;
  file *p;
  if (ep == NULL)
    return NULL;
  p = ep->value;
  file_stack = ep->next;
  obstack_free (&file_stack_obstack, ep);
  p->tweaking = 0;
  return p;
}

/* Other machinery.  */

static void
tlink_init (void)
{
  char *p;

  hash_table_init (&symbol_table, symbol_hash_newfunc);
  hash_table_init (&file_table, file_hash_newfunc);
  hash_table_init (&demangled_table, demangled_hash_newfunc);
  obstack_begin (&symbol_stack_obstack, 0);
  obstack_begin (&file_stack_obstack, 0);

  p = getenv ("TLINK_VERBOSE");
  if (p)
    tlink_verbose = atoi (p);
  else
    {
      tlink_verbose = 0;
    }
}

static int
tlink_execute (const char *prog, char **argv, char *redir, char *viafile)
{
  char *command, *temp;
  char **p_argv, *str, *s = NULL, filename[256];
  FILE *handle;
  int system_result;
  unsigned int command_size;
#ifdef __riscos__
  pid_t pid;
#endif

  /* Reserve some space for the command line.  */
  command_size = 512;
  command = xmalloc (command_size);

  if (tlink_verbose >= 5)
    printf ("Entered tlink_execute\n");

  if (tlink_verbose >= 4)
    printf ("Opening '%s' for object file storage\n", viafile);

  handle = fopen (viafile, "w");
  if (!handle)
    {
      printf ("ld: failed to open '%s' for writing. Trying without\n",
	      viafile);
      /* Reserve lots more space for the command line.  */
      command = xrealloc (command, 4096);
    }

  if (argv[0])
    {
#ifdef CROSS_COMPILE
      s = stpcpy (command, argv[0]);
#else
      temp = __riscosify (argv[0], 0, 0, filename, sizeof (filename), NULL);
      s = stpcpy (command, filename);
#endif
      if (tlink_verbose >= 5)
	fprintf (stderr, "*%s", argv[0]);
    }
  else
    fprintf (stderr, "[cannot find %s]", prog);

  /* Place the switches on the command line.  */
  for (p_argv = &argv[1]; (str = *p_argv) != (char *)0; p_argv++)
    {
      *s++ = ' ';
      if (*str == '-')
	{
	  /* The -throwback option has the effect of stiffing the
	     template repository system but there is nothing we
	     can do about it.  */
	  s = stpcpy (s, str);
	}
      else
	{
#ifdef CROSS_COMPILE
	  if (s != NULL)
	    s = stpcpy (s, str);
#else
	  temp = __riscosify (str, 0, 0, filename, sizeof (filename), NULL);
	  if (s != NULL)
	    s = stpcpy (s, filename);
#endif
	}

      /* Increase the command line buffer if we're reaching the limit.  */
      if (strlen (command) > command_size - 256)
	{
	  command_size += 512;
	  command = xrealloc (command, command_size);
	}

      if (tlink_verbose >= 5)
	fprintf (stderr, " %s", str);
    }

  /* If we managed to open a viafile then place object files in that.  */
  for (p_argv = (char **)&object_list[0];
       (str = *p_argv) != (char *)0;
       p_argv++)
    {
#ifdef CROSS_COMPILE
      temp = strcpy (filename, str);
#else
      /* Convert all the objects to a RISC OS format for the linker.  */
      temp = __riscosify (str, 0, 0, filename,
			  sizeof (filename), NULL);
#endif
      if (temp != NULL)
	{
	  if (handle)
	    {
	      /* Place objects into a temporary file.  */
	      fprintf (handle, "%s\n", filename);
	    }
	  else
	    {
	      /* We couldn't create a temporary file, so place objects
		 on the command line.  */
	      *s++ = ' ';
	      s = stpcpy (s, filename);

	      /* Increase the command line buffer if we're reaching
		 the limit.  */
	      if (strlen (command) > command_size - 256)
		{
		  command_size += 512;
		  command = xrealloc (command, command_size);
		}
	    }
	}

      if (tlink_verbose >= 5)
	fprintf (stderr, " %s", str);
    }

  if (handle)
    {
#ifdef CROSS_COMPILE
      temp = strcpy (filename, viafile);
#else
      temp = __riscosify (viafile, 0, 0, filename, sizeof (filename), NULL);
#endif
      if (temp != NULL)
	{
	  /* Reference our object file, if we created one.  */
	  s = stpcpy (s, " -via ");
	  s = stpcpy (s, filename);
	  if (tlink_verbose >= 5)
	    fprintf (stderr, " -via %s", viafile);
	  fclose (handle);
	}
    }

#ifdef CROSS_COMPILE
  temp = strcpy (filename, redir);
#else
  temp = __riscosify (redir, 0, 0, filename, sizeof (filename), NULL);
#endif
  if (temp != NULL)
    {
      /* Redirect linker output to another file for further processing.  */
#if 1 /*def CROSS_COMPILE */
      *s++ = ' ';
      *s++ = '>';
      s = stpcpy (s, filename);
#else
      s = stpcpy (s, " { > ");
      s = stpcpy (s, filename);
      *s++ = ' ';
      *s++ = '}';
#endif
      *s++ = '\0';
    }

  if (tlink_verbose >= 5)
    fprintf (stderr, " { > %s }\n", redir);

  /* Flush streams to save us reading garbage that might appear in
     the output buffers.  */
  fflush (stdout);
  fflush (stderr);

  /* If we can't find a program we need, complain and die.  */

  if (argv[0] == 0)
    ld_error ("cannot find `%s'", prog);

  if (tlink_verbose >= 3)
    printf ("Command line to execute: '%s'\n", command);

#ifdef __riscos__
  pid = vfork ();
  if (pid == (pid_t) 0)
    {
      execl ("*", "", command, 0);
      _exit (1);
    }
  else if (pid < (pid_t) 0)
    /* The fork failed.  */
    system_result = -1;
  else
    {
      /* Parent process.  */
      if (waitpid (pid, &system_result, 0) != pid)
	system_result = -1;
    }
#else
  system_result = system (command);
#endif

  free (command);

  return system_result;
}

static char *
frob_extension (char *s, const char *ext)
{
  char *p = (char *) rindex (s, '/');
  if (! p)
    p = s;
  p = (char *) rindex (p, '.');
  if (! p)
    p = s + strlen (s);

  obstack_grow (&temporary_obstack, s, p - s);
  return obstack_copy0 (&temporary_obstack, (void *)ext, strlen (ext));
}

static char *
obstack_fgets (FILE *stream, struct obstack *ob)
{
  int c;
  while ((c = getc (stream)) != EOF && c != '\n')
    obstack_1grow (ob, c);
  if (obstack_object_size (ob) == 0)
    return NULL;
  obstack_1grow (ob, '\0');
  return obstack_finish (ob);
}

static char *
tfgets (FILE *stream)
{
  return obstack_fgets (stream, &temporary_obstack);
}

static char *
pfgets (FILE *stream)
{
  return obstack_fgets (stream, &permanent_obstack);
}

/* Real tlink code.  */

static void
freadsym (FILE *stream, file *f, int chosen)
{
  symbol *sym;

  {
    char *name = tfgets (stream);

    if (tlink_verbose >= 4)
      fprintf (stderr, "freadsym: name = '%s'\n", name);

    sym = symbol_hash_lookup (name, true);
  }

  if (sym->file == NULL)
    {
      symbol_push (sym);
      sym->file = f;
      sym->chosen = chosen;
    }
  else if (chosen)
    {
      if (sym->chosen && sym->file != f)
	{
	  if (sym->chosen == 1)
	    file_push (sym->file);
	  else
	    {
	      file_push (f);
	      f = sym->file;
	      chosen = sym->chosen;
	    }
	}
      sym->file = f;
      sym->chosen = chosen;
    }
}

static void
read_repo_file (file *f)
{
  char c;
  FILE *stream = fopen (f->root.string, "r");

  if (tlink_verbose >= 2)
    fprintf (stderr, "ld: reading %s\n", f->root.string);

  while (fscanf (stream, "%c ", &c) == 1)
    {
      switch (c)
	{
	case 'A':
	  f->args = pfgets (stream);
	  break;
	case 'D':
	  f->dir = pfgets (stream);
	  break;
	case 'M':
	  f->main = pfgets (stream);
	  break;
	case 'P':
	  freadsym (stream, f, 2);
	  break;
	case 'C':
	  freadsym (stream, f, 1);
	  break;
	case 'O':
	  freadsym (stream, f, 0);
	  break;
	}
      obstack_free (&temporary_obstack, temporary_firstobj);
    }
  fclose (stream);
  if (f->args == NULL)
    f->args = getenv ("COLLECT_GCC_OPTIONS");
  if (f->dir == NULL)
    f->dir = ".";
}

static void
maybe_tweak (char *line, file *f)
{
  symbol *sym = symbol_hash_lookup (line + 2, false);

  if ((sym->file == f && sym->tweaking)
      || (sym->file != f && line[0] == 'C'))
    {
      sym->tweaking = 0;
      sym->tweaked = 1;

      if (line[0] == 'O')
	line[0] = 'C';
      else
	line[0] = 'O';
    }
}

static int
recompile_files (void)
{
  file *f;

  while ((f = file_pop ()) != NULL)
    {
      char *line, *command;
      FILE *stream = fopen (f->root.string, "r");
      char *outname = frob_extension ((char *)f->root.string, ".rnw");
      FILE *output = fopen (outname, "w");

      while ((line = tfgets (stream)) != NULL)
	{
	  switch (line[0])
	    {
	    case 'C':
	    case 'O':
	      maybe_tweak (line, f);
	    }
	  fprintf (output, "%s\n", line);
	}
      fclose (stream);
      fclose (output);
      rename (outname, f->root.string);


      if (tlink_verbose)
	fprintf (stderr, "ld: recompiling %s\n", f->main);

      /* Change directory, so we can re-compile the source.  */
      chdir (f->dir);

      obstack_grow (&temporary_obstack, c_file_name, strlen (c_file_name));
      obstack_1grow (&temporary_obstack, ' ');
      obstack_grow (&temporary_obstack, (void *)f->args, strlen (f->args));
      obstack_1grow (&temporary_obstack, ' ');
      command = obstack_copy0 (&temporary_obstack, (void *)f->main,
			       strlen (f->main));

      if (tlink_verbose >= 2)
	fprintf (stderr, "%s\n", command);

      if (system (command) != 0)
	return 0;

      read_repo_file (f);

      obstack_free (&temporary_obstack, temporary_firstobj);
    }
  return 1;
}

static int
read_repo_files (args *object_lst)
{
  for (; object_lst->arg; object_lst++)
    {
      char *p = frob_extension (object_lst->arg, ".rpo");
      file *f;

      /* access() returns false if we have read access.  */
      if (access (p, R_OK))
	continue;

      f = file_hash_lookup (p);

      read_repo_file (f);
    }

  if (file_stack != NULL && ! recompile_files ())
    return 0;

  return (symbol_stack != NULL);
}

static void
demangle_new_symbols (void)
{
  symbol *sym;

  while ((sym = symbol_pop ()) != NULL)
    {
      demangled *dem;
      char *p = cplus_demangle (sym->root.string, DMGL_PARAMS | DMGL_ANSI);

      if (tlink_verbose >= 4)
	fprintf (stderr, "demangle_new_symbols: sym = '%s', p = '%s'\n",
		 sym->root.string, p);

      if (! p)
	continue;

      dem = demangled_hash_lookup (p, true);
      dem->mangled = sym->root.string;
    }
}

static int scan_link_output (FILE *stream, char *line, int linker)
{
  char *symbol_buf;
  int symbol_buf_size;

  if (linker == LINKER_DRLINK)
    {
      /* If we're using drlink, the first line reads:
	 'Error: The following symbols could not be found:'

	 This line must be skipped.  */

      line = tfgets (stream);

      /* If line is null, we have linked successfully.  */
      if (line == 0)
	return 0;
    }

  symbol_buf_size = 256;
  symbol_buf = (char *)xmalloc (symbol_buf_size + 1);

  /* Drlink symbol lists are of the following format:
      <4 spaces>'<missing symbol>' referenced in '<object file>'

     Acorn link v4 symbol lists are of the following format:
      <4 spaces><missing symbol>

     Acord link v5 symbol lists are of the following format:
      ARM Linker:<5 spaces><missing symbol>, referred to from <object file> */

  while ((line = tfgets (stream)) != NULL)
    {
      symbol *sym;
      int end, length;

      length = strlen (line);
      /* Extend the symbol buffer if the line length is longer.  */
      if (length > symbol_buf_size)
	{
	  symbol_buf_size = length;
	  symbol_buf = xrealloc (symbol_buf, symbol_buf_size);
	}

      if (linker == LINKER_DRLINK)
	{
	  char *s = strchr (line, '\'');

	  if (s == NULL)
	    symbol_buf[0] = '\0';
	  else
	    {
	      /* Find end delimiter.  */
	      char *s1 = strchr (s + 1, '\'');

	      if (s1 == NULL)
		symbol_buf[0] = '\0';
	      else
		{
		  strncpy (symbol_buf, s + 1, s1 - s);
		  symbol_buf[s1 - s - 1] = '\0';
		}
	    }
	}
      else if (linker == LINKER_LINK_V4)
	{
	  /* Acorn's linker v4.00.  */
	  sscanf (line, "    %s", symbol_buf);
	  end = strlen (symbol_buf) - 1;
	  symbol_buf[(end < 0) ? 0 : end] = '\0';
	}
      else if (linker == LINKER_LINK_V5)
	{
	  /* Acorn's linker v5.00.  */
	  if (strncmp (line, "ARM Linker:     ", 16) == 0)
	    {
	      char *s, *t;

	      s = line + 16;
	      t = symbol_buf;
	      while (*s && *s != ',')
		*t++ = *s++;
	      *t = '\0';
	    }
	  else
	    {
	      symbol_buf[0] = '\0';
	    }
	}

      if (tlink_verbose >= 4)
	fprintf (stderr, "symbol = '%s'\n", symbol_buf);

      /* Look the symbol up.  */
      sym = symbol_hash_lookup (symbol_buf, false);
      if (!sym)
	{
	  /* This is probably a mangled name.  */
	  demangled *dem = 0;

	  dem = demangled_hash_lookup (symbol_buf, false);
	  if (dem)
	    sym = symbol_hash_lookup (dem->mangled, false);
	}

      if (sym && sym->tweaked)
	{
	  free (symbol_buf);
	  return 0;
	}
      if (sym && !sym->tweaking)
	{
	  if (tlink_verbose >= 2)
	    fprintf (stderr, "ld: tweaking %s in %s\n",
		     sym->root.string, sym->file->root.string);
	  sym->tweaking = 1;
	  file_push (sym->file);
	}

      obstack_free (&temporary_obstack, temporary_firstobj);
    }

  free (symbol_buf);
  return (file_stack != NULL);
}

static int
scan_linker_output (char *fname)
{
  FILE *stream = fopen (fname, "r");
  char *line = tfgets (stream);
  int r;
  int link_type;

  /* Determine which linker produced the output.  */
#ifndef CROSS_COMPILE
  if (strncmp (line, "link:", 5) == 0)
  {
    /* Acorn's link will prefix any errors by 'link:' on the first line.  */
    link_type = LINKER_LINK_V4;
    if (tlink_verbose >= 3)
      fprintf (stderr, "Recognised output from Acorn Link v4\n");
    r = scan_link_output (stream, line, link_type);
  }
  else if (strncmp (line, "Drlink", 6) == 0)
  {
#endif
    /* Drlink always has the linker name on the first line 'Drlink'.  */
    link_type = LINKER_DRLINK;
    if (tlink_verbose >= 3)
      fprintf (stderr, "Recognised output from Drlink\n");
    r = scan_link_output (stream, line, link_type);
#ifndef CROSS_COMPILE
  }
  else if (strncmp (line, "ARM Linker:", 11) == 0)
  {
    link_type = LINKER_LINK_V5;
    if (tlink_verbose >= 3)
      fprintf (stderr, "Recognised output from Acorn Link v5\n");
    r = scan_link_output (stream, line, link_type);
  }
  else
    r = 0;
#endif

  fclose (stream);

  return r;
}


void
do_tlink (const char *linker, char **ld_argv, args *object_lst)
{
  int exit_code = tlink_execute (linker, ld_argv, ldout, ld_viafile);

  if (exit_code)
    {
      int i = 0;

      if (read_repo_files (object_lst))
	while (exit && i++ < MAX_ITERATIONS)
	  {
	    if (tlink_verbose >= 3)
	      dump_file (ldout);
	    demangle_new_symbols ();
	    if (! scan_linker_output (ldout))
	      break;
	    if (! recompile_files ())
	      break;
	    if (tlink_verbose)
	      fprintf (stderr, "ld: relinking\n");
	    exit_code = tlink_execute (linker, ld_argv, ldout, ld_viafile);
	  }
    }

  dump_file (ldout);

  if (exit_code)
    {
      ld_error ("program %s returned exit status %d: %s", linker,
		exit_code, (exit_code == 33) ? strerror (errno) : "");
      exit (exit_code);
    }
}

static int
choose_temp_base (void)
{
  const char *base = getenv ("TMPDIR");
  int len;

  if (base == (char *)0)
    {
#ifdef P_tmpdir
      if (access (P_tmpdir, R_OK | W_OK) == 0)
	base = P_tmpdir;
#endif
      if (base == NULL)
	{
	  if (access ("/usr/tmp", R_OK | W_OK) == 0)
	    base = "/usr/tmp/";
	  else
	    base = "/tmp/";
	}
    }

  len = strlen (base);
  temp_filename = xmalloc (len + sizeof("/ccXXXXXX") + 1);
  strcpy (temp_filename, base);
  if (len > 0 && temp_filename[len-1] != '/')
    temp_filename[len++] = '/';
  strcpy (temp_filename + len, "ccXXXXXX");

  return mkstemp (temp_filename);
}

static void
dump_file (char *name)
{
  FILE *stream = fopen (name, "r");
  int no_demangle = !! getenv ("COLLECT_NO_DEMANGLE");

  if (stream == 0)
    return;
  while (1)
    {
      int c;
      while (c = getc (stream),
	     c != EOF && (isalnum (c) || c == '_' || c == '$' || c == '.'))
	obstack_1grow (&temporary_obstack, c);
      if (obstack_object_size (&temporary_obstack) > 0)
	{
	  char *word, *p, *result;
	  obstack_1grow (&temporary_obstack, '\0');
	  word = obstack_finish (&temporary_obstack);

	  if (*word == '.')
	    ++word, putc ('.', stderr);
	  p = word;

	  if (no_demangle)
	    result = 0;
	  else
	    result = cplus_demangle (p, DMGL_PARAMS | DMGL_ANSI);

	  if (result)
	    {
	      int diff;
	      fputs (result, stderr);

	      diff = strlen (word) - strlen (result);
	      while (diff > 0)
		--diff, putc (' ', stderr);
	      while (diff < 0 && c == ' ')
		++diff, c = getc (stream);

	      free (result);
	    }
	  else
	    fputs (word, stderr);

	  fflush (stderr);
	  obstack_free (&temporary_obstack, temporary_firstobj);
	}
      if (c == EOF)
	break;
      putc (c, stderr);
    }
  fclose (stream);
}

/* ************** Command line processing ********************  */

static void ldversion (int noisy)
{
#ifdef CROSS_COMPILE
  fprintf (stdout, "ld: RISC OS cross-linker front end version %s [%s]\n",
	   LD_VERSION, LD_DATE);
#else
  fprintf (stdout, "ld: RISC OS linker front end version %s [%s]\n",
	   LD_VERSION, LD_DATE);
#endif
  if (noisy >= 1)
    fprintf (stdout, "Including C++ template repository support\n\n");
  else
    fprintf (stdout, "\n");
}

static void out (const char *s)
{
  fprintf (stdout, "%s\n", s);
}

static void ldhelp (void)
{
  ldversion (1);
  out ("LD is used by GCC to convert command line options passed to");
#ifdef CROSS_COMPILE
  out ("GNU LD into something that Drlink can understand.\n");
#else
  out ("GNU LD into something that Drlink or Link can understand.\n");
#endif
  out ("LD also provides support for C++ template instantiation by");
  out ("compiling a C++ source file several times to resolve missing");
  out ("symbol references.\n");

  out ("Syntax:");
#ifdef CROSS_COMPILE
  out ("  ld -o <exec> <obj>.o [<obj>.o ...] [-L<path>] [-l<lib>]");
#else
  out ("  ld -o <exec> o.<obj> [o.<obj> ...] [-L<path>] [-l<lib>]");
#endif

  out ("Where:");
  out ("  -L<path>     <path> is a library search path");
  out ("  -l<lib>      <lib> is a library to link against");
#ifdef CROSS_COMPILE
  out ("  <obj>.o      Object files\n");
#else
  out ("  o.<obj>      Object files\n");
#endif

  out ("The following Drlink linker commands are recognised:");
  out ("  -acornmap, -area[map] <file>, -aif, -aof, -bin, -case");
  out ("  -leave[weak], -map, -m[odule], -no[unused], -output, -qui[et]");
  out ("  -res[can], -throwback, -via <file>, -verbose\n");

#ifndef CROSS_COMPILE
  out ("The following Acorn Link linker (version 4) commands are recognised:");
  out ("  -aif, -aof, -bin, -map, -via <file>, -verbose\n");

  out ("The following Acorn Link linker (version 5) commands are recognised:");
  out ("  -aif, -aof, -bin, -map, -via <file>, -nounused[areas], -verbose");
  out ("\nText within square brackets is optional\n");

  out ("By default, the real linker will be Drlink, and this must be placed");
  out ("in a directory searched by Run$Path. To use an alternative linker");
  out ("(Acorn Link, or one that isn't contained within the Run$Path), ");
  out ("then GCC$Linker should contain the full pathname of the linker e.g.");
  out ("   *Set GCC$Linker \"$.library.link\"");
#endif
}

/* Add string 'name' onto the end of a linked list.  */
static void llist_add (llist **list, const char *name)
{
  llist *y, *z;

  y = (llist *)xmalloc (sizeof (llist));
  y->name = (char *)xmalloc (strlen (name) + 1);
  strcpy (y->name, name);
  y->next = NULL;

  z = *list;

  if (z)
    {
      while (z->next)
	z = z->next;
      z->next = y;
    }
  else
    *list = y;
}

/* Free all elements in a llist.  */
static void llist_free (llist *list)
{
  llist *list1;

  list1 = list;
  while (list1)
    {
      list = list->next;
      free (list1->name);
      free (list1);
      list1 = list;
    }
}

static void
append_arg (args *argv, int *offset, const char *text)
{
  if (tlink_verbose >= 5)
    printf ("append arg: offset = %d, text = %s\n", *offset, text);

  if (*offset >= MAXARGS)
    printf ("append arg: too many arguments (exceeded %d)\n", MAXARGS);
  else
    {
      argv[*offset].arg = (char *)xmalloc (strlen (text) + 1);
      strcpy (argv[*offset].arg, text);
      *offset = *offset + 1;
      argv[*offset].arg = 0;
    }
}

static void
add_library_file (const char *library)
{
  llist *list = libraries;

  if (strcmp (library, "m") == 0 || strcmp (library, "c") == 0 ||
      strcmp (library, "pthread") == 0)
    {
      if (tlink_verbose >= 2)
	printf ("Library lib%s was specified. Ignoring it\n", library);
      return;
    }

  while (list)
    {
      if (!strcmp (library, list->name))
	{
	  if (tlink_verbose >= 4)
	    printf ("Ignoring library %s\n", library);

	  return;
	}
      list = list->next;
    }

  llist_add (&libraries, library);
}

/* Return 1 if library was found and added to object list.
   Return 0 on failure.  */
static int check_and_add_library (const char *file_name)
{
  char converted[256], *temp;

#ifdef CROSS_COMPILE
  temp = strcpy (converted, file_name);
#else
  temp = __riscosify (file_name, 0, 0, converted, sizeof (converted), NULL);
#endif
  if (temp == NULL)
    return 0;

  if (tlink_verbose >= 5)
    printf ("Looking for %s-> %s\n", file_name, converted);

  if (access (converted, R_OK) == 0)
    {
      /* Found the file.  */
      append_arg (object_list, &object_offset, converted);
      return 1;
    }

  return 0;
}

static void parse_library (const char *library)
{
  char file_name[200];
  int pass;

  /* For the first pass, try all the "regular" options,
     for the second last ditch pass, try without the lib prefix */
  for (pass = 0; pass < 2; pass++)
    {
      llist *list = library_path;

      while (list)
        {
          int namelen = strlen (list->name);

          if (pass == 0)
            {
  
              /* Add 'lib' onto the front of each library name. This is what
                'ld' does anyway.  */
              strcpy (file_name, list->name);
              if (list->name[namelen] != ':' && list->name[namelen] != '.'
                  && list->name[namelen] != '/')
                strcat (file_name, "/");
      
#ifdef CROSS_COMPILE
              strcat (file_name, "lib"); 
              strcat (file_name, library); 
              strcat (file_name, ".a"); 
#else
              strcat (file_name, "a/lib"); 
              strcat (file_name, library); 
#endif
              if (check_and_add_library (file_name) == 1)
               return;
       
              /* Couldn't find lib<name>.a so try lib<name>.o */ 
              strcpy (file_name, list->name); 
              if (list->name[namelen] != ':' && list->name[namelen] != '.'
                  && list->name[namelen] != '/')
                strcat (file_name, "/"); 
      
#ifdef CROSS_COMPILE
              strcat (file_name, "lib");
              strcat (file_name, library);
              strcat (file_name, ".o");
#else
              strcat (file_name, "o/lib");
              strcat (file_name, library);
#endif
              if (check_and_add_library (file_name) == 1)
                return;

           }
         else
           {

           /* If we couldn't find the library with lib on the front e.g.
              for the case of UnixLib then try it without.  */
           strcpy (file_name, list->name);
           if (list->name[namelen] != ':' && list->name[namelen] != '.'
               && list->name[namelen] != '/')
             strcat (file_name, "/");

#ifdef CROSS_COMPILE
           strcat (file_name, library);
           strcat (file_name, ".o");
#else
           strcat (file_name, "o/");
           strcat (file_name, library);
#endif
           if (check_and_add_library (file_name) == 1)
             return;
        }
        list = list->next;
      }
    }

  /* Check for precise filename */
  if (check_and_add_library (library) == 1)
    return;

  ld_error ("library file lib%s not found", library);
}

static void parse_libraries (void)
{
  llist *x = libraries;
  while (x)
    {
      parse_library (x->name);
      x = x->next;
    }
}

static void
add_library_search_path (const char *path)
{
  if (tlink_verbose >= 4)
    printf ("adding search path %s\n", path);

  llist_add (&library_path, path);
}


static void add_input_file (const char *fname)
{
#ifndef CROSS_COMPILE
  char tmp[256];
#endif

  if (tlink_verbose >= 4)
    printf ("adding object file %s\n", fname);

  /* Don't add object to the command line, let tlink_execute do that.  */
  /* append_arg (command_line, &command_line_offset, fname); */

#ifdef CROSS_COMPILE
  append_arg (object_list, &object_offset, fname);
#else
  /* For tlink, we need to have all names in Unix format for proper
     processing.  */
  riscos_to_unix (fname, tmp);
  append_arg (object_list, &object_offset, tmp);
#endif
}

static void add_output_file (const char *fname)
{
#if defined(CROSS_COMPILE) && defined(ENABLE_FILETYPE_FF8)
  char tmp[256];
  int len;

  /* If asked for, append a filetype to the RISC OS executable in the
     format of ,xxx.  Network mounts, will automatically remove the
     ,xxx suffix and replace it with proper filetype information.  */
  strcpy (tmp, fname);
  len = strlen (tmp);
  /* If the filetype suffix already exists then don't add it again.  */
  if (len < 4 || (tmp[len - 4] != ',' && len < 252))
    strcat (tmp, ",ff8");
#else
  const char *tmp = fname;
#endif
  if (tlink_verbose >= 4)
    printf ("adding output file %s\n", tmp);

  append_arg (command_line, &command_line_offset, "-o");
  append_arg (command_line, &command_line_offset, tmp);
}

static void add_option_file (const char *option, const char *fname)
{
  if (tlink_verbose >= 4)
    printf ("adding option '%s', file '%s'\n", option, fname);

  append_arg (command_line, &command_line_offset, option);
  append_arg (command_line, &command_line_offset, fname);
}

static void add_option (const char *arg)
{
  if (tlink_verbose >= 4)
    printf ("adding option '%s'\n", arg);

  append_arg (command_line, &command_line_offset, arg);
}

/* Non-zero if we are processing a --defsym from the command line.  */
int parsing_defsym = 0;

static void
parse_args (int argc, char **argv)
{
  /* Starting the short option string with '-' is for programs that
     expect options and other ARGV-elements in any order and that care about
     the ordering of the two.  We describe each non-option ARGV-element
     as if it were the argument of an option with character code 1.  */

  const char *shortopts = "-c:h::L:l:o:Vv";
  int a = 0;
  /* 150 isn't special; it's just an arbitrary non-ASCII char value.  */

#define OPTION_MAP			150
#define OPTION_AIF			151
#define OPTION_CASE			152
#define OPTION_HELP			153
#define OPTION_LEAVEWEAK		154
#define OPTION_NOUNUSED			155
#define OPTION_RESCAN			156
#define OPTION_THROWBACK		157
#define OPTION_VIA			158
#define OPTION_AREAMAP			159
#define OPTION_AOF			160
#define OPTION_BIN			161
#define OPTION_QUIET			162
#define OPTION_MODULE			163

  static struct option longopts[] = {
    {"acornmap", no_argument, NULL, OPTION_MAP},
    {"areamap", required_argument, NULL, OPTION_AREAMAP},
    {"area", required_argument, NULL, OPTION_AREAMAP},
    {"aif", no_argument, NULL, OPTION_AIF},
    {"aof", no_argument, NULL, OPTION_AOF},
    {"bin", no_argument, NULL, OPTION_BIN},
    {"case", no_argument, NULL, 'c'},
    {"help", no_argument, NULL, OPTION_HELP},
    {"leave", no_argument, NULL, OPTION_LEAVEWEAK},
    {"leaveweak", no_argument, NULL, OPTION_LEAVEWEAK},
    {"map", no_argument, NULL, OPTION_MAP},
    {"module", no_argument, NULL, OPTION_MODULE},
    {"no", no_argument, NULL, OPTION_NOUNUSED},
    {"nounused", no_argument, NULL, OPTION_NOUNUSED},
    {"nounusedareas", no_argument, NULL, OPTION_NOUNUSED},
    {"output", no_argument, NULL, 'o'},
    {"qui", no_argument, NULL, OPTION_QUIET},
    {"quiet", no_argument, NULL, OPTION_QUIET},
    {"res", no_argument, NULL, OPTION_RESCAN},
    {"rescan", no_argument, NULL, OPTION_RESCAN},
    {"throwback", no_argument, NULL, OPTION_THROWBACK},
    {"verbose", no_argument, NULL, 'V'},
    {"version", no_argument, NULL, 'v'},
    {"via", required_argument, NULL, OPTION_VIA},
    {NULL, no_argument, NULL, 0}
  };

  while (1)
    {
      /* getopt_long_only is like getopt_long, but '-' as well as '--' can
	 indicate a long option.  */
      int longind;
      int optc;

      optc = getopt_long_only (argc, argv, shortopts, longopts, &longind);

      if (optc == -1)
	break;
      switch (optc)
	{
	default:
	  exit (1);
	case 1:			/* File name.  */
	  add_input_file (optarg);
	  break;
	case OPTION_AIF:
	  add_option ("-aif");
	  break;
	case OPTION_AOF:
	  add_option ("-aof");
	  break;
	case OPTION_AREAMAP:
	  add_option_file ("-areamap", optarg);
	  break;
	case OPTION_BIN:
	  add_option ("-bin");
	  break;
	case 'c':
	  add_option ("-case");
	  break;
	case 'h':
	case OPTION_HELP:
	  ldhelp ();
	  exit (0);
	  break;
	case 'L':
	  /* Library search directory.  */
	  add_library_search_path (optarg);
	  break;
	case 'l':
	  /* Library.  */
	  add_library_file (optarg);
	  break;
	case OPTION_LEAVEWEAK:
	  add_option ("-leaveweak");
	  break;
	case OPTION_MAP:
	  add_option ("-map");
	  break;
	case OPTION_MODULE:
	  add_option ("-module");
	  break;
	case OPTION_NOUNUSED:
	  add_option ("-nounused");
	  break;
	case 'o':
	  add_output_file (optarg);
	  break;
	case OPTION_QUIET:
	  add_option ("-quiet");
	  a |= 1;
	  break;
	case OPTION_RESCAN:
	  add_option ("-rescan");
	  a |= 2;
	  break;
	case OPTION_THROWBACK:
	  add_option ("-throwback");
	  break;
	case 'V':
	  ldversion (1);
	  add_option ("-verbose");
	  break;
	case 'v':
	  ldversion (0);
	  break;
	case OPTION_VIA:
	  add_option_file ("-via", optarg);
	  break;
	}
    }

  if (! (a & 1) && !getenv ("GCC$Linker"))
    add_option ("-quiet");
  if (! (a & 2))
    add_option ("-rescan");
  parse_libraries ();
}
