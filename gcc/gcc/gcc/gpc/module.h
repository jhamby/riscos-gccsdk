/* module.h -- Interface to module.c
   Copyright (C) 1997-2000 Free Software Foundation, Inc.

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
02111-1307, USA.

   Owning Modules:
      module.c

   Modifications:
*/

/* Allow multiple inclusion to work. */

#ifndef _GPC_MODULE_H_
#define _GPC_MODULE_H_

/* Include files needed by this one. */

#include "memfiles.h"

/* Simple definitions and enumerations. */

#define NULL_MODULE ((module_t) 0)

/* Flag arguments to pexecute.  */
#define PEXECUTE_FIRST   1
#define PEXECUTE_LAST    2
#define PEXECUTE_SEARCH  4
#define PEXECUTE_VERBOSE 8

#ifndef WIFSIGNALED
#define WIFSIGNALED(S) (((S) & 0xff) != 0 && ((S) & 0xff) != 0x7f)
#endif
#ifndef WTERMSIG
#define WTERMSIG(S) ((S) & 0x7f)
#endif
#ifndef WIFEXITED
#define WIFEXITED(S) (((S) & 0xff) == 0)
#endif
#ifndef WEXITSTATUS
#define WEXITSTATUS(S) (((S) & 0xff00) >> 8)
#endif

/* By default there is no special suffix for executables.  */
#ifdef EXECUTABLE_SUFFIX
#define HAVE_EXECUTABLE_SUFFIX
#else
#define EXECUTABLE_SUFFIX ""
#endif
#if defined(__DJGPP__)
/* Do not convert the output filename on DJGPP, since it
   will break many makefiles, which assume, that gcc produces
   exactly the file, which was given as output file. And since
   GNU make for DJGPP (which is the only make used) can run
   also the programs without the .exe suffix this not a problem)
*/
#undef HAVE_EXECUTABLE_SUFFIX
#endif

/* By default, the suffix for object files is ".o".  */
#ifdef OBJECT_SUFFIX
#define HAVE_OBJECT_SUFFIX
#else
#define OBJECT_SUFFIX ".o"
#endif

/* Typedefs. */

/* A structure to protocol object and library files we intend to link */

typedef struct string_list
{
  char *string;
  struct string_list *next;
} string_list;

typedef struct module *module_t;

/* Structure definitions. */

struct module
{
  /* TREE_LIST of imported interfaces for this module.
   * TREE_VALUE is an IDENTIFIER_NODE of an imported interface name.
   * TREE_PURPOSE is TREE_LIST chain of the names imported by this
   * interface.
   */
  tree imports;

  /* TREE_LIST of interfaces exported from this module TREE_VALUE is
   * an IDENTIFIER_NODE of an exported interface name.
   * TREE_PURPOSE is TREE_LIST chain of the names exported by this
   * module.
   */
  tree exports;

#if 0
  /* Unexported declarations made in the interface part/module.
   */
  tree pending_decls;
#endif

  /* The name of this module.
   */
  tree name;

  /* Module parameters.
   */
  tree parms;

  /* What shall be exported automatically?
   */
  tree autoexport;

  /* Nonzero if this module has an implementation part
   * (used for error checking only).
   */
  int implementation;

  /* Nonzero if this module has an interface part
   * (used for error checking only).
   */
  int interface;

  /* Files this module wishes to be linked.
   */
  string_list *link_files;

  /* Standard output for this module.
   */
  tree output_file_node;

  /* Standard input for this module.
   */
  tree input_file_node;

  /* Standard error device for this module (GPC extension).
   */
  tree error_file_node;

  /* Nonzero if this is the main program.
   */
  int main_program;

  /* Next module.
   */
  module_t next;
};


/* Global objects accessed by users of this module. */

#ifdef NEED_DECLARATION_FREE
extern void free ();
#endif

#ifdef HAVE_STRERROR
extern char *strerror();
#else
extern int sys_nerr;
extern char *sys_errlist[];
#endif

extern module_t current_module;
extern string_list *pending_link_files;
extern tree standard_interface_input;
extern tree standard_interface_output;
extern tree standard_interface_error;
extern tree export_all;
extern int flag_debug_gpi;
extern int flag_automake;
extern char* automake_gpc;
extern char* automake_gpc_options;
extern char* automake_temp_filename;
extern char *unit_path;
extern char *object_path;
extern tree we_are_loading_a_gpi_file;
extern char *executable_file_name;


/* Declare functions with prototypes. */

extern void associate_external_objects PROTO ((tree));
extern void check_external_objects PROTO ((tree));
extern tree module_export_clause PROTO ((tree,tree,int));
extern tree module_export_range PROTO ((tree,tree));
extern void module_repair_exported_ranges PROTO ((void));
extern int name_exported_p PROTO ((tree));
extern void handle_autoexport PROTO ((tree));
extern void add_automake_gpc_options PROTO ((char*));
extern void add_to_link_file_list PROTO ((char*));
extern char* locate_file PROTO ((char *, char *));
extern void add_to_automake_temp_file PROTO ((char *));
extern void store_executable_name PROTO((void));
extern int compile_module PROTO ((char *, char*));
extern tree itab_get_module_name PROTO ((tree));
extern void store_tree PROTO ((tree, FILE *, int));
extern tree load_tree PROTO ((MEMFILE *, int));
extern char *my_strerror PROTO ((int));
extern tree load_gpi_file PROTO ((tree, char *, char *));
extern int load_gpm_file PROTO ((void));
extern void create_gpm_file PROTO ((void));
extern void create_gpi_files PROTO ((void));
extern void extend_gpi_files PROTO ((void));
extern tree maybe_make_static PROTO ((tree));
extern void initialize_module PROTO ((tree, int));
extern void finalize_module PROTO ((struct module *));
/* extern void handle_pending_decls PROTO ((void)); */
extern void import_interface PROTO ((tree,tree,long,tree));
extern void export_interface PROTO ((tree,tree));
extern void do_extra_import PROTO ((void));


/* Define macros. */


/* End of #include file. */

#endif /* _GPC_MODULE_H_ */
