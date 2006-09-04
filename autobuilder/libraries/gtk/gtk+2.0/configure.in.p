--- configure.in.orig	2006-06-19 04:19:27.000000000 +0200
+++ configure.in	2006-08-04 00:08:36.000000000 +0200
@@ -1045,10 +1046,11 @@
   #
   # Check for basic X packages; we use pkg-config if available
   #
-  if $PKG_CONFIG --exists x11 xext xrender; then
+  if $PKG_CONFIG --exists x11 xext xrender xau xdmcp; then
     have_base_x_pc=true
-    X_PACKAGES="$X_PACKAGES x11 xext xrender"
-    x_libs="`pkg-config --libs x11 xext xrender`"
+    X_CFLAGS="`$PKG_CONFIG --cflags x11 xext xrender xau xdmcp`"
+    X_PACKAGES="$X_PACKAGES x11 xext xrender xau xdmcp"
+    x_libs="`$PKG_CONFIG --libs x11 xext xrender xau xdmcp`"

     # Strip out any .la files that pkg-config might give us (this happens
     # with -uninstalled.pc files)
