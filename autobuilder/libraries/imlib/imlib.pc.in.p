--- imlib.pc.in.orig	2008-01-17 19:48:51.000000000 -0800
+++ imlib.pc.in	2008-01-17 19:49:05.000000000 -0800
@@ -9,5 +9,5 @@
 Description: An image loading and rendering library for X11R6
 Version: @VERSION@
 Requires:
-Libs: -L${libdir} @LDFLAGS@ -lImlib @SUPPORT_LIBS@ @X_LIBS@ @X_PRE_LIBS@ -lXext -lX11 @X_EXTRA_LIBS@
+Libs: -L${libdir} @LDFLAGS@ -lImlib @SUPPORT_LIBS@ @X_LIBS@ @X_PRE_LIBS@ -lXext -lCX11 -lDesk @X_EXTRA_LIBS@
 Cflags: -I${includedir} @X_CFLAGS@
