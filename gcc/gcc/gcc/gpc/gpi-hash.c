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
 * Authors:  Alexei Volokhov <voh@ispras.ru>
 *           Peter Gerwinski <peter@gerwinski.de>
 */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#endif

#include "gbe.h"
#include "gpc-defs.h"
#include "util.h"
#include "module.h"
#include "circle-buf.h"
#include "memfiles.h"
#include "gpi-hash.h"

#include <sys/types.h>
/* #include <sys/stdtypes.h> */
#include <unistd.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#endif

/* Symbolic constants for the "lseek" function: */
#ifndef SEEK_SET
#define SEEK_SET 0      /* Set file pointer to "offset" */
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1      /* Set file pointer to current plus "offset" */
#endif
#ifndef SEEK_END
#define SEEK_END 2      /* Set file pointer to EOF plus "offset" */
#endif


/* integer type for void * representative */
#define IRP_TYPE long

struct uid_ptr
{
  tree node;
  int uid;
  struct uid_ptr * next;
};

#define UID_IS_NOT_SAVED(UID) ((UID) >0)
#define SAVE_UID(UID) (-(UID))
#define NATIVE_UID(UID) ((UID)>0 ? (UID) : -(UID))

#define MAX_HASH_TABLE 8191
static struct uid_ptr *hash_table[MAX_HASH_TABLE];      /* id hash buckets */

off_t * offs_array;
long offs_array_size;
struct obstack imbed_file_obstack;
int imbed_file_obstack_inited = 0;
char * imbed_firstobj;
long uid_counter;

struct circle_buf * cbf;

void *imb_alloc PROTO ((size_t));
void clean_imbed_file_obstack PROTO ((void));

/* alloc and clear memory */
void *
imb_alloc (size)
     size_t size;
{
  void * tmp;
  tmp = obstack_alloc (&imbed_file_obstack,size);
  if (size)
    memset (tmp, 0, size);
  return tmp;
}

/* init data for store gpi file */
static void
init_outbed (a_size)
     int a_size;
{
  uid_counter = 0;
  if (!imbed_file_obstack_inited)
    {
      gcc_obstack_init (&imbed_file_obstack);
      imbed_file_obstack_inited = 1;
      imbed_firstobj = (char *) imb_alloc (0);
      cbf = (struct circle_buf *) imb_alloc (sizeof (struct circle_buf));
      init_circle_buf (cbf, &imbed_file_obstack, a_size);
    }
  else
    {
      if (cbf)
        free_circle_buf (cbf);
      else
        {
          cbf = (struct circle_buf *)
            imb_alloc (sizeof (struct circle_buf));
          init_circle_buf (cbf, &imbed_file_obstack, a_size);
        }
    }
  offs_array_size = a_size;
  offs_array = imb_alloc (offs_array_size * sizeof (off_t));
}


/* We must avoid negative values.  Naturally, we would cast this to
 * `unsigned ptrdiff_t', but since we cannot be sure that the platform
 * we compile GPC on will support this, we use `abs' instead.
 */
#define HASH_FUNC(NODE) ((abs ((NODE) - (tree) 0) >> 2) % MAX_HASH_TABLE)

/* return pointer to a node uid field */
int *
get_uid (node)
     tree node;
{
  IRP_TYPE hi;
  struct uid_ptr *l;

  hi = HASH_FUNC (node);
  /* Search table for node */
  for (l = hash_table[hi]; l; l =l->next)
    {
      if (l->node == node)
          return &l->uid;
    }
  return 0;
}

/* set unique identifier to a node
   uid == 0 means next available.
   return pointer to uid field */

int *
set_uid (node, uid)
     tree node;
     int uid;
{
  IRP_TYPE hi;
  struct uid_ptr *l;

  if (uid)
    {
      assert (uid > uid_counter);
      uid_counter = uid;
    }
  else
    {
      uid = ++uid_counter;
    }
  if (uid_counter >= offs_array_size)
    {
      long new_size =  offs_array_size << 2;
      off_t * new_offs_array = imb_alloc (new_size * sizeof (off_t));
      memcpy (new_offs_array, offs_array, offs_array_size*sizeof (off_t));
      offs_array = new_offs_array;
      offs_array_size = new_size;
    }
  /* Compute hash code */
  hi = HASH_FUNC (node);
  l = (struct uid_ptr *) imb_alloc (sizeof (struct uid_ptr));
  l->node = node;
  l->uid = uid;
  l->next = hash_table[hi];
  hash_table[hi] = l;
  return &l->uid;
}

/* Check/or set uid for node and  add node to offs_array or circule buf*/
int
hashed_save (node, s, depth)
     tree node;
     FILE *s;
     int depth;
{
  int * uid_p;
  int native_uid;

  if (!node)
    {
      native_uid = 0;
      fwrite (&native_uid, sizeof (int), 1, s);
      return 1;
    }
  uid_p = get_uid (node);
  if (!uid_p)
    {
      uid_p = set_uid (node, 0);
    }
  if (UID_IS_NOT_SAVED(*uid_p)) /* not saved yet */
    {
      if (!depth)
        {
          offs_array [*uid_p] = ftell (s);
#ifdef DEBUG_HASH
          {
            int key = DEBUG_HASH_KEY;
            fwrite (&key, sizeof (key), 1, s);
          }
#endif
          *uid_p = SAVE_UID(*uid_p);
          return 0;
        }
      else
        {
          fwrite (uid_p, sizeof (int), 1, s);
          in_circle_buf (cbf, node);
        }
    }
  else
    {
      native_uid = NATIVE_UID(*uid_p);
      fwrite (&native_uid, sizeof (int), 1, s);
    }
  return 1;
}


