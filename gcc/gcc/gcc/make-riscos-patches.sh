#!/bin/bash
# The purpose of this script is to aid the developer who ports
# GCC to re-generate the RISC OS specific patches after a new
# import of the master sources has been completed.

# Patches are placed in the riscos.patches directory to allow
# the developer (or other developers) to keep track of what
# gets changed so nothing gets missed for future upgrades.

# This script is intentionally left basic as only one person
# would really need to use this at any one time.
masterdir=/home/nick/riscos/masters/gcc-3.4
for file in builtins.c c-opts.c cppdefault.c cppdefault.h diagnostic.c function.c gcc.c gcc.h c.opt global.c reload1.c
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
