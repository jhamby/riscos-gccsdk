# Toolchain definitions for building for RISC OS using the GCCSDK cross-compiler
GCCSDK_INSTALL_CROSSBIN ?= /home/riscos/cross/bin
GCCSDK_INSTALL_ENV ?= /home/riscos/env

HOST_CC := gcc

CC := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*gcc)
AR := $(wildcard $(GCCSDK_INSTALL_CROSSBIN)/*ar)
LD := $(CC)
OBJCOPY := $(GCCSDK_INSTALL_CROSSBIN)/arm-unknown-riscos-objcopy

CP := cp
RM := rm
MKDIR := mkdir
MV := mv
ECHO := echo
MAKE := make
PERL := perl
PKGCONFIG := $(GCCSDK_INSTALL_ENV)/ro-pkg-config
INSTALL := install
SED := sed
TOUCH := touch
LCOV := echo
GENHTML := echo
# Filthy hack, as cmunge has the gcc binary name hardcoded, but only looks 
# for gcc in the current path, and not in GCCSDK_INSTALL_CROSSBIN.
CMHG := PATH="$(GCCSDK_INSTALL_CROSSBIN):${PATH}" $(GCCSDK_INSTALL_CROSSBIN)/cmunge

# Toolchain flags
WARNFLAGS := -Wall -Wextra -Wundef -Wpointer-arith -Wcast-align \
	-Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes \
	-Wmissing-declarations -Wnested-externs -Werror -pedantic
CFLAGS = -std=c99 -I$(GCCSDK_INSTALL_ENV)/include $(WARNFLAGS) \
	-mpoke-function-name -mmodule
RELEASECFLAGS = $(CFLAGS) -DNDEBUG -O2
DEBUGCFLAGS = $(CFLAGS) -O0 -g
ARFLAGS := -cru
LDFLAGS = -mmodule

CPFLAGS :=
RMFLAGS := -f
MKDIRFLAGS := -p
MVFLAGS := 
ECHOFLAGS := 
MAKEFLAGS :=
PKGCONFIGFLAGS :=
TOUCHFLAGS :=
CMHGFLAGS := -p -tgcc -32bit -apcs 3/32/nonreent/fpe2/noswst/nofpr/nofp

EXEEXT := ,ffa

# Default installation prefix
PREFIX ?= $(GCCSDK_INSTALL_ENV)

TARGET := riscos

include build/Makefile.common

# And the RISC OS distribution targets

#ITEMS_DISTCLEAN := $(ITEMS_DISTCLEAN) iconv.zip iconv-pkg.zip
#
#riscos-dist: module aliases
#	@svn export riscos riscos-dist
#	@$(CP) $(CPFLAGS) riscos/!Boot/Resources/!Unicode/Files/Aliases \
#			riscos-dist/!Boot/Resources/!Unicode/Files/
#	@$(CP) $(CPFLAGS) Iconv,ffa riscos-dist/!System/310/Modules/
#	@svn export doc riscos-dist/doc
#	@$(RM) $(RMFLAGS) -r riscos-dist/doc/Standards
#	@$(CP) $(CPFLAGS) include/iconv/iconv.h riscos-dist/stubs/
#	@(cd riscos-dist ; $(GCCSDK_INSTALL_ENV)/bin/zip -9r, ../iconv.zip *)
#	@$(MV) $(MVFLAGS) riscos-dist/!Boot/Resources riscos-dist
#	@$(RM) $(RMFLAGS) -r riscos-dist/!Boot
#	@$(MV) $(MVFLAGS) riscos-dist/!System riscos-dist/System
#	@$(RM) $(RMFLAGS) -r riscos-dist/doc riscos-dist/stubs 
#	@$(RM) $(RMFLAGS) riscos-dist/ReadMe
#	@svn export riscpkg/RiscPkg riscos-dist/RiscPkg
#	@$(CP) $(CPFLAGS) COPYING riscos-dist/RiscPkg/Copyright
#	@(cd riscos-dist ; $(GCCSDK_INSTALL_ENV)/bin/zip -9r, ../iconv-pkg.zip *)
#	@$(RM) $(RMFLAGS) -r riscos-dist

