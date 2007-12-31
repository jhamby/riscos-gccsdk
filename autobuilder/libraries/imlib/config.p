--- imlib-config.in.orig	2007-12-30 16:54:18.000000000 -0800
+++ imlib-config.in	2007-12-30 16:54:45.000000000 -0800
@@ -59,11 +59,11 @@
       ;;
     --libs)
       libdirs=-L@libdir@
-      echo $libdirs -lImlib 
+      echo $libdirs -lImlib $gx_libs
       ;;
     --libs-gdk)
       libdirs=-L@libdir@
-      echo $libdirs -lgdk_imlib
+      echo $libdirs -lgdk_imlib $gx_libs
       ;;
     *)
       echo "${usage}" 1>&2
