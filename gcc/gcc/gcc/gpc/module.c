/* Module support for GNU Pascal
   Copyright (C) 1994-2000, Free Software Foundation, Inc.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Pascal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */

/* Authors: Jukka Virtanen <jtv@hut.fi>
 *          Peter Gerwinski <peter@gerwinski.de>
 *          Alexei Volokhov <voh@ispras.ru>
 *          Jan-Jaap van der Heijden <j.j.vanderheijden@student.utwente.nl>
 */

#include <sys/stat.h>

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#include "system.h"

#include "gbe.h"
#include "gpc-defs.h"
#include "objects.h"
#include "util.h"
#include "module.h"
#include "obstack.h"
#include "gpi-hash.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_UNISTD_H
# include <sys/types.h>
# include <unistd.h>
#else
# define R_OK 4
# define W_OK 2
# define X_OK 1
#endif

#if HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <ctype.h>
#include <errno.h>

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef __EMX__
#include <process.h>
#include <stdlib.h>
#include <errno.h>
#endif /* __EMX__ */

#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

/* A list of all modules contained in this source file.
 */
module_t module_list = NULL_MODULE;

/* We are currently compiling this module.
 */
module_t current_module;

/* Files to be linked seen outside of any module.
 */
string_list *pending_link_files = NULL_PTR;

/* A list of all exported names in all modules seen so far.
 *
 * TREE_LIST of exported interfaces we know at the current point
 * of compilation: TREE_VALUE is an IDENTIFIER_NODE of an exported
 * interface name TREE_PURPOSE is a TREE_LIST chain of the names
 * (decls?) exported by this interface.
 */
tree exported_interface_list;

/* A separate list of the names of the interfaces which survives the end
 * of the current module.
 */
static tree internal_exported_interface_list = NULL_TREE;

/* Required module interfaces StandardInput and StandardOutput;
 * StandardError is a GPC extension.
 */
tree standard_interface_input;
tree standard_interface_output;
tree standard_interface_error;

/* Export "all" mark.
 */
tree export_all;

/* Flag for verbose output during AutoMake.
 * @@ Currently always zero!
 */
static int flag_verbose = 0;

/* Flag for debugging the gpi mechanism.
 */
int flag_debug_gpi = 0;

/* Nonzero means to do an AutoMake:
 * 1 = AutoLink (default), 2 = AutoMake, 3 = AutoBuild.
 */
int flag_automake = 1;

/* `gpc' program to use in AutoMake.
 */
char *automake_gpc = NULL;

/* Options to pass to child gpc processes in AutoMake.
 */
char *automake_gpc_options = NULL;

/* Name of a file to store information about targets in.
 */
char *automake_temp_filename = NULL;

/* Search path for GPI and unit source files.
 */
char *unit_path = NULL;

/* Search path for object and non-Pascal source files.
 */
char *object_path = NULL;

/* Holds the name (an LEX_ID node) of the interface
 * currently being loaded from a GPI file, or NULL_TREE if we
 * are not loading a GPI file.
 */
tree we_are_loading_a_gpi_file = NULL_TREE;

/* A table holding all DECL nodes imported from all GPI files
 * together with their UIDs, so duplicates can be identified.
 */
static struct interface_table_t
{
  tree interface_name, module_name;
  int entries;
  tree *entry;
  struct interface_table_t *next;
}
*interface_table = NULL_PTR;

/* Name of the GPI file being loaded (for error messages).
 */
static char* current_gpi_file_name = NULL_PTR;



/* Declare local utility functions with prototypes.
 */

static void gpi_error PROTO ((void));
static void store_string PROTO ((FILE *, const char *));
static char *load_string PROTO ((MEMFILE *));
static void store_flags PROTO ((tree, FILE *));
static void load_flags PROTO ((tree, MEMFILE *));
static void store_type_flags PROTO ((tree, FILE *));
static void load_type_flags PROTO ((tree, MEMFILE *));
static void store_decl_flags PROTO ((tree, FILE *));
static void load_decl_flags PROTO ((tree, MEMFILE *));
static struct interface_table_t *new_interface_table PROTO ((tree, tree, int));
static void resize_itab_entries PROTO ((struct interface_table_t *, int));
static tree record_interface_specifications PROTO ((tree, int));
static int module_must_be_recompiled PROTO ((tree, char *, char *, tree));
static char* locate_interface_source PROTO ((char *, char *, char *));
static MEMFILE *gpi_open PROTO ((tree, char *, char *, int));
static module_t find_module PROTO ((tree, int));



/* Scan the program/module parameter list for entries of FILE_TYPE.
 * If this is at top level, and they are variables of file type,
 * flag the files as external.
 *
 * Since the order of declarations are relaxed, this is checked
 * before every routine.
 *
 * All nodes already handled are marked.
 */
void
associate_external_objects (external_name_list)
     tree external_name_list;
{
  tree link;

  if (external_name_list && top_level_p (current_module->main_program))
    for (link = external_name_list;
         link;
         link = TREE_CHAIN (link))

      {
        tree id = TREE_VALUE (link);

        if (id == identifier_output)
          current_module->output_file_node = global_output_file_node;
        else if (id == identifier_input)
          current_module->input_file_node = global_input_file_node;
        else if (id == identifier_stderr)
          current_module->error_file_node = global_error_file_node;
        else if (! TREE_PURPOSE (link))
          {
            tree name = lookup_name (id);

            if (name
                && TREE_CODE (name) == VAR_DECL
                && TREE_CODE (TREE_TYPE (name)) == FILE_TYPE)
              {
                PASCAL_EXTERNAL_OBJECT (name) = 1;
                TREE_PURPOSE (link) = error_mark_node;
              }
          }
      }
}

/* Check if all the names in program/module param list have
 * been declared.
 *
 * If not, give a warning.
 */
void
check_external_objects (idlist)
     tree idlist;
{
  char *what  = current_module->main_program ? "program" : "module";

  for (; idlist; idlist = TREE_CHAIN (idlist))
    {
      tree id = TREE_VALUE (idlist);

      if (id != identifier_output
          && id != identifier_input
          && id != identifier_stderr)
        {
          tree name = lookup_name (id);

          if (name == NULL_TREE)
            warning ("identifier `%s' in %s heading is undefined",
                     IDENTIFIER_POINTER (id), what);
          else if (TREE_CODE (name) != VAR_DECL
                   || TREE_CODE (TREE_TYPE (name)) != FILE_TYPE)
            warning ("identifier `%s' in %s heading is not a variable of file type",
                     IDENTIFIER_POINTER (id), what);
        }
    }
}

/* Possibly add the `static' qualifier.
 * This makes unexported stuff invisible from outside the module.
 */
tree
maybe_make_static (qualifiers)
     tree qualifiers;
{
  int make_static = 1;

  /* @@@@ Need to verify if the function was declared with a
   *      FORWARD directive.
   */
  if (qualifiers)
    {
      tree scan;
      for (scan = qualifiers; scan; scan = TREE_CHAIN (scan))
        if (TREE_VALUE (scan) == extern_id)
          {
            make_static = 0;
            break;
          }
    }

  if (make_static)
    qualifiers = chainon (qualifiers, build_tree_list (NULL_TREE, static_id));

  return qualifiers;
}

/* Locate a module by its NAME.
 * If CREATE is nonzero, create a new module if an old one is not found.
 */
module_t
find_module (name, create)
     tree name;
     int create;
{
  module_t curr;

  for (curr = module_list; curr; curr = curr->next)
    {
      if (curr->name == name)
        return curr;
    }

  if (create)
    {
      curr = (module_t) obstack_alloc (&permanent_obstack,
                                       sizeof (struct module));
      /* Initialize */
      memset ((void *) curr, 0, sizeof (struct module));
      curr->name = name;
      curr->next = module_list;
      if (pending_link_files)
        {
          curr->link_files = pending_link_files;
          pending_link_files = NULL_PTR;
        }
      module_list = curr;
    }
  return curr;
}

/* Allocate and initialize a new structure for a new module|unit|program
 * named ID (an IDENTIFIER_NODE).  MAIN_PROGRAM is nonzero if this module
 * is the main program.
 */
void
initialize_module (id, main_program)
     tree id;
     int main_program;
{
  current_module = find_module (id, 1);
  current_module->main_program = main_program;
  if (current_module->main_program)
    {
      string_list *link_file;
      for (link_file = current_module->link_files; link_file;
           link_file = link_file->next)
        add_to_automake_temp_file (link_file->string);
    }
}

/* Destroy the info about all identifiers in MOD which will
 * be released with the next poplevel().
 */
void
finalize_module (mod)
    struct module *mod;
{
  mod->imports = NULL_TREE;
  mod->exports = NULL_TREE;
  /* mod->pending_decls = NULL_TREE; */
  mod->autoexport = NULL_TREE;
  mod->implementation = 0;
  mod->interface = 0;
  mod->output_file_node = NULL_TREE;
  mod->input_file_node = NULL_TREE;
  exported_interface_list = NULL_TREE;
  while (interface_table)
    {
      struct interface_table_t *next_interface = interface_table->next;
      free (interface_table);
      interface_table = next_interface;
    }
  interface_table = NULL_PTR;
}

#if 0

/* Perform the interface declarations of the current module.
 */
void
handle_pending_decls ()
{
  if (current_module->pending_decls)
    {
      tree scan;
      for (scan = current_module->pending_decls;
           scan;
           scan = TREE_CHAIN (scan))
        {
          if (TREE_PURPOSE (scan) == void_type_node)
            grok_directive (TREE_VALUE (TREE_VALUE (TREE_VALUE (scan))),
                            TREE_PURPOSE (TREE_VALUE (TREE_VALUE (scan))),
                            TREE_PURPOSE (TREE_VALUE (scan)),
                            0);
          else if (TREE_PURPOSE (scan) == NULL_TREE)
            declare_vars (TREE_VALUE (TREE_VALUE (scan)),               /* names */
                          TREE_PURPOSE (TREE_VALUE (scan)),             /* type */
                          TREE_PURPOSE (TREE_CHAIN (TREE_VALUE (scan))),/* init */
                          TREE_VALUE (TREE_CHAIN (TREE_VALUE (scan))),  /* qual */
                          0);
          else
            abort ();
        }
      current_module->pending_decls = NULL_TREE;
    }
}

#endif 

/* Open a new interface NAME for the current module.  EXPORT_LIST
 * contains the names that should be exported by this interface.
 * In a unit, this is called exactly once with EXPORT_LIST having
 * the special value export_all.
 *
 * Later, after the interface lists have been filled, they will be
 * written to GPI files by create_gpi_files() below.
 *
 * NAME is an IDENTIFIER_NODE of the exported interface name,
 * or a TREE_LIST if exporting a range (TREE_PURPOSE..TREE_VALUE).
 * EXPORT_LIST:
 *   TREE_LIST
 *       TREE_PURPOSE: Export renaming (new name) or NULL_TREE.
 *       TREE_VALUE: IDENTIFIER_NODE of the exported name
 */
void
export_interface (name, export_list)
     tree name;
     tree export_list;
{
  tree exported;
  tree exported_names = NULL_TREE;
  tree nscan, new_export_list;

  for (exported = exported_interface_list;
       exported; exported = TREE_CHAIN (exported))
    if (TREE_VALUE (exported) == name)
      {
        error ("interface `%s' has already been exported",
               IDENTIFIER_POINTER (name));
        return;
      }

  if (export_list == export_all)
    exported_names = build_tree_list (NULL_TREE, NULL_TREE);
  else
    for (nscan = export_list; nscan; nscan = TREE_CHAIN (nscan))
      {
        tree value = build_tree_list (TREE_PURPOSE (nscan), TREE_VALUE (nscan));
        exported_names = chainon (exported_names, value);
      }

  current_module->exports
    = chainon (current_module->exports,
               build_tree_list (exported_names, name));

  new_export_list = build_tree_list (exported_names, name);
  exported_interface_list = chainon (exported_interface_list, new_export_list);
  internal_exported_interface_list = chainon (internal_exported_interface_list,
                                              build_tree_list (NULL_TREE, name));
  if (export_list == export_all)
    current_module->autoexport = chainon (current_module->autoexport,
                                          build_tree_list (NULL_TREE,
                                                           new_export_list));
}

/* Import module/unit interfaces specified on the command line
 * via `--uses=...'.
 */
void
do_extra_import ()
{
  if (extra_imports)
    {
      char *p = extra_imports;
      while (*p)
        {
          char *buffer = xmalloc (strlen (p) + 1);
          char *q = buffer;
          tree interface_name, file_name;
          while (*p &&
                 ((*p >= 'A' && *p <= 'Z')
                   || (*p >= 'a' && *p <= 'z')
                   || (*p >= '0' && *p <= '9')
                   || (*p == '_')))
            {
              if (q == buffer)
                *q = toupper (*p);
              else
                *q = tolower (*p);
              p++;
              q++;
            }
          *q = 0;
          interface_name = get_identifier (buffer);
          if (*p == '(')
            {
              q = buffer;
              p++;
              while (*p && *p != ')')
                {
                  *q = *p;
                  p++;
                  q++;
                }
              *q = 0;
              if (*p == ')')
                p++;
              else
                warning ("missing `)' in `--uses' parameter");
              file_name = build_string (q - buffer + 1, buffer);
            }
          else
            file_name = NULL_TREE;

          import_interface (interface_name, NULL_TREE, 0, file_name);
          while (*p
                 && !((*p >= 'A' && *p <= 'Z')
                     || (*p >= 'a' && *p <= 'z')
                     || (*p >= '0' && *p <= '9')
                     || (*p == '_')))
            {
              if (*p != ',')
                warning ("missing `,' in `--uses' parameter");
              p++;
            }
          free (buffer);
        }
    }
}

int
name_exported_p (name)
     tree name;
{
  tree scan;
  for (scan = current_module->exports; scan; scan = TREE_CHAIN (scan))
    {
      tree id_chain;

      for (id_chain = TREE_PURPOSE (scan); id_chain;
           id_chain = TREE_CHAIN (id_chain))
        {
          /* Exported ranges are not taken into account here
           * because name_exported_p() is never called for them.
           */
          if (name == TREE_VALUE (id_chain))
            return 1;
        }
    }
  return 0;
}

/* Export names of a Borland Pascal Unit.
 * This also handles "export foo = all" clauses
 * and perhaps, one day, PXSC modules.
 */
void
handle_autoexport (name)
     tree name;
{
  tree e;
  if (! we_are_loading_a_gpi_file)
    /* @@@@ Besser nur 1 Autoexport-Interface */
    for (e = current_module->autoexport; e; e = TREE_CHAIN (e))
      {
        tree nscan, p = TREE_PURPOSE (TREE_VALUE (e));
        for (nscan = p; nscan; nscan = TREE_CHAIN (nscan))
          {
            /* @@@@ Besser an den Namen ein Flag drankleben, ob er
             * bereits autoexportiert wurde.
             */
            if (TREE_VALUE (nscan) == name)
              break;
          }
        if (!nscan)
          /* @@@@ überflüssig */
          p = chainon (p, module_export_clause (name, NULL_TREE, 0));
      }
}



