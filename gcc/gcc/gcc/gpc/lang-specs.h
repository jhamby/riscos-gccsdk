/* Definitions for specs for Pascal.
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
along with GNU Pascal; see the file COPYING. If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. */

#include "gpc/version.h"

/* This is the contribution to the `default_compilers' array in gcc.c for
   GNU Pascal. */

  {".pas", {"@Pascal"}},
  {".p", {"@Pascal"}},
  {".pp", {"@Pascal"}},
  {".dpr", {"@Pascal"}},
  {"@pascal", {"@Pascal"}},
  {"@Pascal",
   {"gpcpp -lang-pascal %{nostdinc*} %{C} %{v} %{A*} %{I*} %{P} %I %{f*}\
        %{!fstandard-pascal*:%{!fextended-pascal:%{!fobject-pascal:\
          %{!fborland-pascal:%{!fno-delphi-comments:-fdelphi-comments}}}}}\
        %{fstandard-pascal*:-D__STANDARD_PASCAL__ %{!fno-mixed-comments:-fmixed-comments}}\
        %{fextended-pascal:-D__EXTENDED_PASCAL__ %{!fno-mixed-comments:-fmixed-comments}}\
        %{fobject-pascal:-D__OBJECT_PASCAL__ %{!fno-mixed-comments:-fmixed-comments}}\
        %{fborland-pascal:-D__BORLAND_PASCAL__ %{!fmacros:-fno-macros}}\
        %{fdelphi:-D__DELPHI__ %{!fmacros:-fno-macros}}\
        %{fpascal-sc:-D__PASCAL_SC__}\
        %{!fstandard-pascal*:%{!fextended-pascal:%{!fobject-pascal:%{!fborland-pascal:%{!fdelphi:%{!pascal-sc:-D__GNU_PASCAL__}}}}}}\
        %{C:%{!E:%eGNU Pascal does not support `-C' without using `-E'}}\
        %{M} %{MM} %{MD:-MD %b.d} %{MMD:-MMD %b.d} %{MG}\
        -undef -D__GNUC__=%v1 -D__GNUC_MINOR__=%v2"
        " -D__GPC__=" GPC_MAJOR
        " -D__GPC_MINOR__=" GPC_MINOR
        " -D__GPC_RELEASE__=" GPC_VERSION_STRING
#if BITS_BIG_ENDIAN
       " -D__BITS_BIG_ENDIAN__=1"
#else
       " -D__BITS_LITTLE_ENDIAN__=1"
#endif
#if BYTES_BIG_ENDIAN
       " -D__BYTES_BIG_ENDIAN__=1"
#else
       " -D__BYTES_LITTLE_ENDIAN__=1"
#endif
#if WORDS_BIG_ENDIAN
       " -D__WORDS_BIG_ENDIAN__=1"
#else
       " -D__WORDS_LITTLE_ENDIAN__=1"
#endif
#if STRICT_ALIGNMENT
       " -D__NEED_ALIGNMENT__=1"
#endif
       " %{!undef: %p %P} %c %{O*:%{!O0:-D__OPTIMIZE__}} %{traditional} %{ftraditional:-traditional}\
        %{traditional-cpp:-traditional} %{trigraphs}\
        %{g*} %{W*} %{w} %{pedantic*} %{H} %{d*} %C %{D*} %{U*} %{i*} %Z\
        %i %{!M:%{!MM:%{!E:%{!pipe:%g.i}}}}%{E:%W{o*}}%{M:%W{o*}}%{MM:%W{o*}} |\n",
   "%{!M:%{!MM:%{!E:gpc1    %{!pipe:%g.i} %1\
                            %{!Q:-quiet} -dumpbase %b.pas %{d*} %{m*} %{a}\
                            %{finterface-only:-fsyntax-only}\
                            %{g*} %{O*} %{W*} %{w} %{pedantic*} %{iso}\
                            %{traditional} %{v:-version} %{pg:-p} %{p}"
#ifdef GPC
                          " %{!famtmpfile*:%eInternal GPC problem: internal option `--amtmpfile' not given}"
#endif /* GPC */
                           "%{f*} %{+e*} %{aux-info*}\
                            %{pg:%{fomit-frame-pointer:%e-pg and -fomit-frame-pointer are incompatible}}\
                            %{S:%W{o*}%{!o*:-o %b.s}}%{!S:-o %{|!pipe:%g.s}}|\n\
              %{!finterface-only:%{!S:as %{R} %{j} %{J} %{h} %{d2} %a %Y\
                      %{c:%W{o*}%{!o*:-o %w%b%O}}%{!c:-o %d%w%u%O}\
                      %{!pipe:%g.s} %A\n }}}}}"}},
