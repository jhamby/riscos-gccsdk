/* Tester for string functions.
   Copyright (C) 1995, 1996, 1997, 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

/* Make sure we don't test the optimized inline functions if we want to
   test the real implementation.  */
#if !defined DO_STRING_INLINES
#undef __USE_STRING_INLINES
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>

#ifndef HAVE_GNU_LD
#define _sys_nerr	sys_nerr
#define _sys_errlist	sys_errlist
#endif

#define	STREQ(a, b)	(strcmp((a), (b)) == 0)

const char *it = "<UNSET>";	/* Routine name for message routines. */
size_t errors = 0;

/* Complain if condition is not true.  */
void
check (int thing, int number)
{
  if (!thing)
    {
      printf("%s flunked test %d\n", it, number);
      ++errors;
    }
}

/* Complain if first two args don't strcmp as equal.  */
void
equal (const char *a, const char *b, int number)
{
  check(a != NULL && b != NULL && STREQ (a, b), number);
}

char one[50];
char two[50];
char *cp;

void
test_strcmp (void)
{
  it = "strcmp";
  check (strcmp ("", "") == 0, 1);		/* Trivial case. */
  check (strcmp ("a", "a") == 0, 2);		/* Identity. */
  check (strcmp ("abc", "abc") == 0, 3);	/* Multicharacter. */
  check (strcmp ("abc", "abcd") < 0, 4);	/* Length mismatches. */
  check (strcmp ("abcd", "abc") > 0, 5);
  check (strcmp ("abcd", "abce") < 0, 6);	/* Honest miscompares. */
  check (strcmp ("abce", "abcd") > 0, 7);
  check (strcmp ("a\203", "a") > 0, 8);		/* Tricky if char signed. */
  check (strcmp ("a\203", "a\003") > 0, 9);

  {
    char buf1[0x40], buf2[0x40];
    int i, j;
    for (i=0; i < 0x10; i++)
      for (j = 0; j < 0x10; j++)
	{
	  int k;
	  for (k = 0; k < 0x3f; k++)
	    {
	      buf1[j] = '0' ^ (k & 4);
	      buf2[j] = '4' ^ (k & 4);
	    }
	  buf1[i] = buf1[0x3f] = 0;
	  buf2[j] = buf2[0x3f] = 0;
	  for (k = 0; k < 0xf; k++)
	    {
	      int cnum = 0x10+0x10*k+0x100*j+0x1000*i;
	      check (strcmp (buf1+i,buf2+j) == 0, cnum);
	      buf1[i+k] = 'A' + i + k;
	      buf1[i+k+1] = 0;
	      check (strcmp (buf1+i,buf2+j) > 0, cnum+1);
	      check (strcmp (buf2+j,buf1+i) < 0, cnum+2);
	      buf2[j+k] = 'B' + i + k;
	      buf2[j+k+1] = 0;
	      check (strcmp (buf1+i,buf2+j) < 0, cnum+3);
	      check (strcmp (buf2+j,buf1+i) > 0, cnum+4);
	      buf2[j+k] = 'A' + i + k;
	      buf1[i] = 'A' + i + 0x80;
	      check (strcmp (buf1+i,buf2+j) > 0, cnum+5);
	      check (strcmp (buf2+j,buf1+i) < 0, cnum+6);
	      buf1[i] = 'A' + i;
	    }
	}
  }
}

void
test_strcpy (void)
{
  it = "strcpy";
  check (strcpy (one, "abcd") == one, 1); /* Returned value. */
  equal (one, "abcd", 2);		/* Basic test. */

  (void) strcpy (one, "x");
  equal (one, "x", 3);			/* Writeover. */
  equal (one+2, "cd", 4);		/* Wrote too much? */

  (void) strcpy (two, "hi there");
  (void) strcpy (one, two);
  equal (one, "hi there", 5);		/* Basic test encore. */
  equal (two, "hi there", 6);		/* Stomped on source? */

  (void) strcpy (one, "");
  equal (one, "", 7);			/* Boundary condition. */
}

void
test_stpcpy (void)
{
  it = "stpcpy";
  check ((stpcpy (one, "a") - one) == 1, 1);
  equal (one, "a", 2);

  check ((stpcpy (one, "ab") - one) == 2, 3);
  equal (one, "ab", 4);

  check ((stpcpy (one, "abc") - one) == 3, 5);
  equal (one, "abc", 6);

  check ((stpcpy (one, "abcd") - one) == 4, 7);
  equal (one, "abcd", 8);

  check ((stpcpy (one, "abcde") - one) == 5, 9);
  equal (one, "abcde", 10);

  check ((stpcpy (one, "abcdef") - one) == 6, 11);
  equal (one, "abcdef", 12);

  check ((stpcpy (one, "abcdefg") - one) == 7, 13);
  equal (one, "abcdefg", 14);

  check ((stpcpy (one, "abcdefgh") - one) == 8, 15);
  equal (one, "abcdefgh", 16);

  check ((stpcpy (one, "abcdefghi") - one) == 9, 17);
  equal (one, "abcdefghi", 18);

  check ((stpcpy (one, "x") - one) == 1, 19);
  equal (one, "x", 20);			/* Writeover. */
  equal (one+2, "cdefghi", 21);		/* Wrote too much? */

  check ((stpcpy (one, "xx") - one) == 2, 22);
  equal (one, "xx", 23);		/* Writeover. */
  equal (one+3, "defghi", 24);		/* Wrote too much? */

  check ((stpcpy (one, "xxx") - one) == 3, 25);
  equal (one, "xxx", 26);		/* Writeover. */
  equal (one+4, "efghi", 27);		/* Wrote too much? */

  check ((stpcpy (one, "xxxx") - one) == 4, 28);
  equal (one, "xxxx", 29);		/* Writeover. */
  equal (one+5, "fghi", 30);		/* Wrote too much? */

  check ((stpcpy (one, "xxxxx") - one) == 5, 31);
  equal (one, "xxxxx", 32);		/* Writeover. */
  equal (one+6, "ghi", 33);		/* Wrote too much? */

  check ((stpcpy (one, "xxxxxx") - one) == 6, 34);
  equal (one, "xxxxxx", 35);		/* Writeover. */
  equal (one+7, "hi", 36);		/* Wrote too much? */

  check ((stpcpy (one, "xxxxxxx") - one) == 7, 37);
  equal (one, "xxxxxxx", 38);		/* Writeover. */
  equal (one+8, "i", 39);		/* Wrote too much? */

  check ((stpcpy (stpcpy (stpcpy (one, "a"), "b"), "c") - one) == 3, 40);
  equal (one, "abc", 41);
  equal (one + 4, "xxx", 42);
}

void
test_stpncpy (void)
{
  it = "stpncpy";
  memset (one, 'x', sizeof (one));
  check (stpncpy (one, "abc", 2) == one + 2, 1);
  check (stpncpy (one, "abc", 3) == one + 3, 2);
  check (stpncpy (one, "abc", 4) == one + 3, 3);
  check (one[3] == '\0' && one[4] == 'x', 4);
  check (stpncpy (one, "abcd", 5) == one + 4, 5);
  check (one[4] == '\0' && one[5] == 'x', 6);
  check (stpncpy (one, "abcd", 6) == one + 4, 7);
  check (one[4] == '\0' && one[5] == '\0' && one[6] == 'x', 8);
}

void
test_strcat (void)
{
  it = "strcat";
  (void) strcpy (one, "ijk");
  check (strcat (one, "lmn") == one, 1); /* Returned value. */
  equal (one, "ijklmn", 2);		/* Basic test. */

  (void) strcpy (one, "x");
  (void) strcat (one, "yz");
  equal (one, "xyz", 3);			/* Writeover. */
  equal (one+4, "mn", 4);			/* Wrote too much? */

  (void) strcpy (one, "gh");
  (void) strcpy (two, "ef");
  (void) strcat (one, two);
  equal (one, "ghef", 5);			/* Basic test encore. */
  equal (two, "ef", 6);			/* Stomped on source? */

  (void) strcpy (one, "");
  (void) strcat (one, "");
  equal (one, "", 7);			/* Boundary conditions. */
  (void) strcpy (one, "ab");
  (void) strcat (one, "");
  equal (one, "ab", 8);
  (void) strcpy (one, "");
  (void) strcat (one, "cd");
  equal (one, "cd", 9);
}

void
test_strncat (void)
{
  /* First test it as strcat, with big counts, then test the count
     mechanism.  */
  it = "strncat";
  (void) strcpy (one, "ijk");
  check (strncat (one, "lmn", 99) == one, 1);	/* Returned value. */
  equal (one, "ijklmn", 2);		/* Basic test. */

  (void) strcpy (one, "x");
  (void) strncat (one, "yz", 99);
  equal (one, "xyz", 3);		/* Writeover. */
  equal (one+4, "mn", 4);		/* Wrote too much? */

  (void) strcpy (one, "gh");
  (void) strcpy (two, "ef");
  (void) strncat (one, two, 99);
  equal (one, "ghef", 5);			/* Basic test encore. */
  equal (two, "ef", 6);			/* Stomped on source? */

  (void) strcpy (one, "");
  (void) strncat (one, "", 99);
  equal (one, "", 7);			/* Boundary conditions. */
  (void) strcpy (one, "ab");
  (void) strncat (one, "", 99);
  equal (one, "ab", 8);
  (void) strcpy (one, "");
  (void) strncat (one, "cd", 99);
  equal (one, "cd", 9);

  (void) strcpy (one, "ab");
  (void) strncat (one, "cdef", 2);
  equal (one, "abcd", 10);			/* Count-limited. */

  (void) strncat (one, "gh", 0);
  equal (one, "abcd", 11);			/* Zero count. */

  (void) strncat (one, "gh", 2);
  equal (one, "abcdgh", 12);		/* Count and length equal. */
}

void
test_strncmp (void)
{
  /* First test as strcmp with big counts, then test count code.  */
  it = "strncmp";
  check (strncmp ("", "", 99) == 0, 1);	/* Trivial case. */
  check (strncmp ("a", "a", 99) == 0, 2);	/* Identity. */
  check (strncmp ("abc", "abc", 99) == 0, 3);	/* Multicharacter. */
  check (strncmp ("abc", "abcd", 99) < 0, 4);	/* Length unequal. */
  check (strncmp ("abcd", "abc", 99) > 0, 5);
  check (strncmp ("abcd", "abce", 99) < 0, 6);	/* Honestly unequal. */
  check (strncmp ("abce", "abcd", 99) > 0, 7);
  check (strncmp ("a\203", "a", 2) > 0, 8);	/* Tricky if '\203' < 0 */
  check (strncmp ("a\203", "a\003", 2) > 0, 9);
  check (strncmp ("abce", "abcd", 3) == 0, 10);	/* Count limited. */
  check (strncmp ("abce", "abc", 3) == 0, 11);	/* Count == length. */
  check (strncmp ("abcd", "abce", 4) < 0, 12);	/* Nudging limit. */
  check (strncmp ("abc", "def", 0) == 0, 13);	/* Zero count. */
}

