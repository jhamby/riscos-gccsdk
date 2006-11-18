--- configure.in.orig	2006-11-12 02:15:00.000000000 +0000
+++ configure.in	2006-11-12 02:17:00.000000000 +0000
@@ -1049,7 +1049,7 @@
   if $PKG_CONFIG --exists x11 xext xrender; then
     have_base_x_pc=true
     X_PACKAGES="$X_PACKAGES x11 xext xrender"
-    x_libs="`pkg-config --libs x11 xext xrender`"
+    x_libs="`$PKG_CONFIG --libs x11 xext xrender`"
     
     # Strip out any .la files that pkg-config might give us (this happens
     # with -uninstalled.pc files)
