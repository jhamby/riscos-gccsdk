#!/bin/bash
# Convert UnixLib RISC OS-style source tree into Unix style
# Copyright (c) 2001 Nick Burrett (nick@dsvr.net)
#
# Run this in unixlib/source.

# Remove CVS directories
find . -name "CVS" | xargs rm -rf

c_dirs=`find . -name "c"`
h_dirs=`find . -name "h"`
s_dirs=`find . -name "s"`

for dir in $c_dirs; do
  for file in $dir/*; do
    name=`basename $file`
    mv $file $dir/../$name.c
  done
  rm -rf $dir
done

for dir in $h_dirs; do
  for file in $dir/*; do
    name=`basename $file`
    mv $file $dir/../$name.h
  done
  rm -rf $dir
done

for dir in $s_dirs; do
  for file in $dir/*; do
    name=`basename $file`
    sed -e "s!UnixHdr:unixlib.s.asm_dec!clib/unixlib/asm_dec.s!" $file >$dir/../$name.s
    rm $file
  done
  rm -rf $dir
done
