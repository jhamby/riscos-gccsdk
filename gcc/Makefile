# Top-level Makefile for the GNU Compiler Collection for ARM/RISC OS
# Copyright (c) 2000 Nick Burrett
# Written by Nick Burrett <nick@dsvr.net>

# Set to `yes' if we are cross-compiling
crosscompile=yes

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
ifeq ($(crosscompile),yes)
gccpkg=/home/riscos/dist
standard_exec_prefix=$(gccpkg)/bin/
standard_startfile_prefix=$(gccpkg)/startup/
gcc_include_dir=$(gccpkg)/lib/gcc-lib/$(TARGET_NAME)/$(VERSION)/include
gplusplus_include_dir=$(gccpkg)/lib/gcc-lib/$(TARGET_NAME)/$(VERSION)/include/g++-3
cross_include_dir=$(gccpkg)/include
local_include_dir=
system_include_dir=

# Used in GCC's makefile
hostsystem=HOST_I386_LINUX
else
standard_exec_prefix="gccpkg:bin/"
standard_startfile_prefix="gccpkg:startup/"
gcc_include_dir="gccpkg:lib/gcc-lib/$(TARGET_NAME)/$(VERSION)/include"
gplusplus_include_dir="gccpkg:lib/gcc-lib/$(TARGET_NAME)/$(VERSION)/include/g++-3"
local_include_dir="$(gccpkg)/local/include"
system_include_dir="$(gccpkg)/sys/include"
cross_include_dir=

# Used in GCC's makefile
hostsystem=HOST_ARM_RISCOS
endif

export standard_exec_prefix standard_startfile_prefix
export gcc_include_dir gplusplus_include_dir
export local_include_dir system_include_dir cross_include_dir
export hostsystem

# Path to root of source tree
srcdir=/home/riscos/gccsdk

# For the RISC OS gcc compiler
#OPTS = -mthrowback -mpoke-function-name
# Crazy ass optimisations that free up another 2 registers.  Doesn't work
# properly yet.
#OPTS = -mthrowback -mno-apcs-frame -mno-apcs-stack-check -muse-arg-pointer

# Where object files are stored
objdir=/home/riscos/$(TARGET_NAME)

# Where resulting binaries will be placed
distdir=/home/riscos/dist

# Path to where we place non-target specific compiled binaries
BIN = $(distdir)/bin

# Path to the location target specific compiled binaries.
TBIN = $(distdir)/bin/$(TARGET_NAME)/$(VERSION)

ifeq ($(crosscompile),yes)
TARGET_DEFS += -DCROSS_COMPILE
AR = ar
AR_FLAGS = -c
else
AR = libfile
AR_FLAGS = -c
endif

# Pass these variables through the environment to sub-makes.
export srcdir objdir distdir VERSION TARGET_NAME TARGET_DEFS OPTS BIN TBIN
export CFLAGS AR AR_FLAGS crosscompile

all:	crosshost crosstarget

# Tools that will run natively in any environment
# i.e. under Linux this will build the GCC cross-compiler, cross-assembler
# and cross-linker.  Under RISC OS this will build the compiler, assembler
# and linker natively
crosshost:	libiberty riscos-aof gcc

# These targets build the libraries and binaries suitable for execution
# on RISC OS machines only.
crosstarget:	libgcc unixlib libscl libio libstdc libf2c libgpc riscos-progs riscos-libs

.PHONY: libgcc
.PHONY: libgpc
.PHONY: unixlib
.PHONY: libscl
.PHONY: libiberty
.PHONY: riscos-progs
.PHONY: riscos-libs
.PHONY: riscos-aof
.PHONY: riscos-elf
.PHONY: gcc


# Target (trgXX) and compiler flags (cflXX) definitions.  The XX must
# match for both trg and cfl in all calls to sub-make processes.  We
# write them like this because it makes the rest of the makefile easy
# to read.
trg10="TRG=apcs26/hard/arch2/unixlib"
trg11="TRG=apcs26/hard/arch3/unixlib"
trg12="TRG=apcs26/hard/arch4/unixlib"
trg13="TRG=apcs26/soft/arch2/unixlib"
trg14="TRG=apcs26/soft/arch3/unixlib"
trg15="TRG=apcs26/soft/arch4/unixlib"

trg20="TRG=apcs32/hard/arch3/unixlib"
trg21="TRG=apcs32/hard/arch4/unixlib"
trg22="TRG=apcs32/soft/arch3/unixlib"
trg23="TRG=apcs32/soft/arch4/unixlib"

trg30="TRG=apcs26/hard/arch2/libscl"
trg31="TRG=apcs26/hard/arch3/libscl"
trg32="TRG=apcs26/hard/arch4/libscl"

trg40="TRG=apcs32/hard/arch3/libscl"
trg41="TRG=apcs32/hard/arch4/libscl"

