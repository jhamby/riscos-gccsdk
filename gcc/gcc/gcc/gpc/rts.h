/* rts.h -- interface for rts.c
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
      rts.c

   Modifications:
*/

/* Allow multiple inclusion to work. */

#ifndef _GPC_RTS_CALL_H_
#define _GPC_RTS_CALL_H_

/* Include files needed by this one. */


/* Structure definitions. */

struct rts_symbol
{
  int val;
  char *name;
  rtx symref;
  tree fun;
  int io_critical;
};

/* Simple definitions and enumerations. */


/* Typedefs. */


/* Include files needed by this one. */


/* Structure definitions. */


/* Global objects accessed by users of this module. */
extern int allow_packed_var_parameters;
extern struct rts_symbol rts[];

extern tree ptype_void;
extern tree ptype_int;
extern tree ptype_longint;
extern tree ptype_char;
extern tree ptype_bool;
extern tree ptype_double;
extern tree ptype_long_double;
extern tree ptype_complex;
extern tree ptype_cstring;
extern tree ptype_string_schema;
extern tree ptype_const_string_schema;
extern tree ptype_vararg;

/* RTS calls */
extern tree do_ptype PROTO ((tree, int, int));
extern void declare_rts_types PROTO ((void));
extern tree rts_call PROTO ((int, tree, tree, tree));
extern tree build_rts_call PROTO ((int, register tree));

/* File I/O */
extern tree build_buffer_ref PROTO ((tree, int));
extern tree get_standard_input PROTO ((void));
extern tree get_standard_output PROTO ((void));
extern tree get_standard_error PROTO ((void));

/* Miscellaneous */
extern tree get_builtin_variable PROTO ((char *, tree));

/* Declare functions with prototypes. */


/* Define macros. */

/* End of #include file. */

#endif /* _GPC_RTS_CALL_H_ */
