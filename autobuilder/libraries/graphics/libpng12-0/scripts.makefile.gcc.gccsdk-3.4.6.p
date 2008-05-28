--- scripts/makefile.gcc.orig	2008-05-28 04:12:37.000000000 +0200
+++ scripts/makefile.gcc	2008-05-28 04:14:03.000000000 +0200
@@ -4,8 +4,8 @@
 # For conditions of distribution and use, see copyright notice in png.h
 
 # Location of the zlib library and include files
-ZLIBINC = ../zlib
-ZLIBLIB = ../zlib
+ZLIBINC = $(GCCSDK_INSTALL_ENV)/include 
+ZLIBLIB = $(GCCSDK_INSTALL_ENV)/lib
 
 # Compiler, linker, lib and other tools
 CC = gcc
@@ -51,6 +51,14 @@
 	$(AR_RC) $@ $(OBJS)
 	$(RANLIB) $@
 
+install:
+	mkdir -p  $(GCCSDK_INSTALL_ENV)/lib/pkgconfig
+	cp libpng$(A) $(GCCSDK_INSTALL_ENV)/lib
+	cp libpng.pc $(GCCSDK_INSTALL_ENV)/lib/pkgconfig
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/libpng12
+	cp png.h pngconf.h $(GCCSDK_INSTALL_ENV)/include/libpng12
+	cp png.h pngconf.h $(GCCSDK_INSTALL_ENV)/include
+
 test: pngtest$(EXE)
 	./pngtest$(EXE)
 