void
test_strncpy (void)
{
  /* Testing is a bit different because of odd semantics.  */
  it = "strncpy";
  check (strncpy (one, "abc", 4) == one, 1);	/* Returned value. */
  equal (one, "abc", 2);			/* Did the copy go right? */

  (void) strcpy (one, "abcdefgh");
  (void) strncpy (one, "xyz", 2);
  equal (one, "xycdefgh", 3);			/* Copy cut by count. */

  (void) strcpy (one, "abcdefgh");
  (void) strncpy (one, "xyz", 3);		/* Copy cut just before NUL. */
  equal (one, "xyzdefgh", 4);

  (void) strcpy (one, "abcdefgh");
  (void) strncpy (one, "xyz", 4);		/* Copy just includes NUL. */
  equal (one, "xyz", 5);
  equal (one+4, "efgh", 6);			/* Wrote too much? */

  (void) strcpy (one, "abcdefgh");
  (void) strncpy (one, "xyz", 5);		/* Copy includes padding. */
  equal (one, "xyz", 7);
  equal (one+4, "", 8);
  equal (one+5, "fgh", 9);

  (void) strcpy (one, "abc");
  (void) strncpy (one, "xyz", 0);		/* Zero-length copy. */
  equal (one, "abc", 10);

  (void) strncpy (one, "", 2);		/* Zero-length source. */
  equal (one, "", 11);
  equal (one+1, "", 12);
  equal (one+2, "c", 13);

  (void) strcpy (one, "hi there");
  (void) strncpy (two, one, 9);
  equal (two, "hi there", 14);		/* Just paranoia. */
  equal (one, "hi there", 15);		/* Stomped on source? */
}

void
test_strlen (void)
{
  it = "strlen";
  check (strlen ("") == 0, 1);		/* Empty. */
  check (strlen ("a") == 1, 2);		/* Single char. */
  check (strlen ("abcd") == 4, 3);	/* Multiple chars. */
  {
    char buf[4096];
    int i;
    char *p;
    for (i=0; i < 0x100; i++)
      {
	p = (char *) ((unsigned long int)(buf + 0xff) & ~0xff) + i;
	strcpy (p, "OK");
	strcpy (p+3, "BAD/WRONG");
	check (strlen (p) == 2, 4+i);
      }
   }
}

void
test_strchr (void)
{
  it = "strchr";
  check (strchr ("abcd", 'z') == NULL, 1);	/* Not found. */
  (void) strcpy (one, "abcd");
  check (strchr (one, 'c') == one+2, 2);	/* Basic test. */
  check (strchr (one, 'd') == one+3, 3);	/* End of string. */
  check (strchr (one, 'a') == one, 4);		/* Beginning. */
  check (strchr (one, '\0') == one+4, 5);	/* Finding NUL. */
  (void) strcpy (one, "ababa");
  check (strchr (one, 'b') == one+1, 6);	/* Finding first. */
  (void) strcpy (one, "");
  check (strchr (one, 'b') == NULL, 7);		/* Empty string. */
  check (strchr (one, '\0') == one, 8);		/* NUL in empty string. */
  {
    char buf[4096];
    int i;
    char *p;
    for (i=0; i < 0x100; i++)
      {
	p = (char *) ((unsigned long int) (buf + 0xff) & ~0xff) + i;
	strcpy (p, "OK");
	strcpy (p+3, "BAD/WRONG");
	check (strchr (p, '/') == NULL, 9+i);
      }
   }
}

void
test_index (void)
{
  it = "index";
  check (index ("abcd", 'z') == NULL, 1);	/* Not found. */
  (void) strcpy (one, "abcd");
  check (index (one, 'c') == one+2, 2);	/* Basic test. */
  check (index (one, 'd') == one+3, 3);	/* End of string. */
  check (index (one, 'a') == one, 4);	/* Beginning. */
  check (index (one, '\0') == one+4, 5);	/* Finding NUL. */
  (void) strcpy (one, "ababa");
  check (index (one, 'b') == one+1, 6);	/* Finding first. */
  (void) strcpy (one, "");
  check (index (one, 'b') == NULL, 7);	/* Empty string. */
  check (index (one, '\0') == one, 8);	/* NUL in empty string. */
}

void
test_strrchr (void)
{
  it = "strrchr";
  check (strrchr ("abcd", 'z') == NULL, 1);	/* Not found. */
  (void) strcpy (one, "abcd");
  check (strrchr (one, 'c') == one+2, 2);	/* Basic test. */
  check (strrchr (one, 'd') == one+3, 3);	/* End of string. */
  check (strrchr (one, 'a') == one, 4);		/* Beginning. */
  check (strrchr (one, '\0') == one+4, 5);	/* Finding NUL. */
  (void) strcpy (one, "ababa");
  check (strrchr (one, 'b') == one+3, 6);	/* Finding last. */
  (void) strcpy (one, "");
  check (strrchr (one, 'b') == NULL, 7);	/* Empty string. */
  check (strrchr (one, '\0') == one, 8);	/* NUL in empty string. */
  {
    char buf[4096];
    int i;
    char *p;
    for (i=0; i < 0x100; i++)
      {
	p = (char *) ((unsigned long int) (buf + 0xff) & ~0xff) + i;
	strcpy (p, "OK");
	strcpy (p+3, "BAD/WRONG");
	check (strrchr (p, '/') == NULL, 9+i);
      }
   }
}

void
test_rindex (void)
{
  it = "rindex";
  check (rindex ("abcd", 'z') == NULL, 1);	/* Not found. */
  (void) strcpy (one, "abcd");
  check (rindex (one, 'c') == one+2, 2);	/* Basic test. */
  check (rindex (one, 'd') == one+3, 3);	/* End of string. */
  check (rindex (one, 'a') == one, 4);	/* Beginning. */
  check (rindex (one, '\0') == one+4, 5);	/* Finding NUL. */
  (void) strcpy (one, "ababa");
  check (rindex (one, 'b') == one+3, 6);	/* Finding last. */
  (void) strcpy (one, "");
  check (rindex (one, 'b') == NULL, 7);	/* Empty string. */
  check (rindex (one, '\0') == one, 8);	/* NUL in empty string. */
}

