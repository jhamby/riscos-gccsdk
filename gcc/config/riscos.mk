# Configuration makefile for a ARM/RISC OS compiler for ARM/RISC OS
# Copyright (c) 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Set to `yes' if we are cross-compiling
crosscompile=no

# Define this to be the version of GCC we are compiling
VERSION = 2_95_2
# VERSION = 3_0_0

# Uncomment if we are building for ARM/RISC OS with ELF executables
#TARGET_NAME = arm-riscos-elf
#TARGET_DEFS = -DTARGET_RISCOSELF

# Uncomment if we are building for ARM/RISC OS with AOF executables
TARGET_NAME = arm-riscos-aof
TARGET_DEFS = -DTARGET_RISCOSAOF

# Installation directories
standard_exec_prefix="gccpkg:bin/"
standard_startfile_prefix="gccpkg:startup/"
gcc_bin_dir="gccpkg:bin/$(TARGET_NAME)/$(VERSION)"
gcc_include_dir="gccpkg:lib/gcc-lib/$(TARGET_NAME)/$(VERSION)/include"
gplusplus_include_dir="gccpkg:lib/gcc-lib/$(TARGET_NAME)/$(VERSION)/include/g++-3"
local_include_dir="$(gccpkg)/local/include"
system_include_dir="$(gccpkg)/sys/include"
cross_include_dir=

# Used in GCC's makefile
hostsystem=HOST_ARM_RISCOS

export standard_exec_prefix standard_startfile_prefix
export gcc_include_dir gplusplus_include_dir
export local_include_dir system_include_dir cross_include_dir
export hostsystem gcc_bin_dir

# For the RISC OS gcc compiler
#OPTS = -mthrowback -mpoke-function-name
# Crazy ass optimisations that free up another 2 registers.  Doesn't work
# properly yet.
#OPTS = -mthrowback -mno-apcs-frame -mno-apcs-stack-check -muse-arg-pointer

export OPTS