/*  The AutoMake facility
 *
 * When invoked with an `--automake' or `--autobuild' option,
 * GPC can do an implicit `make' for preprocessing, compiling,
 * and linking.  A Makefile is not needed; everything is
 * extracted from the Pascal source.
 *
 * Read the `AutoMake' section of the Texinfo documentation
 * for more information about this.
 *
 * This will be replaced by an external utility `gp' with the
 * next release.
 *
 * - PG
 */

/* Print an error message
 */
char *
my_strerror (e)
     int e;
{
#ifdef HAVE_STRERROR

  return strerror (e);

#else

  static char buffer[30];
  if (!e)
    return "cannot access";

  if (e > 0 && e < sys_nerr)
    return sys_errlist[e];

  sprintf (buffer, "Unknown error %d", e);
  return buffer;
#endif
}

/* JJ 970809: rewritten to utilize standard pexecute() and pwait() calls
 *
 * For the AutoMake facility we need to execute gpc as a child
 * process. execute() acts as an interface to the underlying
 * pexecute() and pwait() functions borrowed from GCC or libiberty.
 *
 * Return 1 on success, 0 otherwise.
 */
static int
execute (what, args)
     char *what;
     char *args;
{
  char *s;
  int pid, wait_status;
  int i;
  char **argv;
  char *errmsg_fmt, *errmsg_arg;
  char *temp_base = choose_temp_base ();

  /* If -v, print what we are about to do, and maybe query.  */
  if (flag_verbose)
    {
      fprintf (stderr, "%s %s\n", what, args);
#ifdef DEBUG
      fprintf (stderr, "\nGo ahead? (y or n) ");
      fflush (stderr);
      i = getchar ();
      if (i != '\n')
        while (getchar () != '\n') ;
      if (i != 'y' && i != 'Y')
        return 0;
#endif /* DEBUG */
    }

  /* Count the args */
  i = 0;
  for (s = args; *s; s++)
    if (*s == ' ')
      i++;
  i++;
  argv = alloca (sizeof (char *) * (i + 3));
  i = 0;
  argv[i++] = what;
  s = args;
  while (1)
    {
      argv[i++] = s;
      while (*s != ' ' && *s != 0)
        s++;
      if (*s == 0)
        break;
      *s++ = 0;
    }
  argv[i++] = 0;

  pid = pexecute (argv[0], (char * const *) argv, progname,
                  temp_base, &errmsg_fmt, &errmsg_arg,
                  PEXECUTE_FIRST | PEXECUTE_LAST | PEXECUTE_SEARCH);

  if (pid == -1)
    {
      int errno_val = errno;
      fprintf (stderr, "%s: ", progname);
      fprintf (stderr, errmsg_fmt, errmsg_arg);
      fprintf (stderr, ": %s\n", my_strerror (errno_val));
      exit (1);
    }

  pid = pwait (pid, &wait_status, 0);
  if (pid == -1)
    {
      fprintf (stderr, "%s: wait: %s\n", progname, my_strerror (errno));
      exit (1);
    }
  else if (WIFSIGNALED (wait_status))
    {
      fprintf (stderr, "%s: subprocess got fatal signal %d\n",
               progname, WTERMSIG (wait_status));
      exit (1);
    }
  else if (WIFEXITED (wait_status))
    {
      if (WEXITSTATUS (wait_status) != 0)
        {
          fprintf (stderr, "%s: %s exited with status %d\n",
                   progname, what, WEXITSTATUS (wait_status));
          exit (WEXITSTATUS (wait_status));
        }
      return 1;
    }
  abort ();
}

/* Locate the file FILENAME in the directories SEARCHPATH.
 * Return a newly allocated char* holding the result, or NULL
 * if the file is not accessible.
 */
char*
locate_file (filename, searchpath)
     char *filename, *searchpath;
{
  char *p = searchpath, *q;
  int l_filename;
  if (! filename)
    return NULL;
  if (access (filename, R_OK) != -1)
    return save_string (filename);
  if (! p)
    return NULL;
  l_filename = strlen (filename);
  q = filename + l_filename - 1;
  while (q >= filename && *q != '/' && *q != DIR_SEPARATOR)
    q--;
  if (q >= filename)
    {
      filename = q + 1;
      l_filename = strlen (filename);
    }
  while (*p)
    {
      char *pathname, *new_filename;
      int l_pathname;
      pathname = p;
      q = pathname;
      while (*q && *q != PATH_SEPARATOR)
        q++;
      if (*q)
        p = q + 1;
      else
        p = q;
      if (q > pathname && (q[-1] == DIR_SEPARATOR || q[-1] == '/'))
        q--;
      l_pathname = q - pathname;
      new_filename = xmalloc (l_pathname + 1 + l_filename + 1);
      strncpy (new_filename, pathname, l_pathname);
      new_filename [l_pathname] = DIR_SEPARATOR;
      strcpy (new_filename + l_pathname + 1, filename);
      if (access (new_filename, R_OK) != -1)
        return new_filename;
      else
        free (new_filename);
    }
  return NULL;
}

/* Append P to the string containing the AutoMake GPC options.
 */
void
add_automake_gpc_options (p)
     char *p;
{
  if (! automake_gpc_options)
    automake_gpc_options = save_string (p);
  else
    {
      char *q = concat (automake_gpc_options, " ", p, NULL_PTR);
      free (automake_gpc_options);
      automake_gpc_options = q;
    }
}

/* Add a line to the AutoMake temporary file.
 * The contents of that file will be passed to the linker
 * and to recursive calls of the compiler.
 */
void
add_to_automake_temp_file (line)
     char *line;
{
  if (!automake_temp_filename)
    abort ();
  if (line)
    {
      FILE *automake_temp_file = fopen (automake_temp_filename, "at");
      if (!automake_temp_file)
        fatal ("cannot append to automake temp file `%s'", automake_temp_filename);
      fprintf (automake_temp_file, "%s\n", line);
      fclose (automake_temp_file);
    }
}

/* Remember a FILENAME to be linked.
 * This information will be written into the GPI file.
 * From there, it will be read and written to the AutoMake temp file.
 */
void
add_to_link_file_list (filename)
     char *filename;
{
  if (current_module && current_module->main_program)
    add_to_automake_temp_file (filename);
  else
    {
      string_list *link_file;
      string_list **list;

      if (current_module)
        list = &current_module->link_files;
      else
        list = &pending_link_files;

      /* Check whether FILENAME already is in the list.
       */
      for (link_file = *list; link_file; link_file = link_file->next)
        if (strcmp (link_file->string, filename) == 0)
          return;

      link_file = (string_list *) xmalloc (sizeof (string_list));
      link_file->string = save_string (filename);
      link_file->next = NULL_PTR;
      while (*list)
        list = &((*list)->next);
      *list = link_file;
    }
}

/* Store the name of the executable to be produced in
 * the AutoMake temporary file.
 */
void
store_executable_name ()
{
  if (executable_file_name)
    {
      /* Store the name of the program in the automake temp
       * file, so the executable can be named after it.
       */
      char *name;
      if (*executable_file_name)
        ; /* accept it */
      else if (main_input_filename)
        {
          char *p;
          executable_file_name = save_string (main_input_filename);
          p = executable_file_name + strlen (executable_file_name) - 1;
          while (p > executable_file_name && *p != '.')
            p--;
          if (*p == '.')
            *p = 0;
        }
      else  /* Should not happen */
#ifdef HAVE_EXECUTABLE_SUFFIX
        executable_file_name = "a";
#else
        executable_file_name = "a.out";
#endif

      if (executable_path)
        {
          char *p = executable_file_name + strlen (executable_file_name);
          do
            p--;
          while (p >= executable_file_name && *p != '/' && *p != DIR_SEPARATOR);
          p++;
          name = concat ("-o ", executable_path, p,
#ifdef HAVE_EXECUTABLE_SUFFIX
                         EXECUTABLE_SUFFIX,
#endif
                         NULL_PTR);
        }
      else
        name = concat ("-o ", executable_file_name,
#ifdef HAVE_EXECUTABLE_SUFFIX
                       EXECUTABLE_SUFFIX,
#endif
                       NULL_PTR);
      add_to_automake_temp_file (name);
      free (name);
    }
  else if (executable_path)
    {
#ifdef HAVE_EXECUTABLE_SUFFIX
      char *name = concat ("-o ", executable_path, "a", EXECUTABLE_SUFFIX, NULL_PTR);
#else
      char *name = concat ("-o ", executable_path, "a.out", NULL_PTR);
#endif
      add_to_automake_temp_file (name);
      free (name);
    }
}

/* Add common flags to the AutoMake command line CMD_LINE.
 * PASCAL_SOURCE is nonzero if GPC-specific options should be
 * passed, 0 if only general GCC options are allowed.
 * Return nonzero if there actually are such flags.
 * This function is shared by module_must_be_compiled
 * and compile_module.
 */
static int
add_automake_switches (cmd_line, pascal_source)
     struct obstack *cmd_line;
     int pascal_source;
{
  int flag = 0;

  if (pascal_source)
    {
      if (flag)
        obstack_1grow (cmd_line, ' ');

      if (flag_automake == 1)
        obstack_grow (cmd_line, "--autolink", 10);
      else if (flag_automake == 3)
        obstack_grow (cmd_line, "--autobuild", 11);
      else
        obstack_grow (cmd_line, "--automake", 10);

      if (automake_gpc_options)
        {
          obstack_1grow (cmd_line, ' ');
          obstack_grow (cmd_line, automake_gpc_options,
                        strlen (automake_gpc_options));
        }
      if (automake_temp_filename)
        {
          obstack_grow (cmd_line, " --amtmpfile=", 13);
          obstack_grow (cmd_line, automake_temp_filename,
                        strlen (automake_temp_filename));
        }
      flag = 1;
    }
  else if (automake_gpc_options)
    {
      /* Filter out Pascal-specific options when compiling non-Pascal
       * source.  If the `p' subdirectory was present when `cc1' was
       * compiled, this is not necessary, because then `cc1' will know
       * about those options and ignore them.  But we cannot rely on
       * this.
       */
      char *p = automake_gpc_options, *q;

      while (*p)
        {
          while (*p == ' ' || *p == '\t')
            p++;

          /* Find the end of this option.
           *
           * We do not need to care about options containing spaces
           * because they won't survive until here anyway.
           */
          q = p;
          while (*q && *q != ' ' && *q != '\t')
            q++;

          if (*p == '-' && (p[1] == '-' || p[1] == 'f'))
            {
              /* This is a long option.  Filter out GPC specifica.
               */
              int j;
              char *option;

              p[1] = 'f';
              j = 0;
              while ((option = gpc_options[j].name) != NULL_PTR)
                {
                  if (! strncmp (p, option, strlen (option)))
                    break;
                  j++;
                }
              if (option)
                {
                  /* This is a GPC-specific option.  Throw it away.
                   */
                }
              else
               {
                  /* This is a general long GCC option.  Pass it.
                   */
                  if (flag)
                    obstack_1grow (cmd_line, ' ');
                  obstack_grow (cmd_line, p, q - p);
                  flag = 1;
                }
            }
          else if (strncmp (p, "-dY", strlen ("-dY")) != 0)
            {
              /* This is a short option other than `-dY'.  Pass it.
               */
              if (flag)
                obstack_1grow (cmd_line, ' ');
              obstack_grow (cmd_line, p, q - p);
              flag = 1;
            }
          p = q;
        }
    }

  return flag;
}

/* Check whether a module must be recompiled (for AutoMake).
 */