void
test_strpbrk (void)
{
  it = "strpbrk";
  check(strpbrk("abcd", "z") == NULL, 1);	/* Not found. */
  (void) strcpy(one, "abcd");
  check(strpbrk(one, "c") == one+2, 2);	/* Basic test. */
  check(strpbrk(one, "d") == one+3, 3);	/* End of string. */
  check(strpbrk(one, "a") == one, 4);	/* Beginning. */
  check(strpbrk(one, "") == NULL, 5);	/* Empty search list. */
  check(strpbrk(one, "cb") == one+1, 6);	/* Multiple search. */
  (void) strcpy(one, "abcabdea");
  check(strpbrk(one, "b") == one+1, 7);	/* Finding first. */
  check(strpbrk(one, "cb") == one+1, 8);	/* With multiple search. */
  check(strpbrk(one, "db") == one+1, 9);	/* Another variant. */
  (void) strcpy(one, "");
  check(strpbrk(one, "bc") == NULL, 10);	/* Empty string. */
  (void) strcpy(one, "");
  check(strpbrk(one, "bcd") == NULL, 11);	/* Empty string. */
  (void) strcpy(one, "");
  check(strpbrk(one, "bcde") == NULL, 12);	/* Empty string. */
  check(strpbrk(one, "") == NULL, 13);	/* Both strings empty. */
  (void) strcpy(one, "abcabdea");
  check(strpbrk(one, "befg") == one+1, 14);	/* Finding first. */
  check(strpbrk(one, "cbr") == one+1, 15);	/* With multiple search. */
  check(strpbrk(one, "db") == one+1, 16);	/* Another variant. */
  check(strpbrk(one, "efgh") == one+6, 17);	/* And yet another. */
}

void
test_strstr (void)
{
  it = "strstr";
  check(strstr("abcd", "z") == NULL, 1);	/* Not found. */
  check(strstr("abcd", "abx") == NULL, 2);	/* Dead end. */
  (void) strcpy(one, "abcd");
  check(strstr(one, "c") == one+2, 3);	/* Basic test. */
  check(strstr(one, "bc") == one+1, 4);	/* Multichar. */
  check(strstr(one, "d") == one+3, 5);	/* End of string. */
  check(strstr(one, "cd") == one+2, 6);	/* Tail of string. */
  check(strstr(one, "abc") == one, 7);	/* Beginning. */
  check(strstr(one, "abcd") == one, 8);	/* Exact match. */
  check(strstr(one, "abcde") == NULL, 9);	/* Too long. */
  check(strstr(one, "de") == NULL, 10);	/* Past end. */
  check(strstr(one, "") == one, 11);	/* Finding empty. */
  (void) strcpy(one, "ababa");
  check(strstr(one, "ba") == one+1, 12);	/* Finding first. */
  (void) strcpy(one, "");
  check(strstr(one, "b") == NULL, 13);	/* Empty string. */
  check(strstr(one, "") == one, 14);	/* Empty in empty string. */
  (void) strcpy(one, "bcbca");
  check(strstr(one, "bca") == one+2, 15);	/* False start. */
  (void) strcpy(one, "bbbcabbca");
  check(strstr(one, "bbca") == one+1, 16);	/* With overlap. */
}

void
test_strspn (void)
{
  it = "strspn";
  check(strspn("abcba", "abc") == 5, 1);	/* Whole string. */
  check(strspn("abcba", "ab") == 2, 2);	/* Partial. */
  check(strspn("abc", "qx") == 0, 3);	/* None. */
  check(strspn("", "ab") == 0, 4);	/* Null string. */
  check(strspn("abc", "") == 0, 5);	/* Null search list. */
}

void
test_strcspn (void)
{
  it = "strcspn";
  check(strcspn("abcba", "qx") == 5, 1);	/* Whole string. */
  check(strcspn("abcba", "cx") == 2, 2);	/* Partial. */
  check(strcspn("abc", "abc") == 0, 3);	/* None. */
  check(strcspn("", "ab") == 0, 4);	/* Null string. */
  check(strcspn("abc", "") == 3, 5);	/* Null search list. */
}

