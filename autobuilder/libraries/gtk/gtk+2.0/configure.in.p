--- configure.in.org	2006-06-19 23:58:53.000000000 +0200
+++ configure.in	2006-06-20 01:13:19.000000000 +0200
@@ -1048,6 +1048,7 @@
   #
   if $PKG_CONFIG --exists x11 xext xrender; then
     have_base_x_pc=true
+    X_CFLAGS="`pkg-config --cflags x11 xext xrender`"
     X_PACKAGES="$X_PACKAGES x11 xext xrender"
     x_libs="`pkg-config --libs x11 xext xrender`"