int
module_must_be_recompiled (interface_name, gpi_filename, module_filename, import_list)
     tree interface_name;
     char *gpi_filename;
     char *module_filename;
     tree import_list;
{
  char *gpc_name, *gpc_args, *dep_filename, *p;
  tree interface;
  FILE *dep_file, *automake_temp_file;
  struct stat gpi_status, source_status, other_gpi_status;
  struct obstack cmd_line;
  char source_filename [2048], *object_filename = NULL;

  /* No source found --> cannot recompile.
   */
  p = module_filename;
  module_filename = locate_file (p, unit_path);
  if (! module_filename)
    module_filename = locate_file (p, unit_destination_path);
  if (! module_filename)
    {
      char *q = p + strlen (p) - 1;
      while (q >= p && *q != '.')
        q--;
      if (*q == '.')
        {
          /* Tell the linker about the object file.
           */
          char *object_filename = save_string (p);
          q = object_filename + (q - p);
          q[1] = 'o';
          q[2] = 0;
          p = locate_file (object_filename, unit_path);
          if (! p)
            p = locate_file (object_filename, unit_destination_path);
          if (! p)
            p = locate_file (object_filename, object_path);
          if (! p)
            p = locate_file (object_filename, object_destination_path);
          if (p)
            {
              add_to_link_file_list (p);
              free (p);
            }
          else if (flag_automake > 0 && /*@@ linking*/0)
            error ("object file `%s' not found", object_filename);
          free (object_filename);
        }
      return 0;
    }

  /* Module is in this file --> cannot recompile.
   */
  for (interface = internal_exported_interface_list;
       interface; interface = TREE_CHAIN (interface))
    {
      if (TREE_VALUE (interface) == interface_name)
        return 0;
    }

  /* Module is being compiled --> avoid a cycle.
   * This also avoids compiling one source file twice with `--autobuild'.
   */
  if (!automake_temp_filename)
    abort ();
  automake_temp_file = fopen (automake_temp_filename, "rt");
  if (!automake_temp_file)
    fatal ("cannot read automake temp file `%s'", automake_temp_filename);
  while (! feof (automake_temp_file))
    {
      char s[2048];
      if (fgets (s, 2048, automake_temp_file)
          && strncmp (s, "#compiling: ", 12) == 0)
        {
          int l = strlen (s) - 1;
          if (s[l] == '\n')
            s[l] = 0;
          if (strcmp (s + 12, module_filename) == 0)
            {
              free (module_filename);
              return 0;
            }
        }
    }
  fclose (automake_temp_file);

  /* `--autobuild' given --> recompile.
   */
  if (flag_automake > 2)
    {
      free (module_filename);
      return 1;
    }

  /* GPI file older than source --> recompile.
   */
  stat (gpi_filename, &gpi_status);
  stat (module_filename, &source_status);

  if (gpi_status.st_mtime < source_status.st_mtime
      && flag_automake > 1)  /* not with `--autolink' */
    {
      free (module_filename);
      return 1;
    }

  /* GPI file older than imported GPI files --> recompile.
   */
  for (interface = import_list; interface; interface = TREE_CHAIN (interface))
    {
      char *other_gpi_name = (char *) xmalloc (strlen (IDENTIFIER_POINTER (TREE_VALUE (interface))) + 5), *p, *q = other_gpi_name;
      p = IDENTIFIER_POINTER (TREE_VALUE (interface));
      if (p)
        while (*p)
          *q++ = tolower (*p++);
      strcpy (q, ".gpi");
      p = locate_file (other_gpi_name, unit_path);
      if (!p)
        p = locate_file (other_gpi_name, unit_destination_path);
      free (other_gpi_name);
      /* One needed GPI does not exist.  Let a recursive AutoMake generate it.
       * @@@ Do we need to go deeper?
       */
      if (!p && flag_automake > 1)  /* not with `--autolink' */
        {
          free (module_filename);
          return 1;
        }

      if (p)
        {
          stat (p, &other_gpi_status);
          if (gpi_status.st_mtime < other_gpi_status.st_mtime
              && flag_automake > 1)  /* not with `--autolink' */
            {
              free (module_filename);
              free (p);
              return 1;
            }
          free (p);
        }
    }

  /* Run the preprocessor with -M for creating a dependency file.
   *
   * @@@ Should do this when compiling the module and store the
   * information in the GPI file.  That would be faster.
   */
  dep_filename = choose_temp_base ();

  /* Touch dep_filename, so it won't be chosen as the name for another temp file */
  close (open (dep_filename, O_WRONLY | O_CREAT, 0666));

  /* Build the AutoMake command line.
   */
  if (automake_gpc)
    gpc_name = automake_gpc;
  else
    gpc_name = "gpc";

  obstack_init (&cmd_line);
  add_automake_switches (&cmd_line, 1);

  obstack_grow (&cmd_line, " -M ", 4);
  obstack_grow (&cmd_line, module_filename, strlen (module_filename));
  obstack_grow (&cmd_line, " -o ", 4);
  gpc_args = obstack_copy0 (&cmd_line, dep_filename,
                            strlen (dep_filename));

  if (flag_verbose)  /* i.e. gpc was invoked with `--verbose' */
    fprintf (stderr, "GPC AutoMake: ");

  /* Run the command.  */
  execute (gpc_name, gpc_args);

  if (flag_verbose)
    fprintf (stderr, "GPC AutoMake: done\n");

  obstack_free (&cmd_line, gpc_args);

  /* Parse the `.d' file and check if the object file or the gpi file
   * is older than one of the source files.  If yes --> recompile.
   */
  dep_file = fopen (dep_filename, "rt");
  if (!dep_file)
    fatal ("cannot read dependency file `%s'", dep_filename);

  while (! feof (dep_file))
    {
      if (fscanf (dep_file, "%s", source_filename) > 0 && strcmp (source_filename, "\\") != 0)
        {
          p = source_filename + strlen (source_filename) - 1;
          if (*p == ':')
            {
              /* This is the name of the object file.
               * If it doesn't exist, we must compile.
               * If it is older than the gpi file,
               * update the time.
               */
              if (object_filename)
                free (object_filename);
              *p = 0;
              object_filename = locate_file (source_filename, unit_path);
              if (! object_filename)
                object_filename = locate_file (source_filename, unit_destination_path);
              if (! object_filename)
                object_filename = locate_file (source_filename, object_path);
              if (! object_filename)
                object_filename = locate_file (source_filename, object_destination_path);
              if (! object_filename)
                {
                  if (flag_automake > 1)  /* not with `--autolink' */
                    {
                      fclose (dep_file);
                      unlink (dep_filename);
                      free (dep_filename);
                      free (module_filename);
                      return 1;
                    }
                }
              else
                {
                  stat (object_filename, &source_status);
                  if (gpi_status.st_mtime > source_status.st_mtime)
                    gpi_status.st_mtime = source_status.st_mtime;
                }
            }
          else if (flag_automake > 1)  /* not with `--autolink' */
            {
              /* This is the name of a source file.
               * If it is younger than the gpi file,
               * recompile.
               */
              stat (source_filename, &source_status);
              if (gpi_status.st_mtime < source_status.st_mtime)
                {
                  fclose (dep_file);
                  unlink (dep_filename);
                  free (dep_filename);
                  free (module_filename);
                  if (object_filename)
                    free (object_filename);
                  return 1;
                }
            }
        }
    }
  fclose (dep_file);
  unlink (dep_filename);

  /* Recompilation not needed.
   * Tell the linker about the object file.
   */
  if (object_filename)
    {
      add_to_link_file_list (object_filename);
      free (object_filename);
    }

  /* All possible checks passed --> no recompilation.
   */
  free (dep_filename);
  free (module_filename);
  return 0;
}

/* Compile a module during an AutoMake.
 * Return 0 on success, nonzero otherwise.
 */
int
compile_module (filename, destination_path)
     char *filename, *destination_path;
{
  char *gpc_name, *gpc_args;
  int result, pascal_source;
  char *plain_filename, *object_filename, *p;
  struct obstack cmd_line;

  p = locate_file (filename, unit_path);
  if (! p)
    p = locate_file (filename, unit_destination_path);
  if (! p)
    return -1;
  filename = p;

  /* Document what we are doing in the automake temp file.
   */
  p = concat ("#compiling: ", filename, NULL_PTR);
  add_to_automake_temp_file (p);
  free (p);

  /* Build the AutoMake command line.
   */
  if (automake_gpc)
    gpc_name = automake_gpc;
  else
    gpc_name = "gpc";

  obstack_init (&cmd_line);

  /* Pass automake GPC options only if Pascal source.
   */
  p = filename + strlen (filename) - 1;
  while (p > filename && *p != '.')
    p--;
  p++;
  pascal_source = strcmp (p, "pas") == 0 || strcmp (p, "p") == 0
                  || strcmp (p, "pp") == 0 || strcmp (p, "dpr") == 0;

  /* Only add a space if we actually add the AutoMake command line switches.
   */
  if (add_automake_switches (&cmd_line, pascal_source) == 1)
    obstack_1grow (&cmd_line, ' ');

  obstack_grow (&cmd_line, "-c ", 3);

  /* Create the object file in a special directory if one was specified.
   */
  if (destination_path)
    {
      char *q;
      int l = strlen (destination_path);

      /* p still points to the file name extension.
       */
      p--;
      q = p;
      while (q >= filename && *q != '/' && *q != DIR_SEPARATOR)
        q--;
      q++;
      if (p > q)
        {
          obstack_grow (&cmd_line, "-o ", 3);
          obstack_grow (&cmd_line, destination_path, l);
          obstack_grow (&cmd_line, q, p - q);
          obstack_grow (&cmd_line, ".o ", 3);
          if (pascal_source)
            {
              obstack_grow (&cmd_line, "--gpi-destination-path=", 23);
              obstack_grow (&cmd_line, destination_path, l);
              obstack_1grow (&cmd_line, ' ');
            }
        }
    }

  gpc_args = obstack_copy0 (&cmd_line, filename, strlen (filename));

  if (flag_verbose)  /* i.e. gpc was invoked with `--verbose' */
    fprintf (stderr, "GPC AutoMake: ");

  if (execute (gpc_name, gpc_args) == 1)
    result = 0;
  else
    result = -1;

  if (flag_verbose)
    fprintf (stderr, "GPC AutoMake: done\n");

  /* Tell the linker about the object file.
   */
  plain_filename = filename + strlen (filename) - 1;
  while (plain_filename > filename
         && *plain_filename != '/'
         && *plain_filename != DIR_SEPARATOR)
    plain_filename--;
  if (*plain_filename == '/' || *plain_filename == DIR_SEPARATOR)
    plain_filename++;
  object_filename = xmalloc (strlen (plain_filename)
                             + strlen (OBJECT_SUFFIX) + 1);
  strcpy (object_filename, plain_filename);
  p = object_filename + strlen (plain_filename) - 1;
  while (p > object_filename && *p != '.')
    p--;
  *p = 0;
  strcat (object_filename, OBJECT_SUFFIX);
  p = locate_file (object_filename, unit_path);
  if (! p)
    p = locate_file (object_filename, unit_destination_path);
  if (! p)
    p = locate_file (object_filename, object_path);
  if (! p)
    p = locate_file (object_filename, object_destination_path);
  if (p)
    {
      add_to_link_file_list (p);
      free (p);
    }

  obstack_free (&cmd_line, gpc_args);
  free (filename);
  free (object_filename);

  return result;
}



/*  GPI file handling -- storing and retreiving tree nodes in an
 *  implementation-dependent (but not *too* implementation-dependent ;-)
 *  "GNU Pascal Interface" binary file.
 *
 *  For an introduction, read the `GPI' section of the Texinfo documentation.
 *
 *  @@@  This should be generalized to store arbitrary tree structures.
 *  This would have the following advantages:
 *
 *    - It would be cleaner (see `@@@@@@' in gpc-typeck.c).
 *
 *    - GPI would become suitable for other GNU languages, too
 *      ("C precompiled headers").
 *
 *    - The GPI format would not need to be changed so often as it is
 *      necessary now.  (Some version information must be included, too,
 *      for the case if it _does_ change.)
 *
 *    - load_tree() below would no more need to re-do everything the
 *      parser already did.  This would simplify and speed up load_tree()
 *      simultaneously.
 *
 *  - PG
 */

/* Complain about an invalid GPI file.
 */
static void
gpi_error ()
{
  fatal ("invalid GPI file `%s'", current_gpi_file_name);
}

/* Store a string in a binary stream
 */
static void
store_string (f, s)
     FILE *f;
     const char *s;
{
  unsigned char l = strlen (s);
  fwrite (&l, 1, 1, f);
  if (l > 0)
    fwrite (s, l, 1, f);
}

/* Load a string out of a binary stream
 */
static char *
load_string (f)
     MEMFILE *f;
{
  char *s;
  unsigned char l;
  if (mread (&l, sizeof (unsigned char), 1, f) != 1)
    gpi_error ();
  s = (char *) xmalloc ((int) l + 1);
  if (l > 0)
    if (mread (s, l, 1, f) != 1)
      gpi_error ();
  s [l] = 0;
  return s;
}

/* Store a tree node's flags in a stream
 */
static void
store_flags (t, f)
     tree t;
     FILE *f;
{
  fwrite ((tree *) t + 2, 4, 1, f);
}

/* Load a tree node's flags out of a stream
 */
static void
load_flags (t, f)
     tree t;
     MEMFILE *f;
{
  if (mread ((tree *) t + 2, 4, 1, f) != 1)
    gpi_error ();
}

/* Store a *_TYPE tree node's extra flags in a stream
 */
static void
store_type_flags (t, f)
     tree t;
     FILE *f;
{
  fwrite (&TYPE_UID (t), sizeof (unsigned) + 4 + sizeof (int), 1, f);
}

/* Load a *_TYPE tree node's extra flags out of a stream
 */
static void
load_type_flags (t, f)
     tree t;
     MEMFILE *f;
{
  if (mread (&TYPE_UID (t), sizeof (unsigned) + 4 + sizeof (int), 1, f) != 1)
    gpi_error ();
}

/* Store a *_DECL tree node's extra flags in a stream
 */
static void
store_decl_flags (t, f)
     tree t;
     FILE *f;
{
  fwrite (&DECL_UID (t), sizeof (unsigned) + 4, 1, f);
  fwrite (&DECL_FRAME_SIZE (t), sizeof (int), 1, f);
}

/* Load a *_DECL tree node's extra flags out of a stream
 */
static void
load_decl_flags (t, f)
     tree t;
     MEMFILE *f;
{
  if (mread (&DECL_UID (t), sizeof (unsigned) + 4, 1, f) != 1
      || mread (&DECL_FRAME_SIZE (t), sizeof (int), 1, f) != 1)
    gpi_error ();
}

/* Store a tree (containing a declaration) in a stream.
 * (A generalized version of this should go into tree.c.)
 */