void
test_strtok (void)
{
  it = "strtok";
  (void) strcpy(one, "first, second, third");
  equal(strtok(one, ", "), "first", 1);	/* Basic test. */
  equal(one, "first", 2);
  equal(strtok((char *)NULL, ", "), "second", 3);
  equal(strtok((char *)NULL, ", "), "third", 4);
  check(strtok((char *)NULL, ", ") == NULL, 5);
  (void) strcpy(one, ", first, ");
  equal(strtok(one, ", "), "first", 6);	/* Extra delims, 1 tok. */
  check(strtok((char *)NULL, ", ") == NULL, 7);
  (void) strcpy(one, "1a, 1b; 2a, 2b");
  equal(strtok(one, ", "), "1a", 8);	/* Changing delim lists. */
  equal(strtok((char *)NULL, "; "), "1b", 9);
  equal(strtok((char *)NULL, ", "), "2a", 10);
  (void) strcpy(two, "x-y");
  equal(strtok(two, "-"), "x", 11);	/* New string before done. */
  equal(strtok((char *)NULL, "-"), "y", 12);
  check(strtok((char *)NULL, "-") == NULL, 13);
  (void) strcpy(one, "a,b, c,, ,d");
  equal(strtok(one, ", "), "a", 14);	/* Different separators. */
  equal(strtok((char *)NULL, ", "), "b", 15);
  equal(strtok((char *)NULL, " ,"), "c", 16);	/* Permute list too. */
  equal(strtok((char *)NULL, " ,"), "d", 17);
  check(strtok((char *)NULL, ", ") == NULL, 18);
  check(strtok((char *)NULL, ", ") == NULL, 19);	/* Persistence. */
  (void) strcpy(one, ", ");
  check(strtok(one, ", ") == NULL, 20);	/* No tokens. */
  (void) strcpy(one, "");
  check(strtok(one, ", ") == NULL, 21);	/* Empty string. */
  (void) strcpy(one, "abc");
  equal(strtok(one, ", "), "abc", 22);	/* No delimiters. */
  check(strtok((char *)NULL, ", ") == NULL, 23);
  (void) strcpy(one, "abc");
  equal(strtok(one, ""), "abc", 24);	/* Empty delimiter list. */
  check(strtok((char *)NULL, "") == NULL, 25);
  (void) strcpy(one, "abcdefgh");
  (void) strcpy(one, "a,b,c");
  equal(strtok(one, ","), "a", 26);	/* Basics again... */
  equal(strtok((char *)NULL, ","), "b", 27);
  equal(strtok((char *)NULL, ","), "c", 28);
  check(strtok((char *)NULL, ",") == NULL, 29);
  equal(one+6, "gh", 30);			/* Stomped past end? */
  equal(one, "a", 31);			/* Stomped old tokens? */
  equal(one+2, "b", 32);
  equal(one+4, "c", 33);
}

void
test_strtok_r (void)
{
  it = "strtok_r";
  (void) strcpy(one, "first, second, third");
  equal(strtok_r(one, ", ", &cp), "first", 1);	/* Basic test. */
  equal(one, "first", 2);
  equal(strtok_r((char *)NULL, ", ", &cp), "second", 3);
  equal(strtok_r((char *)NULL, ", ", &cp), "third", 4);
  check(strtok_r((char *)NULL, ", ", &cp) == NULL, 5);
  (void) strcpy(one, ", first, ");
  equal(strtok_r(one, ", ", &cp), "first", 6);	/* Extra delims, 1 tok. */
  check(strtok_r((char *)NULL, ", ", &cp) == NULL, 7);
  (void) strcpy(one, "1a, 1b; 2a, 2b");
  equal(strtok_r(one, ", ", &cp), "1a", 8);	/* Changing delim lists. */
  equal(strtok_r((char *)NULL, "; ", &cp), "1b", 9);
  equal(strtok_r((char *)NULL, ", ", &cp), "2a", 10);
  (void) strcpy(two, "x-y");
  equal(strtok_r(two, "-", &cp), "x", 11);	/* New string before done. */
  equal(strtok_r((char *)NULL, "-", &cp), "y", 12);
  check(strtok_r((char *)NULL, "-", &cp) == NULL, 13);
  (void) strcpy(one, "a,b, c,, ,d");
  equal(strtok_r(one, ", ", &cp), "a", 14);	/* Different separators. */
  equal(strtok_r((char *)NULL, ", ", &cp), "b", 15);
  equal(strtok_r((char *)NULL, " ,", &cp), "c", 16);	/* Permute list too. */
  equal(strtok_r((char *)NULL, " ,", &cp), "d", 17);
  check(strtok_r((char *)NULL, ", ", &cp) == NULL, 18);
  check(strtok_r((char *)NULL, ", ", &cp) == NULL, 19);	/* Persistence. */
  (void) strcpy(one, ", ");
  check(strtok_r(one, ", ", &cp) == NULL, 20);	/* No tokens. */
  (void) strcpy(one, "");
  check(strtok_r(one, ", ", &cp) == NULL, 21);	/* Empty string. */
  (void) strcpy(one, "abc");
  equal(strtok_r(one, ", ", &cp), "abc", 22);	/* No delimiters. */
  check(strtok_r((char *)NULL, ", ", &cp) == NULL, 23);
  (void) strcpy(one, "abc");
  equal(strtok_r(one, "", &cp), "abc", 24);	/* Empty delimiter list. */
  check(strtok_r((char *)NULL, "", &cp) == NULL, 25);
  (void) strcpy(one, "abcdefgh");
  (void) strcpy(one, "a,b,c");
  equal(strtok_r(one, ",", &cp), "a", 26);	/* Basics again... */
  equal(strtok_r((char *)NULL, ",", &cp), "b", 27);
  equal(strtok_r((char *)NULL, ",", &cp), "c", 28);
  check(strtok_r((char *)NULL, ",", &cp) == NULL, 29);
  equal(one+6, "gh", 30);			/* Stomped past end? */
  equal(one, "a", 31);			/* Stomped old tokens? */
  equal(one+2, "b", 32);
  equal(one+4, "c", 33);
}

