/* util.h -- interface for util.c
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
      util.c

   Modifications:
*/

/* Allow multiple inclusion to work. */

#ifndef _GPC_UTIL_H_
#define _GPC_UTIL_H_

/* Include files needed by this one. */


/* Structure definitions. */


/* Simple definitions and enumerations. */

#define VQ_EXTERNAL   0x01
#define VQ_STATIC     0x02
#define VQ_VOLATILE   0x04
#define VQ_REGISTER   0x08
#define VQ_TYPEDCONST 0x10

/* Typedefs. */


/* Include files needed by this one. */


/* Structure definitions. */


/* Global objects accessed by users of this module. */

extern char *gpc_main;
extern int flag_interface_only;
extern int flag_implementation_only;
extern int warn_field_name_problem;
extern int warn_typed_const;
extern int warn_near_far;
extern int main_program_has_nonlocal_label;
extern int main_program_needs_context;
extern int main_program_first_label_num;
extern tree main_program_named_labels;
extern tree main_program_nonlocal_labels;
#if 0
extern rtx main_program_tail_recursion_reentry;
extern rtx main_program_nonlocal_goto_handler_slot;
extern rtx main_program_nonlocal_goto_stack_level;
#endif
extern char *executable_file_name;
extern char *executable_path;
extern char *unit_destination_path;
extern char *object_destination_path;
extern int flag_no_default_paths;
extern char *gpi_destination_path;
extern char *extra_imports;
extern int requested_set_size;
extern tree integer_set_size;
extern int int_out_width;
extern int real_out_width;
extern int bool_out_width;
extern int long_int_out_width;
extern int long_real_out_width;
extern tree integer_set_type_node;
extern tree integer_ptr_type_node;
extern tree size_of_file_type;
#ifdef EGCS
extern tree size_unit_of_file_type;
#endif
extern tree boolean_type_node;
extern tree text_type_node;
extern tree complex_type_node;
extern tree identifier_input;
extern tree identifier_output;
extern tree identifier_stderr;
extern tree global_input_file_node;
extern tree global_output_file_node;
extern tree global_error_file_node;
extern tree char_array_index_type_node;
extern tree string_schema_proto_type;
extern tree const_string_schema_proto_type;
extern tree cstring_type_node;
extern tree empty_arglist;

extern tree gpc_type_TIME;
extern tree gpc_type_DATE;
extern tree gpc_type_TIMESTAMP;
extern tree gpc_type_BINDINGTYPE;

extern tree string_id;
extern tree schema_id;
extern tree const_id;
extern tree type_id;
extern tree inline_id;
extern tree varparm_id;
extern tree volatile_id;
extern tree register_id;
extern tree auto_id;
extern tree extern_id;
extern tree asmname_id;
extern tree static_id;
extern tree attribute_id;

extern tree collect_constructors_now;


/* Declare functions with prototypes. */

/* Identifiers */
/* Note: The first parameter to get_unique_identifier() must not be longer than 200 chars! */
extern tree get_unique_identifier PROTO ((char *, int));
extern tree get_identifier_with_blank PROTO ((char *));
extern tree check_identifier PROTO ((tree, tree));

/* Variables */
/* Note: The first parameter to make_new_variable() must not be longer than 200 chars! */
extern tree make_new_variable PROTO ((char *, tree));
extern tree alloca_string PROTO ((tree));
extern tree new_string_by_model PROTO ((tree, tree, int));
extern void declare_vars PROTO ((tree, tree, tree, char *, unsigned, int, tree ));
extern tree build_string_move PROTO ((tree, tree, tree));
extern tree build_string_pad PROTO ((tree, tree, tree));

/* Parameters */
extern void handle_formal_conf_array_param_list PROTO ((tree, tree, int, int));
extern void handle_formal_param_list PROTO ((tree, tree, tree, int, int));

/* Initializers */
extern void assign_tags PROTO ((tree, tree));
extern void init_any PROTO ((tree, int));
extern void un_initialize_block PROTO ((tree, int));
extern tree build_pascal_initializer PROTO ((tree, tree));

/* Procedure and Function calls */
extern int suspend_function_calls PROTO ((void));
extern int allow_function_calls PROTO ((void));
extern void resume_function_calls PROTO ((int));
extern tree function_return_type PROTO ((tree));
extern tree maybe_call_function PROTO ((tree, tree));
extern tree probably_call_function PROTO ((tree));

/* Main program */
extern void do_setjmp PROTO ((void));
extern void output_real_main_program PROTO ((tree));
extern tree get_main_program_name PROTO ((tree));
extern void generate_main_program_rtx PROTO ((void));
extern void init_main_program_rtx PROTO ((void));
extern void expand_goto_main_program PROTO ((tree));
extern void pascal_expand_goto PROTO ((tree));
extern void finish_program_component PROTO ((void));
extern void exit_compilation PROTO ((void));

/* GPC startup */
extern void init_util PROTO ((void));
extern void pascal_init_options PROTO ((void));
extern void pascal_init PROTO ((void));
extern void start_constructor PROTO ((void));
extern void finish_constructor PROTO ((void));

/* Miscellaneous */
extern void pushlevel_expand PROTO ((void));
extern tree poplevel_expand PROTO ((void));
extern void check_for_control_variable PROTO ((tree));
extern void typed_const_warning PROTO ((tree));
extern void object_directive_warning PROTO ((void));
extern tree no_parameters PROTO ((void));
#ifndef EGCS92
extern char *concat PVPROTO((char *, ...));
#endif
extern char *save_string PROTO ((char *));

/* Utility functions */
extern int contains_file_p PROTO ((tree));
extern int contains_file_or_schema_p PROTO ((tree));
extern void declare_known_ids PROTO ((void));

/* Define macros. */

/* Shorthands used in building integer range types
 * A is a tree type low_bound and B is an INT value of the high_bound
 */
#define INT_RANGE_TYPE(a,b) \
        build_range_type (integer_type_node, a, build_int_2 (b, 0))

/* Makes life easier with required record types */
#define DO_FIELD(name, type)                                 \
        grokfield (input_filename, 0, get_identifier (name), \
                   build_tree_list (NULL_TREE, type), NULL_TREE)

/* End of #include file. */

#endif /* _GPC_UTIL_H_ */