void
store_tree (t, s, depth)
     tree t;
     FILE *s;
     int depth;
{
  unsigned char code;
  if (hashed_save (t, s, depth))
    return;
#define PROCESS_NODE(n,node)                                  \
  if (t == node)                                              \
    {                                                         \
      code = (unsigned char) LAST_AND_UNUSED_TREE_CODE + n;   \
      fwrite (&code, 1, 1, s);                                \
      if (flag_debug_gpi)                                     \
        {                                                     \
          char msg[] = STRINGIFY (node);                      \
          fprintf (stderr, "storing (%d): %s\n", depth, msg); \
        }                                                     \
      return;                                                 \
    }
#include "nodes.def"
#undef PROCESS_NODE
  if (flag_debug_gpi)
    {
      fprintf (stderr, "storing (%d):\n", depth);
      debug_tree (t);
    }
  code = (unsigned char) TREE_CODE (t);
  fwrite (&code, 1, 1, s);
  switch (TREE_CODE (t))
    {
      case IDENTIFIER_NODE:
        {
          store_string (s, IDENTIFIER_POINTER (t));
          store_flags (t, s);
          if (IDENTIFIER_LOCAL_VALUE (t))
            store_tree (IDENTIFIER_LOCAL_VALUE (t), s, depth + 1);
          else if (IDENTIFIER_GLOBAL_VALUE (t)
                   && ! PASCAL_REDEFINABLE_DECL (IDENTIFIER_GLOBAL_VALUE (t)))
            store_tree (IDENTIFIER_GLOBAL_VALUE (t), s, depth + 1);
          else
            store_tree (IDENTIFIER_LIMBO_VALUE (t), s, depth + 1);
          break;
        }

      case TREE_LIST:
        {
          while (t)
            {
              store_tree (TREE_PURPOSE (t), s, depth + 1);
              store_tree (TREE_VALUE (t), s, depth + 1);
              store_flags (t, s);
              t = TREE_CHAIN (t);
            }
          store_tree (NULL_TREE, s, depth + 1);
          store_tree (NULL_TREE, s, depth + 1);
          break;
        }

      case VOID_TYPE:
      case REAL_TYPE:
      case COMPLEX_TYPE:
      case BOOLEAN_TYPE:
      case CHAR_TYPE:
      case LANG_TYPE:
        {
          store_flags (t, s);
          store_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          store_tree (TYPE_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&TYPE_LANG_UID (t), sizeof (int), 1, s);
          store_tree (TYPE_SIZE (t), s, depth + 1);
#ifdef EGCS
          store_tree (TYPE_SIZE_UNIT (t), s, depth + 1);
#endif /* EGCS */
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (TYPE_MIN_VALUE (t), s, depth + 1);
          store_tree (TYPE_MAX_VALUE (t), s, depth + 1);
          store_tree (TYPE_MAIN_VARIANT (t), s, depth + 1);
          break;
        }

      case SET_TYPE:
        {
          store_flags (t, s);
          store_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          store_tree (TYPE_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&TYPE_LANG_UID (t), sizeof (int), 1, s);
          store_tree (TYPE_SIZE (t), s, depth + 1);
#ifdef EGCS
          store_tree (TYPE_SIZE_UNIT (t), s, depth + 1);
#endif /* EGCS */
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (TYPE_DOMAIN (t), s, depth + 1);
          store_tree (TYPE_MAIN_VARIANT (t), s, depth + 1);
          break;
        }

      case INTEGER_TYPE:
      case ENUMERAL_TYPE:
        {
          store_flags (t, s);
          store_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          store_tree (TYPE_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&TYPE_LANG_UID (t), sizeof (int), 1, s);
          store_tree (TYPE_SIZE (t), s, depth + 1);
#ifdef EGCS
          store_tree (TYPE_SIZE_UNIT (t), s, depth + 1);
#endif /* EGCS */
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (TYPE_MIN_VALUE (t), s, depth + 1);
          store_tree (TYPE_MAX_VALUE (t), s, depth + 1);
          store_tree (TYPE_MAIN_VARIANT (t), s, depth + 1);
          break;
        }

      case POINTER_TYPE:
      case REFERENCE_TYPE:
        {
          store_flags (t, s);
          store_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          store_tree (TYPE_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&TYPE_LANG_UID (t), sizeof (int), 1, s);
          store_tree (TYPE_SIZE (t), s, depth + 1);
#ifdef EGCS
          store_tree (TYPE_SIZE_UNIT (t), s, depth + 1);
#endif /* EGCS */
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (TYPE_MAIN_VARIANT (t), s, depth + 1);
          break;
        }

      case FILE_TYPE:
        {
          /* Since FILE_TYPE is not documented in tree.def,
           * I only can guess about its structure.
           */
          store_flags (t, s);
          store_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          store_tree (TYPE_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&TYPE_LANG_UID (t), sizeof (int), 1, s);
          store_tree (TYPE_SIZE (t), s, depth + 1);
#ifdef EGCS
          store_tree (TYPE_SIZE_UNIT (t), s, depth + 1);
#endif /* EGCS */
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (TYPE_MIN_VALUE (t), s, depth + 1);  /* is_text :*/
          store_tree (TYPE_MAIN_VARIANT (t), s, depth + 1);
          break;
        }

      case ARRAY_TYPE:
        {
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (TYPE_DOMAIN (t), s, depth + 1);
          store_flags (t, s);
          store_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          store_tree (TYPE_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&TYPE_LANG_UID (t), sizeof (int), 1, s);
          store_tree (TYPE_MAIN_VARIANT (t), s, depth + 1);
          break;
        }

      case RECORD_TYPE:
      case UNION_TYPE:
        {
          tree f;
          signed char lang_code = 0;

          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          store_tree (TYPE_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&TYPE_LANG_UID (t), sizeof (int), 1, s);
          store_flags (t, s);
          store_type_flags (t, s);
          store_tree (TYPE_SIZE (t), s, depth + 1);
#ifdef EGCS
          store_tree (TYPE_SIZE_UNIT (t), s, depth + 1);
#endif /* EGCS */
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          lang_code = TYPE_LANG_CODE (t);
          fwrite (&lang_code, 1, 1, s);
          store_tree (TYPE_LANG_INFO (t), s, depth + 1);
          store_tree (TYPE_LANG_BASE (t), s, depth + 1);
          for (f = TYPE_FIELDS (t); f; f = TREE_CHAIN (f))
            {
              store_tree (f, s, depth + 1);
              store_tree (DECL_FIELD_BITPOS (f), s, depth + 1);
            }
          store_tree (NULL_TREE, s, depth + 1);  /* end of field decls */
          store_tree (TYPE_MAIN_VARIANT (t), s, depth + 1);
          break;
        }

      case FUNCTION_TYPE:  /* @@@ unused? */
        {
          tree a;
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (TYPE_ARG_TYPES (t), s, depth + 1);
          for (a = TYPE_ATTRIBUTES (t); a; a = TREE_CHAIN (a))
            {
              store_tree (TREE_VALUE (a), s, depth + 1);
              store_tree (TREE_PURPOSE (a), s, depth + 1);
            }
          store_tree (NULL_TREE, s, depth + 1);
          store_flags (t, s);
          store_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          store_tree (TYPE_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&TYPE_LANG_UID (t), sizeof (int), 1, s);
          break;
        }

      case INTEGER_CST:
        {
          fwrite (&TREE_INT_CST_LOW (t), sizeof (HOST_WIDE_INT), 1, s);
          fwrite (&TREE_INT_CST_HIGH (t), sizeof (HOST_WIDE_INT), 1, s);
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_flags (t, s);
          break;
        }

      case REAL_CST:
        {
          store_flags (t, s);
          fwrite (&TREE_REAL_CST (t), sizeof (REAL_VALUE_TYPE), 1, s);
          store_tree (TREE_TYPE (t), s, depth + 1);
          break;
        }

      case COMPLEX_CST:
        {
          store_flags (t, s);
          store_tree (TREE_REALPART (t), s, depth + 1);
          store_tree (TREE_IMAGPART (t), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);
          break;
        }

      case STRING_CST:
        {
          store_flags (t, s);
          fwrite (&TREE_STRING_LENGTH (t), sizeof (int), 1, s);
          fwrite (TREE_STRING_POINTER (t), TREE_STRING_LENGTH (t) + 1, 1, s );
          store_tree (TREE_TYPE (t), s, depth + 1);
          break;
        }

      case FUNCTION_DECL:
        {
          tree a;
          store_tree (DECL_NAME (t), s, depth + 1);  /* name of function */
          store_tree (DECL_ASSEMBLER_NAME (t), s, depth + 1);
          store_tree (TREE_TYPE (TREE_TYPE (t)), s, depth + 1);  /* return type */

          /* Arguments.
           */
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          for (a = DECL_LANG_PARMS (t); a; a = TREE_CHAIN (a))
            if (TREE_CODE (a) == PARM_DECL)
              store_tree (a, s, depth + 1);
          /*
           * Store NULL_TREE (ellipsis) or void_type_node (no ellipsis).
           */
          for (a = TYPE_ARG_TYPES (TREE_TYPE (t)); a; a = TREE_CHAIN (a))
            if (TREE_VALUE (a) == void_type_node)
              break;
          if (a)
            store_tree (void_type_node, s, depth + 1);
          else
            store_tree (NULL_TREE, s, depth + 1);

          /* Machine attributes.
           */
          for (a = TYPE_ATTRIBUTES (TREE_TYPE (t)); a; a = TREE_CHAIN (a))
            {
              /* Both may be NULL_TREE.
               */
              store_tree (TREE_VALUE (a), s, depth + 1);
              store_tree (TREE_PURPOSE (a), s, depth + 1);
            }
          store_tree (error_mark_node, s, depth + 1);

          store_tree (DECL_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&DECL_LANG_UID (t), sizeof (int), 1, s);

          /* This might be a constructor or a virtual method.
           */
          store_flags (t, s);

          break;
        }

      case LABEL_DECL:
      case PARM_DECL:
      case RESULT_DECL:
        {
          store_flags (t, s);
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          store_tree (DECL_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&DECL_LANG_UID (t), sizeof (int), 1, s);
          store_tree (DECL_NAME (t), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);
          break;
        }

      case FIELD_DECL:
        {
          store_flags (t, s);
          store_decl_flags (t, s);
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          store_tree (DECL_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&DECL_LANG_UID (t), sizeof (int), 1, s);
          store_tree (DECL_NAME (t), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (DECL_SIZE (t), s, depth + 1);
          store_tree (DECL_FIELD_BITPOS (t), s, depth + 1);
          store_tree (PASCAL_DECL_FIXUPLIST (t), s, depth + 1);
          break;
        }

      case CONST_DECL:
      case TYPE_DECL:
        {
          store_flags (t, s);
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          store_tree (DECL_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&DECL_LANG_UID (t), sizeof (int), 1, s);
          store_tree (DECL_NAME (t), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_tree (DECL_INITIAL (t), s, depth + 1);
          break;
        }

      case VAR_DECL:
        {
          store_tree (DECL_NAME (t), s, depth + 1);
          store_tree (DECL_ASSEMBLER_NAME (t), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);

          /* Only store DECL_INITIAL if this is an `absolute' variable.
           */
          if (DECL_INITIAL (t)
              && PASCAL_ABSOLUTE_CLAUSE (DECL_INITIAL (t)))
            store_tree (DECL_INITIAL (t), s, depth + 1);
          else
            store_tree (NULL_TREE, s, depth + 1);

          store_flags (t, s);
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          store_tree (DECL_LANG_INTERFACE (t), s, depth + 1);
          fwrite (&DECL_LANG_UID (t), sizeof (int), 1, s);
          break;
        }

      case CONSTRUCTOR:
        {
          store_tree (TREE_OPERAND (t, 1), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_flags (t, s);
          break;
        }

      /* 3 operands */
      case BIT_FIELD_REF:
      case COND_EXPR:
      case METHOD_CALL_EXPR:
        {
          store_tree (TREE_OPERAND (t, 0), s, depth + 1);
          store_tree (TREE_OPERAND (t, 1), s, depth + 1);
          store_tree (TREE_OPERAND (t, 2), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_flags (t, s);
          break;
        }
      /* two operands */
      case COMPONENT_REF:
      case ARRAY_REF:
      case COMPOUND_EXPR:
      case MODIFY_EXPR:
      case INIT_EXPR:
      case CALL_EXPR:
      case PLUS_EXPR:
      case MINUS_EXPR:
      case MULT_EXPR:
      case TRUNC_DIV_EXPR:
      case CEIL_DIV_EXPR:
      case FLOOR_DIV_EXPR:
      case ROUND_DIV_EXPR:
      case TRUNC_MOD_EXPR:
      case CEIL_MOD_EXPR:
      case ROUND_MOD_EXPR:
      case RDIV_EXPR:
      case EXACT_DIV_EXPR:
      case MIN_EXPR:
      case MAX_EXPR:
      case LSHIFT_EXPR:
      case RSHIFT_EXPR:
      case LROTATE_EXPR:
      case RROTATE_EXPR:
      case BIT_IOR_EXPR:
      case BIT_XOR_EXPR:
      case BIT_AND_EXPR:
      case BIT_ANDTC_EXPR:
      case TRUTH_ANDIF_EXPR:
      case TRUTH_ORIF_EXPR:
      case TRUTH_AND_EXPR:
      case TRUTH_OR_EXPR:
      case TRUTH_XOR_EXPR:
      case LT_EXPR:
      case LE_EXPR:
      case GT_EXPR:
      case GE_EXPR:
      case EQ_EXPR:
      case NE_EXPR:
      case IN_EXPR:
      case SET_LE_EXPR:
      case RANGE_EXPR:
      case COMPLEX_EXPR:
        {
          store_tree (TREE_OPERAND (t, 0), s, depth + 1);
          store_tree (TREE_OPERAND (t, 1), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_flags (t, s);
          break;
        }
      /* one operand */
      case INDIRECT_REF:
      case BUFFER_REF:
      case FIX_TRUNC_EXPR:
      case FIX_CEIL_EXPR:
      case FIX_FLOOR_EXPR:
      case FIX_ROUND_EXPR:
      case FLOAT_EXPR:
      case NEGATE_EXPR:
      case ABS_EXPR:
      case FFS_EXPR:
      case BIT_NOT_EXPR:
      case TRUTH_NOT_EXPR:
      case CARD_EXPR:
      case CONVERT_EXPR:
      case NOP_EXPR:
      case PLACEHOLDER_EXPR:
      case NON_LVALUE_EXPR:
      case ADDR_EXPR:
      case REFERENCE_EXPR:
      case ENTRY_VALUE_EXPR:
      case CONJ_EXPR:
      case REALPART_EXPR:
      case IMAGPART_EXPR:
        {
          store_tree (TREE_OPERAND (t, 0), s, depth + 1);
          store_tree (TREE_TYPE (t), s, depth + 1);
          store_flags (t, s);
          break;
        }
      default:
        abort ();
    }
}

/* Return a newly allocated interface table.
 */
static struct interface_table_t *
new_interface_table (interface_name, module_name, max_uid)
     tree interface_name, module_name;
     int max_uid;
{
  struct interface_table_t *itab;
  for (itab = interface_table; itab; itab = itab->next)
    if (itab->interface_name == interface_name)
      {
        if (itab->module_name == NULL_PTR)
          itab->module_name = module_name;
        return itab;
      }
  itab = (struct interface_table_t *) xmalloc (sizeof (struct interface_table_t));
  memset ((void *) itab, 0, sizeof (struct interface_table_t));
  itab->interface_name = interface_name;
  itab->module_name = module_name;
  itab->next = interface_table;
  resize_itab_entries (itab, max_uid);
  interface_table = itab;
  return itab;
}

/* Make sure that the table ITAB can hold the entry MAX_UID.
 */
static void
resize_itab_entries (itab, max_uid)
     struct interface_table_t *itab;
     int max_uid;
{
  if (max_uid >= itab->entries)
    {
      int old_entries = itab->entries;
      itab->entries = max_uid + 1;
      itab->entry = (tree *)
                    xrealloc (itab->entry,
                              itab->entries * sizeof (tree));
      memset ((void *) (itab->entry + old_entries), 0,
              (itab->entries - old_entries) * sizeof (tree));
    }
}

/* Store the name of the interface a _DECL node T is loaded from
 * in its DECL_LANG_SPECIFIC slot.
 */
static tree
record_interface_specifications (decl, uid)
     tree decl;
     int uid;
{
  tree interface = we_are_loading_a_gpi_file;
  struct interface_table_t *itab;
  if (! decl)
    return NULL_TREE;
  switch (TREE_CODE_CLASS (TREE_CODE (decl)))
    {
      case 'd':
        {
          if (! DECL_LANG_SPECIFIC (decl))
            DECL_LANG_SPECIFIC (decl) = allocate_decl_lang_specific ();
          if (DECL_LANG_INTERFACE (decl))
            {
              interface = DECL_LANG_INTERFACE (decl);
              uid = DECL_LANG_UID (decl);
            }
          else
            {
              DECL_LANG_INTERFACE (decl) = interface;
              DECL_LANG_UID (decl) = uid;
            }
          break;
        }
      case 't':
        {
          if (! TYPE_LANG_SPECIFIC (decl))
            TYPE_LANG_SPECIFIC (decl) = allocate_type_lang_specific ();
          if (TYPE_LANG_INTERFACE (decl))
            {
              interface = TYPE_LANG_INTERFACE (decl);
              uid = TYPE_LANG_UID (decl);
            }
          else
            {
              TYPE_LANG_INTERFACE (decl) = interface;
              TYPE_LANG_UID (decl) = uid;
            }
          break;
        }
      default:
        return decl;
    }
  for (itab = interface_table; itab; itab = itab->next)
    if (itab->interface_name == interface)
      break;
  if (! itab)
    itab = new_interface_table (interface, NULL_TREE, uid);
  else
    resize_itab_entries (itab, uid);
  if (itab->entry[uid])
    return itab->entry[uid];
  else
    {
      itab->entry[uid] = decl;
      return decl;
    }
}

/* Return the name of the module the interface named NAME is in.
 */
tree
itab_get_module_name (name)
     tree name;
{
  struct interface_table_t *itab;
  for (itab = interface_table; itab; itab = itab->next)
    if (itab->interface_name == name)
      break;
  if (! itab)
    return NULL_TREE;
  else
    return itab->module_name;
}


/* Load (parts of) a tree (containing a declaration) out of a stream.
 * (A generalized version of this should go into tree.c.)
 */
tree
load_tree (s, depth)
     MEMFILE *s;
     int depth;
{
  unsigned char code;
  tree t = NULL_TREE;

  int uid;
  off_t save_pos;
  assert (depth);
  if (mread (&uid, sizeof (uid), 1, s) != 1)
    gpi_error ();
  save_pos = mtell (s);
  if (!uid)
    return 0;
  t = hashed_load (s, uid);
  if (t)
    return t;
  if (mread (&code, 1, 1, s) != 1)
    gpi_error ();
  push_obstacks_nochange ();
  end_temporary_allocation ();
  switch (code)
    {
#define PROCESS_NODE(n,node)                                      \
      case LAST_AND_UNUSED_TREE_CODE + n:                         \
        {                                                         \
          t = node;                                               \
          if (flag_debug_gpi)                                     \
            {                                                     \
              char msg[] = STRINGIFY (node);                      \
              fprintf (stderr, "loaded (%d): %s\n", depth, msg);  \
            }                                                     \
          t = hash_loaded_node (t, uid);                          \
          return t;                                               \
        }
#include "nodes.def"
#undef PROCESS_NODE

      case IDENTIFIER_NODE:
        {
          char *id = load_string (s);
          t = get_identifier (id);
          free (id);
          load_flags (t, s);
          t = hash_loaded_node (t, uid);
          IDENTIFIER_LIMBO_VALUE (t) = load_tree (s, depth + 1);
          /* for renaming etc. */
          break;
        }

      case TREE_LIST:
        {
          tree purpose, value;
          t = NULL_TREE;
          purpose = load_tree (s, depth + 1);
          value = load_tree (s, depth + 1);
          while (purpose || value)
            {
              t = chainon (t, build_tree_list (purpose, value));
              load_flags (t, s);
              t = hash_loaded_node (t, uid);
              purpose = load_tree (s, depth + 1);
              value = load_tree (s, depth + 1);
            }
          break;
        }

      case VOID_TYPE:
      case REAL_TYPE:
      case COMPLEX_TYPE:
      case BOOLEAN_TYPE:
      case CHAR_TYPE:
      case LANG_TYPE:
        {
          t = make_node (code);
          load_flags (t, s);
          load_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&TYPE_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          TYPE_SIZE (t) = load_tree (s, depth + 1);
#ifdef EGCS
          TYPE_SIZE_UNIT (t) = load_tree (s, depth + 1);
#endif /* EGCS */
          TREE_TYPE (t) = load_tree (s, depth + 1);
          TYPE_MIN_VALUE (t) = load_tree (s, depth + 1);
          TYPE_MAX_VALUE (t) = load_tree (s, depth + 1);
          TYPE_MAIN_VARIANT (t) = load_tree (s, depth + 1);
          break;
        }

      case SET_TYPE:
        {
          t = make_node (code);
          load_flags (t, s);
          load_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&TYPE_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          TYPE_SIZE (t) = load_tree (s, depth + 1);
#ifdef EGCS
          TYPE_SIZE_UNIT (t) = load_tree (s, depth + 1);
#endif /* EGCS */
          TREE_TYPE (t) = load_tree (s, depth + 1);
          TYPE_DOMAIN (t) = load_tree (s, depth + 1);
          TYPE_MAIN_VARIANT (t) = load_tree (s, depth + 1);
          break;
        }

      case INTEGER_TYPE:
      case ENUMERAL_TYPE:
        {
          t = make_node (code);
          load_flags (t, s);
          load_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&TYPE_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          TYPE_SIZE (t) = load_tree (s, depth + 1);
#ifdef EGCS
          TYPE_SIZE_UNIT (t) = load_tree (s, depth + 1);
#endif /* EGCS */
          TREE_TYPE (t) = load_tree (s, depth + 1);
          TYPE_MIN_VALUE (t) = load_tree (s, depth + 1);
          TYPE_MAX_VALUE (t) = load_tree (s, depth + 1);
          TYPE_MAIN_VARIANT (t) = load_tree (s, depth + 1);
          break;
        }

      case POINTER_TYPE:
      case REFERENCE_TYPE:
        {
          t = make_node (code);
          TREE_TYPE (t) = error_mark_node;  /* Avoid crash in recursion */
          load_flags (t, s);
          load_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&TYPE_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          TYPE_SIZE (t) = load_tree (s, depth + 1);
#ifdef EGCS
          TYPE_SIZE_UNIT (t) = load_tree (s, depth + 1);
#endif /* EGCS */
          TREE_TYPE (t) = load_tree (s, depth + 1);
          TYPE_MAIN_VARIANT (t) = load_tree (s, depth + 1);
          break;
        }

      case FILE_TYPE:
        {
          /* Since FILE_TYPE is not documented in tree.def,
           * I only can guess about its structure.
           */
          t = make_node (code);
          load_flags (t, s);
          load_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&TYPE_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          TYPE_SIZE (t) = load_tree (s, depth + 1);
#ifdef EGCS
          TYPE_SIZE_UNIT (t) = load_tree (s, depth + 1);
#endif /* EGCS */
          TREE_TYPE (t) = load_tree (s, depth + 1);
          TYPE_MIN_VALUE (t) = load_tree (s, depth + 1);  /* is_text :*/
          TYPE_MAIN_VARIANT (t) = load_tree (s, depth + 1);
          break;
        }

      case ARRAY_TYPE:
        {
          tree type, domain;
          type = load_tree (s, depth + 1);
          domain = load_tree (s, depth + 1);
          /* In case this array already exists, we do not want to
           * spoil its flags, thus we copy_node it.
           */
          t = copy_node (build_array_type (type, domain));
          load_flags (t, s);
          load_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&TYPE_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          TYPE_MAIN_VARIANT (t) = load_tree (s, depth + 1);
          break;
        }

      case RECORD_TYPE:
      case UNION_TYPE:
        {
          tree f, t0, f0 = NULL_TREE;
          tree current_type_name_save = current_type_name;
          signed char lang_code;
          t0 = make_node (code);
          if (! TYPE_LANG_SPECIFIC (t0))
            TYPE_LANG_SPECIFIC (t0) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t0) = load_tree (s, depth + 1);
          if (mread (&TYPE_LANG_UID (t0), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t0, uid);
          if (t != t0)
            break;
          t = hash_loaded_node (t, uid);
          load_flags (t, s);
          load_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t) = TYPE_LANG_INTERFACE (t0);
          TYPE_LANG_UID (t) = TYPE_LANG_UID (t0);
          TYPE_SIZE (t) = load_tree (s, depth + 1);
#ifdef EGCS
          TYPE_SIZE_UNIT (t) = load_tree (s, depth + 1);
#endif /* EGCS */
          if (mread (&lang_code, 1, 1, s) != 1)
            gpi_error ();
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_CODE (t) = lang_code;
          TYPE_LANG_INFO (t) = load_tree (s, depth + 1);
          TYPE_LANG_BASE (t) = load_tree (s, depth + 1);
          if (lang_code == PASCAL_LANG_OBJECT)
            current_type_name = TYPE_LANG_NAME (t);  /* == TYPE_LANG_INFO */
          do
            {
              f = load_tree (s, depth + 1);
              if (f)
                {
                  /* Don't use chainon: danger of circularity
                   * if f0 was loaded via gpi_contents.
                   */
                  if (f0)
                    TREE_CHAIN (f0) = f;
                  else
                    TYPE_FIELDS (t) = f;
                  /* Delete possibly invalid TREE_CHAIN info
                   * if t was loaded via gpi_contents.
                   */
                  TREE_CHAIN (f) = NULL_TREE;
                  f0 = f;
                  DECL_CONTEXT (f) = t;
                  DECL_FIELD_BITPOS (f) = load_tree (s, depth + 1);
                }
            }
          while (f);
          TYPE_MAIN_VARIANT (t) = load_tree (s, depth + 1);
          current_type_name = current_type_name_save;  /* usually NULL_TREE */
          break;
        }

      case FUNCTION_TYPE:  /* @@@ unused? */
        {
          tree type = load_tree (s, depth + 1);
          tree arg_types = load_tree (s, depth + 1);
          tree a;
          t = build_function_type (type, arg_types);
          a = load_tree (s, depth + 1);
          while (a)
            {
              TYPE_ATTRIBUTES (t) = chainon (TYPE_ATTRIBUTES (t),
                  build_tree_list (load_tree (s, depth + 1), a));
              a = load_tree (s, depth + 1);
            }
          load_flags (t, s);
          load_type_flags (t, s);
          if (! TYPE_LANG_SPECIFIC (t))
            TYPE_LANG_SPECIFIC (t) = allocate_type_lang_specific ();
          TYPE_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&TYPE_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          break;
        }

      case INTEGER_CST:
        {
          t = make_node (code);
          if (mread (&TREE_INT_CST_LOW (t), sizeof (HOST_WIDE_INT), 1, s) != 1)
            gpi_error ();
          if (mread (&TREE_INT_CST_HIGH (t), sizeof (HOST_WIDE_INT), 1, s) != 1)
            gpi_error ();
          t = hash_loaded_node (t, uid);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          load_flags (t, s);
          break;
        }

      case REAL_CST:
        {
          t = make_node (code);
          load_flags (t, s);
          if (mread (&TREE_REAL_CST (t), sizeof (REAL_VALUE_TYPE), 1, s) != 1)
            gpi_error ();
          t = hash_loaded_node (t, uid);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          break;
        }

      case COMPLEX_CST:
        {
          t = make_node (code);
          load_flags (t, s);
          t = hash_loaded_node (t, uid);
          TREE_REALPART (t) = load_tree (s, depth + 1);
          TREE_IMAGPART (t) = load_tree (s, depth + 1);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          break;
        }

      case STRING_CST:
        {
          t = make_node (code);
          load_flags (t, s);
          t = hash_loaded_node (t, uid);
          if (mread (&TREE_STRING_LENGTH (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          TREE_STRING_POINTER (t) = oballoc (TREE_STRING_LENGTH (t) + 9);
          if (mread (TREE_STRING_POINTER (t), TREE_STRING_LENGTH (t) + 1, 1, s ) != 1)
            gpi_error ();
          TREE_TYPE (t) = load_tree (s, depth + 1);
          break;
        }

      case FUNCTION_DECL:
        {
          tree name, full_name, asmname, type, d, heading, save_local;
          tree args, a, last_arg, argdecls;
          tree directives = NULL_TREE, attributes = NULL_TREE;
          tree t0 = NULL_TREE;
          int ellipsis;
          int in_parm_list = in_parm_level_p ();
          extern tree d_attribute;

          name = load_tree (s, depth + 1);
          save_local = IDENTIFIER_LOCAL_VALUE (name);
          if (current_type_name)
            full_name = get_method_name (current_type_name, name);
          else
            full_name = name;
          asmname = load_tree (s, depth + 1);
          type = load_tree (s, depth + 1);

          if (in_parm_list)
            {
              /* This is a method definition of an object reference
               * inside the parameter list.  Don't let external
               * function declarations become global here.
               */
              pushlevel (0);
            }

          immediate_size_expand = 0;
          size_volatile++;

          /* Arguments.
           */
          args = NULL_TREE;
          last_arg = NULL_TREE;
          a = load_tree (s, depth + 1);
          while (a != NULL_TREE && a != void_type_node)
            {
              if (last_arg)
                TREE_CHAIN (last_arg) = a;
              else
                args = a;
              last_arg = a;
              a = load_tree (s, depth + 1);
            }
          if (last_arg)
            TREE_CHAIN (last_arg) = NULL_TREE;
          ellipsis = (a == NULL_TREE);

          /* Machine attributes.
           */
          a = load_tree (s, depth + 1);
          while (a != error_mark_node)
            {
              tree b = load_tree (s, depth + 1);
              attributes = chainon (attributes, build_tree_list (b, a));
              a = load_tree (s, depth + 1);
            }
          if (attributes)
            directives = build_tree_list (attributes, d_attribute);

          type = tree_cons (NULL_TREE, type, NULL_TREE);
          type = tree_cons (NULL_TREE, extern_id, type);

          pushlevel (0);
          clear_parm_order ();
          declare_parm_level (1);
          for (a = args; a; a = TREE_CHAIN (a))
            {
              /* Do essentially the same as handle_formal_param_list().
               * We don't need to think about schemata etc. here because
               * they already were prediscriminated before they were
               * written into the GPI file.
               */
              tree name = DECL_NAME (a);
              d = build_tree_list (NULL_TREE, TREE_TYPE (a));
              d = build_tree_list (build_tree_list (d, name),
                                   build_tree_list (NULL_TREE, NULL_TREE));
              push_parm_decl (d);
            }
          argdecls = get_parm_info (! ellipsis);
          poplevel (0, 0, 0);

          size_volatile--;

          /* Avoid a warning in case this Unit is in the same file
           * as the main program.
           */
          IDENTIFIER_LIMBO_VALUE (full_name) = NULL_TREE;

          heading = build_nt (CALL_EXPR, full_name, argdecls, NULL_TREE);
          if (! asmname || full_name == asmname)
            {
              directives = chainon (directives,
                                    build_tree_list (NULL_TREE, d_external));
              t = grok_directive (type, heading, directives, 1);
            }
          else
            {
              directives = chainon (directives,
                                    build_tree_list (asmname, d_asmname));
              t = grok_directive (type, heading, directives, 1);
            }

          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          DECL_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&DECL_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();

          /* This might be a constructor or a virtual method.
           */
          load_flags (t, s);

          if (current_type_name)
            {
              /* Normally, t will be ignored since grok_directive already
               * has done the job of an external function declaration.  Only
               * if this is a method (i.e. current_type_name != NULL_TREE),
               * a copy of t with `name' instead of `full_name' will go as
               * a field into the object (a RECORD_TYPE node).
               */
              t0 = t;
              t = copy_node (t);
              DECL_NAME (t) = name;
              if (! DECL_LANG_SPECIFIC (t))
                DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
              memcpy (DECL_LANG_SPECIFIC (t), DECL_LANG_SPECIFIC (t0),
                      sizeof (struct lang_decl));
              t0 = record_interface_specifications (t0, uid);
            }
          else
            t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);

          if (in_parm_list)
            poplevel (0, 0, 0);

          /* Restore the original local value in order to enable `import only'.
           */
          IDENTIFIER_LIMBO_VALUE (name) = IDENTIFIER_LOCAL_VALUE (name);
          IDENTIFIER_LOCAL_VALUE (name) = save_local;
          break;
        }

      case LABEL_DECL:
      case PARM_DECL:
      case RESULT_DECL:
        {
          t = make_node (code);
          load_flags (t, s);
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          DECL_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&DECL_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          DECL_NAME (t) = load_tree (s, depth + 1);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          DECL_CONTEXT (t) = current_function_decl;
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          break;
        }

      case FIELD_DECL:
        {
          t = make_node (code);
          load_flags (t, s);
          load_decl_flags (t, s);
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          DECL_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&DECL_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          DECL_NAME (t) = load_tree (s, depth + 1);
          size_volatile++;
          TREE_TYPE (t) = load_tree (s, depth + 1);
          size_volatile--;
          DECL_SIZE (t) = load_tree (s, depth + 1);
          DECL_FIELD_BITPOS (t) = load_tree (s, depth + 1);
          PASCAL_DECL_FIXUPLIST (t) = load_tree (s, depth + 1);
          break;
        }

      case CONST_DECL:
      case TYPE_DECL:
        {
          t = make_node (code);
          load_flags (t, s);
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          DECL_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&DECL_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          DECL_NAME (t) = load_tree (s, depth + 1);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          if (code == TYPE_DECL)
            TYPE_NAME (TREE_TYPE (t)) = DECL_NAME (t);
          DECL_INITIAL (t) = load_tree (s, depth + 1);
          DECL_CONTEXT (t) = current_function_decl;
          break;
        }

#ifdef NEW_DECLARE_VARS

      case VAR_DECL:
        {
          tree name = load_tree (s, depth + 1);
          tree asmname = load_tree (s, depth + 1);
          tree type = load_tree (s, depth + 1);
          t = build_decl (code, name, type);
          DECL_ASSEMBLER_NAME (t) = asmname;
          DECL_INITIAL (t) = load_tree (s, depth + 1);
          DECL_CONTEXT (t) = current_function_decl;
          load_flags (t, s);
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          DECL_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&DECL_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);
          if (code == TYPE_DECL)
            TYPE_NAME (TREE_TYPE (t)) = DECL_NAME (t);
          pushdecl (t);
          rest_of_decl_compilation (t, 
            asmname ? IDENTIFIER_POINTER (asmname) : NULL_PTR, 1, 1);
          expand_decl (t);
          break;
        }

#else /* not NEW_DECLARE_VARS */

      case VAR_DECL:
        {
          /* @@@@ We re-build the tree node rather than load it.
           */
          tree name, asmname, type, initial;
          unsigned qualifiers = VQ_EXTERNAL;
          tree save_local, save_global;
          union tree_node t0;
          name = load_tree (s, depth + 1);
          save_local = IDENTIFIER_LOCAL_VALUE (name);
          save_global = IDENTIFIER_GLOBAL_VALUE (name);
          asmname = load_tree (s, depth + 1);
          type = load_tree (s, depth + 1);
          initial = load_tree (s, depth + 1);

          if (initial && PASCAL_ABSOLUTE_CLAUSE (initial))
            {
              t = build_decl (VAR_DECL, name, type);
              DECL_INITIAL (t) = initial;
              pushdecl (t);
              load_flags (t, s);
            }
          else
            {
              /* Use a dummy tree node to load the flags now,
               * because we need the volatile flag.
               */
              load_flags (&t0, s);
              if (TREE_THIS_VOLATILE (&t0))
                qualifiers |= VQ_VOLATILE;
              declare_vars (build_tree_list (NULL_TREE, name), type, NULL_TREE,
                            asmname ? IDENTIFIER_POINTER (asmname) : NULL_PTR,
                            qualifiers, 1, NULL_TREE /* attributes */);
              t = lookup_name (name);
              memcpy ((tree *) t + 2, (tree *) &t0 + 2, 4);
            }
          if (! DECL_LANG_SPECIFIC (t))
            DECL_LANG_SPECIFIC (t) = allocate_decl_lang_specific ();
          DECL_LANG_INTERFACE (t) = load_tree (s, depth + 1);
          if (mread (&DECL_LANG_UID (t), sizeof (int), 1, s) != 1)
            gpi_error ();
          t = record_interface_specifications (t, uid);
          t = hash_loaded_node (t, uid);

          /* Restore the original local value in order to enable `import only'.
           */
          IDENTIFIER_LIMBO_VALUE (name) = IDENTIFIER_LOCAL_VALUE (name);
          IDENTIFIER_LOCAL_VALUE (name) = save_local;
          IDENTIFIER_GLOBAL_VALUE (name) = save_global;
          break;
        }

#endif /* not NEW_DECLARE_VARS */

      case CONSTRUCTOR:
        {
          t = make_node (code);
          t = hash_loaded_node (t, uid);
          TREE_OPERAND (t, 1) = load_tree (s, depth + 1);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          load_flags (t, s);
          break;
        }

      /* 3 operands */
      case BIT_FIELD_REF:
      case COND_EXPR:
      case METHOD_CALL_EXPR:
        {
          t = make_node (code);
          t = hash_loaded_node (t, uid);
          TREE_OPERAND (t, 0) = load_tree (s, depth + 1);
          TREE_OPERAND (t, 1) = load_tree (s, depth + 1);
          TREE_OPERAND (t, 2) = load_tree (s, depth + 1);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          load_flags (t, s);
          break;
        }
      /* two operands */
      case COMPONENT_REF:
      case ARRAY_REF:
      case COMPOUND_EXPR:
      case MODIFY_EXPR:
      case INIT_EXPR:
      case CALL_EXPR:
      case PLUS_EXPR:
      case MINUS_EXPR:
      case MULT_EXPR:
      case TRUNC_DIV_EXPR:
      case CEIL_DIV_EXPR:
      case FLOOR_DIV_EXPR:
      case ROUND_DIV_EXPR:
      case TRUNC_MOD_EXPR:
      case CEIL_MOD_EXPR:
      case ROUND_MOD_EXPR:
      case RDIV_EXPR:
      case EXACT_DIV_EXPR:
      case MIN_EXPR:
      case MAX_EXPR:
      case LSHIFT_EXPR:
      case RSHIFT_EXPR:
      case LROTATE_EXPR:
      case RROTATE_EXPR:
      case BIT_IOR_EXPR:
      case BIT_XOR_EXPR:
      case BIT_AND_EXPR:
      case BIT_ANDTC_EXPR:
      case TRUTH_ANDIF_EXPR:
      case TRUTH_ORIF_EXPR:
      case TRUTH_AND_EXPR:
      case TRUTH_OR_EXPR:
      case TRUTH_XOR_EXPR:
      case LT_EXPR:
      case LE_EXPR:
      case GT_EXPR:
      case GE_EXPR:
      case EQ_EXPR:
      case NE_EXPR:
      case IN_EXPR:
      case SET_LE_EXPR:
      case RANGE_EXPR:
      case COMPLEX_EXPR:
        {
          t = make_node (code);
          t = hash_loaded_node (t, uid);
          TREE_OPERAND (t, 0) = load_tree (s, depth + 1);
          TREE_OPERAND (t, 1) = load_tree (s, depth + 1);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          load_flags (t, s);
          break;
        }
      /* one operand */
      case INDIRECT_REF:
      case BUFFER_REF:
      case FIX_TRUNC_EXPR:
      case FIX_CEIL_EXPR:
      case FIX_FLOOR_EXPR:
      case FIX_ROUND_EXPR:
      case FLOAT_EXPR:
      case NEGATE_EXPR:
      case ABS_EXPR:
      case FFS_EXPR:
      case BIT_NOT_EXPR:
      case TRUTH_NOT_EXPR:
      case CARD_EXPR:
      case CONVERT_EXPR:
      case NOP_EXPR:
      case PLACEHOLDER_EXPR:
      case NON_LVALUE_EXPR:
      case ADDR_EXPR:
      case REFERENCE_EXPR:
      case ENTRY_VALUE_EXPR:
      case CONJ_EXPR:
      case REALPART_EXPR:
      case IMAGPART_EXPR:
        {
          t = make_node (code);
          t = hash_loaded_node (t, uid);
          TREE_OPERAND (t, 0) = load_tree (s, depth + 1);
          TREE_TYPE (t) = load_tree (s, depth + 1);
          load_flags (t, s);
          break;
        }
      default:
        abort ();
    }
  pop_obstacks ();

  if (flag_debug_gpi && code < LAST_AND_UNUSED_TREE_CODE)
    {
      fprintf (stderr, "loaded (%d):\n", depth);
      debug_tree (t);
    }
  mseek (s, save_pos, SEEK_SET);
  return t;
}

/* Create a GPM file (GNU Pascal Module) pointing to the GPI files.
 * This is needed when an Implementation Module is looking for the
 * Interfaces.
 */
void
create_gpm_file ()
{
  tree escan;
  FILE *gpm_file;
  char *gpm_file_name, *p;
  char gpm_header [] = "GNU Pascal Module\n";

  if (flag_implementation_only)
    return;

  if (gpi_destination_path)
    gpm_file_name = concat (gpi_destination_path,
                            IDENTIFIER_POINTER (current_module->name),
                            ".gpm", NULL_PTR);
  else
    gpm_file_name = concat (IDENTIFIER_POINTER (current_module->name),
                            ".gpm", NULL_PTR);
  for (p = gpm_file_name; *p; p++)
    *p = tolower (*p);
  gpm_file = fopen (gpm_file_name, "wb");
  if (!gpm_file)
    fatal ("cannot create GPM file `%s'", gpm_file_name);
  if (flag_debug_gpi)
    fprintf (stderr, "storing GPM header: %s", gpm_header);
  fprintf (gpm_file, gpm_header);
  for (escan = current_module->exports; escan; escan = TREE_CHAIN (escan))
    {
      tree name = TREE_VALUE (escan);
      store_string (gpm_file, IDENTIFIER_POINTER (name));
      if (flag_debug_gpi)
        fprintf (stderr, "storing Interface name %s\n", IDENTIFIER_POINTER (name));
    }
  store_string (gpm_file, "");
  fclose (gpm_file);
}

/* Create GPI files (GNU Pascal Interface) containing precompiled
 * export Interfaces of a Unit or Module.
 */
void
create_gpi_files ()
{
  tree escan;

  if (flag_implementation_only)
    return;

  for (escan = current_module->exports; escan; escan = TREE_CHAIN (escan))
    {
      FILE *gpi_file;
      char *plain_input_filename, *gpi_file_name, *p;
      tree name = TREE_VALUE (escan), nscan, iscan;
      char gpi_header [] = "GNU Pascal Unit/Module Interface\n";
      string_list *link_file;

      p = IDENTIFIER_POINTER (name);
      if (gpi_destination_path)
        gpi_file_name = concat (gpi_destination_path, p, ".gpi", NULL_PTR);
      else
        gpi_file_name = concat (p, ".gpi", NULL_PTR);
      for (p = gpi_file_name; *p; p++)
        *p = tolower (*p);
      gpi_file = fopen (gpi_file_name, "wb");
      if (! gpi_file)
        fatal ("cannot create GPI file `%s'", gpi_file_name);
      if (flag_debug_gpi)
        fprintf (stderr, "storing GPI header: %s", gpi_header);
      fwrite (gpi_header, 33, 1, gpi_file);
      if (flag_debug_gpi)
        fprintf (stderr, "storing Module name: %s\n", IDENTIFIER_POINTER (current_module->name));
      store_string (gpi_file, IDENTIFIER_POINTER (current_module->name));
      plain_input_filename = main_input_filename + strlen (main_input_filename) - 1;
      while (plain_input_filename >= main_input_filename
             && *plain_input_filename != '/'
             && *plain_input_filename != DIR_SEPARATOR)
        plain_input_filename--;
      plain_input_filename++;
      if (flag_debug_gpi)
        fprintf (stderr, "storing Module source file name: %s\n", plain_input_filename);
      store_string (gpi_file, plain_input_filename);
      /* Store names of interfaces imported by this module */
      if (flag_debug_gpi)
        fprintf (stderr, "storing names of imported Interfaces:\n");
      for (iscan = current_module->imports; iscan;
           iscan = TREE_CHAIN (iscan))
        if (! TREE_STATIC (iscan))
          {
            /* Store them as strings, not as tree nodes. */
            /* We don't yet want to use the gpi_contents mechanism. */
            tree iname = TREE_VALUE (iscan);
            if (!iname || TREE_CODE (iname) != IDENTIFIER_NODE)
              abort ();
            if (flag_debug_gpi)
              fprintf (stderr, "  %s\n", IDENTIFIER_POINTER (iname));
            store_string (gpi_file, IDENTIFIER_POINTER (iname));
          }
      store_string (gpi_file, "");
      if (flag_debug_gpi)
        fprintf (stderr, "storing names of files to be linked:\n");
      for (link_file = current_module->link_files; link_file;
           link_file = link_file->next)
        {
          if (flag_debug_gpi)
            fprintf (stderr, "  %s\n", link_file->string);
          store_string (gpi_file, link_file->string);
        }
      store_string (gpi_file, "");
      {
        tree nnscan;
        tree purpose;
        int nn = 0;
        /* ??? how many nodes will be stored ? aprox */
        for (nnscan = TREE_PURPOSE (escan); nnscan; nnscan = TREE_CHAIN (nnscan))
          nn++;
        store_header (gpi_file, nn);
        purpose = build_tree_list (NULL_TREE, TREE_VALUE (escan));
        for (nscan = TREE_PURPOSE (escan); nscan; nscan = TREE_CHAIN (nscan))
          {
            if (TREE_VALUE (nscan)        /* both may be NULL_TREE */
                || TREE_PURPOSE (nscan))
              {
                if (TREE_CODE (TREE_VALUE (nscan)) != IDENTIFIER_NODE)
                  abort ();
                else if (lookup_name (TREE_VALUE (nscan)) == NULL_TREE)
                  error ("exported name `%s' undefined",
                         IDENTIFIER_POINTER (TREE_VALUE (nscan)));
                purpose = chainon (purpose,
                                   build_tree_list (TREE_PURPOSE (nscan),
                                                    TREE_VALUE (nscan)));
              }
          }
        store_tree (purpose, gpi_file, 1);
        flush_suspended (gpi_file);
      }
      update_header (gpi_file);
      clean_imbed_file_obstack ();
      {
        unsigned dummy_length = 0;
        fwrite (&dummy_length, sizeof (dummy_length), 1, gpi_file);
      }
      fclose (gpi_file);
      free (gpi_file_name);
    }
}

/* Extend GPI files to contain additional information from
 * the implementation part.
 */
void
extend_gpi_files ()
{
  tree escan;
  unsigned length = 0;

  if (flag_implementation_only)
    {
      if (flag_automake)
        warning ("`--automake' together with `--implementation-only' can cause problems");
      return;
    }

  for (escan = current_module->exports; escan; escan = TREE_CHAIN (escan))
    {
      FILE *gpi_file;
      char *p;
      tree name = TREE_VALUE (escan), iscan;
      string_list *link_file;

      p = IDENTIFIER_POINTER (name);
      if (gpi_destination_path)
        current_gpi_file_name = concat (gpi_destination_path, p, ".gpi", NULL_PTR);
      else
        current_gpi_file_name = concat (p, ".gpi", NULL_PTR);
      for (p = current_gpi_file_name; *p; p++)
        *p = tolower (*p);
      gpi_file = fopen (current_gpi_file_name, "ab");
      if (! gpi_file)
        fatal ("cannot append to GPI file `%s'", current_gpi_file_name);

      /* Store additional names of interfaces imported by this module
       * and files to be linked.
       */
      if (flag_debug_gpi)
        fprintf (stderr, "storing additional names of imported Interfaces:\n");
      for (iscan = current_module->imports; iscan;
           iscan = TREE_CHAIN (iscan))
        if (TREE_STATIC (iscan))
          {
            /* Store them as strings, not as tree nodes. */
            /* We don't yet want to use the gpi_contents mechanism. */
            tree iname = TREE_VALUE (iscan);
            if (!iname || TREE_CODE (iname) != IDENTIFIER_NODE)
              abort ();
            if (flag_debug_gpi)
              fprintf (stderr, "  %s\n", IDENTIFIER_POINTER (iname));
            store_string (gpi_file, IDENTIFIER_POINTER (iname));
            length += strlen (IDENTIFIER_POINTER (iname)) + 1;
          }
      store_string (gpi_file, "");
      length++;
      if (flag_debug_gpi)
        fprintf (stderr, "storing additional names of files to be linked:\n");
      for (link_file = current_module->link_files; link_file;
           link_file = link_file->next)
        {
          if (flag_debug_gpi)
            fprintf (stderr, "  %s\n", link_file->string);
          store_string (gpi_file, link_file->string);
          length += strlen (link_file->string) + 1;
        }
      store_string (gpi_file, "");
      length++;
      fwrite (&length, sizeof (length), 1, gpi_file);
      fclose (gpi_file);
      free (current_gpi_file_name);
      current_gpi_file_name = NULL_PTR;
    }
}

/* Subroutine of gpi_open():  Search for the source of an interface.
 */
static char*
locate_interface_source (interface_name, explicit_name, gpi_stored_name)
     char *interface_name, *gpi_stored_name, *explicit_name;
{
  char *result = NULL_PTR;
  char *full_unit_path = NULL_PTR;
  char path_separator[2] = {PATH_SEPARATOR, 0};

  if (unit_path && unit_destination_path)
    full_unit_path = concat (unit_path, path_separator,
                             unit_destination_path, NULL_PTR);
  else if (unit_path)
    full_unit_path = save_string (unit_path);
  else if (unit_destination_path)
    full_unit_path = save_string (unit_destination_path);

  /* First try the name given in EXPLICIT_NAME.  The user may omit the
   * extension and/or use capital letters in the filename.
   */
  if (explicit_name)
    {
      char *module_filename = xmalloc (strlen (explicit_name) + 4 + 1), *mfn_end;
      strcpy (module_filename, explicit_name);  /* First, try the given name. */
      mfn_end = strchr (module_filename, 0);
      result = locate_file (module_filename, full_unit_path);
      if (! result)
        {
          strcpy (mfn_end, ".pas");  /* Next, try extension `.pas' */
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".p");  /* Next, try extension `.p' */
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".pp");  /* Next, try extension `.pp' */
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".dpr");  /* Next, try extension `.dpr' */
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          /* No success yet.  But the user did specify the filename
           * by a string constant.  Try decapitalized version.
           */
          char *p = module_filename;
          *mfn_end = 0;
          while (*p)
            {
              *p = tolower (*p);
              p++;
            }
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".pas");
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".p");
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".pp");
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".dpr");
          result = locate_file (module_filename, full_unit_path);
        }
      free (module_filename);
    }
  /* EXPLICIT_NAME not given.  Try to derive the source file name from
   * the INTERFACE_NAME which is the name of the `.gpi' file.
   */
  else if (interface_name)
    {
      char *module_filename = xmalloc (strlen (interface_name) + 4 + 1), *mfn_end;
      strcpy (module_filename, interface_name);
      mfn_end = strchr (module_filename, 0);
      /* Cut the extension `.gpi' */
      do
        mfn_end--;
      while (mfn_end > module_filename && *mfn_end != '.');
      *mfn_end = 0;
      strcpy (mfn_end, ".pas");
      result = locate_file (module_filename, full_unit_path);
      if (! result)
        {
          strcpy (mfn_end, ".p");
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".pp");
          result = locate_file (module_filename, full_unit_path);
        }
      if (! result)
        {
          strcpy (mfn_end, ".dpr");
          result = locate_file (module_filename, full_unit_path);
        }
      free (module_filename);
    }
  /* Last resort:  Try the name stored in the GPI file.
   */
  if (! result && gpi_stored_name)
    result = locate_file (gpi_stored_name, full_unit_path);
  if (full_unit_path)
    free (full_unit_path);
  return result;
}