void
test_strsep (void)
{
  it = "strsep";
  cp = strcpy(one, "first, second, third");
  equal(strsep(&cp, ", "), "first", 1);	/* Basic test. */
  equal(one, "first", 2);
  equal(strsep(&cp, ", "), "", 3);
  equal(strsep(&cp, ", "), "second", 4);
  equal(strsep(&cp, ", "), "", 5);
  equal(strsep(&cp, ", "), "third", 6);
  check(strsep(&cp, ", ") == NULL, 7);
  cp = strcpy(one, ", first, ");
  equal(strsep(&cp, ", "), "", 8);
  equal(strsep(&cp, ", "), "", 9);
  equal(strsep(&cp, ", "), "first", 10);	/* Extra delims, 1 tok. */
  equal(strsep(&cp, ", "), "", 11);
  equal(strsep(&cp, ", "), "", 12);
  check(strsep(&cp, ", ") == NULL, 13);
  cp = strcpy(one, "1a, 1b; 2a, 2b");
  equal(strsep(&cp, ", "), "1a", 14);	/* Changing delim lists. */
  equal(strsep(&cp, ", "), "", 15);
  equal(strsep(&cp, "; "), "1b", 16);
  equal(strsep(&cp, ", "), "", 17);
  equal(strsep(&cp, ", "), "2a", 18);
  cp = strcpy(two, "x-y");
  equal(strsep(&cp, "-"), "x", 19);	/* New string before done. */
  equal(strsep(&cp, "-"), "y", 20);
  check(strsep(&cp, "-") == NULL, 21);
  cp = strcpy(one, "a,b, c,, ,d ");
  equal(strsep(&cp, ", "), "a", 22);	/* Different separators. */
  equal(strsep(&cp, ", "), "b", 23);
  equal(strsep(&cp, " ,"), "", 24);
  equal(strsep(&cp, " ,"), "c", 25);	/* Permute list too. */
  equal(strsep(&cp, " ,"), "", 26);
  equal(strsep(&cp, " ,"), "", 27);
  equal(strsep(&cp, " ,"), "", 28);
  equal(strsep(&cp, " ,"), "d", 29);
  equal(strsep(&cp, " ,"), "", 30);
  check(strsep(&cp, ", ") == NULL, 31);
  check(strsep(&cp, ", ") == NULL, 32);	/* Persistence. */
  cp = strcpy(one, ", ");
  equal(strsep(&cp, ", "), "", 33);
  equal(strsep(&cp, ", "), "", 34);
  equal(strsep(&cp, ", "), "", 35);
  check(strsep(&cp, ", ") == NULL, 36);	/* No tokens. */
  cp = strcpy(one, "");
  equal(strsep(&cp, ", "), "", 37);
  check(strsep(&cp, ", ") == NULL, 38);	/* Empty string. */
  cp = strcpy(one, "abc");
  equal(strsep(&cp, ", "), "abc", 39);	/* No delimiters. */
  check(strsep(&cp, ", ") == NULL, 40);
  cp = strcpy(one, "abc");
  equal(strsep(&cp, ""), "abc", 41);	/* Empty delimiter list. */
  check(strsep(&cp, "") == NULL, 42);
  (void) strcpy(one, "abcdefgh");
  cp = strcpy(one, "a,b,c");
  equal(strsep(&cp, ","), "a", 43);	/* Basics again... */
  equal(strsep(&cp, ","), "b", 44);
  equal(strsep(&cp, ","), "c", 45);
  check(strsep(&cp, ",") == NULL, 46);
  equal(one+6, "gh", 47);		/* Stomped past end? */
  equal(one, "a", 48);			/* Stomped old tokens? */
  equal(one+2, "b", 49);
  equal(one+4, "c", 50);

#if 0
  {
    char text[] = "This,is,a,test";
    char *list = strdupa (text);
    equal (strsep (&list, ","), "This", 51);
    equal (strsep (&list, ","), "is", 52);
    equal (strsep (&list, ","), "a", 53);
    equal (strsep (&list, ","), "test", 54);
    check (strsep (&list, ",") == NULL, 55);
  }
#endif

  cp = strcpy(one, "a,b, c,, ,d,");
  equal(strsep(&cp, ","), "a", 56);	/* Different separators. */
  equal(strsep(&cp, ","), "b", 57);
  equal(strsep(&cp, ","), " c", 58);	/* Permute list too. */
  equal(strsep(&cp, ","), "", 59);
  equal(strsep(&cp, ","), " ", 60);
  equal(strsep(&cp, ","), "d", 61);
  equal(strsep(&cp, ","), "", 62);
  check(strsep(&cp, ",") == NULL, 63);
  check(strsep(&cp, ",") == NULL, 64);	/* Persistence. */

  cp = strcpy(one, "a,b, c,, ,d,");
  equal(strsep(&cp, "xy,"), "a", 65);	/* Different separators. */
  equal(strsep(&cp, "x,y"), "b", 66);
  equal(strsep(&cp, ",xy"), " c", 67);	/* Permute list too. */
  equal(strsep(&cp, "xy,"), "", 68);
  equal(strsep(&cp, "x,y"), " ", 69);
  equal(strsep(&cp, ",xy"), "d", 70);
  equal(strsep(&cp, "xy,"), "", 71);
  check(strsep(&cp, "x,y") == NULL, 72);
  check(strsep(&cp, ",xy") == NULL, 73);	/* Persistence. */
}

void
test_memcmp (void)
{
  it = "memcmp";
  check(memcmp("a", "a", 1) == 0, 1);		/* Identity. */
  check(memcmp("abc", "abc", 3) == 0, 2);	/* Multicharacter. */
  check(memcmp("abcd", "abce", 4) < 0, 3);	/* Honestly unequal. */
  check(memcmp("abce", "abcd", 4) > 0, 4);
  check(memcmp("alph", "beta", 4) < 0, 5);
  check(memcmp("a\203", "a\003", 2) > 0, 6);
  check(memcmp("abce", "abcd", 3) == 0, 7);	/* Count limited. */
  check(memcmp("abc", "def", 0) == 0, 8);	/* Zero count. */
}

