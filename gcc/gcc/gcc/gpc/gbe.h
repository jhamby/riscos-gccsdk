/* gbe.h -- prototypes for GNU back end functions used by GNU Pascal.
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
along with GNU Pascal; see the file COPYING. If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. */

/*
 * Author:  J.J. van der Heijden <J.J.vanderHeijden@student.utwente.nl>
 */

#ifndef _GBE_H_
#define _GBE_H_

#include <stdio.h>

#include "tree.h"
#include "gpc-tree.h"
#include "input.h"
#include "rtl.h"
#include "obstack.h"
#include "flags.h"
#include "output.h"
#include "expr.h"

/* dbxout.c */
extern void dbxout_set_type_status PROTO ((tree, int)); /* GPC extension */

/* expmed.c */
extern void expand_inc PROTO ((rtx, rtx));
extern void expand_dec PROTO ((rtx, rtx));

/* function.c */
extern rtx nonlocal_goto_handler_slot;
extern rtx nonlocal_goto_stack_level;
extern tree nonlocal_labels;
extern struct function *maybe_find_function_data PROTO ((tree)); /* GPC extension */

/* print-tree.c */
extern void debug_tree PROTO ((tree node));

/* stmt.c */
extern rtx tail_recursion_reentry;
extern void emit_jump PROTO ((rtx));
extern void emit_nop PROTO ((void));
extern void expand_computed_goto PROTO ((tree));
extern rtx label_rtx PROTO ((tree));

/* stor-layout.c */
/* GPC extension: Nonzero means that the size of a type
 * may vary within one function context. */
extern int size_volatile;

/* toplev.c */
extern char **save_argv;
extern char *progname;
extern int version_flag;
extern int errorcount;
extern int sorrycount;
#ifdef EGCS92
# include <toplev.h>
#else
extern void pedwarn PVPROTO ((char *, ...));
extern void warning PVPROTO ((char *, ...));
extern void error PVPROTO ((char *, ...));
extern void fatal PVPROTO ((char *, ...));
extern void error_with_decl PVPROTO((tree, char *, ...));
extern void rest_of_decl_compilation PROTO ((tree, char *, int, int));
extern void warning_with_file_and_line PVPROTO ((char *, int, char *, ...));
#endif

/* tree.c */
extern struct obstack permanent_obstack;
extern int int_fits_type_p PROTO ((tree, tree));
extern int really_constant_p PROTO ((tree));
extern void pop_momentary_nofree PROTO ((void));
extern void change_main_variant PROTO ((tree, tree));

/*
 * GCC files not really part of the GBE.
 */

/* choose-temp.c */
extern char *choose_temp_base PROTO ((void));

/* pexecute.c */
extern int pwait PROTO ((int, int *, int));
extern int pexecute PROTO ((const char *, char * const *, const char *,
                            const char *, char **, char **, int));

#endif /* _GBE_H_ */
