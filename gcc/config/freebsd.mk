# Makefile configuration for a Linux-hosted cross-compiler for ARM/RISC OS
# Copyright (c) 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Set to `yes' if we are cross-compiling
crosscompile=yes

# Define this to be the version of GCC we are compiling
VERSION = 2_95_2
#VERSION = 3_0_0

# Installation directories
standard_exec_prefix=$(gccpkg)/bin/
gcc_bin_dir=$(gccpkg)/bin/$(TARGET_NAME)/$(VERSION)
standard_startfile_prefix=$(gccpkg)/startup/
gcc_include_dir=$(gccpkg)/lib/gcc-lib/$(TARGET_NAME)/$(VERSION)/include
gplusplus_include_dir=$(gccpkg)/lib/gcc-lib/$(TARGET_NAME)/$(VERSION)/include/g++-3
cross_include_dir=$(gccpkg)/include
local_include_dir=
system_include_dir=

# Used in GCC's makefile
hostsystem=HOST_I386_FREEBSD

export standard_exec_prefix standard_startfile_prefix
export gcc_include_dir gplusplus_include_dir
export local_include_dir system_include_dir cross_include_dir
export hostsystem gcc_bin_dir

# For the RISC OS gcc compiler
OPTS = -g
#OPTS = -mthrowback -mpoke-function-name
# Crazy ass optimisations that free up another 2 registers.  Doesn't work
# properly yet.
#OPTS = -mthrowback -mno-apcs-frame -mno-apcs-stack-check -muse-arg-pointer

export OPTS
