--- !RiscPkg/Rules.make.orig	2010-03-15 23:19:46.000000000 +0000
+++ !RiscPkg/Rules.make	2010-03-16 21:35:55.000000000 +0000
@@ -4,11 +4,11 @@
 # a copy of which may be found in the file !RiscPkg.Copyright.
 
 CXX = gcc
-LD = gcc
+LD = gcc -static
 FIXDEPS = fixdeps
 
-CPPFLAGS = -Irtk: -Ilibcurl: -Ilibpkg:
-CXXFLAGS = -mthrowback -munixlib -mpoke-function-name \
+CPPFLAGS = -I$(GCC_INSTALL_ENV)/include
+CXXFLAGS = -mthrowback -mpoke-function-name \
  -Wall -W -Wno-unused -Wno-uninitialized -O2
 
 .SUFFIXES: .o .cc .d .dd
@@ -20,6 +20,7 @@
 	$(CXX) -MM $(CPPFLAGS) $< > $@
 
 .dd.d:
-	$(FIXDEPS) $* < $< > $@
+#	$(FIXDEPS) $* < $< > $@
+	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $< > $@
 
 .DELETE_ON_ERROR:
