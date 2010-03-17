--- !LibPkg/libpkg/Makefile.orig	2010-03-15 22:49:48.000000000 +0000
+++ !LibPkg/libpkg/Makefile	2010-03-15 22:52:28.000000000 +0000
@@ -3,11 +3,18 @@
 # Distribution and use are subject to the GNU General Public License,
 # a copy of which may be found in the file !RiscPkg.Copyright.
 
-CPPFLAGS = -Irtk: -Izlib:zlib -Ilibcurl: -Ilibpkg_build:
+#CPPFLAGS = -Irtk: -Izlib:zlib -Ilibcurl: -Ilibpkg_build:
+CPPFLAGS = -I$(GCCSDK_INSTALL_ENV)/zlib -I$(GCCSDK_INSTALL_ENV) -I..
 CXXFLAGS = -mthrowback -munixlib -mpoke-function-name \
  -Wall -W -Wno-unused -Wno-uninitialized -O2
 
-include fixdeps:Rules/make
+#include fixdeps:Rules/make
+
+%.dd: %.cc
+	$(CC) -M $(CPPFLAGS) $< > $@
+
+%.d: %.dd
+	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $< > $@
 
 OBJ = md5.o \
  auto_dir.o \
