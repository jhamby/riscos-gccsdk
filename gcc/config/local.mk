# Change this statement to include the Makefile configuration appropriate
# to your host system.

# Uncomment if we are hosting on GNU Linux
include linux.mk

# Uncomment if we are hosting on ARM/RISC OS
#include riscos.mk


# File paths.
# Set these to whatever you like.

# Path to the root of the source tree
srcdir=/home/riscos/gccsdk

# Where compiler, program and library object files are stored during the
# build process
objdir=/home/riscos/$(TARGET_NAME)

# Once built, compiler, program and libraries will be installed into a
# directory structure starting at this path:
distdir=/home/riscos/dist

export srcdir objdir distdir
