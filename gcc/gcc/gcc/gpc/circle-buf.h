/* Circle buffer routines for the GPC
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
Boston, MA 02111-1370, USA. */

/*
 * Author:  Alexei Volokhov <voh@ispras.ru>
 */

#define T void *

struct circle_buf
{
  /* contents */
  T * items;
  /* available size */
  unsigned long a_size;
  /* first input object */
  unsigned long fio;
  /* last input oject */
  unsigned long lio;
  /* allocation stack */
  struct obstack * buf_obstack;
};

#define AROUND_SIZE(SIZE,BY) (((SIZE) + ((1 << (BY)) -1)) << (BY) >> (BY))

extern void init_circle_buf PROTO ((struct circle_buf *,
                                    struct obstack *,
                                    unsigned long ));
extern void
in_circle_buf PROTO ((struct circle_buf *, T));

extern T
out_circle_buf PROTO ((struct circle_buf *, int *));

extern int
is_empty_buf PROTO ((struct circle_buf * ));

extern void
free_circle_buf PROTO ((struct circle_buf * self));
