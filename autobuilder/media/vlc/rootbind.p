--- src/network/rootbind.c.orig	2009-08-13 08:10:32.000000000 -0700
+++ src/network/rootbind.c	2009-08-13 08:10:44.000000000 -0700
@@ -22,7 +22,7 @@
 # include <config.h>
 #endif
 
-#if !defined (WIN32) && !defined (SYS_BEOS)
+#if !defined (WIN32) && !defined (SYS_BEOS) && !defined (__riscos__)
 # define ENABLE_ROOTWRAP 1
 #endif
 
