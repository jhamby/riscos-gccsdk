/* Copyright (C) 1996 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1996.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <errno.h>
#include <wchar.h>

#include <pthread.h>
#include <internal/unix.h>

#ifndef EILSEQ
#define EILSEQ EINVAL
#endif


static mbstate_t internal;

size_t
mbrtowc (wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
  size_t used = 0;

  PTHREAD_UNSAFE

  if (ps == NULL)
    ps = &internal;

  if (s == NULL)
    {
      /* See first paragraph of description in 7.16.6.3.2.  */
      pwc = NULL;
      s = "";
      n = 1;
    }

  if (n > 0)
    {
      if (ps->__count == 0)
	{
	  unsigned char byte = (unsigned char) *s++;
	  ++used;

	  /* We must look for a possible first byte of a UTF8 sequence.  */
	  if (byte < 0x80)
	    {
	      /* One byte sequence.  */
	      if (pwc != NULL)
		*pwc = (wchar_t) byte;
	      return byte ? used : 0;
	    }

	  if ((byte & 0xc0) == 0x80 || (byte & 0xfe) == 0xfe)
	    {
	      /* Oh, oh.  An encoding error.  */
	      return  __set_errno (EILSEQ);
	    }

	  if ((byte & 0xe0) == 0xc0)
	    {
	      /* We expect two bytes.  */
	      ps->__count = 1;
	      ps->__value.__wch = byte & 0x1f;
	    }
	  else if ((byte & 0xf0) == 0xe0)
	    {
	      /* We expect three bytes.  */
	      ps->__count = 2;
	      ps->__value.__wch = byte & 0x0f;
	    }
	  else if ((byte & 0xf8) == 0xf0)
	    {
	      /* We expect four bytes.  */
	      ps->__count = 3;
	      ps->__value.__wch = byte & 0x07;
	    }
	  else if ((byte & 0xfc) == 0xf8)
	    {
	      /* We expect five bytes.  */
	      ps->__count = 4;
	      ps->__value.__wch = byte & 0x03;
	    }
	  else
	    {
	      /* We expect six bytes.  */
	      ps->__count = 5;
	      ps->__value.__wch = byte & 0x01;
	    }
	}

      /* We know we have to handle a multibyte character and there are
	 some more bytes to read.  */
      while (used < n)
	{
	  /* The second to sixths byte must be of the form 10xxxxxx.  */
	  unsigned char byte = (const unsigned char) *s++;
	  ++used;

	  if ((byte & 0xc0) != 0x80)
	    {
	      /* Oh, oh.  An encoding error.  */
	      return __set_errno (EILSEQ);
	    }

	  ps->__value.__wch <<= 6;
	  ps->__value.__wch |= byte & 0x3f;

	  if (--ps->__count == 0)
	    {
	      /* The character is finished.  */
	      if (pwc != NULL)
		*pwc = (wchar_t) ps->__value.__wch;
	      return ps->__value.__wch ? used : 0;
	    }
	}
    }

  return (size_t) -2;
}
