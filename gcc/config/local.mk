# RISCOS GCCSDK local host configuration.
# These values are meant to be freely modifiable by the end-user.

# File paths.
# Set these to whatever you like.

# Tool directory.  All compiler binaries, standard headers and libraries
# will be installed below this directory.
gccpkg=/home/riscos/dist

# Path to the root of the source tree
srcdir=/home/riscos/gccsdk

# Where compiler, program and library object files are stored during the
# build process
objdir=/home/riscos/$(TARGET_NAME)

# Once built, compiler, program and libraries will be installed into a
# directory structure starting at this path:
distdir=/home/riscos/dist

export gccpkg srcdir objdir distdir


# Uncomment if we are hosting on GNU Linux
include linux.mk

# Uncomment if we are hosting on ARM/RISC OS
#include riscos.mk

