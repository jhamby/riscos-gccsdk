/* Module support (additional routines) for GNU Pascal
   Copyright (C) 1997-2000 Free Software Foundation, Inc.

This file is part of GNU Pascal.

GNU Pascal is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
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
 * Author:  Alexey Volokhov <voh@ispras.ru>
 */

#ifdef DEBUG_HASH
#define DEBUG_HASH_KEY (int) 0x54637281
#endif

/* return pointer to a node uid field */
extern int * get_uid PROTO ((tree));

/* set unique identifier to a node
   uid == 0 means next available.
   return pointer to uid field */
extern int * set_uid PROTO ((tree, int));
/* Check/or set uid for node and  add node to offs_array or circule buf*/
extern int hashed_save PROTO ((tree, FILE *, int ));
/* store all nodes from circule buf */
extern void flush_suspended PROTO ((FILE * ));

extern void store_header PROTO ((FILE *, int));
extern void update_header PROTO ((FILE *));
extern void load_module_header PROTO ((MEMFILE *));
extern tree hashed_load PROTO  ((MEMFILE *, int));
extern tree hash_loaded_node PROTO ((tree, int));
extern void clean_imbed_file_obstack ();

#define mread(P, S, N, F) checked_mread(P, S, N, F)
#define fwrite(P, S, N, F) checked_fwrite(P, S, N, F)

extern size_t
checked_mread PROTO ((void *, size_t, size_t, MEMFILE *));

extern size_t
checked_fwrite PROTO ((const void *, size_t, size_t, FILE *));

extern void
gcc_obstack_init PROTO ((struct obstack *obstack));

extern tree identifier_output;
extern tree identifier_input;
extern tree global_input_file_node;
extern tree global_output_file_node;