void
test_memchr (void)
{
  it = "memchr";
  check(memchr("abcd", 'z', 4) == NULL, 1);	/* Not found. */
  (void) strcpy(one, "abcd");
  check(memchr(one, 'c', 4) == one+2, 2);	/* Basic test. */
  check(memchr(one, ~0xff|'c', 4) == one+2, 2);	/* ignore highorder bits. */
  check(memchr(one, 'd', 4) == one+3, 3);	/* End of string. */
  check(memchr(one, 'a', 4) == one, 4);	/* Beginning. */
  check(memchr(one, '\0', 5) == one+4, 5);	/* Finding NUL. */
  (void) strcpy(one, "ababa");
  check(memchr(one, 'b', 5) == one+1, 6);	/* Finding first. */
  check(memchr(one, 'b', 0) == NULL, 7);	/* Zero count. */
  check(memchr(one, 'a', 1) == one, 8);	/* Singleton case. */
  (void) strcpy(one, "a\203b");
  check(memchr(one, 0203, 3) == one+1, 9);	/* Unsignedness. */

  /* now test all possible alignment and length combinations to catch
     bugs due to unrolled loops (assuming unrolling is limited to no
     more than 128 byte chunks: */
  {
    char buf[128 + sizeof(long)];
    long align, len, i, pos;

    for (align = 0; align < (long) sizeof(long); ++align) {
      for (len = 0; len < (long) (sizeof(buf) - align); ++len) {
	for (i = 0; i < len; ++i) {
	  buf[align + i] = 'x';		/* don't depend on memset... */
	}
	for (pos = 0; pos < len; ++pos) {
#if 0
	  printf("align %d, len %d, pos %d\n", align, len, pos);
#endif
	  check(memchr(buf + align, 'x', len) == buf + align + pos, 10);
	  check(memchr(buf + align, 'x', pos) == NULL, 11);
	  buf[align + pos] = '-';
	}
      }
    }
  }
}

void
test_memcpy (void)
{
  it = "memcpy";
  check(memcpy(one, "abc", 4) == one, 1);	/* Returned value. */
  equal(one, "abc", 2);			/* Did the copy go right? */

  (void) strcpy(one, "abcdefgh");
  (void) memcpy(one+1, "xyz", 2);
  equal(one, "axydefgh", 3);		/* Basic test. */

  (void) strcpy(one, "abc");
  (void) memcpy(one, "xyz", 0);
  equal(one, "abc", 4);			/* Zero-length copy. */

  (void) strcpy(one, "hi there");
  (void) strcpy(two, "foo");
  (void) memcpy(two, one, 9);
  equal(two, "hi there", 5);		/* Just paranoia. */
  equal(one, "hi there", 6);		/* Stomped on source? */
}

void
test_memmove (void)
{
  it = "memmove";
  check(memmove(one, "abc", 4) == one, 1);	/* Returned value. */
  equal(one, "abc", 2);			/* Did the copy go right? */

  (void) strcpy(one, "abcdefgh");
  (void) memmove(one+1, "xyz", 2);
  equal(one, "axydefgh", 3);		/* Basic test. */

  (void) strcpy(one, "abc");
  (void) memmove(one, "xyz", 0);
  equal(one, "abc", 4);			/* Zero-length copy. */

  (void) strcpy(one, "hi there");
  (void) strcpy(two, "foo");
  (void) memmove(two, one, 9);
  equal(two, "hi there", 5);		/* Just paranoia. */
  equal(one, "hi there", 6);		/* Stomped on source? */

  (void) strcpy(one, "abcdefgh");
  (void) memmove(one+1, one, 9);
  equal(one, "aabcdefgh", 7);		/* Overlap, right-to-left. */

  (void) strcpy(one, "abcdefgh");
  (void) memmove(one+1, one+2, 7);
  equal(one, "acdefgh", 8);		/* Overlap, left-to-right. */

  (void) strcpy(one, "abcdefgh");
  (void) memmove(one, one, 9);
  equal(one, "abcdefgh", 9);		/* 100% overlap. */
}

void
test_memccpy (void)
{
  /* First test like memcpy, then the search part The SVID, the only
     place where memccpy is mentioned, says overlap might fail, so we
     don't try it.  Besides, it's hard to see the rationale for a
     non-left-to-right memccpy.  */
  it = "memccpy";
  check(memccpy(one, "abc", 'q', 4) == NULL, 1);	/* Returned value. */
  equal(one, "abc", 2);			/* Did the copy go right? */

  (void) strcpy(one, "abcdefgh");
  (void) memccpy(one+1, "xyz", 'q', 2);
  equal(one, "axydefgh", 3);		/* Basic test. */

  (void) strcpy(one, "abc");
  (void) memccpy(one, "xyz", 'q', 0);
  equal(one, "abc", 4);			/* Zero-length copy. */

  (void) strcpy(one, "hi there");
  (void) strcpy(two, "foo");
  (void) memccpy(two, one, 'q', 9);
  equal(two, "hi there", 5);		/* Just paranoia. */
  equal(one, "hi there", 6);		/* Stomped on source? */

  (void) strcpy(one, "abcdefgh");
  (void) strcpy(two, "horsefeathers");
  check(memccpy(two, one, 'f', 9) == two+6, 7);	/* Returned value. */
  equal(one, "abcdefgh", 8);		/* Source intact? */
  equal(two, "abcdefeathers", 9);		/* Copy correct? */

  (void) strcpy(one, "abcd");
  (void) strcpy(two, "bumblebee");
  check(memccpy(two, one, 'a', 4) == two+1, 10);	/* First char. */
  equal(two, "aumblebee", 11);
  check(memccpy(two, one, 'd', 4) == two+4, 12);	/* Last char. */
  equal(two, "abcdlebee", 13);
  (void) strcpy(one, "xyz");
  check(memccpy(two, one, 'x', 1) == two+1, 14);	/* Singleton. */
  equal(two, "xbcdlebee", 15);
}

