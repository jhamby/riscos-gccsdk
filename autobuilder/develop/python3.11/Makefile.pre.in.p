--- Makefile.pre.in.orig	2023-07-01 13:17:29.227182586 +0100
+++ Makefile.pre.in	2023-07-01 13:17:08.996106131 +0100
@@ -71,7 +71,7 @@
 INSTALL_DATA=	@INSTALL_DATA@
 # Shared libraries must be installed with executable mode on some systems;
 # rather than figuring out exactly which, we always give them executable mode.
-INSTALL_SHARED= ${INSTALL} -m 755
+INSTALL_SHARED= ${INSTALL}
 
 MKDIR_P=	@MKDIR_P@
 
@@ -232,8 +232,8 @@
 
 # Modes for directories, executables and data files created by the
 # install process.  Default to user-only-writable for all file types.
-DIRMODE=	755
-EXEMODE=	755
+DIRMODE=
+EXEMODE=
 FILEMODE=	644
 
 # configure script arguments
