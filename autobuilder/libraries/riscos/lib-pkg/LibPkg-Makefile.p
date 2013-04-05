--- !LibPkg/Makefile.orig	2013-04-05 13:51:32.266274002 +0100
+++ !LibPkg/Makefile	2013-04-05 13:53:06.009941200 +0100
@@ -34,3 +34,11 @@
 clean:
 	make -C libpkg clean
 	make -C libpkg/os clean
+
+install:
+
+	cp libpkg.a $(GCCSDK_INSTALL_ENV)/lib/
+	mkdir -p $(GCCSDK_INSTALL_ENV)/include/libpkg/os
+	cp libpkg/*.h libpkg/os/*.h $(GCCSDK_INSTALL_ENV)/include/libpkg
+	cp libpkg/os/*.h $(GCCSDK_INSTALL_ENV)/include/libpkg/os
+
