# This file is part of RiscPkg-GenControl.
# Copyright © 2004 Graham Shaw.
# Distribution and use are subject to the GNU General Public License,
# a copy of which may be found in the file "<CommonLicences$Dir>.GPL-2"
# or at the URL "http://www.riscpkg.org/licences/GPL-2.txt".

# Makefile to cross-compile from the autobuilder

# Can use same native build of libpkg as the autobuilder packaging tools
LIBPKG_PATH=$(GCCSDK_INSTALL_ENV)/../autobuilder/toolsrc/libpkg

CC=/usr/bin/gcc
CXX=/usr/bin/g++

CPPFLAGS = -I$(LIBPKG_PATH) -MD -MP
CXXFLAGS = -Wall -W -Wno-unused -O2

OBJ = !RiscPkg-GenControl/main.o

LIB = -L$(LIBPKG_PATH) -lpkg

all: riscpkg-gencontrol

riscpkg-gencontrol: libpkg $(OBJ)
	$(CXX) -o riscpkg-gencontrol $(OBJ) $(LIB)

libpkg: $(LIBPKG_PATH)/libpkg.a

$(LIBPKG_PATH)/libpkg.a:
	make -C $(LIBPKG_PATH)

install:
	cp -f riscpkg-gencontrol $(GCCSDK_INSTALL_ENV)/bin

-include $(OBJ:.o=.d)
