/* GNU Pascal compiler option tables
   Copyright (C) 2000, Free Software Foundation, Inc.

   Used by the preprocessor and the compiler in common.

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
02111-1307, USA.  */

/*
 * Authors: Frank Heckenbach <frank@g-n-u.de>
 *          Peter Gerwinski <peter@gerwinski.de>
 */

#ifndef _GPC_OPTIONS_H
#define _GPC_OPTIONS_H

/* @@One-letter BP/Delphi directives still missing: H, P, Q, R, V */
/* $W means warnings in GPC, but something different in BP, so we
   ignore it in BP mode. */
static const struct gpc_short_option
{
  char  short_name;
  int   bp_option;
  char *long_name;
  char *inverted_long_name;
} gpc_short_options[] =
  {
    { 'b', 1, "-fno-short-circuit", "-fshort-circuit"      },
    { 'i', 1, "-fio-checking",      "-fno-io-checking"     },
    { 's', 1, "-fstack-checking",   "-fno-stack-checking"  },
    { 't', 1, "-ftyped-address",    "-fno-typed-address"   },
    { 'w', 0, "-Wwarnings",         "-Wno-warnings"        },
    { 'x', 1, "-fextended-syntax",  "-fno-extended-syntax" },
    /* The followinng options are ignored, but still listed here so
       they can be supported in `ifopt'. The "long options" must be
       unique and start with `-!'. */
    { 'a', 1, "-!a", "-!no-a" },
    { 'd', 1, "-!d", "-!no-d" },
    { 'e', 1, "-!e", "-!no-e" },
    { 'f', 1, "-!f", "-!no-f" },
    { 'g', 1, "-!g", "-!no-g" },
    { 'k', 1, "-!k", "-!no-k" },
    { 'l', 1, "-!l", "-!no-l" },
    { 'n', 1, "-!n", "-!no-n" },
    { 'o', 1, "-!o", "-!no-o" },
    { 'w', 1, "-!w", "-!no-w" }, /* outside of BP mode, the `w' above takes precedence */
    { 'y', 1, "-!y", "-!no-y" },
    { 0, 0, 0, 0 }
  };

static char *default_options[] =
  {
    "-fgnu-pascal",
    "-fno-extended-syntax",
    "-fno-nested-comments",
    "-fno-ignore-function-results",
    "-ftruncate-strings",
    "-fexact-compare-strings",
    "-fio-checking",
    "-fwrite-real-blank",
    "-fno-transparent-file-names",
    "-fno-pedantic",
    "-fno-stack-checking",
    "-ftyped-address",
    "-Wwarnings",
    "-Wfield-name-problem",
    "-Wimplicit",
    "-Wno-write-strings",
    "-Wno-cast-qual",
    "-Wno-bad-function-cast",
    "-Wno-pointer-arith",
    "-Wno-strict-prototypes",
    "-Wno-missing-prototypes",
    "-Wno-missing-declarations",
    "-Wno-redundant-decls",
    "-Wno-nested-externs",
    "-Wno-traditional",
    "-Wno-format",
    "-Wno-char-subscripts",
    "-Wno-conversion",
    "-Wno-parentheses",
    "-Wno-return-type",
    "-Wno-comment",
    "-Wundef",
    "-Wimport",
    "-Wno-missing-braces",
    "-Wno-main",
    0
  };

static const struct lang_option_map
{
  /* The limits here are hard-coded because otherwise the
     initializer doesn't compile. The limits can be increased when
     necessary. */
  char *src [5];
  char *dest [11];
} lang_option_map[] =
  {
    {
      {
        "-fstandard-pascal-level-0",
        "-fstandard-pascal",
        "-fextended-pascal",
        "-fobject-pascal",
        0
      },
      {
        "-fmixed-comments",
        "-fno-delphi-comments",
        "-fno-short-circuit",
        "-fwrite-clip-strings",
        "-fno-write-capital-exponent",
        "-fno-exact-compare-strings",
        "-fno-borland-char-constants",
        "-ffield-widths",
        "-Wtyped-const",
        "-Wnear-far",
        0
      }
    },
    {
      {
        "-fborland-pascal",
        "-fdelphi",
        0
      },
      {
        "-fno-macros",
        "-fno-mixed-comments",
        "-fno-write-clip-strings",
        "-fwrite-capital-exponent",
        "-fexact-compare-strings",
        "-fborland-char-constants",
        "-fno-field-widths",
        "-Wno-typed-const",
        "-Wno-near-far",
        0
      }
    },
    {
      {
        "-fborland-pascal",
        0
      },
      {
        "-fno-delphi-comments",
        0
      }
    },
    {
      {
        "-fdelphi",
        0
      },
      {
        "-fdelphi-comments",
        0
      }
    },
    {
      {
        "-fgnu-pascal",
        0
      },
      {
        "-fmacros",
        "-fno-mixed-comments",
        "-fdelphi-comments",
        "-fshort-circuit",
        "-fwrite-clip-strings",
        "-fno-write-capital-exponent",
        "-fborland-char-constants",
        "-fno-field-widths",
        "-Wtyped-const",
        "-Wnear-far",
        0
      }
    },
    {
      {
        "-Wall",
        0
      },
      {
        "-Wimplicit-int",
        "-Wimplicit-function-declaration",
        "-Wreturn-type",
        "-Wformat",
        "-Wchar-subscripts",
        "-Wparentheses",
        "-Wmissing-braces",
        "-Wtyped-const",
        "-Wnear-far",
        0
      }
    },
    { { 0 }, { 0 } }
  };

#endif