/* Open a gpi file for reading and process the header and trailer.
 * Do an AutoMake, if necessary and/or requested.
 * The file is closed via a normal fclose().
 */
static MEMFILE *
gpi_open (interface_name, name, source, depth)
     tree interface_name;
     char *name;
     char *source;
     int depth;
{
  MEMFILE *s = NULL;
  char *temp_name;
  temp_name = locate_file (name, unit_path);
  if (! temp_name)
    temp_name = locate_file (name, unit_destination_path);
  if (temp_name)
    {
      name = temp_name;
      s = mopen (name, "rb");
    }
  else
    name = save_string (name);
  if (s)
    {
      char gpi_header [34], *source_name, *module_name, *module_filename;
      tree import_list = NULL_TREE, imported;
      int must_recompile = 0, done;
      unsigned length;
      long end_of_header;

      /* Process the header.
       */
      if (mread (gpi_header, 33, 1, s) != 1)
        gpi_error ();
      gpi_header [33] = 0;
      if (flag_debug_gpi)
        fprintf (stderr, "loaded GPI header: %s", gpi_header);
      module_name = load_string (s);
      module_filename = load_string (s);
      if (flag_debug_gpi)
        {
          fprintf (stderr, "loaded Module name: %s\n",
                           module_name);
          fprintf (stderr, "loaded Module source file name: %s\n",
                           module_filename);
          fprintf (stderr, "loaded names of imported Interfaces:\n");
        }
      new_interface_table (interface_name, get_identifier (module_name), 1);
      source_name = locate_interface_source (name, source, module_filename);
      if (! source_name)
        source_name = save_string (module_filename);
      do
        {
          /* Store names of imported Interfaces for later use.
           */
          char *gpi_loaded_string = load_string (s);
          done = gpi_loaded_string [0] == 0;
          if (!done)
            {
              tree interface = get_identifier (gpi_loaded_string);
              if (flag_debug_gpi)
                fprintf (stderr, "  %s\n", gpi_loaded_string);
              for (imported = current_module->imports; imported;
                   imported = TREE_CHAIN (imported))
                if (interface == TREE_VALUE (imported))
                  break;
              if (! imported)
                for (imported = import_list; imported;
                     imported = TREE_CHAIN (imported))
                  if (interface == TREE_VALUE (imported))
                    break;
              if (! imported)
                import_list = chainon (import_list,
                                       build_tree_list (NULL_TREE, interface));
            }
          free (gpi_loaded_string);
        }
      while (!done);
      if (flag_debug_gpi)
        fprintf (stderr, "loaded names of files to be linked:\n");
      do
        {
          /* Copy names of files to be linked to the AutoMake temp file.
           */
          char *gpi_loaded_string = load_string (s);
          done = gpi_loaded_string [0] == 0;
          if (gpi_loaded_string [0] == '-')
            {
              if (flag_debug_gpi)
                fprintf (stderr, "  %s\n", gpi_loaded_string);
              add_to_link_file_list (gpi_loaded_string);
            }
          else if (!done)
            {
              char *object_file_name;
              if (flag_debug_gpi)
                fprintf (stderr, "  %s\n", gpi_loaded_string);
              object_file_name = locate_file (gpi_loaded_string, unit_path);
              if (object_file_name == NULL_PTR)
                object_file_name = locate_file (gpi_loaded_string, unit_destination_path);
              if (object_file_name == NULL_PTR)
                object_file_name = locate_file (gpi_loaded_string, object_path);
              if (object_file_name == NULL_PTR)
                object_file_name = locate_file (gpi_loaded_string, object_destination_path);
              if (object_file_name == NULL_PTR)
                {
                  if (flag_automake && depth == 0)
                    must_recompile = 1;
                  else if (flag_automake > 0 && /*@@ linking*/0)
                    error ("object file `%s' not found", gpi_loaded_string);
                }
              else
                {
                  add_to_link_file_list (object_file_name);
                  free (object_file_name);
                }
            }
          free (gpi_loaded_string);
        }
      while (!done);
      end_of_header = mtell (s);

      /* Process the trailer.
       */
      mseek (s, - sizeof (length), SEEK_END);
      if (mread (&length, sizeof (length), 1, s) != 1)
        gpi_error ();
      if (length)
        {
          tree import_list = NULL_TREE;

          mseek (s, - (length + sizeof (length)), SEEK_END);

          if (flag_debug_gpi)
            fprintf (stderr, "loaded additional names of imported Interfaces:\n");
          do
            {
              /* Store names of imported Interfaces for later use.
               */
              char *gpi_loaded_string = load_string (s);
              done = gpi_loaded_string [0] == 0;
              if (!done)
                {
                  tree interface = get_identifier (gpi_loaded_string);
                  if (flag_debug_gpi)
                    fprintf (stderr, "  %s\n", gpi_loaded_string);
                  for (imported = current_module->imports; imported;
                       imported = TREE_CHAIN (imported))
                    if (interface == TREE_VALUE (imported))
                      break;
                  if (! imported)
                    for (imported = import_list; imported;
                         imported = TREE_CHAIN (imported))
                      if (interface == TREE_VALUE (imported))
                        break;
                  if (! imported)
                    import_list = chainon (import_list,
                                           build_tree_list (NULL_TREE, interface));
                }
              free (gpi_loaded_string);
            }
          while (!done);
          if (flag_debug_gpi)
            fprintf (stderr, "loaded additional names of files to be linked:\n");
          do
            {
              /* Copy names of files to be linked to the AutoMake temp file.
               */
              char *gpi_loaded_string = load_string (s);
              done = gpi_loaded_string [0] == 0;
              if (gpi_loaded_string [0] == '-')
                {
                  if (flag_debug_gpi)
                    fprintf (stderr, "  %s\n", gpi_loaded_string);
                  add_to_link_file_list (gpi_loaded_string);
                }
              else if (!done)
                {
                  char *object_file_name;
                  if (flag_debug_gpi)
                    fprintf (stderr, "  %s\n", gpi_loaded_string);
                  object_file_name = locate_file (gpi_loaded_string, unit_path);
                  if (object_file_name == NULL_PTR)
                    object_file_name = locate_file (gpi_loaded_string, unit_destination_path);
                  if (object_file_name == NULL_PTR)
                    object_file_name = locate_file (gpi_loaded_string, object_path);
                  if (object_file_name == NULL_PTR)
                    object_file_name = locate_file (gpi_loaded_string, object_destination_path);
                  if (object_file_name == NULL_PTR)
                    {
                      if (flag_automake && depth == 0)
                        must_recompile = 1;
                      else if (flag_automake > 0 && /*@@ linking*/0)
                        error ("object file `%s' not found", gpi_loaded_string);
                    }
                  else
                    {
                      add_to_link_file_list (object_file_name);
                      free (object_file_name);
                    }
                }
              free (gpi_loaded_string);
            }
          while (!done);
        }

      /* Record modules used by the one being imported as
       * "implementation imports" for the one being compiled.
       * Also mark it as being imported implicitly.
       */
      for (imported = import_list; imported; imported = TREE_CHAIN (imported))
        {
          TREE_STATIC (imported) = 1;
          TREE_PRIVATE (imported) = 1;
        }
      current_module->imports = chainon (current_module->imports, import_list);

      /* Do an AutoMake, if necessary and/or requested.
       */
      if (flag_automake && depth == 0)
        {
          if (! must_recompile)
            {
              /* Check modules used by the one being imported.
               */
              MEMFILE *u;
              tree i_name;
              char *u_name, *p;

              for (imported = import_list; imported && ! must_recompile;
                   imported = TREE_CHAIN (imported))
                {
                  i_name = TREE_VALUE (imported);
                  u_name = concat (IDENTIFIER_POINTER (i_name),
                                   ".gpi", NULL_PTR);
                  for (p = u_name; *p; p++)
                    *p = tolower (*p);
                  u = gpi_open (interface_name, u_name, NULL, 0);
                  if (u)
                    mclose (u);
                  else
                    must_recompile = 1;
                  free (u_name);
                }
            }
          if (flag_automake > 1
              && (must_recompile
                  || module_must_be_recompiled (interface_name, name,
                                                source_name, import_list)))
            {
              int result;
              mclose (s);
              s = NULL;
              if (flag_debug_gpi)
                fprintf (stderr, "recompiling: %s --> %s\n",
                                 module_filename, name);
              result = compile_module (source_name, unit_destination_path);
              if (result == 0)
                {
                  /* Module has been compiled.  Reload the GPI file.
                   */
                  s = gpi_open (interface_name, name, source, depth + 1);
                }
              else
                {
                  char *p;
                  /* Error.  To produce a nice message, cut the extension `.gpi'.
                   */
                  p = module_filename + strlen (module_filename);
                  do
                    p--;
                  while (p >= module_filename && *p != '.');
                  *p = 0;
                  fatal ("module/unit `%s' could not be compiled", module_filename);
                }
            }
          else
            mseek (s, end_of_header, SEEK_SET);
        }
      else
        mseek (s, end_of_header, SEEK_SET);
      if (source_name)
        free (source_name);
      free (module_name);
      free (module_filename);
    }
  else if (flag_automake > 1 && depth == 0)  /* not with --autolink */
    {
      /* GPI file not found.  Try to compile the module by using the
       * name given explicitly in SOURCE or by deriving the file name
       * from the interface name.
       */
      char *source_name = locate_interface_source (name, source, NULL_PTR);
      int result = -1;
      if (source_name)
        result = compile_module (source_name, unit_destination_path);
      if (result == 0)
        {
          /* Module has been compiled.
           * Try again to load the GPI file.
           */
          s = gpi_open (interface_name, name, source, depth + 1);
        }
      else if (source)
        {
          if (flag_automake > 1)
            fatal ("module/unit `%s' could not be compiled", source);
        }
      else
        {
          int l = strlen (name);
          char *p, *module_filename = (char *) xmalloc (l + 1);
          strcpy (module_filename, name);
          /* Cut the extension `.gpi' */
          p = module_filename + l;
          do
            p--;
          while (p >= module_filename && *p != '.');
          if (p >= module_filename)  /* paranoia check */
            *p = 0;
          if (flag_automake > 1)
            fatal ("module/unit `%s' could not be compiled", module_filename);
          free (module_filename);
        }
      if (source_name)
        free (source_name);
    }
  if (name)
    free (name);
  return s;
}

