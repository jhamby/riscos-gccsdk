/* types.h -- interface to types.c
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

*/

#ifndef _GPC_TYPES_H_
#define _GPC_TYPES_H_

/* Include files needed by this one. */


/* Structure definitions. */


/* Global objects accessed by users of this module. */

extern void cstring_inform PROTO ((void));
extern void ptrarith_inform PROTO ((void));

/* Declare functions with prototypes. */

/* Sets */
extern tree construct_set_member PROTO ((tree, tree));
extern tree build_set_constructor PROTO ((tree));
extern tree construct_set PROTO ((tree, tree, int));
extern tree check_set_bounds PROTO ((tree, tree));
extern tree build_set_type PROTO ((tree));
extern tree assign_set PROTO ((tree, tree));

/* Strings */
extern void assign_string PROTO ((tree, const tree));
extern tree append_null_character PROTO ((tree));
extern tree string_may_be_char PROTO ((tree));
extern tree build_pascal_string_schema PROTO ((tree));
extern int is_string_type PROTO ((tree, int));
extern int is_of_string_type PROTO ((tree, int));
extern int declared_string_capacity PROTO ((tree));
extern int is_variable_string_type PROTO ((tree));

/* Schemata */
extern tree maybe_schema_discriminant PROTO ((tree));
extern tree build_schema_type PROTO ((tree, tree));
extern int number_of_schema_discriminants PROTO ((tree));
extern int is_schema_discriminant_access PROTO ((tree, tree));
extern void prediscriminate_schema PROTO ((tree, int));

/* Other Pascal types */
extern tree base_type PROTO ((tree));
extern tree build_file_type PROTO ((tree, tree));
extern tree build_record_variant_part PROTO ((char *, int, tree, tree));
extern tree grok_packed PROTO ((tree));
extern int is_packed_field PROTO ((tree));
extern tree expand_packed_field PROTO ((tree, tree));
extern tree count_bits PROTO ((tree));
extern tree p_grokfields PROTO ((tree, tree));
extern tree pascal_array_type_nelts PROTO ((tree));
extern tree object_size PROTO ((tree));
extern int int_cst_lt_always PROTO ((tree, tree));
extern tree select_signed_integer_type PROTO ((tree));
extern tree select_integer_type PROTO ((tree, tree, enum tree_code));
extern tree build_pascal_range_type PROTO ((tree, tree));
extern tree build_pascal_pointer_type PROTO ((tree, int));
extern tree build_pascal_array_type PROTO ((tree, tree));
extern tree build_pascal_array_ref PROTO ((tree, tree));
extern tree make_signed_range PROTO ((tree, tree));
extern tree convert_type_to_range PROTO ((tree));

#endif /* _GPC_TYPES_H_ */
