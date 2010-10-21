--- !LibPkg/Makefile.orig	2010-03-15 20:48:41.000000000 +0000
+++ !LibPkg/Makefile	2010-03-16 21:25:10.000000000 +0000
@@ -3,7 +3,7 @@
 # Distribution and use are subject to the GNU Lesser General Public License,
 # a copy of which may be found in the file !LibPkg.Copyright.
 
-AR = libfile
+AR = ar
 
 DOXYFILES = \
  Doxyfile \
@@ -20,12 +20,19 @@
 all: bin doc
 
 libpkg.a: libpkg/timestamp
-	mkdir -p a
-	$(AR) -c a.libpkg libpkg.o.*
-	$(AR) -o a.libpkg
+#	mkdir -p a
+	$(AR) cr libpkg.a libpkg/*.o
+#	$(AR) -o a.libpkg
 
 libpkg/timestamp: always
 	make -C libpkg
 
 html/index.html: libpkg/timestamp $(DOXYFILES)
 	doxygen
+
+install:
+
+	cp libpkg.a $(GCCSDK_INSTALL_ENV)/lib/
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/libpkg
+	cp libpkg/*.h $(GCCSDK_INSTALL_ENV)/include/libpkg
+