cfl10="CFL=-mapcs-26 -mno-apcs-float -mcpu=arm2 -munixlib"
cfl11="CFL=-mapcs-26 -mno-apcs-float -mcpu=arm6 -munixlib"
cfl12="CFL=-mapcs-26 -mno-apcs-float -mcpu=strongarm -munixlib"
cfl13="CFL=-mapcs-26 -mno-apcs-float -mcpu=arm2 -munixlib -msoft-float"
cfl14="CFL=-mapcs-26 -mno-apcs-float -mcpu=arm6 -munixlib -msoft-float"
cfl15="CFL=-mapcs-26 -mno-apcs-float -mcpu=strongarm -munixlib -msoft-float"

cfl20="CFL=-mapcs-32 -mno-apcs-float -mcpu=arm6 -munixlib"
cfl21="CFL=-mapcs-32 -mno-apcs-float -mcpu=strongarm -munixlib"
cfl22="CFL=-mapcs-32 -mno-apcs-float -mcpu=arm6 -munixlib -msoft-float"
cfl23="CFL=-mapcs-32 -mno-apcs-float -mcpu=strongarm -munixlib -msoft-float"

cfl30="CFL=-mapcs-26 -mno-apcs-float -mcpu=arm2 -mlibscl"
cfl31="CFL=-mapcs-26 -mno-apcs-float -mcpu=arm6 -mlibscl"
cfl32="CFL=-mapcs-26 -mno-apcs-float -mcpu=strongarm -mlibscl"

cfl40="CFL=-mapcs-32 -mno-apcs-float -mcpu=arm6 -mlibscl"
cfl41="CFL=-mapcs-32 -mno-apcs-float -mcpu=strongarm -mlibscl"


libscl:
	$(MAKE) -C libscl $(trg30) $(cfl30)

libiberty:
	$(MAKE) -C libiberty "TRG=native"

gcc:
	$(MAKE) -C "gcc-$(VERSION)"

riscos-progs:
	$(MAKE) -C libiberty $(trg10) $(cfl10)
	$(MAKE) -C riscos-progs $(trg10) $(cfl10)

riscos-aof:
	$(MAKE) -C riscos-aof

riscos-elf:
	$(MAKE) -C riscos-elf

# The ballache of having to keep support with Objasm forces us
# to do this (CFL1) brain damage.
unixlib:
	$(MAKE) -C $@ $(trg10) $(cfl10) "CFL1=-Wa,-DAPCS32=no -Wa,-DARMFPE=2"
	$(MAKE) -C $@ $(trg11) $(cfl11) "CFL1=-Wa,-DAPCS32=no -Wa,-DARMFPE=3"
	$(MAKE) -C $@ $(trg12) $(cfl12) "CFL1=-Wa,-DAPCS32=no -Wa,-DARMFPE=3"
	$(MAKE) -C $@ $(trg13) $(cfl13) "CFL1=-Wa,-DAPCS32=no -Wa,-DARMFPE=2"
	$(MAKE) -C $@ $(trg14) $(cfl14) "CFL1=-Wa,-DAPCS32=no -Wa,-DARMFPE=3"
	$(MAKE) -C $@ $(trg15) $(cfl15) "CFL1=-Wa,-DAPCS32=no -Wa,-DARMFPE=3"

	$(MAKE) -C $@ $(trg20) $(cfl20) "CFL1=-Wa,-DAPCS32=yes -Wa,-DARMFPE=3"
	$(MAKE) -C $@ $(trg21) $(cfl21) "CFL1=-Wa,-DAPCS32=yes -Wa,-DARMFPE=3"
	$(MAKE) -C $@ $(trg22) $(cfl22) "CFL1=-Wa,-DAPCS32=yes -Wa,-DARMFPE=3"
	$(MAKE) -C $@ $(trg23) $(cfl23) "CFL1=-Wa,-DAPCS32=yes -Wa,-DARMFPE=3"

riscos-libs:
	$(MAKE) -C $@ $(trg10) $(cfl10)
	$(MAKE) -C $@ $(trg11) $(cfl11)
	#$(MAKE) -C $@ $(trg12) $(cfl12)
	$(MAKE) -C $@ $(trg13) $(cfl13)
	$(MAKE) -C $@ $(trg14) $(cfl14)
	#$(MAKE) -C $@ $(trg15) $(cfl15)

	$(MAKE) -C $@ $(trg20) $(cfl20)
	#$(MAKE) -C $@ $(trg21) $(cfl21)
	$(MAKE) -C $@ $(trg22) $(cfl22)
	#$(MAKE) -C $@ $(trg23) $(cfl23)

libgcc:
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg10) $(cfl10)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg11) $(cfl11)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg12) $(cfl12)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg13) $(cfl13)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg14) $(cfl14)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg15) $(cfl15)

	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg20) $(cfl20)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg21) $(cfl21)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg22) $(cfl22)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg23) $(cfl23)

	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg30) $(cfl30)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg31) $(cfl31)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg32) $(cfl32)

libgpc libio libf2c libstdc:
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg10) $(cfl10)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg11) $(cfl11)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg12) $(cfl12)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg13) $(cfl13)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg14) $(cfl14)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg15) $(cfl15)

	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg20) $(cfl20)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg21) $(cfl21)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg22) $(cfl22)
	$(MAKE) -C "gcc-$(VERSION)" $@ $(trg23) $(cfl23)

