/* Language specific definitions for GPC.
   Copyright (C) 1989-2000 Free Software Foundation, Inc.

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
along with GNU Pascal; see the file COPYING. If not, write to the
Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA. */

/*
 * Authors: Jukka Virtanen <jtv@hut.fi>
 *          Peter Gerwinski <peter@gerwinski.de>
 */

#ifndef _GPC_LANG_H
#define _GPC_LANG_H

/* GPC specific extensions to the tree_node TYPE part.
 */
struct lang_type
{
  /* LEX_ID node telling which interface this type
   * has been declared in.
   */
  union tree_node *interface;

  /* Unique identifier for this type in its GPI file.
   */
  int uid;

  /* Distinguishes schemata, objects, and strings from
   * ordinary records.
   */
  int code;

  /* Name of an object type, tag info for variant records,
   * or declared string capacity.
   */
  union tree_node *info;

  /* Base type of an object type.
   */
  union tree_node *base;

  /* Specifies the initial value associated to the type in
   * Extended Pascal.  NOT used currently.
   */
  union tree_node *initial;
};

/* GPC specific extensions to the tree_node DECL part.
 */
struct lang_decl
{
  /* LEX_ID node telling which interface this type
   * has been declared in.
   */
  union tree_node *interface;

  /* Unique identifier for this type in its GPI file.
   */
  int uid;

  /* TREE_LIST of LEX_ID nodes describing directives.
   */
  union tree_node *directive;

  /* Chain of formal parameters of the routine.
   * (The FUNCTION_DECL contains only the types, not the
   * argument names.)
   */
  union tree_node *parms;
};

/* GPC specific prototypes for functions in lang.c.
 */
extern void finish_file PROTO ((void));

#endif /* not _GPC_LANG_H */
