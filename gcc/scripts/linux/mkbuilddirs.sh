#!/bin/sh
# Make build directory structure for RISC OS SDK
#
objdir=~/arm-riscos-aof
srcdir=~/gccsdk
mkdir ${objdir}

(cd $srcdir/riscos; mkdir ${objdir}/riscos;
 for dir in *; do if [ -d $dir ]; then mkdir $objdir/riscos/$dir; fi; done)
(cd $srcdir/riscos-aof; mkdir $objdir/riscos-aof;
 for dir in *; do if [ -d $dir ]; then mkdir $objdir/riscos-aof/$dir; fi; done)

