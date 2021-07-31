--- configure.in.orig	2019-06-08 21:25:41.538725709 +0100
+++ configure.in	2019-06-08 21:32:18.026801909 +0100
@@ -63,6 +63,15 @@
         BASE_CFLAGS="-I/usr/include/mingw -mno-cygwin"
         BASE_LDFLAGS="-mno-cygwin"
         ;;
+    *-*-riscos*)
+        if test "x$ENABLE_VFP" = "xyes"; then
+            BASE_CFLAGS="-D_GNU_SOURCE=1 -mfpu=vfp"
+            BASE_LDFLAGS="-mfpu=vfp"
+        else
+            BASE_CFLAGS="-D_GNU_SOURCE=1"
+            BASE_LDFLAGS=""
+        fi
+        ;;
     *)
         BASE_CFLAGS="-D_GNU_SOURCE=1"
         BASE_LDFLAGS=""
