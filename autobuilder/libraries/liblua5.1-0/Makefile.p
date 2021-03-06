--- Makefile.orig	2021-03-06 14:08:13.150680700 +0000
+++ Makefile	2021-03-06 14:08:38.714455890 +0000
@@ -9,7 +9,7 @@
 
 # Where to install. The installation starts in the src and doc directories,
 # so take care if INSTALL_TOP is not an absolute path.
-INSTALL_TOP= /usr/local
+INSTALL_TOP= $(GCCSDK_INSTALL_ENV)
 INSTALL_BIN= $(INSTALL_TOP)/bin
 INSTALL_INC= $(INSTALL_TOP)/include
 INSTALL_LIB= $(INSTALL_TOP)/lib/$(DEB_HOST_MULTIARCH)
