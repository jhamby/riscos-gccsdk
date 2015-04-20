--- Source/autotools/SetupCompilerFlags.m4.orig	2014-05-15 18:12:37.000000000 +0100
+++ Source/autotools/SetupCompilerFlags.m4	2014-06-14 14:26:48.496304822 +0100
@@ -37,3 +37,10 @@
     CXXFLAGS="$CXXFLAGS -O0"
     CFLAGS="$CFLAGS -O0"
 fi
+
+if test "$CC" = "gcc"; then
+  CFLAGS="$CFLAGS -march=armv4"
+fi
+if test "$CXX" = "g++"; then
+  CXXFLAGS="$CXXFLAGS -march=armv4"
+fi
