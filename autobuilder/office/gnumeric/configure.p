--- configure.orig	2008-01-17 23:03:08.000000000 -0800
+++ configure	2008-01-17 23:02:27.000000000 -0800
@@ -21806,9 +21806,9 @@
   if test "x$libgoffice" = x; then
     { echo "$as_me:$LINENO: checking libgoffice-$ver" >&5
 echo $ECHO_N "checking libgoffice-$ver... $ECHO_C" >&6; }
-    if pkg-config --exists libgoffice-$ver; then
+    if $PKG_CONFIG --exists libgoffice-$ver; then
       libgoffice=libgoffice-$ver
-      goffice_ver=`pkg-config --modversion libgoffice-$ver`
+      goffice_ver=`$PKG_CONFIG --modversion libgoffice-$ver`
       { echo "$as_me:$LINENO: result: found" >&5
 echo "${ECHO_T}found" >&6; }
     else