/* store all nodes from circule buf */
void
flush_suspended (f)
     FILE * f;
{
  int * uid_p;
  tree node;
  int no_more;
  while (!is_empty_buf(cbf))
    {
      node = out_circle_buf (cbf, &no_more);
      uid_p = get_uid (node);
      assert (uid_p);
      assert (*uid_p);
      if (uid_p && *uid_p > 0)
        store_tree (node, f, 0);
    }
}

off_t header_off;
struct imbed_header
{
  off_t offs_array_off;
  long offs_array_size;
#ifdef DEBUG_HASH
  int key;
#endif
} ss;

/* Save place for header
   set UID for builtin nodes */
void
store_header (s, nn)
     FILE * s;
     int nn;
{
  int * uid_p;
  header_off = ftell (s);
#ifdef DEBUG_HASH
  ss.key = 0;
#endif
  fwrite (&ss, sizeof (ss), 1, s);
  memset (hash_table, 0, MAX_HASH_TABLE * sizeof (struct uid_ptr *));
  init_outbed (nn + 1024);
#define PROCESS_NODE(n,node)  \
   uid_p = set_uid (node, n);  \
   *uid_p = -*uid_p;
#include "nodes.def"
#undef PROCESS_NODE
}

/* Write UID OFFSET table */

void
update_header (s)
     FILE * s;
{
  struct imbed_header ss;
  ss.offs_array_off = ftell (s);
  ss.offs_array_size = offs_array_size;
#ifdef DEBUG_HASH
  ss.key = -DEBUG_HASH_KEY;
#endif
  fseek (s, header_off, SEEK_SET);
  fwrite (&ss, sizeof (ss), 1, s);
  fseek (s, 0, SEEK_END);
  fwrite (offs_array, offs_array_size*sizeof (off_t), 1, s);
}


off_t * imbed_offs_array;
tree * loaded_nodes;

/* Read UID OFFSET table */
void
load_module_header (s)
     MEMFILE * s;
{
  off_t cp;
  off_t rp;
  memset (hash_table, 0, MAX_HASH_TABLE * sizeof (struct uid_ptr *));
  if (!imbed_file_obstack_inited)
    {
      gcc_obstack_init (&imbed_file_obstack);
      imbed_file_obstack_inited = 1;
      imbed_firstobj = (char *) imb_alloc (0);
    }
  mread (&ss, sizeof (ss), 1, s);
#ifdef DEBUG_HASH
  assert (ss.key == -DEBUG_HASH_KEY);
#endif
  imbed_offs_array = imb_alloc (ss.offs_array_size * sizeof (off_t));
  loaded_nodes = (tree *) imb_alloc (ss.offs_array_size * sizeof (tree));
  cp = mtell (s);
  mseek (s, ss.offs_array_off, SEEK_SET);
  mread (imbed_offs_array, ss.offs_array_size*sizeof (off_t), 1, s);
  rp = mseek (s, cp, SEEK_SET);
  assert (rp == 0);
#define PROCESS_NODE(n,node) loaded_nodes[n] = node
#include "nodes.def"
#undef PROCESS_NODE
}

/* Check Is the node with UID already loaded.
   If it is returns node
   If not seek file for reading the node and returns 0 */
tree
hashed_load (s, uid)
     MEMFILE * s;
     int uid;
{
  off_t rp;
  if (loaded_nodes[uid])
    return loaded_nodes[uid];
  rp = imbed_offs_array[uid];
  mseek (s, rp, SEEK_SET);
#ifdef DEBUG_HASH
  {
    int key;
    mread (&key, sizeof (key), 1, s);
    assert (key == DEBUG_HASH_KEY);
  }
#endif
  return 0;
}

/* Mark UID as loaded node */
tree
hash_loaded_node (node, uid)
     tree node;
     int uid;
{
  assert (node);
  assert (uid);
  /* Loops are possible */
  if (loaded_nodes[uid])
    {
      assert (TREE_CODE (loaded_nodes[uid]) == TREE_CODE (node));
      return loaded_nodes[uid];
    }
  return ((loaded_nodes[uid] = node));
}

void
clean_imbed_file_obstack ()
{
  imbed_file_obstack_inited = 0;
  obstack_free (&imbed_file_obstack, imbed_firstobj);
}

#undef mread
size_t
checked_mread(pointer, size, num_items, stream)
     void *pointer;
     size_t size;
     size_t num_items;
     MEMFILE *stream;
{
  size_t rni;
  rni = mread (pointer, size, num_items, stream);
  assert (rni == num_items);
  return rni;
}

#undef fwrite
size_t
checked_fwrite (pointer, size, num_items, stream)
     const void *pointer;
     size_t size;
     size_t num_items;
     FILE *stream;
{
  size_t rni;
  rni = fwrite (pointer, size, num_items, stream);
  assert (rni == num_items);
  return rni;
}