/* Try to load a GPI file and to extract an exported module interface.
 */
tree
load_gpi_file (interface_name, name, source)
     tree interface_name;
     char *name;
     char *source;
{
  MEMFILE *gpi_file;
  char *p;
  tree export;
  tree we_were_loading_a_gpi_file = we_are_loading_a_gpi_file;

  we_are_loading_a_gpi_file = interface_name;

  current_gpi_file_name = concat (name, ".gpi", NULL_PTR);
  for (p = current_gpi_file_name; *p; p++)
    *p = tolower (*p);
  gpi_file = gpi_open (interface_name, current_gpi_file_name, source, 0);
  if (gpi_file)
    {
      tree temp, value, purpose;
      load_module_header (gpi_file);
      temp = load_tree (gpi_file, 1);
      value = TREE_VALUE (temp);
      purpose = TREE_CHAIN (temp);
      mclose (gpi_file);
      export = build_tree_list (purpose, value);
    }
  else
    export = NULL_TREE;
  clean_imbed_file_obstack ();
  we_are_loading_a_gpi_file = we_were_loading_a_gpi_file;
  free (current_gpi_file_name);
  current_gpi_file_name = NULL_PTR;
  return export;
}

/* Try to load a GPM file and to extract the interface part of this Module.
 */
int
load_gpm_file ()
{
  FILE *gpm_file;
  int gpm_line_size = 256;
  char *gpm_line = (char *) xmalloc (gpm_line_size), *gpm_file_name, *temp_name, *p;

  gpm_file_name = concat (IDENTIFIER_POINTER (current_module->name),
                          ".gpm", NULL_PTR);
  for (p = gpm_file_name; *p; p++)
    *p = tolower (*p);

  temp_name = locate_file (gpm_file_name, unit_path);
  if (! temp_name)
    temp_name = locate_file (gpm_file_name, unit_destination_path);
  if (temp_name)
    {
      free (gpm_file_name);
      gpm_file_name = temp_name;
    }

  if (! gpm_file_name)
    return -1;

  gpm_file = fopen (gpm_file_name, "rb");

  if (! gpm_file)
    return -1;

  fread (gpm_line, 18, 1, gpm_file);
  gpm_line [18] = 0;
  if (flag_debug_gpi)
    fprintf (stderr, "loaded GPM header: %s", gpm_line);
  do
    {
      tree exported;
      unsigned char l;

      fread (&l, sizeof (unsigned char), 1, gpm_file);

      if (l >= gpm_line_size)
        gpm_line = (char *) xrealloc (gpm_line, (gpm_line_size = l + 1));
      if (l > 0)
        fread (gpm_line, l, 1, gpm_file);
      gpm_line [l] = 0;

      if (gpm_line [0])
        {
          tree interface_name = get_identifier (gpm_line);
          if (flag_debug_gpi)
            fprintf (stderr, "loaded Interface name: %s\n", gpm_line);
          exported = load_gpi_file (interface_name, gpm_line, NULL);
          if (exported)
            {
              tree ename;
              for (ename = TREE_PURPOSE (exported); ename;
                   ename = TREE_CHAIN (ename))
                {
                  /* "Activate" all names.
                   */
                  if (TREE_VALUE (ename))
                    IDENTIFIER_LOCAL_VALUE (TREE_VALUE (ename)) =
                        IDENTIFIER_LIMBO_VALUE (TREE_VALUE (ename));
                }
              export_interface (interface_name, TREE_PURPOSE (exported));
            }
        }
    }
  while (gpm_line [0]);
  free (gpm_line);
  fclose (gpm_file);
  return 0;
}



