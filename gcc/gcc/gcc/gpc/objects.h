/* Interface to public routines in objects.c
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

#ifndef _GPC_OBJECTS_H_
#define _GPC_OBJECTS_H_

/* Include files needed by this one. */


/* Structure definitions. */


/* Global objects accessed by users of this module. */


/* Declare functions with prototypes. */

extern tree simple_get_field PROTO ((register tree, tree, char *));
extern tree call_method PROTO ((tree, tree));
extern tree get_method_name PROTO((tree, tree));
extern void push_self_parameter PROTO((tree, int));
extern void check_object_pointer PROTO((void));
extern void store_object_name PROTO((tree));
extern void inherit PROTO((tree, tree));
extern void add_vmt_field PROTO((tree));
extern void check_method_parameters PROTO((tree));
extern void construct_vmt PROTO((tree));
extern tree mark_virtual_method PROTO((tree));

#endif /* _GPC_OBJECTS_H_ */
