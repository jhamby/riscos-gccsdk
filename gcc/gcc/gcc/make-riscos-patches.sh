#!/bin/bash
#
masterdir=/home/nick/riscos/masters/gcc-3.4
for file in builtins.c c-opts.c cppdefault.c cppdefault.h gcc.c gcc.h c.opt global.c reload1.c
do
  diff -up $masterdir/gcc/$file $file > riscos.patches/$file.patch
done

for file in com.c lex.c ste.c where.c
do
  diff -up $masterdir/gcc/f/$file f77/$file > riscos.patches/$file.patch
done

for file in arm.c arm.h arm.md
do
  diff -up $masterdir/gcc/config/arm/$file config/arm/$file > riscos.patches/$file.patch
done
