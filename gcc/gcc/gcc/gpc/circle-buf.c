/* Circle buffer routines for GNU Pascal
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
 * Author:  Alexei Volokhov <voh@ispras.ru>
 */

#include "gcc-version.h"
#include "config.h"
#include "gansidecl.h"
#ifdef EGCS
#include "system.h"
#else
#ifdef HAVE_STRING_H
#include <string.h>
#else
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#endif
#endif
#include "obstack.h"
#include "circle-buf.h"

/* Initialize procedure.
   buf_obstack must be already initialized.
   start_size must be >0 */

void
init_circle_buf (self,
                 buf_obstack,
                 start_size)
     struct circle_buf * self;
     struct obstack * buf_obstack;
     unsigned long start_size;
{
  self->a_size = AROUND_SIZE (start_size, 2);
  self->buf_obstack = buf_obstack;
  self->items = obstack_alloc (self->buf_obstack, self->a_size * sizeof (T));
  memset ((void *) self->items, 0, self->a_size * sizeof (T));
  self->fio = self->lio = 0;
}


static void
realloc_circle_buf (self)
     struct circle_buf * self;
{
  /* size is grown to old_size *2 */
  unsigned long old_size = self->a_size;
  T * new_items;
  self->a_size = AROUND_SIZE (self->a_size << 1, sizeof (T));
  new_items =  obstack_alloc (self->buf_obstack, self->a_size * sizeof (T));
  /* buffer full lio == fio here */
  if (old_size > self->fio)
    memcpy ((void *) &new_items[0], (void *) &(self->items[self->fio]),
            (old_size - self->fio) * sizeof (T));
  if (self->lio > 0)
    memcpy ((void *) &new_items[old_size - self->fio], (void *) &(self->items[0]),
            self->lio * sizeof (T));
  self->lio = old_size;
  self->fio = 0;
  /* free buf */
  self->items = new_items;
}

/* Insert obj into queue */
void
in_circle_buf (self, obj)
     struct circle_buf * self;
     T obj;
{
  /* empty buffer */
  if (self->fio == self->lio)
    {
      self->fio = 0;
      self->lio = 0;
    }

  self->items [self->lio] = obj;
  self->lio ++;

  /* not empty circle buffer */
  if (self->lio >= self->a_size)
    self->lio = 0;
  /* full buffer ==> realloc it */
  if (self->lio == self->fio)
    {
      realloc_circle_buf (self);
    }
}


/* Get object from queue.
  if no objects in the buffer returns 0 and set no_more = 1 */
T
out_circle_buf (self, no_more)
     struct circle_buf * self;
     int * no_more;
{
  T obj;
  if (self->fio == self->lio)
    {
      *no_more = 1;
      return 0;
    }
  obj = self->items[self->fio++];
  if (self->fio == self->lio)
    {
      self->fio = 0;
      self->lio = 0;
    }
  else if (self->fio >= self->a_size)
    {
      self->fio = 0;
    }
  return obj;
}

/* Returns 1 if buffer is empty */
int
is_empty_buf (struct circle_buf * self)
{
  return self->fio == self->lio;
}

/* Crear buffer */
void
free_circle_buf (struct circle_buf * self)
{
  self->fio = self->lio = 0;
}