void
test_memset (void)
{
  it = "memset";
  (void) strcpy(one, "abcdefgh");
  check(memset(one+1, 'x', 3) == one+1, 1);	/* Return value. */
  equal(one, "axxxefgh", 2);		/* Basic test. */

  (void) memset(one+2, 'y', 0);
  equal(one, "axxxefgh", 3);		/* Zero-length set. */

  (void) memset(one+5, 0, 1);
  equal(one, "axxxe", 4);			/* Zero fill. */
  equal(one+6, "gh", 5);			/* And the leftover. */

  (void) memset(one+2, 010045, 1);
  equal(one, "ax\045xe", 6);		/* Unsigned char convert. */

  /* Test for more complex versions of memset, for all alignments and
     lengths up to 256. This test takes a little while, perhaps it should
     be made weaker? */
  {
    char data[512];
    int i;
    int j;
    int k;
    int c;

    for (i = 0; i < 512; i++)
      data[i] = 'x';
    for (c = 0; c <= 'y'; c += 'y')  /* check for memset(,0,) and
					memset(,'y',) */
      for (j = 0; j < 256; j++)
	for (i = 0; i < 256; i++)
	  {
	    memset(data+i,c,j);
	    for (k = 0; k < i; k++)
	      if (data[k] != 'x')
		goto fail;
	    for (k = i; k < i+j; k++)
	      {
		if (data[k] != c)
		  goto fail;
		data[k] = 'x';
	      }
	    for (k = i+j; k < 512; k++)
	      if (data[k] != 'x')
		goto fail;
	    continue;

	  fail:
	    check(0,7+i+j*256+(c != 0)*256*256);
	  }
  }
}

void
test_bcopy (void)
{
  /* Much like memcpy.  Berklix manual is silent about overlap, so
     don't test it.  */
  it = "bcopy";
  (void) bcopy("abc", one, 4);
  equal(one, "abc", 1);			/* Simple copy. */

  (void) strcpy(one, "abcdefgh");
  (void) bcopy("xyz", one+1, 2);
  equal(one, "axydefgh", 2);		/* Basic test. */

  (void) strcpy(one, "abc");
  (void) bcopy("xyz", one, 0);
  equal(one, "abc", 3);			/* Zero-length copy. */

  (void) strcpy(one, "hi there");
  (void) strcpy(two, "foo");
  (void) bcopy(one, two, 9);
  equal(two, "hi there", 4);		/* Just paranoia. */
  equal(one, "hi there", 5);		/* Stomped on source? */
}

void
test_bzero (void)
{
  it = "bzero";
  (void) strcpy(one, "abcdef");
  bzero(one+2, 2);
  equal(one, "ab", 1);			/* Basic test. */
  equal(one+3, "", 2);
  equal(one+4, "ef", 3);

  (void) strcpy(one, "abcdef");
  bzero(one+2, 0);
  equal(one, "abcdef", 4);		/* Zero-length copy. */
}

void
test_bcmp (void)
{
  it = "bcmp";
  check(bcmp("a", "a", 1) == 0, 1);	/* Identity. */
  check(bcmp("abc", "abc", 3) == 0, 2);	/* Multicharacter. */
  check(bcmp("abcd", "abce", 4) != 0, 3);	/* Honestly unequal. */
  check(bcmp("abce", "abcd", 4) != 0, 4);
  check(bcmp("alph", "beta", 4) != 0, 5);
  check(bcmp("abce", "abcd", 3) == 0, 6);	/* Count limited. */
  check(bcmp("abc", "def", 0) == 0, 8);	/* Zero count. */
}

void
test_strerror (void)
{
  int f;
  it = "strerror";
  f = __open("/", O_WRONLY);	/* Should always fail. */
  check(f < 0 && errno > 0 && errno < _sys_nerr, 1);
  equal(strerror(errno), _sys_errlist[errno], 2);
}

int
main (void)
{
  int status;

  /* Test strcmp first because we use it to test other things.  */
  test_strcmp ();

  /* Test strcpy next because we need it to set up other tests.  */
  test_strcpy ();

  /* A closely related function is stpcpy.  */
  test_stpcpy ();

  /* stpncpy.  */
  test_stpncpy ();

  /* strcat.  */
  test_strcat ();

  /* strncat.  */
  test_strncat ();

  /* strncmp.  */
  test_strncmp ();

  /* strncpy.  */
  test_strncpy ();

  /* strlen.  */
  test_strlen ();

  /* strchr.  */
  test_strchr ();

  /* index - just like strchr.  */
  test_index ();

  /* strrchr.  */
  test_strrchr ();

  /* rindex - just like strrchr.  */
  test_rindex ();

  /* strpbrk - somewhat like strchr.  */
  test_strpbrk ();

  /* strstr - somewhat like strchr.  */
  test_strstr ();

  /* strspn.  */
  test_strspn ();

  /* strcspn.  */
  test_strcspn ();

  /* strtok - the hard one.  */
  test_strtok ();

  /* strtok_r.  */
  test_strtok_r ();

  /* strsep.  */
  test_strsep ();

  /* memcmp.  */
  test_memcmp ();

  /* memchr.  */
  test_memchr ();

  /* memcpy - need not work for overlap.  */
  test_memcpy ();

  /* memmove - must work on overlap.  */
  test_memmove ();

  /* memccpy.  */
  test_memccpy ();

  /* memset.  */
  test_memset ();

  /* bcopy.  */
  test_bcopy ();

  /* bzero.  */
  test_bzero ();

  /* bcmp - somewhat like memcmp.  */
  test_bcmp ();

  /* strerror - VERY system-dependent.  */
  test_strerror ();


  if (errors == 0)
    {
      status = EXIT_SUCCESS;
      puts("No errors.");
    }
  else
    {
      status = EXIT_FAILURE;
      printf("%Zd errors.\n", errors);
    }
  exit(status);
}
