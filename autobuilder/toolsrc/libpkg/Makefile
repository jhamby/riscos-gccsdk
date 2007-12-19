# This file is part of !RiscPkg.
# Copyright © 2003-2004 Graham Shaw.            
# Distribution and use are subject to the GNU General Public License.
#
# This is a cut down version used by the pkgindex and pkgweb
# applications that can be built on most gcc supporting hosts
# This version create by Alan Buckley

TARGET=libpkg.a
CPPFLAGS= 
CXXFLAGS= -I. -Wall -W -Wno-unused -O2

OBJ = md5.o \
 uri.o \
 version.o \
 control.o \
 binary_control.o \
 zipfile.o


$(TARGET): $(OBJ)
	$(AR) rcs $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ)
	rm -f $(TARGET)