/* Import an interface of a unit or module.  Look up and read the
 * GPI file and import either everything or only the requested parts
 * exported by the interface.
 *
 * INTERFACE is an IDENTIFIER_NODE of the interface name.
 *
 * IMPORT_QUALIFIER:
 *     NULL_TREE if no qualifiers given.
 *     TREE_LIST:
 *       TREE_PURPOSE:
 *           NULL_TREE  -> no restricted_import_option given
 *           !NULL_TREE -> restricted_import_option (== `only') given
 *       TREE_VALUE:
 *           TREE_LIST
 *             TREE_PURPOSE: imported <name> from the interface
 *             TREE_VALUE:
 *                 NULL_TREE: name has not been renamed
 *                 identifier_node: new name of the renamed <name>
 *
 * QUALIFIED_IMPORT is 0 if unqualified references are allowed;
 *                     1 if qualified references are mandatory.
 *
 * UNIT_FILENAME is an optional IDENTIFIER_NODE holding the name of the
 * source file of this unit.  (Not used with modules.)
 */
void
import_interface (interface, import_qualifier, qualified_import, unit_filename)
     tree interface;
     tree import_qualifier;
     long qualified_import;
     tree unit_filename;
{
  tree exported_name_list;
  tree imported, exported;

  /* Handle a special case of a circular dependency.
   */
  if (interface == current_module->name)
    {
      if (current_module->main_program)
        error ("program trying to import itself like a module or unit");
      else
        error ("self-dependent module or unit");
      return;
    }

  for (imported = current_module->imports; imported;
       imported = TREE_CHAIN (imported))
    if (interface == TREE_VALUE (imported))
      {
        if (TREE_PRIVATE (imported))
          break;
        else
          {
            error ("interface `%s' already has been imported",
                   IDENTIFIER_POINTER (interface));
            return;
          }
      }

  /* Mark whether this interface was imported in the interface or
   * implementation part.
   */
  if (! imported)
    {
      imported = build_tree_list (unit_filename, interface);
      current_module->imports = chainon (current_module->imports, imported);
    }
  TREE_STATIC (imported) = ! this_is_an_interface_module;
  TREE_PRIVATE (imported) = 0;

  if (interface == standard_interface_input)
    {
      current_module->input_file_node = global_input_file_node;
      exported_name_list = build_tree_list (NULL_TREE, identifier_input);
    }
  else if (interface == standard_interface_output)
    {
      current_module->output_file_node = global_output_file_node;
      exported_name_list = build_tree_list (NULL_TREE, identifier_output);
    }
  else if (interface == standard_interface_error)
    {
      if (pedantic || flag_what_pascal)
        warning ("ISO Pascal does not define `StandardError'");
      current_module->error_file_node = global_error_file_node;
      exported_name_list = build_tree_list (NULL_TREE, identifier_stderr);
    }
  else
    {
      for (exported = exported_interface_list;
           exported;
           exported = TREE_CHAIN (exported))
        if (TREE_VALUE (exported) == interface)
          break;

      if (! exported)
        {
          char *gpi_source;
          if (unit_filename)
            gpi_source = TREE_STRING_POINTER (unit_filename);
          else
            gpi_source = NULL;
          exported = load_gpi_file (interface, IDENTIFIER_POINTER (interface),
                                    gpi_source);
          if (exported)
            {
              exported_interface_list = chainon (exported_interface_list,
                                                 exported);
              internal_exported_interface_list = chainon (internal_exported_interface_list,
                                                          build_tree_list (NULL_TREE, interface));
            }
          else
            {
              fatal ("module/unit interface `%s' could not be imported",
                     IDENTIFIER_POINTER (interface));
              return;
            }
        }

      exported_name_list = TREE_PURPOSE (exported);
    }

  if (qualified_import)
    warning ("`qualified' not yet supported - ignored");

  /* EXPORTED is now the correct interface.
   */

  if (import_qualifier)
    {
      tree iname;
      for (iname = TREE_VALUE (import_qualifier); iname;
           iname = TREE_CHAIN (iname))
        {
          tree ename = exported_name_list;

          while (ename && TREE_VALUE (ename) != TREE_PURPOSE (iname))
            ename = TREE_CHAIN (ename);

          if (ename && TREE_VALUE (ename) == TREE_PURPOSE (iname))
            {
              tree local;
              /* Activate this name and resolve import/export renaming.
               */
              if (TREE_VALUE (iname))  /* import renaming */
                local = TREE_VALUE (iname);
              else if (TREE_PURPOSE (ename))  /* export renamimg */
                local = TREE_PURPOSE (ename);
              else  /* no renaming */
                local = TREE_VALUE (ename);
              IDENTIFIER_LOCAL_VALUE (local) =
                  IDENTIFIER_LIMBO_VALUE (TREE_VALUE (ename));
              if (TREE_READONLY (TREE_VALUE (ename)))
                TREE_READONLY (IDENTIFIER_LOCAL_VALUE (local)) = 1;
              IDENTIFIER_LIMBO_VALUE (TREE_VALUE (ename)) = NULL_TREE;
            }
          else
            error ("interface `%s' does not export `%s'",
                   IDENTIFIER_POINTER (interface),
                   IDENTIFIER_POINTER (TREE_PURPOSE (iname)));
        }
    }

  if (import_qualifier == NULL_TREE  /* without qualifiers */
      || TREE_PURPOSE (import_qualifier) == NULL_TREE)  /* without `only' */
    {
      tree ename;
      for (ename = exported_name_list; ename; ename = TREE_CHAIN (ename))
        {
          /* "Activate" all remaining names and resolve export renaming.
           */
          if (IDENTIFIER_LIMBO_VALUE (TREE_VALUE (ename)))
            {
              tree local;
              if (TREE_PURPOSE (ename))
                local = TREE_PURPOSE (ename);
              else
                local = TREE_VALUE (ename);
              IDENTIFIER_LOCAL_VALUE (local) =
                  IDENTIFIER_LIMBO_VALUE (TREE_VALUE (ename));
              if (TREE_READONLY (TREE_VALUE (ename)))
                TREE_READONLY (IDENTIFIER_LOCAL_VALUE (local)) = 1;
              IDENTIFIER_LIMBO_VALUE (TREE_VALUE (ename)) = NULL_TREE;
            }
        }
    }
}

