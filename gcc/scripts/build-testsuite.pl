#!/bin/bash
# Crude script to extract GCC tests for compilation on a
# cross-compiler
gccsrc=/home/nick/riscos/gcc-3.3
testdir=/home/nick/riscos/gcc-tests

# Copy C++ execute tests
for x in `grep -l -r "dg-do run" $gccsrc/gcc/testsuite/g++.dg`; do cp $x $testdir/c++; done

# Copy C execute tests
cp $gccsrc/gcc/testsuite/gcc.c-torture/execute/*.c $testdir/c
for x in `grep -l -r "{ dg-do run }" $gccsrc/gcc/testsuite/gcc.dg`; do cp $x $testdir/c; done

# Copy libstdc execute tests
for x in `grep -l -r "{ dg-do run }" $gccsrc/libstdc++-v3/testsuite`; do cp $x $testdir/libstdc/execute; done

for x in `grep -l -r "{ dg-do compile }" $gccsrc/libstdc++-v3/testsuite`; do cp $x $testdir/libstdc/compile; done
