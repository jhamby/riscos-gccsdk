/* Copyright (C) 1994, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Joel Sherrill (jsherril@redstone-emh2.army.mil),
     On-Line Applications Research Corporation.

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

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

int failures = 0;

static void try (unsigned long long int value, int expected)
{
  if (ffsll (value) != expected)
    {
      fprintf (stderr, "%lld expected %d got %d\n",
	       value, expected, ffsll (value));
      ++failures;
    }
}

int
main (void)
{
  int i;
  try (0, 0);
  for (i=0 ; i<64 ; i++)
    try (1ULL << i, i+1);
  for (i=0 ; i<64 ; i++)
    try ((~0ULL >> i) << i, i+1);
  try (0x8000800000000000ULL, 48);
  try (0x8000800080000000ULL, 32);
  try (0x0000000080008000ULL, 16);

  if (failures)
    printf ("Test FAILED!  %d failure%s.\n", failures, &"s"[failures == 1]);
  else
    puts ("Test succeeded.");

  exit (failures);
}