/* Return a TREE_LIST to be chained to an interface export list
 * containing the name NAME (an IDENTIFIER_NODE) of the thing to be
 * exported plus, if given, the qualifiers RENAMED (an IDENTIFIER_NODE
 * holding the new name) and PROTECTED.
 *
 * We use TREE_READONLY of an IDENTIFIER_NODE to indicate `protected'.
 *
 * Only exportable names allowed as the NAME here:
 *        constant_name
 *      | type_name
 *      | schema_name
 *      | procedure_name
 *      | function_name
 *      | variable_name
 *      | `protected' variable_name
 */
tree
module_export_clause (name, renamed, protected)
     tree name;
     tree renamed;
     int  protected;
{
  tree rval = build_tree_list (renamed, name);
  TREE_READONLY (name) = protected;
  return rval;
}

/* Export a range of an enumerated type.  These need special care
 * because they contain identifiers in between which must be exported
 * as well.
 */
tree
module_export_range (low, high)
     tree low;
     tree high;
{
  return module_export_clause (build_tree_list (low, high), 0, 0);
}

/* Replace the exported TREE_LISTs denoting ranges by
 * the actual LEX_ID nodes.  Exported enumerated types
 * are handled here, too.
 */
void
module_repair_exported_ranges ()
{
  tree escan;
  for (escan = current_module->exports; escan; escan = TREE_CHAIN (escan))
    {
      tree nscan = TREE_PURPOSE (escan);
      while (nscan)
        {
          tree exported = NULL_TREE;
          tree meaning;

          if (TREE_VALUE (nscan)
              && TREE_CODE (TREE_VALUE (nscan)) == IDENTIFIER_NODE
              && (meaning = lookup_name (TREE_VALUE (nscan))) != NULL_TREE
              && TREE_CODE (meaning) == TYPE_DECL  /* @@@ or better always? */
              && TREE_TYPE (meaning)
              && TREE_CODE (TREE_TYPE (meaning)) == ENUMERAL_TYPE)
            {
              /* "Repair" exported enumerated types here.
               */
              tree item;
              for (item = TYPE_VALUES (TREE_TYPE (meaning)); item;
                   item = TREE_CHAIN (item))
                {
                  exported = chainon (exported,
                                      build_tree_list (NULL_TREE,
                                                       TREE_PURPOSE (item)));
                }
            }

          else if (TREE_VALUE (nscan)
                   && TREE_CODE (TREE_VALUE (nscan)) == TREE_LIST)
            {
              tree low = TREE_PURPOSE (TREE_VALUE (nscan));
              tree high = TREE_VALUE (TREE_VALUE (nscan));
              tree type = TREE_TYPE (lookup_name (low));

              if (TREE_CODE (type) != ENUMERAL_TYPE
                  || TREE_TYPE (lookup_name (high)) != type)
                abort ();
              else
                {
                  tree item;
                  int export_it = 0;
                  for (item = TYPE_VALUES (type); item;
                       item = TREE_CHAIN (item))
                    {
                      if (TREE_PURPOSE (item) == low)
                        export_it = 1;
                      if (export_it)
                        exported = chainon (exported,
                                     build_tree_list (NULL_TREE,
                                                      TREE_PURPOSE (item)));
                      if (TREE_PURPOSE (item) == high)
                        {
                          if (! export_it)
                            error ("wrong order in exported range");
                          export_it = 0;
                        }
                    }
                }

            }
          /* Chain EXPORTED to NSCAN.
           * A TREE_LIST at NSCAN will be removed later.
           */
          if (exported)
            {
              tree new_nscan = exported;
              while (TREE_CHAIN (new_nscan))
                new_nscan = TREE_CHAIN (new_nscan);
              TREE_CHAIN (nscan) = chainon (exported, TREE_CHAIN (nscan));
              nscan = new_nscan;
            }
          nscan = TREE_CHAIN (nscan);
        }

      /* Remove the TREE_LISTs from the chain of exported names.
       */
      for (nscan = TREE_PURPOSE (escan); nscan; nscan = TREE_CHAIN (nscan))
        if (TREE_VALUE (nscan)
            && TREE_CODE (TREE_VALUE (nscan)) == TREE_LIST)
          TREE_VALUE (nscan) = TREE_PURPOSE (nscan) = NULL_TREE;
    }
}
